
#include "kark.h"
#pragma warning (disable : 4311)
#pragma warning (disable : 4302)







void EnumProcessThreads(PVOID ProcId, ENUM_THREAD_CALLBACK CallBackFun, ArkThreadInfo * ThreadInfo, ULONG InformationLength)
{
	NTSTATUS st;
	PEPROCESS process;
	PLIST_ENTRY nextThread;
	ULONG totalSize = 0;
	PETHREAD thread;
	ArkThreadInfoEntry *threadInfoEntry = ThreadInfo->Threads;

	st = PsLookupProcessByProcessId((HANDLE)ProcId, &process);
	if (NT_SUCCESS(st))
	{
		nextThread = process->ThreadListHead.Flink;
		while (nextThread != &process->ThreadListHead)
		{
			totalSize += sizeof(ArkThreadInfoEntry);

			if (totalSize > InformationLength) 
			{
				dprintf("EnumProcessThreads：空间不足");
				return;
			}
			else 
			{
				thread = (PETHREAD)(CONTAINING_RECORD(nextThread,ETHREAD,ThreadListEntry));
				CallBackFun(thread, (PVOID)threadInfoEntry);
				ThreadInfo->ThreadCnt += 1;
				
				threadInfoEntry++;
			}

			nextThread = nextThread->Flink;
		}

		ObDereferenceObject(process);
	}

}

void CallbackGetThreadInfo(PETHREAD Thread, PVOID  ThreadInfoBuffer)
{
	ArkThreadInfoEntry *threadInfo = (ArkThreadInfoEntry *)ThreadInfoBuffer;

	threadInfo->ContextSwitches = Thread->Tcb.ContextSwitches;
	threadInfo->Ethread = Thread;
	threadInfo->Priority = Thread->Tcb.Priority;
	threadInfo->StartAddress = Thread->StartAddress;
	threadInfo->State = Thread->Tcb.State;
	threadInfo->Teb = Thread->Tcb.Teb;
	threadInfo->ThreadId = Thread->Cid.UniqueThread;
	threadInfo->Win32StartAddress = Thread->Win32StartAddress;
	
}

BOOLEAN  ArkGetProcList(PCHAR pIndata, ULONG cbInData, StuProcInfo *pOutData, ULONG cbOutData)
{
	char result = false;
	ULONG_PTR u_pOutData = (ULONG_PTR)pOutData;

	__try
	{
		ProbeForWrite(pOutData, cbOutData, 1);
		if (u_pOutData < MmUserProbeAddress && u_pOutData + cbOutData < MmUserProbeAddress)
		{
			PsGetAllProcessInfomation(pOutData, cbOutData);
			GetIdlelProcessInfomatio(pOutData, cbOutData);
			if (pOutData->ProcessCnt)
				result = true;
		}
	}
	__except (1)
	{
		DbgPrint("__except:ArkGetProcList");
	}
	return  result;
}

BOOLEAN ArkHideProcess(PCHAR pIndata, ULONG cbInData, StuProcInfo * pOutData, ULONG cbOutData)
{
	char result = false;

	__try
	{
		ProbeForRead(pIndata, 8, 1);
		PVOID procId = (PVOID)GETQWORD(pIndata);
		PEPROCESS process;
		NTSTATUS st;
		st = PsLookupProcessByProcessId(procId, &process);
		if (NT_SUCCESS(st) && NT::EPROCESS::ActiveProcessLinksOffset) {

			if (NULL == RemoveEntryList((PLIST_ENTRY)((ULONG_PTR)process + NT::EPROCESS::ActiveProcessLinksOffset))) {
				result = true;
			}
		}
	}
	__except (1)
	{
		DbgPrint("__except:ArkHideProcess");
	}
	return  result;
}

BOOLEAN PsGetProcessPathByPeb(PEPROCESS process, PVOID procId, PWSTR path, int *pathLen)
{
	NTSTATUS statu = STATUS_UNSUCCESSFUL;
	NTSTATUS stLock = STATUS_UNSUCCESSFUL;
	KAPC_STATE kapc;
	PPEB peb;
	BOOLEAN result;
	using namespace NT::EPROCESS;

	__try {

		if (PebOffset) {

			//如果进程参数为空使用进程id得到进程
			if (NULL == process && procId) {
				statu = PsLookupProcessByProcessId(procId, &process);
				if (!NT_SUCCESS(statu)) {
					return false;
				}
			}

			stLock = PsAcquireProcessExitSynchronization(process);
			KeStackAttachProcess((PRKPROCESS)process, &kapc);
			peb = *(PPEB*)((ULONG_PTR)process + PebOffset);
			if (peb && peb < MmSystemRangeStart) {

				PLDR_DATA_TABLE_ENTRY ldrEntry;
				PPEB_LDR_DATA	pebLdr;

				pebLdr = peb->Ldr;
				ldrEntry = (PLDR_DATA_TABLE_ENTRY)pebLdr->InLoadOrderModuleList.Flink;
				if (ldrEntry) {
					if (*pathLen >= ldrEntry->FullDllName.Length) {
						RtlCopyMemory(path, ldrEntry->FullDllName.Buffer, ldrEntry->FullDllName.Length);
						result = true;
					}
					else {
						result = false;
					}

					*pathLen = ldrEntry->FullDllName.Length;
				}
			}
			KeUnstackDetachProcess(&kapc);
			if (NT_SUCCESS(stLock)) {
				PsReleaseProcessExitSynchronization(process);
			}
			if (NT_SUCCESS(statu)) {
				ObDereferenceObject(process);
			}
		}
	}
	__except (1)
	{
		DbgPrint("PsGetProcessPathByPeb");
		return false;
	}

	return result;
}

