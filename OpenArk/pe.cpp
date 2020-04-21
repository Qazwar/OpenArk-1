#include "pe.h"
#include <qfile.h>


//
// This function is written by sang cho
//                                                 .. october 5, 1997
//
/* function returns the actual address of given RVA,      lpFile must
   be a memory mapped file pointer to the beginning of the image file */
PVOID
GetActualAddress(
	PVOID lpFile,
	ULONG dwRVA)
{
	//    PIMAGE_OPTIONAL_HEADER   poh = (PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET (lpFile);
	PIMAGE_SECTION_HEADER psh = (PIMAGE_SECTION_HEADER)SECHDROFFSET(lpFile);
	int nSections = NumOfSections(lpFile);
	int i = 0;

	if (dwRVA == 0)
		return NULL;


	/* locate section containing image directory */
	while (i++ < nSections)
	{
		if (psh->VirtualAddress <= (DWORD)dwRVA &&
			psh->VirtualAddress + psh->SizeOfRawData > (DWORD)dwRVA)
			break;
		psh++;
	}

	if (i > nSections)
		return NULL;

	/* return image import directory offset */
	return (PVOID)(((ULONG_PTR)lpFile + dwRVA - psh->VirtualAddress) +
		(int)psh->PointerToRawData);
}


ULONG  PeGetNumOfExportName(PCHAR lpImage, ULONG SizeOfImage)
{

	PIMAGE_EXPORT_DIRECTORY ped;

	ped = (PIMAGE_EXPORT_DIRECTORY)PeGetDataDirTable(lpImage, IMAGE_DIRECTORY_ENTRY_EXPORT);

	return ped->NumberOfNames;
}



/* return the total number of sections in the module */
int
NumOfSections(
	PVOID lpFile)
{
	/* number os sections is indicated in file header */
	return ((int)((PIMAGE_FILE_HEADER)PEFHDROFFSET(lpFile))->NumberOfSections);
}

