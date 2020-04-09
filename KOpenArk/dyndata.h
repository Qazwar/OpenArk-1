#ifndef DYNDATA_H
#define DYNDATA_H










enum {
//windows 版本
	Windows_2000 = 0x893,
	Windows_Vista = 0x1770,
	Windows7_RTM = 7600,
	Windows7_SP1 = 7601,
	
	Windows8 = 0x23F0,
	Windows8_1 = 0x2580,
	Windows10_2015 = 0x2800,


	
	

};



#define TAG 'pc'






//#define  _FILE_OBJECT FILE_OBJECT 

typedef struct _KSERVICE_TABLE_DESCRIPTOR {
	PULONG_PTR Base;
	PULONG Count;
	ULONG Limit;
	PUCHAR Number;
} KSERVICE_TABLE_DESCRIPTOR, *PKSERVICE_TABLE_DESCRIPTOR;






//function declare
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
typedef void (*ArkEX_ENUMERATE_HANDLE_ROUTINE)(ULONG_PTR pObject, StuProcInfo *pOutData, ULONG cbOutData, PVOID uEnumParam);
typedef NTSTATUS
(*FunNtSuspendThread)(
	__in HANDLE ThreadHandle,
	__out_opt PULONG PreviousSuspendCount
	);
typedef NTSTATUS
(*FunNtResumeThread)(
	__in HANDLE ThreadHandle,
	__out_opt PULONG PreviousSuspendCount
);




#ifndef _DYNDATA_PRIVATE
#define EXTERN extern
#else
#define EXTERN
#endif // !_DYNDATA_PRIVATE



//----------------------------------------------------
namespace NT
{
	//ntoskrnl信息
	EXTERN PEPROCESS			SystemProcess;
	EXTERN PCHAR				ImageBaseCopyNt;
	EXTERN PCHAR				ImageBaseRunNt;
	EXTERN ULONG				SizeOfNtImage;
	EXTERN WCHAR				NtFullName[];
	EXTERN USHORT				OsVersion;
	EXTERN PLIST_ENTRY			PsLoadedModuleList;

	//未导出
	EXTERN POBJECT_TYPE			*ObTypeIndexTable;
	EXTERN POBJECT_DIRECTORY	ObpRootDirectoryObject;
	EXTERN PHANDLE_TABLE		*PPspCidTable;
	EXTERN PKSERVICE_TABLE_DESCRIPTOR		KeServiceDescriptorTable;
	EXTERN ObjectType			ArrObjectType[ObjectType::LastObjectType];

	//ntkrnl导出
	EXTERN POBJECT_TYPE						IoFileObjectType;
	EXTERN FunPsLookupProcessByProcessId	PsLookupProcessByProcessId;
	EXTERN FunNtSuspendThread				NtSuspendThread;
	EXTERN FunNtResumeThread				NtResumeThread;

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
		EXTERN ULONG   PreviousModeOffset;
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
#endif // !DYNDATA_H