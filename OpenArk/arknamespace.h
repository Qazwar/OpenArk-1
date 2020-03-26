#pragma once
#include "openark.h"
#include "processmgr.h"

#ifdef ARKNAMESPACE
#define EXTERN 
#else
#define EXTERN extern

#endif // ARKNAMESPACE

namespace Ark {


	EXTERN HANDLE Device;
	EXTERN OpenArk* Instance;
	EXTERN ProcessMgr* Ps;
	EXTERN char *Buffer;

};



