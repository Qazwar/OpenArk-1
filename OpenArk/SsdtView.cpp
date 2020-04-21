#include "SsdtView.h"
#include "common.h"
#include "arknamespace.h"
#include "qtwrapper.h"


PCHAR SsdtView::mSsdtFunName[] = {
"NtMapUserPhysicalPagesScatter",
"NtWaitForSingleObject",
"NtCallbackReturn",
"NtReadFile",
"NtDeviceIoControlFile",
"NtWriteFile",
"NtRemoveIoCompletion",
"NtReleaseSemaphore",
"NtReplyWaitReceivePort",
"NtReplyPort",
"NtSetInformationThread",
"NtSetEvent",
"NtClose",
"NtQueryObject",
"NtQueryInformationFile",
"NtOpenKey",
"NtEnumerateValueKey",
"NtFindAtom",
"NtQueryDefaultLocale",
"NtQueryKey",
"NtQueryValueKey",
"NtAllocateVirtualMemory",
"NtQueryInformationProcess",
"NtWaitForMultipleObjects32",
"NtWriteFileGather",
"NtSetInformationProcess",
"NtCreateKey",
"NtFreeVirtualMemory",
"NtImpersonateClientOfPort",
"NtReleaseMutant",
"NtQueryInformationToken",
"NtRequestWaitReplyPort",
"NtQueryVirtualMemory",
"NtOpenThreadToken",
"NtQueryInformationThread",
"NtOpenProcess",
"NtSetInformationFile",
"NtMapViewOfSection",
"NtAccessCheckAndAuditAlarm",
"NtUnmapViewOfSection",
"NtReplyWaitReceivePortEx",
"NtTerminateProcess",
"NtSetEventBoostPriority",
"NtReadFileScatter",
"NtOpenThreadTokenEx",
"NtOpenProcessTokenEx",
"NtQueryPerformanceCounter",
"NtEnumerateKey",
"NtOpenFile",
"NtDelayExecution",
"NtQueryDirectoryFile",
"NtQuerySystemInformation",
"NtOpenSection",
"NtQueryTimer",
"NtFsControlFile",
"NtWriteVirtualMemory",
"NtCloseObjectAuditAlarm",
"NtDuplicateObject",
"NtQueryAttributesFile",
"NtClearEvent",
"NtReadVirtualMemory",
"NtOpenEvent",
"NtAdjustPrivilegesToken",
"NtDuplicateToken",
"NtContinue",
"NtQueryDefaultUILanguage",
"NtQueueApcThread",
"NtYieldExecution",
"NtAddAtom",
"NtCreateEvent",
"NtQueryVolumeInformationFile",
"NtCreateSection",
"NtFlushBuffersFile",
"NtApphelpCacheControl",
"NtCreateProcessEx",
"NtCreateThread",
"NtIsProcessInJob",
"NtProtectVirtualMemory",
"NtQuerySection",
"NtResumeThread",
"NtTerminateThread",
"NtReadRequestData",
"NtCreateFile",
"NtQueryEvent",
"NtWriteRequestData",
"NtOpenDirectoryObject",
"NtAccessCheckByTypeAndAuditAlarm",
"NtQuerySystemTime",
"NtWaitForMultipleObjects",
"NtSetInformationObject",
"NtCancelIoFile",
"NtTraceEvent",
"NtPowerInformation",
"NtSetValueKey",
"NtCancelTimer",
"NtSetTimer",
"NtAcceptConnectPort",
"NtAccessCheck",
"NtAccessCheckByType",
"NtAccessCheckByTypeResultList",
"NtAccessCheckByTypeResultListAndAuditAlarm",
"NtAccessCheckByTypeResultListAndAuditAlarmByHandle",
"NtAddBootEntry",
"NtAddDriverEntry",
"NtAdjustGroupsToken",
"NtAlertResumeThread",
"NtAlertThread",
"NtAllocateLocallyUniqueId",
"NtAllocateReserveObject",
"NtAllocateUserPhysicalPages",
"NtAllocateUuids",
"NtAlpcAcceptConnectPort",
"NtAlpcCancelMessage",
"NtAlpcConnectPort",
"NtAlpcCreatePort",
"NtAlpcCreatePortSection",
"NtAlpcCreateResourceReserve",
"NtAlpcCreateSectionView",
"NtAlpcCreateSecurityContext",
"NtAlpcDeletePortSection",
"NtAlpcDeleteResourceReserve",
"NtAlpcDeleteSectionView",
"NtAlpcDeleteSecurityContext",
"NtAlpcDisconnectPort",
"NtAlpcImpersonateClientOfPort",
"NtAlpcOpenSenderProcess",
"NtAlpcOpenSenderThread",
"NtAlpcQueryInformation",
"NtAlpcQueryInformationMessage",
"NtAlpcRevokeSecurityContext",
"NtAlpcSendWaitReceivePort",
"NtAlpcSetInformation",
"NtAreMappedFilesTheSame",
"NtAssignProcessToJobObject",
"NtCancelIoFileEx",
"NtCancelSynchronousIoFile",
"NtCommitComplete",
"NtCommitEnlistment",
"NtCommitTransaction",
"NtCompactKeys",
"NtCompareTokens",
"NtCompleteConnectPort",
"NtCompressKey",
"NtConnectPort",
"NtCreateDebugObject",
"NtCreateDirectoryObject",
"NtCreateEnlistment",
"NtCreateEventPair",
"NtCreateIoCompletion",
"NtCreateJobObject",
"NtCreateJobSet",
"NtCreateKeyTransacte",
"NtCreateKeyedEvent",
"NtCreateMailslotFile",
"NtCreateMutant",
"NtCreateNamedPipeFile",
"NtCreatePagingFile",
"NtCreatePort",
"NtCreatePrivateNamespace",
"NtCreateProcess",
"NtCreateProfile",
"NtCreateProfileEx",
"NtCreateResourceManager",
"NtCreateSemaphore",
"NtCreateSymbolicLinkObject",
"NtCreateThreadEx",
"NtCreateTimer",
"NtCreateToken",
"NtCreateTransaction",
"NtCreateTransactionManager",
"NtCreateUserProcess",
"NtCreateWaitablePort",
"NtCreateWorkerFactory",
"NtDebugActiveProcess",
"NtDebugContinue",
"NtDeleteAtom",
"NtDeleteBootEntry",
"NtDeleteDriverEntry",
"NtDeleteFile",
"NtDeleteKey",
"NtDeleteObjectAuditAlarm",
"NtDeletePrivateNamespace",
"NtDeleteValueKey",
"NtDisableLastKnownGood",
"NtDisplayString",
"NtDrawText",
"NtEnableLastKnownGood",
"NtEnumerateBootEntries",
"NtEnumerateDriverEntries",
"NtEnumerateSystemEnvironmentValuesEx",
"NtEnumerateTransactionObject",
"NtExtendSection",
"NtFilterToken",
"NtFlushInstallUILanguage",
"NtFlushInstructionCache",
"NtFlushKey",
"NtFlushProcessWriteBuffers",
"NtFlushVirtualMemory",
"NtFlushWriteBuffer",
"NtFreeUserPhysicalPages",
"NtFreezeRegistry",
"NtFreezeTransactions",
"NtGetContextThread",
"NtGetCurrentProcessorNumber",
"NtGetDevicePowerState",
"NtGetMUIRegistryInfo",
"NtGetNextProcess",
"NtGetNextThread",
"NtGetNlsSectionPtr",
"NtGetNotificationResourceManager",
"NtGetPlugPlayEvent",
"NtGetWriteWatch",
"NtImpersonateAnonymousToken",
"NtImpersonateThread",
"NtInitializeNlsFiles",
"NtInitializeRegistry",
"NtInitiatePowerAction",
"NtIsSystemResumeAutomatic",
"NtIsUILanguageComitted",
"NtListenPort",
"NtLoadDriver",
"NtLoadKey",
"NtLoadKey2",
"NtLoadKeyEx",
"NtLockFile",
"NtLockProductActivationKeys",
"NtLockRegistryKey",
"NtLockVirtualMemory",
"NtMakePermanentObject",
"NtMakeTemporaryObject",
"NtMapCMFModule",
"NtMapUserPhysicalPages",
"NtModifyBootEntry",
"NtModifyDriverEntry",
"NtNotifyChangeDirectoryFile",
"NtNotifyChangeKey",
"NtNotifyChangeMultipleKeys",
"NtNotifyChangeSession",
"NtOpenEnlistment",
"NtOpenEventPair",
"NtOpenIoCompletion",
"NtOpenJobObject",
"NtOpenKeyEx",
"NtOpenKeyTransacted",
"NtOpenKeyTransactedEx",
"NtOpenKeyedEvent",
"NtOpenMutant",
"NtOpenObjectAuditAlarm",
"NtOpenPrivateNamespace",
"NtOpenProcessToken",
"NtOpenResourceManager",
"NtOpenSemaphore",
"NtOpenSession",
"NtOpenSymbolicLinkObject",
"NtOpenThread",
"NtOpenTimer",
"NtOpenTransaction",
"NtOpenTransactionManager",
"NtPlugPlayControl",
"NtPrePrepareComplete",
"NtPrePrepareEnlistment",
"NtPrepareComplete",
"NtPrepareEnlistment",
"NtPrivilegeCheck",
"NtPrivilegeObjectAuditAlarm",
"NtPrivilegedServiceAuditAlarm",
"NtPropagationComplete",
"NtPropagationFailed",
"NtPulseEvent",
"NtQueryBootEntryOrder",
"NtQueryBootOptions",
"NtQueryDebugFilterState",
"NtQueryDirectoryObject",
"NtQueryDriverEntryOrder",
"NtQueryEaFile",
"NtQueryFullAttributesFile",
"NtQueryInformationAtom",
"NtQueryInformationEnlistment",
"NtQueryInformationJobObject",
"NtQueryInformationPort",
"NtQueryInformationResourceManager",
"NtQueryInformationTransaction",
"NtQueryInformationTransactionManager",
"NtQueryInformationWorkerFactory",
"NtQueryInstallUILanguage",
"NtQueryIntervalProfile",
"NtQueryIoCompletion",
"NtQueryLicenseValue",
"NtQueryMultipleValueKey",
"NtQueryMutant",
"NtQueryOpenSubKeys",
"NtQueryOpenSubKeysEx",
"NtQueryPortInformationProcess",
"NtQueryQuotaInformationFile",
"NtQuerySecurityAttributesToken",
"NtQuerySecurityObject",
"NtQuerySemaphore",
"NtQuerySymbolicLinkObject",
"NtQuerySystemEnvironmentValue",
"NtQuerySystemEnvironmentValueEx",
"NtQuerySystemInformationEx",
"NtQueryTimerResolution",
"NtQueueApcThreadEx",
"NtRaiseException",
"NtRaiseHardError",
"NtReadOnlyEnlistment",
"NtRecoverEnlistment",
"NtRecoverResourceManager",
"NtRecoverTransactionManager",
"NtRegisterProtocolAddressInformation",
"NtRegisterThreadTerminatePort",
"NtReleaseKeyedEvent",
"NtReleaseWorkerFactoryWorker",
"NtRemoveIoCompletionEx",
"NtRemoveProcessDebug",
"NtRenameKey",
"NtRenameTransactionManager",
"NtReplaceKey",
"NtReplacePartitionUnit",
"NtReplyWaitReplyPort",
"NtRequestPort",
"NtResetEvent",
"NtResetWriteWatch",
"NtRestoreKey",
"NtResumeProcess",
"NtRollbackComplete",
"NtRollbackEnlistment",
"NtRollbackTransaction",
"NtRollforwardTransactionManager",
"NtSaveKey",
"NtSaveKeyEx",
"NtSaveMergedKeys",
"NtSecureConnectPort",
"NtSerializeBoot",
"NtSetBootEntryOrder",
"NtSetBootOptions",
"NtSetContextThread",
"NtSetDebugFilterState",
"NtSetDefaultHardErrorPort",
"NtSetDefaultLocale",
"NtSetDefaultUILanguage",
"NtSetDriverEntryOrder",
"NtSetEaFile",
"NtSetHighEventPair",
"NtSetHighWaitLowEventPair",
"NtSetInformationDebugObject",
"NtSetInformationEnlistment",
"NtSetInformationJobObject",
"NtSetInformationKey",
"NtSetInformationResourceManager",
"NtSetInformationToken",
"NtSetInformationTransaction",
"NtSetInformationTransactionManager",
"NtSetInformationWorkerFactory",
"NtSetIntervalProfile",
"NtSetIoCompletion",
"NtSetIoCompletionEx",
"NtSetLdtEntries",
"NtSetLowEventPair",
"NtSetLowWaitHighEventPair",
"NtSetQuotaInformationFile",
"NtSetSecurityObject",
"NtSetSystemEnvironmentValue",
"NtSetSystemEnvironmentValueEx",
"NtSetSystemInformation",
"NtSetSystemPowerState",
"NtSetSystemTime",
"NtSetThreadExecutionState",
"NtSetTimerEx",
"NtSetTimerResolution",
"NtSetUuidSeed",
"NtSetVolumeInformationFile",
"NtShutdownSystem",
"NtShutdownWorkerFactory",
"NtSignalAndWaitForSingleObject",
"NtSinglePhaseReject",
"NtStartProfile",
"NtStopProfile",
"NtSuspendProcess",
"NtSuspendThread",
"NtSystemDebugControl",
"NtTerminateJobObject",
"NtTestAlert",
"NtThawRegistry",
"NtThawTransactions",
"NtTraceControl",
"NtTranslateFilePath",
"NtUmsThreadYield",
"NtUnloadDriver",
"NtUnloadKey",
"NtUnloadKey2",
"NtUnloadKeyEx",
"NtUnlockFile",
"NtUnlockVirtualMemory",
"NtVdmControl",
"NtWaitForDebugEvent",
"NtWaitForKeyedEvent",
"NtWaitForWorkViaWorkerFactory",
"NtWaitHighEventPair",
"NtWaitLowEventPair",
"NtWorkerFactoryWorkerReady"


};

