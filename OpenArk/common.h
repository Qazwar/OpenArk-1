/**************************************************************************************
* AUTHOR : 
* DATE   : 2020-2-12
* MODULE : common.h
*
* Command: 
*	IOCTRL Common Header
*
* Description:
*	Common data for the IoCtrl driver and application
*
****************************************************************************************
* Copyright (C) 2010 .
****************************************************************************************/

#pragma once 

#ifdef OPENARK
#include "qt_windows.h"

#else
#include "ntkrnl.h"

//typedef unsigned char  BOOL, *PBOOL;
//typedef unsigned char  BYTE, *PBYTE;
//typedef unsigned long  DWORD, _DWORD, *PDWORD;
//typedef unsigned short WORD, *PWORD;
//typedef void  *HMODULE;
//typedef long NTSTATUS, *PNTSTATUS;
//typedef unsigned long DWORD;
//typedef DWORD * PDWORD;
//typedef unsigned long ULONG;
////typedef unsigned long ULONG_PTR;
//typedef ULONG *PULONG;
//typedef unsigned short WORD;
//typedef unsigned char BYTE; 
//typedef unsigned char UCHAR;
//typedef unsigned short USHORT;
//typedef void *PVOID;
//typedef BYTE BOOLEAN;
//typedef long LONG;

#endif // OPENARK

//#######################################################################################
// D E F I N E S
//#######################################################################################

#if DBG
#define dprintf DbgPrint
#else
#define dprintf
#endif


#ifndef NTKRNL

#endif // NTKRNL


//不支持符号链接用户相关性
#define DEVICE_NAME                  L"\\Device\\devhunter"             // Driver Name
#define SYMBOLIC_LINK_NAME           L"\\DosDevices\\hunter"            // Symbolic Link Name
#define WIN32_LINK_NAME              "\\\\.\\hunter"                    // Win32 Link Name
#define SERVICE_ANME                 "Qhunter"

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
// TODO: Add your IOCTL define here
//



//
// TODO: Add your struct,enum(public) define here
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

enum SYSCALL
{
	ProcList,
	ObjDirectory,

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
	ULONG ProcessCnt;		//0x0

	BOOLEAN IsWoW64Process;//0x9

	ULONG_PTR ParentProcessId;			//0x10
	ULONG_PTR ProcessId;			//0x18
	LARGE_INTEGER CreateTinme;//0x20
	ULONG_PTR	Process;//0x28
	WCHAR  wStrProcessPath[252];

};

#define SIZEOFPROCINFO sizeof(StuProcInfo)

/* EOF */

