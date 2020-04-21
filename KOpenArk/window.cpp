#include "kark.h"



window::window()
{
}


window::~window()
{
}

BOOLEAN ArkGetWindowsForProcess(PVOID Param, ULONG ParamLen, ArkWindowInfo *OutBuf, ULONG ReturnLen)
{
	ULONG hwndCnt = 0;
	HWND *handles;
	NTSTATUS st;
	ArkWindowInfo *hwndInfo;

	hwndInfo = OutBuf + 1;
	handles = (HWND *)ExAllocatePool(PagedPool, 4000*8);

	if (handles)
	{
		st = NT::NtUserBuildHwndList(0, 0, 0, 0, 4000, hwndInfo, (PULONG)OutBuf);
		if (NT_SUCCESS(st))
		{
			hwndCnt = *(PULONG)OutBuf;
			RtlCopyMemory(handles, hwndInfo, hwndCnt * 8);
			for (ULONG i = 0; i < hwndCnt; i++,OutBuf++)
			{
				OutBuf->Hwnd = handles[i];
				OutBuf->ProcessId = NT::NtUserQueryWindow(handles[i], 0);
			}
		}
		ExFreePool(handles);
	}

	dprintf("leaving ArkGetWindowsForProcess %d",hwndCnt);
	return true;
}
