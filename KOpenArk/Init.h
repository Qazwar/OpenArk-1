#ifndef INIT_H
#define INIT_H


typedef BOOLEAN(*DrvCallFun)(PCHAR pIndata, ULONG cbInData, PCHAR pOutData, ULONG cbOutData);
extern DrvCallFun  DrvCallTable[DrvCall::LastId];//调用表
BOOLEAN InitDrvCallTable();

BOOLEAN InitVarible();
BOOLEAN CheckVersion();

BOOLEAN InitNtInfo();
BOOLEAN InitExportByNtkrnl();

BOOLEAN InitDriver();
BOOLEAN InitExportByNtkrnl();
BOOLEAN InitUnExportByNtkrnl();
BOOLEAN InitOffset();

//初始化未导出变量
BOOLEAN InitPspCidTable();
BOOLEAN InitObTypeIndexTable();
BOOLEAN InitPsLoadedModuleList();

#define NTKROSPATH  L"\\??\\c:\\windows\\system32\\ntoskrnl.exe"
 #define LOADEXPORT(name) \
    *((void**)&NT::##name)=PeGetProcRva(NT::ImageBaseCopyNt, #name) + NT::ImageBaseRunNt; \
    if(!NT::##name) \
    { \
        DbgPrint("Export %s could not be found!", #name); \
		return false;		\
    }

#define LOADUNEXPORT(name)   if(!name())  return false;
		
#endif // !INIT_H