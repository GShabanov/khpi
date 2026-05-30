/*
*   File: 
*       NtInternal.h
*
*   Abstract:
*
*   Author: 
*
*   Revision History:
*/
// $Revision: 132 $

#ifndef __NTINTERNAL_H__
#define __NTINTERNAL_H__

#define InitializeListHead(ListHead) (\
    (ListHead)->Flink = (ListHead)->Blink = (ListHead))

#define InsertTailList(ListHead,Entry) {\
    PLIST_ENTRY _EX_Blink;\
    PLIST_ENTRY _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Blink = _EX_ListHead->Blink;\
    (Entry)->Flink = _EX_ListHead;\
    (Entry)->Blink = _EX_Blink;\
    _EX_Blink->Flink = (Entry);\
    _EX_ListHead->Blink = (Entry);\
    }

#define InsertHeadList(ListHead,Entry) {\
    PLIST_ENTRY _EX_Flink;\
    PLIST_ENTRY _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Flink = _EX_ListHead->Flink;\
    (Entry)->Flink = _EX_Flink;\
    (Entry)->Blink = _EX_ListHead;\
    _EX_Flink->Blink = (Entry);\
    _EX_ListHead->Flink = (Entry);\
    }

#define RemoveEntryList(Entry) {\
    PLIST_ENTRY _EX_Blink;\
    PLIST_ENTRY _EX_Flink;\
    _EX_Flink = (Entry)->Flink;\
    _EX_Blink = (Entry)->Blink;\
    _EX_Blink->Flink = _EX_Flink;\
    _EX_Flink->Blink = _EX_Blink;\
    }

#define RemoveTailList(ListHead) \
    (ListHead)->Blink;\
    {RemoveEntryList((ListHead)->Blink)}

#define RemoveHeadList(ListHead) \
    (ListHead)->Flink;\
    {RemoveEntryList((ListHead)->Flink)}

#define IsListEmpty(ListHead) \
    ((ListHead)->Flink == (ListHead))

//
//
//  PSINGLE_LIST_ENTRY
//  PopEntryList(
//      PSINGLE_LIST_ENTRY ListHead
//      );
//
#ifndef PopEntryList
#define PopEntryList(ListHead) \
    (ListHead)->Next;\
    {\
        PSINGLE_LIST_ENTRY FirstEntry;\
        FirstEntry = (ListHead)->Next;\
        if (FirstEntry != NULL) {     \
            (ListHead)->Next = FirstEntry->Next;\
        }                             \
    }
#endif
//
//  VOID
//  PushEntryList(
//      PSINGLE_LIST_ENTRY ListHead,
//      PSINGLE_LIST_ENTRY Entry
//      );
//
#ifndef PushEntryList
#define PushEntryList(ListHead,Entry) \
    (Entry)->Next = (ListHead)->Next; \
    (ListHead)->Next = (Entry)
#endif

//
// Calculate the address of the base of the structure given its type, and an
// address of a field within the structure.
//
#ifndef CONTAINING_RECORD
#define CONTAINING_RECORD(address, type, field) \
    ((type *)((PCHAR)(address) - (ULONG_PTR)(&((type *)0)->field)))
#endif // CONTAINING_RECORD

#ifndef FIELD_OFFSET
#define FIELD_OFFSET(type, field)    ((LONG)(LONG_PTR)&(((type *)0)->field))
#endif // FIELD_OFFSET

typedef LONG NTSTATUS;

// success status
#define STATUS_SUCCESS                   ((NTSTATUS)0x00000000L) // ntsubauth
#define STATUS_WAIT_1                    ((NTSTATUS)0x00000001L)
#define STATUS_WAIT_2                    ((NTSTATUS)0x00000002L)

#define STATUS_MORE_ENTRIES              ((NTSTATUS)0x00000105L)
#define STATUS_NOT_ALL_ASSIGNED          ((NTSTATUS)0x00000106L)

// warning status
#define STATUS_BUFFER_OVERFLOW           ((NTSTATUS)0x80000005L)
#define STATUS_NO_MORE_ENTRIES           ((NTSTATUS)0x8000001AL)
// error status
#define STATUS_UNSUCCESSFUL              ((NTSTATUS)0xC0000001L)
#define STATUS_NOT_IMPLEMENTED           ((NTSTATUS)0xC0000002L)
#define STATUS_INVALID_INFO_CLASS        ((NTSTATUS)0xC0000003L)    // ntsubauth
#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)
#define STATUS_NO_SUCH_DEVICE            ((NTSTATUS)0xC000000EL)
#define STATUS_ACCESS_DENIED             ((NTSTATUS)0xC0000022L)
#define STATUS_BUFFER_TOO_SMALL          ((NTSTATUS)0xC0000023L)
#define STATUS_INVALID_SID               ((NTSTATUS)0xC0000078L)
#define STATUS_INSUFFICIENT_RESOURCES    ((NTSTATUS)0xC000009AL)     // ntsubauth
#define STATUS_IMAGE_ALREADY_LOADED      ((NTSTATUS)0xC000010EL)
//#define STATUS_DLL_NOT_FOUND             ((NTSTATUS)0xC0000135L)
#define STATUS_DRIVER_UNABLE_TO_LOAD     ((NTSTATUS)0xC000026CL)


#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

#define PREPARE_UNICODE_STRING(string)                                  \
    (string)->Buffer = reinterpret_cast<PWSTR>(                         \
        reinterpret_cast<UCHAR *>(string) + sizeof(UNICODE_STRING));    \

#define _InitUnicodeString(destination, source)                                 \
        (destination)->Length = wcslen(source) * sizeof(WCHAR);                 \
        (destination)->MaximumLength = (destination)->Length + sizeof(WCHAR);   \
        (destination)->Buffer = source;


typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _STRING {
    USHORT  Length;
    USHORT  MaximumLength;
    PCHAR   Buffer;
} STRING;

#if (_WIN32_WINNT < 0x0501)

typedef enum _PLUGPLAY_CONTROL_CLASS {
    PlugPlayControlEnumerateDevice,
    PlugPlayControlRegisterNewDevice,
    PlugPlayControlDeregisterDevice,
    PlugPlayControlInitializeDevice,
    PlugPlayControlStartDevice,
    PlugPlayControlEjectDevice,
    PlugPlayControlUnlockDevice,
    PlugPlayControlQueryAndRemoveDevice,
    PlugPlayControlUserResponse,
    PlugPlayControlGenerateLegacyDevice,
    PlugPlayControlDetectResourceConflict,
    PlugPlayControlGetInterfaceDeviceList,
    PlugPlayControlProperty,
    PlugPlayControlDeviceClassAssociation,
    PlugPlayControlGetRelatedDevice,
    PlugPlayControlGetInterfaceDeviceAlias,
    PlugPlayControlDeviceStatus,
    PlugPlayControlGetDeviceDepth,
    PlugPlayControlQueryDeviceRelations,
    PlugPlayControlTargetDeviceRelation,
    PlugPlayControlQueryInstallList,
    PlugPlayControlRequestEject,
    PlugPlayControlQueryConflictList,
    MaxPlugPlayControl
} PLUGPLAY_CONTROL_CLASS, *PPLUGPLAY_CONTROL_CLASS;


#else //#if _WIN32_WINNT>=0x0501

typedef enum _PLUGPLAY_CONTROL_CLASS {
    PlugPlayControlEnumerateDevice,
    PlugPlayControlRegisterNewDevice,
    PlugPlayControlDeregisterDevice,
    PlugPlayControlInitializeDevice,
    PlugPlayControlStartDevice,
    PlugPlayControlUnlockDevice,
    PlugPlayControlQueryAndRemoveDevice,
    PlugPlayControlUserResponse,
    PlugPlayControlGenerateLegacyDevice,
    PlugPlayControlGetInterfaceDeviceList,
    PlugPlayControlProperty,
    PlugPlayControlDeviceClassAssociation,
    PlugPlayControlGetRelatedDevice,
    PlugPlayControlGetInterfaceDeviceAlias,
    PlugPlayControlDeviceStatus,
    PlugPlayControlGetDeviceDepth,
    PlugPlayControlQueryDeviceRelations,
    PlugPlayControlTargetDeviceRelation,
    PlugPlayControlQueryConflictList,
    PlugPlayControlRetrieveDock,
    PlugPlayControlResetDevice,
    PlugPlayControlHaltDevice,
    PlugPlayControlGetBlockedDriverList,
    MaxPlugPlayControl,
}PLUGPLAY_CONTROL_CLASS, *PPLUGPLAY_CONTROL_CLASS;
#endif //#if _WIN32_WINNT>=0x0501

