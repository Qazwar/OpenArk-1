/***************************************************************************************
* AUTHOR : 
* DATE   : 2020-2-12
* MODULE : struct.h
* 
* Command: 
*   驱动的头文件
*
* Description:
*   定义一些常量,避免重复劳动; 您可以在此添加需要的函数/结构体
*
****************************************************************************************

Copyright (C) 2010 .
****************************************************************************************/

#pragma once
#include "ntkrnl.h"
#include <ntifs.h>
#include "common.h"
#include "ob.h"
/*
sc  service
*/






enum {
//windows 版本
	Windows_2000 = 0x893,
	Windows_Vista = 0x1770,
	Windows7_RTM = 7600,
	Windows7_SP1 = 7601,
	
	Windows8 = 0x23F0,
	Windows8_1 = 0x2580,
	Windows10_2015 = 0x2800,

//偏移定义
	
	

};


#define IN
#define OUT
#define TAG 'pc'
#define  GETWORD(x)   (*(USHORT*)(x))
#define  GETDWORD(x)  (*(ULONG*)(x))
#define  GETINT(x)  (*(int*)(x))
#define  GETQWORD(x)  (*(ULONG64*)(x))
#define offsetof(s,m) ((size_t)&(((s*)0)->m))
#define   _DWORD ULONG
#define   _WORD USHORT
#define  _BYTE  BYTE
#define DBGERRINFO  DbgPrint("%s--%d", __FILE__,__LINE__);
#define THREE_FF 0xFFFFFF

#define _TRY  __try {



#define TRY_   }__except(1){DbgPrint("%s--%d", __FILE__,__LINE__);}
//typedef   void* PVOID;

#define  _FILE_OBJECT FILE_OBJECT 
//typedef long LONG;
//typedef unsigned char  BOOL, *PBOOL;
//typedef unsigned char  BYTE, *PBYTE;
//typedef unsigned long  DWORD, _DWORD, *PDWORD;
//typedef unsigned short WORD, *PWORD;
//typedef ULONG64 _QWORD, QWORD;
////
//typedef void  *HMODULE;
//typedef long NTSTATUS, *PNTSTATUS;
//typedef unsigned long DWORD;
//typedef DWORD * PDWORD;
//typedef unsigned long ULONG;
//typedef unsigned long ULONG_PTR;
//typedef ULONG *PULONG;
//typedef unsigned short WORD;
//typedef unsigned char BYTE; 
//typedef unsigned char UCHAR;
//typedef unsigned short USHORT;
//typedef void *PVOID;
//typedef BYTE BOOLEAN;

#define SEC_IMAGE    0x01000000





//function declare
typedef NTSTATUS(*FunObQueryNameString)(IN PVOID  pObject, OUT POBJECT_NAME_INFORMATION  ObjectNameInfo, IN ULONG  Length, OUT PULONG  ReturnLength);
typedef POBJECT_TYPE(*FunObGetObjectType)(IN PVOID pObject);
typedef PEPROCESS(*FunIoThreadToProcess)(IN PETHREAD Thread);
typedef PEPROCESS(*FunPsGetCurrentProcess)(VOID);
typedef BOOLEAN(*FunMmIsAddressValid)(IN ULONG_PTR VirtualAddress);
typedef BOOLEAN(*FunSysCall)(PCHAR pIndata, ULONG cbInData, PCHAR pOutData, ULONG cbOutData);
typedef PEPROCESS(*FunIoGetCurrentProcess)(VOID);
typedef NTSTATUS(*FunPsLookupProcessByProcessId)(__in HANDLE ProcessId, __deref_out PEPROCESS *Process);
typedef NTSTATUS(*FunPsAcquireProcessExitSynchronization)(__in  PEPROCESS Process);
	



	





typedef void (*EX_ENUMERATE_HANDLE_ROUTINE)(ULONG_PTR pObject, StuProcInfo *pOutData, ULONG cbOutData, PVOID uEnumParam);
	



#ifndef INIT_H
#define EXTERN extern
#else
#define EXTERN
#endif // !INIT_H



