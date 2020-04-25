#pragma once
class kernel
{
public:
	kernel();
	~kernel();
};


BOOLEAN ArkGetGdtInfo(PVOID, PVOID, ArkGdtInfo* GdtInfo, ULONG BufSize);