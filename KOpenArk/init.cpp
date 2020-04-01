
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
	__try
	{
		PLDR_DATA_TABLE_ENTRY  pFlink;
		PVOID vaNtBuildNumber = &NtBuildNumber;
		NT::OsVersion = (USHORT)NtBuildNumber;
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
	LOADEXPORT(PsLookupProcessByProcessId)
	LOADEXPORT(IoFileObjectType)
	



	return true;
}
BOOLEAN InitUnExportByNtkrnl()
{
	LOADUNEXPORT(InitPspCidTable);
	LOADUNEXPORT(InitObTypeIndexTable);

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
	DrvCallTable[ProcHandleList] = (DrvCallFun)ArkGetProcHandleInfo;

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
		}
		else if (!wcscmp((*(PUNICODE_STRING)((char*)(*pObjectType) + 0x10)).Buffer, L"SymbolicLink")) {
			NT::ArrObjectType[ObjectType::SymbolinkType].TypeIndex = (*pObjectType)->Index;
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



