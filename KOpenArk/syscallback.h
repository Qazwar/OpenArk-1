#pragma once
class syscallback
{
public:
	syscallback();
	~syscallback();
};




BOOLEAN ArkGetCallBackInfo(PVOID, PVOID,
	ArkSysCallBack *CallBackInfo,
	ULONG BufSize);
