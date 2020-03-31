#pragma once

#define MM_ZERO_ACCESS         0  // this value is not used.
#define MM_READONLY            1
#define MM_EXECUTE             2
#define MM_EXECUTE_READ        3
#define MM_READWRITE           4  // bit 2 is set if this is writable.
#define MM_WRITECOPY           5
#define MM_EXECUTE_READWRITE   6
#define MM_EXECUTE_WRITECOPY   7





typedef struct _MMVAD_FLAGS         // 7 elements, 0x8 bytes (sizeof) 
{
	/*0x000*/     UINT64       CommitCharge : 51; // 0 BitPosition                  
	/*0x000*/     UINT64       NoChange : 1;      // 51 BitPosition                 
	/*0x000*/     UINT64       VadType : 3;       // 52 BitPosition                 
	/*0x000*/     UINT64       MemCommit : 1;     // 55 BitPosition                 
	/*0x000*/     UINT64       Protection : 5;    // 56 BitPosition                 
	/*0x000*/     UINT64       Spare : 2;         // 61 BitPosition                 
	/*0x000*/     UINT64       PrivateMemory : 1; // 63 BitPosition                 
}MMVAD_FLAGS, *PMMVAD_FLAGS;


typedef struct _MMPTE_SUBSECTION         // 6 elements, 0x8 bytes (sizeof) 
{
	/*0x000*/     UINT64       Valid : 1;              // 0 BitPosition                  
	/*0x000*/     UINT64       Unused0 : 4;            // 1 BitPosition                  
	/*0x000*/     UINT64       Protection : 5;         // 5 BitPosition                  
	/*0x000*/     UINT64       Prototype : 1;          // 10 BitPosition                 
	/*0x000*/     UINT64       Unused1 : 5;            // 11 BitPosition                 
	/*0x000*/     INT64        SubsectionAddress : 48; // 16 BitPosition                 
}MMPTE_SUBSECTION, *PMMPTE_SUBSECTION;

typedef struct _MMPTE_LIST       // 8 elements, 0x8 bytes (sizeof) 
{
	/*0x000*/     UINT64       Valid : 1;      // 0 BitPosition                  
	/*0x000*/     UINT64       OneEntry : 1;   // 1 BitPosition                  
	/*0x000*/     UINT64       filler0 : 3;    // 2 BitPosition                  
	/*0x000*/     UINT64       Protection : 5; // 5 BitPosition                  
	/*0x000*/     UINT64       Prototype : 1;  // 10 BitPosition                 
	/*0x000*/     UINT64       Transition : 1; // 11 BitPosition                 
	/*0x000*/     UINT64       filler1 : 20;   // 12 BitPosition                 
	/*0x000*/     UINT64       NextEntry : 32; // 32 BitPosition                 
}MMPTE_LIST, *PMMPTE_LIST;




typedef struct _MMPTE_TIMESTAMP        // 7 elements, 0x8 bytes (sizeof) 
{
	/*0x000*/     UINT64       MustBeZero : 1;       // 0 BitPosition                  
	/*0x000*/     UINT64       PageFileLow : 4;      // 1 BitPosition                  
	/*0x000*/     UINT64       Protection : 5;       // 5 BitPosition                  
	/*0x000*/     UINT64       Prototype : 1;        // 10 BitPosition                 
	/*0x000*/     UINT64       Transition : 1;       // 11 BitPosition                 
	/*0x000*/     UINT64       Reserved : 20;        // 12 BitPosition                 
	/*0x000*/     UINT64       GlobalTimeStamp : 32; // 32 BitPosition                 
}MMPTE_TIMESTAMP, *PMMPTE_TIMESTAMP;

