EDG C/C++ version 4.14 (Sep  4 2019 14:47:28)
    /   E:\ItemFile\驱动\processhacker\ProcessHacker 0   E:\ITEMFILE\驱动\PROCESSHACKER\PROCESSHACKER\          _MSC_EXTENSIONS            7                  :                  8       1916            A      16                  _MSC_VER=1916                  _MSC_FULL_VER=191627034                  _MSC_BUILD=0                  _M_AMD64=100                  _M_X64=100                  _WIN64                  _WIN32                                                      _EDG_COMPILER                  _USE_DECLSPECS_FOR_SAL=1               .   E:\ItemFile\驱动\processhacker\phnt\include               /   E:\ItemFile\驱动\processhacker\phlib\include                  include                  _PHLIB_                  _PHAPP_               	   _WINDOWS                  HAVE_CONFIG_H                  WIN64                  _DEBUG                  DEBUG               	   _UNICODE                  UNICODE                  _DEBUG                  _MT                  _M_FP_PRECISE            f   =   E:\ItemFile\驱动\processhacker\ProcessHacker\obj\Debug64\\               `   C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\include               g   C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\atlmfc\include               V   C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\VS\include               A   C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\ucrt               _   C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\VS\UnitTest\include               ?   C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\um               C   C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\shared               B   C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\winrt               E   C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\cppwinrt                  Include\um            �   g   C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\atlmfc\lib\x64            �   `   C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\lib\x64            �   `   C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\lib\x64            �   g   C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\atlmfc\lib\x64            �   V   C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\VS\lib\x64            �   A   C:\Program Files (x86)\Windows Kits\10\lib\10.0.17763.0\ucrt\x64            �   [   C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\VS\UnitTest\lib            �   ?   C:\Program Files (x86)\Windows Kits\10\lib\10.0.17763.0\um\x64            �      lib\um\x64            �      Lib\um\x64            �   /   E:\ItemFile\驱动\processhacker\ProcessHacker            3   U   E:\ItemFile\驱动\processhacker\.vs\ProcessHacker\v15\ipch\AutoPCH\e7ebe1af8c10e26b            �      no_ms_nonreal_base_classes            �                         
   <phapp.h>                <symprv.h>                <hndlprv.h>                <mainwnd.h>                <memprv.h>                <modprv.h>                <netprv.h>                <phappres.h>                 <phsettings.h> !               <procprv.h> "               <srvprv.h> #               <thrdprv.h> $               <uxtheme.h> &          ?   E:/ItemFile/驱动/processhacker/ProcessHacker/include/phapp.h �x^    4   E:\ItemFile\驱动\processhacker\phlib\include\ph.h �x^    8   E:\ItemFile\驱动\processhacker\phlib\include\phbase.h �x^    =   E:\ItemFile\驱动\processhacker\phnt\include\phnt_windows.h �x^    I   C:\Program Files (x86)\Windows Kits\10\IncbOffset = 0x338;
		EPROCESS::ActiveProcessLinksOffset = 0x188;
		EPROCESS::VadRootOffset = 0x448;
		KTHREAD::ProcessOffset = 0x210;
		break;
	}

	default:
		return  0;
		break;
	}

	



	return TRUE;
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
		if (! ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)curDisAddr, length - instruction.length, &instruction)))
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
	if(!result)
		return FALSE;

	return TRUE;
}



BOOLEAN InitSysCallTable()
{
	g_SysCallTable[ProcList] = (FunSysCall)ArkGetProcList;
	g_SysCallTable[ObjDirectory] = (FunSysCall)ArkGetObjectDirectoryInfo;
	g_SysCallTable[HideProcess] = (FunSysCall)ArkHideProcess;
	g_SysCallTable[ModList] = (FunSysCall)ArkGetModListForProc;
	g_SysCallTable[HideMod] = (FunSysCall)ArkHideMod;

	return TRUE;
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





