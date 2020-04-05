#ifndef NTKE_H
#define NTKE_H

typedef union _KWAIT_STATUS_REGISTER // 8 elements, 0x1 bytes (sizeof) 
{
	/*0x000*/     UINT8        Flags;
	struct                           // 7 elements, 0x1 bytes (sizeof) 
	{
		/*0x000*/         UINT8        State : 2;      // 0 BitPosition                  
		/*0x000*/         UINT8        Affinity : 1;   // 2 BitPosition                  
		/*0x000*/         UINT8        Priority : 1;   // 3 BitPosition                  
		/*0x000*/         UINT8        Apc : 1;        // 4 BitPosition                  
		/*0x000*/         UINT8        UserApc : 1;    // 5 BitPosition                  
		/*0x000*/         UINT8        Alert : 1;      // 6 BitPosition                  
		/*0x000*/         UINT8        Unused : 1;     // 7 BitPosition                  
	};
}KWAIT_STATUS_REGISTER, *PKWAIT_STATUS_REGISTER;

//typedef struct _KTHREAD                                 // 130 elements, 0x368 bytes (sizeof) 
//{
//	/*0x000*/     struct _DISPATCHER_HEADER Header;                   // 29 elements, 0x18 bytes (sizeof)   
//	/*0x018*/     UINT64       CycleTime;
//	/*0x020*/     UINT64       QuantumTarget;
//	/*0x028*/     VOID*        InitialStack;
//	/*0x030*/     VOID*        StackLimit;
//	/*0x038*/     VOID*        KernelStack;
//	/*0x040*/     UINT64       ThreadLock;
//	/*0x048*/     union _KWAIT_STATUS_REGISTER WaitRegister;          // 8 elements, 0x1 bytes (sizeof)     
//	/*0x049*/     UINT8        Running;
//	/*0x04A*/     UINT8        Alerted[2];
//	union                                               // 2 elements, 0x4 bytes (sizeof)     
//	{
//		struct                                          // 15 elements, 0x4 bytes (sizeof)    
//		{
//			/*0x04C*/             ULONG32      KernelStackResident : 1;       // 0 BitPosition                      
//			/*0x04C*/             ULONG32      ReadyTransition : 1;           // 1 BitPosition                      
//			/*0x04C*/             ULONG32      ProcessReadyQueue : 1;         // 2 BitPosition                      
//			/*0x04C*/             ULONG32      WaitNext : 1;                  // 3 BitPosition                      
//			/*0x04C*/             ULONG32      SystemAffinityActive : 1;      // 4 BitPosition                      
//			/*0x04C*/             ULONG32      Alertable : 1;                 // 5 BitPosition                      
//			/*0x04C*/             ULONG32      GdiFlushActive : 1;            // 6 BitPosition                      
//			/*0x04C*/             ULONG32      UserStackWalkActive : 1;       // 7 BitPosition                      
//			/*0x04C*/             ULONG32      ApcInterruptRequest : 1;       // 8 BitPosition                      
//			/*0x04C*/             ULONG32      ForceDeferSchedule : 1;        // 9 BitPosition                      
//			/*0x04C*/             ULONG32      QuantumEndMigrate : 1;         // 10 BitPosition                     
//			/*0x04C*/             ULONG32      UmsDirectedSwitchEnable : 1;   // 11 BitPosition                     
//			/*0x04C*/             ULONG32      TimerActive : 1;               // 12 BitPosition                     
//			/*0x04C*/             ULONG32      SystemThread : 1;              // 13 BitPosition                     
//			/*0x04C*/             ULONG32      Reserved : 18;                 // 14 BitPosition                     
//		};
//		/*0x04C*/         LONG32       MiscFlags;
//	};
//	union                                               // 2 elements, 0x30 bytes (sizeof)    
//	{
//		/*0x050*/         struct _KAPC_STATE ApcState;                    // 5 elements, 0x30 bytes (sizeof)    
//		struct                                          // 3 elements, 0x30 bytes (sizeof)    
//		{
//			/*0x050*/             UINT8        ApcStateFill[43];
//			/*0x07B*/             CHAR         Priority;
//			/*0x07C*/             ULONG32      NextProcessor;
//		};
//	};
//	/*0x080*/     ULONG32      DeferredProcessor;
//	/*0x084*/     UINT8        _PADDING0_[0x4];
//	/*0x088*/     UINT64       ApcQueueLock;
//	/*0x090*/     INT64        WaitStatus;
//	/*0x098*/     struct _KWAIT_BLOCK* WaitBlockList;
//	union                                               // 2 elements, 0x10 bytes (sizeof)    
//	{
//		/*0x0A0*/         struct _LIST_ENTRY WaitListEntry;               // 2 elements, 0x10 bytes (sizeof)    
//		/*0x0A0*/         struct _SINGLE_LIST_ENTRY SwapListEntry;        // 1 elements, 0x8 bytes (sizeof)     
//	};
//	/*0x0B0*/     struct _KQUEUE* Queue;
//	/*0x0B8*/     VOID*        Teb;
//	/*0x0C0*/     struct _KTIMER Timer;                               // 6 elements, 0x40 bytes (sizeof)    
//	union                                               // 2 elements, 0x4 bytes (sizeof)     
//	{
//		struct                                          // 12 elements, 0x4 bytes (sizeof)    
//		{
//			/*0x100*/             ULONG32      AutoAlignment : 1;             // 0 BitPosition                      
//			/*0x100*/             ULONG32      DisableBoost : 1;              // 1 BitPosition                      
//			/*0x100*/             ULONG32      EtwStackTraceApc1Inserted : 1; // 2 BitPosition                      
//			/*0x100*/             ULONG32      EtwStackTraceApc2Inserted : 1; // 3 BitPosition                      
//			/*0x100*/             ULONG32      CalloutActive : 1;             // 4 BitPosition                      
//			/*0x100*/             ULONG32      ApcQueueable : 1;              // 5 BitPosition                      
//			/*0x100*/             ULONG32      EnableStackSwap : 1;           // 6 BitPosition                      
//			/*0x100*/             ULONG32      GuiThread : 1;                 // 7 BitPosition                      
//			/*0x100*/             ULONG32      UmsPerformingSyscall : 1;      // 8 BitPosition                      
//			/*0x100*/             ULONG32      VdmSafe : 1;                   // 9 BitPosition                      
//			/*0x100*/             ULONG32      UmsDispatched : 1;             // 10 BitPosition                     
//			/*0x100*/             ULONG32      ReservedFlags : 21;            // 11 BitPosition                     
//		};
//		/*0x100*/         LONG32       ThreadFlags;
//	};
//	/*0x104*/     ULONG32      Spare0;
//	union                                               // 6 elements, 0xC0 bytes (sizeof)    
//	{
//		/*0x108*/         struct _KWAIT_BLOCK WaitBlock[4];
//		struct                                          // 2 elements, 0xC0 bytes (sizeof)    
//		{
//			/*0x108*/             UINT8        WaitBlockFill4[44];
//			/*0x134*/             ULONG32      ContextSwitches;
//			/*0x138*/             UINT8        _PADDING1_[0x90];
//		};
//		struct                                          // 5 elements, 0xC0 bytes (sizeof)    
//		{
//			/*0x108*/             UINT8        WaitBlockFill5[92];
//			/*0x164*/             UINT8        State;
//			/*0x165*/             CHAR         NpxState;
//			/*0x166*/             UINT8        WaitIrql;
//			/*0x167*/             CHAR         WaitMode;
//			/*0x168*/             UINT8        _PADDING2_[0x60];
//		};
//		struct                                          // 2 elements, 0xC0 bytes (sizeof)    
//		{
//			/*0x108*/             UINT8        WaitBlockFill6[140];
//			/*0x194*/             ULONG32      WaitTime;
//			/*0x198*/             UINT8        _PADDING3_[0x30];
//		};
//		struct                                          // 3 elements, 0xC0 bytes (sizeof)    
//		{
//			/*0x108*/             UINT8        WaitBlockFill7[168];
//			/*0x1B0*/             VOID*        TebMappedLowVa;
//			/*0x1B8*/             struct _UMS_CONTROL_BLOCK* Ucb;
//			/*0x1C0*/             UINT8        _PADDING4_[0x8];
//		};
//		struct                                          // 2 elements, 0xC0 bytes (sizeof)    
//		{
//			/*0x108*/             UINT8        WaitBlockFill8[188];
//			union                                       // 2 elements, 0x4 bytes (sizeof)     
//			{
//				struct                                  // 2 elements, 0x4 bytes (sizeof)     
//				{
//					/*0x1C4*/                     INT16        KernelApcDisable;
//					/*0x1C6*/                     INT16        SpecialApcDisable;
//				};
//				/*0x1C4*/                 ULONG32      CombinedApcDisable;
//			};
//		};
//	};
//	/*0x1C8*/     struct _LIST_ENTRY QueueListEntry;                  // 2 elements, 0x10 bytes (sizeof)    
//	/*0x1D8*/     struct _KTRAP_FRAME* TrapFrame;
//	/*0x1E0*/     VOID*        FirstArgument;
//	union                                               // 2 elements, 0x8 bytes (sizeof)     
//	{
//		/*0x1E8*/         VOID*        CallbackStack;
//		/*0x1E8*/         UINT64       CallbackDepth;
//	};
//	/*0x1F0*/     UINT8        ApcStateIndex;
//	/*0x1F1*/     CHAR         BasePriority;
//	union                                               // 2 elements, 0x1 bytes (sizeof)     
//	{
//		/*0x1F2*/         CHAR         PriorityDecrement;
//		struct                                          // 2 elements, 0x1 bytes (sizeof)     
//		{
//			/*0x1F2*/             UINT8        ForegroundBoost : 4;           // 0 BitPosition                      
//			/*0x1F2*/             UINT8        UnusualBoost : 4;              // 4 BitPosition                      
//		};
//	};
//	/*0x1F3*/     UINT8        Preempted;
//	/*0x1F4*/     UINT8        AdjustReason;
//	/*0x1F5*/     CHAR         AdjustIncrement;
//	/*0x1F6*/     CHAR         PreviousMode;
//	/*0x1F7*/     CHAR         Saturation;
//	/*0x1F8*/     ULONG32      SystemCallNumber;
//	/*0x1FC*/     ULONG32      FreezeCount;
//	/*0x200*/     struct _GROUP_AFFINITY UserAffinity;                // 3 elements, 0x10 bytes (sizeof)    
//	/*0x210*/     struct _KPROCESS* Process;
//	/*0x218*/     struct _GROUP_AFFINITY Affinity;                    // 3 elements, 0x10 bytes (sizeof)    
//	/*0x228*/     ULONG32      IdealProcessor;
//	/*0x22C*/     ULONG32      UserIdealProcessor;
//	/*0x230*/     struct _KAPC_STATE* ApcStatePointer[2];
//	union                                               // 2 elements, 0x30 bytes (sizeof)    
//	{
//		/*0x240*/         struct _KAPC_STATE SavedApcState;               // 5 elements, 0x30 bytes (sizeof)    
//		struct                                          // 5 elements, 0x30 bytes (sizeof)    
//		{
//			/*0x240*/             UINT8        SavedApcStateFill[43];
//			/*0x26B*/             UINT8        WaitReason;
//			/*0x26C*/             CHAR         SuspendCount;
//			/*0x26D*/             CHAR         Spare1;
//			/*0x26E*/             UINT8        CodePatchInProgress;
//			/*0x26F*/             UINT8        _PADDING5_[0x1];
//		};
//	};
//	/*0x270*/     VOID*        Win32Thread;
//	/*0x278*/     VOID*        StackBase;
//	union                                               // 7 elements, 0x58 bytes (sizeof)    
//	{
//		/*0x280*/         struct _KAPC SuspendApc;                        // 16 elements, 0x58 bytes (sizeof)   
//		struct                                          // 2 elements, 0x58 bytes (sizeof)    
//		{
//			/*0x280*/             UINT8        SuspendApcFill0[1];
//			/*0x281*/             UINT8        ResourceIndex;
//			/*0x282*/             UINT8        _PADDING6_[0x56];
//		};
//		struct                                          // 2 elements, 0x58 bytes (sizeof)    
//		{
//			/*0x280*/             UINT8        SuspendApcFill1[3];
//			/*0x283*/             UINT8        QuantumReset;
//			/*0x284*/             UINT8        _PADDING7_[0x54];
//		};
//		struct                                          // 2 elements, 0x58 bytes (sizeof)    
//		{
//			/*0x280*/             UINT8        SuspendApcFill2[4];
//			/*0x284*/             ULONG32      KernelTime;
//			/*0x288*/             UINT8        _PADDING8_[0x50];
//		};
//		struct                                          // 2 elements, 0x58 bytes (sizeof)    
//		{
//			/*0x280*/             UINT8        SuspendApcFill3[64];
//			/*0x2C0*/             struct _KPRCB* WaitPrcb;
//			/*0x2C8*/             UINT8        _PADDING9_[0x10];
//		};
//		struct                                          // 2 elements, 0x58 bytes (sizeof)    
//		{
//			/*0x280*/             UINT8        SuspendApcFill4[72];
//			/*0x2C8*/             VOID*        LegoData;
//			/*0x2D0*/             UINT8        _PADDING10_[0x8];
//		};
//		struct                                          // 3 elements, 0x58 bytes (sizeof)    
//		{
//			/*0x280*/             UINT8        SuspendApcFill5[83];
//			/*0x2D3*/             UINT8        LargeStack;
//			/*0x2D4*/             ULONG32      UserTime;
//		};
//	};
//	union                                               // 2 elements, 0x20 bytes (sizeof)    
//	{
//		/*0x2D8*/         struct _KSEMAPHORE SuspendSemaphore;            // 2 elements, 0x20 bytes (sizeof)    
//		struct                                          // 2 elements, 0x20 bytes (sizeof)    
//		{
//			/*0x2D8*/             UINT8        SuspendSemaphorefill[28];
//			/*0x2F4*/             ULONG32      SListFaultCount;
//		};
//	};
//	/*0x2F8*/     struct _LIST_ENTRY ThreadListEntry;                 // 2 elements, 0x10 bytes (sizeof)    
//	/*0x308*/     struct _LIST_ENTRY MutantListHead;                  // 2 elements, 0x10 bytes (sizeof)    
//	/*0x318*/     VOID*        SListFaultAddress;
//	/*0x320*/     INT64        ReadOperationCount;
//	/*0x328*/     INT64        WriteOperationCount;
//	/*0x330*/     INT64        OtherOperationCount;
//	/*0x338*/     INT64        ReadTransferCount;
//	/*0x340*/     INT64        WriteTransferCount;
//	/*0x348*/     INT64        OtherTransferCount;
//	/*0x350*/     struct _KTHREAD_COUNTERS* ThreadCounters;
//	/*0x358*/     struct _XSAVE_FORMAT* StateSaveArea;
//	/*0x360*/     struct _XSTATE_SAVE* XStateSave;
//}KTHREAD, *PKTHREAD;