typedef struct _MMPTE_TRANSITION       // 10 elements, 0x8 bytes (sizeof) 
{
	/*0x000*/     UINT64       Valid : 1;            // 0 BitPosition                   
	/*0x000*/     UINT64       Write : 1;            // 1 BitPosition                   
	/*0x000*/     UINT64       Owner : 1;            // 2 BitPosition                   
	/*0x000*/     UINT64       WriteThrough : 1;     // 3 BitPosition                   
	/*0x000*/     UINT64       CacheDisable : 1;     // 4 BitPosition                   
	/*0x000*/     UINT64       Protection : 5;       // 5 BitPosition                   
	/*0x000*/     UINT64       Prototype : 1;        // 10 BitPosition                  
	/*0x000*/     UINT64       Transition : 1;       // 11 BitPosition                  
	/*0x000*/     UINT64       PageFrameNumber : 36; // 12 BitPosition                  
	/*0x000*/     UINT64       Unused : 16;          // 48 BitPosition                  
}MMPTE_TRANSITION, *PMMPTE_TRANSITION;





typedef struct _MMPTE_SOFTWARE              // 9 elements, 0x8 bytes (sizeof) 
{
	/*0x000*/     UINT64       Valid : 1;                 // 0 BitPosition                  
	/*0x000*/     UINT64       PageFileLow : 4;           // 1 BitPosition                  
	/*0x000*/     UINT64       Protection : 5;            // 5 BitPosition                  
	/*0x000*/     UINT64       Prototype : 1;             // 10 BitPosition                 
	/*0x000*/     UINT64       Transition : 1;            // 11 BitPosition                 
	/*0x000*/     UINT64       UsedPageTableEntries : 10; // 12 BitPosition                 
	/*0x000*/     UINT64       InStore : 1;               // 22 BitPosition                 
	/*0x000*/     UINT64       Reserved : 9;              // 23 BitPosition                 
	/*0x000*/     UINT64       PageFileHigh : 32;         // 32 BitPosition                 
}MMPTE_SOFTWARE, *PMMPTE_SOFTWARE;

typedef struct _MMPTE_PROTOTYPE     // 7 elements, 0x8 bytes (sizeof) 
{
	/*0x000*/     UINT64       Valid : 1;         // 0 BitPosition                  
	/*0x000*/     UINT64       Unused0 : 7;       // 1 BitPosition                  
	/*0x000*/     UINT64       ReadOnly : 1;      // 8 BitPosition                  
	/*0x000*/     UINT64       Unused1 : 1;       // 9 BitPosition                  
	/*0x000*/     UINT64       Prototype : 1;     // 10 BitPosition                 
	/*0x000*/     UINT64       Protection : 5;    // 11 BitPosition                 
	/*0x000*/     INT64        ProtoAddress : 48; // 16 BitPosition                 
}MMPTE_PROTOTYPE, *PMMPTE_PROTOTYPE;

typedef struct _HARDWARE_PTE           // 16 elements, 0x8 bytes (sizeof) 
{
	/*0x000*/     UINT64       Valid : 1;            // 0 BitPosition                   
	/*0x000*/     UINT64       Write : 1;            // 1 BitPosition                   
	/*0x000*/     UINT64       Owner : 1;            // 2 BitPosition                   
	/*0x000*/     UINT64       WriteThrough : 1;     // 3 BitPosition                   
	/*0x000*/     UINT64       CacheDisable : 1;     // 4 BitPosition                   
	/*0x000*/     UINT64       Accessed : 1;         // 5 BitPosition                   
	/*0x000*/     UINT64       Dirty : 1;            // 6 BitPosition                   
	/*0x000*/     UINT64       LargePage : 1;        // 7 BitPosition                   
	/*0x000*/     UINT64       Global : 1;           // 8 BitPosition                   
	/*0x000*/     UINT64       CopyOnWrite : 1;      // 9 BitPosition                   
	/*0x000*/     UINT64       Prototype : 1;        // 10 BitPosition                  
	/*0x000*/     UINT64       reserved0 : 1;        // 11 BitPosition                  
	/*0x000*/     UINT64       PageFrameNumber : 36; // 12 BitPosition                  
	/*0x000*/     UINT64       reserved1 : 4;        // 48 BitPosition                  
	/*0x000*/     UINT64       SoftwareWsIndex : 11; // 52 BitPosition                  
	/*0x000*/     UINT64       NoExecute : 1;        // 63 BitPosition                  
}HARDWARE_PTE, *PHARDWARE_PTE;

