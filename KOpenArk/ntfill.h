#ifndef NTFILL_H
#define NTFILL_H


#ifdef __cplusplus
extern "C" {
#endif


NTKERNELAPI ULONG			NtBuildNumber;





NTKERNELAPI
POBJECT_HEADER_NAME_INFO
ObQueryNameInfo(
	__out PVOID Object
);






NTKERNELAPI
NTSTATUS
ObReferenceObjectByName(
	__in PUNICODE_STRING ObjectName,
	__in ULONG Attributes,
	__in_opt PACCESS_STATE AccessState,
	__in_opt ACCESS_MASK DesiredAccess,
	__in POBJECT_TYPE ObjectType,
	__in KPROCESSOR_MODE AccessMode,
	__inout_opt PVOID ParseContext,
	__out PVOID *Object
);


NTKERNELAPI
NTSTATUS
NTAPI
PsAcquireProcessExitSynchronization(
	_In_ PEPROCESS Process
);

NTKERNELAPI
VOID
NTAPI
PsReleaseProcessExitSynchronization(
	_In_ PEPROCESS Process
);


#ifdef __cplusplus
}
#endif




































#endif // !NTFILL_H

