#pragma once
class kernelhook
{
public:
	kernelhook();
	~kernelhook();
};



/*
得到SSDT表的函数地址
*/
BOOLEAN ArkGetAllShadowSdtFunAddr(PVOID, PVOID, PVOID *FunAddr, ULONG ReturnSize);

BOOLEAN ArkTestHook();




