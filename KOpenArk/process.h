#pragma once


//��������
//����

void ArkEnumHandleTable(PHANDLE_TABLE pHandleTable, StuProcInfo *pOutData, ULONG cbOutData, ArkEX_ENUMERATE_HANDLE_ROUTINE funEnumHandleProcedure, PVOID enumParam);
ULONG_PTR  PsGetProcessIdFromHandleTable(ULONG_PTR pEprocess);
NTSTATUS  PsGetAllProcessInfomation(StuProcInfo *pOutData, ULONG cbOutData);
void  PsGetProcessInfo(ULONG_PTR pObjectBody, StuProcInfo *pOutData, ULONG cbOutData, PVOID param);
BOOLEAN  PsGetProcessPath(PEPROCESS Process, PVOID ProcId, OUT PWSTR Buffer, IN ULONG BufferLength, PULONG ReturnLength);
int  GetIdlelProcessInfomatio(StuProcInfo *pOutData, ULONG cbOutData);
void  FillProcessInfo(ULONG_PTR pEprocess, StuProcInfo *pOutData, ULONG cbOutData);
int  GetIdlelProcessInfomatio(StuProcInfo *pOutData, ULONG cbOutData);
PHANDLE_TABLE_ENTRY ExpLookupHandleTableEntry(IN PHANDLE_TABLE HandleTable,IN EXHANDLE tHandle);
BOOLEAN PsGetProcessPathByPeb(PEPROCESS process, PVOID procId, PWSTR path,_Inout_ int *pathLen);

void GetModInfoByAvlNode(PMMVAD vadNode,ArkModInfo *modInfo);
void TraverseAvlMid(PMMVAD vadNode, ArkModInfo *modInfo);





typedef void(*ENUM_THREAD_CALLBACK)(PETHREAD Thread, PVOID ThreadInfo);
/*
	ö��ָ�����̵������߳�
	����1 ���̵�id
	����2 �����Ļص�����
	����3 �����ص��Ļ�����
	InfoSize �� ��������С

*/
void EnumProcessThreads(PVOID ProcId, ENUM_THREAD_CALLBACK CallBackFun, ArkThreadInfo *ThreadInfo, ULONG InfoSize);


/*
�����̶߳���õ��߳���Ϣ
����1 �̶߳���
����2 ���������
*/
void CallbackGetThreadInfo(PETHREAD Thread, PVOID *ThreadInfo);


//���ú���
BOOLEAN  ArkGetProcList(PCHAR pIndata, ULONG cbInData, StuProcInfo *pOutData, ULONG cbOutData);
BOOLEAN  ArkHideProcess(PCHAR pIndata, ULONG cbInData, StuProcInfo *pOutData, ULONG cbOutData);
BOOLEAN ArkGetModListForProc(PCHAR pIndata, ULONG cbInData, ArkModInfo *pOutData, ULONG cbOutData);
BOOLEAN ArkHideMod(HideModParam *pIndata, ULONG cbInData, ArkModInfo *pOutData, ULONG cbOutData);
BOOLEAN ArkGetProcHandleInfo(PCHAR pIndata, ULONG cbInData, ArkHandleInfo *pOutData, ULONG cbOutData);
BOOLEAN ArkGetProcHandles(PCHAR pIndata, ULONG cbInData, ArkHandleInfo *pOutData, ULONG cbOutData);
BOOLEAN ArkGetProcThreads(PCHAR pIndata, ULONG cbInData, ArkThreadInfo *pOutData, ULONG cbOutData);//�õ����̵������߳���Ϣ

/*
�õ�ϵͳģ��
*/
BOOLEAN 
ArkGetSystemModInfo
(
	PCHAR pIndata,
	ULONG cbInData,
	ArkModInfo *pOutData,
	ULONG cbOutData
);



//���

EXTERN_C{

 PKPRCB GetCurrentPrcb();

}