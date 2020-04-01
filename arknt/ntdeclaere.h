#pragma once
#pragma warning(disable:4091)

typedef struct _ACCESS_REASONS;
typedef struct PHANDLE_TABLE_ENTRY;
typedef struct PHANDLE_TABLE;
typedef struct _ACCESS_STATE;
typedef struct _ACL;
typedef struct _ACTIVATION_CONTEXT;
typedef struct _ACTIVATION_CONTEXT_DATA;
typedef struct _ACTIVATION_CONTEXT_STACK;
typedef struct _ADAPTER_OBJECT;
typedef struct _ALIGNED_AFFINITY_SUMMARY;
typedef struct _ALPC_COMMUNICATION_INFO;
typedef struct _ALPC_COMPLETION_LIST;
typedef struct _ALPC_COMPLETION_LIST_HEADER;
typedef struct _ALPC_COMPLETION_LIST_STATE;
typedef struct _ALPC_COMPLETION_PACKET_LOOKASIDE;
typedef struct _ALPC_COMPLETION_PACKET_LOOKASIDE_ENTRY;
typedef struct _ALPC_DISPATCH_CONTEXT;
typedef struct _ALPC_HANDLE_ENTRY;
typedef struct _ALPC_HANDLE_TABLE;
typedef struct _ALPC_MESSAGE_ATTRIBUTES;
typedef struct _ALPC_MESSAGE_ZONE;
typedef struct _ALPC_PORT;
typedef struct _ALPC_PORT_ATTRIBUTES;
typedef struct _ALPC_PROCESS_CONTEXT;
typedef struct _AMD64_DBGKD_CONTROL_SET;
typedef struct _ARBITER_ADD_RESERVED_PARAMETERS;
typedef struct _ARBITER_ALLOCATION_STATE;
typedef struct _ARBITER_ALTERNATIVE;
typedef struct _ARBITER_BOOT_ALLOCATION_PARAMETERS;
typedef struct _ARBITER_CONFLICT_INFO;
typedef struct _ARBITER_INSTANCE;
typedef struct _ARBITER_INTERFACE;
typedef struct _ARBITER_LIST_ENTRY;
typedef struct _ARBITER_ORDERING;
typedef struct _ARBITER_ORDERING_LIST;
typedef struct _ARBITER_PARAMETERS;
typedef struct _ARBITER_QUERY_ALLOCATED_RESOURCES_PARAMETERS;
typedef struct _ARBITER_QUERY_ARBITRATE_PARAMETERS;
typedef struct _ARBITER_QUERY_CONFLICT_PARAMETERS;
typedef struct _ARBITER_RETEST_ALLOCATION_PARAMETERS;
typedef struct _ARBITER_TEST_ALLOCATION_PARAMETERS;
typedef struct _ARC_DISK_INFORMATION;
typedef struct _ARM_DBGKD_CONTROL_SET;
typedef struct _ASSEMBLY_STORAGE_MAP;
typedef struct _AUTHZBASEP_SECURITY_ATTRIBUTES_INFORMATION;
typedef struct _AUX_ACCESS_DATA;
typedef struct _BATTERY_REPORTING_SCALE;
typedef struct _BITMAP_RANGE;
typedef struct _BLOB;
typedef struct _BLOB_TYPE;
typedef struct _BUS_EXTENSION_LIST;
typedef struct _CACHE_DESCRIPTOR;
typedef struct _CACHE_MANAGER_CALLBACKS;
typedef struct _CACHE_UNINITIALIZE_EVENT;
typedef struct _CACHED_CHILD_LIST;
typedef struct _CACHED_KSTACK_LIST;
typedef struct _CALL_HASH_ENTRY;
typedef struct _CALL_PERFORMANCE_DATA;
typedef struct _CALLBACK_OBJECT;
typedef struct _CELL_DATA;
typedef struct _CHILD_LIST;
typedef struct _CLIENT_ID;
typedef struct _CLIENT_ID32;
typedef struct _CLIENT_ID64;
typedef struct _CM_BIG_DATA;
typedef struct _CM_CACHED_VALUE_INDEX;
typedef struct _CM_CELL_REMAP_BLOCK;
typedef struct _CM_FULL_RESOURCE_DESCRIPTOR;
typedef struct _CM_INDEX_HINT_BLOCK;
typedef struct _CM_INTENT_LOCK;
typedef struct _CM_KCB_UOW;
typedef struct _CM_KEY_BODY;
typedef struct _CM_KEY_CONTROL_BLOCK;
typedef struct _CM_KEY_HASH;
typedef struct _CM_KEY_HASH_TABLE_ENTRY;
typedef struct _CM_KEY_INDEX;
typedef struct _CM_KEY_NODE;
typedef struct _CM_KEY_REFERENCE;
typedef struct _CM_KEY_SECURITY;
typedef struct _CM_KEY_SECURITY_CACHE;
typedef struct _CM_KEY_SECURITY_CACHE_ENTRY;
typedef struct _CM_KEY_VALUE;
typedef struct _CM_NAME_CONTROL_BLOCK;
typedef struct _CM_NAME_HASH;
typedef struct _CM_NOTIFY_BLOCK;
typedef struct _CM_PARTIAL_RESOURCE_DESCRIPTOR;
typedef struct _CM_PARTIAL_RESOURCE_LIST;
typedef struct _CM_RESOURCE_LIST;
typedef struct _CM_RM;
typedef struct _CM_TRANS;
typedef struct _CM_VIEW_OF_FILE;
typedef struct _CM_WORKITEM;
typedef struct _CMHIVE;
typedef struct _CMP_OFFSET_ARRAY;
typedef struct _COMPRESSED_DATA_INFO;
typedef struct _CONFIGURATION_COMPONENT;
typedef struct _CONFIGURATION_COMPONENT_DATA;
typedef struct _CONTEXT;
typedef struct _CONTEXT32_UPDATE;
typedef struct _CONTROL_AREA;
typedef struct _COUNTER_READING;
typedef struct _CPU_INFO;
typedef struct _CURDIR;
typedef struct _DBGKD_ANY_CONTROL_SET;
typedef struct _DBGKD_BREAKPOINTEX;
typedef struct _DBGKD_CONTINUE;
typedef struct _DBGKD_CONTINUE2;
typedef struct _DBGKD_FILL_MEMORY;
typedef struct _DBGKD_GET_CONTEXT;
typedef struct _DBGKD_GET_INTERNAL_BREAKPOINT32;
typedef struct _DBGKD_GET_INTERNAL_BREAKPOINT64;
typedef struct _DBGKD_GET_SET_BUS_DATA;
typedef struct _DBGKD_GET_VERSION32;
typedef struct _DBGKD_GET_VERSION64;
typedef struct _DBGKD_LOAD_SYMBOLS32;
typedef struct _DBGKD_LOAD_SYMBOLS64;
typedef struct _DBGKD_MANIPULATE_STATE32;
typedef struct _DBGKD_MANIPULATE_STATE64;
typedef struct _DBGKD_QUERY_MEMORY;
typedef struct _DBGKD_QUERY_SPECIAL_CALLS;
typedef struct _DBGKD_READ_MEMORY32;
typedef struct _DBGKD_READ_MEMORY64;
typedef struct _DBGKD_READ_WRITE_IO_EXTENDED32;
typedef struct _DBGKD_READ_WRITE_IO_EXTENDED64;
typedef struct _DBGKD_READ_WRITE_IO32;
typedef struct _DBGKD_READ_WRITE_IO64;
typedef struct _DBGKD_READ_WRITE_MSR;
typedef struct _DBGKD_RESTORE_BREAKPOINT;
typedef struct _DBGKD_SEARCH_MEMORY;
typedef struct _DBGKD_SET_CONTEXT;
typedef struct _DBGKD_SET_INTERNAL_BREAKPOINT32;
typedef struct _DBGKD_SET_INTERNAL_BREAKPOINT64;
typedef struct _DBGKD_SET_SPECIAL_CALL32;
typedef struct _DBGKD_SET_SPECIAL_CALL64;
typedef struct _DBGKD_SWITCH_PARTITION;
typedef struct _DBGKD_WRITE_BREAKPOINT32;
typedef struct _DBGKD_WRITE_BREAKPOINT64;
typedef struct _DBGKD_WRITE_MEMORY32;
typedef struct _DBGKD_WRITE_MEMORY64;
typedef struct _DBGKM_EXCEPTION32;
typedef struct _DBGKM_EXCEPTION64;
typedef struct _DEFERRED_WRITE;
typedef struct _DEVICE_CAPABILITIES;
typedef struct _DEVICE_FLAGS;
typedef struct _DEVICE_MAP;
typedef struct _DEVICE_NODE;
typedef struct _DEVICE_OBJECT;
typedef struct _DEVICE_OBJECT_POWER_EXTENSION;
typedef struct _DEVICE_RELATIONS;
typedef struct _DEVOBJ_EXTENSION;
typedef struct _DIAGNOSTIC_BUFFER;
typedef struct _DIAGNOSTIC_CONTEXT;
typedef struct _DISPATCHER_HEADER;
typedef struct _DOCK_INTERFACE;
typedef struct _DRIVER_EXTENSION;
typedef struct _DRIVER_OBJECT;
typedef struct _DUAL;
typedef struct _DUMMY_FILE_OBJECT;
typedef struct _DUMP_INITIALIZATION_CONTEXT;
typedef struct _DUMP_STACK_CONTEXT;
typedef struct _ECP_LIST;
typedef struct _EFI_FIRMWARE_INFORMATION;
typedef struct _EJOB;
typedef struct _EPROCESS;
typedef struct _EPROCESS_QUOTA_BLOCK;
typedef struct _ERESOURCE;
typedef struct _ETHREAD;
typedef struct _ETIMER;
typedef struct _ETW_BUFFER_CONTEXT;
typedef struct _ETW_BUFFER_HANDLE;
typedef struct _ETW_GUID_ENTRY;
typedef struct _ETW_LAST_ENABLE_INFO;
typedef struct _ETW_LOGGER_HANDLE;
typedef struct _ETW_PERF_COUNTERS;
typedef struct _ETW_PROVIDER_TABLE_ENTRY;
typedef struct _ETW_REALTIME_CONSUMER;
typedef struct _ETW_REF_CLOCK;
typedef struct _ETW_REG_ENTRY;
typedef struct _ETW_REPLY_QUEUE;
typedef struct _ETW_SESSION_PERF_COUNTERS;
typedef struct _ETW_SYSTEMTIME;
typedef struct _ETW_WMITRACE_WORK;
typedef struct _EVENT_DATA_DESCRIPTOR;
typedef struct _EVENT_DESCRIPTOR;
typedef struct _EVENT_FILTER_HEADER;
typedef struct _EX_FAST_REF;
typedef struct _EX_PUSH_LOCK;
typedef struct _EX_PUSH_LOCK_CACHE_AWARE;
typedef struct _EX_PUSH_LOCK_WAIT_BLOCK;
typedef union _EX_QUEUE_WORKER_INFO;
typedef struct _EX_RUNDOWN_REF;
typedef struct _EX_WORK_QUEUE;
typedef struct _EXCEPTION_POINTERS;
typedef struct _EXCEPTION_RECORD;
typedef struct _EXCEPTION_RECORD32;
typedef struct _EXCEPTION_RECORD64;
typedef struct _EXCEPTION_REGISTRATION_RECORD;
typedef struct _FAST_IO_DISPATCH;
typedef struct _FAST_MUTEX;
typedef struct _FILE_BASIC_INFORMATION;
typedef struct _FILE_GET_QUOTA_INFORMATION;
typedef struct _FILE_NETWORK_OPEN_INFORMATION;
typedef struct _FILE_OBJECT;
typedef union _FILE_SEGMENT_ELEMENT;
typedef struct _FILE_STANDARD_INFORMATION;
typedef struct _FIRMWARE_INFORMATION_LOADER_BLOCK;
typedef struct _flags;
typedef struct _FLS_CALLBACK_INFO;
typedef struct _FREE_DISPLAY;
typedef struct _FS_FILTER_CALLBACK_DATA;
typedef struct _FS_FILTER_CALLBACKS;
typedef union _FS_FILTER_PARAMETERS;
typedef struct _FSRTL_ADVANCED_FCB_HEADER;
typedef struct _GDI_TEB_BATCH;
typedef struct _GDI_TEB_BATCH32;
typedef struct _GDI_TEB_BATCH64;
typedef struct _GENERAL_LOOKASIDE;
typedef struct _GENERAL_LOOKASIDE_POOL;
typedef struct _GENERIC_MAPPING;
typedef struct _GROUP_AFFINITY;
typedef struct _GUID;
typedef struct _HANDLE_TABLE;
typedef struct _HANDLE_TABLE_ENTRY;
typedef struct _HANDLE_TABLE_ENTRY_INFO;
typedef struct _HANDLE_TRACE_DB_ENTRY;
typedef struct _HANDLE_TRACE_DEBUG_INFO;
typedef struct _HARDWARE_PTE;
typedef struct _HBASE_BLOCK;
typedef struct _HBIN;
typedef struct _HCELL;
typedef struct _HEADLESS_LOADER_BLOCK;
typedef struct _HEAP;
typedef struct _HEAP_COUNTERS;
typedef struct _HEAP_DEBUGGING_INFORMATION;
typedef struct _HEAP_ENTRY;
typedef struct _HEAP_ENTRY_EXTRA;
typedef struct _HEAP_FREE_ENTRY;
typedef struct _HEAP_FREE_ENTRY_EXTRA;
typedef struct _HEAP_LIST_LOOKUP;
typedef struct _HEAP_LOCAL_SEGMENT_INFO;
typedef struct _HEAP_LOCK;
typedef struct _HEAP_LOOKASIDE;
typedef struct _HEAP_PSEUDO_TAG_ENTRY;
typedef struct _HEAP_SEGMENT;
typedef struct _HEAP_STOP_ON_TAG;
typedef struct _HEAP_STOP_ON_VALUES;
typedef struct _HEAP_SUBSEGMENT;
typedef struct _HEAP_TAG_ENTRY;
typedef struct _HEAP_TUNING_PARAMETERS;
typedef struct _HEAP_UCR_DESCRIPTOR;
typedef struct _HEAP_USERDATA_HEADER;
typedef struct _HEAP_VIRTUAL_ALLOC_ENTRY;
typedef struct _HHIVE;
typedef struct _HIVE_LIST_ENTRY;
typedef struct _HIVE_LOAD_FAILURE;
typedef struct _HMAP_DIRECTORY;
typedef struct _HMAP_ENTRY;
typedef struct _HMAP_TABLE;
typedef struct _I386_LOADER_BLOCK;
typedef struct _IA64_DBGKD_CONTROL_SET;
typedef struct _IA64_LOADER_BLOCK;
typedef struct _IMAGE_DATA_DIRECTORY;
typedef struct _IMAGE_DEBUG_DIRECTORY;
typedef struct _IMAGE_DOS_HEADER;
typedef struct _IMAGE_FILE_HEADER;
typedef struct _IMAGE_NT_HEADERS64;
typedef struct _IMAGE_OPTIONAL_HEADER64;
typedef struct _IMAGE_ROM_OPTIONAL_HEADER;
typedef struct _IMAGE_SECTION_HEADER;
typedef struct _IMAGE_SECURITY_CONTEXT;
typedef struct _INITIAL_PRIVILEGE_SET;
typedef struct _INTERFACE;
typedef struct _INTERLOCK_SEQ;
typedef struct _IO_CLIENT_EXTENSION;
typedef struct _IO_COMPLETION_CONTEXT;
typedef struct _IO_DRIVER_CREATE_CONTEXT;
typedef struct _IO_MINI_COMPLETION_PACKET_USER;
typedef struct _IO_PRIORITY_INFO;
typedef struct _IO_RESOURCE_DESCRIPTOR;
typedef struct _IO_RESOURCE_LIST;
typedef struct _IO_RESOURCE_REQUIREMENTS_LIST;
typedef struct _IO_SECURITY_CONTEXT;
typedef struct _IO_STACK_LOCATION;
typedef struct _IO_STATUS_BLOCK;
typedef struct _IO_STATUS_BLOCK32;
typedef struct _IO_TIMER;
typedef struct _IO_WORKITEM;
typedef struct _iobuf;
typedef struct _IOP_FILE_OBJECT_EXTENSION;
typedef struct _IOV_FORCED_PENDING_TRACE;
typedef struct _IRP;
typedef struct _JOB_ACCESS_STATE;
typedef struct _KAFFINITY_ENUMERATION_CONTEXT;
typedef struct _KAFFINITY_EX;
typedef struct _KALPC_HANDLE_DATA;
typedef struct _KALPC_MESSAGE;
typedef struct _KALPC_MESSAGE_ATTRIBUTES;
typedef struct _KALPC_REGION;
typedef struct _KALPC_RESERVE;
typedef struct _KALPC_SECTION;
typedef struct _KALPC_SECURITY_DATA;
typedef struct _KALPC_VIEW;
typedef struct _KAPC;
typedef struct _KAPC_STATE;
typedef union _KBUGCHECK_ACTIVE_STATE;
typedef struct _KDESCRIPTOR;
typedef struct _KDEVICE_QUEUE;
typedef struct _KDEVICE_QUEUE_ENTRY;
typedef struct _KDPC;
typedef struct _KDPC_DATA;
typedef struct _KENLISTMENT;
typedef struct _KENLISTMENT_HISTORY;
typedef struct _KERNEL_STACK_CONTROL;
typedef struct _KERNEL_STACK_SEGMENT;
typedef struct _KEVENT;
typedef struct _KEXCEPTION_FRAME;
typedef union _KEXECUTE_OPTIONS;
typedef struct _KGATE;
typedef union _KGDTENTRY64;
typedef struct _KGUARDED_MUTEX;
typedef union _KIDTENTRY64;
typedef struct _KINTERRUPT;
typedef struct _KLOCK_QUEUE_HANDLE;
typedef struct _KMUTANT;
typedef struct _KNODE;
typedef struct _KPCR;
typedef struct _KPRCB;
typedef struct _KPROCESS;
typedef struct _KPROCESSOR_STATE;
typedef struct _KQUEUE;
typedef struct _KREQUEST_PACKET;
typedef struct _KRESOURCEMANAGER;
typedef struct _KRESOURCEMANAGER_COMPLETION_BINDING;
typedef struct _KSEMAPHORE;
typedef struct _KSPECIAL_REGISTERS;
typedef struct _KSPIN_LOCK_QUEUE;
typedef union _KSTACK_COUNT;
typedef struct _KSYSTEM_TIME;
typedef struct _KTHREAD;
typedef struct _KTHREAD_COUNTERS;
typedef struct _KTIMER;
typedef struct _KTIMER_TABLE;
typedef struct _KTIMER_TABLE_ENTRY;
typedef struct _KTM;
typedef struct _KTMNOTIFICATION_PACKET;
typedef struct _KTMOBJECT_NAMESPACE;
typedef struct _KTMOBJECT_NAMESPACE_LINK;
typedef struct _KTRANSACTION;
typedef struct _KTRANSACTION_HISTORY;
typedef struct _KTRAP_FRAME;
typedef struct _KTSS64;
typedef struct _KUMS_CONTEXT_HEADER;
typedef struct _KUSER_SHARED_DATA;
typedef struct _KWAIT_BLOCK;
typedef union _KWAIT_STATUS_REGISTER;
typedef union _LARGE_INTEGER;
typedef struct _LAZY_WRITER;
typedef struct _LDR_DATA_TABLE_ENTRY;
typedef struct _LIST_ENTRY;
typedef struct _LIST_ENTRY32;
typedef struct _LIST_ENTRY64;
typedef struct _LOADER_PARAMETER_BLOCK;
typedef struct _LOADER_PARAMETER_EXTENSION;
typedef struct _LOADER_PERFORMANCE_DATA;
typedef struct _LOOKASIDE_LIST_EX;
typedef struct _LPCP_MESSAGE;
typedef struct _LPCP_NONPAGED_PORT_QUEUE;
typedef struct _LPCP_PORT_OBJECT;
typedef struct _LPCP_PORT_QUEUE;
typedef struct _LUID;
typedef struct _LUID_AND_ATTRIBUTES;
typedef struct _M128A;
typedef struct _MAILSLOT_CREATE_PARAMETERS;
typedef struct _MAPPED_FILE_SEGMENT;
typedef struct _MBCB;
typedef struct _MDL;
typedef struct _MEMORY_ALLOCATION_DESCRIPTOR;
typedef struct _MI_COLOR_BASE;
typedef struct _MI_EXTRA_IMAGE_INFORMATION;
typedef struct _MI_IMAGE_SECURITY_REFERENCE;
typedef struct _MI_PAGEFILE_TRACES;
typedef struct _MI_SECTION_CREATION_GATE;
typedef struct _MI_SECTION_IMAGE_INFORMATION;
typedef struct _MI_SPECIAL_POOL;
typedef struct _MI_SPECIAL_POOL_PTE_LIST;
typedef struct _MI_SYSTEM_PTE_TYPE;
typedef struct _MI_VERIFIER_POOL_HEADER;
typedef struct _MM_AVL_TABLE;
typedef struct _MM_DRIVER_VERIFIER_DATA;
typedef struct _MM_PAGE_ACCESS_INFO;
typedef union _MM_PAGE_ACCESS_INFO_FLAGS;
typedef struct _MM_PAGE_ACCESS_INFO_HEADER;
typedef struct _MM_PAGED_POOL_INFO;
typedef struct _MM_SESSION_SPACE;
typedef struct _MM_SESSION_SPACE_FLAGS;
typedef union _MM_STORE_KEY;
typedef struct _MM_SUBSECTION_AVL_TABLE;
typedef struct _MMADDRESS_LIST;
typedef struct _MMADDRESS_NODE;
typedef struct _MMBANKED_SECTION;
typedef struct _MMEXTEND_INFO;
typedef struct _MMMOD_WRITER_MDL_ENTRY;
typedef struct _MMPAGING_FILE;
typedef struct _MMPFN;
typedef struct _MMPFNENTRY;
typedef struct _MMPFNLIST;
typedef struct _MMPTE;
typedef struct _MMPTE_HARDWARE;
typedef struct _MMPTE_LIST;
typedef struct _MMPTE_PROTOTYPE;
typedef struct _MMPTE_SOFTWARE;
typedef struct _MMPTE_SUBSECTION;
typedef struct _MMPTE_TIMESTAMP;
typedef struct _MMPTE_TRANSITION;
typedef struct _MMSECTION_FLAGS;
typedef struct _MMSECURE_FLAGS;
typedef struct _MMSESSION;
typedef struct _MMSUBSECTION_FLAGS;
typedef struct _MMSUBSECTION_NODE;
typedef struct _MMSUPPORT;
typedef struct _MMSUPPORT_FLAGS;
typedef struct _MMVAD;
typedef struct _MMVAD_FLAGS;
typedef struct _MMVAD_FLAGS2;
typedef struct _MMVAD_FLAGS3;
typedef struct _MMVAD_LONG;
typedef struct _MMVAD_SHORT;
typedef struct _MMVIEW;
typedef struct _MMWSL;
typedef struct _MMWSLE;
typedef struct _MMWSLE_FREE_ENTRY;
typedef struct _MMWSLE_HASH;
typedef struct _MMWSLE_NONDIRECT_HASH;
typedef struct _MMWSLENTRY;
typedef struct _MSUBSECTION;
typedef struct _NAMED_PIPE_CREATE_PARAMETERS;
typedef struct _NBQUEUE_BLOCK;
typedef struct _NETWORK_LOADER_BLOCK;
typedef struct _NLS_DATA_BLOCK;
typedef struct _NPAGED_LOOKASIDE_LIST;
typedef struct _NT_TIB;
typedef struct _NT_TIB32;
typedef struct _NT_TIB64;
typedef struct _OB_DUPLICATE_OBJECT_STATE;
typedef struct _OBJECT_ATTRIBUTES;
typedef struct _OBJECT_CREATE_INFORMATION;
typedef struct _OBJECT_DIRECTORY;
typedef struct _OBJECT_DIRECTORY_ENTRY;
typedef struct _OBJECT_DUMP_CONTROL;
typedef struct _OBJECT_HANDLE_COUNT_DATABASE;
typedef struct _OBJECT_HANDLE_COUNT_ENTRY;
typedef struct _OBJECT_HANDLE_INFORMATION;
typedef struct _OBJECT_HEADER;
typedef struct _OBJECT_HEADER_CREATOR_INFO;
typedef struct _OBJECT_HEADER_HANDLE_INFO;
typedef struct _OBJECT_HEADER_NAME_INFO;
typedef struct _OBJECT_HEADER_PROCESS_INFO;
typedef struct _OBJECT_HEADER_QUOTA_INFO;
typedef struct _OBJECT_NAME_INFORMATION;
typedef struct _OBJECT_REF_INFO;
typedef struct _OBJECT_REF_STACK_INFO;
typedef struct _OBJECT_REF_TRACE;
typedef struct _OBJECT_SYMBOLIC_LINK;
typedef struct _OBJECT_TYPE;
typedef struct _OBJECT_TYPE_INITIALIZER;
typedef struct _OBP_LOOKUP_CONTEXT;
typedef struct _OPEN_PACKET;
typedef struct _OWNER_ENTRY;
typedef struct _PAGED_LOOKASIDE_LIST;
typedef struct _PAGEFAULT_HISTORY;
typedef struct _PCAT_FIRMWARE_INFORMATION;
typedef struct _PCW_BUFFER;
typedef union _PCW_CALLBACK_INFORMATION;
typedef struct _PCW_COUNTER_DESCRIPTOR;
typedef struct _PCW_COUNTER_INFORMATION;
typedef struct _PCW_DATA;
typedef struct _PCW_INSTANCE;
typedef struct _PCW_MASK_INFORMATION;
typedef struct _PCW_PROCESSOR_INFO;
typedef struct _PCW_REGISTRATION;
typedef struct _PCW_REGISTRATION_INFORMATION;
typedef struct _PEB;
typedef struct _PEB_LDR_DATA;
typedef struct _PEB32;
typedef struct _PEB64;
typedef struct _PENDING_RELATIONS_LIST_ENTRY;
typedef struct _PERFINFO_GROUPMASK;
typedef struct _PERFINFO_TRACE_HEADER;
typedef struct _PF_KERNEL_GLOBALS;
typedef struct _PHYSICAL_MEMORY_DESCRIPTOR;
typedef struct _PHYSICAL_MEMORY_RUN;
typedef struct _PI_BUS_EXTENSION;
typedef struct _PI_RESOURCE_ARBITER_ENTRY;
typedef struct _PLUGPLAY_EVENT_BLOCK;
typedef struct _PNP_ASSIGN_RESOURCES_CONTEXT;
typedef struct _PNP_DEVICE_ACTION_ENTRY;
typedef struct _PNP_DEVICE_COMPLETION_QUEUE;
typedef struct _PNP_DEVICE_COMPLETION_REQUEST;
typedef struct _PNP_DEVICE_EVENT_ENTRY;
typedef struct _PNP_DEVICE_EVENT_LIST;
typedef struct _PNP_RESOURCE_REQUEST;
typedef struct _PO_DEVICE_NOTIFY;
typedef struct _PO_DEVICE_NOTIFY_ORDER;
typedef struct _PO_DIAG_STACK_RECORD;
typedef struct _PO_HIBER_PERF;
typedef struct _PO_IRP_MANAGER;
typedef struct _PO_IRP_QUEUE;
typedef struct _PO_MEMORY_IMAGE;
typedef struct _PO_NOTIFY_ORDER_LEVEL;
typedef struct _POOL_BLOCK_HEAD;
typedef struct _POOL_DESCRIPTOR;
typedef struct _POOL_HACKER;
typedef struct _POOL_HEADER;
typedef struct _POOL_TRACKER_BIG_PAGES;
typedef struct _POOL_TRACKER_TABLE;
typedef struct _POP_ACTION_TRIGGER;
typedef struct _POP_DEVICE_SYS_STATE;
typedef struct _POP_HIBER_CONTEXT;
typedef struct _POP_POWER_ACTION;
typedef struct _POP_SHUTDOWN_BUG_CHECK;
typedef struct _POP_SYSTEM_IDLE;
typedef struct _POP_THERMAL_ZONE;
typedef struct _POP_THERMAL_ZONE_METRICS;
typedef struct _POP_TRIGGER_WAIT;
typedef struct _PORT_MESSAGE;
typedef struct _PORT_MESSAGE32;
typedef struct _POWER_ACTION_POLICY;
typedef struct _POWER_SEQUENCE;
typedef union _POWER_STATE;
typedef struct _PP_LOOKASIDE_LIST;
typedef struct _PPC_DBGKD_CONTROL_SET;
typedef struct _PPM_FFH_THROTTLE_STATE_INFO;
typedef struct _PPM_IDLE_STATE;
typedef struct _PPM_IDLE_STATES;
typedef struct _PPM_PERF_STATE;
typedef struct _PPM_PERF_STATES;
typedef struct _PRIVATE_CACHE_MAP;
typedef struct _PRIVATE_CACHE_MAP_FLAGS;
typedef struct _PRIVILEGE_SET;
typedef struct _PROC_HISTORY_ENTRY;
typedef struct _PROC_IDLE_ACCOUNTING;
typedef struct _PROC_IDLE_SNAP;
typedef struct _PROC_IDLE_STATE_ACCOUNTING;
typedef struct _PROC_IDLE_STATE_BUCKET;
typedef struct _PROC_PERF_CONSTRAINT;
typedef struct _PROC_PERF_DOMAIN;
typedef struct _PROC_PERF_LOAD;
typedef struct _PROCESSOR_IDLESTATE_INFO;
typedef struct _PROCESSOR_IDLESTATE_POLICY;
typedef struct _PROCESSOR_NUMBER;
typedef struct _PROCESSOR_PERFSTATE_POLICY;
typedef struct _PROCESSOR_POWER_STATE;
typedef struct _PROFILE_PARAMETER_BLOCK;
typedef union _PS_CLIENT_SECURITY_CONTEXT;
typedef struct _PS_CPU_QUOTA_BLOCK;
typedef struct _PS_PER_CPU_QUOTA_CACHE_AWARE;
typedef union _PSP_CPU_QUOTA_APC;
typedef union _PSP_CPU_SHARE_CAPTURED_WEIGHT_DATA;
typedef struct _PTE_TRACKER;
typedef struct _QUAD;
typedef struct _RELATION_LIST;
typedef struct _RELATION_LIST_ENTRY;
typedef struct _RELATIVE_SYMLINK_INFO;
typedef struct _REMOTE_PORT_VIEW;
typedef struct _REQUEST_MAILBOX;
typedef struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME;
typedef struct _RTL_ATOM_TABLE;
typedef struct _RTL_ATOM_TABLE_ENTRY;
typedef struct _RTL_AVL_TABLE;
typedef struct _RTL_BALANCED_LINKS;
typedef struct _RTL_BITMAP;
typedef struct _RTL_CRITICAL_SECTION;
typedef struct _RTL_CRITICAL_SECTION_DEBUG;
typedef struct _RTL_DRIVE_LETTER_CURDIR;
typedef struct _RTL_DYNAMIC_HASH_TABLE;
typedef struct _RTL_DYNAMIC_HASH_TABLE_CONTEXT;
typedef struct _RTL_DYNAMIC_HASH_TABLE_ENTRY;
typedef struct _RTL_DYNAMIC_HASH_TABLE_ENUMERATOR;
typedef struct _RTL_HANDLE_TABLE;
typedef struct _RTL_HANDLE_TABLE_ENTRY;
typedef struct _RTL_RANGE;
typedef struct _RTL_RANGE_LIST;
typedef struct _RTL_SRWLOCK;
typedef struct _RTL_UMS_CONTEXT;
typedef struct _RTL_USER_PROCESS_PARAMETERS;
typedef struct _RTLP_RANGE_LIST_ENTRY;
typedef struct _SCSI_REQUEST_BLOCK;
typedef struct _SE_AUDIT_PROCESS_CREATION_INFO;
typedef struct _SECTION_IMAGE_INFORMATION;
typedef struct _SECTION_OBJECT;
typedef struct _SECTION_OBJECT_POINTERS;
typedef struct _SECURITY_CLIENT_CONTEXT;
typedef struct _SECURITY_DESCRIPTOR;
typedef struct _SECURITY_DESCRIPTOR_RELATIVE;
typedef struct _SECURITY_QUALITY_OF_SERVICE;
typedef struct _SECURITY_SUBJECT_CONTEXT;
typedef struct _SEGMENT;
typedef struct _SEGMENT_FLAGS;
typedef struct _SEGMENT_OBJECT;
typedef struct _SEP_AUDIT_POLICY;
typedef struct _SEP_LOGON_SESSION_REFERENCES;
typedef struct _SEP_TOKEN_PRIVILEGES;
typedef struct _SHARED_CACHE_MAP;
typedef struct _SHARED_CACHE_MAP_LIST_CURSOR;
typedef struct _SID;
typedef struct _SID_AND_ATTRIBUTES;
typedef struct _SID_AND_ATTRIBUTES_HASH;
typedef struct _SID_IDENTIFIER_AUTHORITY;
typedef struct _SINGLE_LIST_ENTRY;
typedef struct _SINGLE_LIST_ENTRY32;
typedef struct _SLIST_ENTRY;
typedef union _SLIST_HEADER;
typedef struct _SMBIOS_TABLE_HEADER;
typedef struct _STACK_TABLE;
typedef struct _STRING;
typedef struct _STRING32;
typedef struct _STRING64;
typedef struct _SUBSECTION;
typedef struct _SYSPTES_HEADER;
typedef struct _SYSTEM_POWER_CAPABILITIES;
typedef struct _SYSTEM_POWER_LEVEL;
typedef struct _SYSTEM_POWER_POLICY;
typedef struct _SYSTEM_POWER_STATE_CONTEXT;
typedef struct _SYSTEM_TRACE_HEADER;
typedef struct _TEB;
typedef struct _TEB_ACTIVE_FRAME;
typedef struct _TEB_ACTIVE_FRAME_CONTEXT;
typedef struct _TEB32;
typedef struct _TEB64;
typedef struct _TERMINATION_PORT;
typedef struct _THERMAL_INFORMATION;
typedef struct _THERMAL_INFORMATION_EX;
typedef struct _THREAD_PERFORMANCE_DATA;
typedef struct _TIME_FIELDS;
typedef struct _TOKEN;
typedef struct _TOKEN_AUDIT_POLICY;
typedef struct _TOKEN_CONTROL;
typedef struct _TOKEN_SOURCE;
typedef struct _TP_CALLBACK_ENVIRON_V3;
typedef struct _TP_CALLBACK_INSTANCE;
typedef struct _TP_CLEANUP_GROUP;
typedef struct _TP_DIRECT;
typedef struct _TP_NBQ_GUARD;
typedef struct _TP_POOL;
typedef struct _TP_TASK;
typedef struct _TP_TASK_CALLBACKS;
typedef struct _TPM_BOOT_ENTROPY_LDR_RESULT;
typedef struct _TRACE_ENABLE_CONTEXT;
typedef struct _TRACE_ENABLE_CONTEXT_EX;
typedef struct _TRACE_ENABLE_INFO;
typedef struct _TXN_PARAMETER_BLOCK;
typedef union _ULARGE_INTEGER;
typedef struct _UMS_CONTROL_BLOCK;
typedef struct _UNEXPECTED_INTERRUPT;
typedef struct _UNICODE_STRING;
typedef struct _VACB;
typedef struct _VACB_ARRAY_HEADER;
typedef struct _VACB_LEVEL_ALLOCATION_LIST;
typedef struct _VACB_LEVEL_REFERENCE;
typedef struct _VERIFIER_SHARED_EXPORT_THUNK;
typedef struct _VF_ADDRESS_RANGE;
typedef struct _VF_AVL_TABLE;
typedef struct _VF_AVL_TREE;
typedef struct _VF_AVL_TREE_NODE;
typedef struct _VF_KE_CRITICAL_REGION_TRACE;
typedef struct _VF_POOL_TRACE;
typedef struct _VF_SUSPECT_DRIVER_ENTRY;
typedef struct _VF_TARGET_ALL_SHARED_EXPORT_THUNKS;
typedef struct _VF_TARGET_DRIVER;
typedef struct _VF_TARGET_VERIFIED_DRIVER_DATA;
typedef struct _VF_TRACKER;
typedef struct _VF_TRACKER_STAMP;
typedef struct _VF_WATCHDOG_IRP;
typedef struct _VI_DEADLOCK_GLOBALS;
typedef struct _VI_DEADLOCK_NODE;
typedef struct _VI_DEADLOCK_RESOURCE;
typedef struct _VI_DEADLOCK_THREAD;
typedef struct _VI_FAULT_TRACE;
typedef struct _VI_POOL_ENTRY;
typedef struct _VI_POOL_ENTRY_INUSE;
typedef struct _VI_POOL_PAGE_HEADER;
typedef struct _VI_TRACK_IRQL;
typedef struct _VI_VERIFIER_ISSUE;
typedef struct _VIRTUAL_EFI_RUNTIME_SERVICES;
typedef struct _VOLUME_CACHE_MAP;
typedef struct _VPB;
typedef struct _WAIT_CONTEXT_BLOCK;
typedef union _WHEA_ERROR_PACKET_FLAGS;
typedef struct _WHEA_ERROR_PACKET_V2;
typedef struct _WHEA_ERROR_RECORD;
typedef struct _WHEA_ERROR_RECORD_HEADER;
typedef union _WHEA_ERROR_RECORD_HEADER_FLAGS;
typedef union _WHEA_ERROR_RECORD_HEADER_VALIDBITS;
typedef struct _WHEA_ERROR_RECORD_SECTION_DESCRIPTOR;
typedef union _WHEA_ERROR_RECORD_SECTION_DESCRIPTOR_FLAGS;
typedef union _WHEA_ERROR_RECORD_SECTION_DESCRIPTOR_VALIDBITS;
typedef union _WHEA_ERROR_STATUS;
typedef struct _WHEA_MEMORY_ERROR_SECTION;
typedef union _WHEA_MEMORY_ERROR_SECTION_VALIDBITS;
typedef union _WHEA_PERSISTENCE_INFO;
typedef union _WHEA_REVISION;
typedef union _WHEA_TIMESTAMP;
typedef struct _WMI_BUFFER_HEADER;
typedef struct _WMI_LOGGER_CONTEXT;
typedef struct _WMI_TRACE_PACKET;
typedef struct _WORK_QUEUE_ENTRY;
typedef struct _WORK_QUEUE_ITEM;
typedef struct _X86_DBGKD_CONTROL_SET;
typedef struct _XSAVE_AREA;
typedef struct _XSAVE_AREA_HEADER;
typedef struct _XSAVE_FORMAT;
typedef struct _XSTATE_CONFIGURATION;
typedef struct _XSTATE_CONTEXT;
typedef struct _XSTATE_FEATURE;
typedef struct _XSTATE_SAVE;