BOOLEAN ArkGetModListForProc(PCHAR pIndata, ULONG cbInData, ArkModInfo * pOutData, ULONG cbOutData)
{
	BOOLEAN result = true;
	PVOID psId = *(PVOID*)pIndata;
	PEPROCESS process = 0;
	NTSTATUS st = STATUS_SUCCESS;
	PMM_AVL_TABLE vadRoot;


	st = PsLookupProcessByProcessId(psId, &process);
	if (NT_SUCCESS(st))
	{
		ProbeForWrite(pOutData, cbOutData, 1);
		ProbeForRead(pIndata, cbInData, 1);
		vadRoot = (PMM_AVL_TABLE)PTR_ADD_OFFSET(process, NT::EPROCESS::VadRootOffset);
		if (vadRoot->BalancedRoot.RightChild)
		{
			TraverseAvlMid((PMMVAD)vadRoot->BalancedRoot.RightChild, pOutData);
		}
	}

	if (NT_SUCCESS(st))
	{
		ObDereferenceObject(process);
	}

	return result;
}

BOOLEAN ArkHideMod(HideModParam *pIndata, ULONG cbInData, ArkModInfo * pOutData, ULONG cbOutData)
{

	typedef struct _PEB32
	{
		BOOLEAN InheritedAddressSpace;      // These four fields cannot change unless the
		BOOLEAN ReadImageFileExecOptions;   //
		BOOLEAN BeingDebugged;              //
		BOOLEAN SpareBool;                  //
		DWORD Mutant;                      // INITIAL_PEB structure is also updated.

		DWORD ImageBaseAddress;
		DWORD Ldr;
	}*PPEB32;

	using namespace NT;
	PVOID procId = pIndata->Id;
	NTSTATUS st = STATUS_UNSUCCESSFUL;
	PEPROCESS process;
	PPEB32 ppeb32;
	ULONG_PTR modBase;
	KAPC_STATE apcSt;
	BOOLEAN result = 0;


	ProbeForRead(pIndata, cbInData, 1);

	modBase = pIndata->ModBase;
	st = ::PsLookupProcessByProcessId(procId, &process);



	if (NT_SUCCESS(st) && EPROCESS::WoW64processOffset)
	{
		if (process != ::IoGetCurrentProcess())
		{
			KeStackAttachProcess(process, &apcSt);
		}
		ppeb32 = (PPEB32)GETQWORD((ULONG_PTR)process + EPROCESS::WoW64processOffset);

		__try
		{
			if (ppeb32)
			{
				PPEB_LDR_DATA32 ldr = (PPEB_LDR_DATA32)(ppeb32->Ldr);
				PLDR_DATA_TABLE_ENTRY32 start = (PLDR_DATA_TABLE_ENTRY32)&ldr->InLoadOrderModuleList;
				PLDR_DATA_TABLE_ENTRY32 next = (PLDR_DATA_TABLE_ENTRY32)ldr->InLoadOrderModuleList.Flink;

				while (start != next)
				{
					if (next->DllBase == modBase)
					{
						PLIST_ENTRY32 Blink;
						PLIST_ENTRY32 Flink;
						PLIST_ENTRY32 Entry = (PLIST_ENTRY32)next;

						Flink = (PLIST_ENTRY32)Entry->Flink;
						Blink = (PLIST_ENTRY32)Entry->Blink;
						Blink->Flink = (ULONG)Flink;
						Flink->Blink = (ULONG)Blink;
						result = true;
						break;
					}
					next = (PLDR_DATA_TABLE_ENTRY32)next->InLoadOrderLinks.Flink;
				}
			}
			else
			{
				PPEB peb = (PPEB)GETQWORD((ULONG_PTR)process + EPROCESS::PebOffset);
				PPEB_LDR_DATA ldr = peb->Ldr;
				PLDR_DATA_TABLE_ENTRY start = (PLDR_DATA_TABLE_ENTRY)&ldr->InLoadOrderModuleList;
				PLDR_DATA_TABLE_ENTRY next = (PLDR_DATA_TABLE_ENTRY)ldr->InLoadOrderModuleList.Flink;


				while (start != next)
				{
					if ((ULONG_PTR)next->DllBase == modBase)
					{
						RemoveEntryList((PLIST_ENTRY)next);
						result = true;
						break;
					}
					next = (PLDR_DATA_TABLE_ENTRY)next->InLoadOrderLinks.Flink;
				}
			}
		}
		__except (1)
		{
			DBGERRINFO;
		}
		KeUnstackDetachProcess(&apcSt);
	}

	if (NT_SUCCESS(st))
	{
		ObReferenceObject(process);
	}


	return result;
}