SsdtView::SsdtView(QWidget *parent) :StdDialog(parent)
{

	InitView();
}


SsdtView::~SsdtView()
{
}

void SsdtView::InitView()
{
	mSourceModel->setHorizontalHeaderLabels(QStringList() << tr("serial num") << tr("fun name") << tr("cur funaddr")
		<< "Hook" << tr("original funaddr") << tr("in module"));
	mSortModel->mColSortFunctions.resize(Col::LastCol);

	mSortModel->mColSortFunctions[Col::CurFunAddr] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::FunName] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::Hook] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::InMod] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::OriginalAddr] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::Serial] = MySortModel::SortBy::AsInt;

	SetContextMenu();



}

void SsdtView::SetContextMenu()
{
	mMenu.addAction(tr("refresh"), this, &SsdtView::OnRefresh);

	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, &ProcessMgr::customContextMenuRequested, this, [=](const QPoint &pos)
		{
			mMenu.exec(QCursor::pos());

		}
	);


}

void SsdtView::OnRefresh()
{
	static BOOLEAN refrshed = 0;

	if (refrshed) return;
	refrshed = 1;

	PVOID *funAddress = (PVOID *)Ark::Buffer;


	ParamInfo param;
	param.pOutData = (PCHAR)funAddress;
	param.cbOutData = SIZE4M;
	param.FunIdx = DrvCall::GetAllSsdtFunAddr;

	auto result = OpenArk::IoCallDriver(param);
	if (result == false) {
	return;
	}

	//Çå³ýÄÚÈÝ
	mSourceModel->removeRows(0, mSourceModel->rowCount());



	int i = 0;
	for (; *funAddress != 0; i++, funAddress++)
	{
		mSourceModel->setItem(i, Col::CurFunAddr, MakeItem(*funAddress));
		mSourceModel->setItem(i, Col::FunName, MakeItem(mSsdtFunName[i]));
		mSourceModel->setItem(i, Col::Hook, MakeItem(QString()));
		mSourceModel->setItem(i, Col::InMod, MakeItem("C:\\Windows\\system32\\ntoskrnl.exe"));
		mSourceModel->setItem(i, Col::Serial, MakeItem(i));
	}

	QString msg = QString(tr("SSDT function[%1],beHooked fun: %2")).arg(i).arg(0);
	Ark::Instance->ShowMessage(msg);
}



void SsdtView::OnNouse()
{
}



