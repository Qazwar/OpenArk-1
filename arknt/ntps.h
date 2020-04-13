#pragma once







typedef union _KEXECUTE_OPTIONS                           // 9 elements, 0x1 bytes (sizeof) 
{
	struct                                                // 8 elements, 0x1 bytes (sizeof) 
	{
		/*0x000*/         UINT8        ExecuteDisable : 1;                  // 0 BitPosition                  
		/*0x000*/         UINT8        ExecuteEnable : 1;                   // 1 BitPosition                  
		/*0x000*/         UINT8        DisableThunkEmulation : 1;           // 2 BitPosition                  
		/*0x000*/         UINT8        Permanent : 1;                       // 3 BitPosition                  
		/*0x000*/         UINT8        ExecuteDispatchEnable : 1;           // 4 BitPosition                  
		/*0x000*/         UINT8        ImageDispatchEnable : 1;             // 5 BitPosition                  
		/*0x000*/         UINT8        DisableExceptionChainValidation : 1; // 6 BitPosition                  
		/*0x000*/         UINT8        Spare : 1;                           // 7 BitPosition                  
	};
	/*0x000*/     UINT8        ExecuteOptions;
}KEXECUTE_OPTIONS, *PKEXECUTE_OPTIONS;



typedef union _KSTACK_COUNT           // 3 elements, 0x4 bytes (sizeof) 
{
	/*0x000*/     LONG32       Value;
	struct                            // 2 elements, 0x4 bytes (sizeof) 
	{
		/*0x000*/         ULONG32      State : 3;       // 0 BitPosition                  
		/*0x000*/         ULONG32      StackCount : 29; // 3 BitPosition                  
	};
}KSTACK_COUNT, *PKSTACK_COUNT;




typedef union _KGDTENTRY64                    // 7 elements, 0x10 bytes (sizeof) 
{
	struct                                    // 5 elements, 0x10 bytes (sizeof) 
	{
		/*0x000*/         UINT16       LimitLow;
		/*0x002*/         UINT16       BaseLow;
		union                                 // 2 elements, 0x4 bytes (sizeof)  
		{
			struct                            // 4 elements, 0x4 bytes (sizeof)  
			{
				/*0x004*/                 UINT8        BaseMiddle;
				/*0x005*/                 UINT8        Flags1;
				/*0x006*/                 UINT8        Flags2;
				/*0x007*/                 UINT8        BaseHigh;
			}Bytes;
			struct                            // 10 elements, 0x4 bytes (sizeof) 
			{
				/*0x004*/                 ULONG32      BaseMiddle : 8;  // 0 BitPosition                   
				/*0x004*/                 ULONG32      Type : 5;        // 8 BitPosition                   
				/*0x004*/                 ULONG32      Dpl : 2;         // 13 BitPosition                  
				/*0x004*/                 ULONG32      Present : 1;     // 15 BitPosition                  
				/*0x004*/                 ULONG32      LimitHigh : 4;   // 16 BitPosition                  
				/*0x004*/                 ULONG32      System : 1;      // 20 BitPosition                  
				/*0x004*/                 ULONG32      LongMode : 1;    // 21 BitPosition                  
				/*0x004*/                 ULONG32      DefaultBig : 1;  // 22 BitPosition                  
				/*0x004*/                 ULONG32      Granularity : 1; // 23 BitPosition                  
				/*0x004*/                 ULONG32      BaseHigh : 8;    // 24 BitPosition                  
			}Bits;
		};
		/*0x008*/         ULONG32      BaseUpper;
		/*0x00C*/         ULONG32      MustBeZero;
	};
	/*0x000*/     UINT64       Alignment;
}KGDTENTRY64, *PKGDTENTRY64;



