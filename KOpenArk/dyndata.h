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
	PLONG Base;
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
typedef VOID
(*FunKiInsertQueueApc)(
	IN PKAPC Apc,
	IN KPRIORITY Increment
);
typedef
NTSTATUS
(*FunPspTerminateThreadByPointer)(
	IN PETHREAD Thread,
	IN NTSTATUS ExitStatus,
	IN BOOLEAN DirectTerminate
);

typedef
PEPROCESS
(*FunPsGetNextProcess)(
	IN PEPROCESS Process
	);

typedef
NTSTATUS
(*FunNtTerminateProcess)(
	__in_opt HANDLE ProcessHandle,
	__in NTSTATUS ExitStatus
	);


#ifndef _DYNDATA_PRIVATE
#define EXTERN extern
#else
#define EXTERN
#endif // !_DYNDATA_PRIVATE



//----------------------------------------------------
namespace NT
{

	class ServiceSerial
	{
	public:
		enum {


			NtMapUserPhysicalPagesScatter,
			NtWaitForSingleObject,
			NtCallbackReturn,
			NtReadFile,
			NtDeviceIoControlFile,
			NtWriteFile,
			NtRemoveIoCompletion,
			NtReleaseSemaphore,
			NtReplyWaitReceivePort,
			NtReplyPort,
			NtSetInformationThread,
			NtSetEvent,
			NtClose,
			NtQueryObject,
			NtQueryInformationFile,
			NtOpenKey,
			NtEnumerateValueKey,
			NtFindAtom,
			NtQueryDefaultLocale,
			NtQueryKey,
			NtQueryValueKey,
			NtAllocateVirtualMemory,
			NtQueryInformationProcess,
			NtWaitForMultipleObjects32,
			NtWriteFileGather,
			NtSetInformationProcess,
			NtCreateKey,
			NtFreeVirtualMemory,
			NtImpersonateClientOfPort,
			NtReleaseMutant,
			NtQueryInformationToken,
			NtRequestWaitReplyPort,
			NtQueryVirtualMemory,
			NtOpenThreadToken,
			NtQueryInformationThread,
			NtOpenProcess,
			NtSetInformationFile,
			NtMapViewOfSection,
			NtAccessCheckAndAuditAlarm,
			NtUnmapViewOfSection,
			NtReplyWaitReceivePortEx,
			NtTerminateProcess,
			NtSetEventBoostPriority,
			NtReadFileScatter,
			NtOpenThreadTokenEx,
			NtOpenProcessTokenEx,
			NtQueryPerformanceCounter,
			NtEnumerateKey,
			NtOpenFile,
			NtDelayExecution,
			NtQueryDirectoryFile,
			NtQuerySystemInformation,
			NtOpenSection,
			NtQueryTimer,
			NtFsControlFile,
			NtWriteVirtualMemory,
			NtCloseObjectAuditAlarm,
			NtDuplicateObject,
			NtQueryAttributesFile,
			NtClearEvent,
			NtReadVirtualMemory,
			NtOpenEvent,
			NtAdjustPrivilegesToken,
			NtDuplicateToken,
			NtContinue,
			NtQueryDefaultUILanguage,
			NtQueueApcThread,
			NtYieldExecution,
			NtAddAtom,
			NtCreateEvent,
			NtQueryVolumeInformationFile,
			NtCreateSection,
			NtFlushBuffersFile,
			NtApphelpCacheControl,
			NtCreateProcessEx,
			NtCreateThread,
			NtIsProcessInJob,
			NtProtectVirtualMemory,
			NtQuerySection,
			NtResumeThread,
			NtTerminateThread,
			NtReadRequestData,
			NtCreateFile,
			NtQueryEvent,
			NtWriteRequestData,
			NtOpenDirectoryObject,
			NtAccessCheckByTypeAndAuditAlarm,
			NtQuerySystemTime,
			NtWaitForMultipleObjects,
			NtSetInformationObject,
			NtCancelIoFile,
			NtTraceEvent,
			NtPowerInformation,
			NtSetValueKey,
			NtCancelTimer,
			NtSetTimer,
			NtAcceptConnectPort,
			NtAccessCheck,
			NtAccessCheckByType,
			NtAccessCheckByTypeResultList,
			NtAccessCheckByTypeResultListAndAuditAlarm,
			NtAccessCheckByTypeResultListAndAuditAlarmByHandle,
			NtAddBootEntry,
			NtAddDriverEntry,
			NtAdjustGroupsToken,
			NtAlertResumeThread,
			NtAlertThread,
			NtAllocateLocallyUniqueId,
			NtAllocateReserveObject,
			NtAllocateUserPhysicalPages,
			NtAllocateUuids,
			NtAlpcAcceptConnectPort,
			NtAlpcCancelMessage,
			NtAlpcConnectPort,
			NtAlpcCreatePort,
			NtAlpcCreatePortSection,
			NtAlpcCreateResourceReserve,
			NtAlpcCreateSectionView,
			NtAlpcCreateSecurityContext,
			NtAlpcDeletePortSection,
			NtAlpcDeleteResourceReserve,
			NtAlpcDeleteSectionView,
			NtAlpcDeleteSecurityContext,
			NtAlpcDisconnectPort,
			NtAlpcImpersonateClientOfPort,
			NtAlpcOpenSenderProcess,
			NtAlpcOpenSenderThread,
			NtAlpcQueryInformation,
			NtAlpcQueryInformationMessage,
			NtAlpcRevokeSecurityContext,
			NtAlpcSendWaitReceivePort,
			NtAlpcSetInformation,
			NtAreMappedFilesTheSame,
			NtAssignProcessToJobObject,
			NtCancelIoFileEx,
			NtCancelSynchronousIoFile,
			NtCommitComplete,
			NtCommitEnlistment,
			NtCommitTransaction,
			NtCompactKeys,
			NtCompareTokens,
			NtCompleteConnectPort,
			NtCompressKey,
			NtConnectPort,
			NtCreateDebugObject,
			NtCreateDirectoryObject,
			NtCreateEnlistment,
			NtCreateEventPair,
			NtCreateIoCompletion,
			NtCreateJobObject,
			NtCreateJobSet,
			NtCreateKeyTransacte,
			NtCreateKeyedEvent,
			NtCreateMailslotFile,
			NtCreateMutant,
			NtCreateNamedPipeFile,
			NtCreatePagingFile,
			NtCreatePort,
			NtCreatePrivateNamespace,
			NtCreateProcess,
			NtCreateProfile,
			NtCreateProfileEx,
			NtCreateResourceManager,
			NtCreateSemaphore,
			NtCreateSymbolicLinkObject,
			NtCreateThreadEx,
			NtCreateTimer,
			NtCreateToken,
			NtCreateTransaction,
			NtCreateTransactionManager,
			NtCreateUserProcess,
			NtCreateWaitablePort,
			NtCreateWorkerFactory,
			NtDebugActiveProcess,
			NtDebugContinue,
			NtDeleteAtom,
			NtDeleteBootEntry,
			NtDeleteDriverEntry,
			NtDeleteFile,
			NtDeleteKey,
			NtDeleteObjectAuditAlarm,
			NtDeletePrivateNamespace,
			NtDeleteValueKey,
			NtDisableLastKnownGood,
			NtDisplayString,
			NtDrawText,
			NtEnableLastKnownGood,
			NtEnumerateBootEntries,
			NtEnumerateDriverEntries,
			NtEnumerateSystemEnvironmentValuesEx,
			NtEnumerateTransactionObject,
			NtExtendSection,
			NtFilterToken,
			NtFlushInstallUILanguage,
			NtFlushInstructionCache,
			NtFlushKey,
			NtFlushProcessWriteBuffers,
			NtFlushVirtualMemory,
			NtFlushWriteBuffer,
			NtFreeUserPhysicalPages,
			NtFreezeRegistry,
			NtFreezeTransactions,
			NtGetContextThread,
			NtGetCurrentProcessorNumber,
			NtGetDevicePowerState,
			NtGetMUIRegistryInfo,
			NtGetNextProcess,
			NtGetNextThread,
			NtGetNlsSectionPtr,
			NtGetNotificationResourceManager,
			NtGetPlugPlayEvent,
			NtGetWriteWatch,
			NtImpersonateAnonymousToken,
			NtImpersonateThread,
			NtInitializeNlsFiles,
			NtInitializeRegistry,
			NtInitiatePowerAction,
			NtIsSystemResumeAutomatic,
			NtIsUILanguageComitted,
			NtListenPort,
			NtLoadDriver,
			NtLoadKey,
			NtLoadKey2,
			NtLoadKeyEx,
			NtLockFile,
			NtLockProductActivationKeys,
			NtLockRegistryKey,
			NtLockVirtualMemory,
			NtMakePermanentObject,
			NtMakeTemporaryObject,
			NtMapCMFModule,
			NtMapUserPhysicalPages,
			NtModifyBootEntry,
			NtModifyDriverEntry,
			NtNotifyChangeDirectoryFile,
			NtNotifyChangeKey,
			NtNotifyChangeMultipleKeys,
			NtNotifyChangeSession,
			NtOpenEnlistment,
			NtOpenEventPair,
			NtOpenIoCompletion,
			NtOpenJobObject,
			NtOpenKeyEx,
			NtOpenKeyTransacted,
			NtOpenKeyTransactedEx,
			NtOpenKeyedEvent,
			NtOpenMutant,
			NtOpenObjectAuditAlarm,
			NtOpenPrivateNamespace,
			NtOpenProcessToken,
			NtOpenResourceManager,
			NtOpenSemaphore,
			NtOpenSession,
			NtOpenSymbolicLinkObject,
			NtOpenThread,
			NtOpenTimer,
			NtOpenTransaction,
			NtOpenTransactionManager,
			NtPlugPlayControl,
			NtPrePrepareComplete,
			NtPrePrepareEnlistment,
			NtPrepareComplete,
			NtPrepareEnlistment,
			NtPrivilegeCheck,
			NtPrivilegeObjectAuditAlarm,
			NtPrivilegedServiceAuditAlarm,
			NtPropagationComplete,
			NtPropagationFailed,
			NtPulseEvent,
			NtQueryBootEntryOrder,
			NtQueryBootOptions,
			NtQueryDebugFilterState,
			NtQueryDirectoryObject,
			NtQueryDriverEntryOrder,
			NtQueryEaFile,
			NtQueryFullAttributesFile,
			NtQueryInformationAtom,
			NtQueryInformationEnlistment,
			NtQueryInformationJobObject,
			NtQueryInformationPort,
			NtQueryInformationResourceManager,
			NtQueryInformationTransaction,
			NtQueryInformationTransactionManager,
			NtQueryInformationWorkerFactory,
			NtQueryInstallUILanguage,
			NtQueryIntervalProfile,
			NtQueryIoCompletion,
			NtQueryLicenseValue,
			NtQueryMultipleValueKey,
			NtQueryMutant,
			NtQueryOpenSubKeys,
			NtQueryOpenSubKeysEx,
			NtQueryPortInformationProcess,
			NtQueryQuotaInformationFile,
			NtQuerySecurityAttributesToken,
			NtQuerySecurityObject,
			NtQuerySemaphore,
			NtQuerySymbolicLinkObject,
			NtQuerySystemEnvironmentValue,
			NtQuerySystemEnvironmentValueEx,
			NtQuerySystemInformationEx,
			NtQueryTimerResolution,
			NtQueueApcThreadEx,
			NtRaiseException,
			NtRaiseHardError,
			NtReadOnlyEnlistment,
			NtRecoverEnlistment,
			NtRecoverResourceManager,
			NtRecoverTransactionManager,
			NtRegisterProtocolAddressInformation,
			NtRegisterThreadTerminatePort,
			NtReleaseKeyedEvent,
			NtReleaseWorkerFactoryWorker,
			NtRemoveIoCompletionEx,
			NtRemoveProcessDebug,
			NtRenameKey,
			NtRenameTransactionManager,
			NtReplaceKey,
			NtReplacePartitionUnit,
			NtReplyWaitReplyPort,
			NtRequestPort,
			NtResetEvent,
			NtResetWriteWatch,
			NtRestoreKey,
			NtResumeProcess,
			NtRollbackComplete,
			NtRollbackEnlistment,
			NtRollbackTransaction,
			NtRollforwardTransactionManager,
			NtSaveKey,
			NtSaveKeyEx,
			NtSaveMergedKeys,
			NtSecureConnectPort,
			NtSerializeBoot,
			NtSetBootEntryOrder,
			NtSetBootOptions,
			NtSetContextThread,
			NtSetDebugFilterState,
			NtSetDefaultHardErrorPort,
			NtSetDefaultLocale,
			NtSetDefaultUILanguage,
			NtSetDriverEntryOrder,
			NtSetEaFile,
			NtSetHighEventPair,
			NtSetHighWaitLowEventPair,
			NtSetInformationDebugObject,
			NtSetInformationEnlistment,
			NtSetInformationJobObject,
			NtSetInformationKey,
			NtSetInformationResourceManager,
			NtSetInformationToken,
			NtSetInformationTransaction,
			NtSetInformationTransactionManager,
			NtSetInformationWorkerFactory,
			NtSetIntervalProfile,
			NtSetIoCompletion,
			NtSetIoCompletionEx,
			NtSetLdtEntries,
			NtSetLowEventPair,
			NtSetLowWaitHighEventPair,
			NtSetQuotaInformationFile,
			NtSetSecurityObject,
			NtSetSystemEnvironmentValue,
			NtSetSystemEnvironmentValueEx,
			NtSetSystemInformation,
			NtSetSystemPowerState,
			NtSetSystemTime,
			NtSetThreadExecutionState,
			NtSetTimerEx,
			NtSetTimerResolution,
			NtSetUuidSeed,
			NtSetVolumeInformationFile,
			NtShutdownSystem,
			NtShutdownWorkerFactory,
			NtSignalAndWaitForSingleObject,
			NtSinglePhaseReject,
			NtStartProfile,
			NtStopProfile,
			NtSuspendProcess,
			NtSuspendThread,
			NtSystemDebugControl,
			NtTerminateJobObject,
			NtTestAlert,
			NtThawRegistry,
			NtThawTransactions,
			NtTraceControl,
			NtTranslateFilePath,
			NtUmsThreadYield,
			NtUnloadDriver,
			NtUnloadKey,
			NtUnloadKey2,
			NtUnloadKeyEx,
			NtUnlockFile,
			NtUnlockVirtualMemory,
			NtVdmControl,
			NtWaitForDebugEvent,
			NtWaitForKeyedEvent,
			NtWaitForWorkViaWorkerFactory,
			NtWaitHighEventPair,
			NtWaitLowEventPair,
			NtWorkerFactoryWorkerReady


		};

	};

