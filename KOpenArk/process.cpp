
#include "kark.h"
#pragma warning (disable : 4311)
#pragma warning (disable : 4302)


//
//  This is the sign low bit used to lock handle table entries
//
#define MIDLEVEL_COUNT (PAGE_SIZE / sizeof(PHANDLE_TABLE_ENTRY))
#define TABLE_PAGE_SIZE PAGE_SIZE
#define LEVEL_CODE_MASK 3
#define EXHANDLE_TABLE_ENTRY_LOCK_BIT    1

#define LOWLEVEL_COUNT (TABLE_PAGE_SIZE / sizeof(HANDLE_TABLE_ENTRY))
#define EX_ADDITIONAL_INFO_SIGNATURE (-2)
#define ExpIsValidObjectEntry(Entry) \
    ( (Entry != NULL) && (Entry->Object != NULL) && (Entry->NextFreeTableEntry != EX_ADDITIONAL_INFO_SIGNATURE) )



BOOLEAN  ArkGetProcList(PCHAR pIndata, ULONG cbInData, StuProcInfo *pOutData, ULONG cbOutData)
{
	char result = FALSE;
	ULONG_PTR u_pOutData = (ULONG_PTR)pOutData;

	__try
	{
		ProbeForWrite(pOutData, cbOutData, 1);
		if (u_pOutData < MmUserProbeAddress && u_pOutData + cbOutData < MmUserProbeAddress)
		{
			PsGetAllProcessInfomation(pOutData, cbOutData);
			GetIdlelProcessInfomatio(pOutData, cbOutData);
			if (pOutData->ProcessCnt)
				result = TRUE;
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
	char result = FALSE;

	__try
	{
		ProbeForRead(pIndata, 8, 1);
		PVOID procId = (PVOID)GETQWORD(pIndata);
		PEPROCESS process;
		NTSTATUS st;
		st = PsLookupProcessByProcessId(procId, &process);
		if (NT_SUCCESS(st) && NT::EPROCESS::ActiveProcessLinksOffset) {

			if (NULL == RemoveEntryList((PLIST_ENTRY)((ULONG_PTR)process + NT::EPROCESS::ActiveProcessLinksOffset))) {
				result = TRUE;
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

			stLock = NT::PsAcquireProcessExitSynchronization(process);
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
						result = TRUE;
					}
					else {
						result = FALSE;
					}

					*pathLen = ldrEntry->FullDllName.Length;
				}
			}
			KeUnstackDetachProcess(&kapc);
			if (NT_SUCCESS(stLock)) {
				NT::PsReleaseProcessExitSynchronization(process);
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
	BOOLEAN result = 0;
	PVOID psId = *(PVOID*)pIndata;
	PEPROCESS process = 0;
	NTSTATUS st;
	PMM_AVL_TABLE vadRoot;

	__try
	{

		st = PsLookupProcessByProcessId(psId, &process);
		if (NT_SUCCESS(st)) {

			ProbeForWrite(pOutData, cbOutData, 1);
			ProbeForRead(pIndata, cbInData, 1);
			vadRoot = (PMM_AVL_TABLE)((ULONG_PTR)process + NT::EPROCESS::VadRootOffset);
			if (MmIsAddressValid(vadRoot->BalancedRoot.RightChild)) {
				TraverseAvlMid((PMMVAD)vadRoot->BalancedRoot.RightChild, pOutData);
			}
		}
	}
	__except (1)
	{
		return FALSE;
	}


	return TRUE;
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
						result = TRUE;
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
						result = TRUE;
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
	

	return BOOLEAN();
}

void GetModInfoByAvlNode(PMMVAD vadNode, ModInfo * modInfo)
{

	PCONTROL_AREA controlArea;
	PSEGMENT segment;
	PFILE_OBJECT filePointer;


	if (MmIsAddressValid(vadNode->Subsection)) {
		controlArea = vadNode->Subsection->ControlArea;
	}
	else {
		return;
	}




	if (controlArea && MmIsAddressValid(controlArea)
		&& (controlArea->u.Flags.Image && controlArea->u.Flags.File)
		&& (vadNode->u.VadFlags.Protection == MM_EXECUTE_WRITECOPY))

	{
		filePointer = (PFILE_OBJECT)(controlArea->FilePointer.Value & 0xFFFFFFFFFFFFFFF0);
		if (filePointer->Size == 0xd8 || filePointer->Size == 0xb8) {

			PWSTR imagePath;
			ULONG len;
			BOOLEAN sucess;

			sucess = FsGetFilePathByFileObject(filePointer, &imagePath, &len);
			if (sucess) {

				memmove(modInfo[modInfo->NumberOfMods].Path, imagePath, len);
				modInfo[modInfo->NumberOfMods].Path[len / 2 + 1] = 0;
				ExFreePool(imagePath);
			}

			modInfo[modInfo->NumberOfMods].RegionBase = vadNode->StartingVpn << 12;
			modInfo[modInfo->NumberOfMods].RegionSize = (vadNode->EndingVpn - vadNode->StartingVpn + 1) << 12;
			modInfo->NumberOfMods++;
		}
	}
}

void TraverseAvlMid(PMMVAD vadNode, ModInfo *modInfo)
{
	while (vadNode)
	{
		if (MmIsAddressValid(vadNode)) {
			TraverseAvlMid(vadNode->LeftChild, modInfo);
		}
		else {
			break;
		}
		GetModInfoByAvlNode(vadNode, modInfo);

		//到这里此节点的左节点为null
		vadNode = vadNode->RightChild;
	}
}


NTSTATUS  PsGetAllProcessInfomation(StuProcInfo *pOutData, ULONG cbOutData)
{

	PHANDLE_TABLE PspCidTable;

	if (NT::PPspCidTable)
	{
		PspCidTable = *(PHANDLE_TABLE*)NT::PPspCidTable;
		ExEnumHandleTable(PspCidTable, pOutData, cbOutData, PsGetProcessInfo, 0);
	}
	if (pOutData->ProcessCnt)
		return TRUE;


	return FALSE;
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
	if (NT::MmIsAddressValid(pHandleTable) != TRUE)
		return FALSE;

	return GETQWORD(pHandleTable + NT::EPROCESS::HANDLE_TABLE::UniqueProcessIdOffset);
}

void ExEnumHandleTable(PHANDLE_TABLE pHandleTable, StuProcInfo *pOutData, ULONG cbOutData, EX_ENUMERATE_HANDLE_ROUTINE funEnumHandleProcedure, PVOID enumParam)
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


	return TRUE;
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

			bIsWow64 = FALSE;
			if (WoW64processOffset)
			{
				if (GETQWORD(WoW64processOffset + process))// WoW64process
					bIsWow64 = TRUE;
			}
			pStuProcInfo[nProcessNum].IsWoW64Process = bIsWow64;// 填充wow64进程标志

			if (CreateTimeOffset)
				pStuProcInfo[nProcessNum].CreateTinme = *(PLARGE_INTEGER)(CreateTimeOffset + process);
			else
				pStuProcInfo[nProcessNum].CreateTinme.QuadPart = 0;


			if (pStuProcInfo[nProcessNum].ProcessId != 0 && pStuProcInfo[nProcessNum].ProcessId != 4) {
				result = PsGetProcessPath((PEPROCESS)process, (PVOID)pStuProcInfo[nProcessNum].ProcessId, &pwStrProcPath, 0);// 接受一个进程路径的缓冲区地址
				if (result)
				{
					wcscpy(pStuProcInfo[nProcessNum].wStrProcessPath, pwStrProcPath);
					ExFreePoolWithTag(pwStrProcPath, TAG);
				}
			}
			pOutData->ProcessCnt = nProcessNum + 1;
		}
	}
}