typedef struct _MMPTE_HARDWARE         // 16 elements, 0x8 bytes (sizeof) 
{
	/*0x000*/     UINT64       Valid : 1;            // 0 BitPosition                   
	/*0x000*/     UINT64       Dirty1 : 1;           // 1 BitPosition                   
	/*0x000*/     UINT64       Owner : 1;            // 2 BitPosition                   
	/*0x000*/     UINT64       WriteThrough : 1;     // 3 BitPosition                   
	/*0x000*/     UINT64       CacheDisable : 1;     // 4 BitPosition                   
	/*0x000*/     UINT64       Accessed : 1;         // 5 BitPosition                   
	/*0x000*/     UINT64       Dirty : 1;            // 6 BitPosition                   
	/*0x000*/     UINT64       LargePage : 1;        // 7 BitPosition                   
	/*0x000*/     UINT64       Global : 1;           // 8 BitPosition                   
	/*0x000*/     UINT64       CopyOnWrite : 1;      // 9 BitPosition                   
	/*0x000*/     UINT64       Unused : 1;           // 10 BitPosition                  
	/*0x000*/     UINT64       Write : 1;            // 11 BitPosition                  
	/*0x000*/     UINT64       PageFrameNumber : 36; // 12 BitPosition                  
	/*0x000*/     UINT64       reserved1 : 4;        // 48 BitPosition                  
	/*0x000*/     UINT64       SoftwareWsIndex : 11; // 52 BitPosition                  
	/*0x000*/     UINT64       NoExecute : 1;        // 63 BitPosition                  
}MMPTE_HARDWARE, *PMMPTE_HARDWARE;


typedef struct _MMPTE                      // 1 elements, 0x8 bytes (sizeof)  
{
	union                                  // 10 elements, 0x8 bytes (sizeof) 
	{
		/*0x000*/         UINT64       Long;
		/*0x000*/         UINT64       VolatileLong;
		/*0x000*/         struct _MMPTE_HARDWARE Hard;       // 16 elements, 0x8 bytes (sizeof) 
		/*0x000*/         struct _HARDWARE_PTE Flush;        // 16 elements, 0x8 bytes (sizeof) 
		/*0x000*/         struct _MMPTE_PROTOTYPE Proto;     // 7 elements, 0x8 bytes (sizeof)  
		/*0x000*/         struct _MMPTE_SOFTWARE Soft;       // 9 elements, 0x8 bytes (sizeof)  
		/*0x000*/         struct _MMPTE_TIMESTAMP TimeStamp; // 7 elements, 0x8 bytes (sizeof)  
		/*0x000*/         struct _MMPTE_TRANSITION Trans;    // 10 elements, 0x8 bytes (sizeof) 
		/*0x000*/         struct _MMPTE_SUBSECTION Subsect;  // 6 elements, 0x8 bytes (sizeof)  
		/*0x000*/         struct _MMPTE_LIST List;           // 8 elements, 0x8 bytes (sizeof)  
	}u;
}MMPTE, *PMMPTE;




