#ifndef PE_H
#define PE_H
#include <Windows.h>


#define SIZE_OF_NT_SIGNATURE	sizeof (DWORD)
#define MAXRESOURCENAME 	13

enum Pe
{
	Stretch,
	NoStretch
};



/* global macros to define header offsets into file */
/* offset to PE file signature                                 */
#define NTSIGNATURE(a) ((PVOID)((BYTE *)a		     +	\
			((PIMAGE_DOS_HEADER)a)->e_lfanew))

/* DOS header identifies the NT PEFile signature dword
   the PEFILE header exists just after that dword              */
#define PEFHDROFFSET(a) ((PVOID)((BYTE *)a		     +	\
			 ((PIMAGE_DOS_HEADER)a)->e_lfanew    +	\
			 SIZE_OF_NT_SIGNATURE))

   /* PE optional header is immediately after PEFile header       */
#define OPTHDROFFSET(a) ((PVOID)((BYTE *)a		     +	\
			 ((PIMAGE_DOS_HEADER)a)->e_lfanew    +	\
			 SIZE_OF_NT_SIGNATURE		     +	\
			 sizeof (IMAGE_FILE_HEADER)))

/* section headers are immediately after PE optional header    */
#define SECHDROFFSET(a) ((PVOID)((BYTE *)a		     +	\
			 ((PIMAGE_DOS_HEADER)a)->e_lfanew    +	\
			 SIZE_OF_NT_SIGNATURE		     +	\
			 sizeof (IMAGE_FILE_HEADER)	     +	\
			 sizeof (IMAGE_OPTIONAL_HEADER)))


PVOID PeGetDataDirTable(PCHAR lpImage, ULONG dwIMAGE_DIRECTORY);

int
NumOfSections(PVOID lpFile);
	

ULONG PeRvaToRaw(CHAR *lpImage, ULONG rva);

PVOID PeGetProcAddress(CHAR *lpImage, CHAR* funName);

PCHAR PeStretchImage(PCHAR lpFile);

BOOLEAN PeFixRelocTable(PCHAR BaseAddress, LONG Delta);

ULONG PeGetProcRva(PCHAR imageBase, PCHAR funName);


PCHAR PeLoader(PCHAR ExePath, ULONG_PTR RelocBase, BOOL DoImport);

#endif