//typedef struct _KGUARDED_MUTEX              // 7 elements, 0x38 bytes (sizeof) 
//{
//	/*0x000*/     LONG32       Count;
//	/*0x004*/     UINT8        _PADDING0_[0x4];
//	/*0x008*/     struct _KTHREAD* Owner;
//	/*0x010*/     ULONG32      Contention;
//	/*0x014*/     UINT8        _PADDING1_[0x4];
//	/*0x018*/     struct _KGATE Gate;                     // 1 elements, 0x18 bytes (sizeof) 
//	union                                   // 2 elements, 0x8 bytes (sizeof)  
//	{
//		struct                              // 2 elements, 0x8 bytes (sizeof)  
//		{
//			/*0x030*/             INT16        KernelApcDisable;
//			/*0x032*/             INT16        SpecialApcDisable;
//			/*0x034*/             UINT8        _PADDING2_[0x4];
//		};
//		/*0x030*/         ULONG32      CombinedApcDisable;
//	};
//}KGUARDED_MUTEX, *PKGUARDED_MUTEX;

//typedef struct _KPROCESS                       // 37 elements, 0x160 bytes (sizeof) 
//{
//	/*0x000*/     struct _DISPATCHER_HEADER Header;          // 29 elements, 0x18 bytes (sizeof)  
//	/*0x018*/     struct _LIST_ENTRY ProfileListHead;        // 2 elements, 0x10 bytes (sizeof)   
//	/*0x028*/     UINT64       DirectoryTableBase;
//	/*0x030*/     struct _LIST_ENTRY ThreadListHead;         // 2 elements, 0x10 bytes (sizeof)   
//	/*0x040*/     UINT64       ProcessLock;
//	/*0x048*/     struct _KAFFINITY_EX Affinity;             // 4 elements, 0x28 bytes (sizeof)   
//	/*0x070*/     struct _LIST_ENTRY ReadyListHead;          // 2 elements, 0x10 bytes (sizeof)   
//	/*0x080*/     struct _SINGLE_LIST_ENTRY SwapListEntry;   // 1 elements, 0x8 bytes (sizeof)    
//	/*0x088*/     struct _KAFFINITY_EX ActiveProcessors;     // 4 elements, 0x28 bytes (sizeof)   
//	union                                      // 2 elements, 0x4 bytes (sizeof)    
//	{
//		struct                                 // 5 elements, 0x4 bytes (sizeof)    
//		{
//			/*0x0B0*/             LONG32       AutoAlignment : 1;    // 0 BitPosition                     
//			/*0x0B0*/             LONG32       DisableBoost : 1;     // 1 BitPosition                     
//			/*0x0B0*/             LONG32       DisableQuantum : 1;   // 2 BitPosition                     
//			/*0x0B0*/             ULONG32      ActiveGroupsMask : 4; // 3 BitPosition                     
//			/*0x0B0*/             LONG32       ReservedFlags : 25;   // 7 BitPosition                     
//		};
//		/*0x0B0*/         LONG32       ProcessFlags;
//	};
//	/*0x0B4*/     CHAR         BasePriority;
//	/*0x0B5*/     CHAR         QuantumReset;
//	/*0x0B6*/     UINT8        Visited;
//	/*0x0B7*/     UINT8        Unused3;
//	/*0x0B8*/     ULONG32      ThreadSeed[4];
//	/*0x0C8*/     UINT16       IdealNode[4];
//	/*0x0D0*/     UINT16       IdealGlobalNode;
//	/*0x0D2*/     union _KEXECUTE_OPTIONS Flags;             // 9 elements, 0x1 bytes (sizeof)    
//	/*0x0D3*/     UINT8        Unused1;
//	/*0x0D4*/     ULONG32      Unused2;
//	/*0x0D8*/     ULONG32      Unused4;
//	/*0x0DC*/     union _KSTACK_COUNT StackCount;            // 3 elements, 0x4 bytes (sizeof)    
//	/*0x0E0*/     struct _LIST_ENTRY ProcessListEntry;       // 2 elements, 0x10 bytes (sizeof)   
//	/*0x0F0*/     UINT64       CycleTime;
//	/*0x0F8*/     ULONG32      KernelTime;
//	/*0x0FC*/     ULONG32      UserTime;
//	/*0x100*/     VOID*        InstrumentationCallback;
//	/*0x108*/     union _KGDTENTRY64 LdtSystemDescriptor;    // 7 elements, 0x10 bytes (sizeof)   
//	/*0x118*/     VOID*        LdtBaseAddress;
//	/*0x120*/     KGUARDED_MUTEX LdtProcessLock;     // 7 elements, 0x38 bytes (sizeof)   
//	/*0x158*/     UINT16       LdtFreeSelectorHint;
//	/*0x15A*/     UINT16       LdtTableLength;
//	/*0x15C*/     UINT8        _PADDING0_[0x4];
//}KPROCESS, *PKPROCESS;



