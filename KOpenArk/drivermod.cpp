#include "kark.h"
#include <string.h>
#pragma warning(disable : 4242)

drivermod::drivermod()
{
}


drivermod::~drivermod()
{
}

BOOLEAN ArkGetDriverModsInfo(PVOID, PVOID, ArkDriverModInfo * driverModInfo, ULONG BufSize)
{

	PLDR_DATA_TABLE_ENTRY nextMod = (PLDR_DATA_TABLE_ENTRY)NT::PsLoadedModuleList->Flink;
	int numOfMod = 0;
	
	int i = 0;
	for (; (PLDR_DATA_TABLE_ENTRY)NT::PsLoadedModuleList != nextMod;
		nextMod = (PLDR_DATA_TABLE_ENTRY)nextMod->InLoadOrderLinks.Flink,
		i++
		)
	{
		
		driverModInfo->Mods[i].RegionBase = nextMod->DllBase;
		driverModInfo->Mods[i].RegionSize = nextMod->SizeOfImage;
		driverModInfo->Mods[i].LoadOrder = i;

		if(nextMod->FullDllName.Buffer)
		memcpy(driverModInfo->Mods[i].DriverPath, nextMod->FullDllName.Buffer, nextMod->FullDllName.Length);//模块路径
	}
	driverModInfo->NumberOfMods = i;//模块总数

	ArkGetDriversInfo(0, 0, driverModInfo, BufSize);
	
	return true;
	
}


struct MyKeyBaseInfo
{
	KEY_BASIC_INFORMATION  KeyBaseInfo;
	WCHAR Buffer[MAX_PATH];
};


BOOLEAN ArkGetDriversInfo(PVOID, PVOID, ArkDriverModInfo * driverModInfo, ULONG BufSize)
{
	
	POBJECT_DIRECTORY curDirObject;;
	int dirverIndex = NT::ArrObjectType[ObjectType::DriverType].TypeIndex;
	HANDLE handle;
	OBJECT_ATTRIBUTES objAttri;
	UNICODE_STRING objName;
	
	RtlInitUnicodeString(&objName, L"\\Driver");
	InitializeObjectAttributes(&objAttri, &objName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 0, 0);
	ZwOpenDirectoryObject(&handle, DIRECTORY_QUERY, &objAttri);
	ObReferenceObjectByHandle(handle, 0, 0, KernelMode, (PVOID*)&curDirObject, 0);
	ObDereferenceObject(curDirObject);
	ZwClose(handle);

	for (size_t i = 0; i < NUMBER_HASH_BUCKETS; i++)
	{
		POBJECT_DIRECTORY_ENTRY curDirEntry = curDirObject->HashBuckets[i];
		while (curDirEntry)
		{
			if (OBJECT_TO_OBJECT_HEADER(curDirEntry->Object)->TypeIndex == dirverIndex) 
			{//是驱动对象
				PDRIVER_OBJECT driverObject = (PDRIVER_OBJECT)curDirEntry->Object;
				for (ULONG i = 0; i < driverModInfo->NumberOfMods; i++)
				{
					if (driverModInfo->Mods[i].RegionBase == driverObject->DriverStart)
					{//找到对应模块

						//拷贝驱动名
						PWSTR driverName;
						int driverNameLen;
						driverName = ArkGetFileNameFromUnicodeString(driverObject->DriverName);
						if (driverName)
						{
							driverNameLen = driverObject->DriverName.Length - (driverName - driverObject->DriverName.Buffer) * 2;
							__try
							{
								memcpy(driverModInfo->Mods[i].DriverName, driverName, driverNameLen);

							}
							__except (1)
							{
								DbgBreakPoint();
							}
						}
						//驱动对象
						driverModInfo->Mods[i].DriverObject = driverObject;
						//服务名
						PWCHAR modName;
						modName = ArkGetFileNameFromFullPath(driverModInfo->Mods[i].DriverPath);
						if(modName)
						{
							ArkGetServiceName(modName , driverModInfo->Mods[i].ServiceName);
						}
					}
				}
			}
			curDirEntry = curDirEntry->ChainLink;
		}
	}



	RtlInitUnicodeString(&objName, L"\\FileSystem");
	InitializeObjectAttributes(&objAttri, &objName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 0, 0);
	ZwOpenDirectoryObject(&handle, DIRECTORY_QUERY, &objAttri);
	ObReferenceObjectByHandle(handle, 0, 0, KernelMode, (PVOID*)&curDirObject, 0);
	ObDereferenceObject(curDirObject);
	ZwClose(handle);

	for (size_t i = 0; i < NUMBER_HASH_BUCKETS; i++)
	{
		POBJECT_DIRECTORY_ENTRY curDirEntry = curDirObject->HashBuckets[i];
		while (curDirEntry)
		{
			if (OBJECT_TO_OBJECT_HEADER(curDirEntry->Object)->TypeIndex == dirverIndex)
			{//是驱动对象
				PDRIVER_OBJECT driverObject = (PDRIVER_OBJECT)curDirEntry->Object;
				for (ULONG i = 0; i < driverModInfo->NumberOfMods; i++)
				{
					if (driverModInfo->Mods[i].RegionBase == driverObject->DriverStart)
					{//找到对应模块

						//拷贝驱动名
						PWSTR driverName;
						int driverNameLen;
						driverName = ArkGetFileNameFromUnicodeString(driverObject->DriverName);
						if (driverName)
						{
							driverNameLen = driverObject->DriverName.Length - (driverName - driverObject->DriverName.Buffer) * 2;
							__try
							{
								memcpy(driverModInfo->Mods[i].DriverName, driverName, driverNameLen);

							}
							__except (1)
							{
								DbgBreakPoint();
							}
						}
						//驱动对象
						driverModInfo->Mods[i].DriverObject = driverObject;
						//服务名
						PWCHAR modName;
						modName = ArkGetFileNameFromFullPath(driverModInfo->Mods[i].DriverPath);
						if (modName)
						{
							ArkGetServiceName(modName, driverModInfo->Mods[i].ServiceName);
						}
					}
				}
			}

			curDirEntry = curDirEntry->ChainLink;
		}

	}

	return true;
}

