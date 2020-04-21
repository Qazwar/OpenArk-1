/***************************************************************************************
* AUTHOR :
* DATE   : 2020-2-12
* MODULE : hunter.C
*
* Command:
*	Source of IOCTRL Sample Driver
*
* Description:
*		Demonstrates communications between USER and KERNEL.
*
****************************************************************************************
* Copyright (C) 2010 .
****************************************************************************************/

//#######################################################################################
//# I N C L U D E S
//#######################################################################################




#include "kark.h"
#include "Init.h"
#include "main.h"




//////////////////////////////////////////////////////////////////////////

//#######################################################################################
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@				D R I V E R   E N T R Y   P O I N T						 @@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//#######################################################################################


namespace NT {

	PDRIVER_OBJECT		DriverObject;
}

NTSTATUS
DriverEntry(IN PDRIVER_OBJECT pDriverObj, IN PUNICODE_STRING pRegistryString)
{
	__try
	{
		NTSTATUS		status = STATUS_SUCCESS;
		UNICODE_STRING  ustrLinkName;
		UNICODE_STRING  ustrDevName;
		PDEVICE_OBJECT  pDevObj;
		int i = 0;
		BOOLEAN bResult;

		NT::DriverObject = pDriverObj;//保存驱动对象
		bResult = InitDriver();

		if (!bResult)
			return STATUS_UNSUCCESSFUL;



		dprintf("[hunter] EasySys Sample Driver\r\n"
			"[hunter] Compiled %s %s\r\n[hunter] In DriverEntry : %wZ\r\n",
			__DATE__, __TIME__, pRegistryString);

		// Register dispatch routines
	/*
		for(i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
		{
			pDriverObj->MajorFunction[i] = DispatchCommon;
		}
	*/
		pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
		pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;

		// Dispatch routine for communications
		pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;

		// Unload routine
		pDriverObj->DriverUnload = DriverUnload;

		// Initialize the device name.
		RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);

		// Create the device object and device extension
		status = IoCreateDevice(pDriverObj,
			0,
			&ustrDevName,
			FILE_DEVICE_UNKNOWN,
			0,
			false,
			&pDevObj);

		if (!NT_SUCCESS(status))
		{
			dprintf("[hunter] Error, IoCreateDevice = 0x%x\r\n", status);
			return status;
		}

		//// Get a pointer to our device extension
		//deviceExtension = (PDEVICE_EXTENSION) deviceObject->DeviceExtension;

		//// Save a pointer to the device object
		//deviceExtension->DeviceObject = deviceObject;

		if (IoIsWdmVersionAvailable(1, 0x10))
		{
			//如果是支持符号链接用户相关性的系统
			RtlInitUnicodeString(&ustrLinkName, SYMBOLIC_LINK_GLOBAL_NAME);
		}
		else
		{
			//不支持
			RtlInitUnicodeString(&ustrLinkName, SYMBOLIC_LINK_NAME);
		}

		// Create a symbolic link to allow USER applications to access it. 
		status = IoCreateSymbolicLink(&ustrLinkName, &ustrDevName);

		if (!NT_SUCCESS(status))
		{
			dprintf("[hunter] Error, IoCreateSymbolicLink = 0x%x\r\n", status);

			IoDeleteDevice(pDevObj);
			return status;
		}

		//
		//	TODO: Add initialization code here.
		//

		//// Tell the I/O Manger to do BUFFERED IO
		//deviceObject->Flags |= DO_BUFFERED_IO;

		//// Save the DeviveObject
		//deviceExtension->DeviceObject = deviceObject;

		dprintf("[hunter] DriverEntry Success\r\n");
	}
	__except (1)
	{
		dprintf("加载驱动失败");
		return STATUS_UNSUCCESSFUL;
	}



	return STATUS_SUCCESS;
}

