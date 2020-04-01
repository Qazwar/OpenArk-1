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
