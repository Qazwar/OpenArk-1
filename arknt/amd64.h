#pragma once





typedef struct _KDESCRIPTOR // 3 elements, 0x10 bytes (sizeof) 
{
	/*0x000*/     UINT16       Pad[3];
	/*0x006*/     UINT16       Limit;
	/*0x008*/     VOID*        Base;
}KDESCRIPTOR, *PKDESCRIPTOR;


typedef struct _KAFFINITY_EX // 4 elements, 0x28 bytes (sizeof) 
{
	/*0x000*/     UINT16       Count;
	/*0x002*/     UINT16       Size;
	/*0x004*/     ULONG32      Reserved;
	/*0x008*/     UINT64       Bitmap[4];
}KAFFINITY_EX, *PKAFFINITY_EX;
typedef struct _KSPECIAL_REGISTERS     // 27 elements, 0xD8 bytes (sizeof) 
{
	/*0x000*/     UINT64       Cr0;
	/*0x008*/     UINT64       Cr2;
	/*0x010*/     UINT64       Cr3;
	/*0x018*/     UINT64       Cr4;
	/*0x020*/     UINT64       KernelDr0;
	/*0x028*/     UINT64       KernelDr1;
	/*0x030*/     UINT64       KernelDr2;
	/*0x038*/     UINT64       KernelDr3;
	/*0x040*/     UINT64       KernelDr6;
	/*0x048*/     UINT64       KernelDr7;
	/*0x050*/     struct _KDESCRIPTOR Gdtr;          // 3 elements, 0x10 bytes (sizeof)  
	/*0x060*/     struct _KDESCRIPTOR Idtr;          // 3 elements, 0x10 bytes (sizeof)  
	/*0x070*/     UINT16       Tr;
	/*0x072*/     UINT16       Ldtr;
	/*0x074*/     ULONG32      MxCsr;
	/*0x078*/     UINT64       DebugControl;
	/*0x080*/     UINT64       LastBranchToRip;
	/*0x088*/     UINT64       LastBranchFromRip;
	/*0x090*/     UINT64       LastExceptionToRip;
	/*0x098*/     UINT64       LastExceptionFromRip;
	/*0x0A0*/     UINT64       Cr8;
	/*0x0A8*/     UINT64       MsrGsBase;
	/*0x0B0*/     UINT64       MsrGsSwap;
	/*0x0B8*/     UINT64       MsrStar;
	/*0x0C0*/     UINT64       MsrLStar;
	/*0x0C8*/     UINT64       MsrCStar;
	/*0x0D0*/     UINT64       MsrSyscallMask;
}KSPECIAL_REGISTERS, *PKSPECIAL_REGISTERS;
typedef struct _KTIMER_TABLE_ENTRY // 3 elements, 0x20 bytes (sizeof) 
{
	/*0x000*/     UINT64       Lock;
	/*0x008*/     struct _LIST_ENTRY Entry;      // 2 elements, 0x10 bytes (sizeof) 
	/*0x018*/     union _ULARGE_INTEGER Time;    // 4 elements, 0x8 bytes (sizeof)  
}KTIMER_TABLE_ENTRY, *PKTIMER_TABLE_ENTRY;
typedef struct _KPROCESSOR_STATE                 // 2 elements, 0x5B0 bytes (sizeof)  
{
	/*0x000*/     struct _KSPECIAL_REGISTERS SpecialRegisters; // 27 elements, 0xD8 bytes (sizeof)  
	/*0x0D8*/     UINT8        _PADDING0_[0x8];
	/*0x0E0*/     struct _CONTEXT ContextFrame;                // 64 elements, 0x4D0 bytes (sizeof) 
}KPROCESSOR_STATE, *PKPROCESSOR_STATE;

typedef struct _KREQUEST_PACKET                   // 2 elements, 0x20 bytes (sizeof) 
{
	/*0x000*/     void*        CurrentPacket[3];
	/*0x018*/     void* WorkerRoutine;
}KREQUEST_PACKET, *PKREQUEST_PACKET;


