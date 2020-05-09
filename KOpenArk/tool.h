#pragma once




void UnicodeString2Wstr( PWCH dstStr , const PUNICODE_STRING srcStr);


PVOID ArkGetSystemRoutineAddress( 
	_In_ PWSTR SystemRoutineName
);

PVOID ArkGetServiceAddressByIndex(USHORT funIndex);

int ArkGetFileNameLenFromParam(PWSTR FileName);
PWSTR ArkGetFileNameFromFullPath(PWSTR Path);
PWSTR ArkGetFileNameFromUnicodeString(UNICODE_STRING Path);
//»ã±à

EXTERN_C
{

	PKPRCB GetCurrentPrcb();

	void ArkReadGdtr(ArkGdtr *Gdt);

}