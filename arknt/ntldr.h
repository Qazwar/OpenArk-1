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








#endif