//----------------------------------------------------
namespace NT
{
	//ntoskrnl信息
	EXTERN PCHAR				ImageBaseCopyNt;
	EXTERN PCHAR				ImageBaseRunNt;
	EXTERN ULONG				SizeOfNtImage;
	EXTERN WCHAR				NtFullName[];
	EXTERN USHORT				OsVersion;
	
	//未导出
	EXTERN POBJECT_TYPE			*ObTypeIndexTable;
	EXTERN POBJECT_DIRECTORY	ObpRootDirectoryObject;
	EXTERN PHANDLE_TABLE		*PPspCidTable;
	EXTERN ObjectType			ArrObjectType[ObjectType::LastObjectType];

	//ntkrnl导出
	EXTERN POBJECT_TYPE						IoFileObjectType;
	EXTERN FunIoGetCurrentProcess 			IoGetCurrentProcess;
	EXTERN FunIoThreadToProcess				IoThreadToProcess;
	EXTERN FunMmIsAddressValid				MmIsAddressValid;
	EXTERN FunPsLookupProcessByProcessId	PsLookupProcessByProcessId;
	EXTERN FunPsAcquireProcessExitSynchronization	PsAcquireProcessExitSynchronization;
	EXTERN FunPsAcquireProcessExitSynchronization	PsReleaseProcessExitSynchronization;
		

	extern FunObQueryNameString ObQueryNameString;
	extern FunObGetObjectType   ObGetObjectType;
	
	//版本间的偏移信息
	EXTERN UCHAR				ProcessTypeIndex;
	extern UCHAR				ThreadTypeIndex;

	//此驱动相关
	extern PDRIVER_OBJECT		DriverObject;

	


	namespace EPROCESS
	{
		extern UCHAR			TypeIndexOffset;
		EXTERN ULONG			ObjectTableOffset;
		
		namespace HANDLE_TABLE 
		{
			EXTERN	ULONG UniqueProcessIdOffset;

		};
		EXTERN ULONG			UniqueProcessIdOffset;
		EXTERN ULONG			InheritedFromUniqueProcessIdOffset;
		EXTERN ULONG			WoW64processOffset;
		EXTERN ULONG			CreateTimeOffset;
		EXTERN ULONG			SectionObjectOffset;
		EXTERN ULONG			ImageFileNameOffset;
		EXTERN ULONG			PebOffset;
		EXTERN ULONG			ActiveProcessLinksOffset;
		EXTERN ULONG			VadRootOffset;


		

	};

	namespace KTHREAD
	{
		EXTERN ULONG   ProcessOffset;
	}

	namespace KPRCB
	{
		

	};

};
/*
 xx_ 之后
 _xx 之前

*/
namespace WIN8_ {

	enum HANDLE_TABLE {
		UniqueProcessIdOffset = 0x28,

	};

}


namespace _WIN7 {

	enum HANDLE_TABLE {
		UniqueProcessIdOffset = 0x10,

	};

}