typedef enum _PROCESSINFOCLASS {
    ProcessBasicInformation,
    ProcessQuotaLimits,
    ProcessIoCounters,
    ProcessVmCounters,
    ProcessTimes,
    ProcessBasePriority,
    ProcessRaisePriority,
    ProcessDebugPort,
    ProcessExceptionPort,
    ProcessAccessToken,
    ProcessLdtInformation,
    ProcessLdtSize,
    ProcessDefaultHardErrorMode,
    ProcessIoPortHandlers,          // Note: this is kernel mode only
    ProcessPooledUsageAndLimits,
    ProcessWorkingSetWatch,
    ProcessUserModeIOPL,
    ProcessEnableAlignmentFaultFixup,
    ProcessPriorityClass,
    ProcessWx86Information,
    ProcessHandleCount,
    ProcessAffinityMask,
    ProcessPriorityBoost,
    ProcessDeviceMap,
    ProcessSessionInformation,
    ProcessForegroundInformation,
    ProcessWow64Information,
    ProcessImageFileName,
    ProcessLUIDDeviceMapsEnabled,
    ProcessBreakOnTermination,
    ProcessDebugObjectHandle,
    ProcessDebugFlags,
    ProcessHandleTracing,
    ProcessIoPriority,
    ProcessExecuteFlags,
    ProcessTlsInformation,
    ProcessCookie,
    ProcessImageInformation,
    ProcessCycleTime,
    ProcessPagePriority,
    ProcessInstrumentationCallback,
    ProcessThreadStackAllocation,
    ProcessWorkingSetWatchEx,
    ProcessImageFileNameWin32,
    ProcessImageFileMapping,
    MaxProcessInfoClass             // MaxProcessInfoClass should always be the last enum
} PROCESSINFOCLASS;

typedef enum _THREAD_INFORMATION_CLASS_X {
    ThreadBasicInformation, 
    ThreadTimes, 
    ThreadPriority, 
    ThreadBasePriority, 
    ThreadAffinityMask, 
    ThreadImpersonationToken, 
    ThreadDescriptorTableEntry, 
    ThreadEnableAlignmentFaultFixup, 
    ThreadEventPair, 
    ThreadQuerySetWin32StartAddress, 
    ThreadZeroTlsCell, 
    ThreadPerformanceCount, 
    ThreadAmILastThread, 
    ThreadIdealProcessor, 
    ThreadPriorityBoost, 
    ThreadSetTlsArrayAddress, 
    ThreadIsIoPending, 
    ThreadHideFromDebugger,
} THREAD_INFORMATION_CLASS_X;

//
// Process Device Map information
//  NtQueryInformationProcess using ProcessDeviceMap
//  NtSetInformationProcess using ProcessDeviceMap
//

typedef struct _PROCESS_DEVICEMAP_INFORMATION {
    union {
        struct {
            HANDLE DirectoryHandle;
        } Set;
        struct {
            ULONG DriveMap;
            UCHAR DriveType[ 32 ];
        } Query;
    };
} PROCESS_DEVICEMAP_INFORMATION, *PPROCESS_DEVICEMAP_INFORMATION;

typedef struct _PROCESS_DEVICEMAP_INFORMATION_EX {
    union {
        struct {
            HANDLE DirectoryHandle;
        } Set;
        struct {
            ULONG DriveMap;
            UCHAR DriveType[ 32 ];
        } Query;
    };
    ULONG Flags;    // specifies that the query type
} PROCESS_DEVICEMAP_INFORMATION_EX, *PPROCESS_DEVICEMAP_INFORMATION_EX;

#define DOSDEVICE_DRIVE_UNKNOWN     0
#define DOSDEVICE_DRIVE_CALCULATE   1
#define DOSDEVICE_DRIVE_REMOVABLE   2
#define DOSDEVICE_DRIVE_FIXED       3
#define DOSDEVICE_DRIVE_REMOTE      4
#define DOSDEVICE_DRIVE_CDROM       5
#define DOSDEVICE_DRIVE_RAMDISK     6


#define DUPLICATE_SAME_ATTRIBUTES   0x00000004
#define OBJ_AUDIT_OBJECT_CLOSE 0x00000004L

typedef enum _OBJECT_INFORMATION_CLASS {
    ObjectBasicInformation,         // OBJECT_BASIC_INFORMATION
    ObjectNameInformation,          // OBJECT_NAME_INFORMATION
    ObjectTypeInformation,          // OBJECT_TYPE_INFORMATION
    ObjectAllInformation,           // OBJECT_ALL_INFORMATION
    ObjectHandleFlagInformation,    // OBJECT_HANDLE_FLAG_INFORMATION
} OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

typedef struct _OBJECT_HANDLE_FLAG_INFORMATION {
    BOOLEAN Inherit;
    BOOLEAN ProtectFromClose;
} OBJECT_HANDLE_FLAG_INFORMATION, *POBJECT_HANDLE_FLAG_INFORMATION;

typedef struct _OBJECT_BASIC_INFORMATION {
    ULONG Attributes; 
    ACCESS_MASK DesiredAccess; 
    ULONG HandleCount; 
    ULONG ReferenceCount; 
    ULONG PagedPoolQuota; 
    ULONG NonPagedPoolQuota; 
    ULONG HandleTableAddress;
    ULONG ObjectAddress;
    ULONG ObjectHeaderAddress;
    ULONG NameInformationLength;
    ULONG TypeInformationLength;
    ULONG SecurityDescriptorLength;
    LARGE_INTEGER CreateTime;
} OBJECT_BASIC_INFORMATION, *POBJECT_BASIC_INFORMATION;

#pragma warning(push)
#pragma warning(disable : 4200)
typedef struct _OBJECT_NAME_INFORMATION {
  UNICODE_STRING          Name;
  WCHAR                   NameBuffer[];
} OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;
#pragma warning(pop)

typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef struct _OBJECT_TYPE_INFORMATION {
    UNICODE_STRING TypeName; 
    ULONG TotalNumberOfHandles; 
    ULONG TotalNumberOfObjects; 
    WCHAR Unused1[8]; 
    ULONG HighWaterNumberOfHandles; 
    ULONG HighWaterNumberOfObjects; 
    WCHAR Unused2[8]; 
    ACCESS_MASK InvalidAttributes; 
    GENERIC_MAPPING GenericMapping; 
    ACCESS_MASK ValidAttributes; 
    BOOLEAN SecurityRequired; 
    BOOLEAN MaintainHandleCount; 
    USHORT  MaintainTypeList; 
    int     PoolType; 
    ULONG   DefaultPagedPoolCharge; 
    ULONG DefaultNonPagedPoolCharge;
} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

typedef struct _OBJECT_DIRECTORY_INFORMATION {
    UNICODE_STRING Name;
    UNICODE_STRING TypeName;
} OBJECT_DIRECTORY_INFORMATION, *POBJECT_DIRECTORY_INFORMATION;

#ifndef InitializeObjectAttributes
#define InitializeObjectAttributes(p, n, a, r, s)   { \
    (p)->Length = sizeof(OBJECT_ATTRIBUTES);          \
    (p)->RootDirectory = r;                           \
    (p)->Attributes = a;                              \
    (p)->ObjectName = n;                              \
    (p)->SecurityDescriptor = s;                      \
    (p)->SecurityQualityOfService = NULL; }
#endif // InitializeObjectAttributes

//
// Object Manager Object Type Specific Access Rights.
//

#define OBJECT_TYPE_CREATE (0x0001)

#define OBJECT_TYPE_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0x1)

//
// Object Manager Directory Specific Access Rights.
//

#define DIRECTORY_QUERY                 (0x0001)
#define DIRECTORY_TRAVERSE              (0x0002)
#define DIRECTORY_CREATE_OBJECT         (0x0004)
#define DIRECTORY_CREATE_SUBDIRECTORY   (0x0008)

#define DIRECTORY_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0xF)

#define SYMBOLIC_LINK_QUERY (0x0001)

#define SYMBOLIC_LINK_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0x1)

#define DUPLICATE_CLOSE_SOURCE      0x00000001  
#define DUPLICATE_SAME_ACCESS       0x00000002  
#define DUPLICATE_SAME_ATTRIBUTES   0x00000004

//
// Valid values for the Attributes field
//

#define OBJ_INHERIT             0x00000002L
#define OBJ_PERMANENT           0x00000010L
#define OBJ_EXCLUSIVE           0x00000020L
#define OBJ_CASE_INSENSITIVE    0x00000040L
#define OBJ_OPENIF              0x00000080L
#define OBJ_OPENLINK            0x00000100L
#define OBJ_KERNEL_HANDLE       0x00000200L
#define OBJ_VALID_ATTRIBUTES    0x000003F2L

