#include "kark.h"


syscallback::syscallback()
{
}


syscallback::~syscallback()
{
}

BOOLEAN ArkGetCallBackInfo(PVOID, PVOID, ArkSysCallBack * CallBackInfo, ULONG BufSize)
{
	__try
	{
		ProbeForWrite(CallBackInfo, BufSize, 4);
		PVOID *tempNotifyRoutine = NT::PspCreateProcessNotifyRoutine;
		
		//���̴����ص�
		for (int i = 0; i < 64; i++, tempNotifyRoutine++)
		{
			PVOID callbakRecord = *tempNotifyRoutine;
			if (callbakRecord)
			{
				CallBackInfo->Type = ArkSysCallBack::CreateProcessType;
				CallBackInfo->CallBackRoutine = *(PVOID*)PTR_ADD_OFFSET(((ULONG_PTR)callbakRecord & ~0xf), 8);
				CallBackInfo++;
			}
		}
		//�̴߳����ص�
		tempNotifyRoutine = NT::PspCreateThreadNotifyRoutine;
		for (int i = 0; i < 64; i++, tempNotifyRoutine++)
		{
			PVOID callbakRecord = *tempNotifyRoutine;
			if (callbakRecord)
			{
				CallBackInfo->Type = ArkSysCallBack::CreateThread;
				CallBackInfo->CallBackRoutine = *(PVOID*)PTR_ADD_OFFSET(((ULONG_PTR)callbakRecord & ~0xf), 8);
				CallBackInfo++;
			}
		}

		//����ģ��ص�
		tempNotifyRoutine = NT::PspLoadImageNotifyRoutine;
		for (int i = 0; i < 8; i++, tempNotifyRoutine++)
		{
			PVOID callbakRecord = *tempNotifyRoutine;
			if (callbakRecord)
			{
				CallBackInfo->Type = ArkSysCallBack::LoadImageType;
				CallBackInfo->CallBackRoutine = *(PVOID*)PTR_ADD_OFFSET(((ULONG_PTR)callbakRecord & ~0xf), 8);
				CallBackInfo++;
			}
		}

		//ע���ص�
		PLIST_ENTRY tempList = NT::CallbackListHead;
		for (PLIST_ENTRY nextList = tempList->Flink; nextList != NT::CallbackListHead; nextList = nextList->Flink)
		{
			CallBackInfo->Type = ArkSysCallBack::CmpCallback;
			CallBackInfo->CallBackRoutine = PTR_ADD_OFFSET(nextList,0x28);
			CallBackInfo++;
		}

	}
	__except (1)
	{
		return false;
	}



	return true;
}
