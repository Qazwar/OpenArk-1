#pragma once
class window
{
public:
	window();
	~window();
};


typedef
NTSTATUS
(*FunNtUserBuildHwndList)(
	PVOID hDesktop,
	PVOID hwndParent,
	BOOLEAN bChildren,
	ULONG dwThreadId,
	ULONG lParam,
	PVOID pWnd,
	PULONG HwndCnt);

typedef
PVOID
(*FunNtUserQueryWindow)
(PVOID hWnd, DWORD Index);

/*
得到指定进程的所有窗口句柄
*/
BOOLEAN ArkGetWindowsForProcess(
	PVOID Param,
	ULONG ParamLen,
	ArkWindowInfo *Windows,
	ULONG ReturnLen);