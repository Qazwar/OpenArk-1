#include "kark.h"
#include <intrin.h>

//关闭写保护
KIRQL WPOFFx64()
{
	_disable();
	KIRQL irql = 0;// = KeRaiseIrqlToDpcLevel();
	UINT64 cr0 = __readcr0();
	cr0 &= 0xfffffffffffeffff;
	__writecr0(cr0);
	return irql;
}
//开启写保护
void WPONx64(KIRQL irql)
{
	UINT64 cr0 = __readcr0();
	cr0 |= 0x10000;
	__writecr0(cr0);
	//KeLowerIrql(irql);
	_enable();
}

kernelhook::kernelhook()
{
}


kernelhook::~kernelhook()
{
}

PVOID NtTerminateTrucateAddr;
UCHAR OldOpcode[MAX_PATH] = {
	0x48,0xb8,0,0,0,0, 0,0,0,0,
	0xff,0xe0
};


EXTERN_C void FakeTerminate();
BOOLEAN ArkGetAllShadowSdtFunAddr(PVOID, PVOID, PVOID * FunAddr, ULONG ReturnSize)
{
	ULONG funLimit;
	PEPROCESS csrss = 0;
	PLONG offsetTable = 0;
	long offset;
	KAPC_STATE apcState;


	funLimit = NT::KeServiceDescriptorTableShadow[1].Limit;
	ProbeForWrite(FunAddr, funLimit * sizeof(PVOID), 8);
	offsetTable = (PLONG)ExAllocatePool(PagedPool, funLimit * sizeof(PVOID));
	csrss = ArkGetProcess("csrss.exe");


	if (csrss)
	{
		KeStackAttachProcess(csrss, &apcState);
		RtlCopyMemory(offsetTable, NT::KeServiceDescriptorTableShadow[1].Base, funLimit * sizeof(PVOID));
		KeUnstackDetachProcess(&apcState);
		

		for (ULONG i = 0; i < funLimit; i++)
		{
			offset = offsetTable[i] >> 4;
			FunAddr[i] = PTR_ADD_OFFSET(NT::KeServiceDescriptorTableShadow[1].Base, offset);
		}


	}
	ExFreePool(offsetTable);
	return true;
}
BOOLEAN ArkTestHook()
{
	
	PVOID ntTerminateProcess;
	PVOID dstAddr = (PVOID)&FakeTerminate;

	ntTerminateProcess = ArkGetServiceAddressByIndex(41);

	WPOFFx64();
	memmove(OldOpcode + 2, &dstAddr, 8);
	memcpy(ntTerminateProcess, OldOpcode, 12);
	WPONx64(0);

	return BOOLEAN();
}
