#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <assert.h>
#include <Psapi.h>
#include <TlHelp32.h>


#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS) 0xC0000004)
#define NT_SUCCESS(Status)			(((NTSTATUS)(Status)) >= 0)
#define STATUS_SUCCESS				((NTSTATUS)0x00000000L)
#define AE_RESACCESS2 18

#define 	DEBUG_READ_EVENT   0x0001
#define 	DEBUG_PROCESS_ASSIGN   0x0002
#define 	DEBUG_SET_INFORMATION   0x0004
#define 	DEBUG_QUERY_INFORMATION   0x0008
#define DEBUG_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | \
    DEBUG_READ_EVENT | DEBUG_PROCESS_ASSIGN | DEBUG_SET_INFORMATION | \
    DEBUG_QUERY_INFORMATION)
#define DEBUG_OBJECT_KILLONCLOSE 0x1

#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }

namespace
{
	typedef struct _UNICODE_STRING
	{
		USHORT Length;
		USHORT MaximumLength;
		PWSTR  Buffer;
	} UNICODE_STRING, *PUNICODE_STRING;

	typedef struct _OBJECT_ATTRIBUTES {
		ULONG Length;
		HANDLE RootDirectory;
		PUNICODE_STRING ObjectName;
		ULONG Attributes;
		PVOID SecurityDescriptor;
		PVOID SecurityQualityOfService;
	} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

