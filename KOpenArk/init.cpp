
#define _DYNDATA_PRIVATE
#include "kark.h"
#include "Init.h"
#include "Zydis/Zydis.h"
#include <ntimage.h>
/*
zydis 要增加预处理器定义  包含的lib并没有导出，
增加预处理器防止名称不对称


*/
DrvCallFun  DrvCallTable[DrvCall::LastId];

namespace NT {

	WCHAR				NtFullName[NAMELEN];
}

BOOLEAN InitVarible()
{
	//........................................................................................................................................初始化ntkrnl导出
	if (!InitExportByNtkrnl())
		return false;

	//........................................................................................................................................//初始化未导出变量
	if (!InitUnExportByNtkrnl())
		return false;

	//........................................................................................................................................初始化Eprocess相关数据
	

	return true;
}

BOOLEAN CheckVersion()
{
	if (NT::OsVersion != Windows7_SP1)
	{
		return false;
	}


	return true;
}


/*
1.通过引入导入变量NtBuildNumber从驱动的模块链表找到ntoskrnl模块的地址
2.复制ntoskenl文件,拉伸重定位初始化相关变量
*/
BOOLEAN InitNtInfo()
{
	//得到空闲进程
	NT::PsIdleProcess = IoThreadToProcess(GetCurrentPrcb()->IdleThread);



	__try
	{
		PLDR_DATA_TABLE_ENTRY  pFlink;
		PVOID vaNtBuildNumber = &NtBuildNumber;
		NT::OsVersion = (USHORT)NtBuildNumber;
		NT::SystemProcess = IoGetCurrentProcess();
		pFlink = (PLDR_DATA_TABLE_ENTRY)((PLDR_DATA_TABLE_ENTRY)NT::DriverObject->DriverSection)->InLoadOrderLinks.Flink;

		while (pFlink != (PLDR_DATA_TABLE_ENTRY)NT::DriverObject->DriverSection)
		{

			if (vaNtBuildNumber > pFlink->DllBase && (ULONG_PTR)vaNtBuildNumber < (ULONG_PTR)pFlink->DllBase + pFlink->SizeOfImage)
			{
				NT::ImageBaseRunNt = (PCHAR)pFlink->DllBase;
				NT::SizeOfNtImage = pFlink->SizeOfImage;
				memmove(NT::NtFullName, pFlink->FullDllName.Buffer, pFlink->FullDllName.Length);

				HANDLE fileHandle;
				OBJECT_ATTRIBUTES objAttribute;
				UNICODE_STRING objName;
				NTSTATUS status;

				RtlInitUnicodeString(&objName, NTKROSPATH);
				IO_STATUS_BLOCK iosb;
				/* Initialize the object attributes */
				InitializeObjectAttributes(&objAttribute,&objName,OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,NULL,NULL);
					
					
				status = ZwCreateFile(&fileHandle, GENERIC_READ, &objAttribute, &iosb, 0, 
					FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN,
					FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, 0, 0);
				if (NT_SUCCESS(status)) {
					FILE_STANDARD_INFORMATION standardInfo;

					if (NT_SUCCESS(status)) 
					{
						ZwQueryInformationFile(fileHandle, &iosb, &standardInfo,sizeof(standardInfo), FileStandardInformation);
							

						PCHAR buffer = (PCHAR)ExAllocatePool(PagedPool, standardInfo.AllocationSize.QuadPart);
						if (buffer) 
						{
							status = ZwReadFile(fileHandle, 0, 0, 0, &iosb, buffer, standardInfo.AllocationSize.LowPart, 0, 0);
							if (NT_SUCCESS(status)) 
							{
								NT::ImageBaseCopyNt = PeStretchImage(buffer);
								ULONG_PTR ImageBase = ((PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET(NT::ImageBaseCopyNt))->ImageBase;
								PeFixRelocTable(NT::ImageBaseCopyNt, (ULONG)((ULONG_PTR)NT::ImageBaseRunNt - ImageBase));
							}
							ExFreePool(buffer);
						}
					}
				}
				ZwClose(fileHandle);
				return true;
			}
			pFlink = (PLDR_DATA_TABLE_ENTRY)pFlink->InLoadOrderLinks.Flink;
		}

		


	}
	__except (1)
	{
		DBGERRINFO;
	
	}
	return false;
}

BOOLEAN InitExportByNtkrnl()
{
	ULONG rva = 0;

	LOADEXPORT(PsLookupProcessByProcessId);
	LOADEXPORT(IoFileObjectType);
	



	return true;
}
BOOLEAN InitUnExportByNtkrnl()
{
	LOADUNEXPORT(InitPspCidTable);
	LOADUNEXPORT(InitObTypeIndexTable);
	LOADUNEXPORT(InitPsLoadedModuleList);
	LOADUNEXPORT(InitKeServiceDescriptorTable);
	LOADUNEXPORT(InitPspTerminateThreadByPointer);
	LOADUNEXPORT(InitKiInsertQueueApc);
	LOADUNEXPORT(InitPsGetNextProcess);
	LOADUNEXPORT(InitPspCreateProcessNotifyRoutine);
	LOADUNEXPORT(InitPspCreateThreadNotifyRoutine);
	LOADUNEXPORT(InitPspLoadImageNotifyRoutine);
	LOADUNEXPORT(InitCallbackListHead);

	return true;
}
BOOLEAN InitOffset()
{
	using namespace NT;
	switch (NT::OsVersion)
	{
	case Windows7_RTM:
	case Windows7_SP1:
	{
		NT::ProcessTypeIndex = 7;
		EPROCESS::ObjectTableOffset = 0x200;
		EPROCESS::HANDLE_TABLE::UniqueProcessIdOffset = 0x10;
		EPROCESS::UniqueProcessIdOffset = 0x180;
		EPROCESS::InheritedFromUniqueProcessIdOffset = 0x290;
		EPROCESS::WoW64processOffset = 0x320;
		EPROCESS::CreateTimeOffset = 0x168;
		EPROCESS::SectionObjectOffset = 0x268;
		EPROCESS::ImageFileNameOffset = 0x2E0;
		EPROCESS::PebOffset = 0x338;
		EPROCESS::ActiveProcessLinksOffset = 0x188;
		EPROCESS::VadRootOffset = 0x448;

		KTHREAD::ProcessOffset = 0x210;
		KTHREAD::PreviousModeOffset = 0x1F6;



		break;
	}

	default:
		return  0;
	}





	return true;
}
BOOLEAN InitPspCidTable()
{
	ZydisDecoder			decoder;								//解码器
	ZydisFormatter			formatter;
	ULONG_PTR curDisAddr = (ULONG_PTR)NT::PsLookupProcessByProcessId;
	const ZyanUSize length = 100;
	ZydisDecodedInstruction instruction;

	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);


	for (int i = 0; i < length; i += instruction.length)
	{
		if (!ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)curDisAddr, length - instruction.length, &instruction)))
			break;

		
		if ((GETDWORD(curDisAddr) & 0xFFFFFF) == 0xD8B48 && instruction.length == 7)
		{
			ULONG offset = GETDWORD(curDisAddr + 3);
			LARGE_INTEGER dstAddr;
			dstAddr.QuadPart = curDisAddr + instruction.length;

			dstAddr.LowPart += offset;


			NT::PPspCidTable = (PHANDLE_TABLE*)dstAddr.QuadPart;
			return true;
		}
		curDisAddr += instruction.length;
	}

	return false;
}
/*
1.初始化顺序
	1.1  先初始化nt的基本信息
	1.2  初始化导出变量
	1.3  初始化版本差异的offset
	1.4  初始化驱动服务
	*/