BOOLEAN ArkGetProcHandleInfo(PCHAR pIndata, ULONG cbInData, ArkModInfo * pOutData, ULONG cbOutData)
{
	PVOID procId = *(PVOID*)pIndata;
	NTSTATUS st = STATUS_UNSUCCESSFUL;
	PEPROCESS process;
	PHANDLE_TABLE handleTable;
	HANDLE handle;

	ProbeForRead(pIndata, cbInData, 1);
	st = PsLookupProcessByProcessId(procId, &process);
	if (NT_SUCCESS(st))
	{
		ProbeForWrite(pOutData, cbOutData, 1);
		handleTable = *(PHANDLE_TABLE*)PTR_ADD_OFFSET(process, NT::EPROCESS::ObjectTableOffset);
		ExEnumHandleTable(handleTable, (EX_ENUMERATE_HANDLE_ROUTINE)ExEnumHandleCallBack, pOutData, &handle);
	}

	if (NT_SUCCESS(st))
	{
		ObDereferenceObject(process);
	}

	return true;
}

BOOLEAN ArkGetProcHandles(PCHAR pIndata, ULONG cbInData, ArkHandleInfo * pOutData, ULONG cbOutData)
{

	PVOID procId = *(PVOID*)pIndata;
	NTSTATUS st = STATUS_UNSUCCESSFUL;
	PEPROCESS process = 0;
	PSYSTEM_HANDLE_INFORMATION handles = 0;
	PSYSTEM_HANDLE_TABLE_ENTRY_INFO handleEntry;
	POBJECT_TYPE_INFORMATION typeInfo = 0;
	POBJECT_NAME_INFORMATION nameInfo = 0;
	KAPC_STATE apcSt;
	BOOLEAN isDstProcess;
	NTSTATUS syncFlag = STATUS_UNSUCCESSFUL;
	ArkHandleInfo tempHandleInfo;
	ULONG handleCnt = 0;
	HANDLE handleQuery = 0;


	ProbeForRead(pIndata, cbInData, 1);
	ProbeForWrite(pOutData, cbOutData, 1);


	st = PsLookupProcessByProcessId(procId, &process);
	if (!NT_SUCCESS(st))
		return false;

	typeInfo = (POBJECT_TYPE_INFORMATION)ExAllocatePool(PagedPool, 0x1000);
	nameInfo = (POBJECT_NAME_INFORMATION)ExAllocatePool(PagedPool, 0x1000);

	isDstProcess = IoGetCurrentProcess() == process;

	st = ArkEnumHandles(&handles);
	if (NT_SUCCESS(st))
	{
		handleEntry = handles->Handles;
		if (typeInfo && nameInfo)
		{
			for (ULONG i = 0; i < handles->NumberOfHandles; i++, handleEntry++)
			{
				if (handleEntry->UniqueProcessId == (USHORT)procId)
				{
					tempHandleInfo.Handle = (HANDLE)handleEntry->HandleValue;
					tempHandleInfo.Object = handleEntry->Object;
					tempHandleInfo.TypeIndex = handleEntry->ObjectTypeIndex;
					tempHandleInfo.RefreceCount =
						OBJECT_TO_OBJECT_HEADER(handleEntry->Object)->PointerCount;
					/*

					通过句柄查询信息，附加到目标进程
					*/
					if (!isDstProcess)
					{
						syncFlag = PsAcquireProcessExitSynchronization(process);
						KeStackAttachProcess(process, &apcSt);
					}

					handleQuery = tempHandleInfo.Handle;
					if (process == NT::SystemProcess)
					{
						handleQuery = (HANDLE)((ULONG_PTR)tempHandleInfo.Handle | (ULONG_PTR)SYSTEM_HANDLE);
					}

					st = ZwQueryObject(handleQuery, ObjectTypeInformation, typeInfo, PAGE_SIZE, 0);
					if (NT_SUCCESS(st))
					{
						memcpy(tempHandleInfo.TypeName, typeInfo->TypeName.Buffer,typeInfo->TypeName.Length);
						tempHandleInfo.TypeName[typeInfo->TypeName.Length / 2] = 0;
					}
					st = ZwQueryObject(handleQuery, (OBJECT_INFORMATION_CLASS)ObjectNameInformation, nameInfo, PAGE_SIZE, 0);
					if (NT_SUCCESS(st))
					{
						memcpy(tempHandleInfo.HandleName, nameInfo->Name.Buffer, nameInfo->Name.Length);
						tempHandleInfo.HandleName[nameInfo->Name.Length / 2] = 0;
					}
					if (!isDstProcess)
					{
						PsReleaseProcessExitSynchronization(process);
						KeUnstackDetachProcess(&apcSt);
					}
					memcpy(&pOutData[handleCnt], &tempHandleInfo, sizeof(tempHandleInfo));
					handleCnt++;
				}

			}
			pOutData->HandleCnt = handleCnt;
			ExFreePool(handles);
		}
		ExFreePool(typeInfo);
		ExFreePool(nameInfo);
	}

	if (process)
	{
		ObDereferenceObject(process);
	}

	dprintf("ArkGetProcHandles:leveing");
	return true;
}

BOOLEAN ArkGetProcThreads(PCHAR pIndata, ULONG cbInData, ArkThreadInfo * pOutData, ULONG cbOutData)
{
	PVOID ProcId = *(PVOID*)pIndata;
	__try
	{
		ProbeForWrite(pOutData, cbOutData, 1);
		ProbeForRead(pIndata, 4, 1);
	}
	__except(1)
	{
		dprintf("AekGetProcThreads");
	}


	EnumProcessThreads(ProcId, CallbackGetThreadInfo, pOutData, cbOutData);

	return true;
}

