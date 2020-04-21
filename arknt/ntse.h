#pragma once






// begin_ntosp
typedef struct _AUX_ACCESS_DATA {
	PPRIVILEGE_SET PrivilegesUsed;
	GENERIC_MAPPING GenericMapping;
	ACCESS_MASK AccessesToAudit;
	ACCESS_MASK MaximumAuditMask;
} AUX_ACCESS_DATA, *PAUX_ACCESS_DATA;


EXTERN_C
{
	NTSYSAPI
	NTSTATUS
SeCreateAccessState(
	IN PACCESS_STATE AccessState,
	IN PAUX_ACCESS_DATA AuxData,
	IN ACCESS_MASK DesiredAccess,
	IN PGENERIC_MAPPING GenericMapping OPTIONAL
	);


}
