#pragma once


typedef struct _QUERY_DIRECTORY {
	ULONG Length;
	PUNICODE_STRING FileName;
	FILE_INFORMATION_CLASS FileInformationClass;
	ULONG FileIndex;
} QUERY_DIRECTORY, *PQUERY_DIRECTORY;



NTSTATUS
fnCreateFile(
	IN PFILE_REQUEST_CREATE FileRequestCreate,
	IN ULONG InputBufferLength,
	OUT PIO_STATUS_BLOCK IoStatusBlock);

/*
打开文件名的文件得到句柄

*/
BOOLEAN ArkOpenFile(
	PCHAR FileName,
	ULONG Len,
	PHANDLE Handle, 
	ULONG OutSize);
/*
通过句柄查询文件信息
*/
BOOLEAN ArkQueryFileInfomation(
	HANDLE FileHandle,
	PVOID InfoMation ,
	ULONG Len, 
	FILE_INFORMATION_CLASS InfomationClass);


NTSTATUS
IrpFileQuery(
	IN PFILE_OBJECT FileObject,
	OUT PVOID FileInformation,
	IN ULONG Length,
	IN FILE_INFORMATION_CLASS FileInformationClass
);