typedef struct _ALPC_PROCESS_CONTEXT  // 3 elements, 0x20 bytes (sizeof) 
{
	/*0x000*/     struct _EX_PUSH_LOCK Lock;        // 7 elements, 0x8 bytes (sizeof)  
	/*0x008*/     struct _LIST_ENTRY ViewListHead;  // 2 elements, 0x10 bytes (sizeof) 
	/*0x018*/     UINT64       PagedPoolQuotaCache;
}ALPC_PROCESS_CONTEXT, *PALPC_PROCESS_CONTEXT;


typedef struct _SE_AUDIT_PROCESS_CREATION_INFO      // 1 elements, 0x8 bytes (sizeof) 
{
	/*0x000*/     struct _OBJECT_NAME_INFORMATION* ImageFileName;
}SE_AUDIT_PROCESS_CREATION_INFO, *PSE_AUDIT_PROCESS_CREATION_INFO;



typedef struct _KPROCESS                                               // 139 elements, 0x4D0 bytes (sizeof) 
{
	 struct                     // 37 elements, 0x160 bytes (sizeof) 
	{
		/*0x000*/     struct _DISPATCHER_HEADER Header;          // 29 elements, 0x18 bytes (sizeof)  
		/*0x018*/     struct _LIST_ENTRY ProfileListHead;        // 2 elements, 0x10 bytes (sizeof)   
		/*0x028*/     UINT64       DirectoryTableBase;
		/*0x030*/     struct _LIST_ENTRY ThreadListHead;         // 2 elements, 0x10 bytes (sizeof)   
		/*0x040*/     UINT64       ProcessLock;
		/*0x048*/     struct _KAFFINITY_EX Affinity;             // 4 elements, 0x28 bytes (sizeof)   
		/*0x070*/     struct _LIST_ENTRY ReadyListHead;          // 2 elements, 0x10 bytes (sizeof)   
		/*0x080*/     struct _SINGLE_LIST_ENTRY SwapListEntry;   // 1 elements, 0x8 bytes (sizeof)    
		/*0x088*/     struct _KAFFINITY_EX ActiveProcessors;     // 4 elements, 0x28 bytes (sizeof)   
		union                                      // 2 elements, 0x4 bytes (sizeof)    
		{
			struct                                 // 5 elements, 0x4 bytes (sizeof)    
			{
				/*0x0B0*/             LONG32       AutoAlignment : 1;    // 0 BitPosition                     
				/*0x0B0*/             LONG32       DisableBoost : 1;     // 1 BitPosition                     
				/*0x0B0*/             LONG32       DisableQuantum : 1;   // 2 BitPosition                     
				/*0x0B0*/             ULONG32      ActiveGroupsMask : 4; // 3 BitPosition                     
				/*0x0B0*/             LONG32       ReservedFlags : 25;   // 7 BitPosition                     
			};
			/*0x0B0*/         LONG32       ProcessFlags;
		};
		/*0x0B4*/     CHAR         BasePriority;
		/*0x0B5*/     CHAR         QuantumReset;
		/*0x0B6*/     UINT8        Visited;
		/*0x0B7*/     UINT8        Unused3;
		/*0x0B8*/     ULONG32      ThreadSeed[4];
		/*0x0C8*/     UINT16       IdealNode[4];
		/*0x0D0*/     UINT16       IdealGlobalNode;
		/*0x0D2*/     union _KEXECUTE_OPTIONS Flags;             // 9 elements, 0x1 bytes (sizeof)    
		/*0x0D3*/     UINT8        Unused1;
		/*0x0D4*/     ULONG32      Unused2;
		/*0x0D8*/     ULONG32      Unused4;
		/*0x0DC*/     union _KSTACK_COUNT StackCount;            // 3 elements, 0x4 bytes (sizeof)    
		/*0x0E0*/     struct _LIST_ENTRY ProcessListEntry;       // 2 elements, 0x10 bytes (sizeof)   
		/*0x0F0*/     UINT64       CycleTime;
		/*0x0F8*/     ULONG32      KernelTime;
		/*0x0FC*/     ULONG32      UserTime;
		/*0x100*/     VOID*        InstrumentationCallback;
		/*0x108*/     union _KGDTENTRY64 LdtSystemDescriptor;    // 7 elements, 0x10 bytes (sizeof)   
		/*0x118*/     VOID*        LdtBaseAddress;
		/*0x120*/     KGUARDED_MUTEX LdtProcessLock;     // 7 elements, 0x38 bytes (sizeof)   
		/*0x158*/     UINT16       LdtFreeSelectorHint;
		/*0x15A*/     UINT16       LdtTableLength;
		/*0x15C*/     UINT8        _PADDING0_[0x4];
	}Pcb;
	///*0x000*/     struct _KPROCESS Pcb;                                              // 37 elements, 0x160 bytes (sizeof)  
	/*0x160*/     struct _EX_PUSH_LOCK ProcessLock;                                  // 7 elements, 0x8 bytes (sizeof)     
	/*0x168*/     union _LARGE_INTEGER CreateTime;                                   // 4 elements, 0x8 bytes (sizeof)     
	/*0x170*/     union _LARGE_INTEGER ExitTime;                                     // 4 elements, 0x8 bytes (sizeof)     
	/*0x178*/     struct _EX_RUNDOWN_REF RundownProtect;                             // 2 elements, 0x8 bytes (sizeof)     
	/*0x180*/     VOID*        UniqueProcessId;
	/*0x188*/     struct _LIST_ENTRY ActiveProcessLinks;                             // 2 elements, 0x10 bytes (sizeof)    
	/*0x198*/     UINT64       ProcessQuotaUsage[2];
	/*0x1A8*/     UINT64       ProcessQuotaPeak[2];
	/*0x1B8*/     UINT64       CommitCharge;
	/*0x1C0*/     struct _EPROCESS_QUOTA_BLOCK* QuotaBlock;
	/*0x1C8*/     struct _PS_CPU_QUOTA_BLOCK* CpuQuotaBlock;
	/*0x1D0*/     UINT64       PeakVirtualSize;
	/*0x1D8*/     UINT64       VirtualSize;
	/*0x1E0*/     struct _LIST_ENTRY SessionProcessLinks;                            // 2 elements, 0x10 bytes (sizeof)    
	/*0x1F0*/     VOID*        DebugPort;
	union                                                              // 3 elements, 0x8 bytes (sizeof)     
	{
		/*0x1F8*/         VOID*        ExceptionPortData;
		/*0x1F8*/         UINT64       ExceptionPortValue;
		/*0x1F8*/         UINT64       ExceptionPortState : 3;                           // 0 BitPosition                      
	};
	/*0x200*/     struct _HANDLE_TABLE* ObjectTable;
	/*0x208*/     struct _EX_FAST_REF Token;                                         // 3 elements, 0x8 bytes (sizeof)     
	/*0x210*/     UINT64       WorkingSetPage;
	/*0x218*/     struct _EX_PUSH_LOCK AddressCreationLock;                          // 7 elements, 0x8 bytes (sizeof)     
	/*0x220*/     struct _ETHREAD* RotateInProgress;
	/*0x228*/     struct _ETHREAD* ForkInProgress;
	/*0x230*/     UINT64       HardwareTrigger;
	/*0x238*/     struct _MM_AVL_TABLE* PhysicalVadRoot;
	/*0x240*/     VOID*        CloneRoot;
	/*0x248*/     UINT64       NumberOfPrivatePages;
	/*0x250*/     UINT64       NumberOfLockedPages;
	/*0x258*/     VOID*        Win32Process;
	/*0x260*/     struct _EJOB* Job;
	/*0x268*/     VOID*        SectionObject;
	/*0x270*/     VOID*        SectionBaseAddress;
	/*0x278*/     ULONG32      Cookie;
	/*0x27C*/     ULONG32      UmsScheduledThreads;
	/*0x280*/     struct _PAGEFAULT_HISTORY* WorkingSetWatch;
	/*0x288*/     VOID*        Win32WindowStation;
	/*0x290*/     VOID*        InheritedFromUniqueProcessId;
	/*0x298*/     VOID*        LdtInformation;
	/*0x2A0*/     VOID*        Spare;
	/*0x2A8*/     UINT64       ConsoleHostProcess;
	/*0x2B0*/     VOID*        DeviceMap;
	/*0x2B8*/     VOID*        EtwDataSource;
	/*0x2C0*/     VOID*        FreeTebHint;
	/*0x2C8*/     VOID*        FreeUmsTebHint;
	union                                                              // 2 elements, 0x8 bytes (sizeof)     
	{
		/*0x2D0*/         struct _HARDWARE_PTE PageDirectoryPte;                         // 16 elements, 0x8 bytes (sizeof)    
		/*0x2D0*/         UINT64       Filler;
	};
	/*0x2D8*/     VOID*        Session;
	/*0x2E0*/     UINT8        ImageFileName[15];
	/*0x2EF*/     UINT8        PriorityClass;
	/*0x2F0*/     struct _LIST_ENTRY JobLinks;                                       // 2 elements, 0x10 bytes (sizeof)    
	/*0x300*/     VOID*        LockedPagesList;
	/*0x308*/     struct _LIST_ENTRY ThreadListHead;                                 // 2 elements, 0x10 bytes (sizeof)    
	/*0x318*/     VOID*        SecurityPort;
	/*0x320*/     VOID*        Wow64Process;
	/*0x328*/     ULONG32      ActiveThreads;
	/*0x32C*/     ULONG32      ImagePathHash;
	/*0x330*/     ULONG32      DefaultHardErrorProcessing;
	/*0x334*/     LONG32       LastThreadExitStatus;
	/*0x338*/     struct _PEB* Peb;
	/*0x340*/     struct _EX_FAST_REF PrefetchTrace;                                 // 3 elements, 0x8 bytes (sizeof)     
	/*0x348*/     union _LARGE_INTEGER ReadOperationCount;                           // 4 elements, 0x8 bytes (sizeof)     
	/*0x350*/     union _LARGE_INTEGER WriteOperationCount;                          // 4 elements, 0x8 bytes (sizeof)     
	/*0x358*/     union _LARGE_INTEGER OtherOperationCount;                          // 4 elements, 0x8 bytes (sizeof)     
	/*0x360*/     union _LARGE_INTEGER ReadTransferCount;                            // 4 elements, 0x8 bytes (sizeof)     
	/*0x368*/     union _LARGE_INTEGER WriteTransferCount;                           // 4 elements, 0x8 bytes (sizeof)     
	/*0x370*/     union _LARGE_INTEGER OtherTransferCount;                           // 4 elements, 0x8 bytes (sizeof)     
	/*0x378*/     UINT64       CommitChargeLimit;
	/*0x380*/     UINT64       CommitChargePeak;
	/*0x388*/     VOID*        AweInfo;
	/*0x390*/     struct _SE_AUDIT_PROCESS_CREATION_INFO SeAuditProcessCreationInfo; // 1 elements, 0x8 bytes (sizeof)     
	/*0x398*/     struct _MMSUPPORT Vm;                                              // 21 elements, 0x88 bytes (sizeof)   
	/*0x420*/     struct _LIST_ENTRY MmProcessLinks;                                 // 2 elements, 0x10 bytes (sizeof)    
	/*0x430*/     VOID*        HighestUserAddress;
	/*0x438*/     ULONG32      ModifiedPageCount;
	union                                                              // 2 elements, 0x4 bytes (sizeof)     
	{
		/*0x43C*/         ULONG32      Flags2;
		struct                                                         // 24 elements, 0x4 bytes (sizeof)    
		{
			/*0x43C*/             ULONG32      JobNotReallyActive : 1;                       // 0 BitPosition                      
			/*0x43C*/             ULONG32      AccountingFolded : 1;                         // 1 BitPosition                      
			/*0x43C*/             ULONG32      NewProcessReported : 1;                       // 2 BitPosition                      
			/*0x43C*/             ULONG32      ExitProcessReported : 1;                      // 3 BitPosition                      
			/*0x43C*/             ULONG32      ReportCommitChanges : 1;                      // 4 BitPosition                      
			/*0x43C*/             ULONG32      LastReportMemory : 1;                         // 5 BitPosition                      
			/*0x43C*/             ULONG32      ReportPhysicalPageChanges : 1;                // 6 BitPosition                      
			/*0x43C*/             ULONG32      HandleTableRundown : 1;                       // 7 BitPosition                      
			/*0x43C*/             ULONG32      NeedsHandleRundown : 1;                       // 8 BitPosition                      
			/*0x43C*/             ULONG32      RefTraceEnabled : 1;                          // 9 BitPosition                      
			/*0x43C*/             ULONG32      NumaAware : 1;                                // 10 BitPosition                     
			/*0x43C*/             ULONG32      ProtectedProcess : 1;                         // 11 BitPosition                     
			/*0x43C*/             ULONG32      DefaultPagePriority : 3;                      // 12 BitPosition                     
			/*0x43C*/             ULONG32      PrimaryTokenFrozen : 1;                       // 15 BitPosition                     
			/*0x43C*/             ULONG32      ProcessVerifierTarget : 1;                    // 16 BitPosition                     
			/*0x43C*/             ULONG32      StackRandomizationDisabled : 1;               // 17 BitPosition                     
			/*0x43C*/             ULONG32      AffinityPermanent : 1;                        // 18 BitPosition                     
			/*0x43C*/             ULONG32      AffinityUpdateEnable : 1;                     // 19 BitPosition                     
			/*0x43C*/             ULONG32      PropagateNode : 1;                            // 20 BitPosition                     
			/*0x43C*/             ULONG32      ExplicitAffinity : 1;                         // 21 BitPosition                     
			/*0x43C*/             ULONG32      Spare1 : 1;                                   // 22 BitPosition                     
			/*0x43C*/             ULONG32      ForceRelocateImages : 1;                      // 23 BitPosition                     
			/*0x43C*/             ULONG32      DisallowStrippedImages : 1;                   // 24 BitPosition                     
			/*0x43C*/             ULONG32      LowVaAccessible : 1;                          // 25 BitPosition                     
		};
	};
	union                                                              // 2 elements, 0x4 bytes (sizeof)     
	{
		/*0x440*/         ULONG32      Flags;
		struct                                                         // 29 elements, 0x4 bytes (sizeof)    
		{
			/*0x440*/             ULONG32      CreateReported : 1;                           // 0 BitPosition                      
			/*0x440*/             ULONG32      NoDebugInherit : 1;                           // 1 BitPosition                      
			/*0x440*/             ULONG32      ProcessExiting : 1;                           // 2 BitPosition                      
			/*0x440*/             ULONG32      ProcessDelete : 1;                            // 3 BitPosition                      
			/*0x440*/             ULONG32      Wow64SplitPages : 1;                          // 4 BitPosition                      
			/*0x440*/             ULONG32      VmDeleted : 1;                                // 5 BitPosition                      
			/*0x440*/             ULONG32      OutswapEnabled : 1;                           // 6 BitPosition                      
			/*0x440*/             ULONG32      Outswapped : 1;                               // 7 BitPosition                      
			/*0x440*/             ULONG32      ForkFailed : 1;                               // 8 BitPosition                      
			/*0x440*/             ULONG32      Wow64VaSpace4Gb : 1;                          // 9 BitPosition                      
			/*0x440*/             ULONG32      AddressSpaceInitialized : 2;                  // 10 BitPosition                     
			/*0x440*/             ULONG32      SetTimerResolution : 1;                       // 12 BitPosition                     
			/*0x440*/             ULONG32      BreakOnTermination : 1;                       // 13 BitPosition                     
			/*0x440*/             ULONG32      DeprioritizeViews : 1;                        // 14 BitPosition                     
			/*0x440*/             ULONG32      WriteWatch : 1;                               // 15 BitPosition                     
			/*0x440*/             ULONG32      ProcessInSession : 1;                         // 16 BitPosition                     
			/*0x440*/             ULONG32      OverrideAddressSpace : 1;                     // 17 BitPosition                     
			/*0x440*/             ULONG32      HasAddressSpace : 1;                          // 18 BitPosition                     
			/*0x440*/             ULONG32      LaunchPrefetched : 1;                         // 19 BitPosition                     
			/*0x440*/             ULONG32      InjectInpageErrors : 1;                       // 20 BitPosition                     
			/*0x440*/             ULONG32      VmTopDown : 1;                                // 21 BitPosition                     
			/*0x440*/             ULONG32      ImageNotifyDone : 1;                          // 22 BitPosition                     
			/*0x440*/             ULONG32      PdeUpdateNeeded : 1;                          // 23 BitPosition                     
			/*0x440*/             ULONG32      VdmAllowed : 1;                               // 24 BitPosition                     
			/*0x440*/             ULONG32      CrossSessionCreate : 1;                       // 25 BitPosition                     
			/*0x440*/             ULONG32      ProcessInserted : 1;                          // 26 BitPosition                     
			/*0x440*/             ULONG32      DefaultIoPriority : 3;                        // 27 BitPosition                     
			/*0x440*/             ULONG32      ProcessSelfDelete : 1;                        // 30 BitPosition                     
			/*0x440*/             ULONG32      SetTimerResolutionLink : 1;                   // 31 BitPosition                     
		};
	};
	/*0x444*/     LONG32       ExitStatus;
	/*0x448*/     struct _MM_AVL_TABLE VadRoot;                                      // 6 elements, 0x40 bytes (sizeof)    
	/*0x488*/     struct _ALPC_PROCESS_CONTEXT AlpcContext;                          // 3 elements, 0x20 bytes (sizeof)    
	/*0x4A8*/     struct _LIST_ENTRY TimerResolutionLink;                            // 2 elements, 0x10 bytes (sizeof)    
	/*0x4B8*/     ULONG32      RequestedTimerResolution;
	/*0x4BC*/     ULONG32      ActiveThreadsHighWatermark;
	/*0x4C0*/     ULONG32      SmallestTimerResolution;
	/*0x4C4*/     UINT8        _PADDING0_[0x4];
	/*0x4C8*/     struct _PO_DIAG_STACK_RECORD* TimerResolutionStackRecord;
} *PEPROCESS;


EXTERN_C
{
NTSYSAPI
NTSTATUS
PsTerminateSystemThread(
	__in NTSTATUS ExitStatus
);

}