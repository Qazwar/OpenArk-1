#ifndef OB_H
#define OB_H





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
#endif // !OB_H