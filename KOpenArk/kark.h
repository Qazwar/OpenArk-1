#ifndef KARK_H
#define KARK_H




#include <ntifs.h>
#include <ntintsafe.h>
#define ARK_MODE_KERNEL  

#include <arknt.h>
#include "ntfill.h"
#include "common.h"
#include "pe.h"
#include "ob.h"
#include "dyndata.h"
#include "tool.h"
#include "fs.h"
#include "process.h"
#include "ex.h"





// Memory

#define PTR_ADD_OFFSET(Pointer, Offset) ((PVOID)((ULONG_PTR)(Pointer) + (ULONG_PTR)(Offset)))
#define PTR_SUB_OFFSET(Pointer, Offset) ((PVOID)((ULONG_PTR)(Pointer) - (ULONG_PTR)(Offset)))
#define  GETWORD(x)   (*(USHORT*)(x))
#define  GETDWORD(x)  (*(ULONG*)(x))
#define  GETQWORD(x)  (*(ULONG64*)(x))


//Debugging

#ifdef DBG
#define dprintf(Format, ...) DbgPrint("KProcessHacker: " Format, __VA_ARGS__)
#define DBGERRINFO  DbgPrint("%s--%d", __FILE__,__LINE__);
#else
#define dprintf
#endif



#endif // !KARK_H
