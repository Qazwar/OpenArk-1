/*
 * Process Hacker -
 *   Loader support functions
 *
 * This file is part of Process Hacker.
 *
 * Process Hacker is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Process Hacker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Process Hacker.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _NTLDR_H
#define _NTLDR_H




typedef struct _LDR_DATA_TABLE_ENTRY32 {
	LIST_ENTRY32 InLoadOrderLinks;
	LIST_ENTRY32 InMemoryOrderLinks;
	LIST_ENTRY32 InInitializationOrderLinks;
	ULONG DllBase;
	ULONG EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING32 FullDllName;
	UNICODE_STRING32 BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union {
		LIST_ENTRY32 HashLinks;
		struct {
			ULONG SectionPointer;
			ULONG  CheckSum;
		};
	};
	union {
		struct {
			ULONG  TimeDateStamp;
		};
		struct {
			ULONG LoadedImports;
		};
	};

	//
	// NOTE : Do not grow this structure at the dump files used a packed
	// array of these structures.
	//

} LDR_DATA_TABLE_ENTRY32, *PLDR_DATA_TABLE_ENTRY32;


 // DLLs

typedef BOOLEAN(NTAPI *PLDR_INIT_ROUTINE)(
	_In_ PVOID DllHandle,
	_In_ ULONG Reason,
	_In_opt_ PVOID Context
	);



 // symbols
typedef struct _LDR_SERVICE_TAG_RECORD
{
	struct _LDR_SERVICE_TAG_RECORD *Next;
	ULONG ServiceTag;
} LDR_SERVICE_TAG_RECORD, *PLDR_SERVICE_TAG_RECORD;

// symbols
typedef struct _LDRP_CSLIST
{
	PSINGLE_LIST_ENTRY Tail;
} LDRP_CSLIST, *PLDRP_CSLIST;

// symbols
typedef enum _LDR_DDAG_STATE
{
	LdrModulesMerged = -5,
	LdrModulesInitError = -4,
	LdrModulesSnapError = -3,
	LdrModulesUnloaded = -2,
	LdrModulesUnloading = -1,
	LdrModulesPlaceHolder = 0,
	LdrModulesMapping = 1,
	LdrModulesMapped = 2,
	LdrModulesWaitingForDependencies = 3,
	LdrModulesSnapping = 4,
	LdrModulesSnapped = 5,
	LdrModulesCondensed = 6,
	LdrModulesReadyToInit = 7,
	LdrModulesInitializing = 8,
	LdrModulesReadyToRun = 9
} LDR_DDAG_STATE;



 // symbols
typedef struct _LDR_DDAG_NODE
{
	LIST_ENTRY Modules;
	PLDR_SERVICE_TAG_RECORD ServiceTagList;
	ULONG LoadCount;
	ULONG LoadWhileUnloadingCount;
	ULONG LowestLink;
	union
	{
		LDRP_CSLIST Dependencies;
		SINGLE_LIST_ENTRY RemovalLink;
	};
	LDRP_CSLIST IncomingDependencies;
	LDR_DDAG_STATE State;
	SINGLE_LIST_ENTRY CondenseLink;
	ULONG PreorderNumber;
} LDR_DDAG_NODE, *PLDR_DDAG_NODE;

// symbols
typedef enum _LDR_DLL_LOAD_REASON
{
	LoadReasonStaticDependency,
	LoadReasonStaticForwarderDependency,
	LoadReasonDynamicForwarderDependency,
	LoadReasonDelayloadDependency,
	LoadReasonDynamicLoad,
	LoadReasonAsImageLoad,
	LoadReasonAsDataLoad,
	LoadReasonEnclavePrimary, // REDSTONE3
	LoadReasonEnclaveDependency,
	LoadReasonUnknown = -1
} LDR_DLL_LOAD_REASON, *PLDR_DLL_LOAD_REASON;
typedef struct _PEB_LDR_DATA                            // 9 elements, 0x58 bytes (sizeof) 
{
	/*0x000*/     ULONG32      Length;
	/*0x004*/     UINT8        Initialized;
	/*0x005*/     UINT8        _PADDING0_[0x3];
	/*0x008*/     VOID*        SsHandle;
	/*0x010*/     struct _LIST_ENTRY InLoadOrderModuleList;           // 2 elements, 0x10 bytes (sizeof) 
	/*0x020*/     struct _LIST_ENTRY InMemoryOrderModuleList;         // 2 elements, 0x10 bytes (sizeof) 
	/*0x030*/     struct _LIST_ENTRY InInitializationOrderModuleList; // 2 elements, 0x10 bytes (sizeof) 
	/*0x040*/     VOID*        EntryInProgress;
	/*0x048*/     UINT8        ShutdownInProgress;
	/*0x049*/     UINT8        _PADDING1_[0x7];
	/*0x050*/     VOID*        ShutdownThreadId;
}PEB_LDR_DATA, *PPEB_LDR_DATA;
 // symbols
typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	union
	{
		LIST_ENTRY InInitializationOrderLinks;
		LIST_ENTRY InProgressLinks;
	};
	PVOID DllBase;
	PLDR_INIT_ROUTINE EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	union
	{
		UCHAR FlagGroup[4];
		ULONG Flags;
		struct
		{
			ULONG PackagedBinary : 1;
			ULONG MarkedForRemoval : 1;
			ULONG ImageDll : 1;
			ULONG LoadNotificationsSent : 1;
			ULONG TelemetryEntryProcessed : 1;
			ULONG ProcessStaticImport : 1;
			ULONG InLegacyLists : 1;
			ULONG InIndexes : 1;
			ULONG ShimDll : 1;
			ULONG InExceptionTable : 1;
			ULONG ReservedFlags1 : 2;
			ULONG LoadInProgress : 1;
			ULONG LoadConfigProcessed : 1;
			ULONG EntryProcessed : 1;
			ULONG ProtectDelayLoad : 1;
			ULONG ReservedFlags3 : 2;
			ULONG DontCallForThreads : 1;
			ULONG ProcessAttachCalled : 1;
			ULONG ProcessAttachFailed : 1;
			ULONG CorDeferredValidate : 1;
			ULONG CorImage : 1;
			ULONG DontRelocate : 1;
			ULONG CorILOnly : 1;
			ULONG ChpeImage : 1;
			ULONG ReservedFlags5 : 2;
			ULONG Redirected : 1;
			ULONG ReservedFlags6 : 2;
			ULONG CompatDatabaseProcessed : 1;
		};
	};
	USHORT ObsoleteLoadCount;
	USHORT TlsIndex;
	LIST_ENTRY HashLinks;
	ULONG TimeDateStamp;
	struct _ACTIVATION_CONTEXT *EntryPointActivationContext;
	PVOID Lock; // RtlAcquireSRWLockExclusive
	PLDR_DDAG_NODE DdagNode;
	LIST_ENTRY NodeModuleLink;
	struct _LDRP_LOAD_CONTEXT *LoadContext;
	PVOID ParentDllBase;
	PVOID SwitchBackContext;
	RTL_BALANCED_NODE BaseAddressIndexNode;
	RTL_BALANCED_NODE MappingInfoIndexNode;
	ULONG_PTR OriginalBase;
	LARGE_INTEGER LoadTime;
	ULONG BaseNameHashValue;
	LDR_DLL_LOAD_REASON LoadReason;
	ULONG ImplicitPathOptions;
	ULONG ReferenceCount;
	ULONG DependentLoadFlags;
	UCHAR SigningLevel; // since REDSTONE2
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;