/*
设置线程状态 终结位清零
设置线程状态为等待，内核禁用apc设置为0，可插入apc对列设为真
调用NtSuspendThread

*/


BOOLEAN ArkSusPendOrResumeThread(ArkThreadSuspendParam *ThreadParam, ULONG cbInData, PVOID  pOutData, ULONG cbOutData)
{
	PETHREAD thread = 0;
	NTSTATUS st;
	PVOID threadId;
	char preMode;
	HANDLE hThread = 0;
	ULONG suspendCount;
	BOOLEAN toSuspend;
	

	threadId = ThreadParam->ThreadId;
	toSuspend = ThreadParam->ToSuspend;
	st = PsLookupThreadByThreadId(threadId, &thread);
	ObOpenObjectByPointer(thread, OBJ_KERNEL_HANDLE, 0, GENERIC_ALL, *PsThreadType, 0, &hThread);

	if (thread && hThread)
	{
		thread->Terminated = 0;
		thread->Tcb.ApcQueueable = 1;
		thread->Tcb.State = KTHREAD_STATE::Waiting;
		thread->Tcb.KernelApcDisable = 0;
		preMode =  KeToKernekModel(KeGetCurrentThread());
		if(toSuspend)
		{
			NT::NtSuspendThread(hThread, &suspendCount);
		}
		else
		{
			NT::NtResumeThread(hThread, &suspendCount);
		}
		KeResumePreviousMode(KeGetCurrentThread(), preMode);
	}

	if (thread)ObpDecodeObject(thread);
	if (hThread)ZwClose(hThread);

	return true;
}

BOOLEAN ArkLookUpSuspendCount(PVOID *ThreadIdPointer, ULONG cbInData, ULONG * SuspendCount, ULONG cbOutData)
{
	PETHREAD thread;
	NTSTATUS st;
	PVOID threadId;

	threadId = *ThreadIdPointer;
	st = PsLookupThreadByThreadId(threadId, &thread);

	if (NT_SUCCESS(st))
	{
		*SuspendCount = thread->Tcb.SuspendCount;
		ObpDecodeObject(thread);
	}
	return true;
}

BOOLEAN ArkTerminateThread(PVOID ThreadId, ULONG cbInData, ULONG * SuspendCount, ULONG cbOutData)
{

	PETHREAD thread;
	NTSTATUS st;
	

	ThreadId = *(PVOID*)ThreadId;
	st = PsLookupThreadByThreadId(ThreadId, &thread);

	if (NT_SUCCESS(st))
	{
		ArkTermianteThreadByThread(thread, 0);
		ObDereferenceObject(thread);
	}
	return true;
}

BOOLEAN ArkForceTerminateThread(PVOID ThreadId, ULONG cbInData, ULONG * SuspendCount, ULONG cbOutData)
{
	PETHREAD thread;
	NTSTATUS st;


	ThreadId = *(PVOID*)ThreadId;
	st = PsLookupThreadByThreadId(ThreadId, &thread);

	if (NT_SUCCESS(st))
	{
		ArkTermianteThreadByThread(thread, 1);
		ObDereferenceObject(thread);
	}
	return true;
}


BOOLEAN ArkInitTerminateThread(PETHREAD Thread)
{
	Thread->Tcb.SystemThread = 0;
	Thread->BreakOnTermination = 0;
	Thread->Terminated = 0;
	if (Thread->Tcb.State == KTHREAD_STATE::Terminated)
	{
		Thread->Tcb.State = KTHREAD_STATE::Initialized;
	}
	Thread->Tcb.SpecialApcDisable = 0;


	return true;
}


void ArkTerminateThreadKernelRoutine(PKAPC Apc)
{
	PETHREAD curThread;

	ExFreePool(Apc);
	curThread = KeGetCurrentThread();
	curThread->Tcb.SystemThread = 1;
	curThread->BreakOnTermination = 0;
	curThread->Terminated = 0;
	PsTerminateSystemThread(0);
}

PEPROCESS ArkGetProcess(PCHAR ProcName)
{
	
	PEPROCESS process;

	for (process = NT::PsIdleProcess;
		process != 0;
		process = NT::PsGetNextProcess(process == NT::PsIdleProcess ? NULL : process)
		)
	{
		
		if (!strncmp( (const char*)process->ImageFileName, ProcName, sizeof(process->ImageFileName)))
		{
			return process;
		}
		
	}
	return 0;
}
#pragma warning (disable : 4244)
BOOLEAN ArkGetAllSsdtFunAddr(PVOID, PVOID, PVOID *FunAddr, ULONG ReturnSize)
{
	
	ULONG funLimit;
	PEPROCESS csrss = 0;
	PLONG OffsetTable = 0;
	long offset;
	KAPC_STATE apcState;
	

	funLimit = NT::KeServiceDescriptorTable->Limit;
	ProbeForWrite( FunAddr, funLimit * sizeof(PVOID),8);
	OffsetTable = NT::KeServiceDescriptorTable->Base;

	for (ULONG i = 0; i < funLimit; i++)
	{
		offset = OffsetTable[i] >> 4;
		FunAddr[i] = PTR_ADD_OFFSET(OffsetTable, offset);
	}

	return true;
}