typedef enum _SYSTEM_INFORMATION_CLASS {    /* Class */ /*Query*/ /*Set*/
    SystemBasicInformation,                     // 0        Y        N
    SystemProcessorInformation,                 // 1        Y        N
    SystemPerformanceInformation,               // 2        Y        N
    SystemTimeOfDayInformation,                 // 3        Y        N
    SystemNotImplemented1,                      // 4        Y        N
    SystemProcessesAndThreadsInformation,       // 5        Y        N
    SystemCallCounts,                           // 6        Y        N
    SystemConfigurationInformation,             // 7        Y        N
    SystemProcessorTimes,                       // 8        Y        N
    SystemGlobalFlag,                           // 9        Y        Y
    SystemNotImplemented2,                      // 10       Y        N
    SystemModuleInformation,                    // 11       Y        N
    SystemLockInformation,                      // 12       Y        N
    SystemNotImplemented3,                      // 13       Y        N
    SystemNotImplemented4,                      // 14       Y        N
    SystemNotImplemented5,                      // 15       Y        N
    SystemHandleInformation,                    // 16       Y        N
    SystemObjectInformation,                    // 17       Y        N
    SystemPagefileInformation,                  // 18       Y        N
    SystemInstructionEmulationCounts,           // 19       Y        N
    SystemInvalidInfoClass1,                    // 20
    SystemCacheInformation,                     // 21       Y        Y
    SystemPoolTagInformation,                   // 22       Y        N
    SystemProcessorStatistics,                  // 23       Y        N
    SystemDpcInformation,                       // 24       Y        Y
    SystemNotImplemented6,                      // 25       Y        N
    SystemLoadImage,                            // 26       N        Y
    SystemUnloadImage,                          // 27       N        Y
    SystemTimeAdjustment,                       // 28       Y        Y
    SystemNotImplemented7,                      // 29       Y        N
    SystemNotImplemented8,                      // 30       Y        N
    SystemNotImplemented9,                      // 31       Y        N
    SystemCrashDumpInformation,                 // 32       Y        N
    SystemExceptionInformation,                 // 33       Y        N
    SystemCrashDumpStateInformation,            // 34       Y       Y/N
    SystemKernelDebuggerInformation,            // 35       Y        N
    SystemContextSwitchInformation,             // 36       Y        N
    SystemRegistryQuotaInformation,             // 37       Y        Y
    SystemLoadAndCallImage,                     // 38       N        Y
    SystemPrioritySeparation,                   // 39       N        Y
    SystemNotImplemented10,                     // 40       Y        N
    SystemNotImplemented11,                     // 41       Y        N
    SystemInvalidInfoClass2,                    // 42
    SystemInvalidInfoClass3,                    // 43
    SystemTimeZoneInformation,                  // 44       Y        N
    SystemLookasideInformation,                 // 45       Y        N
    SystemSetTimeSlipEvent,                     // 46       N        Y
    SystemCreateSession,                        // 47       N        Y
    SystemDeleteSession,                        // 48       N        Y
    SystemInvalidInfoClass4,                    // 49
    SystemRangeStartInformation,                // 50       Y        N
    SystemVerifierInformation,                  // 51       Y        Y
    SystemAddVerifier,                          // 52       N        Y
    SystemSessionProcessesInformation           // 53       Y        N
} SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_BASIC_INFORMATION { // Information Class 0
    ULONG Reserved;
    ULONG TimerResolution;
    ULONG PageSize;
    ULONG NumberOfPhysicalPages;
    ULONG LowestPhysicalPageNumber;
    ULONG HighestPhysicalPageNumber;
    ULONG AllocationGranularity;
    ULONG MinimumUserModeAddress;
    ULONG MaximumUserModeAddress;
    KAFFINITY ActiveProcessorsAffinityMask;
    CCHAR NumberOfProcessors;
} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;

typedef struct _SYSTEM_PROCESSOR_INFORMATION { // Information Class 1
    USHORT ProcessorArchitecture;
    USHORT ProcessorLevel;
    USHORT ProcessorRevision;
    USHORT Reserved;
    ULONG FeatureBits;
} SYSTEM_PROCESSOR_INFORMATION, *PSYSTEM_PROCESSOR_INFORMATION;

