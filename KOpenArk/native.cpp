#include "kark.h"



/**
 * Enumerates all open handles.
 *
 * \param Handles A variable which receives a pointer to a structure containing information about
 * all opened handles. You must free the structure using PhFree() when you no longer need it.
 *
 * \retval STATUS_INSUFFICIENT_RESOURCES The handle information returned by the kernel is too large.
 */
NTSTATUS ArkEnumHandles(
	_Out_ PSYSTEM_HANDLE_INFORMATION *Handles
)
{
	static ULONG initialBufferSize = 0x4000;
	NTSTATUS status;
	PVOID buffer;
	ULONG bufferSize;
	PKTHREAD thread;
	char previousMode;

	bufferSize = initialBufferSize;
	buffer = ExAllocatePool(PagedPool,bufferSize);

	thread = KeGetCurrentThread();
	previousMode = KeToKernekModel(thread);

	while ((status = NtQuerySystemInformation(
		SystemHandleInformation,
		buffer,
		bufferSize,
		NULL
	)) == STATUS_INFO_LENGTH_MISMATCH)
	{
		ExFreePool(buffer);
		bufferSize *= 2;


		buffer = ExAllocatePool(PagedPool, bufferSize);
	}

	if (!NT_SUCCESS(status))
	{
		ExFreePool(buffer);
		return status;
	}
	
	*Handles = (PSYSTEM_HANDLE_INFORMATION)buffer;
	KeResumePreviousMode(thread, previousMode);
	return status;
}