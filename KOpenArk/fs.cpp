#include "kark.h"


#include <ntddk.h>

//#include "wintypes.h"
//---------------------------------------------------------------------------


NTSTATUS
IoCompletionRoutine(
	IN PDEVICE_OBJECT  DeviceObject,
	IN PIRP  Irp,
	IN PVOID  Context
)
{
	DbgPrint(("IoCompletionRoutine!\n"));
	*Irp->UserIosb = Irp->IoStatus;

	if (Irp->UserEvent)
		KeSetEvent(Irp->UserEvent, IO_NO_INCREMENT, 0);

	if (Irp->MdlAddress)
	{
		IoFreeMdl(Irp->MdlAddress);
		Irp->MdlAddress = NULL;
	}

	IoFreeIrp(Irp);

	return STATUS_MORE_PROCESSING_REQUIRED;
}

NTSTATUS
IrpFileCreate(
	IN PUNICODE_STRING FileName,
	IN ACCESS_MASK DesiredAccess,
	IN ULONG FileAttributes,
	IN ULONG ShareAccess,
	IN ULONG CreateDisposition,
	IN ULONG CreateOptions,
	IN PDEVICE_OBJECT DeviceObject,
	IN PDEVICE_OBJECT RealDevice,
	OUT PVOID *Object
)
{
	NTSTATUS status;
	KEVENT event;
	PIRP irp;
	IO_STATUS_BLOCK ioStatus;
	PIO_STACK_LOCATION irpSp;
	IO_SECURITY_CONTEXT securityContext;
	ACCESS_STATE accessState;
	OBJECT_ATTRIBUTES objectAttributes;
	PFILE_OBJECT fileObject;
	AUX_ACCESS_DATA auxData;

	RtlZeroMemory(&auxData, sizeof(AUX_ACCESS_DATA));
	KeInitializeEvent(&event, SynchronizationEvent, FALSE);
	irp = IoAllocateIrp(DeviceObject->StackSize, FALSE);

	if (irp == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	InitializeObjectAttributes(&objectAttributes, NULL, OBJ_CASE_INSENSITIVE, 0, NULL);

	status = ObCreateObject(KernelMode,
		*IoFileObjectType,
		&objectAttributes,
		KernelMode,
		NULL,
		sizeof(FILE_OBJECT),
		0,
		0,
		(PVOID *)&fileObject);

	if (!NT_SUCCESS(status))
	{
		IoFreeIrp(irp);
		return status;
	}

	RtlZeroMemory(fileObject, sizeof(FILE_OBJECT));
	fileObject->Type = IO_TYPE_FILE;
	fileObject->Size = sizeof(FILE_OBJECT);
	fileObject->DeviceObject = RealDevice;
	//	fileObject->RelatedFileObject = NULL;
	fileObject->Flags = FO_SYNCHRONOUS_IO;
	fileObject->FileName.MaximumLength = FileName->MaximumLength;
	fileObject->FileName.Buffer =(PWCH)ExAllocatePool(NonPagedPool, FileName->MaximumLength);

	if (fileObject->FileName.Buffer == NULL)
	{
		IoFreeIrp(irp);
		ObDereferenceObject(fileObject);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	RtlCopyUnicodeString(&fileObject->FileName, FileName);
	KeInitializeEvent(&fileObject->Lock, SynchronizationEvent, FALSE);
	KeInitializeEvent(&fileObject->Event, NotificationEvent, FALSE);

	irp->MdlAddress = NULL;
	irp->Flags |= IRP_CREATE_OPERATION | IRP_SYNCHRONOUS_API;
	irp->RequestorMode = KernelMode;
	irp->UserIosb = &ioStatus;
	irp->UserEvent = &event;
	irp->PendingReturned = FALSE;
	irp->Cancel = FALSE;
	irp->CancelRoutine = NULL;
	irp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	irp->Tail.Overlay.AuxiliaryBuffer = NULL;
	irp->Tail.Overlay.OriginalFileObject = fileObject;

	status = SeCreateAccessState(&accessState,
		&auxData,
		DesiredAccess,
		IoGetFileObjectGenericMapping());

	if (!NT_SUCCESS(status))
	{
		IoFreeIrp(irp);
		ExFreePool(fileObject->FileName.Buffer);
		ObDereferenceObject(fileObject);
		return status;
	}

	securityContext.SecurityQos = NULL;
	securityContext.AccessState = &accessState;
	securityContext.DesiredAccess = DesiredAccess;
	securityContext.FullCreateOptions = 0;

	irpSp = IoGetNextIrpStackLocation(irp);
	irpSp->MajorFunction = IRP_MJ_CREATE;
	irpSp->DeviceObject = DeviceObject;
	irpSp->FileObject = fileObject;
	irpSp->Parameters.Create.SecurityContext = &securityContext;
	irpSp->Parameters.Create.Options = (CreateDisposition << 24) | CreateOptions;
	irpSp->Parameters.Create.FileAttributes = (USHORT)FileAttributes;
	irpSp->Parameters.Create.ShareAccess = (USHORT)ShareAccess;
	irpSp->Parameters.Create.EaLength = 0;

	IoSetCompletionRoutine(irp, IoCompletionRoutine, NULL, TRUE, TRUE, TRUE);
	status = IoCallDriver(DeviceObject, irp);

	if (status == STATUS_PENDING)
		KeWaitForSingleObject(&event, Executive, KernelMode, TRUE, NULL);

	status = ioStatus.Status;

	if (!NT_SUCCESS(status))
	{
		ExFreePool(fileObject->FileName.Buffer);
		fileObject->FileName.Length = 0;
		fileObject->DeviceObject = NULL;
		ObDereferenceObject(fileObject);
	}
	else
	{
		InterlockedIncrement(&fileObject->DeviceObject->ReferenceCount);

		if (fileObject->Vpb)
		{
			InterlockedIncrement((volatile long*)&fileObject->Vpb->ReferenceCount);
		}
		*Object = fileObject;
		dprintf("Open file success! object = %x\n", fileObject);
	}

	return status;
}

NTSTATUS
IrpFileClose(
	IN PDEVICE_OBJECT DeviceObject,
	IN PFILE_OBJECT FileObject
)
{
	NTSTATUS status;
	KEVENT event;
	PIRP irp;
	PVPB vpb;
	IO_STATUS_BLOCK ioStatusBlock;
	PIO_STACK_LOCATION irpSp;

	KeInitializeEvent(&event, SynchronizationEvent, FALSE);
	irp = IoAllocateIrp(DeviceObject->StackSize, FALSE);

	if (irp == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	irp->Tail.Overlay.OriginalFileObject = FileObject;
	irp->Tail.Overlay.Thread = PsGetCurrentThread();
	irp->RequestorMode = KernelMode;
	irp->UserEvent = &event;
	irp->UserIosb = &irp->IoStatus;
	irp->Overlay.AsynchronousParameters.UserApcRoutine = (PIO_APC_ROUTINE)NULL;
	irp->Flags = IRP_SYNCHRONOUS_API | IRP_CLOSE_OPERATION;

	irpSp = IoGetNextIrpStackLocation(irp);
	irpSp->MajorFunction = IRP_MJ_CLEANUP;
	irpSp->FileObject = FileObject;

	status = IoCallDriver(DeviceObject, irp);

	if (status == STATUS_PENDING)
	{
		KeWaitForSingleObject(&event,
			UserRequest,
			KernelMode,
			FALSE,
			NULL);
	}

	IoReuseIrp(irp, STATUS_SUCCESS);
	KeClearEvent(&event);

	irpSp = IoGetNextIrpStackLocation(irp);
	irpSp->MajorFunction = IRP_MJ_CLOSE;
	irpSp->FileObject = FileObject;

	irp->UserIosb = &ioStatusBlock;
	irp->UserEvent = &event;
	irp->Tail.Overlay.OriginalFileObject = FileObject;
	irp->Tail.Overlay.Thread = PsGetCurrentThread();
	irp->AssociatedIrp.SystemBuffer = (PVOID)NULL;
	irp->Flags = IRP_CLOSE_OPERATION | IRP_SYNCHRONOUS_API;

	vpb = FileObject->Vpb;

	if (vpb && !(FileObject->Flags & FO_DIRECT_DEVICE_OPEN))
	{
		InterlockedDecrement((volatile long*)&vpb->ReferenceCount);
		FileObject->Flags |= FO_FILE_OPEN_CANCELLED;
	}

	status = IoCallDriver(DeviceObject, irp);

	if (status == STATUS_PENDING)
	{
		KeWaitForSingleObject(&event,
			Executive,
			KernelMode,
			FALSE,
			NULL);
	}

	IoFreeIrp(irp);

	return status;
}

NTSTATUS
IrpFileRead(
	IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER ByteOffset OPTIONAL,
	IN ULONG Length,
	OUT PVOID Buffer,
	OUT PIO_STATUS_BLOCK IoStatusBlock
)
{
	NTSTATUS status;
	KEVENT event;
	PIRP irp;
	PIO_STACK_LOCATION irpSp;
	PDEVICE_OBJECT deviceObject;

	if (ByteOffset == NULL)
	{
		if (!(FileObject->Flags & FO_SYNCHRONOUS_IO))
			return STATUS_INVALID_PARAMETER;

		ByteOffset = &FileObject->CurrentByteOffset;
	}

	if (FileObject->Vpb == 0 || FileObject->Vpb->RealDevice == NULL)
		return STATUS_UNSUCCESSFUL;

	deviceObject = FileObject->Vpb->DeviceObject;
	irp = IoAllocateIrp(deviceObject->StackSize, FALSE);

	if (irp == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	irp->MdlAddress = IoAllocateMdl(Buffer, Length, FALSE, TRUE, NULL);

	if (irp->MdlAddress == NULL)
	{
		IoFreeIrp(irp);
		return STATUS_INSUFFICIENT_RESOURCES;;
	}

	MmBuildMdlForNonPagedPool(irp->MdlAddress);

	irp->Flags = IRP_READ_OPERATION;
	irp->RequestorMode = KernelMode;
	irp->UserIosb = IoStatusBlock;
	irp->UserEvent = &event;
	irp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	irp->Tail.Overlay.OriginalFileObject = FileObject;

	irpSp = IoGetNextIrpStackLocation(irp);
	irpSp->MajorFunction = IRP_MJ_READ;
	irpSp->MinorFunction = IRP_MN_NORMAL;
	irpSp->DeviceObject = deviceObject;
	irpSp->FileObject = FileObject;
	irpSp->Parameters.Read.Length = Length;
	irpSp->Parameters.Read.ByteOffset = *ByteOffset;

	KeInitializeEvent(&event, SynchronizationEvent, FALSE);
	IoSetCompletionRoutine(irp, IoCompletionRoutine, NULL, TRUE, TRUE, TRUE);
	status = IoCallDriver(deviceObject, irp);

	if (status == STATUS_PENDING)
		status = KeWaitForSingleObject(&event, Executive, KernelMode, TRUE, NULL);

	return status;
}

NTSTATUS
IrpFileWrite(
	IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER ByteOffset OPTIONAL,
	IN ULONG Length,
	IN PVOID Buffer,
	OUT PIO_STATUS_BLOCK IoStatusBlock
)
{
	NTSTATUS status;
	KEVENT event;
	PIRP irp;
	PIO_STACK_LOCATION irpSp;
	PDEVICE_OBJECT deviceObject;

	if (ByteOffset == NULL)
	{
		if (!(FileObject->Flags & FO_SYNCHRONOUS_IO))
			return STATUS_INVALID_PARAMETER;

		ByteOffset = &FileObject->CurrentByteOffset;
	}

	if (FileObject->Vpb == 0 || FileObject->Vpb->RealDevice == NULL)
		return STATUS_UNSUCCESSFUL;

	deviceObject = FileObject->Vpb->DeviceObject;
	irp = IoAllocateIrp(deviceObject->StackSize, FALSE);

	if (irp == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	irp->MdlAddress = IoAllocateMdl(Buffer, Length, FALSE, TRUE, NULL);

	if (irp->MdlAddress == NULL)
	{
		IoFreeIrp(irp);
		return STATUS_INSUFFICIENT_RESOURCES;;
	}

	MmBuildMdlForNonPagedPool(irp->MdlAddress);

	irp->Flags = IRP_WRITE_OPERATION;
	irp->RequestorMode = KernelMode;
	irp->UserIosb = IoStatusBlock;
	irp->UserEvent = &event;
	irp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	irp->Tail.Overlay.OriginalFileObject = FileObject;

	irpSp = IoGetNextIrpStackLocation(irp);
	irpSp->MajorFunction = IRP_MJ_WRITE;
	irpSp->MinorFunction = IRP_MN_NORMAL;
	irpSp->DeviceObject = deviceObject;
	irpSp->FileObject = FileObject;
	irpSp->Parameters.Write.Length = Length;
	irpSp->Parameters.Write.ByteOffset = *ByteOffset;

	KeInitializeEvent(&event, SynchronizationEvent, FALSE);
	IoSetCompletionRoutine(irp, IoCompletionRoutine, NULL, TRUE, TRUE, TRUE);
	status = IoCallDriver(deviceObject, irp);

	if (status == STATUS_PENDING)
		status = KeWaitForSingleObject(&event, Executive, KernelMode, TRUE, NULL);

	return status;
}

BOOLEAN ArkQueryFileInfomation(HANDLE FileHandle, PVOID InfoMation, ULONG Len, FILE_INFORMATION_CLASS InfomationClass)
{
	NTSTATUS st;
	FILE_OBJECT fileObj;

	st = ObReferenceObjectByHandle(FileHandle, 0, *IoFileObjectType, KernelMode, (PVOID*)&fileObj, 0);
	if (NT_SUCCESS(st))
	{
		/*DEVICE_OBJECT
		fileObj.Size
			SynchronizationEvent*/


	}




	return BOOLEAN();
}

NTSTATUS
IrpFileQuery(
	IN PFILE_OBJECT FileObject,
	OUT PVOID FileInformation,
	IN ULONG Length,
	IN FILE_INFORMATION_CLASS FileInformationClass
)
{
	NTSTATUS status;
	KEVENT event;
	PIRP irp;
	IO_STATUS_BLOCK ioStatus;
	PIO_STACK_LOCATION irpSp;
	PDEVICE_OBJECT deviceObject;

	if (FileObject->Vpb == 0 || FileObject->Vpb->RealDevice == NULL)
		return STATUS_UNSUCCESSFUL;

	deviceObject = FileObject->Vpb->DeviceObject;
	KeInitializeEvent(&event, SynchronizationEvent, FALSE);
	irp = IoAllocateIrp(deviceObject->StackSize, FALSE);

	if (irp == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	irp->Flags = IRP_BUFFERED_IO;
	irp->AssociatedIrp.SystemBuffer = FileInformation;
	irp->RequestorMode = KernelMode;
	irp->Overlay.AsynchronousParameters.UserApcRoutine = (PIO_APC_ROUTINE)NULL;
	irp->UserEvent = &event;
	irp->UserIosb = &ioStatus;
	irp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	irp->Tail.Overlay.OriginalFileObject = FileObject;

	irpSp = IoGetNextIrpStackLocation(irp);
	irpSp->MajorFunction = IRP_MJ_QUERY_INFORMATION;
	irpSp->DeviceObject = deviceObject;
	irpSp->FileObject = FileObject;
	irpSp->Parameters.QueryFile.Length = Length;
	irpSp->Parameters.QueryFile.FileInformationClass = FileInformationClass;

	IoSetCompletionRoutine(irp, IoCompletionRoutine, NULL, TRUE, TRUE, TRUE);
	status = IoCallDriver(deviceObject, irp);

	if (status == STATUS_PENDING)
		KeWaitForSingleObject(&event, Executive, KernelMode, TRUE, NULL);

	return ioStatus.Status;
}

NTSTATUS
IrpDirectoryQuery(
	IN PFILE_OBJECT FileObject,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	OUT PVOID Buffer,
	IN ULONG Length
)
{
	NTSTATUS status;
	KEVENT event;
	PIRP irp;
	IO_STATUS_BLOCK ioStatus;
	PIO_STACK_LOCATION irpSp;
	PDEVICE_OBJECT deviceObject;
	PQUERY_DIRECTORY queryDirectory;

	if (FileObject->Vpb == 0 || FileObject->Vpb->RealDevice == NULL)
		return STATUS_UNSUCCESSFUL;

	deviceObject = FileObject->Vpb->DeviceObject;
	KeInitializeEvent(&event, SynchronizationEvent, FALSE);
	irp = IoAllocateIrp(deviceObject->StackSize, FALSE);

	if (irp == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	irp->Flags = IRP_INPUT_OPERATION | IRP_BUFFERED_IO;
	irp->RequestorMode = KernelMode;
	irp->UserEvent = &event;
	irp->UserIosb = &ioStatus;
	irp->UserBuffer = Buffer;
	irp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	irp->Tail.Overlay.OriginalFileObject = FileObject;
	irp->Overlay.AsynchronousParameters.UserApcRoutine = (PIO_APC_ROUTINE)NULL;
	//irp->Pointer = FileObject;

	irpSp = IoGetNextIrpStackLocation(irp);
	irpSp->MajorFunction = IRP_MJ_DIRECTORY_CONTROL;
	irpSp->MinorFunction = IRP_MN_QUERY_DIRECTORY;
	irpSp->DeviceObject = deviceObject;
	irpSp->FileObject = FileObject;

	queryDirectory = (PQUERY_DIRECTORY)&irpSp->Parameters;
	queryDirectory->Length = Length;
	queryDirectory->FileName = NULL;
	queryDirectory->FileInformationClass = FileInformationClass;
	queryDirectory->FileIndex = 0;

	IoSetCompletionRoutine(irp, IoCompletionRoutine, NULL, TRUE, TRUE, TRUE);
	status = IoCallDriver(deviceObject, irp);

	if (status == STATUS_PENDING)
	{
		KeWaitForSingleObject(&event, Executive, KernelMode, TRUE, NULL);
		status = ioStatus.Status;
	}

	return status;
}

BOOLEAN GetDriveObject(
	IN ULONG DriveNumber,
	OUT PDEVICE_OBJECT *DeviceObject,
	OUT PDEVICE_OBJECT *ReadDevice
)
{
	WCHAR driveName[] = L"\\DosDevices\\A:\\";
	UNICODE_STRING deviceName;
	HANDLE deviceHandle;
	OBJECT_ATTRIBUTES objectAttributes;
	IO_STATUS_BLOCK	ioStatus;
	PFILE_OBJECT fileObject;
	NTSTATUS status;

	if (DriveNumber >= 'A' && DriveNumber <= 'Z')
	{
		driveName[12] = (CHAR)DriveNumber;
	}
	else if (DriveNumber >= 'a' && DriveNumber <= 'z')
	{
		driveName[12] = (CHAR)DriveNumber - 'a' + 'A';
	}
	else
	{
		return FALSE;
	}

	RtlInitUnicodeString(&deviceName, driveName);

	InitializeObjectAttributes(&objectAttributes,
		&deviceName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);

	status = IoCreateFile(&deviceHandle,
		SYNCHRONIZE | FILE_ANY_ACCESS,
		&objectAttributes,
		&ioStatus,
		NULL,
		0,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT | FILE_DIRECTORY_FILE,
		NULL,
		0,
		CreateFileTypeNone,
		NULL,
		IO_NO_PARAMETER_CHECKING);

	if (!NT_SUCCESS(status))
	{
		dprintf("Could not open drive %c: %x\n", DriveNumber, status);
		return FALSE;
	}

	status = ObReferenceObjectByHandle(deviceHandle,
		FILE_READ_DATA,
		*IoFileObjectType,
		KernelMode,
		(PVOID*)&fileObject,
		NULL);

	if (!NT_SUCCESS(status))
	{
		dprintf("Could not get fileobject from handle: %c\n", DriveNumber);
		ZwClose(deviceHandle);
		return FALSE;
	}

	if (fileObject->Vpb == 0 || fileObject->Vpb->RealDevice == NULL)
	{
		ObDereferenceObject(fileObject);
		ZwClose(deviceHandle);
		return FALSE;
	}

	*DeviceObject = fileObject->Vpb->DeviceObject;
	*ReadDevice = fileObject->Vpb->RealDevice;

	ObDereferenceObject(fileObject);
	ZwClose(deviceHandle);

	return TRUE;
}

NTSTATUS
fnCreateFile(
	IN PFILE_REQUEST_CREATE FileRequestCreate,
	IN ULONG InputBufferLength,
	OUT PIO_STATUS_BLOCK IoStatusBlock
)
{
	PDEVICE_OBJECT deviceObject;
	PDEVICE_OBJECT realDevice;
	PFILE_OBJECT fileObject;
	NTSTATUS status;
	HANDLE newHandle;
	ANSI_STRING fname;
	UNICODE_STRING fileName;

	if (InputBufferLength <= sizeof(FILE_REQUEST_CREATE))
		return STATUS_INVALID_PARAMETER;

	if ((FileRequestCreate->ShareAccess & ~FILE_SHARE_VALID_FLAGS) ||
		(FileRequestCreate->CreateDisposition > FILE_MAXIMUM_DISPOSITION))
		return STATUS_INVALID_PARAMETER;

	fname.Length = (USHORT)InputBufferLength - sizeof(FILE_REQUEST_CREATE);
	fname.Buffer = FileRequestCreate->FileName;
	fname.MaximumLength = fname.Length;

	if (fname.Buffer[fname.Length - 1] == '\0')
		fname.Length--;

	if (fname.Length < 3)
		return STATUS_INVALID_PARAMETER;

	dprintf("Open %s %d\n", fname.Buffer, fname.Length);
	FileRequestCreate->FileHandle = NULL;

	if (fname.Buffer[1] != ':')
		return STATUS_INVALID_PARAMETER;

	if (!GetDriveObject(fname.Buffer[0], &deviceObject, &realDevice))
	{
		return STATUS_UNSUCCESSFUL;
	}

	fname.Buffer += 2;
	fname.MaximumLength -= 2;
	fname.Length -= 2;

	if (!NT_SUCCESS(RtlAnsiStringToUnicodeString(&fileName,
		&fname,
		TRUE)))
	{
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	__try
	{
		dprintf("deviceObject = %x\n", deviceObject);

		status = IrpFileCreate(&fileName,
			FileRequestCreate->DesiredAccess,
			FILE_ATTRIBUTE_NORMAL,
			FileRequestCreate->ShareAccess,
			FileRequestCreate->CreateDisposition,
			0,
			deviceObject,
			realDevice,
			(PVOID*)&fileObject);

	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		status = GetExceptionCode();
		dprintf("IrpCreate exception! error=%x\n", status);
		RtlFreeUnicodeString(&fileName);
		return status;
	}

	RtlFreeUnicodeString(&fileName);

	if (!NT_SUCCESS(status))
	{
		DbgPrint(("Irp open file failed\n"));
		return status;
	}

	status = ObOpenObjectByPointer(fileObject,
		0,
		NULL,
		FileRequestCreate->DesiredAccess,
		*IoFileObjectType,
		KernelMode,
		&newHandle);

	ObDereferenceObject(fileObject);

	if (!NT_SUCCESS(status))
	{
		DbgPrint(("ObOpenObjectByPointer failed\n"));
		return status;
	}

	FileRequestCreate->FileHandle = newHandle;
	IoStatusBlock->Information = sizeof(HANDLE);

	return status;
}

BOOLEAN ArkOpenFile(PCHAR FileName, ULONG Len, PHANDLE Handle, ULONG OutSize)
{
	FILE_REQUEST_CREATE request;
	IO_STATUS_BLOCK ioBlock;

	ProbeForRead(FileName, Len, 1);
	ProbeForWrite(Handle, 4, 1);
	FileName[Len] = 0;
	RtlZeroMemory(&request, sizeof(request));


	request.FileName = FileName;
	request.CreateDisposition = FILE_OPEN;
	request.DesiredAccess = FILE_READ_DATA | FILE_READ_ATTRIBUTES | SYNCHRONIZE;
	request.ShareAccess = FILE_SHARE_READ;
	
	fnCreateFile(&request, sizeof(request) + Len, &ioBlock);
	if (ioBlock.Status = STATUS_SUCCESS)
	{
		*Handle = request.FileHandle;
		return true;
	}
		
	return false;
}

NTSTATUS
fnReadFile(
	IN PFILE_REQUEST_READ FileRequestRead,
	IN ULONG InputBufferLength,
	OUT PIO_STATUS_BLOCK IoStatusBlock
)
{
	NTSTATUS status;
	PFILE_OBJECT fileObject;
	IO_STATUS_BLOCK ioStatus;

	if (InputBufferLength < sizeof(FILE_REQUEST_READ))
		return STATUS_INVALID_PARAMETER;

	if (InputBufferLength < sizeof(FILE_REQUEST_READ) + FileRequestRead->Length)
		return STATUS_INVALID_PARAMETER;

	DbgPrint(("read file\n"));

	status = ObReferenceObjectByHandle(FileRequestRead->FileHandle,
		GENERIC_READ,
		*IoFileObjectType,
		KernelMode,
		(PVOID*)&fileObject,
		NULL);

	if (!NT_SUCCESS(status))
		return status;

	__try
	{
		status = IrpFileRead(fileObject,
			NULL,
			FileRequestRead->Length,
			FileRequestRead->Buffer,
			&ioStatus);

		status = ioStatus.Status;
		FileRequestRead->ReadLength = sizeof(FILE_REQUEST_READ) + ioStatus.Information;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		status = GetExceptionCode();
		dprintf("IrpRead exception! error=%x\n", status);
	}

	ObDereferenceObject(fileObject);

	return status;
}

NTSTATUS
fnDeleteFile(
	IN HANDLE FileHandle
)
{
	NTSTATUS status;
	PFILE_OBJECT fileObject;
	PDEVICE_OBJECT deviceObject;
	FILE_DISPOSITION_INFORMATION fdi;

	status = ObReferenceObjectByHandle(FileHandle,
		0,
		*IoFileObjectType,
		KernelMode,
		(PVOID*)&fileObject,
		NULL);

	if (!NT_SUCCESS(status))
	{
		return status;
	}

	deviceObject = IoGetRelatedDeviceObject(fileObject);

	//fnCloseFile(deviceObject, fileObject);
	fdi.DeleteFile = TRUE;

	status = IoSetInformation(fileObject,
		FileDispositionInformation,
		sizeof(FILE_DISPOSITION_INFORMATION),
		&fdi);

	ObDereferenceObject(fileObject);

	return status;
}
//
//NTSTATUS
//QueryHandleInfo(
//	IN PQUERY_HANDLE_INFO QueryHandleInfo,
//	OUT PVOID OutputBuffer,
//	IN ULONG OutputBufferLength
//)
//{
//	NTSTATUS status;
//	HANDLE processHandle, objectHandle;
//	PEPROCESS process;
//	PVOID object;
//	OBJECT_ATTRIBUTES objectAttributes;
//	CLIENT_ID clientID;
//	POBJECT_NAME_INFORMATION objectName;
//	ANSI_STRING objectNameA;
//	ULONG returnLength;
//
//	__try
//	{
//		if (QueryHandleInfo->Pid < 8)
//		{
//			PsLookupProcessByProcessId((HANDLE)QueryHandleInfo->Pid,
//				&process);
//
//			KeAttachProcess(process);
//
//			status = ObReferenceObjectByHandle((HANDLE)QueryHandleInfo->Handle,
//				GENERIC_READ,
//				NULL,
//				KernelMode,
//				&object,
//				NULL);
//
//			KeDetachProcess();
//
//			if (!NT_SUCCESS(status))
//			{
//				return status;
//			}
//		}
//		else
//		{
//			InitializeObjectAttributes(&objectAttributes,
//				NULL,
//				0,
//				NULL,
//				NULL);
//
//			clientID.UniqueProcess = (HANDLE)QueryHandleInfo->Pid;
//			clientID.UniqueThread = 0;
//
//			status = ZwOpenProcess(&processHandle,
//				PROCESS_DUP_HANDLE,
//				&objectAttributes,
//				&clientID);
//
//			if (!NT_SUCCESS(status))
//			{
//				return status;
//			}
//
//			status = ZwDuplicateObject(processHandle,
//				(HANDLE)QueryHandleInfo->Handle,
//				NtCurrentProcess(),
//				&objectHandle,
//				0,
//				FALSE,
//				DUPLICATE_SAME_ACCESS);
//
//			ZwClose(processHandle);
//
//			if (!NT_SUCCESS(status))
//			{
//				return status;
//			}
//
//			status = ObReferenceObjectByHandle(objectHandle,
//				GENERIC_READ,
//				NULL,
//				KernelMode,
//				&object,
//				NULL);
//
//			ZwClose(objectHandle);
//
//			if (!NT_SUCCESS(status))
//			{
//				return status;
//			}
//		}
//
//		if (object != (PVOID)QueryHandleInfo->Object)
//		{
//			ObDereferenceObject(object);
//			return STATUS_OBJECT_TYPE_MISMATCH;
//		}
//
//		if (*(PULONG)object == 0x700005)
//		{
//			DbgPrint(("0x700005, link?\n"));
//			return STATUS_SUCCESS;
//		}
//
//		objectName = ExAllocatePool(NonPagedPool, 0x400);
//
//		if (objectName == NULL)
//		{
//			ObDereferenceObject(object);
//			return STATUS_INSUFFICIENT_RESOURCES;
//		}
//
//		status = ObQueryNameString(object,
//			objectName,
//			0x400,
//			&returnLength);
//
//		ObDereferenceObject(object);
//
//		if (!NT_SUCCESS(status))
//		{
//			ExFreePool(objectName);
//			return status;
//		}
//
//		status = RtlUnicodeStringToAnsiString(&objectNameA,
//			&objectName->Name,
//			TRUE);
//
//		ExFreePool(objectName);
//
//		if (!NT_SUCCESS(status))
//		{
//			return status;
//		}
//
//		if (objectNameA.Length >= OutputBufferLength)
//		{
//			RtlFreeAnsiString(&objectNameA);
//			return STATUS_BUFFER_TOO_SMALL;
//		}
//
//		strcpy(OutputBuffer, objectNameA.Buffer);
//		RtlFreeAnsiString(&objectNameA);
//		return STATUS_SUCCESS;
//	}
//	__except (EXCEPTION_EXECUTE_HANDLER)
//	{
//		return STATUS_ACCESS_VIOLATION;
//	}
//}
//
//VOID
//PsExitSpecialApc(
//	IN PKAPC Apc,
//	IN PKNORMAL_ROUTINE *NormalRoutine,
//	IN PVOID *NormalContext,
//	IN PVOID *SystemArgument1,
//	IN PVOID *SystemArgument2
//)
//{
//	NTSTATUS status;
//	NTSTATUS ExitStatus;
//	PKTHREAD KThread;
//	CHAR ch;
//
//	ExitStatus = (NTSTATUS)((LONG_PTR)Apc->NormalContext);
//	ExFreePool(Apc);
//	status = PsTerminateSystemThread(ExitStatus);
//
//	if (status == STATUS_INVALID_PARAMETER)
//	{
//		KThread = KeGetCurrentThread();
//
//		if (*NtBuildNumber == 2195)
//		{
//			ch = *((PCHAR)KThread + 0x1fc);
//			*((PCHAR)KThread + 0x1fc) = '\0';
//			//NtTerminateThread((HANDLE)-1, ExitStatus);
//			*((PCHAR)KThread + 0x1fc) = ch;
//		}
//	}
//}
//
//NTSTATUS
//KillThread(
//	IN PVOID ThreadId,
//	IN NTSTATUS ExitStatus
//)
//{
//	NTSTATUS status;
//	PETHREAD Thread;
//	PKAPC ExitApc;
//
//	status = PsLookupThreadByThreadId(ThreadId, &Thread);
//
//	if (!NT_SUCCESS(status))
//		return STATUS_UNSUCCESSFUL;
//
//	status = ObReferenceObjectByPointer(Thread,
//		0x1f03ff,
//		*PsThreadType,
//		KernelMode);
//
//	if (!NT_SUCCESS(status))
//		return STATUS_UNSUCCESSFUL;
//
//	if (Thread == PsGetCurrentThread())
//	{
//		ObDereferenceObject(Thread);
//		PsTerminateSystemThread(ExitStatus);
//	}
//	else
//	{
//		ExitApc = ExAllocatePool(NonPagedPool, sizeof(KAPC));
//
//		if (!ExitApc)
//			return STATUS_INSUFFICIENT_RESOURCES;
//
//		KeInitializeApc(ExitApc,
//			Thread,
//			OriginalApcEnvironment,
//			PsExitSpecialApc,
//			NULL,
//			0,
//			KernelMode,
//			ULongToPtr(ExitStatus));
//
//		if (!KeInsertQueueApc(ExitApc, ExitApc, NULL, 2))
//		{
//			ExFreePool(ExitApc);
//			return STATUS_UNSUCCESSFUL;
//		}
//	}
//
//	return STATUS_SUCCESS;
//}
//
//#define PROCESS_PAGE_DIR_BASE             0xC0300000
//#define PROCESS_PAGE_TABLE_BASE           0xC0000000
//typedef unsigned long* PPTE;
//
///**************************************************************************
//* GetPteAddress - Returns a pointer to the page table entry corresponding
//*           to a given memory address.
//*
//* Parameters:
//*     PVOID VirtualAddress - Address you wish to acquire a pointer to the
//*                     page table entry for.
//*
//* Return - Pointer to the page table entry for VirtualAddress or an error
//*       code.
//*
//* Error Codes:
//*     ERROR_PTE_NOT_PRESENT - The page table for the given virtual
//*                     address is not present in memory.
//*     ERROR_PAGE_NOT_PRESENT - The page containing the data for the
//*                     given virtual address is not present in
//*                     memory.
//**************************************************************************/
//PPTE GetPteAddress(PVOID VirtualAddress)
//{
//	PPTE pPTE = 0;
//	__asm
//	{
//		cli               //disable interrupts           
//		pushad
//		mov esi, PROCESS_PAGE_DIR_BASE
//		mov edx, VirtualAddress
//		mov eax, edx
//		shr eax, 22
//		lea eax, [esi + eax * 4] //pointer to page directory entry 
//		test[eax], 0x80     //is it a large page?         
//		jnz Is_Large_Page     //it's a large page           
//		mov esi, PROCESS_PAGE_TABLE_BASE
//		shr edx, 12
//		lea eax, [esi + edx * 4] //pointer to page table entry (PTE)
//		mov pPTE, eax
//		jmp Done
//
//		//NOTE: There is not a page table for large pages because 
//		//the phys frames are contained in the page directory.   
//		Is_Large_Page :
//		mov pPTE, eax
//
//			Done :
//		popad
//			sti             //reenable interrupts           
//	}//end asm                                       
//
//	return pPTE;
//
//}//end GetPteAddress                                     
//
///**************************************************************************
//* GetPhysicalFrameAddress - Gets the base physical address in memory where
//*                   the page is mapped. This corresponds to the
//*                   bits 12 - 32 in the page table entry.
//*
//* Parameters -
//*     PPTE pPte - Pointer to the PTE that you wish to retrieve the
//*     physical address from.
//*
//* Return - The physical address of the page.
//**************************************************************************/
//ULONG GetPhysicalFrameAddress(PPTE pPte)
//{
//	ULONG Frame = 0;
//
//	__asm
//	{
//		cli
//		pushad
//		mov eax, pPte
//		mov ecx, [eax]
//		shr ecx, 12 //physical page frame consists of the       
//				//upper 20 bits
//				mov Frame, ecx
//				popad
//				sti
//	}//end asm                                       
//	return Frame;
//
//}//end GetPhysicalFrameAddress                               
//
//#define HIGHEST_USER_ADDRESS 0x80000000
//#define ERROR_PAGE_NOT_IN_LIST 0
//
//typedef struct _HIDDEN_CODE_LIST
//{
//	PVOID ExecuteView;
//	PVOID ReadWriteView;
//	ULONG PageNumber;
//	PVOID LoadExecutePage;
//	PEPROCESS PageOwner;
//} HIDDEN_CODE_LIST, *PHIDDEN_CODE_LIST;
//
//ULONG OldInt0EHandler = 0;
//
//VOID
//EnableWriteProtect(
//	BOOLEAN Enable
//)
//{
//	if (!Enable)
//	{
//		__asm
//		{
//			mov		eax, cr0;
//			and		eax, 0xFFFEFFFF; // CR0 16 BIT = 0
//			mov		cr0, eax;
//		}
//	}
//	else
//	{
//		__asm
//		{
//			mov		eax, cr0;
//			or eax, 0x10000
//				mov		cr0, eax;
//		}
//	}
//}
//
//void __declspec(naked) NewInt0EHandler(void)
//{
//	__asm
//	{
//		pushad
//		//		mov		edx, dword ptr [esp + 0x20] //PageFault.ErrorCode
//		//		test	edx, 0x04 //if the processor was in user mode, then
//		//		jnz		PassDown   //pass it down
//
//		mov		eax, cr2
//		cmp		eax, HIGHEST_USER_ADDRESS
//		jae		PassDown
//
//		push	eax
//		//		call	FindPageInHookedList
//		mov		ebp, eax
//		cmp		ebp, ERROR_PAGE_NOT_IN_LIST
//		jz		PassDown
//
//		mov		eax, cr2
//		mov		esi, PROCESS_PAGE_DIR_BASE
//		mov		ebx, eax
//		shr		ebx, 22
//		lea		ebx, [esi + ebx * 4] //ebx = pPTE for large page
//		test[ebx], 0x80     //check if its a large page
//		jnz		IsLargePage
//
//		mov		esi, PROCESS_PAGE_TABLE_BASE
//		mov		ebx, eax
//		shr		ebx, 12
//		lea		ebx, [esi + ebx * 4] //ebx = pPTE
//
//		IsLargePage:
//		cmp[esp + 0x24], eax   //Is due to an attepmted execute?
//			jne		LoadDTLB
//
//			cli
//			or dword ptr[ebx], 0x01
//			mov		eax, [ebx]
//			and eax, 0xFFFFF000
//			push	eax
//			call[ebp].LoadExecutePage	//Eax返回新的执行页
//
//			mov		eax, [ebp].ExecuteView
//			mov		edi, [ebx]
//			and edi, 0x00000FFF //preserve the lower 12 bits of the faulting page's PTE
//			and eax, 0xFFFFF000 //isolate the physical address in the "fake" page's PTE
//			or eax, edi
//			mov		edx, [ebx]		//保存原来的PTE
//			mov[ebx], eax		//替换物理帧
//			mov		eax, cr2		//页失败地址
//			mov		eax, dword ptr[eax]		//载入ITLB
//			mov[ebx], edx		//还原PTE
//			and		dword ptr[ebx], 0xFFFFFFFE
//			sti
//			jmp		ReturnWithoutPassdown
//
//			LoadDTLB :
//		cli
//			or dword ptr[ebx], 0x01
//			mov		eax, dword ptr[eax]		//载入DTLB
//			and dword ptr[ebx], 0xFFFFFFFE
//			sti
//
//			ReturnWithoutPassDown :
//		popad
//			add		esp, 4
//			iretd
//
//			PassDown :
//		popad
//			jmp		OldInt0EHandler
//	}
//}
/*
void HookMemoryPage(
	PVOID ExecutePage,
	PVOID ReadWritePage,
	ULONG PageNumber
	)
{
	HOOKED_LIST_ENTRY HookedPage = {0};

	HookedPage.ExecuteView = ExecutePage;
	HookedPage.ReadWriteView = pReadWritePage;
	HookedPage.PageNumber = PageNumber;
	HookedPage.PageOwner = PsGetCurrentProcess();

	__asm cli //disable interrupts

	if (g_OldInt0EHandler == NULL)
	{
		HookInt(&g_OldInt0EHandler, (unsigned long)NewInt0EHandler, 0x0E);
	}

	HookedPage.pExecutePte = GetPteAddress(pExecutePage);
	HookedPage.pReadWritePte = GetPteAddress(pReadWritePage);

	//Insert the hooked page into the list
	PushPageIntoHookedList(HookedPage);

	//Enable the global page feature
	EnableGlobalPageFeature(HookedPage.pExecutePte);

	//Mark the page non present
	MarkPageNotPresent(HookedPage.pExecutePte);

	//Go ahead and flush the TLBs. We want to guarantee that all
	//subsequent accesses to this hooked page are filtered
	//through our new page fault handler.
	__asm invlpg ExecutePage

	__asm sti //reenable interrupts
}//*/

//typedef struct tagIDTR
//{
//	WORD IDTLimit;
//	WORD LowIDTbase;
//	WORD HiIDTbase;
//} IDTR, *PIDTR;
//
//typedef struct tagIDTENTRY
//{
//	WORD OffsetLow;
//	WORD selector;
//	WORD unused_lo : 13;
//	WORD DPL : 2;
//	WORD P : 1;
//	WORD OffsetHigh;
//} IDTENTRY, *PIDTENTRY;
//
//VOID
//EnableInt0EHook(
//	BOOLEAN Enable
//)
//{
//	IDTR idtr;
//	PIDTENTRY OIdt;
//	PIDTENTRY NIdt;
//
//	if (Enable)
//	{
//		__asm sidt idtr
//
//		OIdt = (PIDTENTRY)MAKELONG(idtr.LowIDTbase, idtr.HiIDTbase);
//		NIdt = &(OIdt[0x0e]);
//
//		__asm cli
//
//		OldInt0EHandler = MAKELONG(OIdt[0x0e].OffsetLow, OIdt[0x0e].OffsetHigh);
//		NIdt->OffsetLow = LOWORD(NewInt0EHandler);
//		NIdt->OffsetHigh = HIWORD(NewInt0EHandler);
//
//		__asm lidt idtr
//		__asm sti
//	}
//	else if (OldInt0EHandler)
//	{
//		__asm sidt idtr
//
//		OIdt = (PIDTENTRY)MAKELONG(idtr.LowIDTbase, idtr.HiIDTbase);
//		NIdt = &(OIdt[0x0e]);
//
//		__asm cli
//
//		NIdt->OffsetLow = LOWORD(OldInt0EHandler);
//		NIdt->OffsetHigh = HIWORD(OldInt0EHandler);
//		OldInt0EHandler = 0;
//
//		__asm lidt idtr
//		__asm sti
//	}
//}




