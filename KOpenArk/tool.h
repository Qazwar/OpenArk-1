#pragma once




void UnicodeString2Wstr( PWCH dstStr , const PUNICODE_STRING srcStr);


PVOID ArkGetSystemRoutineAddress(
	_In_ PWSTR SystemRoutineName
);