BOOLEAN ArkTerminateProcess(PVOID ProcessId)
{
	PEPROCESS process;
	NTSTATUS st;

	
	if ((ULONG_PTR)ProcessId == 4 ||
		IoGetCurrentProcess()->UniqueProcessId == ProcessId)
	{
		return true;
	}


	st = PsLookupProcessByProcessId(ProcessId, &process);

	if (NT_SUCCESS(st))
	{
		PLIST_ENTRY entry;
		PLIST_ENTRY nextEntry;
		PETHREAD thread;
		HANDLE processHandle;
		PETHREAD curThread;

		st = PsAcquireProcessExitSynchronization(process);
		process->ProcessDelete = 0;
		process->ProcessSelfDelete = 0;

		entry = &process->ThreadListHead;
		nextEntry = entry->Flink;

		for (; entry != nextEntry; nextEntry = nextEntry->Flink)
		{
			thread = CONTAINING_RECORD(nextEntry, _KTHREAD, ThreadListEntry);
			thread->Tcb.SystemThread = 0;
			thread->BreakOnTermination = 0;
			thread->Terminated = 0;
		}
		ObOpenObjectByPointer(process, OBJ_KERNEL_HANDLE, 0, GENERIC_ALL, *PsProcessType, 0, &processHandle);
		curThread = KeGetCurrentThread();
		curThread->Tcb.PreviousMode = 0;
		NT::NtTerminateProcess(processHandle, 0);
		NtClose(processHandle);
		ObDereferenceObject(process);

		if (NT_SUCCESS(st)) {
			PsReleaseProcessExitSynchronization(process);
			dprintf("ArkTerminateProcess:leaving");
		}
		curThread->Tcb.PreviousMode = 1;
	}
	return true;
}


BOOLEAN ArkTerminateSystemThread(PETHREAD Thread)
{
	PKAPC apc;
	BOOLEAN result;

	apc = (PKAPC)ExAllocatePool(POOL_TYPE::PagedPool, sizeof(KAPC));
	if (apc)
	{
		RtlZeroMemory(apc, sizeof(apc));
		apc->Type = ApcObject;
		apc->Size = sizeof(apc);
		apc->ApcStateIndex = OriginalApcEnvironment;
		apc->Thread = Thread;
		apc->Reserved[0] = ArkTerminateThreadKernelRoutine;
		result = KeInsertQueueApc(apc, 0, 0, 0);
		if (!result)
		{
			ExFreePool(apc);
		}
	}

	return result;
}



BOOLEAN ArkTermianteThreadByThread(PETHREAD Thread, BOOLEAN InitTerminateFalg)
{
	NTSTATUS st;
	HANDLE handle;
	BOOLEAN result = false;

	st = ObOpenObjectByPointer(Thread, OBJ_KERNEL_HANDLE, 0, GENERIC_ALL, *PsThreadType, 0, &handle);
	if (NT_SUCCESS(st))
	{
		ArkAlterThread(handle, Thread);
		if (PsInitialSystemProcess == IoThreadToProcess(Thread))// 如果是系统线程
		{
			ArkInitTerminateThread(Thread);
			result = ArkTerminateSystemThread(Thread);
		}
		else
		{
			if (InitTerminateFalg)
				ArkInitTerminateThread(Thread);

			st = NT::PspTerminateThreadByPointer(Thread, 0, 0);
			result = NT_SUCCESS(st) ? 1 : 0;
			
		}
		ZwClose(handle);
	}
	return result;
}

NTSTATUS ArkAlterThread(HANDLE Handle, PETHREAD Thread)
{
	NTSTATUS st;
	UCHAR kernelAlterd;
	UCHAR userAlterd;
	PETHREAD curThread;
	char preMode;

	Thread->Tcb.Alertable = 1;
	kernelAlterd = Thread->Tcb.Alerted[KernelMode];
	userAlterd = Thread->Tcb.Alerted[UserMode];

	Thread->Tcb.Alerted[KernelMode] = 0;
	Thread->Tcb.Alerted[UserMode] = 0;

	curThread = KeGetCurrentThread();
	preMode = KeToKernekModel(curThread);

	st = ZwAlertThread(Handle);

	KeResumePreviousMode(curThread, preMode);
	Thread->Tcb.Alerted[KernelMode] = kernelAlterd;
	Thread->Tcb.Alerted[UserMode] = userAlterd;

	return st;
}

BOOLEAN ArkGetSystemModInfo(PCHAR pIndata, ULONG cbInData, ArkModInfo * ModInfo, ULONG cbOutData)
{
	PLDR_DATA_TABLE_ENTRY nextMod = (PLDR_DATA_TABLE_ENTRY)NT::PsLoadedModuleList->Flink;
	int numOfMod = 0;
	auto tempModInfo = ModInfo;

	for (;(PLDR_DATA_TABLE_ENTRY)NT::PsLoadedModuleList != nextMod;
		nextMod = (PLDR_DATA_TABLE_ENTRY)nextMod->InLoadOrderLinks.Flink,
		numOfMod++,
		ModInfo++
		)
	{
		ModInfo->RegionBase = (ULONG_PTR)nextMod->DllBase;
		ModInfo->RegionSize = nextMod->SizeOfImage;
		memcpy( ModInfo->Path,nextMod->FullDllName.Buffer,  nextMod->FullDllName.Length);
	}

	tempModInfo->NumberOfMods = numOfMod;
	return true;
}



