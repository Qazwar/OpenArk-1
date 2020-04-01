#include "kark.h"


PObInfo ObpInfo;


void GetObInfo(PVOID pObject)
{
	POBJECT_HEADER pObHdr = OBJECT_TO_OBJECT_HEADER(pObject);

	__try
	{

		ProbeForWrite(ObpInfo, sizeof(ObInfo), 1);

		if (pObHdr->TypeIndex == NT::ArrObjectType[ObjectType::DirectoryType].TypeIndex)
			ObpInfo->IsDirectory = true;
		else if (pObHdr->TypeIndex == NT::ArrObjectType[ObjectType::SymbolinkType].TypeIndex) {
			ObpInfo->IsSymLink = true;
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
			return false;
		RtlZeroMemory(pOutData, cbOutData);
		
		ObpInfo = pOutData;
		RecurSionGetObInfo(NT::ObpRootDirectoryObject);

	}
	__except (1)
	{
		DBGERRINFO;
		return false;
	}

	return true;

}
 PVOID ObpDecodeObject(PVOID Object)
{
	return (PVOID)((ULONG_PTR)Object & ~OBJ_HANDLE_ATTRIBUTES);
}

/**
 * Queries the name of a file object.
 *
 * \param FileObject A pointer to a file object.
 * \param Buffer The buffer in which the object name will be stored.
 * \param BufferLength The number of bytes available in \a Buffer.
 * \param ReturnLength A variable which receives the number of bytes required to be available in
 * \a Buffer.
 */
BOOLEAN ObQueryNameFileObject(
	_In_ PFILE_OBJECT FileObject,
	_Out_ PWSTR Buffer,
	_In_ ULONG BufferLength,
	_Out_ PULONG ReturnLength
)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG returnLength;
	PCHAR objectName;
	ULONG usedLength;
	ULONG subNameLength;
	PFILE_OBJECT relatedFileObject;
	UNICODE_STRING dosName;
	PAGED_CODE();

	objectName = (PCHAR)Buffer;
	usedLength = 0;

	if (FileObject->DeviceObject
		&& KeAreAllApcsDisabled() == false
		)
	{
		/*
		IoVolumeDeviceToDosName -> "c:"
		*/
		if (NT_SUCCESS(IoVolumeDeviceToDosName(FileObject->DeviceObject, &dosName))
			&& dosName.Buffer)
		{
			memcpy(objectName, dosName.Buffer, dosName.Length);
			ExFreePool(dosName.Buffer);
			usedLength += dosName.Length;
		}
		else
		{
			*ReturnLength = 0;
			return false;
		}

		objectName += dosName.Length;
		
	}

	usedLength += FileObject->FileName.Length;
	// Check if we have enough space to write the whole thing.
	if (usedLength > BufferLength)
	{
		*ReturnLength = usedLength;
		return false;
	}
	memcpy(objectName, FileObject->FileName.Buffer, FileObject->FileName.Length);
	// Pass the return length back.
	Buffer[usedLength / 2] = 0;

	*ReturnLength = usedLength;

	return true;
}