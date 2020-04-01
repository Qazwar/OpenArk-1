#pragma once


//��������
//����

void ExEnumHandleTable(PHANDLE_TABLE pHandleTable, StuProcInfo *pOutData, ULONG cbOutData, EX_ENUMERATE_HANDLE_ROUTINE funEnumHandleProcedure, PVOID enumParam);
ULONG_PTR  PsGetProcessIdFromHandleTable(ULONG_PTR pEprocess);
NTSTATUS  PsGetAllProcessInfomation(StuProcInfo *pOutData, ULONG cbOutData);
void  PsGetProcessInfo(ULONG_PTR pObjectBody, StuProcInfo *pOutData, ULONG cbOutData, PVOID param);
BOOLEAN  PsGetProcessPath(PEPROCESS Process, PVOID ProcId, OUT PWSTR Buffer, IN ULONG BufferLength, PULONG ReturnLength);
int  GetIdlelProcessInfomatio(StuProcInfo *pOutData, ULONG cbOutData);
void  FillProcessInfo(ULONG_PTR pEprocess, StuProcInfo *pOutData, ULONG cbOutData);
int  GetIdlelProcessInfomatio(StuProcInfo *pOutData, ULONG cbOutData);
PHANDLE_TABLE_ENTRY ExpLookupHandleTableEntry(IN PHANDLE_TABLE HandleTable,IN EXHANDLE tHandle);
BOOLEAN PsGetProcessPathByPeb(PEPROCESS process, PVOID procId, PWSTR path,_Inout_ int *pathLen);

void GetModInfoByAvlNode(PMMVAD vadNode,ModInfo *modInfo);
void TraverseAvlMid(PMMVAD vadNode,ModInfo *modInfo);

//���ú���
BOOLEAN  ArkGetProcList(PCHAR pIndata, ULONG cbInData, StuProcInfo *pOutData, ULONG cbOutData);
BOOLEAN  ArkHideProcess(PCHAR pIndata, ULONG cbInData, StuProcInfo *pOutData, ULONG cbOutData);
BOOLEAN ArkGetModListForProc(PCHAR pIndata, ULONG cbInData, ModInfo *pOutData, ULONG cbOutData);
BOOLEAN ArkHideMod(HideModParam *pIndata, ULONG cbInData, ModInfo *pOutData, ULONG cbOutData);
BOOLEAN ArkGetProcHandleInfo(PCHAR pIndata, ULONG cbInData, ModInfo *pOutData, ULONG cbOutData);






//���

EXTERN_C{

 PKPRCB GetCurrentPrcb();

}