void GetModInfoByAvlNode(PMMVAD VadNode, ArkModInfo * ModInfo)
{
	PCONTROL_AREA controlArea;
	PSEGMENT segment;
	PFILE_OBJECT filePointer;


	if (VadNode->u.VadFlags.VadType == MI_VAD_TYPE::VadImageMap
		&& VadNode->Subsection)
		controlArea = VadNode->Subsection->ControlArea;
	else
		return;

	if (controlArea
		&& (controlArea->u.Flags.Image && controlArea->u.Flags.File)
		&& (VadNode->u.VadFlags.Protection == MM_EXECUTE_WRITECOPY))

	{
		filePointer = (PFILE_OBJECT)(controlArea->FilePointer.Value & 0xFFFFFFFFFFFFFFF0);
		if (filePointer->Size == 0xd8 || filePointer->Size == 0xb8)
		{
			BOOLEAN sucess;
			ULONG pathLen;

			sucess = ObQueryNameFileObject(filePointer, ModInfo[ModInfo->NumberOfMods].Path,
				sizeof(ModInfo[ModInfo->NumberOfMods].Path), &pathLen);
			if (sucess)
			{
				ModInfo[ModInfo->NumberOfMods].Path[pathLen / 2 + 1] = 0;
			}

			ModInfo[ModInfo->NumberOfMods].RegionBase = VadNode->StartingVpn << 12;
			ModInfo[ModInfo->NumberOfMods].RegionSize = (VadNode->EndingVpn - VadNode->StartingVpn + 1) << 12;
			ModInfo->NumberOfMods++;
		}
	}
}

void TraverseAvlMid(PMMVAD VadNode, ArkModInfo *ModInfo)
{
	while (VadNode)
	{
		TraverseAvlMid(VadNode->LeftChild, ModInfo);
		GetModInfoByAvlNode(VadNode, ModInfo);

		//到这里此节点的左节点为null
		VadNode = VadNode->RightChild;
	}
}

PEPROCESS ArkGetNextProcess(PEPROCESS Process)
{
	PEPROCESS newProcess = NULL;
	PLIST_ENTRY listEntry;
	if (Process)
	{
		listEntry = Process->ActiveProcessLinks.Flink;
		newProcess = CONTAINING_RECORD(listEntry, _KPROCESS, ActiveProcessLinks);
	}
	else
	{
		newProcess = NT::PsIdleProcess;
	}
	return newProcess;
}


NTSTATUS  PsGetAllProcessInfomation(StuProcInfo *pOutData, ULONG cbOutData)
{

	PHANDLE_TABLE PspCidTable;

	if (NT::PPspCidTable)
	{
		PspCidTable = *(PHANDLE_TABLE*)NT::PPspCidTable;
		ArkEnumHandleTable(PspCidTable, pOutData, cbOutData, PsGetProcessInfo, 0);
	}
	if (pOutData->ProcessCnt)
		return true;


	return false;
}


void  PsGetProcessInfo(ULONG_PTR pObject, StuProcInfo *pOutData, ULONG cbOutData, PVOID param)
{
	using namespace NT::EPROCESS;
	POBJECT_HEADER pObjectHeader;
	UCHAR chTypeIndex;


	pObjectHeader = OBJECT_TO_OBJECT_HEADER(pObject);

	if ((PVOID)pObject > MmSystemRangeStart && MmIsAddressValid(pObjectHeader))
	{
		if (NT::OsVersion >= Windows7_RTM)
		{
			// 得到对象类型下标
			chTypeIndex = pObjectHeader->TypeIndex;

			/*if (NT::OsVersion >= Windows10_2015){ }*/
				//TypeIndex = chTypeIndx ^ BYTE1(pObjectHeader);

			// 是进程对象填充缓冲区并返回
			if (chTypeIndex == NT::ProcessTypeIndex)
			{
				if (GETQWORD(pObject + ObjectTableOffset))
				{
					FillProcessInfo(pObject, pOutData, cbOutData);
				}
				return;
			}
		}
	}

}






ULONG_PTR  PsGetProcessIdFromHandleTable(ULONG_PTR pEprocess)
{
	using namespace NT::EPROCESS;
	ULONG_PTR pHandleTable;


	pHandleTable = GETQWORD(ObjectTableOffset + pEprocess);


	return GETQWORD(pHandleTable + NT::EPROCESS::HANDLE_TABLE::UniqueProcessIdOffset);
}