BOOLEAN InitDriver()
{

	BOOLEAN result = false;

	result = InitNtInfo();
	if (!result)
		return false;

	result = CheckVersion();
	if (!result)
		return false;

	result = InitVarible();
	if (!result)
		return false;

	result = InitOffset();
	if (!result)
		return false;

	result = InitNtServiceByIndex();
	if (!result)
		return false;

	result = InitDrvCallTable();
	if (!result)
		return false;

	return true;
}

BOOLEAN InitDrvCallTable()
{
	DrvCallTable[ProcList] = (DrvCallFun)ArkGetProcList;
	DrvCallTable[ObjDirectory] = (DrvCallFun)ArkGetObjectDirectoryInfo;
	DrvCallTable[HideMod] = (DrvCallFun)ArkHideMod;
	DrvCallTable[HideProcess] = (DrvCallFun)ArkHideProcess;
	DrvCallTable[ModList] = (DrvCallFun)ArkGetModListForProc;
	DrvCallTable[ProcHandleList] = (DrvCallFun)ArkGetProcHandles;
	DrvCallTable[ProcThreadList] = (DrvCallFun)ArkGetProcThreads;
	DrvCallTable[SystemMods] = (DrvCallFun)ArkGetSystemModInfo;
	DrvCallTable[SusPendCount] = (DrvCallFun)ArkLookUpSuspendCount;
	DrvCallTable[SuspendThreadEnum] = (DrvCallFun)ArkSusPendOrResumeThread;
	DrvCallTable[TerminateThreadFunIndex] = (DrvCallFun)ArkTerminateThread;
	DrvCallTable[ForceTerminateThread] = (DrvCallFun)ArkForceTerminateThread;
	//DrvCallTable[OpenFile] = (DrvCallFun)ArkOpenFile;
	DrvCallTable[QueryHwnd] = (DrvCallFun)ArkGetWindowsForProcess;
	DrvCallTable[GetAllSsdtFunAddr] = (DrvCallFun)ArkGetAllSsdtFunAddr;
	DrvCallTable[GetAllShadowSdtFunAddr] = (DrvCallFun)ArkGetAllShadowSdtFunAddr;
	DrvCallTable[CallIdxTerminate] = (DrvCallFun)ArkTerminateProcess;
	DrvCallTable[CallIdxGdtInfo] = (DrvCallFun)ArkGetGdtInfo;
	DrvCallTable[CallIdxGetDriverInfo] = (DrvCallFun)ArkGetDriverModsInfo;
	DrvCallTable[CallIdxGetCallbackInfo] = (DrvCallFun)ArkGetCallBackInfo;

	return true;
}