VOID
DriverUnload(IN PDRIVER_OBJECT pDriverObj)
{
	UNICODE_STRING strLink;

	// Unloading - no resources to free so just return.
	dprintf("[hunter] Unloading...\r\n");;

	//
	// TODO: Add uninstall code here.
	//
	if (NT::ImageBaseCopyNt)
	{
		ExFreePool(NT::ImageBaseCopyNt);
	}
	// Delete the symbolic link
	RtlInitUnicodeString(&strLink, SYMBOLIC_LINK_NAME);
	IoDeleteSymbolicLink(&strLink);

	// Delete the DeviceObject
	IoDeleteDevice(pDriverObj->DeviceObject);

	dprintf("[hunter] Unloaded Success\r\n");

	return;
}

NTSTATUS
DispatchCreate(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	dprintf("[hunter] IRP_MJ_CREATE\r\n");

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


NTSTATUS
DispatchClose(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	dprintf("[hunter] IRP_MJ_CLOSE\r\n");

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	// Return success
	return STATUS_SUCCESS;
}

NTSTATUS
DispatchCommon(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0L;

	dprintf("[hunter] Common Dispatch\r\n");

	IoCompleteRequest(pIrp, 0);

	// Return success
	return STATUS_SUCCESS;
}

NTSTATUS
DispatchDeviceControl(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;	 // STATUS_UNSUCCESSFUL
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	ULONG uIoControlCode = 0;
	PVOID pIoBuffer = NULL;
	ULONG uInSize = 0;
	ULONG uOutSize = 0;

	// Get the IoCtrl Code
	uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;

	pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

	switch (uIoControlCode)
	{
	case IOCTL_HELLO_WORLD:
	{
		dprintf("[hunter] Hello World!\r\n");
		ParamInfo *pParem = (ParamInfo*)pIoBuffer;

		RtlZeroMemory(pParem->pOutData, pParem->cbOutData);
		if (pParem->FunIdx < DrvCall::LastId)
		{
			status = DrvCallTable[pParem->FunIdx](pParem->pInData, pParem->cbInData, pParem->pOutData, pParem->cbOutData);
		}

		// Return success
		status = status ? STATUS_SUCCESS : STATUS_INVALID_PARAMETER;
	}
	break;

	case IOCTRL_REC_FROM_APP:
	{
		// Receive data form Application
		DbgPrint("[hunter] Hello World!\r\n");

		dprintf("[hunter] IOCTRL_REC_FROM_APP\r\n");

		// Do we have any data?
		if (uInSize > 0)
		{
			dprintf("[hunter] Get Data from App: %s\r\n", pIoBuffer);
		}

		// Return success
		status = STATUS_SUCCESS;
	}
	break;

	case IOCTRL_SEND_TO_APP:
	{
		// Send data to Application
		dprintf("[hunter] IOCTRL_SEND_TO_APP\r\n");

		// If we have enough room copy the data upto the App - note copy the terminating character as well...
		if (uOutSize >= strlen(DATA_TO_APP) + 1)
		{
			RtlCopyMemory(pIoBuffer,
				DATA_TO_APP,
				strlen(DATA_TO_APP) + 1);

			// Update the length for the App
			pIrp->IoStatus.Information = strlen(DATA_TO_APP) + 1;

			dprintf("[hunter] Send Data to App: %s\r\n", pIoBuffer);

			// Return success
			status = STATUS_SUCCESS;
		}
	}
	break;

	//
	// TODO: Add execute code here.
	//

	default:
	{
		// Invalid code sent
		dprintf("[hunter] Unknown IOCTL: 0x%X (%04X,%04X)\r\n",
			uIoControlCode,
			DEVICE_TYPE_FROM_CTL_CODE(uIoControlCode),
			IoGetFunctionCodeFromCtlCode(uIoControlCode));
		status = STATUS_INVALID_PARAMETER;
	}
	break;
	}

	if (status == STATUS_SUCCESS)
	{
		pIrp->IoStatus.Information = uOutSize;
	}
	else
	{
		pIrp->IoStatus.Information = 0;
	}

	// Complete the I/O Request
	pIrp->IoStatus.Status = status;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}


//
// TODO: Add your module definitions here.
//



/* EOF */
