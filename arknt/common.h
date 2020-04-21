#ifndef COMMON_H
#define COMMON_H





#ifdef ARK_MODE_USER
#include <Windows.h>



#else
#define MAX_PATH 260
typedef void* HWND;

#endif // ARK_MODE_USER

//#######################################################################################
// D E F I N E S
//#######################################################################################





//不支持符号链接用户相关性
#define DEVICE_NAME                  L"\\Device\\devhunter"             // Driver Name
#define SYMBOLIC_LINK_NAME           L"\\DosDevices\\hunter"            // Symbolic Link Name
#define WIN32_LINK_NAME              "\\\\.\\hunter"                    // Win32 Link Name
#define SERVICE_ANME                 "KOpenArk"

//支持符号链接用户相关性
#define SYMBOLIC_LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\hunter"    // Symbolic Link Name

#define DATA_TO_APP                  "Hello World from Driver"

//
// Device IO Control Codes
//
#define IOCTL_BASE          0x800
#define MY_CTL_CODE(i)        \
    CTL_CODE                  \
    (                         \
        FILE_DEVICE_UNKNOWN,  \
        IOCTL_BASE + i,       \
        METHOD_BUFFERED,      \
        FILE_ANY_ACCESS       \
    )

#define IOCTL_HELLO_WORLD            MY_CTL_CODE(0)
#define IOCTRL_REC_FROM_APP          MY_CTL_CODE(1)
#define IOCTRL_SEND_TO_APP           MY_CTL_CODE(2)



//
#define SIZE4M   0x400000  //4m
typedef struct ObInfo {
#define NAMELEN  0x100
	BOOLEAN IsDirectory;
	BOOLEAN IsSymLink;
	ULONG SubItems;
	ULONG_PTR ObjAddr;
	WCHAR ObjName[NAMELEN];
	WCHAR TypeName[NAMELEN / 2];
	WCHAR SymLinkName[NAMELEN];
}*PObInfo;

enum DrvCall
{
	ProcList,
	ObjDirectory,
	HideProcess,
	ModList,
	HideMod,
	ProcHandleList,
	ProcThreadList,
	SystemMods,
	SusPendCount,
	SuspendThreadEnum,
	TerminateThreadFunIndex,
	ForceTerminateThread,
	QueryHwnd,
	GetAllSsdtFunAddr,
	GetAllShadowSdtFunAddr,
	TestHook,
	LastId
};

struct ParamInfo
{
	ULONG FunIdx;
	PCHAR pInData;
	ULONG cbInData;
	PCHAR pOutData;
	ULONG cbOutData;
};


struct StuProcInfo
{
	ULONG ProcessCnt;		
	BOOLEAN IsWoW64Process;
	ULONG_PTR ParentProcessId;			
	ULONG_PTR ProcessId;			
	LARGE_INTEGER CreateTinme;
	ULONG_PTR	Process;
	WCHAR  wStrProcessPath[MAX_PATH];

};

struct ArkModInfo
{
	ULONG NumberOfMods;
	ULONG_PTR RegionBase;
	ULONG_PTR RegionSize;
	WCHAR Path[MAX_PATH];

};
struct HideModParam
{
	PVOID Id;
	ULONG_PTR ModBase;
};

typedef struct ArkHandleInfo
{
	ULONG HandleCnt;
	PVOID Object;
	HANDLE Handle;
	ULONG TypeIndex;
	INT64 RefreceCount;
	WCHAR HandleName[MAX_PATH];
	WCHAR TypeName[MAX_PATH];
}*PArkHandleInfo;

//得到线程信息
 struct ArkThreadInfoEntry
{
	 PVOID ThreadId;
	 PVOID Ethread;
	 PVOID Teb;
	 PVOID StartAddress;
	 PVOID Win32StartAddress;
	 LONGLONG ContextSwitches;
	 char Priority;
	 char State;
	 WCHAR InModName[MAX_PATH];
};
struct ArkThreadInfo
{
	ULONG ThreadCnt;
	ArkThreadInfoEntry Threads[1];
};

struct ArkThreadSuspendParam
{
	PVOID ThreadId;
	BOOLEAN ToSuspend;
};

//文件相关。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。
typedef struct _FILE_REQUEST_CREATE
{
	HANDLE FileHandle;
	ULONG ShareAccess;
	ULONG DesiredAccess;
	ULONG CreateDisposition;
	PCHAR FileName;
} FILE_REQUEST_CREATE, *PFILE_REQUEST_CREATE;

typedef struct _FILE_REQUEST_READ
{
	HANDLE FileHandle;
	ULONG Length;
	ULONG ReadLength;
	PCHAR Buffer;
} FILE_REQUEST_READ, *PFILE_REQUEST_READ;



//窗口相关
struct  ArkWindowInfo
{
	HWND Hwnd;
	PVOID ProcessId;
};

#endif // !COMMON_H