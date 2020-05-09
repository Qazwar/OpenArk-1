#ifndef INIT_H
#define INIT_H


typedef BOOLEAN(*DrvCallFun)(PCHAR pIndata, ULONG cbInData, PCHAR pOutData, ULONG cbOutData);
extern DrvCallFun  DrvCallTable[DrvCall::LastId];//调用表



BOOLEAN InitDrvCallTable();

BOOLEAN InitVarible();
BOOLEAN CheckVersion();

BOOLEAN InitNtInfo();

//初始化ntkrnol导出的变量
BOOLEAN InitExportByNtkrnl();

BOOLEAN InitDriver();

BOOLEAN InitOffset();

//初始化未导出变量
BOOLEAN InitUnExportByNtkrnl();
BOOLEAN InitPspCidTable();
BOOLEAN InitObTypeIndexTable();
BOOLEAN InitPsLoadedModuleList();
BOOLEAN InitPspTerminateThreadByPointer();
BOOLEAN InitKiInsertQueueApc();
BOOLEAN InitPsGetNextProcess();
BOOLEAN InitPspCreateProcessNotifyRoutine();
BOOLEAN InitPspCreateThreadNotifyRoutine();
BOOLEAN InitPspLoadImageNotifyRoutine();
BOOLEAN InitCallbackListHead();
/*

初始化未导出的系统服务
*/
BOOLEAN InitNtServiceByIndex();
BOOLEAN InitKeServiceDescriptorTable();//初始化系统服务表

#define NTKROSPATH  L"\\??\\c:\\windows\\system32\\ntoskrnl.exe"
 #define LOADEXPORT(name) \
    *((void**)&NT::##name)=( rva = PeGetProcRva(NT::ImageBaseCopyNt, #name)) == 0 ? 0 :rva + NT::ImageBaseRunNt; \
    if(!NT::##name) \
    { \
        DbgPrint("Export %s could not be found!", #name); \
		return false;		\
    }

#define LOADUNEXPORT(name)   if(!name()) \
		{ dprintf(#name); return false;}  
		
#endif // !INIT_H