//
// This function is modified by sang cho
//
//
/* return offset to specified IMAGE_DIRECTORY entry
	得到未拉伸文件的目录的地址
*/
PVOID
ImageDirectoryOffset(
	PVOID lpFile,
	DWORD dwIMAGE_DIRECTORY)
{
	PIMAGE_OPTIONAL_HEADER poh = (PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET(lpFile);
	PIMAGE_SECTION_HEADER psh = (PIMAGE_SECTION_HEADER)SECHDROFFSET(lpFile);
	int nSections = NumOfSections(lpFile);
	int i = 0;
	ULONG RVAImageDir;

	/* must be 0 thru (NumberOfRvaAndSizes-1) */
	if (dwIMAGE_DIRECTORY >= poh->NumberOfRvaAndSizes)
		return NULL;

	/* locate specific image directory's relative virtual address */
	RVAImageDir = poh->DataDirectory[dwIMAGE_DIRECTORY].VirtualAddress;

	if (RVAImageDir == NULL)
		return NULL;
	/* locate section containing image directory */
	while (i++ < nSections)
	{
		if (psh->VirtualAddress <= RVAImageDir &&
			psh->VirtualAddress + psh->SizeOfRawData > RVAImageDir)
			break;
		psh++;
	}

	if (i > nSections)
		return NULL;

	/* return image import directory offset */
	return (PVOID)(((ULONG_PTR)lpFile + (ULONG_PTR)RVAImageDir - psh->VirtualAddress) + (ULONG_PTR)psh->PointerToRawData);

}


PVOID PeGetDataDirTable(PCHAR lpImage, DWORD dirIndex)
{
	PIMAGE_OPTIONAL_HEADER poh = (PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET(lpImage);


	ULONG dirRva;

	/* must be 0 thru (NumberOfRvaAndSizes-1) */
	if (dirIndex >= poh->NumberOfRvaAndSizes)
		return NULL;

	/* locate specific image directory's relative virtual address */
	dirRva = poh->DataDirectory[dirIndex].VirtualAddress;

	if (dirRva == NULL)
		return NULL;

	/* return image import directory offset */
	return (PVOID)(lpImage + dirRva);
}



DWORD PeRvaToRaw(CHAR *lpImage, DWORD rva)
{
	DWORD offset = rva, limit;
	PIMAGE_SECTION_HEADER psh;
	WORD i;

	PIMAGE_NT_HEADERS nthdrs = (PIMAGE_NT_HEADERS)NTSIGNATURE(lpImage);
	psh = IMAGE_FIRST_SECTION(nthdrs);

	if (rva < psh->PointerToRawData)
		return rva;

	for (i = 0; i < nthdrs->FileHeader.NumberOfSections; i++)
	{
		if (psh[i].SizeOfRawData)
			limit = psh[i].SizeOfRawData;
		else
			limit = psh[i].Misc.VirtualSize;

		if (rva >= psh[i].VirtualAddress &&
			rva < (psh[i].VirtualAddress + limit))
		{
			if (psh[i].PointerToRawData != 0)
			{
				offset -= psh[i].VirtualAddress;
				offset += psh[i].PointerToRawData;
			}

			return offset;
		}
	}

	return 0;
}

/*++
Description:
	get function address, analogous GetProcAddress
Arguments:
	base - image/file base
	proc_name - function name
	base_type = implies image or file
Return:
	function address
--*/
PVOID PeGetProcAddress(CHAR *lpImage, CHAR* funName)
{
	PVOID funcAddr = NULL;
	PIMAGE_EXPORT_DIRECTORY ped;


	ped = (PIMAGE_EXPORT_DIRECTORY)PeGetDataDirTable(lpImage, IMAGE_DIRECTORY_ENTRY_EXPORT);
	if (!ped)
		return NULL;

	PULONG arrNameAddr = (PULONG)((ped->AddressOfNames) + lpImage);
	PULONG arrFunRva = (PULONG)((ped->AddressOfFunctions) + lpImage);
	PUSHORT arrOrdinals = (PUSHORT)((ped->AddressOfNameOrdinals) + lpImage);

	if ((ULONG_PTR)funName & (ULONG_PTR)(~0xFFFF)) { // names

		for (DWORD i = 0; i < ped->NumberOfNames; i++) {

			CHAR* exportName = lpImage + arrNameAddr[i];

			if (!_stricmp(exportName, funName)) {
				DWORD idx = arrOrdinals[i];
				if (idx >= ped->NumberOfFunctions)
					return NULL;
				funcAddr = (arrFunRva[idx]) + lpImage;
				break;
			}
		}
	}
	else { // ordinal
		ULONG_PTR ordinal = (ULONG_PTR)funName;
		if (ordinal < ped->Base || ordinal >= (ped->Base + ped->NumberOfFunctions))
			return NULL;
		funcAddr = (arrFunRva[ordinal - ped->Base]) + lpImage;
	}

	return funcAddr;
}

PCHAR PeStretchImage(PCHAR lpFile)
{

	DWORD  limit;
	PIMAGE_SECTION_HEADER psh;
	PIMAGE_NT_HEADERS nthdrs;
	PIMAGE_OPTIONAL_HEADER poh;
	PCHAR lpImage = 0;

	nthdrs = (PIMAGE_NT_HEADERS)NTSIGNATURE(lpFile);
	psh = IMAGE_FIRST_SECTION(nthdrs);
	poh = (PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET(lpFile);

	lpImage = (PCHAR)new char[poh->SizeOfImage];
	if (!lpImage)
		return 0;
	RtlZeroMemory(lpImage, poh->SizeOfImage);
	memcpy(lpImage, lpFile, poh->SizeOfHeaders);

	for (int i = 0; i < nthdrs->FileHeader.NumberOfSections; i++)
	{
		limit = min(psh[i].SizeOfRawData, psh[i].Misc.VirtualSize);
		RtlCopyMemory(lpImage + psh[i].VirtualAddress, lpFile + psh[i].PointerToRawData, limit);
	}
	return lpImage;
}

BOOLEAN PeFixRelocTable(PCHAR baseAddress, ULONG_PTR delta)
{

	
	PIMAGE_NT_HEADERS ntHeaders;
	PIMAGE_DATA_DIRECTORY relocationDDir;
	PIMAGE_BASE_RELOCATION relocationDir, relocationEnd;
	ULONG count;
	ULONG_PTR address;
	PUSHORT typeOffset;


	ntHeaders = (PIMAGE_NT_HEADERS)NTSIGNATURE(baseAddress);

	if (ntHeaders == NULL)
		return false;

	if (ntHeaders->FileHeader.Characteristics & IMAGE_FILE_RELOCS_STRIPPED)
	{
		return false;
	}

	relocationDDir = &ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];

	if ((relocationDDir->VirtualAddress) == 0 || (relocationDDir->Size) == 0)
	{
		return false;
	}


	relocationDir = (PIMAGE_BASE_RELOCATION)(baseAddress + relocationDDir->VirtualAddress);
	relocationEnd = (PIMAGE_BASE_RELOCATION)((ULONG_PTR)relocationDir + (relocationDDir->Size));

	while (relocationDir < relocationEnd &&(relocationDir->SizeOfBlock) > 0)
	{
		count = ((relocationDir->SizeOfBlock) - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(USHORT);
		address = (ULONG_PTR)baseAddress + relocationDir->VirtualAddress;
		typeOffset = (PUSHORT)(relocationDir + 1);

		for (size_t i = 0; i < count; i++)
		{
			USHORT offset = (*typeOffset) & 0xFFF;
			USHORT type = (*typeOffset) >> 12;
			ULONG_PTR *longLongPtr;
			if (type == IMAGE_REL_BASED_DIR64)
			{
				longLongPtr = (PULONG_PTR)(address + offset);
				*longLongPtr = *longLongPtr + delta;
			}
			typeOffset++;
		}
		relocationDir = (PIMAGE_BASE_RELOCATION)((ULONG_PTR)relocationDir + relocationDir->SizeOfBlock);
	}

	return true;

}

ULONG PeGetProcRva(PCHAR lpImage, PCHAR funName)
{

	ULONG funcAddr = NULL;
	PIMAGE_EXPORT_DIRECTORY ped;


	ped = (PIMAGE_EXPORT_DIRECTORY)PeGetDataDirTable(lpImage, IMAGE_DIRECTORY_ENTRY_EXPORT);
	if (!ped)
		return NULL;

	PULONG nameRva = (PULONG)((ped->AddressOfNames) + lpImage);
	PULONG funRva = (PULONG)((ped->AddressOfFunctions) + lpImage);
	PUSHORT ordinals = (PUSHORT)((ped->AddressOfNameOrdinals) + lpImage);

	if ((ULONG_PTR)funName & (ULONG_PTR)(~0xFFFF)) 
	{ // names

		for (DWORD i = 0; i < ped->NumberOfNames; i++) 
		{

			PCHAR exportName = lpImage + nameRva[i];

			if (!_stricmp(exportName, funName))
			{
				DWORD idx = ordinals[i];
				if (idx >= ped->NumberOfFunctions)
					return NULL;
				return funRva[idx];
			}
		}
	}
	else { // ordinal
		ULONG_PTR ordinal = (ULONG_PTR)funName;
		if (ordinal < ped->Base || ordinal >= (ped->Base + ped->NumberOfFunctions))
			return NULL;
		return funRva[ordinal - ped->Base];
	}

	return 0;
}

PCHAR PeLoader(PCHAR ExePath,  ULONG_PTR RelocBase, BOOL DoImport)
{
	QFile qfile(ExePath);
	PCHAR  mappedBase;
	PCHAR imageBase;
	BOOLEAN sucess = 1;

	if (qfile.open(QIODevice::ReadOnly))
	{
		mappedBase = (PCHAR)qfile.map(0, qfile.size());
		imageBase = PeStretchImage(mappedBase);
		auto orignalBase = ((PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET(imageBase))->ImageBase;

		if (imageBase && RelocBase)
		{
			ULONG_PTR delta = RelocBase - orignalBase;
			if (!PeFixRelocTable(imageBase, delta))
			{
				sucess = 0;
			}
			
		}
		else
		{
			if (!PeFixRelocTable(imageBase, (ULONG_PTR)orignalBase - (ULONG_PTR)imageBase ))
			{
				sucess = 0;
			}
		}
	}

	
	if (!sucess  )
	{
		if (imageBase)
		{
			delete imageBase;
			imageBase = 0;
		}
	}

	return imageBase;
}