typedef struct _SYSTEM_PROCESS_INFORMATION {
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    LARGE_INTEGER SpareLi1;
    LARGE_INTEGER SpareLi2;
    LARGE_INTEGER SpareLi3;
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ImageName;
    ULONG BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
    ULONG HandleCount;
    ULONG SpareUl2;
    ULONG SpareUl3;
    ULONG PeakVirtualSize;
    ULONG VirtualSize;
    ULONG PageFaultCount;
    ULONG PeakWorkingSetSize;
    ULONG WorkingSetSize;
    ULONG QuotaPeakPagedPoolUsage;
    ULONG QuotaPagedPoolUsage;
    ULONG QuotaPeakNonPagedPoolUsage;
    ULONG QuotaNonPagedPoolUsage;
    ULONG PagefileUsage;
    ULONG PeakPagefileUsage;
    ULONG PrivatePageCount;
    LARGE_INTEGER ReadOperationCount;
    LARGE_INTEGER WriteOperationCount;
    LARGE_INTEGER OtherOperationCount;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;


//
// Define the create disposition values
//

#define FILE_SUPERSEDE                  0x00000000
#define FILE_OPEN                       0x00000001
#define FILE_CREATE                     0x00000002
#define FILE_OPEN_IF                    0x00000003
#define FILE_OVERWRITE                  0x00000004
#define FILE_OVERWRITE_IF               0x00000005
#define FILE_MAXIMUM_DISPOSITION        0x00000005

//
// Define the create/open option flags
//

#define FILE_DIRECTORY_FILE                     0x00000001
#define FILE_WRITE_THROUGH                      0x00000002
#define FILE_SEQUENTIAL_ONLY                    0x00000004
#define FILE_NO_INTERMEDIATE_BUFFERING          0x00000008

#define FILE_SYNCHRONOUS_IO_ALERT               0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT            0x00000020
#define FILE_NON_DIRECTORY_FILE                 0x00000040
#define FILE_CREATE_TREE_CONNECTION             0x00000080

#define FILE_COMPLETE_IF_OPLOCKED               0x00000100
#define FILE_NO_EA_KNOWLEDGE                    0x00000200
#define FILE_OPEN_FOR_RECOVERY                  0x00000400
#define FILE_RANDOM_ACCESS                      0x00000800

#define FILE_DELETE_ON_CLOSE                    0x00001000
#define FILE_OPEN_BY_FILE_ID                    0x00002000
#define FILE_OPEN_FOR_BACKUP_INTENT             0x00004000
#define FILE_NO_COMPRESSION                     0x00008000

#define FILE_RESERVE_OPFILTER                   0x00100000
#define FILE_OPEN_REPARSE_POINT                 0x00200000
#define FILE_OPEN_NO_RECALL                     0x00400000
#define FILE_OPEN_FOR_FREE_SPACE_QUERY          0x00800000

#define FILE_COPY_STRUCTURED_STORAGE            0x00000041
#define FILE_STRUCTURED_STORAGE                 0x00000441

#define FILE_VALID_OPTION_FLAGS                 0x00ffffff
#define FILE_VALID_PIPE_OPTION_FLAGS            0x00000032
#define FILE_VALID_MAILSLOT_OPTION_FLAGS        0x00000032
#define FILE_VALID_SET_FLAGS                    0x00000036

#define FILE_ATTRIBUTE_READONLY             0x00000001  
#define FILE_ATTRIBUTE_HIDDEN               0x00000002  
#define FILE_ATTRIBUTE_SYSTEM               0x00000004  
//OLD DOS VOLID                             0x00000008

#define FILE_ATTRIBUTE_DIRECTORY            0x00000010  
#define FILE_ATTRIBUTE_ARCHIVE              0x00000020  
#define FILE_ATTRIBUTE_DEVICE               0x00000040  
#define FILE_ATTRIBUTE_NORMAL               0x00000080  

#define FILE_ATTRIBUTE_TEMPORARY            0x00000100  
#define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200  
#define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400  
#define FILE_ATTRIBUTE_COMPRESSED           0x00000800  

#define FILE_ATTRIBUTE_OFFLINE              0x00001000  
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000  
#define FILE_ATTRIBUTE_ENCRYPTED            0x00004000  

#define FILE_ATTRIBUTE_VIRTUAL              0x00010000  

#define FILE_ATTRIBUTE_VALID_FLAGS          0x00007fb7
#define FILE_ATTRIBUTE_VALID_SET_FLAGS      0x000031a7

//
// Define the I/O status information return values for NtCreateFile/NtOpenFile
//

#define FILE_SUPERSEDED                 0x00000000
#define FILE_OPENED                     0x00000001
#define FILE_CREATED                    0x00000002
#define FILE_OVERWRITTEN                0x00000003
#define FILE_EXISTS                     0x00000004
#define FILE_DOES_NOT_EXIST             0x00000005

typedef enum _FILE_INFORMATION_CLASS_FULL {
// end_wdm
    FileDirectoryInformation         = 1,
    FileFullDirectoryInformation,   // 2
    FileBothDirectoryInformation,   // 3
    FileBasicInformation,           // 4  wdm
    FileStandardInformation,        // 5  wdm
    FileInternalInformation,        // 6
    FileEaInformation,              // 7
    FileAccessInformation,          // 8
    FileNameInformation,            // 9
    FileRenameInformation,          // 10
    FileLinkInformation,            // 11
    FileNamesInformation,           // 12
    FileDispositionInformation,     // 13
    FilePositionInformation,        // 14 wdm
    FileFullEaInformation,          // 15
    FileModeInformation,            // 16
    FileAlignmentInformation,       // 17
    FileAllInformation,             // 18
    FileAllocationInformation,      // 19
    FileEndOfFileInformation,       // 20 wdm
    FileAlternateNameInformation,   // 21
    FileStreamInformation,          // 22
    FilePipeInformation,            // 23
    FilePipeLocalInformation,       // 24
    FilePipeRemoteInformation,      // 25
    FileMailslotQueryInformation,   // 26
    FileMailslotSetInformation,     // 27
    FileCompressionInformation,     // 28
    FileObjectIdInformation,        // 29
    FileCompletionInformation,      // 30
    FileMoveClusterInformation,     // 31
    FileQuotaInformation,           // 32
    FileReparsePointInformation,    // 33
    FileNetworkOpenInformation,     // 34
    FileAttributeTagInformation,    // 35
    FileTrackingInformation,        // 36
    FileIdBothDirectoryInformation, // 37
    FileIdFullDirectoryInformation, // 38
    FileValidDataLengthInformation, // 39
    FileShortNameInformation,       // 40
    FileMaximumInformation
// begin_wdm
} FILE_INFORMATION_CLASS_FULL, *PFILE_INFORMATION_CLASS_FULL;

typedef struct _FILE_BASIC_INFORMATION {                    // ntddk wdm nthal
    LARGE_INTEGER CreationTime;                             // ntddk wdm nthal
    LARGE_INTEGER LastAccessTime;                           // ntddk wdm nthal
    LARGE_INTEGER LastWriteTime;                            // ntddk wdm nthal
    LARGE_INTEGER ChangeTime;                               // ntddk wdm nthal
    ULONG FileAttributes;                                   // ntddk wdm nthal
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;         // ntddk wdm nthal

typedef struct _FILE_NAMES_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_NAMES_INFORMATION, *PFILE_NAMES_INFORMATION;

typedef struct _FILE_NAME_INFORMATION {                     // ntddk
    ULONG FileNameLength;                                   // ntddk
    WCHAR FileName[1];                                      // ntddk
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;           // ntddk

typedef struct _FILE_DISPOSITION_INFORMATION {                  // ntddk nthal
    BOOLEAN DeleteFile;                                         // ntddk nthal
} FILE_DISPOSITION_INFORMATION, *PFILE_DISPOSITION_INFORMATION; // ntddk nthal

typedef struct _FILE_STANDARD_INFORMATION {                 // ntddk wdm nthal
    LARGE_INTEGER AllocationSize;                           // ntddk wdm nthal
    LARGE_INTEGER EndOfFile;                                // ntddk wdm nthal
    ULONG NumberOfLinks;                                    // ntddk wdm nthal
    BOOLEAN DeletePending;                                  // ntddk wdm nthal
    BOOLEAN Directory;                                      // ntddk wdm nthal
} FILE_STANDARD_INFORMATION, *PFILE_STANDARD_INFORMATION;   // ntddk wdm nthal

typedef struct _FILE_DIRECTORY_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_DIRECTORY_INFORMATION, *PFILE_DIRECTORY_INFORMATION;

typedef struct _FILE_ACCESS_INFORMATION {
    ACCESS_MASK AccessFlags;
} FILE_ACCESS_INFORMATION, *PFILE_ACCESS_INFORMATION;

typedef struct _FILE_PIPE_INFORMATION {
     ULONG ReadMode;
     ULONG CompletionMode;
} FILE_PIPE_INFORMATION, *PFILE_PIPE_INFORMATION;

typedef struct _FILE_FULL_EA_INFORMATION {
    ULONG NextEntryOffset;
    UCHAR Flags;
    UCHAR EaNameLength;
    USHORT EaValueLength;
    CHAR EaName[1];
} FILE_FULL_EA_INFORMATION, *PFILE_FULL_EA_INFORMATION;

typedef struct _FILE_PIPE_LOCAL_INFORMATION {
     ULONG NamedPipeType;
     ULONG NamedPipeConfiguration;
     ULONG MaximumInstances;
     ULONG CurrentInstances;
     ULONG InboundQuota;
     ULONG ReadDataAvailable;
     ULONG OutboundQuota;
     ULONG WriteQuotaAvailable;
     ULONG NamedPipeState;
     ULONG NamedPipeEnd;
} FILE_PIPE_LOCAL_INFORMATION, *PFILE_PIPE_LOCAL_INFORMATION;

typedef struct _FILE_PIPE_REMOTE_INFORMATION {
     LARGE_INTEGER CollectDataTime;
     ULONG MaximumCollectionCount;
} FILE_PIPE_REMOTE_INFORMATION, *PFILE_PIPE_REMOTE_INFORMATION;

typedef enum _KEY_INFORMATION_CLASS {
  KeyBasicInformation,
  KeyNodeInformation,
  KeyFullInformation
} KEY_INFORMATION_CLASS;

typedef struct _KEY_BASIC_INFORMATION {
  LARGE_INTEGER LastWriteTime;
  ULONG  TitleIndex;
  ULONG  NameLength;
  WCHAR  Name[1];  //  Variable-length string
} KEY_BASIC_INFORMATION, *PKEY_BASIC_INFORMATION;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };

    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

//
// Define an Asynchronous Procedure Call from I/O viewpoint
//

typedef
VOID
(NTAPI *PIO_APC_ROUTINE) (
    IN PVOID ApcContext,
    IN PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG Reserved
    );

typedef
VOID
(*PPS_APC_ROUTINE) (
    IN PVOID NormalContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
    );


typedef struct _LDR_DATA_TABLE_ENTRY {

  LIST_ENTRY        InLoadOrderLinks;
  LIST_ENTRY        InMemoryOrderLinks;
  LIST_ENTRY        InInitializationOrderLinks;
  PVOID             DllBase;
  PVOID             EntryPoint;
  ULONG             SizeOfImage;
  UNICODE_STRING    FullDllName;
  UNICODE_STRING    BaseDllName;
  ULONG             Flags;
  USHORT            LoadCount;
  USHORT            TlsIndex;
  LIST_ENTRY        HashLinks;
  PVOID             SectionPointer;
  ULONG             CheckSum;
  ULONG             TimeDateStamp;
  PVOID             LoadedImports;
  PVOID             EntryPointActivationContext;
  PVOID             PatchInformation;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB_LDR_DATA {

  ULONG Length;
  UCHAR Initialized;
  PVOID SsHandle;
  LIST_ENTRY InLoadOrderModuleList;
  LIST_ENTRY InMemoryOrderModuleList;
  LIST_ENTRY InInitializationOrderModuleList;
  PVOID EntryInProgress;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _CLIENT_ID {

  DWORD UniqueProcess;
  DWORD UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _CURDIR {
  UNICODE_STRING    DosPath;
  HANDLE            Handle;
} CURDIR, *PCURDIR;

typedef struct _RTL_DRIVE_LETTER_CURDIR {
    USHORT Flags;
    USHORT Length;
    ULONG  TimeStamp;
    STRING DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;


typedef struct _RTL_USER_PROCESS_PARAMETERS {
    ULONG           MaximumLength;
    ULONG           Length;
    ULONG           Flags;
    ULONG           DebugFlags;
    HANDLE          ConsoleHandle;
    ULONG           ConsoleFlags;
    HANDLE          StandardInput;
    HANDLE          StandardOutput;
    HANDLE          StandardError;
    CURDIR          CurrentDirectory;
    UNICODE_STRING  DllPath;
    UNICODE_STRING  ImagePathName;
    UNICODE_STRING  CommandLine;
    PVOID           Environment;
    ULONG           StartingX;
    ULONG           StartingY;
    ULONG           CountX;
    ULONG           CountY;
    ULONG           CountCharsX;
    ULONG           CountCharsY;
    ULONG           FillAttribute;
    ULONG           WindowFlags;
    ULONG           ShowWindowFlags;
    UNICODE_STRING  WindowTitle;
    UNICODE_STRING  DesktopInfo;
    UNICODE_STRING  ShellInfo;
    UNICODE_STRING  RuntimeData;
    RTL_DRIVE_LETTER_CURDIR CurrentDirectores[32];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB {
    UCHAR                         InheritedAddressSpace;
    UCHAR                         ReadImageFileExecOptions;
    UCHAR                         BeingDebugged;
    UCHAR                         SpareBool;
    PVOID                         Mutant;
    PVOID                         ImageBaseAddress;
    PPEB_LDR_DATA                 Ldr;
    RTL_USER_PROCESS_PARAMETERS  *ProcessParameters;
    PVOID                         SubSystemData;
    PVOID                         ProcessHeap;
    struct _RTL_CRITICAL_SECTION* FastPebLock;
    PVOID                         AtlThunkSListPtr;
    PVOID                         IFEOKey;
    ULONG                         CrossProcessFlags;
    PVOID                         UserSharedInfoPtr;
} PEB, *PPEB;

typedef struct _TEB {

  NT_TIB    NtTib;
  PVOID     EnvironmentPointer;
  CLIENT_ID ClientId;
  PVOID     ActiveRpcHandle;
  PVOID     ThreadLocalStoragePointer;
  PPEB      ProcessEnvironmentBlock;
  DWORD     LastErrorValue;
  ULONG     CountOfOwnedCriticalSections;
  PVOID     CsrClientThread;
  PVOID     Win32ThreadInfo;
} TEB, *PTEB;

typedef struct _EXHANDLE 
{
    union 
    {
        struct 
        {
            //  Application available tag bits
            ULONG TagBits : 2;

            //  The handle table entry index
            ULONG Index : 30;
        } a;

        HANDLE GenericHandleOverlay;
    };
} EXHANDLE, *PEXHANDLE;

typedef struct _THREAD_BASIC_INFORMATION {
    NTSTATUS  ExitStatus;
    PVOID     TebBaseAddress;
    CLIENT_ID ClientId;
    ULONG     AffinityMask;
    ULONG     Priority;
    ULONG     BasePriority;
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

typedef struct _PROCESS_BASIC_INFORMATION {
    DWORD     ExitStatus;
    PPEB      PebBaseAddress;
    DWORD     AffinityMask;
    DWORD     BasePriority;
    DWORD     UniqueProcessId;
    DWORD     InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;

typedef struct _OLDINITIAL_TEB {
    PVOID OldStackBase;
    PVOID OldStackLimit;
} OLDINITIAL_TEB, *POLDINITIAL_TEB;

typedef struct _INITIAL_TEB {
    OLDINITIAL_TEB OldInitialTeb;
    PVOID StackBase;
    PVOID StackLimit;
    PVOID StackAllocationBase;
} INITIAL_TEB, *PINITIAL_TEB;

#ifndef IMAGE_NT_HEADER
#define IMAGE_NT_HEADER(_base) \
    reinterpret_cast<PIMAGE_NT_HEADERS32>( \
    reinterpret_cast<PCHAR>(_base) + \
    reinterpret_cast<PIMAGE_DOS_HEADER>(_base)->e_lfanew)
#endif


enum KEY_SET_INFORMATION_CLASS {
    KeyWriteTimeInformation = 0,
};

typedef struct _KEY_WRITE_TIME_INFORMATION {
    LARGE_INTEGER    WriteTime;
} KEY_WRITE_TIME_INFORMATION, *PKEY_WRITE_TIME_INFORMATION;

typedef enum _EVENT_TYPE {
    NotificationEvent,
    SynchronizationEvent
} EVENT_TYPE;

//
// Wait type
//

typedef enum _WAIT_TYPE {
    WaitAll,
    WaitAny
} WAIT_TYPE;

typedef struct _SEMAPHORE_BASIC_INFORMATION {
    LONG CurrentCount;
    LONG MaximumCount;
} SEMAPHORE_BASIC_INFORMATION, *PSEMAPHORE_BASIC_INFORMATION;

#ifndef THREAD_ALERT
#define THREAD_ALERT          (0x0004)
#endif // THREAD_ALERT

typedef enum _DEBUG_CONTROL_CODE {
    DebugGetTraceInformation = 1,
    DebugSetInternalBreakpoint,
    DebugSetSpecialCall,
    DebugClearSpecialCalls,
    DebugQuerySpecialCalls,
    DebugDbgBreakPoint
} DEBUG_CONTROL_CODE;

typedef struct _DBGKD_QUERY_SPECIAL_CALLS { // DebugQuerySpecialCalls
    DWORD NumberOfSpecialCalls;
    DWORD SpecialCalls[ANYSIZE_ARRAY];
} DBGKD_QUERY_SPECIAL_CALLS, *PDBGKD_QUERY_SPECIAL_CALLS;

//
// Define the I/O bus interface types.
//

typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined = -1,
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    ProcessorInternal,
    InternalPowerBus,
    PNPISABus,
    PNPBus,
    MaximumInterfaceType
} INTERFACE_TYPE, *PINTERFACE_TYPE;

//
// Interrupt modes.
//

typedef enum _KINTERRUPT_MODE {
    LevelSensitive,
    Latched
} KINTERRUPT_MODE;

//
// Define the DMA transfer widths.
//

typedef enum _DMA_WIDTH {
    Width8Bits,
    Width16Bits,
    Width32Bits,
    MaximumDmaWidth
}DMA_WIDTH, *PDMA_WIDTH;

//
// Define DMA transfer speeds.
//

typedef enum _DMA_SPEED {
    Compatible,
    TypeA,
    TypeB,
    TypeC,
    TypeF,
    MaximumDmaSpeed
}DMA_SPEED, *PDMA_SPEED;

//
// Cardinal Data Types [0 - 2**N-2)
//

typedef char CCHAR;          // winnt
typedef short CSHORT;

typedef CCHAR *PCCHAR;
typedef CSHORT *PCSHORT;

typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;

//
// Section Information Structures.
//

typedef enum _SECTION_INHERIT {
    ViewShare = 1,
    ViewUnmap = 2
} SECTION_INHERIT;

#define PAGE_SIZE 0x1000

#define SECTION_ALIGN(Va, Align) ((LONG)((LONG_PTR)(Va) + Align - 1) & ~(Align - 1))

#define MAKEPTR(cast, ptr, addValue) (cast)((LONG_PTR)(ptr) + (LONG_PTR)(addValue))

#define ROUND_TO_PAGES(Size)  ((LONG)((LONG_PTR)(Size) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define ROUND_UP(data, To)  \
    (((ULONG_PTR)(data) + (ULONG_PTR)(To) - 1) & ~((ULONG_PTR)(To) - 1))

#define ROUND_DOWN(Address, To) ((ULONG_PTR)(Address) & ~((ULONG_PTR)(To)-1))

#define DELAY_ONE_MICROSECOND   (-10)
#define DELAY_ONE_MILLISECOND   (DELAY_ONE_MICROSECOND*1000)
#define DELAY_ONE_SECOND        (DELAY_ONE_MILLISECOND*1000)

//--------------------------------------------------
// SYSTEM
//--------------------------------------------------
typedef NTSTATUS
(NTAPI *_NtQuerySystemInformation)(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtQueryPerformanceCounter)(
    OUT PLARGE_INTEGER PerformanceCounter,
    OUT PLARGE_INTEGER PerformanceFrequency OPTIONAL);

//--------------------------------------------------
// PROCESS AND THREADS
//--------------------------------------------------
typedef NTSTATUS
(NTAPI *_NtOpenProcess)(
    OUT PHANDLE ProcessHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN PCLIENT_ID ClientId OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtSetContextThread)(
    IN HANDLE ThreadHandle,
    IN PCONTEXT Context);

typedef NTSTATUS
(NTAPI *_NtGetContextThread)(
    IN HANDLE ThreadHandle,
    OUT PCONTEXT pContext);

typedef NTSTATUS
(NTAPI *_NtQueryInformationProcess)(
    IN HANDLE ProcessHandle,
    IN int ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN ULONG ProcessInformationLength,
    OUT PULONG ReturnLength OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtCreateThread)(
    OUT PHANDLE ThreadHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN HANDLE ProcessHandle,
    OUT PCLIENT_ID ClientId,
    IN PCONTEXT ThreadContext,
    IN PINITIAL_TEB InitialTeb,
    IN BOOLEAN CreateSuspended);

typedef NTSTATUS
(NTAPI *_NtTerminateThread)(
    IN HANDLE ThreadHandle OPTIONAL,
    IN NTSTATUS ExitStatus);

typedef NTSTATUS
(NTAPI *_NtQueueApcThread)(
    IN HANDLE ThreadHandle,
    IN PPS_APC_ROUTINE ApcRoutine,
    IN PVOID ApcArgument1,
    IN PVOID ApcArgument2,
    IN PVOID ApcArgument3);

typedef NTSTATUS
(NTAPI *_NtResumeThread)(
    IN HANDLE ThreadHandle,
    OUT PULONG PreviousSuspendCount OPTIONAL
    );

//--------------------------------------------------
// Virtual Memory
//--------------------------------------------------
typedef NTSTATUS
(NTAPI *_NtAllocateVirtualMemory)(
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN ULONG_PTR ZeroBits,
    IN OUT PSIZE_T RegionSize,
    IN ULONG AllocationType,
    IN ULONG Protect);

typedef NTSTATUS
(NTAPI *_NtFreeVirtualMemory)(
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize,
    IN ULONG FreeType);

typedef NTSTATUS
(NTAPI *_NtReadVirtualMemory)(
    IN HANDLE      ProcessHandle,
    IN PVOID       BaseAddress,
    OUT PVOID      Buffer,
    IN ULONG       NumberOfBytesToRead,
    OUT PULONG     NumberOfBytesReaded OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtWriteVirtualMemory)(
     IN HANDLE ProcessHandle,
     OUT PVOID BaseAddress,
     IN PVOID Buffer,
     IN ULONG BufferSize,
     OUT PULONG NumberOfBytesWritten OPTIONAL);


typedef NTSTATUS
(NTAPI *_NtProtectVirtualMemory)(
    IN HANDLE       ProcessHandle,
    IN OUT PVOID    *BaseAddress,
    IN OUT PULONG   NumberOfBytesToProtect,
    IN ULONG        NewAccessProtection,
    OUT PULONG      OldAccessProtection);

typedef NTSTATUS
(NTAPI *_NtCreateSection) (
    OUT PHANDLE SectionHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN PLARGE_INTEGER MaximumSize OPTIONAL,
    IN ULONG SectionPageProtection,
    IN ULONG AllocationAttributes,
    IN HANDLE FileHandle OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtMapViewOfSection) (
    IN HANDLE SectionHandle,
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN ULONG_PTR ZeroBits,
    IN SIZE_T CommitSize,
    IN OUT PLARGE_INTEGER SectionOffset OPTIONAL,
    IN OUT PSIZE_T ViewSize,
    IN SECTION_INHERIT InheritDisposition,
    IN ULONG AllocationType,
    IN ULONG Protect);

typedef NTSTATUS 
(NTAPI *_NtUnmapViewOfSection)(
    IN HANDLE ProcessHandle,
    IN PVOID BaseAddress);

//--------------------------------------------------
// IO MANAGER
//--------------------------------------------------
typedef NTSTATUS
(NTAPI *_NtCreateFile)(
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize OPTIONAL,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG CreateDisposition,
    IN ULONG CreateOptions,
    IN PVOID EaBuffer OPTIONAL,
    IN ULONG EaLength);

typedef NTSTATUS
(NTAPI *_NtDeviceIoControlFile)(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PVOID ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG IoControlCode,
    IN PVOID InputBuffer OPTIONAL,
    IN ULONG InputBufferLength,
    OUT PVOID OutputBuffer OPTIONAL,
    IN ULONG OutputBufferLength);

typedef NTSTATUS
(NTAPI *_NtWriteFile)(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PVOID Buffer,
    IN ULONG Length,
    IN PLARGE_INTEGER ByteOffset OPTIONAL,
    IN PULONG Key OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtReadFile)(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID Buffer,
    IN ULONG Length,
    IN PLARGE_INTEGER ByteOffset OPTIONAL,
    IN PULONG Key OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtLoadDriver)(
    IN PUNICODE_STRING DriverServiceName);

typedef NTSTATUS
(NTAPI *_NtUnloadDriver)(
    IN PUNICODE_STRING DriverServiceName);

//--------------------------------------------------
// REGISTRY
//--------------------------------------------------
typedef NTSTATUS
(NTAPI *_NtOpenKey)(
    OUT PHANDLE KeyHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes);

typedef NTSTATUS
(NTAPI *_NtCreateKey)(
    OUT PHANDLE  KeyHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes,
    IN ULONG  TitleIndex,
    IN PUNICODE_STRING  Class  OPTIONAL,
    IN ULONG  CreateOptions,
    OUT PULONG  Disposition  OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtDeleteKey)(
    IN HANDLE  KeyHandle);

typedef NTSTATUS 
(NTAPI *_NtSetValueKey)(
    IN HANDLE  KeyHandle,
    IN PUNICODE_STRING  ValueName,
    IN ULONG  TitleIndex  OPTIONAL,
    IN ULONG  Type,
    IN PVOID  Data,
    IN ULONG  DataSize);

typedef NTSTATUS
(NTAPI *_NtSetInformationKey)(
    IN HANDLE KeyHandle,
    IN KEY_SET_INFORMATION_CLASS KeySetInformationClass,
    IN PVOID KeySetInformation,
    IN ULONG KeySetInformationLength);

typedef NTSTATUS
(NTAPI *_NtEnumerateKey)(
    IN HANDLE KeyHandle,
    IN ULONG Index,
    IN KEY_INFORMATION_CLASS KeyInformationClass,
    IN PVOID KeyInformation,
    IN ULONG Length,
    IN PULONG ResultLength);

typedef NTSTATUS
(NTAPI *_NtNotifyChangeKey)(
    IN HANDLE KeyHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG CompletionFilter,
    IN BOOLEAN WatchTree,
    OUT PVOID Buffer,
    IN ULONG BufferSize,
    IN BOOLEAN Asynchronous);

//--------------------------------------------------
// OBJECTS
//--------------------------------------------------
typedef NTSTATUS
(NTAPI *_NtQueryObject)(
    IN HANDLE Handle,
    IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
    OUT PVOID ObjectInformation,
    IN ULONG ObjectInformationLength,
    OUT PULONG ReturnLength OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtSetSecurityObject)(
    IN HANDLE Handle,
    IN SECURITY_INFORMATION SecurityInformation,
    IN PSECURITY_DESCRIPTOR SecurityDescriptor);

typedef NTSTATUS
(NTAPI *_NtQuerySecurityObject) (
    IN HANDLE Handle,
    IN SECURITY_INFORMATION SecurityInformation,
    OUT PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN ULONG Length,
    OUT PULONG LengthNeeded);

typedef NTSTATUS
(NTAPI *_NtClose)(IN HANDLE Handle);

typedef NTSTATUS
(NTAPI *_NtCreateEvent)(
    OUT PHANDLE EventHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN EVENT_TYPE EventType,
    IN BOOLEAN InitialState);

typedef NTSTATUS
(NTAPI *_NtSetEvent)(
    IN HANDLE EventHandle,
    OUT PLONG PreviousState OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtWaitForSingleObject)(
    IN HANDLE Handle,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtWaitForMultipleObjects)(
    IN ULONG Count,
    IN HANDLE Handles[],
    IN WAIT_TYPE WaitType,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtCreateSymbolicLinkObject)(
    OUT PHANDLE LinkHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN PUNICODE_STRING LinkTarget);

typedef NTSTATUS
(NTAPI *_NtOpenDirectoryObject)(
    OUT PHANDLE DirectoryHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes);

typedef NTSTATUS
(NTAPI *_NtQueryDirectoryObject)(
    IN HANDLE DirectoryHandle,
    OUT PVOID Buffer,
    IN ULONG Length,
    IN BOOLEAN ReturnSingleEntry,
    IN BOOLEAN RestartScan,
    IN OUT PULONG Context,
    OUT PULONG ReturnLength OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtOpenSymbolicLinkObject)(
    OUT PHANDLE LinkHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes);

typedef NTSTATUS
(NTAPI *_NtQuerySymbolicLinkObject)(
    IN HANDLE LinkHandle,
    IN OUT PUNICODE_STRING LinkTarget,
    OUT PULONG ReturnedLength OPTIONAL);


//--------------------------------------------------
// SECURITY
//--------------------------------------------------
typedef NTSTATUS
(NTAPI *_NtOpenProcessToken)(
    IN HANDLE ProcessHandle,
    IN ACCESS_MASK DesiredAccess,
    OUT PHANDLE TokenHandle);

typedef NTSTATUS
(NTAPI *_NtAdjustPrivilegesToken)(
    IN HANDLE TokenHandle,
    IN BOOLEAN DisableAllPrivileges,
    IN PTOKEN_PRIVILEGES NewState OPTIONAL,
    IN ULONG BufferLength OPTIONAL,
    OUT PTOKEN_PRIVILEGES PreviousState OPTIONAL,
    OUT PULONG ReturnLength);

typedef NTSTATUS
(NTAPI *_NtDuplicateToken)(
    IN HANDLE ExistingTokenHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN BOOLEAN EffectiveOnly,
    IN TOKEN_TYPE TokenType,
    OUT PHANDLE NewTokenHandle
    );

typedef NTSTATUS
(NTAPI *_NtCallbackReturn)(
   IN PVOID OutputBuffer OPTIONAL,
   IN ULONG OutputLength,
   IN NTSTATUS Status);

//--------------------------------------------------
// PlugAndPlay
//--------------------------------------------------
typedef NTSTATUS
(NTAPI *_NtPlugPlayControl)(
    IN     PLUGPLAY_CONTROL_CLASS PnPControlClass,
    IN OUT PVOID PnPControlData,
    IN     ULONG PnPControlDataLength);

typedef NTSTATUS
(NTAPI *_NtGetPlugPlayEvent)(
    IN ULONG Reserved1,
    IN ULONG Reserved2,
    OUT struct _PLUGPLAY_EVENT_BLOCK *Buffer,
    IN ULONG BufferSize);


//--------------------------------------------------
// LPC
//--------------------------------------------------
typedef struct _PORT_VIEW {
    ULONG Length;
    HANDLE SectionHandle;
    ULONG SectionOffset;
    ULONG ViewSize;
    PVOID ViewBase;
    PVOID ViewRemoteBase;
} PORT_VIEW, *PPORT_VIEW;

typedef struct _REMOTE_PORT_VIEW {
    ULONG Length;
    ULONG ViewSize;
    PVOID ViewBase;
} REMOTE_PORT_VIEW, *PREMOTE_PORT_VIEW;

#define LPC_REQUEST 1
#define LPC_REPLY 2
#define LPC_DATAGRAM 3
#define LPC_LOST_REPLY 4
#define LPC_PORT_CLOSED 5
#define LPC_CLIENT_DIED 6
#define LPC_EXCEPTION 7
#define LPC_DEBUG_EVENT 8
#define LPC_ERROR_EVENT 9
#define LPC_CONNECTION_REQUEST 10

typedef struct _PORT_MESSAGE {
    union {
        struct {
            SHORT DataLength;
            SHORT TotalLength;
        } a;
        ULONG Length;
    };

    union {
        struct {
            SHORT Type;
            SHORT DataInfoOffset;
        } b;
        ULONG ZeroBits;
    };

    CLIENT_ID ClientId;
    ULONG MessageId;
    ULONG ClientViewSize;
} PORT_MESSAGE, *PPORT_MESSAGE;

typedef NTSTATUS
(NTAPI *_NtConnectPort)(
    OUT PHANDLE PortHandle,
    IN PUNICODE_STRING PortName,
    IN PSECURITY_QUALITY_OF_SERVICE SecurityQos,
    IN OUT PPORT_VIEW ClientView OPTIONAL,
    OUT PREMOTE_PORT_VIEW ServerView OPTIONAL,
    OUT PULONG MaxMessageLength OPTIONAL,
    IN OUT PVOID ConnectionInformation OPTIONAL,
    IN OUT PULONG ConnectionInformationLength OPTIONAL
    );

typedef NTSTATUS
(NTAPI *_NtCreatePort)(
    OUT PHANDLE PortHandle,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN ULONG MaxConnectionInfoLength,
    IN ULONG MaxMessageLength,
    IN ULONG MaxPoolUsage
    );


typedef NTSTATUS
(NTAPI *_NtListenPort)(
    IN HANDLE PortHandle,
    OUT PPORT_MESSAGE ConnectionRequest
    );

NTSTATUS
(NTAPI *_NtAcceptConnectPort)(
    OUT PHANDLE PortHandle,
    IN PVOID PortContext,
    IN PPORT_MESSAGE ConnectionRequest,
    IN BOOLEAN AcceptConnection,
    IN OUT PPORT_VIEW ServerView OPTIONAL,
    OUT PREMOTE_PORT_VIEW ClientView OPTIONAL
    );

NTSTATUS
(NTAPI *_NtCompleteConnectPort)(
    IN HANDLE PortHandle
    );

NTSTATUS
(NTAPI *_NtRequestPort)(
    IN HANDLE PortHandle,
    IN PPORT_MESSAGE RequestMessage
    );

NTSTATUS
(NTAPI *_NtRequestWaitReplyPort)(
    IN HANDLE PortHandle,
    IN PPORT_MESSAGE RequestMessage,
    OUT PPORT_MESSAGE ReplyMessage
    );

NTSTATUS
(NTAPI *_NtReplyPort)(
    IN HANDLE PortHandle,
    IN PPORT_MESSAGE ReplyMessage
    );


NTSTATUS
(NTAPI *_NtReplyWaitReplyPort)(
    IN HANDLE PortHandle,
    IN OUT PPORT_MESSAGE ReplyMessage
    );

NTSTATUS
(NTAPI *_NtReplyWaitReceivePort)(
    IN HANDLE PortHandle,
    OUT PVOID *PortContext OPTIONAL,
    IN PPORT_MESSAGE ReplyMessage OPTIONAL,
    OUT PPORT_MESSAGE ReceiveMessage
    );

NTSTATUS
(NTAPI *_NtImpersonateClientOfPort)(
    IN HANDLE PortHandle,
    IN PPORT_MESSAGE Message
    );

NTSTATUS
(NTAPI *_NtReadRequestData)(
    IN HANDLE PortHandle,
    IN PPORT_MESSAGE Message,
    IN ULONG DataEntryIndex,
    OUT PVOID Buffer,
    IN ULONG BufferSize,
    OUT PULONG NumberOfBytesRead OPTIONAL
    );

NTSTATUS
(NTAPI *_NtWriteRequestData)(
    IN HANDLE PortHandle,
    IN PPORT_MESSAGE Message,
    IN ULONG DataEntryIndex,
    IN PVOID Buffer,
    IN ULONG BufferSize,
    OUT PULONG NumberOfBytesWritten OPTIONAL
    );


typedef enum _PORT_INFORMATION_CLASS {
    PortBasicInformation
#if DEVL
,   PortDumpInformation
#endif
} PORT_INFORMATION_CLASS;


NTSTATUS
(NTAPI *_NtQueryInformationPort)(
    IN HANDLE PortHandle,
    IN PORT_INFORMATION_CLASS PortInformationClass,
    OUT PVOID PortInformation,
    IN ULONG Length,
    OUT PULONG ReturnLength OPTIONAL
    );

//--------------------------------------------------
// ALPC
//--------------------------------------------------

typedef PVOID ALPC_HANDLE, *PALPC_HANDLE;

// begin_rev
#define ALPC_MESSAGE_SECURITY_ATTRIBUTE 0x80000000
#define ALPC_MESSAGE_VIEW_ATTRIBUTE 0x40000000
#define ALPC_MESSAGE_CONTEXT_ATTRIBUTE 0x20000000
#define ALPC_MESSAGE_HANDLE_ATTRIBUTE 0x10000000
// end_rev

// symbols
typedef struct _ALPC_MESSAGE_ATTRIBUTES {
    ULONG AllocatedAttributes;
    ULONG ValidAttributes;
} ALPC_MESSAGE_ATTRIBUTES, *PALPC_MESSAGE_ATTRIBUTES;

#define ALPC_PORFLG_ALLOW_LPC_REQUESTS 0x20000 // rev
#define ALPC_PORFLG_WAITABLE_PORT 0x40000 // dbg
#define ALPC_PORFLG_SYSTEM_PROCESS 0x100000 // dbg

#define ALPC_SYNC_CONNECTION 0x020000 // Synchronous connection request
#define ALPC_USER_WAIT_MODE 0x100000 // Wait in user mode
#define ALPC_WAIT_IS_ALERTABLE 0x200000 // Wait in alertable mode

// Message buffer structure for section view
typedef struct _ALPC_MESSAGE_VIEW {
    ULONG Flag;
    HANDLE AlpcSectionHandle;
    PVOID MapBase;
    SIZE_T MapSize;
} ALPC_MESSAGE_VIEW, *PALPC_MESSAGE_VIEW;

typedef struct _ALPC_PORT_ATTRIBUTES {
    ULONG Flags;
    SECURITY_QUALITY_OF_SERVICE SecurityQos;
    SIZE_T MaxMessageLength;
    SIZE_T MemoryBandwidth;
    SIZE_T MaxPoolUsage;
    SIZE_T MaxSectionSize;
    SIZE_T MaxViewSize;
    SIZE_T MaxTotalSectionSize;
    ULONG DupObjectTypes;
#ifdef _M_X64
    ULONG Reserved;
#endif
} ALPC_PORT_ATTRIBUTES, *PALPC_PORT_ATTRIBUTES;

// private
typedef struct _ALPC_DATA_VIEW_ATTR {
    ULONG Flags;
    ALPC_HANDLE SectionHandle;
    PVOID ViewBase; // must be zero on input
    SIZE_T ViewSize;
} ALPC_DATA_VIEW_ATTR, *PALPC_DATA_VIEW_ATTR;

// private
typedef struct _ALPC_CONTEXT_ATTR {
    PVOID PortContext;
    PVOID MessageContext;
    ULONG Sequence;
    ULONG MessageId;
    ULONG CallbackId;
} ALPC_CONTEXT_ATTR, *PALPC_CONTEXT_ATTR;


typedef NTSTATUS
(NTAPI *_NtAlpcCreatePort)(
    OUT PHANDLE PortHandle,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN OUT PALPC_PORT_ATTRIBUTES PortAttributes 
	); 

typedef NTSTATUS
(NTAPI *_NtAlpcDisconnectPort)(
    IN HANDLE PortHandle,
    IN ULONG Flags);

#define ALPC_MSGFLG_REPLY_MESSAGE 0x1
#define ALPC_MSGFLG_LPC_MODE 0x2 // ?
#define ALPC_MSGFLG_RELEASE_MESSAGE 0x10000 // dbg
#define ALPC_MSGFLG_SYNC_REQUEST 0x20000 // dbg
#define ALPC_MSGFLG_WAIT_USER_MODE 0x100000
#define ALPC_MSGFLG_WAIT_ALERTABLE 0x200000
#define ALPC_MSGFLG_WOW64_CALL 0x80000000 // dbg

typedef NTSTATUS
(NTAPI *_NtAlpcConnectPort)(
    OUT PHANDLE PortHandle,
    IN PUNICODE_STRING PortName,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN PALPC_PORT_ATTRIBUTES PortAttributes OPTIONAL,
    IN ULONG Flags,
    IN PSID RequiredServerSid OPTIONAL,
    IN OUT PPORT_MESSAGE ConnectionMessage,
    IN OUT PULONG BufferLength OPTIONAL,
    IN OUT PALPC_MESSAGE_ATTRIBUTES OutMessageAttributes OPTIONAL,
    IN OUT PALPC_MESSAGE_ATTRIBUTES InMessageAttributes OPTIONAL,
    IN PLARGE_INTEGER Timeout OPTIONAL
    );


typedef NTSTATUS
(NTAPI *_NtAlpcSendWaitReceivePort)(
    HANDLE PortHandle,
    DWORD SendFlags,
    PORT_MESSAGE *SendMessage OPTIONAL,
    PVOID InMessageBuffer OPTIONAL,
    PORT_MESSAGE *ReceiveBuffer OPTIONAL,
    PULONG ReceiveBufferSize OPTIONAL,
    PVOID OutMessageBuffer OPTIONAL,
    PLARGE_INTEGER Timeout OPTIONAL);

typedef NTSTATUS
(NTAPI *_NtAlpcDisconnectPort)(
    IN HANDLE PortHandle,
    IN ULONG Flags
    );

// With this flag, next function creates its own section (SectionHandle argument must be zero)
#define ALPC_SECTION_NOSECTIONHANDLE 0x40000

typedef NTSTATUS
(NTAPI *_NtAlpcCreatePortSection)(
    IN HANDLE PortHandle,
    IN ULONG Flags,
    IN HANDLE SectionHandle OPTIONAL,
    IN SIZE_T SectionSize,
    OUT PALPC_HANDLE AlpcSectionHandle,
    OUT PSIZE_T ActualSectionSize
    );

typedef NTSTATUS
(NTAPI *_NtAlpcDeletePortSection)(
    IN HANDLE PortHandle,
    IN ULONG Flags,
    IN ALPC_HANDLE SectionHandle
    );

typedef NTSTATUS
(NTAPI *_NtAlpcCreateSectionView)(
    IN HANDLE PortHandle,
    IN ULONG Flags,
    IN OUT PALPC_DATA_VIEW_ATTR ViewAttributes
    );

typedef NTSTATUS
(NTAPI *_NtAlpcDeleteSectionView)(
    IN HANDLE PortHandle,
    ULONG Flags,
    IN PVOID ViewBase
    );

#define ALPC_CANCELFLG_TRY_CANCEL 0x1 // dbg
#define ALPC_CANCELFLG_NO_CONTEXT_CHECK 0x8
#define ALPC_CANCELFLGP_FLUSH 0x10000 // dbg

typedef NTSTATUS
(NTAPI *_NtAlpcCancelMessage)(
    IN HANDLE PortHandle,
    IN ULONG Flags,
    IN PALPC_CONTEXT_ATTR MessageContext
    );

typedef NTSTATUS
(NTAPI *_NtAlpcAcceptConnectPort)(
    OUT PHANDLE PortHandle,
    IN HANDLE ConnectionPortHandle,
    IN ULONG Flags,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN PALPC_PORT_ATTRIBUTES PortAttributes,
    IN PVOID PortContext OPTIONAL,
    IN PPORT_MESSAGE ConnectionRequest,
    IN OUT PALPC_MESSAGE_ATTRIBUTES ConnectionMessageAttributes OPTIONAL,
    IN BOOLEAN AcceptConnection
    );

typedef NTSTATUS
(NTAPI *_NtAlpcOpenSenderProcess)(
    OUT PHANDLE ProcessHandle,
    IN HANDLE PortHandle,
    IN PPORT_MESSAGE PortMessage,
    ULONG Flags,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes
    );

typedef NTSTATUS
(NTAPI *_NtAlpcOpenSenderThread)(
    OUT PHANDLE ThreadHandle,
    IN HANDLE PortHandle,
    IN PPORT_MESSAGE PortMessage,
    ULONG Flags,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes
    );

typedef enum _ALPC_PORT_INFORMATION_CLASS {
    AlpcBasicInformation, // q: out ALPC_BASIC_INFORMATION
    AlpcPortInformation, // s: in ALPC_PORT_ATTRIBUTES
    AlpcAssociateCompletionPortInformation, // s: in ALPC_PORT_ASSOCIATE_COMPLETION_PORT
    AlpcConnectedSIDInformation, // q: in SID
    AlpcServerInformation, // q: inout ALPC_SERVER_INFORMATION
    AlpcMessageZoneInformation, // s: in ALPC_PORT_MESSAGE_ZONE_INFORMATION
    AlpcRegisterCompletionListInformation, // s: in ALPC_PORT_COMPLETION_LIST_INFORMATION
    AlpcUnregisterCompletionListInformation, // s: VOID
    AlpcAdjustCompletionListConcurrencyCountInformation, // s: in ULONG
    AlpcRegisterCallback, // kernel-mode only // rev
    AlpcDisableCompletionList, // s: VOID // rev
    MaxAlpcPortInfoClass
} ALPC_PORT_INFORMATION_CLASS;

typedef struct _ALPC_BASIC_INFORMATION {
    ULONG Flags;
    ULONG SequenceNo;
    PVOID PortContext;
} ALPC_BASIC_INFORMATION, *PALPC_BASIC_INFORMATION;

// private
typedef struct _ALPC_PORT_ASSOCIATE_COMPLETION_PORT {
    PVOID CompletionKey;
    HANDLE CompletionPort;
} ALPC_PORT_ASSOCIATE_COMPLETION_PORT, *PALPC_PORT_ASSOCIATE_COMPLETION_PORT;

typedef struct _ALPC_SERVER_INFORMATION {
    union
    {
        struct {
            HANDLE ThreadHandle;
        } In;
        struct {
            BOOLEAN ThreadBlocked;
            HANDLE ConnectedProcessId;
            UNICODE_STRING ConnectionPortName;
        } Out;
    };
} ALPC_SERVER_INFORMATION, *PALPC_SERVER_INFORMATION;

typedef NTSTATUS
(NTAPI *_NtAlpcQueryInformation)(
    IN HANDLE PortHandle,
    IN ALPC_PORT_INFORMATION_CLASS PortInformationClass,
    OUT PVOID PortInformation,
    IN ULONG Length,
    OUT PULONG ReturnLength OPTIONAL
    );

//--------------------------------------------------
// csrss
//--------------------------------------------------

typedef struct _CSR_CAPTURE_HEADER {
    ULONG Length;
    struct _CSR_CAPTURE_HEADER *RelatedCaptureBuffer;
    ULONG CountMessagePointers;
    ULONG CountCapturePointers;
    PULONG MessagePointerOffsets;   // Offsets within CSR_API_MSG of pointers
    PULONG CapturePointerOffsets;   // Offsets within CaptureBuffer of pointers
    PCHAR FreeSpace;
} CSR_CAPTURE_HEADER, *PCSR_CAPTURE_HEADER;

typedef struct _CSRSS_MESSAGE {
    PCSR_CAPTURE_HEADER CaptureBuffer;     // buffer captured
    ULONG Opcode;            // opcode of current operation
    ULONG Status;            // status returned by function
    ULONG Reserved;
} CSRSS_MESSAGE;

typedef struct _BASE_CREATETHREAD_MSG {
    HANDLE ThreadHandle;
    CLIENT_ID ClientId;
} BASE_CREATETHREAD_MSG, *PBASE_CREATETHREAD_MSG;

typedef struct _CSRSS_API_REQUEST {
    PORT_MESSAGE  PortMessage;
    CSRSS_MESSAGE CsrssMessage;

    union {
        BASE_CREATETHREAD_MSG  CreateThread;
        UCHAR                  _padding[0x100];
    };
} CSRSS_API_REQUEST, *PCSRSS_API_REQUEST;


//--------------------------------------------------
// NTDLL.API
//--------------------------------------------------
typedef VOID
(NTAPI *_CsrNewThread)();

typedef NTSTATUS
(NTAPI *_CsrClientCallServer)(
    IN CSRSS_API_REQUEST *Message, 
    IN PVOID             CapturedBuffer OPTIONAL,
    IN ULONG             ApiNumber,
    IN ULONG             ArgLength);

typedef ULONG
(NTAPI *_RtlNtStatusToDosError)(IN NTSTATUS Status);

typedef NTSTATUS
(NTAPI *_RtlGetVersion)(
    IN OUT PRTL_OSVERSIONINFOW  lpVersionInformation);

typedef struct _RTL_RELATIVE_NAME {
  STRING RelativeName;
  PVOID ContainingDirectory;
} RTL_RELATIVE_NAME, *PRTL_RELATIVE_NAME;

typedef NTSTATUS
(NTAPI *_RtlDosPathNameToNtPathName_U)(
    IN PCWSTR DosPathName,
    OUT PUNICODE_STRING NtPathName,
    OUT PWSTR* FilePathInNtPathName OPTIONAL,
    OUT struct _PRELATIVE_NAME* RelativeName OPTIONAL);

/*
    _RtlDosPathNameToNtPathName_U   RtlDosPathNameToNtPathName_U = (_RtlDosPathNameToNtPathName_U)
        GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "RtlDosPathNameToNtPathName_U");

    UNICODE_STRING NtPathName;
    RtlDosPathNameToNtPathName_U(L"\\\\.\\PROCEXP111", &NtPathName, NULL, NULL);
*/

typedef VOID
(NTAPI *_RtlAcquirePebLock)(VOID);

typedef VOID
(NTAPI *_RtlReleasePebLock)(VOID);

//
// LdrLockLoaderLock Flags
//
#define LDR_LOCK_LOADER_LOCK_FLAG_RAISE_STATUS  0x00000001  /* i don't whant raise exception */
#define LDR_LOCK_LOADER_LOCK_FLAG_TRY_ONLY      0x00000002  /* and try only this not for my tasks */

typedef NTSTATUS 
(NTAPI *_LdrLockLoaderLock)(
    IN ULONG Flags,
    OUT PULONG Disposition OPTIONAL,
    OUT PULONG Cookie OPTIONAL);

typedef NTSTATUS
(NTAPI *_LdrUnlockLoaderLock)(
    IN ULONG Flags,
    IN ULONG Cookie OPTIONAL);

typedef NTSTATUS
(NTAPI *_LdrGetDllHandle)(
    IN PWSTR DllPath OPTIONAL,
    IN PULONG DllCharacteristics OPTIONAL,
    IN PUNICODE_STRING DllName,
    OUT PVOID *DllHandle);

typedef NTSTATUS
(NTAPI *_LdrLoadDll)(
    IN PWSTR DllPath OPTIONAL,
    IN PULONG DllCharacteristics OPTIONAL,
    IN PUNICODE_STRING DllName,
    OUT PVOID *DllHandle);

typedef HANDLE
(WINAPI *_CreateRemoteThread)(
  HANDLE hProcess,
  LPSECURITY_ATTRIBUTES lpThreadAttributes,
  SIZE_T dwStackSize,
  LPTHREAD_START_ROUTINE lpStartAddress,
  LPVOID lpParameter,
  DWORD dwCreationFlags,
  LPDWORD lpThreadId);



#endif // __NTINTERNAL_H__