typedef union _PS_CLIENT_SECURITY_CONTEXT    // 4 elements, 0x8 bytes (sizeof) 
{
	/*0x000*/     UINT64       ImpersonationData;
	/*0x000*/     VOID*        ImpersonationToken;
	struct                                   // 2 elements, 0x8 bytes (sizeof) 
	{
		/*0x000*/         UINT64       ImpersonationLevel : 2; // 0 BitPosition                  
		/*0x000*/         UINT64       EffectiveOnly : 1;      // 2 BitPosition                  
	};
}PS_CLIENT_SECURITY_CONTEXT, *PPS_CLIENT_SECURITY_CONTEXT;

typedef struct      _KTHREAD                                         // 89 elements, 0x4A8 bytes (sizeof)  
{




	struct// _KTHREAD                                 // 130 elements, 0x368 bytes (sizeof) 
	{
		/*0x000*/     struct _DISPATCHER_HEADER Header;                   // 29 elements, 0x18 bytes (sizeof)   
		/*0x018*/     UINT64       CycleTime;
		/*0x020*/     UINT64       QuantumTarget;
		/*0x028*/     VOID*        InitialStack;
		/*0x030*/     VOID*        StackLimit;
		/*0x038*/     VOID*        KernelStack;
		/*0x040*/     UINT64       ThreadLock;
		/*0x048*/     union _KWAIT_STATUS_REGISTER WaitRegister;          // 8 elements, 0x1 bytes (sizeof)     
		/*0x049*/     UINT8        Running;
		/*0x04A*/     UINT8        Alerted[2];
		union                                               // 2 elements, 0x4 bytes (sizeof)     
		{
			struct                                          // 15 elements, 0x4 bytes (sizeof)    
			{
				/*0x04C*/             ULONG32      KernelStackResident : 1;       // 0 BitPosition                      
				/*0x04C*/             ULONG32      ReadyTransition : 1;           // 1 BitPosition                      
				/*0x04C*/             ULONG32      ProcessReadyQueue : 1;         // 2 BitPosition                      
				/*0x04C*/             ULONG32      WaitNext : 1;                  // 3 BitPosition                      
				/*0x04C*/             ULONG32      SystemAffinityActive : 1;      // 4 BitPosition                      
				/*0x04C*/             ULONG32      Alertable : 1;                 // 5 BitPosition                      
				/*0x04C*/             ULONG32      GdiFlushActive : 1;            // 6 BitPosition                      
				/*0x04C*/             ULONG32      UserStackWalkActive : 1;       // 7 BitPosition                      
				/*0x04C*/             ULONG32      ApcInterruptRequest : 1;       // 8 BitPosition                      
				/*0x04C*/             ULONG32      ForceDeferSchedule : 1;        // 9 BitPosition                      
				/*0x04C*/             ULONG32      QuantumEndMigrate : 1;         // 10 BitPosition                     
				/*0x04C*/             ULONG32      UmsDirectedSwitchEnable : 1;   // 11 BitPosition                     
				/*0x04C*/             ULONG32      TimerActive : 1;               // 12 BitPosition                     
				/*0x04C*/             ULONG32      SystemThread : 1;              // 13 BitPosition                     
				/*0x04C*/             ULONG32      Reserved : 18;                 // 14 BitPosition                     
			};
			/*0x04C*/         LONG32       MiscFlags;
		};
		union                                               // 2 elements, 0x30 bytes (sizeof)    
		{
			/*0x050*/         struct _KAPC_STATE ApcState;                    // 5 elements, 0x30 bytes (sizeof)    
			struct                                          // 3 elements, 0x30 bytes (sizeof)    
			{
				/*0x050*/             UINT8        ApcStateFill[43];
				/*0x07B*/             CHAR         Priority;
				/*0x07C*/             ULONG32      NextProcessor;
			};
		};
		/*0x080*/     ULONG32      DeferredProcessor;
		/*0x084*/     UINT8        _PADDING0_[0x4];
		/*0x088*/     UINT64       ApcQueueLock;
		/*0x090*/     INT64        WaitStatus;
		/*0x098*/     struct _KWAIT_BLOCK* WaitBlockList;
		union                                               // 2 elements, 0x10 bytes (sizeof)    
		{
			/*0x0A0*/         struct _LIST_ENTRY WaitListEntry;               // 2 elements, 0x10 bytes (sizeof)    
			/*0x0A0*/         struct _SINGLE_LIST_ENTRY SwapListEntry;        // 1 elements, 0x8 bytes (sizeof)     
		};
		/*0x0B0*/     struct _KQUEUE* Queue;
		/*0x0B8*/     VOID*        Teb;
		/*0x0C0*/     struct _KTIMER Timer;                               // 6 elements, 0x40 bytes (sizeof)    
		union                                               // 2 elements, 0x4 bytes (sizeof)     
		{
			struct                                          // 12 elements, 0x4 bytes (sizeof)    
			{
				/*0x100*/             ULONG32      AutoAlignment : 1;             // 0 BitPosition                      
				/*0x100*/             ULONG32      DisableBoost : 1;              // 1 BitPosition                      
				/*0x100*/             ULONG32      EtwStackTraceApc1Inserted : 1; // 2 BitPosition                      
				/*0x100*/             ULONG32      EtwStackTraceApc2Inserted : 1; // 3 BitPosition                      
				/*0x100*/             ULONG32      CalloutActive : 1;             // 4 BitPosition                      
				/*0x100*/             ULONG32      ApcQueueable : 1;              // 5 BitPosition                      
				/*0x100*/             ULONG32      EnableStackSwap : 1;           // 6 BitPosition                      
				/*0x100*/             ULONG32      GuiThread : 1;                 // 7 BitPosition                      
				/*0x100*/             ULONG32      UmsPerformingSyscall : 1;      // 8 BitPosition                      
				/*0x100*/             ULONG32      VdmSafe : 1;                   // 9 BitPosition                      
				/*0x100*/             ULONG32      UmsDispatched : 1;             // 10 BitPosition                     
				/*0x100*/             ULONG32      ReservedFlags : 21;            // 11 BitPosition                     
			};
			/*0x100*/         LONG32       ThreadFlags;
		};
		/*0x104*/     ULONG32      Spare0;
		union                                               // 6 elements, 0xC0 bytes (sizeof)    
		{
			/*0x108*/         struct _KWAIT_BLOCK WaitBlock[4];
			struct                                          // 2 elements, 0xC0 bytes (sizeof)    
			{
				/*0x108*/             UINT8        WaitBlockFill4[44];
				/*0x134*/             ULONG32      ContextSwitches;
				/*0x138*/             UINT8        _PADDING1_[0x90];
			};
			struct                                          // 5 elements, 0xC0 bytes (sizeof)    
			{
				/*0x108*/             UINT8        WaitBlockFill5[92];
				/*0x164*/             UINT8        State;
				/*0x165*/             CHAR         NpxState;
				/*0x166*/             UINT8        WaitIrql;
				/*0x167*/             CHAR         WaitMode;
				/*0x168*/             UINT8        _PADDING2_[0x60];
			};
			struct                                          // 2 elements, 0xC0 bytes (sizeof)    
			{
				/*0x108*/             UINT8        WaitBlockFill6[140];
				/*0x194*/             ULONG32      WaitTime;
				/*0x198*/             UINT8        _PADDING3_[0x30];
			};
			struct                                          // 3 elements, 0xC0 bytes (sizeof)    
			{
				/*0x108*/             UINT8        WaitBlockFill7[168];
				/*0x1B0*/             VOID*        TebMappedLowVa;
				/*0x1B8*/             struct _UMS_CONTROL_BLOCK* Ucb;
				/*0x1C0*/             UINT8        _PADDING4_[0x8];
			};
			struct                                          // 2 elements, 0xC0 bytes (sizeof)    
			{
				/*0x108*/             UINT8        WaitBlockFill8[188];
				union                                       // 2 elements, 0x4 bytes (sizeof)     
				{
					struct                                  // 2 elements, 0x4 bytes (sizeof)     
					{
						/*0x1C4*/                     INT16        KernelApcDisable;
						/*0x1C6*/                     INT16        SpecialApcDisable;
					};
					/*0x1C4*/                 ULONG32      CombinedApcDisable;
				};
			};
		};
		/*0x1C8*/     struct _LIST_ENTRY QueueListEntry;                  // 2 elements, 0x10 bytes (sizeof)    
		/*0x1D8*/     struct _KTRAP_FRAME* TrapFrame;
		/*0x1E0*/     VOID*        FirstArgument;
		union                                               // 2 elements, 0x8 bytes (sizeof)     
		{
			/*0x1E8*/         VOID*        CallbackStack;
			/*0x1E8*/         UINT64       CallbackDepth;
		};
		/*0x1F0*/     UINT8        ApcStateIndex;
		/*0x1F1*/     CHAR         BasePriority;
		union                                               // 2 elements, 0x1 bytes (sizeof)     
		{
			/*0x1F2*/         CHAR         PriorityDecrement;
			struct                                          // 2 elements, 0x1 bytes (sizeof)     
			{
				/*0x1F2*/             UINT8        ForegroundBoost : 4;           // 0 BitPosition                      
				/*0x1F2*/             UINT8        UnusualBoost : 4;              // 4 BitPosition                      
			};
		};
		/*0x1F3*/     UINT8        Preempted;
		/*0x1F4*/     UINT8        AdjustReason;
		/*0x1F5*/     CHAR         AdjustIncrement;
		/*0x1F6*/     CHAR         PreviousMode;
		/*0x1F7*/     CHAR         Saturation;
		/*0x1F8*/     ULONG32      SystemCallNumber;
		/*0x1FC*/     ULONG32      FreezeCount;
		/*0x200*/     struct _GROUP_AFFINITY UserAffinity;                // 3 elements, 0x10 bytes (sizeof)    
		/*0x210*/     struct _KPROCESS* Process;
		/*0x218*/     struct _GROUP_AFFINITY Affinity;                    // 3 elements, 0x10 bytes (sizeof)    
		/*0x228*/     ULONG32      IdealProcessor;
		/*0x22C*/     ULONG32      UserIdealProcessor;
		/*0x230*/     struct _KAPC_STATE* ApcStatePointer[2];
		union                                               // 2 elements, 0x30 bytes (sizeof)    
		{
			/*0x240*/         struct _KAPC_STATE SavedApcState;               // 5 elements, 0x30 bytes (sizeof)    
			struct                                          // 5 elements, 0x30 bytes (sizeof)    
			{
				/*0x240*/             UINT8        SavedApcStateFill[43];
				/*0x26B*/             UINT8        WaitReason;
				/*0x26C*/             CHAR         SuspendCount;
				/*0x26D*/             CHAR         Spare1;
				/*0x26E*/             UINT8        CodePatchInProgress;
				/*0x26F*/             UINT8        _PADDING5_[0x1];
			};
		};
		/*0x270*/     VOID*        Win32Thread;
		/*0x278*/     VOID*        StackBase;
		union                                               // 7 elements, 0x58 bytes (sizeof)    
		{
			/*0x280*/         struct _KAPC SuspendApc;                        // 16 elements, 0x58 bytes (sizeof)   
			struct                                          // 2 elements, 0x58 bytes (sizeof)    
			{
				/*0x280*/             UINT8        SuspendApcFill0[1];
				/*0x281*/             UINT8        ResourceIndex;
				/*0x282*/             UINT8        _PADDING6_[0x56];
			};
			struct                                          // 2 elements, 0x58 bytes (sizeof)    
			{
				/*0x280*/             UINT8        SuspendApcFill1[3];
				/*0x283*/             UINT8        QuantumReset;
				/*0x284*/             UINT8        _PADDING7_[0x54];
			};
			struct                                          // 2 elements, 0x58 bytes (sizeof)    
			{
				/*0x280*/             UINT8        SuspendApcFill2[4];
				/*0x284*/             ULONG32      KernelTime;
				/*0x288*/             UINT8        _PADDING8_[0x50];
			};
			struct                                          // 2 elements, 0x58 bytes (sizeof)    
			{
				/*0x280*/             UINT8        SuspendApcFill3[64];
				/*0x2C0*/             struct _KPRCB* WaitPrcb;
				/*0x2C8*/             UINT8        _PADDING9_[0x10];
			};
			struct                                          // 2 elements, 0x58 bytes (sizeof)    
			{
				/*0x280*/             UINT8        SuspendApcFill4[72];
				/*0x2C8*/             VOID*        LegoData;
				/*0x2D0*/             UINT8        _PADDING10_[0x8];
			};
			struct                                          // 3 elements, 0x58 bytes (sizeof)    
			{
				/*0x280*/             UINT8        SuspendApcFill5[83];
				/*0x2D3*/             UINT8        LargeStack;
				/*0x2D4*/             ULONG32      UserTime;
			};
		};
		union                                               // 2 elements, 0x20 bytes (sizeof)    
		{
			/*0x2D8*/         struct _KSEMAPHORE SuspendSemaphore;            // 2 elements, 0x20 bytes (sizeof)    
			struct                                          // 2 elements, 0x20 bytes (sizeof)    
			{
				/*0x2D8*/             UINT8        SuspendSemaphorefill[28];
				/*0x2F4*/             ULONG32      SListFaultCount;
			};
		};
		/*0x2F8*/     struct _LIST_ENTRY ThreadListEntry;                 // 2 elements, 0x10 bytes (sizeof)    
		/*0x308*/     struct _LIST_ENTRY MutantListHead;                  // 2 elements, 0x10 bytes (sizeof)    
		/*0x318*/     VOID*        SListFaultAddress;
		/*0x320*/     INT64        ReadOperationCount;
		/*0x328*/     INT64        WriteOperationCount;
		/*0x330*/     INT64        OtherOperationCount;
		/*0x338*/     INT64        ReadTransferCount;
		/*0x340*/     INT64        WriteTransferCount;
		/*0x348*/     INT64        OtherTransferCount;
		/*0x350*/     struct _KTHREAD_COUNTERS* ThreadCounters;
		/*0x358*/     struct _XSAVE_FORMAT* StateSaveArea;
		/*0x360*/     struct _XSTATE_SAVE* XStateSave;
	}Tcb;




	///*0x000*/     struct _KTHREAD Tcb;                                             // 130 elements, 0x368 bytes (sizeof) 

















	/*0x368*/     union _LARGE_INTEGER CreateTime;                                 // 4 elements, 0x8 bytes (sizeof)     
	union                                                            // 2 elements, 0x10 bytes (sizeof)    
	{
		/*0x370*/         union _LARGE_INTEGER ExitTime;                               // 4 elements, 0x8 bytes (sizeof)     
		/*0x370*/         struct _LIST_ENTRY KeyedWaitChain;                           // 2 elements, 0x10 bytes (sizeof)    
	};
	/*0x380*/     LONG32       ExitStatus;
	/*0x384*/     UINT8        _PADDING0_[0x4];
	union                                                            // 2 elements, 0x10 bytes (sizeof)    
	{
		/*0x388*/         struct _LIST_ENTRY PostBlockList;                            // 2 elements, 0x10 bytes (sizeof)    
		struct                                                       // 2 elements, 0x10 bytes (sizeof)    
		{
			/*0x388*/             VOID*        ForwardLinkShadow;
			/*0x390*/             VOID*        StartAddress;
		};
	};
	union                                                            // 3 elements, 0x8 bytes (sizeof)     
	{
		/*0x398*/         struct _TERMINATION_PORT* TerminationPort;
		/*0x398*/         struct _ETHREAD* ReaperLink;
		/*0x398*/         VOID*        KeyedWaitValue;
	};
	/*0x3A0*/     UINT64       ActiveTimerListLock;
	/*0x3A8*/     struct _LIST_ENTRY ActiveTimerListHead;                          // 2 elements, 0x10 bytes (sizeof)    
	/*0x3B8*/     struct _CLIENT_ID Cid;                                           // 2 elements, 0x10 bytes (sizeof)    
	union                                                            // 2 elements, 0x20 bytes (sizeof)    
	{
		/*0x3C8*/         struct _KSEMAPHORE KeyedWaitSemaphore;                       // 2 elements, 0x20 bytes (sizeof)    
		/*0x3C8*/         struct _KSEMAPHORE AlpcWaitSemaphore;                        // 2 elements, 0x20 bytes (sizeof)    
	};
	/*0x3E8*/     union _PS_CLIENT_SECURITY_CONTEXT ClientSecurity;                // 4 elements, 0x8 bytes (sizeof)     
	/*0x3F0*/     struct _LIST_ENTRY IrpList;                                      // 2 elements, 0x10 bytes (sizeof)    
	/*0x400*/     UINT64       TopLevelIrp;
	/*0x408*/     struct _DEVICE_OBJECT* DeviceToVerify;
	/*0x410*/     union _PSP_CPU_QUOTA_APC* CpuQuotaApc;
	/*0x418*/     VOID*        Win32StartAddress;
	/*0x420*/     VOID*        LegacyPowerObject;
	/*0x428*/     struct _LIST_ENTRY ThreadListEntry;                              // 2 elements, 0x10 bytes (sizeof)    
	/*0x438*/     struct _EX_RUNDOWN_REF RundownProtect;                           // 2 elements, 0x8 bytes (sizeof)     
	/*0x440*/     struct _EX_PUSH_LOCK ThreadLock;                                 // 7 elements, 0x8 bytes (sizeof)     
	/*0x448*/     ULONG32      ReadClusterSize;
	/*0x44C*/     LONG32       MmLockOrdering;
	union                                                            // 2 elements, 0x4 bytes (sizeof)     
	{
		/*0x450*/         ULONG32      CrossThreadFlags;
		struct                                                       // 14 elements, 0x4 bytes (sizeof)    
		{
			/*0x450*/             ULONG32      Terminated : 1;                             // 0 BitPosition                      
			/*0x450*/             ULONG32      ThreadInserted : 1;                         // 1 BitPosition                      
			/*0x450*/             ULONG32      HideFromDebugger : 1;                       // 2 BitPosition                      
			/*0x450*/             ULONG32      ActiveImpersonationInfo : 1;                // 3 BitPosition                      
			/*0x450*/             ULONG32      Reserved : 1;                               // 4 BitPosition                      
			/*0x450*/             ULONG32      HardErrorsAreDisabled : 1;                  // 5 BitPosition                      
			/*0x450*/             ULONG32      BreakOnTermination : 1;                     // 6 BitPosition                      
			/*0x450*/             ULONG32      SkipCreationMsg : 1;                        // 7 BitPosition                      
			/*0x450*/             ULONG32      SkipTerminationMsg : 1;                     // 8 BitPosition                      
			/*0x450*/             ULONG32      CopyTokenOnOpen : 1;                        // 9 BitPosition                      
			/*0x450*/             ULONG32      ThreadIoPriority : 3;                       // 10 BitPosition                     
			/*0x450*/             ULONG32      ThreadPagePriority : 3;                     // 13 BitPosition                     
			/*0x450*/             ULONG32      RundownFail : 1;                            // 16 BitPosition                     
			/*0x450*/             ULONG32      NeedsWorkingSetAging : 1;                   // 17 BitPosition                     
		};
	};
	union                                                            // 2 elements, 0x4 bytes (sizeof)     
	{
		/*0x454*/         ULONG32      SameThreadPassiveFlags;
		struct                                                       // 7 elements, 0x4 bytes (sizeof)     
		{
			/*0x454*/             ULONG32      ActiveExWorker : 1;                         // 0 BitPosition                      
			/*0x454*/             ULONG32      ExWorkerCanWaitUser : 1;                    // 1 BitPosition                      
			/*0x454*/             ULONG32      MemoryMaker : 1;                            // 2 BitPosition                      
			/*0x454*/             ULONG32      ClonedThread : 1;                           // 3 BitPosition                      
			/*0x454*/             ULONG32      KeyedEventInUse : 1;                        // 4 BitPosition                      
			/*0x454*/             ULONG32      RateApcState : 2;                           // 5 BitPosition                      
			/*0x454*/             ULONG32      SelfTerminate : 1;                          // 7 BitPosition                      
		};
	};
	union                                                            // 2 elements, 0x4 bytes (sizeof)     
	{
		/*0x458*/         ULONG32      SameThreadApcFlags;
		struct                                                       // 4 elements, 0x4 bytes (sizeof)     
		{
			struct                                                   // 8 elements, 0x1 bytes (sizeof)     
			{
				/*0x458*/                 UINT8        Spare : 1;                              // 0 BitPosition                      
				/*0x458*/                 UINT8        StartAddressInvalid : 1;                // 1 BitPosition                      
				/*0x458*/                 UINT8        EtwPageFaultCalloutActive : 1;          // 2 BitPosition                      
				/*0x458*/                 UINT8        OwnsProcessWorkingSetExclusive : 1;     // 3 BitPosition                      
				/*0x458*/                 UINT8        OwnsProcessWorkingSetShared : 1;        // 4 BitPosition                      
				/*0x458*/                 UINT8        OwnsSystemCacheWorkingSetExclusive : 1; // 5 BitPosition                      
				/*0x458*/                 UINT8        OwnsSystemCacheWorkingSetShared : 1;    // 6 BitPosition                      
				/*0x458*/                 UINT8        OwnsSessionWorkingSetExclusive : 1;     // 7 BitPosition                      
			};
			struct                                                   // 8 elements, 0x1 bytes (sizeof)     
			{
				/*0x459*/                 UINT8        OwnsSessionWorkingSetShared : 1;        // 0 BitPosition                      
				/*0x459*/                 UINT8        OwnsProcessAddressSpaceExclusive : 1;   // 1 BitPosition                      
				/*0x459*/                 UINT8        OwnsProcessAddressSpaceShared : 1;      // 2 BitPosition                      
				/*0x459*/                 UINT8        SuppressSymbolLoad : 1;                 // 3 BitPosition                      
				/*0x459*/                 UINT8        Prefetching : 1;                        // 4 BitPosition                      
				/*0x459*/                 UINT8        OwnsDynamicMemoryShared : 1;            // 5 BitPosition                      
				/*0x459*/                 UINT8        OwnsChangeControlAreaExclusive : 1;     // 6 BitPosition                      
				/*0x459*/                 UINT8        OwnsChangeControlAreaShared : 1;        // 7 BitPosition                      
			};
			struct                                                   // 6 elements, 0x1 bytes (sizeof)     
			{
				/*0x45A*/                 UINT8        OwnsPagedPoolWorkingSetExclusive : 1;   // 0 BitPosition                      
				/*0x45A*/                 UINT8        OwnsPagedPoolWorkingSetShared : 1;      // 1 BitPosition                      
				/*0x45A*/                 UINT8        OwnsSystemPtesWorkingSetExclusive : 1;  // 2 BitPosition                      
				/*0x45A*/                 UINT8        OwnsSystemPtesWorkingSetShared : 1;     // 3 BitPosition                      
				/*0x45A*/                 UINT8        TrimTrigger : 2;                        // 4 BitPosition                      
				/*0x45A*/                 UINT8        Spare1 : 2;                             // 6 BitPosition                      
			};
			/*0x45B*/             UINT8        PriorityRegionActive;
		};
	};
	/*0x45C*/     UINT8        CacheManagerActive;
	/*0x45D*/     UINT8        DisablePageFaultClustering;
	/*0x45E*/     UINT8        ActiveFaultCount;
	/*0x45F*/     UINT8        LockOrderState;
	/*0x460*/     UINT64       AlpcMessageId;
	union                                                            // 2 elements, 0x8 bytes (sizeof)     
	{
		/*0x468*/         VOID*        AlpcMessage;
		/*0x468*/         ULONG32      AlpcReceiveAttributeSet;
	};
	/*0x470*/     struct _LIST_ENTRY AlpcWaitListEntry;                            // 2 elements, 0x10 bytes (sizeof)    
	/*0x480*/     ULONG32      CacheManagerCount;
	/*0x484*/     ULONG32      IoBoostCount;
	/*0x488*/     UINT64       IrpListLock;
	/*0x490*/     VOID*        ReservedForSynchTracking;
	/*0x498*/     struct _SINGLE_LIST_ENTRY CmCallbackListHead;                    // 1 elements, 0x8 bytes (sizeof)     
	/*0x4A0*/     ULONG32      KernelStackReference;
	/*0x4A4*/     UINT8        _PADDING1_[0x4];
}ETHREAD, *PETHREAD;
















#endif // !NTKE_H