void ArkEnumHandleTable(PHANDLE_TABLE pHandleTable, StuProcInfo *pOutData, ULONG cbOutData, ArkEX_ENUMERATE_HANDLE_ROUTINE funEnumHandleProcedure, PVOID enumParam)
{

	BOOLEAN ResultValue;
	EXHANDLE LocalHandle;
	PHANDLE_TABLE_ENTRY HandleTableEntry;


	for (LocalHandle.Value = 0; // does essentially the following "LocalHandle.Index = 0, LocalHandle.TagBits = 0;"
		(HandleTableEntry = ExpLookupHandleTableEntry(pHandleTable, LocalHandle)) != NULL;
		LocalHandle.Value += HANDLE_VALUE_INC) {

		//
		//  Only do the callback if the entry is not free
		//

		ULONG_PTR pObject = 0;
		if (ExpIsValidObjectEntry(HandleTableEntry)) {

			if (NT::OsVersion >= Windows8) {

				if (Windows8 == NT::OsVersion)
					pObject = HandleTableEntry->Value >> 19 & 0xFFFFFFFFFFFFFFF0;
				else
					pObject = HandleTableEntry->Value >> 16 & 0xFFFFFFFFFFFFFFF0;
			}
			else {
				pObject = HandleTableEntry->Value & 0xFFFFFFFFFFFFFFFE;
			}

			(*funEnumHandleProcedure)(pObject, pOutData, cbOutData, enumParam);
		}

	}
}






PHANDLE_TABLE_ENTRY
ExpLookupHandleTableEntry(
	IN PHANDLE_TABLE HandleTable,
	IN EXHANDLE tHandle
)

/*++

Routine Description:

	This routine looks up and returns the table entry for the
	specified handle value.

Arguments:

	HandleTable - Supplies the handle table being queried

	tHandle - Supplies the handle value being queried

Return Value:

	Returns a pointer to the corresponding table entry for the input
		handle.  Or NULL if the handle value is invalid (i.e., too large
		for the tables current allocation.

--*/

{
	ULONG_PTR i, j, k;
	ULONG_PTR CapturedTable;
	ULONG TableLevel;
	PHANDLE_TABLE_ENTRY Entry = NULL;
	EXHANDLE Handle;

	PUCHAR TableLevel1;
	PUCHAR TableLevel2;
	PUCHAR TableLevel3;

	ULONG_PTR MaxHandle;




	//
	// Extract the handle index
	//
	Handle = tHandle;

	Handle.TagBits = 0;

	MaxHandle = *(volatile ULONG *)&HandleTable->NextHandleNeedingPool;

	//
	// See if this can be a valid handle given the table levels.
	//
	if (Handle.Value >= MaxHandle) {
		return NULL;
	}

	//
	// Now fetch the table address and level bits. We must preserve the
	// ordering here.
	//
	CapturedTable = *(volatile ULONG_PTR *)&HandleTable->TableCode;

	//
	//  we need to capture the current table. This routine is lock free
	//  so another thread may change the table at HandleTable->TableCode
	//

	TableLevel = (ULONG)(CapturedTable & LEVEL_CODE_MASK);
	CapturedTable = CapturedTable - TableLevel;

	//
	//  The lookup code depends on number of levels we have
	//

	switch (TableLevel) {

	case 0:

		//
		//  We have a simple index into the array, for a single level
		//  handle table
		//


		TableLevel1 = (PUCHAR)CapturedTable;

		//
		// The index for this level is already scaled by a factor of 4. Take advantage of this
		//

		Entry = (PHANDLE_TABLE_ENTRY)&TableLevel1[Handle.Value *
			(sizeof(HANDLE_TABLE_ENTRY) / HANDLE_VALUE_INC)];

		break;

	case 1:

		//
		//  we have a 2 level handle table. We need to get the upper index
		//  and lower index into the array
		//


		TableLevel2 = (PUCHAR)CapturedTable;

		i = Handle.Value % (LOWLEVEL_COUNT * HANDLE_VALUE_INC);//i==1级句柄值

		Handle.Value -= i;
		j = Handle.Value / ((LOWLEVEL_COUNT * HANDLE_VALUE_INC) / sizeof(PHANDLE_TABLE_ENTRY));
		//j==2级句柄表的字节偏移           j = value / (512*4) * 4 

		TableLevel1 = (PUCHAR) *(PHANDLE_TABLE_ENTRY *)&TableLevel2[j];
		Entry = (PHANDLE_TABLE_ENTRY)&TableLevel1[i * (sizeof(HANDLE_TABLE_ENTRY) / HANDLE_VALUE_INC)];
		//一级句柄表的句柄值除4得到index乘8得到entry地址偏移
		break;

	case 2:

		//
		//  We have here a three level handle table.
		//


		TableLevel3 = (PUCHAR)CapturedTable;

		i = Handle.Value % (LOWLEVEL_COUNT * HANDLE_VALUE_INC);

		Handle.Value -= i;

		k = Handle.Value / ((LOWLEVEL_COUNT * HANDLE_VALUE_INC) / sizeof(PHANDLE_TABLE_ENTRY));//得到总的2级句柄的字节偏移

		j = k % (MIDLEVEL_COUNT * sizeof(PHANDLE_TABLE_ENTRY));//得到2级句柄表字节偏移

		k -= j;

		k /= MIDLEVEL_COUNT;//得到3级表的字节偏移


		TableLevel2 = (PUCHAR) *(PHANDLE_TABLE_ENTRY *)&TableLevel3[k];
		TableLevel1 = (PUCHAR) *(PHANDLE_TABLE_ENTRY *)&TableLevel2[j];
		Entry = (PHANDLE_TABLE_ENTRY)&TableLevel1[i * (sizeof(HANDLE_TABLE_ENTRY) / HANDLE_VALUE_INC)];

		break;

	default:
		_assume(0);
	}

	return Entry;
}