	//ntoskrnl信息
	EXTERN PEPROCESS			SystemProcess;
	EXTERN PEPROCESS			CsrssProcess;
	EXTERN PCHAR				ImageBaseCopyNt;
	EXTERN PCHAR				ImageBaseRunNt;
	EXTERN ULONG				SizeOfNtImage;
	EXTERN PEPROCESS			PsIdleProcess;
	EXTERN WCHAR				NtFullName[];
	EXTERN USHORT				OsVersion;
	EXTERN PLIST_ENTRY			PsLoadedModuleList;

	//未导出
	EXTERN POBJECT_TYPE			*ObTypeIndexTable;
	EXTERN POBJECT_DIRECTORY	ObpRootDirectoryObject;
	EXTERN PHANDLE_TABLE		*PPspCidTable;
	EXTERN PKSERVICE_TABLE_DESCRIPTOR		KeServiceDescriptorTable;
	EXTERN PKSERVICE_TABLE_DESCRIPTOR		KeServiceDescriptorTableShadow;
	EXTERN FunKiInsertQueueApc		KiInsertQueueApc;
	EXTERN FunPspTerminateThreadByPointer		PspTerminateThreadByPointer;
	EXTERN FunNtUserBuildHwndList			NtUserBuildHwndList;
	EXTERN FunNtUserQueryWindow			NtUserQueryWindow;
	EXTERN FunPsGetNextProcess		PsGetNextProcess;
	EXTERN FunNtTerminateProcess		NtTerminateProcess;
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