BOOLEAN InitObTypeIndexTable()
{
	ZydisDecoder			decoder;								//解码器
	ZydisFormatter			formatter;
	ULONG_PTR curDisAddr = (ULONG_PTR)&ObReferenceObjectByPointerWithTag;
	const ZyanUSize length = 100;
	ZydisDecodedInstruction instruction;

	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);


	for (int i = 0; i < length; i += instruction.length)
	{
		if (!ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)curDisAddr, length - instruction.length, &instruction)))
			break;


		if ((GETDWORD(curDisAddr) & 0xFFFFFF) == 0x0d8d48 && instruction.length == 7)
		{
			ULONG offset = GETDWORD(curDisAddr + 3);
			LARGE_INTEGER dstAddr;

			dstAddr.QuadPart = curDisAddr + instruction.length;
			dstAddr.LowPart += offset;
			NT::ObTypeIndexTable = (POBJECT_TYPE *)dstAddr.QuadPart;
			break;
		}
		curDisAddr += instruction.length;
	}

	POBJECT_TYPE *pObjectType;
	POBJECT_TYPE dirType = 0;


	if (!NT::ObTypeIndexTable)
		return false;

	pObjectType = NT::ObTypeIndexTable;
	pObjectType += 2;

	while (*pObjectType)
	{
		//得到各类型的数组中的下标
		if (!wcscmp((*(PUNICODE_STRING)((char*)(*pObjectType) + 0x10)).Buffer, L"Directory")) {
			dirType = *pObjectType;
			NT::ArrObjectType[ObjectType::DirectoryType].TypeIndex = dirType->Index;
			NT::ArrObjectType[ObjectType::DirectoryType].ObjectType = dirType;
		}
		else if (!wcscmp((*(PUNICODE_STRING)((char*)(*pObjectType) + 0x10)).Buffer, L"SymbolicLink")) {
			NT::ArrObjectType[ObjectType::SymbolinkType].TypeIndex = (*pObjectType)->Index;
			NT::ArrObjectType[ObjectType::SymbolinkType].ObjectType = *pObjectType;
		}
		else if (!wcscmp((*(PUNICODE_STRING)((char*)(*pObjectType) + 0x10)).Buffer, L"Driver"))
		{
			NT::ArrObjectType[ObjectType::DriverType].TypeIndex = (*pObjectType)->Index;
			NT::ArrObjectType[ObjectType::DriverType].ObjectType = *pObjectType;
		}
		pObjectType++;
	}

	//获取根目录对象
	UNICODE_STRING dst;
	RtlInitUnicodeString(&dst, L"\\");
	NTSTATUS ntsta = ObReferenceObjectByName(&dst, 0, 0, 0, dirType, 0, 0, (PVOID*)&NT::ObpRootDirectoryObject);
	if (!NT_SUCCESS(ntsta))
		return false;
	ObDereferenceObject(NT::ObpRootDirectoryObject);

	return true;
}