BOOLEAN  PsGetProcessPath(PEPROCESS process, PVOID procId, OUT PWSTR *pStrProcPath, ULONG *pathLen)
{
	using namespace NT::EPROCESS;
	ULONG ntstatus = STATUS_UNSUCCESSFUL;
	PSECTION_OBJECT sectionObject;
	BOOLEAN result = FALSE;


	if (!SectionObjectOffset)
		return FALSE;

	if (NULL == process) {
		ntstatus = PsLookupProcessByProcessId(procId, (PEPROCESS*)&process);
	}

	if (process && process > MmSystemRangeStart) {

		sectionObject = (PSECTION_OBJECT)GETQWORD((ULONG_PTR)process + SectionObjectOffset);
		if (sectionObject) 
		{
			result = FsGetFilePathBySection(sectionObject, pStrProcPath, pathLen);
			if (FALSE == result) 
			{

				WCHAR path[MAX_PATH];
				int returnLen = MAX_PATH - 2;
				result = PsGetProcessPathByPeb(process, procId, path, &returnLen);
				if (result) 
				{

					*pStrProcPath = (PWSTR)ExAllocatePool(PagedPool, MAX_PATH);
					RtlZeroMemory(*pStrProcPath, MAX_PATH);
					if (*pStrProcPath) 
					{

						RtlCopyMemory(*pStrProcPath, path, returnLen);
						if (pathLen) 
						{
							*pathLen = returnLen;
						}
					}
				}
			}
		}
	}
	if (NT_SUCCESS(ntstatus))
		ObfDereferenceObject((PVOID)process);

	return result;
}