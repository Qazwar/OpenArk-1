#ifndef OB_H
#define OB_H



PVOID ObpDecodeObject(PVOID Object);


BOOLEAN  ArkGetObjectDirectoryInfo(PCHAR pIndata, ULONG cbInData, PObInfo pOutData, ULONG cbOutData);

BOOLEAN ObQueryNameFileObject(
	_In_ PFILE_OBJECT FileObject,
	_Out_ PWSTR Buffer,
	_In_ ULONG BufferLength,
	_Out_ PULONG ReturnLength
);

struct ObjectType
{
public:
	enum 
	{
		DirectoryType,
		SymbolinkType,
		FileType,
		DriverType,
		LastObjectType,
	};

	int TypeIndex;
	POBJECT_TYPE ObjectType;
};
#endif // !OB_H