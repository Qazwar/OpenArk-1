#include "kark.h"

BOOLEAN ExEnumHandleCallBack(IN PHANDLE_TABLE_ENTRY HandleTableEntry, IN HANDLE Handle, OUT PArkHandleInfo HandleInfo)
{
	POBJECT_HEADER objectHeader;
	POBJECT_TYPE objectType;
	ULONG handleCnt = HandleInfo->HandleCnt;
	ULONG requsetLen;
	NTSTATUS st;

	objectHeader = (POBJECT_HEADER)(HandleTableEntry->Value & ~OBJ_HANDLE_ATTRIBUTES);
	objectType = NT::ObTypeIndexTable[objectHeader->TypeIndex];
	HandleInfo[handleCnt].Handle = Handle;
	HandleInfo[handleCnt].Object = &objectHeader->Body;
	HandleInfo[handleCnt].RefreceCount = objectHeader->PointerCount;
	HandleInfo[handleCnt].TypeIndex = objectHeader->TypeIndex;

	memcpy(HandleInfo[handleCnt].TypeName,
		objectType->Name.Buffer, objectType->Name.Length);
	HandleInfo[handleCnt].TypeName[objectType->Name.Length / 2] = 0;
	
	st = ObQueryNameString(&objectHeader->Body, 
		(POBJECT_NAME_INFORMATION)&HandleInfo[handleCnt].HandleName, MAX_PATH, &requsetLen);

	if (NT_SUCCESS(st))
	{
		memcpy(HandleInfo[handleCnt].HandleName,
			((POBJECT_NAME_INFORMATION)&HandleInfo[handleCnt].HandleName)->Name.Buffer,
			((POBJECT_NAME_INFORMATION)&HandleInfo[handleCnt].HandleName)->Name.Length);
	}

	return false;
}