typedef struct _SEGMENT_FLAGS                   // 12 elements, 0x4 bytes (sizeof) 
{
	/*0x000*/     ULONG32      TotalNumberOfPtes4132 : 10;    // 0 BitPosition                   
	/*0x000*/     ULONG32      ExtraSharedWowSubsections : 1; // 10 BitPosition                  
	/*0x000*/     ULONG32      LargePages : 1;                // 11 BitPosition                  
	/*0x000*/     ULONG32      WatchProto : 1;                // 12 BitPosition                  
	/*0x000*/     ULONG32      DebugSymbolsLoaded : 1;        // 13 BitPosition                  
	/*0x000*/     ULONG32      WriteCombined : 1;             // 14 BitPosition                  
	/*0x000*/     ULONG32      NoCache : 1;                   // 15 BitPosition                  
	/*0x000*/     ULONG32      FloppyMedia : 1;               // 16 BitPosition                  
	/*0x000*/     ULONG32      DefaultProtectionMask : 5;     // 17 BitPosition                  
	/*0x000*/     ULONG32      Binary32 : 1;                  // 22 BitPosition                  
	/*0x000*/     ULONG32      ContainsDebug : 1;             // 23 BitPosition                  
	/*0x000*/     ULONG32      Spare : 8;                     // 24 BitPosition                  
}SEGMENT_FLAGS, *PSEGMENT_FLAGS;
typedef struct _SEGMENT                                         // 12 elements, 0x50 bytes (sizeof) 
{
	/*0x000*/     struct _CONTROL_AREA* ControlArea;
	/*0x008*/     ULONG32      TotalNumberOfPtes;
	/*0x00C*/     struct _SEGMENT_FLAGS SegmentFlags;                         // 12 elements, 0x4 bytes (sizeof)  
	/*0x010*/     UINT64       NumberOfCommittedPages;
	/*0x018*/     UINT64       SizeOfSegment;
	union                                                       // 2 elements, 0x8 bytes (sizeof)   
	{
		/*0x020*/         struct _MMEXTEND_INFO* ExtendInfo;
		/*0x020*/         VOID*        BasedAddress;
	};
	/*0x028*/     struct _EX_PUSH_LOCK SegmentLock;                           // 7 elements, 0x8 bytes (sizeof)   
	union                                                       // 2 elements, 0x8 bytes (sizeof)   
	{
		/*0x030*/         UINT64       ImageCommitment;
		/*0x030*/         struct _EPROCESS* CreatingProcess;
	}u1;
	union                                                       // 2 elements, 0x8 bytes (sizeof)   
	{
		/*0x038*/         struct _MI_SECTION_IMAGE_INFORMATION* ImageInformation;
		/*0x038*/         VOID*        FirstMappedVa;
	}u2;
	/*0x040*/     struct _MMPTE* PrototypePte;
	/*0x048*/     struct _MMPTE ThePtes[1];
}SEGMENT, *PSEGMENT;


