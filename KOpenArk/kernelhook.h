#pragma once
class kernelhook
{
public:
	kernelhook();
	~kernelhook();
};



/*
�õ�SSDT��ĺ�����ַ
*/
BOOLEAN ArkGetAllShadowSdtFunAddr(PVOID, PVOID, PVOID *FunAddr, ULONG ReturnSize);

BOOLEAN ArkTestHook();




