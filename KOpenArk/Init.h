#ifndef INIT_H
#define INIT_H


typedef BOOLEAN(*DrvCallFun)(PCHAR pIndata, ULONG cbInData, PCHAR pOutData, ULONG cbOutData);
extern DrvCallFun  DrvCallTable[DrvCall::LastId];//���ñ�





BOOLEAN InitDrvCallTable();

BOOLEAN InitVarible();
BOOLEAN CheckVersion();

BOOLEAN InitNtInfo();

//��ʼ��ntkrnol�����ı���
BOOLEAN InitExportByNtkrnl();

BOOLEAN InitDriver();

BOOLEAN InitUnExportByNtkrnl();
BOOLEAN InitOffset();

//��ʼ��δ��������
BOOLEAN InitPspCidTable();
BOOLEAN InitObTypeIndexTable();
BOOLEAN InitPsLoadedModuleList();
/*

��ʼ��δ������ϵͳ����
*/
BOOLEAN InitNtServiceByIndex();
BOOLEAN InitKeServiceDescriptorTable();//��ʼ��ϵͳ�����

#define NTKROSPATH  L"\\??\\c:\\windows\\system32\\ntoskrnl.exe"
 #define LOADEXPORT(name) \
    *((void**)&NT::##name)=( rva = PeGetProcRva(NT::ImageBaseCopyNt, #name)) == 0 ? 0 :rva + NT::ImageBaseRunNt; \
    if(!NT::##name) \
    { \
        DbgPrint("Export %s could not be found!", #name); \
		return false;		\
    }

#define LOADUNEXPORT(name)   if(!name())  return false;
		
#endif // !INIT_H