typedef struct _MMVAD_FLAGS3              // 8 elements, 0x8 bytes (sizeof) 
{
	/*0x000*/     UINT64       PreferredNode : 6;       // 0 BitPosition                  
	/*0x000*/     UINT64       Teb : 1;                 // 6 BitPosition                  
	/*0x000*/     UINT64       Spare : 1;               // 7 BitPosition                  
	/*0x000*/     UINT64       SequentialAccess : 1;    // 8 BitPosition                  
	/*0x000*/     UINT64       LastSequentialTrim : 15; // 9 BitPosition                  
	/*0x000*/     UINT64       Spare2 : 8;              // 24 BitPosition                 
	/*0x000*/     UINT64       LargePageCreating : 1;   // 32 BitPosition                 
	/*0x000*/     UINT64       Spare3 : 31;             // 33 BitPosition                 
}MMVAD_FLAGS3, *PMMVAD_FLAGS3;
typedef struct _MMSECTION_FLAGS               // 27 elements, 0x4 bytes (sizeof) 
{
	/*0x000*/     UINT32       BeingDeleted : 1;            // 0 BitPosition                   
	/*0x000*/     UINT32       BeingCreated : 1;            // 1 BitPosition                   
	/*0x000*/     UINT32       BeingPurged : 1;             // 2 BitPosition                   
	/*0x000*/     UINT32       NoModifiedWriting : 1;       // 3 BitPosition                   
	/*0x000*/     UINT32       FailAllIo : 1;               // 4 BitPosition                   
	/*0x000*/     UINT32       Image : 1;                   // 5 BitPosition                   
	/*0x000*/     UINT32       Based : 1;                   // 6 BitPosition                   
	/*0x000*/     UINT32       File : 1;                    // 7 BitPosition                   
	/*0x000*/     UINT32       Networked : 1;               // 8 BitPosition                   
	/*0x000*/     UINT32       Rom : 1;                     // 9 BitPosition                   
	/*0x000*/     UINT32       PhysicalMemory : 1;          // 10 BitPosition                  
	/*0x000*/     UINT32       CopyOnWrite : 1;             // 11 BitPosition                  
	/*0x000*/     UINT32       Reserve : 1;                 // 12 BitPosition                  
	/*0x000*/     UINT32       Commit : 1;                  // 13 BitPosition                  
	/*0x000*/     UINT32       Accessed : 1;                // 14 BitPosition                  
	/*0x000*/     UINT32       WasPurged : 1;               // 15 BitPosition                  
	/*0x000*/     UINT32       UserReference : 1;           // 16 BitPosition                  
	/*0x000*/     UINT32       GlobalMemory : 1;            // 17 BitPosition                  
	/*0x000*/     UINT32       DeleteOnClose : 1;           // 18 BitPosition                  
	/*0x000*/     UINT32       FilePointerNull : 1;         // 19 BitPosition                  
	/*0x000*/     UINT32       GlobalOnlyPerSession : 1;    // 20 BitPosition                  
	/*0x000*/     UINT32       SetMappedFileIoComplete : 1; // 21 BitPosition                  
	/*0x000*/     UINT32       CollidedFlush : 1;           // 22 BitPosition                  
	/*0x000*/     UINT32       NoChange : 1;                // 23 BitPosition                  
	/*0x000*/     UINT32       Spare : 1;                   // 24 BitPosition                  
	/*0x000*/     UINT32       UserWritable : 1;            // 25 BitPosition                  
	/*0x000*/     UINT32       PreferredNode : 6;           // 26 BitPosition                  
}MMSECTION_FLAGS, *PMMSECTION_FLAGS;
typedef struct _EX_FAST_REF      // 3 elements, 0x8 bytes (sizeof) 
{
	union                        // 3 elements, 0x8 bytes (sizeof) 
	{
		/*0x000*/         VOID*        Object;
		/*0x000*/         UINT64       RefCnt : 4; // 0 BitPosition                  
		/*0x000*/         UINT64       Value;
	};
}EX_FAST_REF, *PEX_FAST_REF;
typedef struct _CONTROL_AREA                                      // 16 elements, 0x80 bytes (sizeof) 
{
	/*0x000*/     struct _SEGMENT* Segment;
	/*0x008*/     struct _LIST_ENTRY DereferenceList;                           // 2 elements, 0x10 bytes (sizeof)  
	/*0x018*/     UINT64       NumberOfSectionReferences;
	/*0x020*/     UINT64       NumberOfPfnReferences;
	/*0x028*/     UINT64       NumberOfMappedViews;
	/*0x030*/     UINT64       NumberOfUserReferences;
	union                                                         // 2 elements, 0x4 bytes (sizeof)   
	{
		/*0x038*/         ULONG32      LongFlags;
		/*0x038*/         struct _MMSECTION_FLAGS Flags;                            // 27 elements, 0x4 bytes (sizeof)  
	}u;
	/*0x03C*/     ULONG32      FlushInProgressCount;
	/*0x040*/     struct _EX_FAST_REF FilePointer;                              // 3 elements, 0x8 bytes (sizeof)   
	/*0x048*/     LONG32       ControlAreaLock;
	union                                                         // 2 elements, 0x4 bytes (sizeof)   
	{
		/*0x04C*/         ULONG32      ModifiedWriteCount;
		/*0x04C*/         ULONG32      StartingFrame;
	};
	/*0x050*/     struct _MI_SECTION_CREATION_GATE* WaitingForDeletion;
	union                                                         // 1 elements, 0x10 bytes (sizeof)  
	{
		struct                                                    // 9 elements, 0x10 bytes (sizeof)  
		{
			union                                                 // 2 elements, 0x4 bytes (sizeof)   
			{
				/*0x058*/                 ULONG32      NumberOfSystemCacheViews;
				/*0x058*/                 ULONG32      ImageRelocationStartBit;
			};
			union                                                 // 2 elements, 0x4 bytes (sizeof)   
			{
				/*0x05C*/                 LONG32       WritableUserReferences;
				struct                                            // 4 elements, 0x4 bytes (sizeof)   
				{
					/*0x05C*/                     ULONG32      ImageRelocationSizeIn64k : 16;   // 0 BitPosition                    
					/*0x05C*/                     ULONG32      Unused : 14;                     // 16 BitPosition                   
					/*0x05C*/                     ULONG32      BitMap64 : 1;                    // 30 BitPosition                   
					/*0x05C*/                     ULONG32      ImageActive : 1;                 // 31 BitPosition                   
				};
			};
			union                                                 // 2 elements, 0x8 bytes (sizeof)   
			{
				/*0x060*/                 struct _MM_SUBSECTION_AVL_TABLE* SubsectionRoot;
				/*0x060*/                 struct _MI_IMAGE_SECURITY_REFERENCE* SeImageStub;
			};
		}e2;
	}u2;
	/*0x068*/     INT64        LockedPages;
	/*0x070*/     struct _LIST_ENTRY ViewList;                                  // 2 elements, 0x10 bytes (sizeof)  
}CONTROL_AREA, *PCONTROL_AREA;
typedef struct _MMADDRESS_NODE          // 5 elements, 0x28 bytes (sizeof) 
{
	union                               // 2 elements, 0x8 bytes (sizeof)  
	{
		/*0x000*/         INT64        Balance : 2;       // 0 BitPosition                   
		/*0x000*/         struct _MMADDRESS_NODE* Parent;
	}u1;
	/*0x008*/     struct _MMADDRESS_NODE* LeftChild;
	/*0x010*/     struct _MMADDRESS_NODE* RightChild;
	/*0x018*/     UINT64       StartingVpn;
	/*0x020*/     UINT64       EndingVpn;
}MMADDRESS_NODE, *PMMADDRESS_NODE;
typedef struct _MM_AVL_TABLE                          // 6 elements, 0x40 bytes (sizeof) 
{
	/*0x000*/     struct _MMADDRESS_NODE BalancedRoot;              // 5 elements, 0x28 bytes (sizeof) 
	struct                                            // 3 elements, 0x8 bytes (sizeof)  
	{
		/*0x028*/         UINT64       DepthOfTree : 5;                 // 0 BitPosition                   
		/*0x028*/         UINT64       Unused : 3;                      // 5 BitPosition                   
		/*0x028*/         UINT64       NumberGenericTableElements : 56; // 8 BitPosition                   
	};
	/*0x030*/     VOID*        NodeHint;
	/*0x038*/     VOID*        NodeFreeHint;
}MM_AVL_TABLE, *PMM_AVL_TABLE;
typedef struct _MMVAD_FLAGS2          // 9 elements, 0x4 bytes (sizeof) 
{
	/*0x000*/     UINT32       FileOffset : 24;     // 0 BitPosition                  
	/*0x000*/     UINT32       SecNoChange : 1;     // 24 BitPosition                 
	/*0x000*/     UINT32       OneSecured : 1;      // 25 BitPosition                 
	/*0x000*/     UINT32       MultipleSecured : 1; // 26 BitPosition                 
	/*0x000*/     UINT32       Spare : 1;           // 27 BitPosition                 
	/*0x000*/     UINT32       LongVad : 1;         // 28 BitPosition                 
	/*0x000*/     UINT32       ExtendableFile : 1;  // 29 BitPosition                 
	/*0x000*/     UINT32       Inherit : 1;         // 30 BitPosition                 
	/*0x000*/     UINT32       CopyOnWrite : 1;     // 31 BitPosition                 
}MMVAD_FLAGS2, *PMMVAD_FLAGS2;
typedef struct _MMSUBSECTION_FLAGS            // 8 elements, 0x4 bytes (sizeof) 
{
	struct                                    // 3 elements, 0x2 bytes (sizeof) 
	{
		/*0x000*/         UINT16       SubsectionAccessed : 1;  // 0 BitPosition                  
		/*0x000*/         UINT16       Protection : 5;          // 1 BitPosition                  
		/*0x000*/         UINT16       StartingSector4132 : 10; // 6 BitPosition                  
	};
	struct                                    // 5 elements, 0x2 bytes (sizeof) 
	{
		/*0x002*/         UINT16       SubsectionStatic : 1;    // 0 BitPosition                  
		/*0x002*/         UINT16       GlobalMemory : 1;        // 1 BitPosition                  
		/*0x002*/         UINT16       DirtyPages : 1;          // 2 BitPosition                  
		/*0x002*/         UINT16       Spare : 1;               // 3 BitPosition                  
		/*0x002*/         UINT16       SectorEndOffset : 12;    // 4 BitPosition                  
	};
}MMSUBSECTION_FLAGS, *PMMSUBSECTION_FLAGS;
typedef struct _SUBSECTION                          // 9 elements, 0x38 bytes (sizeof) 
{
	/*0x000*/     struct _CONTROL_AREA* ControlArea;
	/*0x008*/     struct _MMPTE* SubsectionBase;
	/*0x010*/     struct _SUBSECTION* NextSubsection;
	/*0x018*/     ULONG32      PtesInSubsection;
	/*0x01C*/     UINT8        _PADDING0_[0x4];
	union                                           // 2 elements, 0x8 bytes (sizeof)  
	{
		/*0x020*/         ULONG32      UnusedPtes;
		/*0x020*/         struct _MM_AVL_TABLE* GlobalPerSessionHead;
	};
	union                                           // 2 elements, 0x4 bytes (sizeof)  
	{
		/*0x028*/         ULONG32      LongFlags;
		/*0x028*/         struct _MMSUBSECTION_FLAGS SubsectionFlags; // 8 elements, 0x4 bytes (sizeof)  
	}u;
	/*0x02C*/     ULONG32      StartingSector;
	/*0x030*/     ULONG32      NumberOfFullSectors;
	/*0x034*/     UINT8        _PADDING1_[0x4];
}SUBSECTION, *PSUBSECTION;
typedef struct _MMVAD                          // 15 elements, 0x78 bytes (sizeof) 
{
	union                                      // 2 elements, 0x8 bytes (sizeof)   
	{
		/*0x000*/         INT64        Balance : 2;              // 0 BitPosition                    
		/*0x000*/         struct _MMVAD* Parent;
	}u1;
	/*0x008*/     struct _MMVAD* LeftChild;
	/*0x010*/     struct _MMVAD* RightChild;
	/*0x018*/     UINT64       StartingVpn;
	/*0x020*/     UINT64       EndingVpn;
	union                                      // 2 elements, 0x8 bytes (sizeof)   
	{
		/*0x028*/         UINT64       LongFlags;
		/*0x028*/         struct _MMVAD_FLAGS VadFlags;          // 7 elements, 0x8 bytes (sizeof)   
	}u;
	/*0x030*/     struct _EX_PUSH_LOCK PushLock;             // 7 elements, 0x8 bytes (sizeof)   
	union                                      // 2 elements, 0x8 bytes (sizeof)   
	{
		/*0x038*/         UINT64       LongFlags3;
		/*0x038*/         struct _MMVAD_FLAGS3 VadFlags3;        // 8 elements, 0x8 bytes (sizeof)   
	}u5;
	union                                      // 2 elements, 0x4 bytes (sizeof)   
	{
		/*0x040*/         ULONG32      LongFlags2;
		/*0x040*/         struct _MMVAD_FLAGS2 VadFlags2;        // 9 elements, 0x4 bytes (sizeof)   
	}u2;
	union                                      // 2 elements, 0x8 bytes (sizeof)   
	{
		/*0x048*/         struct _SUBSECTION* Subsection;
		/*0x048*/         struct _MSUBSECTION* MappedSubsection;
	};
	/*0x050*/     struct _MMPTE* FirstPrototypePte;
	/*0x058*/     struct _MMPTE* LastContiguousPte;
	/*0x060*/     struct _LIST_ENTRY ViewLinks;              // 2 elements, 0x10 bytes (sizeof)  
	/*0x070*/     struct _EPROCESS* VadsProcess;
}MMVAD, *PMMVAD;