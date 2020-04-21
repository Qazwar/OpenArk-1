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

PVOID ArkGetServiceAddressByIndex(USHORT funIndex)
{
	static PEPROCESS csrss = 0;

	

	int tableIndex = funIndex >> 12;
	PLONG OffsetTable = 0;
	long offset;
	KAPC_STATE apcState;
	
	if (csrss == 0 && tableIndex == 1)
	{
		csrss = ArkGetProcess("csrss.exe");
	}


	__try
	{
		if (tableIndex)
		{
			KeStackAttachProcess(csrss, &apcState);
		}
		OffsetTable = NT::KeServiceDescriptorTableShadow[tableIndex].Base;
		offset = OffsetTable[funIndex & 0xfff] >> 4;
		if (tableIndex)
		{
			KeUnstackDetachProcess(&apcState);
		}
	}
	__except (1)
	{
		dprintf("AekGetServiceAddressByIndex fail %x",OffsetTable);
	}



	return PTR_ADD_OFFSET(OffsetTable,offset);
}
