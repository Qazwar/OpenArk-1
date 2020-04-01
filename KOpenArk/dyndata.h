#ifndef DYNDATA_H
#define DYNDATA_H










enum {
//windows �汾
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
typedef void (*EX_ENUMERATE_HANDLE_ROUTINE)(ULONG_PTR pObject, StuProcInfo *pOutData, ULONG cbOutData, PVOID uEnumParam);
	



#ifndef _DYNDATA_PRIVATE
#define EXTERN extern
#else
#define EXTERN
#endif // !_DYNDATA_PRIVATE



//----------------------------------------------------
namespace NT
{
	//ntoskrnl��Ϣ
	EXTERN PCHAR				ImageBaseCopyNt;
	EXTERN PCHAR				ImageBaseRunNt;
	EXTERN ULONG				SizeOfNtImage;
	EXTERN WCHAR				NtFullName[];
	EXTERN USHORT				OsVersion;

	//δ����
	EXTERN POBJECT_TYPE			*ObTypeIndexTable;
	EXTERN POBJECT_DIRECTORY	ObpRootDirectoryObject;
	EXTERN PHANDLE_TABLE		*PPspCidTable;
	EXTERN ObjectType			ArrObjectType[ObjectType::LastObjectType];

	//ntkrnl����
	EXTERN POBJECT_TYPE						IoFileObjectType;
	EXTERN FunIoGetCurrentProcess 			IoGetCurrentProcess;
	EXTERN FunIoThreadToProcess				IoThreadToProcess;
	EXTERN FunMmIsAddressValid				MmIsAddressValid;
	EXTERN FunPsLookupProcessByProcessId	PsLookupProcessByProcessId;
	EXTERN FunPsAcquireProcessExitSynchronization	PsAcquireProcessExitSynchronization;
	EXTERN FunPsAcquireProcessExitSynchronization	PsReleaseProcessExitSynchronization;


	extern FunObQueryNameString ObQueryNameString;
	extern FunObGetObjectType   ObGetObjectType;

	//�汾���ƫ����Ϣ
	EXTERN UCHAR				ProcessTypeIndex;
	extern UCHAR				ThreadTypeIndex;

	//���������
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
 xx_ ֮��
 _xx ֮ǰ

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