typedef struct _KDPC_DATA           // 4 elements, 0x20 bytes (sizeof) 
{
	/*0x000*/     struct _LIST_ENTRY DpcListHead; // 2 elements, 0x10 bytes (sizeof) 
	/*0x010*/     UINT64       DpcLock;
	/*0x018*/     LONG32       DpcQueueDepth;
	/*0x01C*/     ULONG32      DpcCount;
}KDPC_DATA, *PKDPC_DATA;


typedef struct _REQUEST_MAILBOX            // 3 elements, 0x40 bytes (sizeof) 
{
	/*0x000*/     struct _REQUEST_MAILBOX* Next;
	/*0x008*/     INT64        RequestSummary;
	/*0x010*/     struct _KREQUEST_PACKET RequestPacket; // 2 elements, 0x20 bytes (sizeof) 
	/*0x030*/     UINT8        _PADDING0_[0x10];
}REQUEST_MAILBOX, *PREQUEST_MAILBOX;


typedef struct _PP_LOOKASIDE_LIST // 2 elements, 0x10 bytes (sizeof) 
{
	/*0x000*/     struct _GENERAL_LOOKASIDE* P;
	/*0x008*/     struct _GENERAL_LOOKASIDE* L;
}PP_LOOKASIDE_LIST, *PPP_LOOKASIDE_LIST;

typedef struct _KTIMER_TABLE                      // 2 elements, 0x2200 bytes (sizeof) 
{
	/*0x000*/     struct _KTIMER* TimerExpiry[64];
	/*0x200*/     struct _KTIMER_TABLE_ENTRY TimerEntries[256];
}KTIMER_TABLE, *PKTIMER_TABLE;









typedef struct _PROC_IDLE_SNAP // 2 elements, 0x10 bytes (sizeof) 
{
	/*0x000*/     UINT64       Time;
	/*0x008*/     UINT64       Idle;
}PROC_IDLE_SNAP, *PPROC_IDLE_SNAP;
typedef struct _PPM_FFH_THROTTLE_STATE_INFO // 5 elements, 0x20 bytes (sizeof) 
{
	/*0x000*/     UINT8        EnableLogging;
	/*0x001*/     UINT8        _PADDING0_[0x3];
	/*0x004*/     ULONG32      MismatchCount;
	/*0x008*/     UINT8        Initialized;
	/*0x009*/     UINT8        _PADDING1_[0x7];
	/*0x010*/     UINT64       LastValue;
	/*0x018*/     union _LARGE_INTEGER LastLogTickCount;  // 4 elements, 0x8 bytes (sizeof)  
}PPM_FFH_THROTTLE_STATE_INFO, *PPPM_FFH_THROTTLE_STATE_INFO;
typedef struct _PROCESSOR_POWER_STATE                         // 27 elements, 0x100 bytes (sizeof) 
{
	/*0x000*/     struct _PPM_IDLE_STATES* IdleStates;
	/*0x008*/     UINT64       IdleTimeLast;
	/*0x010*/     UINT64       IdleTimeTotal;
	/*0x018*/     UINT64       IdleTimeEntry;
	/*0x020*/     struct _PROC_IDLE_ACCOUNTING* IdleAccounting;
	/*0x028*/     enum _PROC_HYPERVISOR_STATE Hypervisor;
	/*0x02C*/     ULONG32      PerfHistoryTotal;
	/*0x030*/     UINT8        ThermalConstraint;
	/*0x031*/     UINT8        PerfHistoryCount;
	/*0x032*/     UINT8        PerfHistorySlot;
	/*0x033*/     UINT8        Reserved;
	/*0x034*/     ULONG32      LastSysTime;
	/*0x038*/     UINT64       WmiDispatchPtr;
	/*0x040*/     LONG32       WmiInterfaceEnabled;
	/*0x044*/     UINT8        _PADDING0_[0x4];
	/*0x048*/     struct _PPM_FFH_THROTTLE_STATE_INFO FFHThrottleStateInfo; // 5 elements, 0x20 bytes (sizeof)   
	/*0x068*/     struct _KDPC PerfActionDpc;                               // 9 elements, 0x40 bytes (sizeof)   
	/*0x0A8*/     LONG32       PerfActionMask;
	/*0x0AC*/     UINT8        _PADDING1_[0x4];
	/*0x0B0*/     struct _PROC_IDLE_SNAP IdleCheck;                         // 2 elements, 0x10 bytes (sizeof)   
	/*0x0C0*/     struct _PROC_IDLE_SNAP PerfCheck;                         // 2 elements, 0x10 bytes (sizeof)   
	/*0x0D0*/     struct _PROC_PERF_DOMAIN* Domain;
	/*0x0D8*/     struct _PROC_PERF_CONSTRAINT* PerfConstraint;
	/*0x0E0*/     struct _PROC_PERF_LOAD* Load;
	/*0x0E8*/     struct _PROC_HISTORY_ENTRY* PerfHistory;
	/*0x0F0*/     ULONG32      Utility;
	/*0x0F4*/     ULONG32      OverUtilizedHistory;
	/*0x0F8*/     ULONG32      AffinityCount;
	/*0x0FC*/     ULONG32      AffinityHistory;
}PROCESSOR_POWER_STATE, *PPROCESSOR_POWER_STATE;