BOOLEAN ArkGetServiceName(PWCHAR ModName, PWCHAR ReturnName)
{
	HANDLE serviceKey;
	HANDLE subKey;
	UNICODE_STRING keyPath;
	NTSTATUS st;
	PKEY_NAME_INFORMATION keyName;
	ULONG returnSize;
	MyKeyBaseInfo *keyBaseInfos = 0;
	OBJECT_ATTRIBUTES objAttri;

	RtlInitUnicodeString(&keyPath, L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\Services");

	InitializeObjectAttributes(
		&objAttri,
		&keyPath,
		OBJ_CASE_INSENSITIVE ,
		NULL,
		NULL);

	st = ZwOpenKey(&serviceKey, READ_CONTROL | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &objAttri);
	if (!NT_SUCCESS(st))
	{
		return false;
	}

	//分配查询键完整信息所需的缓冲区
	//调用ZwQueryKey，为了获取KEY_FULL_INFORMATION数据
	PKEY_FULL_INFORMATION pkfi = (PKEY_FULL_INFORMATION)ExAllocatePool(PagedPool, PAGE_SIZE);


	st = ZwQueryKey(serviceKey, KeyFullInformation, pkfi, PAGE_SIZE, &returnSize);
	if (NT_SUCCESS(st))
	{
		//分配保存服务名的unicode字符串
		keyBaseInfos = (MyKeyBaseInfo*)ExAllocatePool(PagedPool, pkfi->SubKeys * PAGE_SIZE);
		RtlZeroMemory(keyBaseInfos, pkfi->SubKeys * PAGE_SIZE);
		for (ULONG i = 0; i < pkfi->SubKeys; i++)
		{
			//调用ZwEnumerateKey，为了获取KEY_BASIC_INFORMATION数据                             
			ZwEnumerateKey(serviceKey, i, KeyBasicInformation, &keyBaseInfos[i], sizeof(MyKeyBaseInfo), &returnSize);
		}

		PKEY_VALUE_PARTIAL_INFORMATION valueInfo = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePool(PagedPool, PAGE_SIZE);
		for (ULONG i = 0; i < pkfi->SubKeys; i++)
		{
			WCHAR keyName[MAX_PATH] = { 0 };
			UNICODE_STRING subKeyPath;
			
			wcscpy(keyName, L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\Services\\");
			wcscat(keyName, keyBaseInfos[i].KeyBaseInfo.Name);
			subKeyPath.Length = (USHORT)keyBaseInfos[i].KeyBaseInfo.NameLength + wcslen(L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\Services\\")*2;
			subKeyPath.Buffer = keyName;
			subKeyPath.MaximumLength = subKeyPath.Length;
			InitializeObjectAttributes(
				&objAttri,
				&subKeyPath,
				OBJ_CASE_INSENSITIVE,
				NULL,
				NULL);
			//打开这个子健
			st = ZwOpenKey(&subKey, READ_CONTROL | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &objAttri);
			if (!NT_SUCCESS(st))
			{
				return false;
			}
			UNICODE_STRING valueName;
			PWCHAR imageName;

			RtlInitUnicodeString(&valueName, L"ImagePath");
			//查询映像路径
			st = ZwQueryValueKey(subKey, &valueName, KeyValuePartialInformation, valueInfo, PAGE_SIZE, &returnSize);
			if (NT_SUCCESS(st))
			{
				imageName = ArkGetFileNameFromFullPath((PWCHAR)&valueInfo->Data);
				if (imageName)
				{
					BOOLEAN isEqual;
					int nameLen = ArkGetFileNameLenFromParam(imageName);

					isEqual = _wcsnicmp(imageName, ModName, nameLen);
					if (isEqual == 0)
					{
						//拷贝服务名
						wcscpy(ReturnName, keyBaseInfos[i].KeyBaseInfo.Name);
						ZwClose(subKey);
						break;
					}
				}
				//关闭句柄

			}
			ZwClose(subKey);
		}


		

		//释放资源
		if (pkfi)
		{
			ExFreePool(pkfi);
		}
		if (keyBaseInfos)
		{
			ExFreePool(keyBaseInfos);
		}
		if (valueInfo)
		{
			ExFreePool(valueInfo);
		}

	}

	ZwClose(serviceKey);

	return true;
}
