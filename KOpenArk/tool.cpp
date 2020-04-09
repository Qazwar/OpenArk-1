#include "kark.h"

void UnicodeString2Wstr(PWCH dstStr, const PUNICODE_STRING srcStr)
{
	memmove(dstStr, srcStr->Buffer, srcStr->Length);
	dstStr[srcStr->Length / 2] = 0;
}

PVOID ArkGetSystemRoutineAddress(
	_In_ PWSTR SystemRoutineName
)
{
	UNICODE_STRING systemRoutineName;

	PAGED_CODE();

	RtlInitUnicodeString(&systemRoutineName, SystemRoutineName);

	return MmGetSystemRoutineAddress(&systemRoutineName);
}

PVOID AekGetServiceAddressByIndex(USHORT funIndex)
{
	int tableIndex = funIndex & 0x1000;
	PULONG OffsetTable;
	ULONG offset;


	OffsetTable = (PULONG)NT::KeServiceDescriptorTable[tableIndex].Base;
	offset = OffsetTable[funIndex & 0xfff] >> 4;


	return PTR_ADD_OFFSET(OffsetTable,offset);
}