////  PEB
//   
//#pragma pack(4)
//typedef struct _PEB_LDR_DATA
//{
//	ULONG Length;
//	BOOLEAN Initialized;
//	PVOID SsHandle;
//	LIST_ENTRY InLoadOrderModuleList;
//	LIST_ENTRY InMemoryOrderModuleList;
//	LIST_ENTRY InInitializationOrderModuleList;
//} PEB_LDR_DATA, *PPEB_LDR_DATA;
//#pragma pack() 
//
//typedef struct _PEB_ORIG {
//    BYTE Reserved1[2];
//    BYTE BeingDebugged;
//    BYTE Reserved2[229];
//    PVOID Reserved3[59];
//    ULONG SessionId;
//} PEB_ORIG, *PPEB_ORIG;
//
//typedef void (*PPEBLOCKROUTINE)(PVOID PebLock);
//
//struct _PEB_FREE_BLOCK {
//	struct _PEB_FREE_BLOCK *Next;
//	ULONG Size;
//};
//typedef struct _PEB_FREE_BLOCK PEB_FREE_BLOCK;
//typedef struct _PEB_FREE_BLOCK *PPEB_FREE_BLOCK;
//
//typedef struct _RTL_DRIVE_LETTER_CURDIR {
//	USHORT Flags;
//	USHORT Length;
//	ULONG TimeStamp;
//	UNICODE_STRING DosPath;
//} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;
//
//typedef struct _RTL_USER_PROCESS_PARAMETERS {
//	ULONG MaximumLength;
//	ULONG Length;
//	ULONG Flags;
//	ULONG DebugFlags;
//	PVOID ConsoleHandle;
//	ULONG ConsoleFlags;
//	HANDLE StdInputHandle;
//	HANDLE StdOutputHandle;
//	HANDLE StdErrorHandle;
//	UNICODE_STRING CurrentDirectoryPath;
//	HANDLE CurrentDirectoryHandle;
//	UNICODE_STRING DllPath;
//	UNICODE_STRING ImagePathName;
//	UNICODE_STRING CommandLine;
//	PVOID Environment;
//	ULONG StartingPositionLeft;
//	ULONG StartingPositionTop;
//	ULONG Width;
//	ULONG Height;
//	ULONG CharWidth;
//	ULONG CharHeight;
//	ULONG ConsoleTextAttributes;
//	ULONG WindowFlags;
//	ULONG ShowWindowFlags;
//	UNICODE_STRING WindowTitle;
//	UNICODE_STRING DesktopName;
//	UNICODE_STRING ShellInfo;
//	UNICODE_STRING RuntimeData;
//	RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];
//} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;
//
//typedef struct _PEB {
//	BOOLEAN InheritedAddressSpace;
//	BOOLEAN ReadImageFileExecOptions;
//	BOOLEAN BeingDebugged;
//	BOOLEAN Spare;
//	HANDLE Mutant;
//	PVOID ImageBaseAddress;
//	PPEB_LDR_DATA LoaderData;
//	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
//	PVOID SubSystemData;
//	PVOID ProcessHeap;
//	PVOID FastPebLock;
//	PPEBLOCKROUTINE FastPebLockRoutine;
//	PPEBLOCKROUTINE FastPebUnlockRoutine;
//	ULONG EnvironmentUpdateCount;
//	PVOID *KernelCallbackTable;
//	PVOID EventLogSection;
//	PVOID EventLog;
//	PPEB_FREE_BLOCK FreeList;
//	ULONG TlsExpansionCounter;
//	PVOID TlsBitmap;
//	ULONG TlsBitmapBits[0x2];
//	PVOID ReadOnlySharedMemoryBase;
//	PVOID ReadOnlySharedMemoryHeap;
//	PVOID *ReadOnlyStaticServerData;
//	PVOID AnsiCodePageData;
//	PVOID OemCodePageData;
//	PVOID UnicodeCaseTableData;
//	ULONG NumberOfProcessors;
//	ULONG NtGlobalFlag;
//	BYTE Spare2[0x4];
//	LARGE_INTEGER CriticalSectionTimeout;
//	ULONG HeapSegmentReserve;
//	ULONG HeapSegmentCommit;
//	ULONG HeapDeCommitTotalFreeThreshold;
//	ULONG HeapDeCommitFreeBlockThreshold;
//	ULONG NumberOfHeaps;
//	ULONG MaximumNumberOfHeaps;
//	PVOID **ProcessHeaps;
//	PVOID GdiSharedHandleTable;
//	PVOID ProcessStarterHelper;
//	PVOID GdiDCAttributeList;
//	PVOID LoaderLock;
//	ULONG OSMajorVersion;
//	ULONG OSMinorVersion;
//	ULONG OSBuildNumber;
//	ULONG OSPlatformId;
//	ULONG ImageSubSystem;
//	ULONG ImageSubSystemMajorVersion;
//	ULONG ImageSubSystemMinorVersion;
//	ULONG GdiHandleBuffer[0x22];
//	ULONG PostProcessInitRoutine;
//	ULONG TlsExpansionBitmap;
//	BYTE TlsExpansionBitmapBits[0x80];
//	ULONG SessionId;
//} PEB, *PPEB;
//
//typedef struct _SYSTEM_PROCESS_INFORMATION {
//    ULONG NextEntryOffset;
//    ULONG NumberOfThreads;
//    LARGE_INTEGER SpareLi1;
//    LARGE_INTEGER SpareLi2;
//    LARGE_INTEGER SpareLi3;
//    LARGE_INTEGER CreateTime;
//    LARGE_INTEGER UserTime;
//    LARGE_INTEGER KernelTime;
//    UNICODE_STRING ImageName;
//    KPRIORITY BasePriority;
//    HANDLE UniqueProcessId;
//    HANDLE InheritedFromUniqueProcessId;
//    ULONG HandleCount;
//    ULONG SpareUl2;
//    ULONG SpareUl3;
//    ULONG PeakVirtualSize;
//    ULONG VirtualSize;
//    ULONG PageFaultCount;
//    ULONG PeakWorkingSetSize;
//    ULONG WorkingSetSize;
//    ULONG QuotaPeakPagedPoolUsage;
//    ULONG QuotaPagedPoolUsage;
//    ULONG QuotaPeakNonPagedPoolUsage;
//    ULONG QuotaNonPagedPoolUsage;
//    ULONG PagefileUsage;
//    ULONG PeakPagefileUsage;
//    ULONG PrivatePageCount;
//} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;
//
//typedef struct _SYSTEM_THREAD_INFORMATION {
//    LARGE_INTEGER KernelTime;
//    LARGE_INTEGER UserTime;
//    LARGE_INTEGER CreateTime;
//    ULONG WaitTime;
//    PVOID StartAddress;
//    CLIENT_ID ClientId;
//    KPRIORITY Priority;
//    LONG BasePriority;
//    ULONG ContextSwitches;
//    ULONG ThreadState;
//    ULONG WaitReason;
//} SYSTEM_THREAD_INFORMATION, *PSYSTEM_THREAD_INFORMATION;
//
//struct _SYSTEM_THREADS
//{
//	LARGE_INTEGER		KernelTime;
//	LARGE_INTEGER		UserTime;
//	LARGE_INTEGER		CreateTime;
//	ULONG				WaitTime;
//	PVOID				StartAddress;
//	CLIENT_ID			ClientIs;
//	KPRIORITY			Priority;
//	KPRIORITY			BasePriority;
//	ULONG				ContextSwitchCount;
//	ULONG				ThreadState;
//	KWAIT_REASON		WaitReason;
//};
//
//struct _SYSTEM_PROCESSES
//{
//	ULONG				NextEntryDelta;
//	ULONG				ThreadCount;
//	ULONG				Reserved[6];
//	LARGE_INTEGER		CreateTime;
//	LARGE_INTEGER		UserTime;
//	LARGE_INTEGER		KernelTime;
//	UNICODE_STRING		ProcessName;
//	KPRIORITY			BasePriority;
//	ULONG				ProcessId;
//	ULONG				InheritedFromProcessId;
//	ULONG				HandleCount;
//	ULONG				Reserved2[2];
//	VM_COUNTERS			VmCounters;
//	IO_COUNTERS			IoCounters; //windows 2000 only
//	struct _SYSTEM_THREADS	Threads[1];
//};
//
//typedef struct _HANDLE_TABLE_ENTRY_INFO
//{
//    ULONG AuditMask;
//} HANDLE_TABLE_ENTRY_INFO, *PHANDLE_TABLE_ENTRY_INFO;
//
//typedef struct _HANDLE_TABLE_ENTRY
//{
//    union
//    {
//        PVOID Object;
//        ULONG_PTR ObAttributes;
//        PHANDLE_TABLE_ENTRY_INFO InfoTable;
//        ULONG_PTR Value;
//    };
//    union
//    {
//        ULONG GrantedAccess;
//        struct
//        {
//            USHORT GrantedAccessIndex;
//            USHORT CreatorBackTraceIndex;
//        };
//        LONG NextFreeTableEntry;
//    };
//} HANDLE_TABLE_ENTRY, *PHANDLE_TABLE_ENTRY;
//
//typedef struct _HANDLE_TABLE
//{
//    ULONG TableCode;
//    PEPROCESS QuotaProcess;
//    PVOID UniqueProcessId;
//    ULONG HandleTableLock[4];
//    LIST_ENTRY HandleTableList;
//    ULONG HandleContentionEvent;
//    PVOID DebugInfo;
//    LONG ExtraInfoPages;
//    ULONG FirstFree;
//    ULONG LastFree;
//    ULONG NextHandleNeedingPool;
//    LONG HandleCount;
//    union
//    {
//        ULONG Flags;
//        UCHAR StrictFIFO:1;
//    };
//} HANDLE_TABLE, *PHANDLE_TABLE;
//
//typedef struct _OBJECT_TYPE_INITIALIZER {
//	USHORT Length;
//	BOOLEAN UseDefaultObject;
//	BOOLEAN CaseInsensitive;
//	ULONG InvalidAttributes;
//	GENERIC_MAPPING GenericMapping;
//	ULONG ValidAccessMask;
//	BOOLEAN SecurityRequired;
//	BOOLEAN MaintainHandleCount;
//	BOOLEAN MaintainTypeList;
//	POOL_TYPE PoolType;
//	ULONG DefaultPagedPoolCharge;
//	ULONG DefaultNonPagedPoolCharge;
//	PVOID DumpProcedure;
//	PVOID OpenProcedure;
//	PVOID CloseProcedure;
//	PVOID DeleteProcedure;
//	PVOID ParseProcedure;
//	PVOID SecurityProcedure;
//	PVOID QueryNameProcedure;
//	PVOID OkayToCloseProcedure;
//} OBJECT_TYPE_INITIALIZER, *POBJECT_TYPE_INITIALIZER;
//
//
//typedef struct _OBJECT_TYPE {
//	ERESOURCE Mutex;
//	LIST_ENTRY TypeList;
//	UNICODE_STRING Name;            // Copy from object header for convenience
//	PVOID DefaultObject;
//	ULONG Index;
//	ULONG TotalNumberOfObjects;
//	ULONG TotalNumberOfHandles;
//	ULONG HighWaterNumberOfObjects;
//	ULONG HighWaterNumberOfHandles;
//	OBJECT_TYPE_INITIALIZER TypeInfo;
//	ULONG Key;
//	ERESOURCE ObjectLocks[4];
//} OBJECT_TYPE, *POBJECT_TYPE;
//
//typedef struct _OBJECT_DIRECTORY {
//    struct _OBJECT_DIRECTORY_ENTRY *HashBuckets[ 37 ];
//    ULONG Lock;
//    PVOID DeviceMap;
//    ULONG SessionId;
//	USHORT Reserved;
//	USHORT SymbolicLinkUsageCount;
//} OBJECT_DIRECTORY, *POBJECT_DIRECTORY;