BOOLEAN InitPsLoadedModuleList()
{
	PLIST_ENTRY start = (PLIST_ENTRY)NT::DriverObject->DriverSection;
	PLIST_ENTRY next = start->Flink;

	while (start != next)
	{
		if ((PVOID)next >= NT::ImageBaseRunNt && next < PTR_ADD_OFFSET(NT::ImageBaseRunNt, NT::SizeOfNtImage))
		{
			NT::PsLoadedModuleList = next;
			return true;
		}
		next = next->Flink;
	}




	return BOOLEAN();
}

BOOLEAN InitPspTerminateThreadByPointer()
{
	ZydisDecoder			decoder;
	ZydisFormatter			formatter;
	ULONG_PTR			curDisAddr;
	const ZyanUSize length = 100;
	ZydisDecodedInstruction instruction;


	curDisAddr = (ULONG_PTR)ArkGetSystemRoutineAddress(L"PsTerminateSystemThread");
	if (!curDisAddr)
	{
		return false;
	}

	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);


	for (int i = 0; i < length; i += instruction.length)
	{
		if (!ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)curDisAddr, length - instruction.length, &instruction)))
			break;


		if ((GETDWORD(curDisAddr) & 0xFF) == 0xE8)
		{
			ULONG offset = GETDWORD(curDisAddr + 1);
			LARGE_INTEGER dstAddr;
			dstAddr.QuadPart = (LONGLONG)curDisAddr + instruction.length;
			dstAddr.LowPart += offset;


			NT::PspTerminateThreadByPointer = (FunPspTerminateThreadByPointer)dstAddr.QuadPart;
			return true;
		}
		curDisAddr += instruction.length;
	}

	return false;
}

BOOLEAN InitKiInsertQueueApc()
{
	ZydisDecoder			decoder;
	ZydisFormatter			formatter;
	ULONG_PTR			curDisAddr;
	const ZyanUSize length = 0x140;
	ZydisDecodedInstruction instruction;


	curDisAddr = (ULONG_PTR)ArkGetSystemRoutineAddress(L"KeInsertQueueApc");
	if (!curDisAddr)
	{
		return false;
	}

	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);


	for (int i = 0; i < length; i += instruction.length)
	{
		if (!ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)curDisAddr, length - instruction.length, &instruction)))
			break;


		if ((GETDWORD(curDisAddr) & 0xFF) == 0xE8)
		{
			ULONG offset = GETDWORD(curDisAddr + 1);
			LARGE_INTEGER dstAddr;
			dstAddr.QuadPart = (LONGLONG)curDisAddr + instruction.length;
			dstAddr.LowPart += offset;


			NT::KiInsertQueueApc = (FunKiInsertQueueApc)dstAddr.QuadPart;
			return true;
		}
		curDisAddr += instruction.length;
	}

	return false;
	
}

