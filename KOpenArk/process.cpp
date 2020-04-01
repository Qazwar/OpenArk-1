
#include "kark.h"
#pragma warning (disable : 4311)
#pragma warning (disable : 4302)





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
			KeStackAttachProcess(process, &kapc);
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

BOOLEAN ArkGetModListForProc(PCHAR pIndata, ULONG cbInData, ModInfo * pOutData, ULONG cbOutData)
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

BOOLEAN ArkHideMod(HideModParam *pIndata, ULONG cbInData, ModInfo * pOutData, ULONG cbOutData)
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


BOOLEAN ArkGetProcHandleInfo(PCHAR pIndata, ULONG cbInData, ModInfo * pOutData, ULONG cbOutData)
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

void GetModInfoByAvlNode(PMMVAD VadNode, ModInfo * ModInfo)
{
	PCONTROL_AREA controlArea;
	PSEGMENT segment;
	PFILE_OBJECT filePointer;


	if ( VadNode->u.VadFlags.VadType == MI_VAD_TYPE::VadImageMap
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

			sucess = ObQueryNameFileObject(filePointer, ModInfo[ModInfo->NumberOfMods].Path, MAX_PATH,&pathLen);
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

void TraverseAvlMid(PMMVAD VadNode, ModInfo *ModInfo)
{
	while (VadNode)
	{
		TraverseAvlMid(VadNode->LeftChild, ModInfo);
		GetModInfoByAvlNode(VadNode, ModInfo);

		//到这里此节点的左节点为null
		VadNode = VadNode->RightChild;
	}
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
					pStuProcInfo[nProcessNum].wStrProcessPath,MAX_PATH,
					 &returnLength);
			}
			pOutData->ProcessCnt = nProcessNum + 1;
		}
	}
}




BOOLEAN  PsGetProcessPath(PEPROCESS Process, PVOID ProcId, OUT PWSTR Buffer,IN ULONG BufferLength, PULONG ReturnLength)
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