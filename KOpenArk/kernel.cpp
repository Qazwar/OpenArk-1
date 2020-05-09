#include "kark.h"
#pragma warning (disable:4334)


kernel::kernel()
{
}


kernel::~kernel()
{
}

BOOLEAN ArkGetGdtInfo(PVOID, PVOID, ArkGdtInfo * GdtInfo, ULONG BufSize)
{
	long numOfprocessors = KeNumberProcessors;
	PROCESSOR_NUMBER  processorNumber;
	GROUP_AFFINITY affinity;
	GROUP_AFFINITY previousAffinity;
	NTSTATUS st;
	BOOLEAN retResult = 0;
	ArkGdtr gdtr;
	
	int i = 0;
	for (; i < numOfprocessors; i++)
	{
		st = KeGetProcessorNumberFromIndex(i, &processorNumber);
		if (NT_SUCCESS(st))
		{
			affinity.Group = processorNumber.Group;
			affinity.Mask = 1 << processorNumber.Number;
			KeSetSystemGroupAffinityThread(&affinity, &previousAffinity);
			retResult = true;

			//¸´ÖÆgdt±í
			ArkReadGdtr(&gdtr);
			GdtInfo->Size = gdtr.Limit + 1 + sizeof(long);
			memcpy(GdtInfo->Gdts, gdtr.Base, gdtr.Limit + 1);
			KeRevertToUserGroupAffinityThread(&previousAffinity);
			GdtInfo =(ArkGdtInfo *) ((ULONG_PTR)GdtInfo + GdtInfo->Size);
		}

	}


	return retResult;
}
