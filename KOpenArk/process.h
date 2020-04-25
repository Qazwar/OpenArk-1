#pragma once
class process
{
public:
	process();
	~process();
};

//函数声明
//进程

void ArkEnumHandleTable
(PHANDLE_TABLE pHandleTable, 
	StuProcInfo *pOutData, 
	ULONG cbOutData, 
	ArkEX_ENUMERATE_HANDLE_ROUTINE funEnumHandleProcedure, 
	PVOID enumParam);

ULONG_PTR  
PsGetProcessIdFromHandleTable
(ULONG_PTR pEprocess);

NTSTATUS  
PsGetAllProcessInfomation
(StuProcInfo *pOutData, 
	ULONG cbOutData);


void  PsGetProcessInfo
(ULONG_PTR pObjectBody,
	StuProcInfo *pOutData, 
	ULONG cbOutData, 
	PVOID param);


BOOLEAN  
PsGetProcessPath(PEPROCESS Process,
	PVOID ProcId, 
	OUT PWSTR Buffer,
	IN ULONG BufferLength,
	PULONG ReturnLength);


int  GetIdlelProcessInfomatio
(StuProcInfo *pOutData,
	ULONG cbOutData);

void  FillProcessInfo
(ULONG_PTR pEprocess, 
	StuProcInfo *pOutData,
	ULONG cbOutData);


int  GetIdlelProcessInfomatio
(StuProcInfo *pOutData, 
	ULONG cbOutData);

PHANDLE_TABLE_ENTRY 
ExpLookupHandleTableEntry
(IN PHANDLE_TABLE HandleTable,
	IN EXHANDLE tHandle);


BOOLEAN PsGetProcessPathByPeb(PEPROCESS process, PVOID procId, PWSTR path,_Inout_ int *pathLen);

void GetModInfoByAvlNode(PMMVAD vadNode,ArkModInfo *modInfo);
void TraverseAvlMid(PMMVAD vadNode, ArkModInfo *modInfo);

//得到下一个进程
PEPROCESS ArkGetNextProcess(PEPROCESS Process);



typedef void(*ENUM_THREAD_CALLBACK)(PETHREAD Thread, PVOID ThreadInfo);
/*
	枚举指定进程的所有线程
	参数1 进程的id
	参数2 触发的回调函数
	参数3 穿给回调的缓冲区
	InfoSize ： 缓冲区大小

*/
void EnumProcessThreads(PVOID ProcId, ENUM_THREAD_CALLBACK CallBackFun, ArkThreadInfo *ThreadInfo, ULONG InfoSize);


/*
根据线程对象得到线程信息
参数1 线程对象
参数2 输出缓冲区
*/
void CallbackGetThreadInfo(PETHREAD Thread, PVOID *ThreadInfo);


//调用函数
BOOLEAN  ArkGetProcList
(PCHAR pIndata, 
	ULONG cbInData,
	StuProcInfo *pOutData, 
	ULONG cbOutData);

//隐藏进程
BOOLEAN  ArkHideProcess(
	PCHAR pIndata, 
	ULONG cbInData, 
	StuProcInfo *pOutData,
	ULONG cbOutData);


BOOLEAN ArkGetModListForProc(
	PCHAR pIndata, 
	ULONG cbInData, 
	ArkModInfo *pOutData, 
	ULONG cbOutData);

BOOLEAN ArkHideMod(HideModParam *pIndata, ULONG cbInData, ArkModInfo *pOutData, ULONG cbOutData);
BOOLEAN ArkGetProcHandleInfo(PCHAR pIndata, ULONG cbInData, ArkHandleInfo *pOutData, ULONG cbOutData);
BOOLEAN ArkGetProcHandles(PCHAR pIndata, ULONG cbInData, ArkHandleInfo *pOutData, ULONG cbOutData);
BOOLEAN ArkGetProcThreads(PCHAR pIndata, ULONG cbInData, ArkThreadInfo *pOutData, ULONG cbOutData);//得到进程的所有线程信息
/*
ThreadId ：线程的id

return bool
*/
BOOLEAN ArkSusPendOrResumeThread(ArkThreadSuspendParam *ThreadParam, ULONG cbInData, PVOID pOutData, ULONG cbOutData);//暂停或恢复线程



BOOLEAN ArkLookUpSuspendCount(PVOID *ThreadIdPointer, ULONG cbInData, ULONG * SuspendCount, ULONG cbOutData);//查看线程的暂停次数

/*
终结指定id的线程
*/
BOOLEAN ArkTerminateThread(
	PVOID ThreadId, 
	ULONG cbInData, 
	ULONG * SuspendCount, 
	ULONG cbOutData);

/*
强制终结指定id的线程
*/
BOOLEAN ArkForceTerminateThread(
	PVOID ThreadId, 
	ULONG cbInData, 
	ULONG * SuspendCount, 
	ULONG cbOutData);




/*
终结线程通过指定的线程对象
*/
BOOLEAN ArkTermianteThreadByThread(
	PETHREAD Thread,
	BOOLEAN InitTerminateFalg
);


/*

提醒线程对象
*/
NTSTATUS ArkAlterThread(
	HANDLE Handle,
	PETHREAD Thread
);


/*
得到系统模块
*/
BOOLEAN 
ArkGetSystemModInfo(
	PCHAR pIndata,
	ULONG cbInData,
	ArkModInfo *pOutData,
	ULONG cbOutData
);

/*
位结束线程做准备
*/
BOOLEAN ArkInitTerminateThread(PETHREAD Thread);


BOOLEAN ArkTerminateSystemThread(PETHREAD Thread);

/*
结束内核线程挂入的内核历程
*/
void ArkTerminateThreadKernelRoutine(PKAPC Apc);




/*
得到进程地址通过进程名
*/
PEPROCESS ArkGetProcess(PCHAR ProcName);



/*
得到SSDT表的函数地址
*/
BOOLEAN ArkGetAllSsdtFunAddr(PVOID, PVOID, PVOID *FunAddr, ULONG ReturnSize);



BOOLEAN ArkTerminateProcess(PVOID ProcessId);