typedef struct _KPRCB                                                   // 242 elements, 0x4D00 bytes (sizeof) 
{
	/*0x000*/      ULONG32      MxCsr;
	/*0x004*/      UINT8        LegacyNumber;
	/*0x005*/      UINT8        ReservedMustBeZero;
	/*0x006*/      UINT8        InterruptRequest;
	/*0x007*/      UINT8        IdleHalt;
	/*0x008*/      struct _KTHREAD* CurrentThread;
	/*0x010*/      struct _KTHREAD* NextThread;
	/*0x018*/      struct _KTHREAD* IdleThread;
	/*0x020*/      UINT8        NestingLevel;
	/*0x021*/      UINT8        PrcbPad00[3];
	/*0x024*/      ULONG32      Number;
	/*0x028*/      UINT64       RspBase;
	/*0x030*/      UINT64       PrcbLock;
	/*0x038*/      UINT64       PrcbPad01;
	/*0x040*/      struct _KPROCESSOR_STATE ProcessorState;                            // 2 elements, 0x5B0 bytes (sizeof)    
	/*0x5F0*/      CHAR         CpuType;
	/*0x5F1*/      CHAR         CpuID;
	union                                                               // 2 elements, 0x2 bytes (sizeof)      
	{
		/*0x5F2*/          UINT16       CpuStep;
		struct                                                          // 2 elements, 0x2 bytes (sizeof)      
		{
			/*0x5F2*/              UINT8        CpuStepping;
			/*0x5F3*/              UINT8        CpuModel;
		};
	};
	/*0x5F4*/      ULONG32      MHz;
	/*0x5F8*/      UINT64       HalReserved[8];
	/*0x638*/      UINT16       MinorVersion;
	/*0x63A*/      UINT16       MajorVersion;
	/*0x63C*/      UINT8        BuildType;
	/*0x63D*/      UINT8        CpuVendor;
	/*0x63E*/      UINT8        CoresPerPhysicalProcessor;
	/*0x63F*/      UINT8        LogicalProcessorsPerCore;
	/*0x640*/      ULONG32      ApicMask;
	/*0x644*/      ULONG32      CFlushSize;
	/*0x648*/      VOID*        AcpiReserved;
	/*0x650*/      ULONG32      InitialApicId;
	/*0x654*/      ULONG32      Stride;
	/*0x658*/      UINT16       Group;
	/*0x65A*/      UINT8        _PADDING0_[0x6];
	/*0x660*/      UINT64       GroupSetMember;
	/*0x668*/      UINT8        GroupIndex;
	/*0x669*/      UINT8        _PADDING1_[0x7];
	/*0x670*/      struct _KSPIN_LOCK_QUEUE LockQueue[17];
	/*0x780*/      struct _PP_LOOKASIDE_LIST PPLookasideList[16];
	/*0x880*/      struct _GENERAL_LOOKASIDE_POOL PPNPagedLookasideList[32];
	/*0x1480*/     struct _GENERAL_LOOKASIDE_POOL PPPagedLookasideList[32];
	/*0x2080*/     LONG32       PacketBarrier;
	/*0x2084*/     UINT8        _PADDING2_[0x4];
	/*0x2088*/     struct _SINGLE_LIST_ENTRY DeferredReadyListHead;                    // 1 elements, 0x8 bytes (sizeof)      
	/*0x2090*/     LONG32       MmPageFaultCount;
	/*0x2094*/     LONG32       MmCopyOnWriteCount;
	/*0x2098*/     LONG32       MmTransitionCount;
	/*0x209C*/     LONG32       MmDemandZeroCount;
	/*0x20A0*/     LONG32       MmPageReadCount;
	/*0x20A4*/     LONG32       MmPageReadIoCount;
	/*0x20A8*/     LONG32       MmDirtyPagesWriteCount;
	/*0x20AC*/     LONG32       MmDirtyWriteIoCount;
	/*0x20B0*/     LONG32       MmMappedPagesWriteCount;
	/*0x20B4*/     LONG32       MmMappedWriteIoCount;
	/*0x20B8*/     ULONG32      KeSystemCalls;
	/*0x20BC*/     ULONG32      KeContextSwitches;
	/*0x20C0*/     ULONG32      CcFastReadNoWait;
	/*0x20C4*/     ULONG32      CcFastReadWait;
	/*0x20C8*/     ULONG32      CcFastReadNotPossible;
	/*0x20CC*/     ULONG32      CcCopyReadNoWait;
	/*0x20D0*/     ULONG32      CcCopyReadWait;
	/*0x20D4*/     ULONG32      CcCopyReadNoWaitMiss;
	/*0x20D8*/     LONG32       LookasideIrpFloat;
	/*0x20DC*/     LONG32       IoReadOperationCount;
	/*0x20E0*/     LONG32       IoWriteOperationCount;
	/*0x20E4*/     LONG32       IoOtherOperationCount;
	/*0x20E8*/     union _LARGE_INTEGER IoReadTransferCount;                           // 4 elements, 0x8 bytes (sizeof)      
	/*0x20F0*/     union _LARGE_INTEGER IoWriteTransferCount;                          // 4 elements, 0x8 bytes (sizeof)      
	/*0x20F8*/     union _LARGE_INTEGER IoOtherTransferCount;                          // 4 elements, 0x8 bytes (sizeof)      
	/*0x2100*/     LONG32       TargetCount;
	/*0x2104*/     ULONG32      IpiFrozen;
	/*0x2108*/     UINT8        _PADDING3_[0x78];
	/*0x2180*/     struct _KDPC_DATA DpcData[2];
	/*0x21C0*/     VOID*        DpcStack;
	/*0x21C8*/     LONG32       MaximumDpcQueueDepth;
	/*0x21CC*/     ULONG32      DpcRequestRate;
	/*0x21D0*/     ULONG32      MinimumDpcRate;
	/*0x21D4*/     ULONG32      DpcLastCount;
	/*0x21D8*/     UINT8        ThreadDpcEnable;
	/*0x21D9*/     UINT8        QuantumEnd;
	/*0x21DA*/     UINT8        DpcRoutineActive;
	/*0x21DB*/     UINT8        IdleSchedule;
	union                                                               // 3 elements, 0x4 bytes (sizeof)      
	{
		/*0x21DC*/         LONG32       DpcRequestSummary;
		/*0x21DC*/         INT16        DpcRequestSlot[2];
		struct                                                          // 2 elements, 0x4 bytes (sizeof)      
		{
			/*0x21DC*/             INT16        NormalDpcState;
			union                                                       // 2 elements, 0x2 bytes (sizeof)      
			{
				/*0x21DE*/                 UINT16       DpcThreadActive : 1;                       // 0 BitPosition                       
				/*0x21DE*/                 INT16        ThreadDpcState;
			};
		};
	};
	/*0x21E0*/     ULONG32      TimerHand;
	/*0x21E4*/     LONG32       MasterOffset;
	/*0x21E8*/     ULONG32      LastTick;
	/*0x21EC*/     ULONG32      UnusedPad;
	/*0x21F0*/     UINT64       PrcbPad50[2];
	/*0x2200*/     struct _KTIMER_TABLE TimerTable;                                    // 2 elements, 0x2200 bytes (sizeof)   
	/*0x4400*/     struct _KGATE DpcGate;                                              // 1 elements, 0x18 bytes (sizeof)     
	/*0x4418*/     VOID*        PrcbPad52;
	/*0x4420*/     struct _KDPC CallDpc;                                               // 9 elements, 0x40 bytes (sizeof)     
	/*0x4460*/     LONG32       ClockKeepAlive;
	/*0x4464*/     UINT8        ClockCheckSlot;
	/*0x4465*/     UINT8        ClockPollCycle;
	/*0x4466*/     UINT16       NmiActive;
	/*0x4468*/     LONG32       DpcWatchdogPeriod;
	/*0x446C*/     LONG32       DpcWatchdogCount;
	/*0x4470*/     UINT64       TickOffset;
	/*0x4478*/     LONG32       KeSpinLockOrdering;
	/*0x447C*/     ULONG32      PrcbPad70;
	/*0x4480*/     struct _LIST_ENTRY WaitListHead;                                    // 2 elements, 0x10 bytes (sizeof)     
	/*0x4490*/     UINT64       WaitLock;
	/*0x4498*/     ULONG32      ReadySummary;
	/*0x449C*/     ULONG32      QueueIndex;
	/*0x44A0*/     struct _KDPC TimerExpirationDpc;                                    // 9 elements, 0x40 bytes (sizeof)     
	/*0x44E0*/     UINT64       PrcbPad72[4];
	/*0x4500*/     struct _LIST_ENTRY DispatcherReadyListHead[32];
	/*0x4700*/     ULONG32      InterruptCount;
	/*0x4704*/     ULONG32      KernelTime;
	/*0x4708*/     ULONG32      UserTime;
	/*0x470C*/     ULONG32      DpcTime;
	/*0x4710*/     ULONG32      InterruptTime;
	/*0x4714*/     ULONG32      AdjustDpcThreshold;
	/*0x4718*/     UINT8        DebuggerSavedIRQL;
	/*0x4719*/     UINT8        PrcbPad80[7];
	/*0x4720*/     ULONG32      DpcTimeCount;
	/*0x4724*/     ULONG32      DpcTimeLimit;
	/*0x4728*/     ULONG32      PeriodicCount;
	/*0x472C*/     ULONG32      PeriodicBias;
	/*0x4730*/     ULONG32      AvailableTime;
	/*0x4734*/     ULONG32      KeExceptionDispatchCount;
	/*0x4738*/     struct _KNODE* ParentNode;
	/*0x4740*/     UINT64       StartCycles;
	/*0x4748*/     UINT64       PrcbPad82[3];
	/*0x4760*/     LONG32       MmSpinLockOrdering;
	/*0x4764*/     ULONG32      PageColor;
	/*0x4768*/     ULONG32      NodeColor;
	/*0x476C*/     ULONG32      NodeShiftedColor;
	/*0x4770*/     ULONG32      SecondaryColorMask;
	/*0x4774*/     ULONG32      PrcbPad83;
	/*0x4778*/     UINT64       CycleTime;
	/*0x4780*/     ULONG32      CcFastMdlReadNoWait;
	/*0x4784*/     ULONG32      CcFastMdlReadWait;
	/*0x4788*/     ULONG32      CcFastMdlReadNotPossible;
	/*0x478C*/     ULONG32      CcMapDataNoWait;
	/*0x4790*/     ULONG32      CcMapDataWait;
	/*0x4794*/     ULONG32      CcPinMappedDataCount;
	/*0x4798*/     ULONG32      CcPinReadNoWait;
	/*0x479C*/     ULONG32      CcPinReadWait;
	/*0x47A0*/     ULONG32      CcMdlReadNoWait;
	/*0x47A4*/     ULONG32      CcMdlReadWait;
	/*0x47A8*/     ULONG32      CcLazyWriteHotSpots;
	/*0x47AC*/     ULONG32      CcLazyWriteIos;
	/*0x47B0*/     ULONG32      CcLazyWritePages;
	/*0x47B4*/     ULONG32      CcDataFlushes;
	/*0x47B8*/     ULONG32      CcDataPages;
	/*0x47BC*/     ULONG32      CcLostDelayedWrites;
	/*0x47C0*/     ULONG32      CcFastReadResourceMiss;
	/*0x47C4*/     ULONG32      CcCopyReadWaitMiss;
	/*0x47C8*/     ULONG32      CcFastMdlReadResourceMiss;
	/*0x47CC*/     ULONG32      CcMapDataNoWaitMiss;
	/*0x47D0*/     ULONG32      CcMapDataWaitMiss;
	/*0x47D4*/     ULONG32      CcPinReadNoWaitMiss;
	/*0x47D8*/     ULONG32      CcPinReadWaitMiss;
	/*0x47DC*/     ULONG32      CcMdlReadNoWaitMiss;
	/*0x47E0*/     ULONG32      CcMdlReadWaitMiss;
	/*0x47E4*/     ULONG32      CcReadAheadIos;
	/*0x47E8*/     LONG32       MmCacheTransitionCount;
	/*0x47EC*/     LONG32       MmCacheReadCount;
	/*0x47F0*/     LONG32       MmCacheIoCount;
	/*0x47F4*/     ULONG32      PrcbPad91[1];
	/*0x47F8*/     UINT64       RuntimeAccumulation;
	/*0x4800*/     struct _PROCESSOR_POWER_STATE PowerState;                           // 27 elements, 0x100 bytes (sizeof)   
	/*0x4900*/     UINT8        PrcbPad92[16];
	/*0x4910*/     ULONG32      KeAlignmentFixupCount;
	/*0x4914*/     UINT8        _PADDING4_[0x4];
	/*0x4918*/     struct _KDPC DpcWatchdogDpc;                                        // 9 elements, 0x40 bytes (sizeof)     
	/*0x4958*/     struct _KTIMER DpcWatchdogTimer;                                    // 6 elements, 0x40 bytes (sizeof)     
	/*0x4998*/     struct _CACHE_DESCRIPTOR Cache[5];
	/*0x49D4*/     ULONG32      CacheCount;
	/*0x49D8*/     ULONG32      CachedCommit;
	/*0x49DC*/     ULONG32      CachedResidentAvailable;
	/*0x49E0*/     VOID*        HyperPte;
	/*0x49E8*/     VOID*        WheaInfo;
	/*0x49F0*/     VOID*        EtwSupport;
	/*0x49F8*/     UINT8        _PADDING5_[0x8];
	/*0x4A00*/     union _SLIST_HEADER InterruptObjectPool;                            // 5 elements, 0x10 bytes (sizeof)     
	/*0x4A10*/     union _SLIST_HEADER HypercallPageList;                              // 5 elements, 0x10 bytes (sizeof)     
	/*0x4A20*/     VOID*        HypercallPageVirtual;
	/*0x4A28*/     VOID*        VirtualApicAssist;
	/*0x4A30*/     UINT64*      StatisticsPage;
	/*0x4A38*/     VOID*        RateControl;
	/*0x4A40*/     UINT64       CacheProcessorMask[5];
	/*0x4A68*/     struct _KAFFINITY_EX PackageProcessorSet;                           // 4 elements, 0x28 bytes (sizeof)     
	/*0x4A90*/     UINT64       CoreProcessorSet;
	/*0x4A98*/     VOID*        PebsIndexAddress;
	/*0x4AA0*/     UINT64       PrcbPad93[12];
	/*0x4B00*/     ULONG32      SpinLockAcquireCount;
	/*0x4B04*/     ULONG32      SpinLockContentionCount;
	/*0x4B08*/     ULONG32      SpinLockSpinCount;
	/*0x4B0C*/     ULONG32      IpiSendRequestBroadcastCount;
	/*0x4B10*/     ULONG32      IpiSendRequestRoutineCount;
	/*0x4B14*/     ULONG32      IpiSendSoftwareInterruptCount;
	/*0x4B18*/     ULONG32      ExInitializeResourceCount;
	/*0x4B1C*/     ULONG32      ExReInitializeResourceCount;
	/*0x4B20*/     ULONG32      ExDeleteResourceCount;
	/*0x4B24*/     ULONG32      ExecutiveResourceAcquiresCount;
	/*0x4B28*/     ULONG32      ExecutiveResourceContentionsCount;
	/*0x4B2C*/     ULONG32      ExecutiveResourceReleaseExclusiveCount;
	/*0x4B30*/     ULONG32      ExecutiveResourceReleaseSharedCount;
	/*0x4B34*/     ULONG32      ExecutiveResourceConvertsCount;
	/*0x4B38*/     ULONG32      ExAcqResExclusiveAttempts;
	/*0x4B3C*/     ULONG32      ExAcqResExclusiveAcquiresExclusive;
	/*0x4B40*/     ULONG32      ExAcqResExclusiveAcquiresExclusiveRecursive;
	/*0x4B44*/     ULONG32      ExAcqResExclusiveWaits;
	/*0x4B48*/     ULONG32      ExAcqResExclusiveNotAcquires;
	/*0x4B4C*/     ULONG32      ExAcqResSharedAttempts;
	/*0x4B50*/     ULONG32      ExAcqResSharedAcquiresExclusive;
	/*0x4B54*/     ULONG32      ExAcqResSharedAcquiresShared;
	/*0x4B58*/     ULONG32      ExAcqResSharedAcquiresSharedRecursive;
	/*0x4B5C*/     ULONG32      ExAcqResSharedWaits;
	/*0x4B60*/     ULONG32      ExAcqResSharedNotAcquires;
	/*0x4B64*/     ULONG32      ExAcqResSharedStarveExclusiveAttempts;
	/*0x4B68*/     ULONG32      ExAcqResSharedStarveExclusiveAcquiresExclusive;
	/*0x4B6C*/     ULONG32      ExAcqResSharedStarveExclusiveAcquiresShared;
	/*0x4B70*/     ULONG32      ExAcqResSharedStarveExclusiveAcquiresSharedRecursive;
	/*0x4B74*/     ULONG32      ExAcqResSharedStarveExclusiveWaits;
	/*0x4B78*/     ULONG32      ExAcqResSharedStarveExclusiveNotAcquires;
	/*0x4B7C*/     ULONG32      ExAcqResSharedWaitForExclusiveAttempts;
	/*0x4B80*/     ULONG32      ExAcqResSharedWaitForExclusiveAcquiresExclusive;
	/*0x4B84*/     ULONG32      ExAcqResSharedWaitForExclusiveAcquiresShared;
	/*0x4B88*/     ULONG32      ExAcqResSharedWaitForExclusiveAcquiresSharedRecursive;
	/*0x4B8C*/     ULONG32      ExAcqResSharedWaitForExclusiveWaits;
	/*0x4B90*/     ULONG32      ExAcqResSharedWaitForExclusiveNotAcquires;
	/*0x4B94*/     ULONG32      ExSetResOwnerPointerExclusive;
	/*0x4B98*/     ULONG32      ExSetResOwnerPointerSharedNew;
	/*0x4B9C*/     ULONG32      ExSetResOwnerPointerSharedOld;
	/*0x4BA0*/     ULONG32      ExTryToAcqExclusiveAttempts;
	/*0x4BA4*/     ULONG32      ExTryToAcqExclusiveAcquires;
	/*0x4BA8*/     ULONG32      ExBoostExclusiveOwner;
	/*0x4BAC*/     ULONG32      ExBoostSharedOwners;
	/*0x4BB0*/     ULONG32      ExEtwSynchTrackingNotificationsCount;
	/*0x4BB4*/     ULONG32      ExEtwSynchTrackingNotificationsAccountedCount;
	/*0x4BB8*/     UINT8        VendorString[13];
	/*0x4BC5*/     UINT8        PrcbPad10[3];
	/*0x4BC8*/     ULONG32      FeatureBits;
	/*0x4BCC*/     UINT8        _PADDING6_[0x4];
	/*0x4BD0*/     union _LARGE_INTEGER UpdateSignature;                               // 4 elements, 0x8 bytes (sizeof)      
	/*0x4BD8*/     struct _CONTEXT* Context;
	/*0x4BE0*/     ULONG32      ContextFlags;
	/*0x4BE4*/     UINT8        _PADDING7_[0x4];
	/*0x4BE8*/     struct _XSAVE_AREA* ExtendedState;
	/*0x4BF0*/     UINT8        _PADDING8_[0x10];
	/*0x4C00*/     struct _REQUEST_MAILBOX* Mailbox;
	/*0x4C08*/     UINT8        _PADDING9_[0x78];
	/*0x4C80*/     struct _REQUEST_MAILBOX RequestMailbox[1];
	/*0x4CC0*/     UINT8        _PADDING10_[0x40];
}KPRCB, *PKPRCB;
