#ifndef NTEX_H
#define NTEX_H





#undef EX_PUSH_LOCK
#undef PEX_PUSH_LOCK



typedef struct _EX_PUSH_LOCK                 // 7 elements, 0x8 bytes (sizeof) 
{
	union                                    // 3 elements, 0x8 bytes (sizeof) 
	{
		struct                               // 5 elements, 0x8 bytes (sizeof) 
		{
			/*0x000*/             UINT64       Locked : 1;         // 0 BitPosition                  
			/*0x000*/             UINT64       Waiting : 1;        // 1 BitPosition                  
			/*0x000*/             UINT64       Waking : 1;         // 2 BitPosition                  
			/*0x000*/             UINT64       MultipleShared : 1; // 3 BitPosition                  
			/*0x000*/             UINT64       Shared : 60;        // 4 BitPosition                  
		};
		/*0x000*/         UINT64       Value;
		/*0x000*/         VOID*        Ptr;
	};
}EX_PUSH_LOCK, *PEX_PUSH_LOCK;



















#endif // !NTEX_H