BOOLEAN InitPsGetNextProcess()
{
	ZydisDecoder			decoder;
	ZydisFormatter			formatter;
	ULONG_PTR			curDisAddr;
	const ZyanUSize length = 0x300;
	ZydisDecodedInstruction instruction;


	curDisAddr = (ULONG_PTR)ArkGetServiceAddressByIndex(206);
	if (!curDisAddr)
	{
		return false;
	}

	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);


	for (int i = 0; i < length; i += instruction.length)
	{
		if (!ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)curDisAddr, length - instruction.length, &instruction)))
			break;


		if ((GETDWORD(curDisAddr) & 0xFF) == 0xE8 && ((GETDWORD(curDisAddr + instruction.length) & 0xffffff) == 0xf88b48))
		{
			ULONG offset = GETDWORD(curDisAddr + 1);
			LARGE_INTEGER dstAddr;
			dstAddr.QuadPart = (LONGLONG)curDisAddr + instruction.length;
			dstAddr.LowPart += offset;


			NT::PsGetNextProcess = (FunPsGetNextProcess)dstAddr.QuadPart;
			return true;
		}
		curDisAddr += instruction.length;
	}

	return false;
}

BOOLEAN InitPspCreateProcessNotifyRoutine()
{
	ZydisDecoder			decoder;
	ZydisFormatter			formatter;
	ULONG_PTR			curDisAddr;
	const ZyanUSize length = 0x140;
	ZydisDecodedInstruction instruction;

	int tempoffset;

	curDisAddr = (ULONG_PTR)ArkGetSystemRoutineAddress(L"PsSetCreateProcessNotifyRoutine");
	tempoffset = *(int*)(curDisAddr + 4);
	*(int*)&curDisAddr = LODWORD(curDisAddr) + 8 + tempoffset;
	if (!curDisAddr)
	{
		return false;
	}

	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);


	for (int i = 0; i < length; i += instruction.length)
	{
		if (!ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)curDisAddr, length - instruction.length, &instruction)))
			break;


		if (instruction.length == 7 && ((GETDWORD(curDisAddr) & 0xFFFFFF) == 0x358d4c))
		{
			ULONG offset = GETDWORD(curDisAddr + 3);
			LARGE_INTEGER dstAddr;
			dstAddr.QuadPart = (LONGLONG)curDisAddr + instruction.length;
			dstAddr.LowPart += offset;


			NT::PspCreateProcessNotifyRoutine = (PVOID*)dstAddr.QuadPart;
			return true;
		}
		curDisAddr += instruction.length;
	}

	return false;

}


BOOLEAN InitPspCreateThreadNotifyRoutine()
{
	ZydisDecoder			decoder;
	ZydisFormatter			formatter;
	ULONG_PTR			curDisAddr;
	const ZyanUSize length = 0x140;
	ZydisDecodedInstruction instruction;

	int tempoffset;

	curDisAddr = (ULONG_PTR)ArkGetSystemRoutineAddress(L"PsSetCreateThreadNotifyRoutine");

	if (!curDisAddr)
	{
		return false;
	}

	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);


	for (int i = 0; i < length; i += instruction.length)
	{
		if (!ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)curDisAddr, length - instruction.length, &instruction)))
			break;


		if (instruction.length == 7 && ((GETDWORD(curDisAddr) & 0xFFFFFF) == 0x0d8d48))
		{
			ULONG offset = GETDWORD(curDisAddr + 3);
			LARGE_INTEGER dstAddr;
			dstAddr.QuadPart = (LONGLONG)curDisAddr + instruction.length;
			dstAddr.LowPart += offset;


			NT::PspCreateThreadNotifyRoutine = (PVOID*)dstAddr.QuadPart;
			return true;
		}
		curDisAddr += instruction.length;
	}

	return false;

}

BOOLEAN InitPspLoadImageNotifyRoutine()
{

	ZydisDecoder			decoder;
	ZydisFormatter			formatter;
	ULONG_PTR			curDisAddr;
	const ZyanUSize length = 0x140;
	ZydisDecodedInstruction instruction;

	int tempoffset;

	curDisAddr = (ULONG_PTR)ArkGetSystemRoutineAddress(L"PsSetLoadImageNotifyRoutine");

	if (!curDisAddr)
	{
		return false;
	}

	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);


	for (int i = 0; i < length; i += instruction.length)
	{
		if (!ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)curDisAddr, length - instruction.length, &instruction)))
			break;


		if (instruction.length == 7 && ((GETDWORD(curDisAddr) & 0xFFFFFF) == 0x0d8d48))
		{
			ULONG offset = GETDWORD(curDisAddr + 3);
			LARGE_INTEGER dstAddr;
			dstAddr.QuadPart = (LONGLONG)curDisAddr + instruction.length;
			dstAddr.LowPart += offset;


			NT::PspLoadImageNotifyRoutine = (PVOID*)dstAddr.QuadPart;
			return true;
		}
		curDisAddr += instruction.length;
	}

	return false;
}

