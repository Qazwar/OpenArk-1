#pragma once




//BOOLEAN  ExEnumHandleCallBack(
//	IN PHANDLE_TABLE_ENTRY HandleTableEntry,
//	IN HANDLE Handle,
//	IN PVOID EnumParameter
//);

BOOLEAN ExEnumHandleCallBack(IN PHANDLE_TABLE_ENTRY HandleTableEntry, IN HANDLE Handle, OUT PArkHandleInfo HandleInfo);





