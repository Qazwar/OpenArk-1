#include "tool.h"

void UnicodeString2Wstr(PWCH dstStr, const PUNICODE_STRING srcStr)
{
	memmove(dstStr, srcStr->Buffer, srcStr->Length);
	dstStr[srcStr->Length / 2] = 0;
}
