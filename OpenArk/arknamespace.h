#pragma once
#include "openark.h"

#ifdef ARKNAMESPACE
#define EXTERN 
#else
#define EXTERN extern

#endif // ARKNAMESPACE

namespace Ark {


	EXTERN HANDLE Device;
	EXTERN OpenArk* Instance;

};