	typedef enum _SYSTEM_INFORMATION_CLASS
	{
		SystemBasicInformation, // q: SYSTEM_BASIC_INFORMATION
		SystemProcessorInformation, // q: SYSTEM_PROCESSOR_INFORMATION
		SystemPerformanceInformation, // q: SYSTEM_PERFORMANCE_INFORMATION
		SystemTimeOfDayInformation, // q: SYSTEM_TIMEOFDAY_INFORMATION
		SystemPathInformation, // not implemented
		SystemProcessInformation, // q: SYSTEM_PROCESS_INFORMATION
		SystemCallCountInformation, // q: SYSTEM_CALL_COUNT_INFORMATION
		SystemDeviceInformation, // q: SYSTEM_DEVICE_INFORMATION
		SystemProcessorPerformanceInformation, // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
		SystemFlagsInformation, // q: SYSTEM_FLAGS_INFORMATION
		SystemCallTimeInformation, // 10, not implemented
		SystemModuleInformation, // q: RTL_PROCESS_MODULES
		SystemLocksInformation,
		SystemStackTraceInformation,
		SystemPagedPoolInformation, // not implemented
		SystemNonPagedPoolInformation, // not implemented
		SystemHandleInformation, // q: SYSTEM_HANDLE_INFORMATION
		SystemObjectInformation, // q: SYSTEM_OBJECTTYPE_INFORMATION mixed with SYSTEM_OBJECT_INFORMATION
		SystemPageFileInformation, // q: SYSTEM_PAGEFILE_INFORMATION
		SystemVdmInstemulInformation, // q
		SystemVdmBopInformation, // 20, not implemented
		SystemFileCacheInformation, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemCache)
		SystemPoolTagInformation, // q: SYSTEM_POOLTAG_INFORMATION
		SystemInterruptInformation, // q: SYSTEM_INTERRUPT_INFORMATION
		SystemDpcBehaviorInformation, // q: SYSTEM_DPC_BEHAVIOR_INFORMATION; s: SYSTEM_DPC_BEHAVIOR_INFORMATION (requires SeLoadDriverPrivilege)
		SystemFullMemoryInformation, // not implemented
		SystemLoadGdiDriverInformation, // s (kernel-mode only)
		SystemUnloadGdiDriverInformation, // s (kernel-mode only)
		SystemTimeAdjustmentInformation, // q: SYSTEM_QUERY_TIME_ADJUST_INFORMATION; s: SYSTEM_SET_TIME_ADJUST_INFORMATION (requires SeSystemtimePrivilege)
		SystemSummaryMemoryInformation, // not implemented
		SystemMirrorMemoryInformation, // 30, s (requires license value "Kernel-MemoryMirroringSupported") (requires SeShutdownPrivilege)
		SystemPerformanceTraceInformation, // s
		SystemObsolete0, // not implemented
		SystemExceptionInformation, // q: SYSTEM_EXCEPTION_INFORMATION
		SystemCrashDumpStateInformation, // s (requires SeDebugPrivilege)
		SystemKernelDebuggerInformation, // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION
		SystemContextSwitchInformation, // q: SYSTEM_CONTEXT_SWITCH_INFORMATION
		SystemRegistryQuotaInformation, // q: SYSTEM_REGISTRY_QUOTA_INFORMATION; s (requires SeIncreaseQuotaPrivilege)
		SystemExtendServiceTableInformation, // s (requires SeLoadDriverPrivilege) // loads win32k only
		SystemPrioritySeperation, // s (requires SeTcbPrivilege)
		SystemVerifierAddDriverInformation, // 40, s (requires SeDebugPrivilege)
		SystemVerifierRemoveDriverInformation, // s (requires SeDebugPrivilege)
		SystemProcessorIdleInformation, // q: SYSTEM_PROCESSOR_IDLE_INFORMATION
		SystemLegacyDriverInformation, // q: SYSTEM_LEGACY_DRIVER_INFORMATION
		SystemCurrentTimeZoneInformation, // q
		SystemLookasideInformation, // q: SYSTEM_LOOKASIDE_INFORMATION
		SystemTimeSlipNotification, // s (requires SeSystemtimePrivilege)
		SystemSessionCreate, // not implemented
		SystemSessionDetach, // not implemented
		SystemSessionInformation, // not implemented
		SystemRangeStartInformation, // 50, q
		SystemVerifierInformation, // q: SYSTEM_VERIFIER_INFORMATION; s (requires SeDebugPrivilege)
		SystemVerifierThunkExtend, // s (kernel-mode only)
		SystemSessionProcessInformation, // q: SYSTEM_SESSION_PROCESS_INFORMATION
		SystemLoadGdiDriverInSystemSpace, // s (kernel-mode only) (same as SystemLoadGdiDriverInformation)
		SystemNumaProcessorMap, // q
		SystemPrefetcherInformation, // q: PREFETCHER_INFORMATION; s: PREFETCHER_INFORMATION // PfSnQueryPrefetcherInformation
		SystemExtendedProcessInformation, // q: SYSTEM_PROCESS_INFORMATION
		SystemRecommendedSharedDataAlignment, // q
		SystemComPlusPackage, // q; s
		SystemNumaAvailableMemory, // 60
		SystemProcessorPowerInformation, // q: SYSTEM_PROCESSOR_POWER_INFORMATION
		SystemEmulationBasicInformation, // q
		SystemEmulationProcessorInformation,
		SystemExtendedHandleInformation, // q: SYSTEM_HANDLE_INFORMATION_EX
		SystemLostDelayedWriteInformation, // q: ULONG
		SystemBigPoolInformation, // q: SYSTEM_BIGPOOL_INFORMATION
		SystemSessionPoolTagInformation, // q: SYSTEM_SESSION_POOLTAG_INFORMATION
		SystemSessionMappedViewInformation, // q: SYSTEM_SESSION_MAPPED_VIEW_INFORMATION
		SystemHotpatchInformation, // q; s
		SystemObjectSecurityMode, // 70, q
		SystemWatchdogTimerHandler, // s (kernel-mode only)
		SystemWatchdogTimerInformation, // q (kernel-mode only); s (kernel-mode only)
		SystemLogicalProcessorInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION
		SystemWow64SharedInformationObsolete, // not implemented
		SystemRegisterFirmwareTableInformationHandler, // s (kernel-mode only)
		SystemFirmwareTableInformation, // not implemented
		SystemModuleInformationEx, // q: RTL_PROCESS_MODULE_INFORMATION_EX
		SystemVerifierTriageInformation, // not implemented
		SystemSuperfetchInformation, // q: SUPERFETCH_INFORMATION; s: SUPERFETCH_INFORMATION // PfQuerySuperfetchInformation
		SystemMemoryListInformation, // 80, q: SYSTEM_MEMORY_LIST_INFORMATION; s: SYSTEM_MEMORY_LIST_COMMAND (requires SeProfileSingleProcessPrivilege)
		SystemFileCacheInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (same as SystemFileCacheInformation)
		SystemThreadPriorityClientIdInformation, // s: SYSTEM_THREAD_CID_PRIORITY_INFORMATION (requires SeIncreaseBasePriorityPrivilege)
		SystemProcessorIdleCycleTimeInformation, // q: SYSTEM_PROCESSOR_IDLE_CYCLE_TIME_INFORMATION[]
		SystemVerifierCancellationInformation, // not implemented // name:wow64:whNT32QuerySystemVerifierCancellationInformation
		SystemProcessorPowerInformationEx, // not implemented
		SystemRefTraceInformation, // q; s // ObQueryRefTraceInformation
		SystemSpecialPoolInformation, // q; s (requires SeDebugPrivilege) // MmSpecialPoolTag, then MmSpecialPoolCatchOverruns != 0
		SystemProcessIdInformation, // q: SYSTEM_PROCESS_ID_INFORMATION
		SystemErrorPortInformation, // s (requires SeTcbPrivilege)
		SystemBootEnvironmentInformation, // 90, q: SYSTEM_BOOT_ENVIRONMENT_INFORMATION
		SystemHypervisorInformation, // q; s (kernel-mode only)
		SystemVerifierInformationEx, // q; s
		SystemTimeZoneInformation, // s (requires SeTimeZonePrivilege)
		SystemImageFileExecutionOptionsInformation, // s: SYSTEM_IMAGE_FILE_EXECUTION_OPTIONS_INFORMATION (requires SeTcbPrivilege)
		SystemCoverageInformation, // q; s // name:wow64:whNT32QuerySystemCoverageInformation; ExpCovQueryInformation
		SystemPrefetchPatchInformation, // not implemented
		SystemVerifierFaultsInformation, // s (requires SeDebugPrivilege)
		SystemSystemPartitionInformation, // q: SYSTEM_SYSTEM_PARTITION_INFORMATION
		SystemSystemDiskInformation, // q: SYSTEM_SYSTEM_DISK_INFORMATION
		SystemProcessorPerformanceDistribution, // 100, q: SYSTEM_PROCESSOR_PERFORMANCE_DISTRIBUTION
		SystemNumaProximityNodeInformation, // q
		SystemDynamicTimeZoneInformation, // q; s (requires SeTimeZonePrivilege)
		SystemCodeIntegrityInformation, // q // SeCodeIntegrityQueryInformation
		SystemProcessorMicrocodeUpdateInformation, // s
		SystemProcessorBrandString, // q // HaliQuerySystemInformation -> HalpGetProcessorBrandString, info class 23
		SystemVirtualAddressInformation, // q: SYSTEM_VA_LIST_INFORMATION[]; s: SYSTEM_VA_LIST_INFORMATION[] (requires SeIncreaseQuotaPrivilege) // MmQuerySystemVaInformation
		SystemLogicalProcessorAndGroupInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX // since WIN7 // KeQueryLogicalProcessorRelationship
		SystemProcessorCycleTimeInformation, // q: SYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION[]
		SystemStoreInformation, // q; s // SmQueryStoreInformation
		SystemRegistryAppendString, // 110, s: SYSTEM_REGISTRY_APPEND_STRING_PARAMETERS
		SystemAitSamplingValue, // s: ULONG (requires SeProfileSingleProcessPrivilege)
		SystemVhdBootInformation, // q: SYSTEM_VHD_BOOT_INFORMATION
		SystemCpuQuotaInformation, // q; s // PsQueryCpuQuotaInformation
		SystemNativeBasicInformation, // not implemented
		SystemSpare1, // not implemented
		SystemLowPriorityIoInformation, // q: SYSTEM_LOW_PRIORITY_IO_INFORMATION
		SystemTpmBootEntropyInformation, // q: TPM_BOOT_ENTROPY_NT_RESULT // ExQueryTpmBootEntropyInformation
		SystemVerifierCountersInformation, // q: SYSTEM_VERIFIER_COUNTERS_INFORMATION
		SystemPagedPoolInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypePagedPool)
		SystemSystemPtesInformationEx, // 120, q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemPtes)
		SystemNodeDistanceInformation, // q
		SystemAcpiAuditInformation, // q: SYSTEM_ACPI_AUDIT_INFORMATION // HaliQuerySystemInformation -> HalpAuditQueryResults, info class 26
		SystemBasicPerformanceInformation, // q: SYSTEM_BASIC_PERFORMANCE_INFORMATION // name:wow64:whNtQuerySystemInformation_SystemBasicPerformanceInformation
		SystemQueryPerformanceCounterInformation, // q: SYSTEM_QUERY_PERFORMANCE_COUNTER_INFORMATION // since WIN7 SP1
		SystemSessionBigPoolInformation, // since WIN8
		SystemBootGraphicsInformation,
		SystemScrubPhysicalMemoryInformation,
		SystemBadPageInformation,
		SystemProcessorProfileControlArea,
		SystemCombinePhysicalMemoryInformation, // 130
		SystemEntropyInterruptTimingCallback,
		SystemConsoleInformation,
		SystemPlatformBinaryInformation,
		SystemThrottleNotificationInformation,
		SystemHypervisorProcessorCountInformation,
		SystemDeviceDataInformation,
		SystemDeviceDataEnumerationInformation,
		SystemMemoryTopologyInformation,
		SystemMemoryChannelInformation,
		SystemBootLogoInformation, // 140
		SystemProcessorPerformanceInformationEx, // since WINBLUE
		SystemSpare0,
		SystemSecureBootPolicyInformation,
		SystemPageFileInformationEx,
		SystemSecureBootInformation,
		SystemEntropyInterruptTimingRawInformation,
		SystemPortableWorkspaceEfiLauncherInformation,
		SystemFullProcessInformation, // q: SYSTEM_PROCESS_INFORMATION with SYSTEM_PROCESS_INFORMATION_EXTENSION (requires admin)
		SystemKernelDebuggerInformationEx,
		SystemBootMetadataInformation, // 150
		SystemSoftRebootInformation,
		SystemElamCertificateInformation,
		SystemOfflineDumpConfigInformation,
		SystemProcessorFeaturesInformation,
		SystemRegistryReconciliationInformation,
		SystemEdidInformation,
		MaxSystemInfoClass
	} SYSTEM_INFORMATION_CLASS;

	typedef struct _SYSTEM_HANDLE
	{
		ULONG ProcessId;
		BYTE ObjectTypeNumber;
		BYTE Flags;
		USHORT Handle;
		PVOID Object;
		ACCESS_MASK GrantedAccess;
	} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

	typedef struct _SYSTEM_HANDLE_INFORMATION
	{
		ULONG HandleCount;
		SYSTEM_HANDLE Handles[1];
	} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;
};