BOOLEAN InitCallbackListHead()
{

	ZydisDecoder			decoder;
	ZydisFormatter			formatter;
	ULONG_PTR			curDisAddr;
	const ZyanUSize length = 0x140;
	ZydisDecodedInstruction instruction;

	int tempoffset;

	curDisAddr = (ULONG_PTR)ArkGetSystemRoutineAddress(L"CmUnRegisterCallback");

	if (!curDisAddr)
	{
		return false;
	}

	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);


	for (int i = 0; i < length; i += instruction.length)
	{
		int cnt = 0;

		if (!ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)curDisAddr, length - instruction.length, &instruction)))
			break;


		if (instruction.length == 5 && ((GETDWORD(curDisAddr) & 0xFFFFFF) == 0x548d48))
		{
			curDisAddr += instruction.length;
			ULONG offset = GETDWORD(curDisAddr  + 3);
			LARGE_INTEGER dstAddr;
			dstAddr.QuadPart = (LONGLONG)curDisAddr + 7;
			dstAddr.LowPart += offset;


			NT::CallbackListHead = (PLIST_ENTRY)dstAddr.QuadPart;
			return true;
		}
		curDisAddr += instruction.length;
	}

	return false;
}

#define SHADOW_SSDT  0x1000
BOOLEAN InitNtServiceByIndex()
{
	NT::NtSuspendThread = (FunNtSuspendThread)ArkGetServiceAddressByIndex(0x17b);
	NT::NtResumeThread = (FunNtSuspendThread)ArkGetServiceAddressByIndex(79);
	NT::NtUserBuildHwndList = (FunNtUserBuildHwndList)ArkGetServiceAddressByIndex(28 | SHADOW_SSDT);
	NT::NtUserQueryWindow = (FunNtUserQueryWindow)ArkGetServiceAddressByIndex(16 | SHADOW_SSDT);
	NT::NtTerminateProcess = (FunNtTerminateProcess)ArkGetServiceAddressByIndex(NT::ServiceSerial::NtTerminateProcess);







	return true;
}

BOOLEAN InitKeServiceDescriptorTable()
{
	ZydisDecoder			decoder;
	ZydisFormatter			formatter;
	ULONG_PTR			curDisAddr;
	const ZyanUSize length = 100;
	ZydisDecodedInstruction instruction;


	curDisAddr = (ULONG_PTR)ArkGetSystemRoutineAddress(L"KeAddSystemServiceTable");
	if (!curDisAddr)
	{
		return false;
	}



	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);


	for (int i = 0; i < length; i += instruction.length)
	{
		if (!ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)curDisAddr, length - instruction.length, &instruction)))
			break;


		if ((GETDWORD(curDisAddr) & 0xFFFFFF) == 0x1D8D4C && *(USHORT*)(curDisAddr + 11) == 0x834B)
		{
			ULONG offset = GETDWORD(curDisAddr + 15);
			LARGE_INTEGER dstAddr;
			dstAddr.QuadPart = (LONGLONG)NT::ImageBaseRunNt;
			dstAddr.LowPart += offset;


			NT::KeServiceDescriptorTable = (PKSERVICE_TABLE_DESCRIPTOR)dstAddr.QuadPart;
			NT::KeServiceDescriptorTableShadow = (PKSERVICE_TABLE_DESCRIPTOR)(dstAddr.QuadPart + sizeof(KSERVICE_TABLE_DESCRIPTOR)*2);

			return true;
		}
		curDisAddr += instruction.length;
	}

	return false;
}



