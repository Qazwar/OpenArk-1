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
�õ�ָ�����̵����д��ھ��
*/
BOOLEAN ArkGetWindowsForProcess(
	PVOID Param,
	ULONG ParamLen,
	ArkWindowInfo *Windows,
	ULONG ReturnLen);