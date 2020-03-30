#include "ob.h"
#include "struct.h"
#include "tool.h"

ob::ob()
{
}


ob::~ob()
{
}


PObInfo ObpInfo;


void GetObInfo(PVOID pObject)
{
	POBJECT_HEADER pObHdr = OBJECT_TO_OBJECT_HEADER(pObject);

	__try
	{

		ProbeForWrite(ObpInfo, sizeof(ObInfo), 1);

		if (pObHdr->TypeIndex == NT::ArrObjectType[ObjectType::DirectoryType].TypeIndex)
			ObpInfo->IsDirectory = TRUE;
		else if (pObHdr->TypeIndex == NT::ArrObjectType[ObjectType::SymbolinkType].TypeIndex) {
			ObpInfo->IsSymLink = TRUE;
			UNICODE_STRING symbolName;

			symbolName = ((POBJECT_SYMBOLIC_LINK)pObject)->LinkTarget;
			UnicodeString2Wstr(ObpInfo->SymLinkName, &symbolName);//拷贝链接对象名
		}

		POBJECT_HEADER_NAME_INFO pObjName = ObQueryNameInfo(pObject);
		if (pObjName) {
			UnicodeString2Wstr(ObpInfo->ObjName, &pObjName->Name);//拷贝对象名
		}

		UnicodeString2Wstr(ObpInfo->TypeName, &NT::ObTypeIndexTable[pObHdr->TypeIndex]->Name);//拷贝类型名
		ObpInfo->ObjAddr = (ULONG_PTR)pObject;
	}
	__except (1)
	{
		DbgPrint("GetObInfo");
	}
}

static void RecurSionGetObInfo(POBJECT_DIRECTORY pDirObj)
{
	__try
	{

		PObInfo pDirInfo = ObpInfo;
		GetObInfo(pDirObj);
		ObpInfo++;

		for (size_t i = 0; i < NUMBER_HASH_BUCKETS; i++)
		{
			POBJECT_DIRECTORY_ENTRY pCurDirEntry = pDirObj->HashBuckets[i];
			while (pCurDirEntry)
			{
				if (OBJECT_TO_OBJECT_HEADER(pCurDirEntry->Object)->TypeIndex == NT::ArrObjectType[ObjectType::DirectoryType].TypeIndex) {
					RecurSionGetObInfo((POBJECT_DIRECTORY)pCurDirEntry->Object);
				}else{
					GetObInfo(pCurDirEntry->Object);
					ObpInfo++;
				}
				pDirInfo->SubItems++;
				pCurDirEntry = pCurDirEntry->ChainLink;
			}

		}
	}
	__except (1)
	{
		DBGERRINFO
	}

}

BOOLEAN  ArkGetObjectDirectoryInfo(PCHAR pIndata, ULONG cbInData, PObInfo pOutData, ULONG cbOutData)
{
	
	__try
	{
		ProbeForWrite(pOutData, cbOutData, 1);
		if (!NT::ObpRootDirectoryObject || !NT::ArrObjectType[ObjectType::DirectoryType].TypeIndex || !NT::ArrObjectType[ObjectType::SymbolinkType].TypeIndex)
			return FALSE;
		RtlZeroMemory(pOutData, cbOutData);
		
		ObpInfo = pOutData;
		RecurSionGetObInfo(NT::ObpRootDirectoryObject);

	}
	__except (1)
	{
		DBGERRINFO;
		return FALSE;
	}

	return TRUE;

}