typedef NTSTATUS(NTAPI* lpNtGetNextProcess)(HANDLE ProcessHandle, ACCESS_MASK DesiredAccess, ULONG HandleAttributes, ULONG Flags, PHANDLE NewProcessHandle);
typedef NTSTATUS(NTAPI* lpNtGetNextThread)(HANDLE ProcessHandle, HANDLE ThreadHandle, ACCESS_MASK DesiredAccess, ULONG HandleAttributes, ULONG Flags, PHANDLE NewThreadHandle);
typedef NTSTATUS(NTAPI* lpNtTerminateProcess)(HANDLE ProcessHandle, NTSTATUS ExitStatus);
typedef NTSTATUS(NTAPI* lpNtTerminateThread)(HANDLE ProcessHandle, UINT ExitStatus);
typedef NTSTATUS(NTAPI* lpNtCreateDebugObject)(PHANDLE DebugObjectHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, ULONG Flags);
typedef NTSTATUS(NTAPI* lpNtDebugActiveProcess)(HANDLE ProcessHandle,HANDLE DebugObjectHandle);
typedef BOOL	(WINAPI* lpEndTask)(HWND, BOOL, BOOL);
typedef int		(WINAPI* lpWinStationTerminateProcess)(HANDLE hServer, DWORD dwProcessId, UINT uExitCode);
typedef NTSTATUS(NTAPI* lpNtSuspendProcess)(IN HANDLE ProcessHandle);
typedef NTSTATUS(NTAPI* lpNtQuerySystemInformation)(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
typedef NTSTATUS(NTAPI* lpNtDuplicateObject)(HANDLE SourceProcessHandle, HANDLE SourceHandle, HANDLE TargetProcessHandle, PHANDLE TargetHandle, ACCESS_MASK DesiredAccess, ULONG Attributes, ULONG Options);


extern std::string GetProcessName(HANDLE hProcess);
