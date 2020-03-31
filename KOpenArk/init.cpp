
#define _DYNDATA_PRIVATE
#include "kark.h"
#include "Init.h"
#include "Zydis/Zydis.h"
#include <ntimage.h>
/*
zydis Ҫ����Ԥ����������  ������lib��û�е�����
����Ԥ��������ֹ���Ʋ��Գ�


*/
DrvCallFun  DrvCallTable[DrvCall::LastId];

namespace NT {

	WCHAR				NtFullName[NAMELEN];
}

BOOLEAN InitVarible()
{
	//........................................................................................................................................��ʼ��ntkrnl����
	if (!InitExportByNtkrnl())
		return FALSE;

	//........................................................................................................................................//��ʼ��δ��������
	if (!InitUnExportByNtkrnl())
		return FALSE;

	//........................................................................................................................................��ʼ��Eprocess�������
	if (!NT::OsVersion)
		return FALSE;

	return TRUE;
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
1.ͨ�����뵼�����NtBuildNumber��������ģ�������ҵ�ntoskrnlģ��ĵ�ַ
2.����ntoskenl�ļ�,�����ض�λ��ʼ����ر���
*/
BOOLEAN InitNtInfo()
{
	__try
	{
		PLDR_DATA_TABLE_ENTRY  pFlink;
		PVOID pNtBuildNumber = &NtBuildNumber;
		NT::OsVersion = (USHORT)NtBuildNumber;
		pFlink = (PLDR_DATA_TABLE_ENTRY)((PLDR_DATA_TABLE_ENTRY)NT::DriverObject->DriverSection)->InLoadOrderLinks.Flink;

		while (pFlink != (PLDR_DATA_TABLE_ENTRY)NT::DriverObject->DriverSection)
		{
			if (pNtBuildNumber > pFlink->DllBase && (ULONG_PTR)pNtBuildNumber < (ULONG_PTR)pFlink->DllBase + pFlink->SizeOfImage)
			{
				NT::ImageBaseRunNt = (PCHAR)pFlink->DllBase;
				NT::SizeOfNtImage = pFlink->SizeOfImage;
				memmove(NT::NtFullName, pFlink->FullDllName.Buffer, pFlink->FullDllName.Length);

				HANDLE hfile;
				OBJECT_ATTRIBUTES objAttribute;
				UNICODE_STRING objName;
				NTSTATUS ntstatu;

				RtlInitUnicodeString(&objName, L"\\??\\c:/windows/system32/ntoskrnl.exe");
				IO_STATUS_BLOCK ioStatuBlock;
				/* Initialize the object attributes */
				InitializeObjectAttributes(&objAttribute,
					&objName,
					OBJ_CASE_INSENSITIVE,
					NULL,
					NULL);
				ntstatu = ZwCreateFile(&hfile, GENERIC_READ, &objAttribute, &ioStatuBlock, 0, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN, 0, 0, 0);
				if (NT_SUCCESS(ntstatu)) {
					FILE_STANDARD_INFORMATION fileInfo;

					if (NT_SUCCESS(ntstatu)) {
						ZwQueryInformationFile(hfile, &ioStatuBlock, &fileInfo, sizeof(fileInfo), FileStandardInformation);

						PCHAR pbuf = (PCHAR)ExAllocatePool(PagedPool, fileInfo.AllocationSize.QuadPart);
						if (pbuf) {
							ntstatu = ZwReadFile(hfile, 0, 0, 0, &ioStatuBlock, pbuf, fileInfo.AllocationSize.LowPart, 0, 0);
							if (NT_SUCCESS(ntstatu)) {
								ULONG_PTR ImageBase = ((PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET(NT::ImageBaseCopyNt))->ImageBase;
								PeFixRelocTable(pbuf, (ULONG)((ULONG_PTR)NT::ImageBaseRunNt - ImageBase));

								NT::ImageBaseCopyNt = pbuf;
							}
							else {
								ExFreePool(pbuf);
							}
						}
					}
				}
				break;
			}
		}
	}
	__except (1)
	{
		DBGERRINFO;
	}


	return BOOLEAN();
}

BOOLEAN InitExportByNtkrnl()
{
	LOADEXPORT(PsLookupProcessByProcessId)
	LOADEXPORT(IoFileObjectType)
	



	return TRUE;
}
BOOLEAN InitUnExportByNtkrnl()
{
	LOADUNEXPORT(InitPspCidTable);
	LOADUNEXPORT(InitObTypeIndexTable);

	return TRUE;
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

		KTHREAD::ProcessOffset = 0x210;
	}

	default:
		return  0;
		break;
	}





	return FALSE;
}
BOOLEAN InitPspCidTable()
{
	ZydisDecoder			decoder;								//������
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
			return TRUE;
		}
		curDisAddr += instruction.length;
	}

	return FALSE;
}
/*
1.��ʼ��˳��
	1.1  �ȳ�ʼ��nt�Ļ�����Ϣ
	1.2  ��ʼ����������
	1.3  ��ʼ���汾�����offset
	1.4  ��ʼ����������
	*/
BOOLEAN InitDriver()
{
	BOOLEAN result = FALSE;
	result = InitNtInfo();
	if (!result)
		return FALSE;

	result = InitVarible();
	if (!result)
		return FALSE;

	result = InitOffset();
	if (!result)
		return FALSE;


	result = InitSysCallTable();
	if (!result)
		return FALSE;

	return TRUE;
}

BOOLEAN InitSysCallTable()
{
	DrvCallTable[ProcList] = (DrvCallFun)ArkGetProcList;
	DrvCallTable[ObjDirectory] = (DrvCallFun)ArkGetObjectDirectoryInfo;
	DrvCallTable[HideMod] = (DrvCallFun)ArkHideMod;
	DrvCallTable[HideProcess] = (DrvCallFun)ArkHideProcess;
	DrvCallTable[ModList] = (DrvCallFun)ArkGetModListForProc;

	return FALSE;
}




BOOLEAN InitObTypeIndexTable()
{
	ZydisDecoder			decoder;								//������
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
		//�õ������͵������е��±�
		if (!wcscmp((*(PUNICODE_STRING)((char*)(*pObjectType) + 0x10)).Buffer, L"Directory")) {
			dirType = *pObjectType;
			NT::ArrObjectType[ObjectType::DirectoryType].TypeIndex = dirType->Index;
		}
		else if (!wcscmp((*(PUNICODE_STRING)((char*)(*pObjectType) + 0x10)).Buffer, L"SymbolicLink")) {
			NT::ArrObjectType[ObjectType::SymbolinkType].TypeIndex = (*pObjectType)->Index;
		}
		pObjectType++;
	}

	//��ȡ��Ŀ¼����
	UNICODE_STRING dst;
	RtlInitUnicodeString(&dst, L"\\");
	NTSTATUS ntsta = ObReferenceObjectByName(&dst, 0, 0, 0, dirType, 0, 0, (PVOID*)&NT::ObpRootDirectoryObject);
	if (!NT_SUCCESS(ntsta))
		return FALSE;
	ObDereferenceObject(NT::ObpRootDirectoryObject);

	return TRUE;
}