/*
typedef enum _KAPC_ENVIRONMENT {
  OriginalApcEnvironment,
  AttachedApcEnvironment,
  CurrentApcEnvironment,
  InsertApcEnvironment
} KAPC_ENVIRONMENT;
*/

//typedef enum
//{
//    OriginalApcEnvironment,
//	AttachedApcEnvironment,
//	CurrentApcEnvironment
//} KAPC_ENVIRONMENT;
//
////----------------------------------------------------
//
//NTSYSAPI
//NTSTATUS
//NTAPI ZwQuerySystemInformation(
//							   IN ULONG SystemInformationClass,
//							   IN PVOID SystemInformation,
//							   IN ULONG SystemInformationLength,
//							   OUT PULONG ReturnLength);
//
//
//
//NTSTATUS
//  NtOpenFile(
//    OUT PHANDLE  FileHandle,
//    IN ACCESS_MASK  DesiredAccess,
//    IN POBJECT_ATTRIBUTES  ObjectAttributes,
//    OUT PIO_STATUS_BLOCK  IoStatusBlock,
//    IN ULONG  ShareAccess,
//    IN ULONG  OpenOptions
//    );
//
//NTSTATUS 
//ZwOpenProcess(
//   OUT PHANDLE ProcessHandle, 
//   IN ACCESS_MASK DesiredAccess, 
//   IN POBJECT_ATTRIBUTES ObjectAttributes, 
//   IN PCLIENT_ID ClientId
//   );
//
//NTSTATUS
//PsLookupProcessByProcessId(
//    IN HANDLE ProcessId,
//    OUT PEPROCESS *Process
//    );
//
//HANDLE
//  PsGetProcessId(
//    IN PEPROCESS  Process
//    );
//
//NTSTATUS
//RtlFormatCurrentUserKeyPath(
//    OUT PUNICODE_STRING CurrentUserKeyPath
//    );
//
//VOID KeAttachProcess( PEPROCESS proc );
//VOID KeDetachProcess();
