#pragma once
#include <ntdef.h>
#include "common.h"
class ob
{
public:
	ob();
	~ob();
};

BOOLEAN  ArkGetObjectDirectoryInfo(PCHAR pIndata, ULONG cbInData, PObInfo pOutData, ULONG cbOutData);



struct ObjectType
{
	enum 
	{
		DirectoryType,
		SymbolinkType,
		FileType,
		LastObjectType
	};

	int TypeIndex;
	POBJECT_TYPE ObjectType;
};