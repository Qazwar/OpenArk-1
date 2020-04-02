#ifndef NTOB_H
#define NTOB_H






#ifdef __cplusplus
extern "C" {
#endif


	//
	//  This is the sign low bit used to lock handle table entries
	//

	// OB

	// These definitions are no longer correct, but they produce correct results.

#define OBJ_PROTECT_CLOSE		0x00000001
#define OBJ_AUDIT_OBJECT_CLOSE  0x00000004
#define OBJ_HANDLE_ATTRIBUTES (OBJ_PROTECT_CLOSE | OBJ_INHERIT | OBJ_AUDIT_OBJECT_CLOSE)


#define MIDLEVEL_COUNT (PAGE_SIZE / sizeof(PHANDLE_TABLE_ENTRY))
#define TABLE_PAGE_SIZE PAGE_SIZE
#define LEVEL_CODE_MASK 3
#define EXHANDLE_TABLE_ENTRY_LOCK_BIT    1

#define LOWLEVEL_COUNT (TABLE_PAGE_SIZE / sizeof(HANDLE_TABLE_ENTRY))
#define EX_ADDITIONAL_INFO_SIGNATURE (-2)
#define ExpIsValidObjectEntry(Entry) \
    ( (Entry != NULL) && (Entry->Object != NULL) && (Entry->NextFreeTableEntry != EX_ADDITIONAL_INFO_SIGNATURE) )
	typedef struct _SECTION_OBJECT       // 6 elements, 0x30 bytes (sizeof) 
	{
		/*0x000*/     VOID*        StartingVa;
		/*0x008*/     VOID*        EndingVa;
		/*0x010*/     VOID*        Parent;
		/*0x018*/     VOID*        LeftChild;
		/*0x020*/     VOID*        RightChild;
		/*0x028*/     struct _SEGMENT_OBJECT* Segment;
	}SECTION_OBJECT, *PSECTION_OBJECT;

	typedef struct _EXHANDLE {

		union {

			struct {

				//
				//  Application available tag bits
				//

				ULONG TagBits : 2;

				//
				//  The handle table entry index
				//

				ULONG Index : 30;

			};

			HANDLE GenericHandleOverlay;

#define HANDLE_VALUE_INC 4 // Amount to increment the Value to get to the next handle

			ULONG_PTR Value;
		};

	} EXHANDLE, *PEXHANDLE;

	typedef struct _HANDLE_TABLE_ENTRY                  // 8 elements, 0x10 bytes (sizeof) 
	{
		union                                           // 4 elements, 0x8 bytes (sizeof)  
		{
			/*0x000*/         VOID*        Object;
			/*0x000*/         ULONG32      ObAttributes;
			/*0x000*/         struct _HANDLE_TABLE_ENTRY_INFO* InfoTable;
			/*0x000*/         UINT64       Value;
		};
		union                                           // 3 elements, 0x8 bytes (sizeof)  
		{
			/*0x008*/         ULONG32      GrantedAccess;
			struct                                      // 2 elements, 0x8 bytes (sizeof)  
			{
				/*0x008*/             UINT16       GrantedAccessIndex;
				/*0x00A*/             UINT16       CreatorBackTraceIndex;
				/*0x00C*/             UINT8        _PADDING0_[0x4];
			};
			/*0x008*/         ULONG32      NextFreeTableEntry;
		};
	}HANDLE_TABLE_ENTRY, *PHANDLE_TABLE_ENTRY;
	//
	// Object Directory Structure
	//

#define NUMBER_HASH_BUCKETS 37
#define OBJ_INVALID_SESSION_ID 0xFFFFFFFF






#ifdef ARK_MODE_KERNEL

#define ObjectNameInformation 1
#define ObjectTypesInformation 3
#define ObjectHandleFlagInformation 4
#define ObjectSessionInformation 5
#define ObjectSessionObjectInformation 6

#else

typedef enum _OBJECT_INFORMATION_CLASS {
	ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,
	ObjectTypesInformation,
	ObjectHandleFlagInformation,
	ObjectSessionInformation,
	MaxObjectInfoClass  // MaxObjectInfoClass should always be the last enum
} OBJECT_INFORMATION_CLASS;

#endif // ARK_KERNEL_MODE


	


	typedef struct _OBJECT_TYPE_INFORMATION
	{
		UNICODE_STRING TypeName;
		ULONG TotalNumberOfObjects;
		ULONG TotalNumberOfHandles;
		ULONG TotalPagedPoolUsage;
		ULONG TotalNonPagedPoolUsage;
		ULONG TotalNamePoolUsage;
		ULONG TotalHandleTableUsage;
		ULONG HighWaterNumberOfObjects;
		ULONG HighWaterNumberOfHandles;
		ULONG HighWaterPagedPoolUsage;
		ULONG HighWaterNonPagedPoolUsage;
		ULONG HighWaterNamePoolUsage;
		ULONG HighWaterHandleTableUsage;
		ULONG InvalidAttributes;
		GENERIC_MAPPING GenericMapping;
		ULONG ValidAccessMask;
		BOOLEAN SecurityRequired;
		BOOLEAN MaintainHandleCount;
		UCHAR TypeIndex; // since WINBLUE
		CHAR ReservedByte;
		ULONG PoolType;
		ULONG DefaultPagedPoolCharge;
		ULONG DefaultNonPagedPoolCharge;
	} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;






	//
	// Object Directory Entry Structure
	//
	typedef struct _OBJECT_DIRECTORY_ENTRY {
		struct _OBJECT_DIRECTORY_ENTRY *ChainLink;
		PVOID Object;
		ULONG HashValue;
	} OBJECT_DIRECTORY_ENTRY, *POBJECT_DIRECTORY_ENTRY;

	typedef struct _OBJECT_HEADER_NAME_INFO  // 3 elements, 0x20 bytes (sizeof) 
	{
		/*0x000*/     struct _OBJECT_DIRECTORY* Directory;
		/*0x008*/     struct _UNICODE_STRING Name;         // 3 elements, 0x10 bytes (sizeof) 
		/*0x018*/     LONG32       ReferenceCount;
		/*0x01C*/     UINT8        _PADDING0_[0x4];
	}OBJECT_HEADER_NAME_INFO, *POBJECT_HEADER_NAME_INFO;

#define OBJECT_TO_OBJECT_HEADER( o ) \
    CONTAINING_RECORD( (o), OBJECT_HEADER, Body )

	typedef struct _OBJECT_SYMBOLIC_LINK   // 3 elements, 0x20 bytes (sizeof) 
	{
		/*0x000*/     union _LARGE_INTEGER CreationTime; // 4 elements, 0x8 bytes (sizeof)  
		/*0x008*/     struct _UNICODE_STRING LinkTarget; // 3 elements, 0x10 bytes (sizeof) 
		/*0x018*/     ULONG32      DosDeviceDriveIndex;
		/*0x01C*/     UINT8        _PADDING0_[0x4];
	}OBJECT_SYMBOLIC_LINK, *POBJECT_SYMBOLIC_LINK;

	typedef struct _OBJECT_HEADER                                // 12 elements, 0x38 bytes (sizeof) 
	{
		/*0x000*/     INT64        PointerCount;
		union                                                    // 2 elements, 0x8 bytes (sizeof)   
		{
			/*0x008*/         INT64        HandleCount;
			/*0x008*/         VOID*        NextToFree;
		};
		/*0x010*/     struct _EX_PUSH_LOCK Lock;                               // 7 elements, 0x8 bytes (sizeof)   
		/*0x018*/     UINT8        TypeIndex;
		/*0x019*/     UINT8        TraceFlags;
		/*0x01A*/     UINT8        InfoMask;
		/*0x01B*/     UINT8        Flags;
		/*0x01C*/     UINT8        _PADDING0_[0x4];
		union                                                    // 2 elements, 0x8 bytes (sizeof)   
		{
			/*0x020*/         struct _OBJECT_CREATE_INFORMATION* ObjectCreateInfo;
			/*0x020*/         VOID*        QuotaBlockCharged;
		};
		/*0x028*/     VOID*        SecurityDescriptor;
		/*0x030*/     struct _QUAD Body;                                       // 2 elements, 0x8 bytes (sizeof)   
	}OBJECT_HEADER, *POBJECT_HEADER;



	typedef struct _OBJECT_TYPE_INITIALIZER                                                                                                                                         // 26 elements, 0x70 bytes (sizeof) 
	{
		/*0x000*/     UINT16       Length;
		union                                                                                                                                                                       // 2 elements, 0x1 bytes (sizeof)   
		{
			/*0x002*/         UINT8        ObjectTypeFlags;
			struct                                                                                                                                                                  // 8 elements, 0x1 bytes (sizeof)   
			{
				/*0x002*/             UINT8        CaseInsensitive : 1;                                                                                                                                   // 0 BitPosition                    
				/*0x002*/             UINT8        UnnamedObjectsOnly : 1;                                                                                                                                // 1 BitPosition                    
				/*0x002*/             UINT8        UseDefaultObject : 1;                                                                                                                                  // 2 BitPosition                    
				/*0x002*/             UINT8        SecurityRequired : 1;                                                                                                                                  // 3 BitPosition                    
				/*0x002*/             UINT8        MaintainHandleCount : 1;                                                                                                                               // 4 BitPosition                    
				/*0x002*/             UINT8        MaintainTypeList : 1;                                                                                                                                  // 5 BitPosition                    
				/*0x002*/             UINT8        SupportsObjectCallbacks : 1;                                                                                                                           // 6 BitPosition                    
				/*0x002*/             UINT8        CacheAligned : 1;                                                                                                                                      // 7 BitPosition                    
			};
		};
		/*0x004*/     ULONG32      ObjectTypeCode;
		/*0x008*/     ULONG32      InvalidAttributes;
		/*0x00C*/     struct _GENERIC_MAPPING GenericMapping;                                                                                                                                     // 4 elements, 0x10 bytes (sizeof)  
		/*0x01C*/     ULONG32      ValidAccessMask;
		/*0x020*/     ULONG32      RetainAccess;
		/*0x024*/     enum _POOL_TYPE PoolType;
		/*0x028*/     ULONG32      DefaultPagedPoolCharge;
		/*0x02C*/     ULONG32      DefaultNonPagedPoolCharge;
		/*0x030*/     void* DumpProcedure;
		/*0x038*/     void* OpenProcedure;
		/*0x040*/     void* CloseProcedure;
		/*0x048*/     void* DeleteProcedure;
		/*0x050*/     void* ParseProcedure;
		/*0x058*/     void* SecurityProcedure;
		/*0x060*/     void* QueryNameProcedure;
		/*0x068*/     void* OkayToCloseProcedure;
	}OBJECT_TYPE_INITIALIZER, *POBJECT_TYPE_INITIALIZER;

	typedef struct _OBJECT_TYPE                   // 12 elements, 0xD0 bytes (sizeof) 
	{
		/*0x000*/     struct _LIST_ENTRY TypeList;              // 2 elements, 0x10 bytes (sizeof)  
		/*0x010*/     struct _UNICODE_STRING Name;              // 3 elements, 0x10 bytes (sizeof)  
		/*0x020*/     VOID*        DefaultObject;
		/*0x028*/     UINT8        Index;
		/*0x029*/     UINT8        _PADDING0_[0x3];
		/*0x02C*/     ULONG32      TotalNumberOfObjects;
		/*0x030*/     ULONG32      TotalNumberOfHandles;
		/*0x034*/     ULONG32      HighWaterNumberOfObjects;
		/*0x038*/     ULONG32      HighWaterNumberOfHandles;
		/*0x03C*/     UINT8        _PADDING1_[0x4];
		/*0x040*/     struct _OBJECT_TYPE_INITIALIZER TypeInfo; // 26 elements, 0x70 bytes (sizeof) 
		/*0x0B0*/     struct _EX_PUSH_LOCK TypeLock;            // 7 elements, 0x8 bytes (sizeof)   
		/*0x0B8*/     ULONG32      Key;
		/*0x0BC*/     UINT8        _PADDING2_[0x4];
		/*0x0C0*/     struct _LIST_ENTRY CallbackList;          // 2 elements, 0x10 bytes (sizeof)  
	}OBJECT_TYPE, *POBJECT_TYPE;

	typedef struct _OBJECT_DIRECTORY                     // 6 elements, 0x150 bytes (sizeof) 
	{
		/*0x000*/     struct _OBJECT_DIRECTORY_ENTRY* HashBuckets[37];
		/*0x128*/     struct _EX_PUSH_LOCK Lock;                       // 7 elements, 0x8 bytes (sizeof)   
		/*0x130*/     struct _DEVICE_MAP* DeviceMap;
		/*0x138*/     ULONG32      SessionId;
		/*0x13C*/     UINT8        _PADDING0_[0x4];
		/*0x140*/     VOID*        NamespaceEntry;
		/*0x148*/     ULONG32      Flags;
		/*0x14C*/     UINT8        _PADDING1_[0x4];
	}OBJECT_DIRECTORY, *POBJECT_DIRECTORY;


	typedef struct _HANDLE_TABLE                         // 15 elements, 0x68 bytes (sizeof) 
	{
		/*0x000*/     UINT64       TableCode;
		/*0x008*/     struct _EPROCESS* QuotaProcess;
		/*0x010*/     VOID*        UniqueProcessId;
		/*0x018*/     struct _EX_PUSH_LOCK HandleLock;                 // 7 elements, 0x8 bytes (sizeof)   
		/*0x020*/     struct _LIST_ENTRY HandleTableList;              // 2 elements, 0x10 bytes (sizeof)  
		/*0x030*/     struct _EX_PUSH_LOCK HandleContentionEvent;      // 7 elements, 0x8 bytes (sizeof)   
		/*0x038*/     struct _HANDLE_TRACE_DEBUG_INFO* DebugInfo;
		/*0x040*/     LONG32       ExtraInfoPages;
		union                                            // 2 elements, 0x4 bytes (sizeof)   
		{
			/*0x044*/         ULONG32      Flags;
			/*0x044*/         UINT8        StrictFIFO : 1;                 // 0 BitPosition                    
		};
		/*0x048*/     ULONG32      FirstFreeHandle;
		/*0x04C*/     UINT8        _PADDING0_[0x4];
		/*0x050*/     struct _HANDLE_TABLE_ENTRY* LastFreeHandleEntry;
		/*0x058*/     ULONG32      HandleCount;
		/*0x05C*/     ULONG32      NextHandleNeedingPool;
		/*0x060*/     ULONG32      HandleCountHighWatermark;
		/*0x064*/     UINT8        _PADDING1_[0x4];
	}HANDLE_TABLE, *PHANDLE_TABLE;























#ifdef __cplusplus
}
#endif



#endif // !NTOB_H