int  GetIdlelProcessInfomatio(StuProcInfo *pOutData, ULONG cbOutData)
{
	using namespace NT;
	/*

	1.得到当前线程的kprcb
	2.得到kprcb->idelthread
	3.通过线程的到进程
	4.填写位段
	*/
	PKPRCB kprcb;
	PETHREAD idleThread;
	PEPROCESS eprocess;

	kprcb = GetCurrentPrcb();
	idleThread = kprcb->IdleThread;
	eprocess = (PEPROCESS)(GETQWORD((ULONG_PTR)idleThread + KTHREAD::ProcessOffset));

	int nProcessNum;
	nProcessNum = pOutData->ProcessCnt;

	pOutData[nProcessNum].Process = (ULONG_PTR)eprocess;
	pOutData[nProcessNum].ProcessId = GETQWORD((ULONG_PTR)eprocess + EPROCESS::UniqueProcessIdOffset);
	pOutData[nProcessNum].CreateTinme.QuadPart = 0;
	pOutData[nProcessNum].CreateTinme.QuadPart = GETQWORD((ULONG_PTR)eprocess + EPROCESS::CreateTimeOffset);
	pOutData->ProcessCnt++;


	return true;
}







void  FillProcessInfo(ULONG_PTR process, StuProcInfo *pOutData, ULONG cbOutData)
{
	using namespace NT::EPROCESS;
	/*
	在进程列表功能中前两个参数一样,后三个参数为0

	*/

	ULONG result;
	ULONG nProcessNum;
	ULONG_PTR nProcessId;
	BOOLEAN bIsWow64;
	WCHAR *pwStrProcPath = 0;


	result = MmIsAddressValid((PVOID)process);
	if (result)
	{
		if (!UniqueProcessIdOffset || !InheritedFromUniqueProcessIdOffset)
			return;

		nProcessNum = GETDWORD(pOutData);
		result = sizeof(StuProcInfo) * nProcessNum;
		if (result < cbOutData)
		{
			auto pStuProcInfo = pOutData;

			// 检验这个进程是否已经在数据里
			for (size_t i = 0; i < nProcessNum; i++, pStuProcInfo++)
			{
				if (process == pStuProcInfo->Process)
					return;
			}

			pStuProcInfo = pOutData;
			pStuProcInfo[nProcessNum].Process = process;
			nProcessId = PsGetProcessIdFromHandleTable(process);
			pStuProcInfo[nProcessNum].ProcessId = nProcessId;// 填充id
			if (!nProcessId)//从句柄表中得到id失败转为从eprocess中得到
				pStuProcInfo[nProcessNum].ProcessId = GETQWORD(process + UniqueProcessIdOffset);// 从eprocess得到进程id
			pStuProcInfo[nProcessNum].ParentProcessId = GETQWORD(process + InheritedFromUniqueProcessIdOffset);// 填充父进程id

			bIsWow64 = false;
			if (WoW64processOffset)
			{
				if (GETQWORD(WoW64processOffset + process))// WoW64process
					bIsWow64 = true;
			}
			pStuProcInfo[nProcessNum].IsWoW64Process = bIsWow64;// 填充wow64进程标志

			if (CreateTimeOffset)
				pStuProcInfo[nProcessNum].CreateTinme = *(PLARGE_INTEGER)(CreateTimeOffset + process);
			else
				pStuProcInfo[nProcessNum].CreateTinme.QuadPart = 0;


			if (pStuProcInfo[nProcessNum].ProcessId != 0 && pStuProcInfo[nProcessNum].ProcessId != 4) {
				ULONG returnLength;

				result = PsGetProcessPath((PEPROCESS)process, (PVOID)pStuProcInfo[nProcessNum].ProcessId,
					pStuProcInfo[nProcessNum].wStrProcessPath, sizeof(pStuProcInfo[nProcessNum].wStrProcessPath),
					&returnLength);
			}
			pOutData->ProcessCnt = nProcessNum + 1;
		}
	}
}




BOOLEAN  PsGetProcessPath(PEPROCESS Process, PVOID ProcId, OUT PWSTR Buffer, IN ULONG BufferLength, PULONG ReturnLength)
{
	using namespace NT::EPROCESS;
	ULONG status = STATUS_UNSUCCESSFUL;
	PSECTION_OBJECT sectionObject;
	BOOLEAN result = false;


	if (!SectionObjectOffset)
		return false;

	if (NULL == Process)
	{
		status = PsLookupProcessByProcessId(ProcId, &Process);
	}

	if (Process > MmSystemRangeStart)
	{

		sectionObject = *(PSECTION_OBJECT*)PTR_ADD_OFFSET(Process, SectionObjectOffset);
		if (sectionObject)
		{
			PSEGMENT      segment;
			PCONTROL_AREA controlArea;
			PFILE_OBJECT	fileObject;

			segment = (PSEGMENT)sectionObject->Segment;
			controlArea = segment->ControlArea;
			fileObject = (PFILE_OBJECT)controlArea->FilePointer.Object;
			//check version
			if (NT::OsVersion >= Windows_Vista)
				fileObject = (PFILE_OBJECT)((ULONG_PTR)fileObject & 0xFFFFFFFFFFFFFFF0);

			return ObQueryNameFileObject(fileObject, Buffer, BufferLength, ReturnLength);
		}
	}

	if (NT_SUCCESS(status))
	{
		ObDereferenceObject(Process);
	}

	return result;
}