typedef struct _PEB                                                                               // 91 elements, 0x380 bytes (sizeof) 
{
	/*0x000*/     UINT8        InheritedAddressSpace;
	/*0x001*/     UINT8        ReadImageFileExecOptions;
	/*0x002*/     UINT8        BeingDebugged;
	union                                                                                         // 2 elements, 0x1 bytes (sizeof)    
	{
		/*0x003*/         UINT8        BitField;
		struct                                                                                    // 6 elements, 0x1 bytes (sizeof)    
		{
			/*0x003*/             UINT8        ImageUsesLargePages : 1;                                                 // 0 BitPosition                     
			/*0x003*/             UINT8        IsProtectedProcess : 1;                                                  // 1 BitPosition                     
			/*0x003*/             UINT8        IsLegacyProcess : 1;                                                     // 2 BitPosition                     
			/*0x003*/             UINT8        IsImageDynamicallyRelocated : 1;                                         // 3 BitPosition                     
			/*0x003*/             UINT8        SkipPatchingUser32Forwarders : 1;                                        // 4 BitPosition                     
			/*0x003*/             UINT8        SpareBits : 3;                                                           // 5 BitPosition                     
		};
	};
	/*0x008*/     VOID*        Mutant;
	/*0x010*/     VOID*        ImageBaseAddress;
	/*0x018*/     struct _PEB_LDR_DATA* Ldr;
	/*0x020*/     struct _RTL_USER_PROCESS_PARAMETERS* ProcessParameters;
	/*0x028*/     VOID*        SubSystemData;
	/*0x030*/     VOID*        ProcessHeap;
	/*0x038*/     struct _RTL_CRITICAL_SECTION* FastPebLock;
	/*0x040*/     VOID*        AtlThunkSListPtr;
	/*0x048*/     VOID*        IFEOKey;
	union                                                                                         // 2 elements, 0x4 bytes (sizeof)    
	{
		/*0x050*/         ULONG32      CrossProcessFlags;
		struct                                                                                    // 6 elements, 0x4 bytes (sizeof)    
		{
			/*0x050*/             ULONG32      ProcessInJob : 1;                                                        // 0 BitPosition                     
			/*0x050*/             ULONG32      ProcessInitializing : 1;                                                 // 1 BitPosition                     
			/*0x050*/             ULONG32      ProcessUsingVEH : 1;                                                     // 2 BitPosition                     
			/*0x050*/             ULONG32      ProcessUsingVCH : 1;                                                     // 3 BitPosition                     
			/*0x050*/             ULONG32      ProcessUsingFTH : 1;                                                     // 4 BitPosition                     
			/*0x050*/             ULONG32      ReservedBits0 : 27;                                                      // 5 BitPosition                     
		};
	};
	union                                                                                         // 2 elements, 0x8 bytes (sizeof)    
	{
		/*0x058*/         VOID*        KernelCallbackTable;
		/*0x058*/         VOID*        UserSharedInfoPtr;
	};
	/*0x060*/     ULONG32      SystemReserved[1];
	/*0x064*/     ULONG32      AtlThunkSListPtr32;
	/*0x068*/     VOID*        ApiSetMap;
	/*0x070*/     ULONG32      TlsExpansionCounter;
	/*0x074*/     UINT8        _PADDING0_[0x4];
	/*0x078*/     VOID*        TlsBitmap;
	/*0x080*/     ULONG32      TlsBitmapBits[2];
	/*0x088*/     VOID*        ReadOnlySharedMemoryBase;
	/*0x090*/     VOID*        HotpatchInformation;
	/*0x098*/     VOID**       ReadOnlyStaticServerData;
	/*0x0A0*/     VOID*        AnsiCodePageData;
	/*0x0A8*/     VOID*        OemCodePageData;
	/*0x0B0*/     VOID*        UnicodeCaseTableData;
	/*0x0B8*/     ULONG32      NumberOfProcessors;
	/*0x0BC*/     ULONG32      NtGlobalFlag;
	/*0x0C0*/     union _LARGE_INTEGER CriticalSectionTimeout;                                                  // 4 elements, 0x8 bytes (sizeof)    
	/*0x0C8*/     UINT64       HeapSegmentReserve;
	/*0x0D0*/     UINT64       HeapSegmentCommit;
	/*0x0D8*/     UINT64       HeapDeCommitTotalFreeThreshold;
	/*0x0E0*/     UINT64       HeapDeCommitFreeBlockThreshold;
	/*0x0E8*/     ULONG32      NumberOfHeaps;
	/*0x0EC*/     ULONG32      MaximumNumberOfHeaps;
	/*0x0F0*/     VOID**       ProcessHeaps;
	/*0x0F8*/     VOID*        GdiSharedHandleTable;
	/*0x100*/     VOID*        ProcessStarterHelper;
	/*0x108*/     ULONG32      GdiDCAttributeList;
	/*0x10C*/     UINT8        _PADDING1_[0x4];
	/*0x110*/     struct _RTL_CRITICAL_SECTION* LoaderLock;
	/*0x118*/     ULONG32      OSMajorVersion;
	/*0x11C*/     ULONG32      OSMinorVersion;
	/*0x120*/     UINT16       OSBuildNumber;
	/*0x122*/     UINT16       OSCSDVersion;
	/*0x124*/     ULONG32      OSPlatformId;
	/*0x128*/     ULONG32      ImageSubsystem;
	/*0x12C*/     ULONG32      ImageSubsystemMajorVersion;
	/*0x130*/     ULONG32      ImageSubsystemMinorVersion;
	/*0x134*/     UINT8        _PADDING2_[0x4];
	/*0x138*/     UINT64       ActiveProcessAffinityMask;
	/*0x140*/     ULONG32      GdiHandleBuffer[60];
	/*0x230*/     void* PostProcessInitRoutine;
	/*0x238*/     VOID*        TlsExpansionBitmap;
	/*0x240*/     ULONG32      TlsExpansionBitmapBits[32];
	/*0x2C0*/     ULONG32      SessionId;
	/*0x2C4*/     UINT8        _PADDING3_[0x4];
	/*0x2C8*/     union _ULARGE_INTEGER AppCompatFlags;                                                         // 4 elements, 0x8 bytes (sizeof)    
	/*0x2D0*/     union _ULARGE_INTEGER AppCompatFlagsUser;                                                     // 4 elements, 0x8 bytes (sizeof)    
	/*0x2D8*/     VOID*        pShimData;
	/*0x2E0*/     VOID*        AppCompatInfo;
	/*0x2E8*/     struct _UNICODE_STRING CSDVersion;                                                            // 3 elements, 0x10 bytes (sizeof)   
	/*0x2F8*/     struct _ACTIVATION_CONTEXT_DATA* ActivationContextData;
	/*0x300*/     struct _ASSEMBLY_STORAGE_MAP* ProcessAssemblyStorageMap;
	/*0x308*/     struct _ACTIVATION_CONTEXT_DATA* SystemDefaultActivationContextData;
	/*0x310*/     struct _ASSEMBLY_STORAGE_MAP* SystemAssemblyStorageMap;
	/*0x318*/     UINT64       MinimumStackCommit;
	/*0x320*/     struct _FLS_CALLBACK_INFO* FlsCallback;
	/*0x328*/     struct _LIST_ENTRY FlsListHead;                                                               // 2 elements, 0x10 bytes (sizeof)   
	/*0x338*/     VOID*        FlsBitmap;
	/*0x340*/     ULONG32      FlsBitmapBits[4];
	/*0x350*/     ULONG32      FlsHighIndex;
	/*0x354*/     UINT8        _PADDING4_[0x4];
	/*0x358*/     VOID*        WerRegistrationData;
	/*0x360*/     VOID*        WerShipAssertPtr;
	/*0x368*/     VOID*        pContextData;
	/*0x370*/     VOID*        pImageHeaderHash;
	union                                                                                         // 2 elements, 0x4 bytes (sizeof)    
	{
		/*0x378*/         ULONG32      TracingFlags;
		struct                                                                                    // 3 elements, 0x4 bytes (sizeof)    
		{
			/*0x378*/             ULONG32      HeapTracingEnabled : 1;                                                  // 0 BitPosition                     
			/*0x378*/             ULONG32      CritSecTracingEnabled : 1;                                               // 1 BitPosition                     
			/*0x378*/             ULONG32      SpareTracingBits : 30;                                                   // 2 BitPosition                     
		};
	};
}PEB, *PPEB;
typedef struct _PEB_LDR_DATA32 {
	ULONG Length;
	BOOLEAN Initialized;
	TYPE32(HANDLE) SsHandle;
	LIST_ENTRY32 InLoadOrderModuleList;
	LIST_ENTRY32 InMemoryOrderModuleList;
	LIST_ENTRY32 InInitializationOrderModuleList;
	TYPE32(PVOID) EntryInProgress;
} PEB_LDR_DATA32, *PPEB_LDR_DATA32;



#endif
