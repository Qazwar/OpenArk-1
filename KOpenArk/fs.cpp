//#include "process.h"
//#include "struct.h"
//#include "common.h"
//
//
//
//BOOLEAN  FsGetFilePathBySection(PSECTION_OBJECT pSectionObject, PWSTR *ppwStrPath, ULONG *pPathLen)
//{
//	using namespace NT::EPROCESS;
//	PSEGMENT      pSegment;
//	PCONTROL_AREA pControlArea;
//
//	
//
//	if (pSectionObject > MmSystemRangeStart
//		&& MmIsAddressValid(pSectionObject)
//		&& MmIsAddressValid(((PCHAR)pSectionObject + sizeof(SECTION_OBJECT) - 1)))
//	{
//		if (NT::OsVersion < Windows8 || NT::OsVersion >= Windows8_1)
//			pSegment = (PSEGMENT)pSectionObject->Segment;
//		else
//			return FALSE;
//
//		if (pSegment > MmSystemRangeStart
//			&& MmIsAddressValid(pSegment)
//			&& MmIsAddressValid((char *)pSegment + 7))
//		{
//			pControlArea = pSegment->ControlArea;
//			if (pControlArea > MmSystemRangeStart
//				&& MmIsAddressValid(pControlArea)
//				&& MmIsAddressValid((PCHAR)pControlArea + sizeof(CONTROL_AREA) - 1))
//			{
//				return FsGetFilePathByFileObject((PFILE_OBJECT)(pControlArea->FilePointer.Object), ppwStrPath, pPathLen);// 成功返回1
//			}
//		}
//	}
//	return FALSE;
//}
//
//BOOLEAN  FsGetFilePathByFileObject(PFILE_OBJECT pFileObject, PWSTR *ppwStrPath, ULONG *pPathLen)
//{
//
//	PWSTR		 pBuf;
//	unsigned int nDevPathLen;
//	PDEVICE_OBJECT pDeviceObject;
//	UNICODE_STRING dosName;
//
//	__try
//	{
//
//		if (!NT::IoFileObjectType)
//			return FALSE;
//
//		//check version
//		if (NT::OsVersion >= Windows_Vista)
//			pFileObject = (PFILE_OBJECT)((ULONG_PTR)pFileObject & 0xFFFFFFFFFFFFFFF0);
//
//		//check addr valid
//		if (!pFileObject || pFileObject < MmSystemRangeStart || !MmIsAddressValid(pFileObject))
//			return FALSE;
//
//		pDeviceObject = pFileObject->DeviceObject;
//
//
//		//check apc 没有被禁用
//		if (!pDeviceObject || !MmIsAddressValid(pDeviceObject) || KeAreAllApcsDisabled() || ObReferenceObjectByPointer(pFileObject, 0, NT::IoFileObjectType, KernelMode) != STATUS_SUCCESS)
//			return FALSE;
//
//		/*
//		IoVolumeDeviceToDosName -> "c:"
//		*/
//		if (NT_SUCCESS(IoVolumeDeviceToDosName(pDeviceObject, &dosName)))
//		{
//			if (!dosName.Buffer || !MmIsAddressValid(dosName.Buffer))
//			{
//				ObfDereferenceObject(pFileObject);
//				return FALSE;
//			}
//
//			pBuf = (PWSTR)ExAllocatePoolWithTag(NonPagedPool, PAGE_SIZE, TAG);
//			if (pBuf)
//			{
//				RtlZeroMemory(pBuf, PAGE_SIZE);
//
//				// 复制盘符路径
//				memmove(pBuf, dosName.Buffer, dosName.Length);
//				wcscat(pBuf, pFileObject->FileName.Buffer);
//				*ppwStrPath = pBuf;
//				if (pPathLen)
//					*pPathLen = pFileObject->FileName.Length + dosName.Length;
//				ExFreePool(dosName.Buffer);// 释放iovolumtoDos所分配的缓冲区
//				ObfDereferenceObject(pFileObject);
//				return TRUE;
//			}
//
//		}
//
//	}
//	__except (1)
//	{
//		DbgPrint("__except:FsGetFilePathByFileObject");
//	}
//
//	return FALSE;
//}
//
//
//void sector()
//{
//	
//
//
//
//
//}
