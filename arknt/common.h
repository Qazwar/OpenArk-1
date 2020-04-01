#ifndef COMMON_H
#define COMMON_H





#ifdef ARK_MODE_USER
#include "qt_windows.h"



#else
#define MAX_PATH 260


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

struct ModInfo
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






#endif // !COMMON_H