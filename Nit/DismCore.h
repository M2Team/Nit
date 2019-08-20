/*
 * PROJECT:   Nit - A Windows Image Tweaker based on DismCore
 * FILE:      DismCore.h
 * PURPOSE:   Definition for the DismCore interfaces (10.0.18362)
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#ifndef DISM_CORE_H
#define DISM_CORE_H

#include <Windows.h>

#ifndef DEVPROPKEY_DEFINED
#define DEVPROPKEY_DEFINED

typedef GUID  DEVPROPGUID, *PDEVPROPGUID;
typedef ULONG DEVPROPID, *PDEVPROPID;

typedef struct _DEVPROPKEY {
    DEVPROPGUID fmtid;
    DEVPROPID   pid;
} DEVPROPKEY, *PDEVPROPKEY;

#endif // DEVPROPKEY_DEFINED

enum DISM_LOGLEVEL_TYPE {
    DismOutputSilent = 0,
    DismOutputFailure = 1,
    DismOutputWarning = 2,
    DismOutputInformation = 3,
    DismOutputDebug = 4
};

enum DISM_SESSION_STATE {
    DISM_SESSION_NONE = 0,
    DISM_SESSION_REBOOT_IMAGE_REQUIRED = 1,
    DISM_SESSION_RELOAD_SESSION_REQUIRED = 2
};

enum DISM_SESSION_TYPE {
    DISM_SESSION_TYPE_NONE = 0,
    DISM_SESSION_TYPE_LOCAL = 1,
    DISM_SESSION_TYPE_IMAGE = 2
};

enum DISM_IMAGE_FLAGS {
    DISM_IMAGE_FLAG_NONE = 0,
    DISM_IMAGE_FLAG_HELP = 1,
    DISM_IMAGE_FLAG_DOWNLEVEL = 2
};

enum DISM_DISPLAY_TYPE {
    DISM_DISPLAY_TYPE_NONE = 0,
    DISM_DISPLAY_TYPE_DEFAULT_LANG = 1
};

enum DISM_FORMAT_TYPE {
    DismFormatList = 0,
    DismFormatTable = 1,
    DismFormatXml = 2
};

enum DISM_OS_STATE {
    DISM_OS_STATE_INVALID = 0xffffffff,
    DISM_OS_STATE_OFFLINE = 0,
    DISM_OS_STATE_ONLINE = 1
};

enum DISM_REGISTRY {
    DISM_REGISTRY_UNKNOWN = 0,
    DISM_REGISTRY_SOFTWARE = 1,
    DISM_REGISTRY_SYSTEM = 2,
    DISM_REGISTRY_SECURITY = 3,
    DISM_REGISTRY_SAM = 4,
    DISM_REGISTRY_DEFAULT = 5,
    DISM_REGISTRY_HKCU = 6,
    DISM_REGISTRY_COMPONENTS = 7,
    DISM_REGISTRY_DRIVERS = 8
};

enum DISM_PROCESS_CHANGES_OPTION {
    DISM_PROCESS_CHANGES_OPTION_FORCE_REBOOT = 1,
    DISM_PROCESS_CHANGES_OPTION_SYNCHRONOUS_SCAVENGE = 2,
    DISM_PROCESS_CHANGES_OPTION_ENABLE_COMPRESSION = 4,
    DISM_PROCESS_CHANGES_OPTION_DISABLE_COMPRESSION = 8,
    DISM_PROCESS_CHANGES_OPTION_SCAN_HEALTH = 16,
    DISM_PROCESS_CHANGES_OPTION_RESTORE_HEALTH = 32,
    DISM_PROCESS_CHANGES_OPTION_PREVENT_PENDING = 64,
    DISM_PROCESS_CHANGES_OPTION_USE_LOCAL_SOURCE_ONLY = 128,
    DISM_PROCESS_CHANGES_OPTION_USE_WINDOWS_UPDATE = 256,
    DISM_PROCESS_CHANGES_OPTION_ASYNCHRONOUS_CLEANUP = 512,
    DISM_PROCESS_CHANGES_OPTION_SYNCHRONOUS_CLEANUP = 1024
};

enum DISM_FEATURE_OPERATION_OPTION {
    DISM_FEATURE_OPERATION_OPTION_DEFAULT = 0,
    DISM_FEATURE_OPERATION_OPTION_REMOVEPAYLOAD = 1
};

enum DISM_INSTALL_STATE {
    DISM_INSTALL_STATE_UNKNOWN = 0,
    DISM_INSTALL_STATE_NOTPRESENT = 1,
    DISM_INSTALL_STATE_UNINSTALLREQUESTED = 2,
    DISM_INSTALL_STATE_STAGED = 3,
    DISM_INSTALL_STATE_STAGING = 4,
    DISM_INSTALL_STATE_INSTALLED = 5,
    DISM_INSTALL_STATE_INSTALLREQUESTED = 6,
    DISM_INSTALL_STATE_SUPERSEDED = 7,
    DISM_INSTALL_STATE_PARTIALLYINSTALLED = 8,
    DISM_INSTALL_STATE_REMOVED = 9
};

enum DISM_PACKAGE_FULLY_OFFLINE_INSTALLABLE_STATE {
    DISM_PACKAGE_FULLY_OFFLINE_INSTALLABLE = 0,
    DISM_PACKAGE_FULLY_OFFLINE_NOT_INSTALLABLE = 1,
    DISM_PACKAGE_FULLY_OFFLINE_INSTALLABLE_UNDETERMINED = 2
};

enum DISM_PACKAGE_INSTALL_OPTION {
    DISM_PACKAGE_INSTALL_OPTION_DEFAULT = 0,
    DISM_PACKAGE_INSTALL_OPTION_DEFER = 1
};

enum DISM_ON_DEMAND_SOURCE_FILTER {
    DISM_ON_DEMAND_SOURCE_PACKAGE_STORE = 1,
    DISM_ON_DEMAND_SOURCE_LOCAL_SOURCE = 2,
    DISM_ON_DEMAND_SOURCE_CLOUD = 4
};

enum DISM_RESETBASE_OPTION {
    DISM_RESETBASE_OPTION_DEFAULT = 0,
    DISM_RESETBASE_OPTION_DEFER = 1
};

enum DISM_CONFIGURABLE_PROPERTY {
    DISM_COFIGURABLE_PROPERTY_NONE = 0,
    DISM_COFIGURABLE_PROPERTY_SOURCE_REPOSITORY_PATH = 1,
    DISM_COFIGURABLE_PROPERTY_TARGET_REPOSITORY_PATH = 2,
    DISM_COFIGURABLE_PROPERTY_INCLUDE_IMAGE_CAPABILITIES = 3,
    DISM_COFIGURABLE_PROPERTY_REPOSITORY_RECIPE_FILE = 4
};

enum DISM_CBS_OPERATION_TYPE {
    DISM_CBS_OPERATION_TYPE_NONE = 0,
    DISM_CBS_OPERATION_TYPE_EXPORT_REPOSITORY = 1
};

enum DRIVER_COLL_OUTPUT {
    DRIVER_COLL_ALL = 0,
    DRIVER_COLL_INBOX = 1,
    DRIVER_COLL_OUTOFBOX = 2
};

enum DRIVER_SIG_STATUS {
    DRIVER_SIG_UNKNOWN = 0,
    DRIVER_SIG_UNSIGNED = 1,
    DRIVER_SIG_SIGNED = 2
};

enum DRIVER_INSTALL_FLAGS {
    DRIVER_INSTALL_DEFAULT = 0,
    DRIVER_INSTALL_NOREFLECT = 1,
    DRIVER_INSTALL_HARDLINK = 2,
    DRIVER_INSTALL_FORCEREFLECT = 4
};

enum ProfileStatus {
    Disabled = 0,
    Enabled = 1,
    Applied = 2
};

enum DISM_FIRST_BOOT_PHASE {
    DismFirstBootPreSysPrep = 0,
    DismFirstBootPostSysPrep = 1,
    DismFirstBootPreOobe = 2,
    DismFirstBootPostOobe = 3,
    DismFirstBootOnError = 4
};

enum SYSPREP_RESEAL_TARGET {
    SYSPREP_RESEAL_AUDIT = 0,
    SYSPREP_RESEAL_OOBE = 1
};

enum DISM_ELEMENT_TYPE {
    DISM_ELEMENT_TYPE_INVALID = 0,
    DISM_ELEMENT_TYPE_APPLICATION = 1,
    DISM_ELEMENT_TYPE_DATAASSET = 2,
    DISM_ELEMENT_TYPE_MULTIVARIANT = 3,
    DISM_ELEMENT_TYPE_ICB = 4,
    DISM_ELEMENT_TYPE_DRIVER = 5,
    DISM_ELEMENT_TYPE_LANGPACK = 6,
    DISM_ELEMENT_TYPE_REGISTRY = 7,
    DISM_ELEMENT_TYPE_CERTIFICATE = 8,
    DISM_ELEMENT_TYPE_COMMONSETTINGS = 9,
    DISM_ELEMENT_TYPE_FEATURESONDEMAND = 10,
    DISM_ELEMENT_TYPE_OSUPDATE = 11
};

enum DISM_IMAGEINFO_MOUNT_FLAGS {
    DismImageInfoMountReadOnly = 1,
    DismImageInfoMountReadWrite = 2,
    DismImageInfoMountOptimize = 4,
    DismImageInfoMountCheckIntegrity = 8,
    DismImageInfoMountSupportEa = 16
};

enum DISM_MOUNTEDIMAGEINFO_STATUS_FLAGS {
    DismMountedImageInfoStatusValid = 0,
    DismMountedImageInfoStatusMounting = 1,
    DismMountedImageInfoStatusInvalid = 2,
    DismMountedImageInfoStatusNeedsRemount = 4
};

enum DISM_IMAGE_UNMOUNT_FLAGS {
    DismImageUnmountDiscard = 0,
    DismImageUnmountCommit = 1
};

enum DISM_IMAGE_SPLIT_FLAGS {
    DismImageSplitCheckIntegrity = 1
};

enum DISM_IMAGE_APPLY_FLAGS {
    DismImageApplyCheckIntegrity = 1
};

enum DISM_IMAGE_COMMIT_FLAGS {
    DismImageCommitMask = 0xffff0000,
    DismImageCommitAppend = 0x00010000,
    DismImageCommitGenerateIntegrity = 0x00020000,
    DismImageCommitNoDirAcl = 0x00040000,
    DismImageCommitNoFileAcl = 0x00080000,
    DismImageCommitNoRpFix = 0x00100000,
    DismImageCommitVerify = 0x00200000,
    DismImageCommitSupportEa = 0x00400000
};

#define DISMCORE_COM_INTERFACE_FWD_DEFINE(x) typedef interface x x

DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismLogger);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismProviderStore);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismObject);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismProvider);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismProviderCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismImage);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismImageSession);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismManagerPrivate);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismEventManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismError);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismErrorCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismImageManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismImageInfoCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismImageInfo);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismMountedImageInfoCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismMountedImageInfo);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismImageSessionPrivate);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismImageSessionPrivate2);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismConfigObject);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismConfiguration);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismConfiguration2);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismProperty);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPropertyCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismStringCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismProviderPrivate);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismServicingProvider);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismOSServiceManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismRegistry);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismItemManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismItemCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismSupportUnattend);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismSupportCommands);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismTokenCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismToken);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismCommandCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismCommand);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismHelpItemCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismHelpItem);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismProviderStorePrivate);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismHostManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPackage);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPackageFeature);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPackageFeatureCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPackage2);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPackage3);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPackage4);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPackageCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPackageFeature2);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismCapability);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismCapabilityCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPackageManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismMsuPackage);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPackageManager2);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPackageManager3);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismComponentStoreReport);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPackageManager4);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismPackageManager5);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismServicePackScavenge);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismXmlPackageManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismDriverManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismDriverPackageCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismDriverPackage);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismDriverCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismDriver);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismDeviceIdCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismDriverPackage2);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismDriverPackage3);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismDriverPackage4);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IUnattend);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IUnattendManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IUnattendSettings);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismCompatManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismIntlManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismIntl);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IPEImageManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(ITransmogrify);
DISMCORE_COM_INTERFACE_FWD_DEFINE(ITransmogrifyEditionCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(ITransmogrify2);
DISMCORE_COM_INTERFACE_FWD_DEFINE(ITransmogrify3);
DISMCORE_COM_INTERFACE_FWD_DEFINE(ITransmogrify4);
DISMCORE_COM_INTERFACE_FWD_DEFINE(ITransmogrify5);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismMsiManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismIBSSupport);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismAppxManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismAppxPackageInfoCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismAppxPackageInfo);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismAppxManager2);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismAppxManager3);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismAppxManager4);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismAppxManager5);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismAppxManager6);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismAppxPackageInfo2);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismAssocSupport);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismKcacheManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismSetMachineName);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismGenericManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismSysprep);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismSysprepManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismSetupPlatform);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismSetupPlatformManager);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismFfuProvider);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismFfuProvider2);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismProvisioningSupport);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismProvPackageElementCollection);
DISMCORE_COM_INTERFACE_FWD_DEFINE(IDismProvPackageElement);
DISMCORE_COM_INTERFACE_FWD_DEFINE(_IDismEvents);
DISMCORE_COM_INTERFACE_FWD_DEFINE(DismManager);

MIDL_INTERFACE("bd7fc505-69bc-49cb-8c58-1d1da547b7e2")
IDismLogger : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE Initialize(
        _In_ BSTR logFile,
        _In_ DISM_LOGLEVEL_TYPE logLevel) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteEntry(
        _In_ DISM_LOGLEVEL_TYPE level,
        _In_ LONG providerID,
        _In_ BSTR providerName,
        _In_ BSTR message) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_LogFilePath(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_MaxLevelLogged(
        _Out_ DISM_LOGLEVEL_TYPE* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Enabled(
        _Out_ VARIANT_BOOL* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_WdsCoreFilePath(
        _Out_ LPBSTR pVal) = 0;
};

MIDL_INTERFACE("d8066238-fdd5-4195-8155-2371e0e61cc5")
IDismObject : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE OnConnect(
        _In_ IUnknown * Parent,
        _In_ IDismLogger* DismLogger) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnDisconnect() = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Parent(
        _Out_ IUnknown** Parent) = 0;
};

MIDL_INTERFACE("33f93018-edb7-4092-978a-4b37d5dfb1c0")
IDismProvider : IDismObject
{
    virtual HRESULT STDMETHODCALLTYPE get_Name(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Version(
        _Out_ LPBSTR pVal) = 0;
};

MIDL_INTERFACE("722de115-1404-4bf8-bcee-de0b7b580671")
IDismProviderCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismProvider** ppProvider) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("9bb62bf9-01a0-49a7-be40-fb5445b29bef")
IDismImage : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_PhysicalLocation(
        _Out_ LPBSTR PhysicalLocation) = 0;
    virtual HRESULT STDMETHODCALLTYPE Save(
        _In_ ULONG ulFlags) = 0;
    virtual HRESULT STDMETHODCALLTYPE Discard() = 0;
    virtual HRESULT STDMETHODCALLTYPE get_IsClosed(
        _Out_ VARIANT_BOOL* IsClosed) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ProviderStorePath(
        _Out_ LPBSTR ProviderStorePath) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetProviderStorePath(
        _In_ BSTR ProviderStorePath) = 0;
    virtual HRESULT STDMETHODCALLTYPE Unmount(
        _In_ ULONG ulFlags) = 0;
    virtual HRESULT STDMETHODCALLTYPE ValidateStatus() = 0;
    virtual HRESULT STDMETHODCALLTYPE Remount() = 0;
};

MIDL_INTERFACE("5df7b1b0-93a5-4f03-8bcb-f39396899443")
IDismProviderStore : IDismObject
{
    virtual HRESULT STDMETHODCALLTYPE GetProvider(
        _In_ BSTR providerName,
        _Out_ IDismProvider** Provider) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetExternalProvider(
        _In_ BSTR providerName,
        _Out_ IDismProvider** Provider) = 0;
    virtual HRESULT STDMETHODCALLTYPE AddProvider(
        _In_ BSTR RelativeProviderLocation) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveProvider(
        _In_ BSTR providerName) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetProviderCollection(
        _Out_ IDismProviderCollection** Providers) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DismImage(
        _Out_ IDismImage** DismImage) = 0;
};

MIDL_INTERFACE("a08538a6-b9ff-47c2-b228-b93612ef114b")
IDismImageSession : IDismObject
{
    virtual HRESULT STDMETHODCALLTYPE get_ProviderStore(
        _Out_ IDismProviderStore** ProviderStore) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ImageState(
        _Out_ DISM_SESSION_STATE* State) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DismImage(
        _Out_ IDismImage** DismImage) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_SessionType(
        _Out_ DISM_SESSION_TYPE* SessionType) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_SessionVersion(
        _Out_ LPBSTR SessionVersion) = 0;
};

MIDL_INTERFACE("4d4fce31-f767-45e4-b2e9-5400e3e1389a")
IDismManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Logger(
        _Out_ IDismLogger** pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLocalProviderStore(
        _Out_ IDismProviderStore** ppUnknown) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateImageSession(
        _In_ IDismImage* pImage,
        _Out_ IDismImageSession** ppUnknown) = 0;
    virtual HRESULT STDMETHODCALLTYPE CloseImageSession(
        _In_ IDismImageSession* pImageSession,
        _Out_ DISM_SESSION_STATE* State) = 0;
    virtual HRESULT STDMETHODCALLTYPE Close() = 0;
    virtual HRESULT STDMETHODCALLTYPE ValidateDismTarget(
        _In_ IDismImage* pImage,
        _Out_ VARIANT_BOOL* pvbDismTargetValid) = 0;
};

MIDL_INTERFACE("8817cb2a-e80a-480b-a7b4-2232443a9e4b")
IDismManagerPrivate : IUnknown
{};

MIDL_INTERFACE("d9894135-1aa7-49af-ad11-27e784cac931")
IDismEventManager : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE OnInitiate(
        _In_ BSTR providerName,
        _In_ BSTR ActionName,
        _In_ IDismImage* Image,
        _In_ BSTR message,
        _Inout_ LPINT Response) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnError(
        _In_ BSTR providerName,
        _In_ BSTR ActionName,
        _In_ IDismImage* Image,
        _In_ SCODE errorCode,
        _In_ BSTR message,
        _Inout_ LPINT Response) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnProgress(
        _In_ BSTR providerName,
        _In_ BSTR ActionName,
        _In_ IDismImage* Image,
        _In_ LONG ticks,
        _In_ LONG totalTicks,
        _In_ BSTR message,
        _Inout_ LPINT Response) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnTerminate(
        _In_ BSTR providerName,
        _In_ BSTR ActionName,
        _In_ IDismImage* Image,
        _In_ BSTR message) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnCLIOutput(
        _In_ BSTR providerName,
        _In_ BSTR ActionName,
        _In_ IDismImage* Image,
        _In_ BSTR message,
        _Inout_ LPINT Response) = 0;
};

MIDL_INTERFACE("18caabcd-eb4c-4df3-9ff1-b5d24e655f33")
IDismError : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Hresult(
        _Out_ HRESULT * hr) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Source(
        _Out_ LPBSTR Source) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Description(
        _Out_ LPBSTR Description) = 0;
};

MIDL_INTERFACE("74d82b92-ae58-4962-9133-3701e1510a81")
IDismErrorCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismError** Error) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("7970de73-63e8-43e9-9f61-81bdc748f29b")
IDismImageInfo : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Name(
        _Out_ LPBSTR pbszName) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_Name(
        _In_ BSTR pbszName) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Description(
        _Out_ LPBSTR pbszDescription) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_Description(
        _In_ BSTR pbszDescription) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Index(
        _Out_ LPDWORD pulIndex) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetProperty(
        _In_ BSTR bszName,
        _Out_ LPBSTR pbszValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPropertyEx(
        _In_ BSTR bszName,
        _In_ ULONG ulIndex,
        _Out_ LPBSTR pbszValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetProperty(
        _In_ BSTR bszName,
        _In_ BSTR bszValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE Mount(
        _In_ BSTR bszMountPath,
        _In_ ULONG ulFlags,
        _Out_ IDismImage** ppDISMImage) = 0;
};

MIDL_INTERFACE("572f3374-7dae-47ab-88af-bd6ca29252fe")
IDismImageInfoCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismImageInfo** ppDismImageInfo) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("ee9b6624-8acc-440c-9564-f5d7568d6e01")
IDismMountedImageInfo : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_MountDir(
        _Out_ LPBSTR pbszMountDir) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ImagePath(
        _Out_ LPBSTR pbszImagePath) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Index(
        _Out_ LPDWORD pulIndex) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ReadWritable(
        _Out_ VARIANT_BOOL* pvbReadWritable) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Status(
        _Out_ LPDWORD pulStatus) = 0;
};

MIDL_INTERFACE("931fc35d-0976-48b8-8bde-b5954aadd3f2")
IDismMountedImageInfoCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismMountedImageInfo** ppDismMountedImageInfo) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("a654ff2f-d9fa-4df1-aabf-982c0a3c5f52")
IDismImageManager : IDismObject
{
    virtual HRESULT STDMETHODCALLTYPE GetImageInfoCollection(
        _In_ BSTR bszPath,
        _Out_ IDismImageInfoCollection** ppImageInfoCollection) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateDismImage(
        _In_ BSTR bszPath,
        _Out_ IDismImage** ppDISMImage) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetMountedImageInfoCollection(
        _Out_ IDismMountedImageInfoCollection** ppMountedImageInfoCollection) = 0;
    virtual HRESULT STDMETHODCALLTYPE CleanupMountpoints() = 0;
};

MIDL_INTERFACE("ff0e7a8b-7b7c-411b-968b-11aa898bb03a")
IDismImageSessionPrivate : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE put_HostManager(
        _In_ IUnknown * _arg1) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_ImageState(
        _In_ DISM_SESSION_STATE _arg1) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_DismImage(
        _In_ IDismImage* _arg1) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_SessionType(
        _In_ DISM_SESSION_TYPE _arg1) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReleaseHostManager() = 0;
};

MIDL_INTERFACE("13a81f76-50e2-11e0-b744-00123f3fc6dd")
IDismImageSessionPrivate2 : IDismImageSessionPrivate
{
    virtual HRESULT STDMETHODCALLTYPE get_CanUnloadClient(
        _Out_ VARIANT_BOOL * pvbCanUnloadClient) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_CanUnloadClient(
        _In_ VARIANT_BOOL pvbCanUnloadClient) = 0;
};

MIDL_INTERFACE("705a28e2-a26f-4a43-8d82-a56941b5c250")
IDismConfigObject : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_MaxProgress(
        _Out_ LPINT MaxTick) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_MaxProgress(
        _In_ int MaxTick) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ScratchDir(
        _Out_ LPBSTR DirectoryPath) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_ScratchDir(
        _In_ BSTR DirectoryPath) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_logFile(
        _Out_ LPBSTR logFile) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_logFile(
        _In_ BSTR logFile) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_logLevel(
        _Out_ DISM_LOGLEVEL_TYPE* logLevel) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_logLevel(
        _In_ DISM_LOGLEVEL_TYPE logLevel) = 0;
};

MIDL_INTERFACE("3f92cf95-462b-4bda-a1d4-b6cb74cefb5c")
IDismConfiguration : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE get_MaxProgress(
        _Out_ LPINT MaxTick) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_MaxProgress(
        _In_ int MaxTick) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ScratchDir(
        _Out_ LPBSTR DirectoryPath) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_ScratchDir(
        _In_ BSTR DirectoryPath) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_logFile(
        _Out_ LPBSTR logFile) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_logFile(
        _In_ BSTR logFile) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_logLevel(
        _Out_ DISM_LOGLEVEL_TYPE* logLevel) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_logLevel(
        _In_ DISM_LOGLEVEL_TYPE logLevel) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ImageFlags(
        _Out_ DISM_IMAGE_FLAGS* ImageFlags) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_ImageFlags(
        _In_ DISM_IMAGE_FLAGS ImageFlags) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Display(
        _Out_ DISM_DISPLAY_TYPE* Display) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_Display(
        _In_ DISM_DISPLAY_TYPE Display) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Format(
        _Out_ DISM_FORMAT_TYPE* Format) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_Format(
        _In_ DISM_FORMAT_TYPE Format) = 0;
};

MIDL_INTERFACE("59891552-9ac8-4c89-b2a7-c64b3962354e")
IDismConfiguration2 : IDismConfiguration
{
    virtual HRESULT STDMETHODCALLTYPE get_ExternalStack(
        _Out_ LPBSTR StackPath) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_ExternalStack(
        _In_ BSTR StackPath) = 0;
};

MIDL_INTERFACE("20599df3-1a1b-4eaa-a53f-77f39438b542")
IDismProperty : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE get_Name(
        _Out_ LPBSTR Name) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Path(
        _Out_ LPBSTR Path) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Value(
        _Out_ LPBSTR Value) = 0;
};

MIDL_INTERFACE("88d4518a-41a8-49b0-a087-a3d6fc1d298e")
IDismPropertyCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismProperty** ppProperty) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("7e6eb080-3384-4155-a4a7-5e43bbafb2f3")
IDismStringCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ LPBSTR pValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("2114daec-42fd-4847-8a04-5f2dde3276a3")
IDismProviderPrivate : IDismObject
{};

MIDL_INTERFACE("8e1c59a6-272f-4212-a6d3-8dc6cddf813a")
IDismRegistry : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE GetKeyPath(
        _In_ DISM_REGISTRY RegHive,
        _Out_ LPBSTR KeyPath) = 0;
};

MIDL_INTERFACE("0b49a069-56a5-48eb-9d9f-e9e42478149a")
IDismOSServiceManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_State(
        _Out_ DISM_OS_STATE * State) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_OSVersion(
        _Out_ LPBSTR Version) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Architecture(
        _Out_ LPLONG Architecture) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DismImage(
        _Out_ IDismImage** DismImage) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ServicingStackDirectory(
        _Out_ LPBSTR ServicingStackDirectory) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_WindowsDirectory(
        _Out_ LPBSTR WindowsDirectory) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ProgramFilesDirectory(
        _Out_ LPBSTR ProgramFilesDirectory) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_BootDrive(
        _Out_ LPBSTR BootDrive) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_SystemPath(
        _Out_ LPBSTR SystemPath) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetWindowsDirectory(
        _In_ BSTR WindowsDirectory) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetSystemPath(
        _In_ BSTR SystemPath) = 0;
    virtual HRESULT STDMETHODCALLTYPE MountRegistry(
        _Out_ IDismRegistry** DismRegistry) = 0;
};

MIDL_INTERFACE("c8292388-8d1d-4c34-9a41-8763fcd304f7")
IDismServicingProvider : IDismObject
{
    virtual HRESULT STDMETHODCALLTYPE Initialize(
        _In_ IDismOSServiceManager* OSServiceManager,
        _In_ BSTR ScratchDirectory) = 0;
    virtual HRESULT STDMETHODCALLTYPE Finalize() = 0;
    virtual HRESULT STDMETHODCALLTYPE ValidateServiceability() = 0;
};

MIDL_INTERFACE("e2c60889-8e0f-4a21-9f5b-e80f0b13c8d8")
IDismItemCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG Count) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IUnknown** Item) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** Enum) = 0;
};

MIDL_INTERFACE("f316031b-3c98-4f20-b9cb-a3b8702dddd8")
IDismItemManager : IDismServicingProvider
{
    virtual HRESULT STDMETHODCALLTYPE AddItem(
        _In_ BSTR ItemLocation) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveItemByName(
        _In_ BSTR ItemName) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveItem(
        _In_ IUnknown* Item) = 0;
    virtual HRESULT STDMETHODCALLTYPE OpenItem(
        _In_ BSTR ItemLocation,
        _Out_ IUnknown** Item) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetItems(
        _Out_ IDismItemCollection** Item) = 0;
};

MIDL_INTERFACE("dadad7bb-2b1d-4edb-9014-b1912e1a2e3d")
IDismSupportUnattend : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE Apply(
        _In_ BSTR UnattendPath) = 0;
};

MIDL_INTERFACE("5bed6686-eeb7-40b7-97f9-fabcc5efaec2")
IDismToken : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Option(
        _Out_ LPBSTR Option) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Argument(
        _Out_ LPBSTR Argument) = 0;
};

MIDL_INTERFACE("407c0f0f-0540-4900-afd2-52925246dcb3")
IDismTokenCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismToken** ppToken) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("c112610a-5d2a-4d52-8082-edcd5a5eb11d")
IDismCommand : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE get_Name(
        _Out_ LPBSTR pbstrName) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_providerName(
        _Out_ LPBSTR pbstrProviderName) = 0;
};

MIDL_INTERFACE("8f609999-5d3c-48a8-aadf-5f96cb297279")
IDismCommandCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismCommand** ppToken) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("e3f237d6-8f3c-4443-925a-abf44807690c")
IDismHelpItem : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE get_IsCategory(
        _Out_ VARIANT_BOOL * pvbCategory) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TopicCategory(
        _Out_ LPBSTR pbstrTopicCategory) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TopicName(
        _Out_ LPBSTR pbstrTopicName) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TopicDescription(
        _Out_ LPBSTR pbstrTopicDescription) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_HeaderText(
        _Out_ LPBSTR pbstrHeaderText) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_FooterText(
        _Out_ LPBSTR pbstrFooterText) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_providerName(
        _Out_ LPBSTR pbstrProviderName) = 0;
};

MIDL_INTERFACE("14e23a62-cd9f-4e41-abd5-58d534bf374e")
IDismHelpItemCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismHelpItem** ppToken) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("7234ec82-c68b-4c4e-ba1d-d333f5b87b37")
IDismSupportCommands : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE ExecuteCmdLine(
        _In_ IDismTokenCollection* Tokens) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCommandCollection(
        _Out_ IDismCommandCollection** ppCommandCollection) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHelpItemCollection(
        _Out_ IDismHelpItemCollection** ppHelpItemCollection) = 0;
};

MIDL_INTERFACE("fe104c14-65e6-43b3-aa56-f07b5e57d81f")
IDismProviderStorePrivate : IDismObject
{
    virtual HRESULT STDMETHODCALLTYPE Initialize(
        _In_ IDismProviderStorePrivate* pStore) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetProviderLocation(
        _In_ BSTR providerName,
        _Out_ LPBSTR ProviderLocation) = 0;
    virtual HRESULT STDMETHODCALLTYPE RegisterProvider(
        _In_ IDismProvider* Provider) = 0;
};

MIDL_INTERFACE("c9e39092-53df-42b6-89b4-fba12662ead1")
IDismHostManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE CreateObjectFromDLL(
        _In_ BSTR DLLPath,
        _In_ GUID * ClassIdentifier,
        _In_ GUID * riid,
        _Out_ IUnknown * *ppUnknown) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_ExtendedLogInfo(
        _In_ BSTR WdsCorePath,
        _In_ BSTR LogPath,
        _In_ DISM_LOGLEVEL_TYPE _arg3) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetSynchronizationContext(
        _In_ BSTR SyncObjectName,
        _In_ HANDLE ProcessHandle) = 0;
};

MIDL_INTERFACE("6a7dee63-129c-41e8-b6a4-bd1eb1d85e65")
IDismCapability : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Name(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Id(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_State(
        _Out_ DISM_INSTALL_STATE* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DownloadSize(
        _Out_ LPDWORD pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InstallSize(
        _Out_ LPDWORD pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DisplayName(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Description(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE Install() = 0;
    virtual HRESULT STDMETHODCALLTYPE Uninstall() = 0;
};

MIDL_INTERFACE("9b4784da-e322-4bf0-800b-28bae357ec71")
IDismCapabilityCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismCapability** ppCapability) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("f8ef44d0-94f1-4dfb-bad8-7b13830ba06e")
IDismMsuPackage : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Path(
        _Out_ LPBSTR Path) = 0;
    virtual HRESULT STDMETHODCALLTYPE Install() = 0;
};

MIDL_INTERFACE("46ce4564-e88a-4eae-bd7c-da221f61e0c7")
IDismComponentStoreReport : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_WinSxSVolatileSize(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_WinSxSNonVolatileSize(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_WinSxSSharedWithWindowsSize(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_WinSxSAccordingToExplorerSize(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_LastScavengeDateTime(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_SupersededPackageCount(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ComponentCleanupRecommended(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ActualSize(
        _Out_ LPBSTR pVal) = 0;
};

MIDL_INTERFACE("a457b170-c9c3-499b-85bc-59259e92ae4d")
IDismServicePackScavenge : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE SPScavenge(
        _In_ VARIANT_BOOL bHideSpSpecified) = 0;
    virtual HRESULT STDMETHODCALLTYPE SPScavengeable(
        _Out_ VARIANT_BOOL* bScavenge,
        _Out_ LPLONG plNumberOfSupersededSP) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsSystemPending(
        _Out_ VARIANT_BOOL* pbPending) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCurrentEdition(
        _Out_ LPBSTR pszCurrentEdition,
        _Out_ LPLONG plEditionsCount) = 0;
};

MIDL_INTERFACE("c1817aef-4351-4721-9a62-5deda9da246c")
IDismXmlPackageManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE CheckXmlPackageApplicability(
        _In_ BSTR MetadataBlob,
        _Out_ VARIANT_BOOL * bApplicable,
        _Out_ VARIANT_BOOL * bInstalled) = 0;
};

MIDL_INTERFACE("a3ccf844-82c9-431e-8d29-5bd09f8c9b70")
IDismDeviceIdCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("e7627187-c11b-42fc-91db-4f46bd545374")
IUnattend : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Path(
        _Out_ LPBSTR pVal) = 0;
};

MIDL_INTERFACE("517cdc5a-7a77-4356-81ca-d39f50658726")
IUnattendManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE OpenUnattend(
        _In_ BSTR UnattendPath,
        _Out_ IUnattend** ppUnattend) = 0;
    virtual HRESULT STDMETHODCALLTYPE Validate(
        _In_ IUnattend* pUnattend) = 0;
    virtual HRESULT STDMETHODCALLTYPE Apply(
        _In_ IUnattend* pUnattend,
        _In_ ULONG Flags) = 0;
};

MIDL_INTERFACE("f8a8aef3-1322-4e1d-bbf1-0a228c6fe193")
IUnattendSettings : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Settings(
        _Out_ LPDWORD pSettings) = 0;
};

MIDL_INTERFACE("092187e4-09b8-46a3-87d5-1c9bc6ed8b5d")
IDismCompatManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE CompatExecuteCmdLine(
        _In_ IDismTokenCollection* Tokens,
        _In_ BSTR bszPkgMgrExe,
        _In_ BSTR bszImagePath,
        _In_ BSTR bszWindir,
        _In_ VARIANT_BOOL vbQuiet,
        _In_ VARIANT_BOOL vbNoRestart) = 0;
};

MIDL_INTERFACE("eced57df-5d67-440a-a3cd-b4f1b8b39d1a")
IDismIntlManager : IDispatch
{};

MIDL_INTERFACE("574fd8f3-bc64-455e-a0d8-bcf95d8819b1")
IDismIntl : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE GetEffectiveSystemUILanguage(
        _Out_ LPBSTR pbstrLanguage) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetInstallLanguage(
        _Out_ LPBSTR pbstrLanguage) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetAllIntlSettings(
        _In_ BSTR bstrLanguage) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetSkuIntlDefaults(
        _In_ BSTR bstrLanguage) = 0;
};

MIDL_INTERFACE("30eb8593-fa91-4948-adf5-dc6a354f1873")
IPEImageManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_ScratchSpace(
        _Out_ LPDWORD pulScratchSpace) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_ScratchSpace(
        _In_ ULONG pulScratchSpace) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InstallRoot(
        _Out_ LPBSTR pInstallRoot) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_InstallRoot(
        _In_ BSTR pInstallRoot) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Profiling(
        _Out_ ProfileStatus* pStatus) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_Profiling(
        _In_ ProfileStatus pStatus) = 0;
    virtual HRESULT STDMETHODCALLTYPE Prep() = 0;
    virtual HRESULT STDMETHODCALLTYPE ApplyProfile(
        _In_ BSTR Profiles) = 0;
};

MIDL_INTERFACE("b629eef0-2060-43a0-a180-7d04351c3903")
ITransmogrifyEditionCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG plCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG lIndex,
        _Out_ LPBSTR pbstrEditionId) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Index(
        _In_ BSTR bstrEditionId,
        _Out_ LPLONG plCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE Add(
        _In_ BSTR bstrEditionId) = 0;
};

MIDL_INTERFACE("05bd25af-f5d6-4246-9f91-94b187bc2bf4")
ITransmogrify : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE CanTransmogrify(
        _In_ BSTR EditionId,
        _In_ BSTR ProductKey) = 0;
    virtual HRESULT STDMETHODCALLTYPE DoTransmogrify(
        _In_ BSTR EditionId,
        _In_ BSTR ProductKey) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCurrentEdition(
        _Out_ LPBSTR pEditionId) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateEditionCollection(
        _Out_ ITransmogrifyEditionCollection** ppEditionCollection) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetTargetEditions(
        _Out_ ITransmogrifyEditionCollection** ppTargetEditions) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetStagedEditions(
        _Out_ ITransmogrifyEditionCollection** ppStagedEditions) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveStagedEditions(
        _In_ ITransmogrifyEditionCollection* pStagedEditions) = 0;
};

MIDL_INTERFACE("c6abc167-33b8-4a00-be58-12ec5b013598")
ITransmogrify2 : ITransmogrify
{
    virtual HRESULT STDMETHODCALLTYPE SetProductKey(
        _In_ BSTR ProductKey) = 0;
};

MIDL_INTERFACE("50f2ddd7-90ed-4db1-8eba-3c995486aa0c")
ITransmogrify3 : ITransmogrify2
{
    virtual HRESULT STDMETHODCALLTYPE GetProductKeyInfo(
        _In_ BSTR ProductKey,
        _Out_ LPBSTR pEditionId,
        _Out_ LPBSTR pProductKeyType) = 0;
    virtual HRESULT STDMETHODCALLTYPE ValidateProductKey(
        _In_ BSTR ProductKey) = 0;
};

MIDL_INTERFACE("e6757b45-14fc-42c1-9943-ac630a8b0e1b")
ITransmogrify4 : ITransmogrify3
{
    virtual HRESULT STDMETHODCALLTYPE DoFootprintCleanup() = 0;
};

MIDL_INTERFACE("3f483e2e-f737-481e-a3a5-c64e53e493ae")
ITransmogrify5 : ITransmogrify4
{
    virtual HRESULT STDMETHODCALLTYPE DoTransmogrify2(
        _In_ BSTR EditionId,
        _In_ BSTR Channel) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetTargetCompositionEditions(
        _Out_ ITransmogrifyEditionCollection** ppTargetCompositionEditions) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetTargetVirtualEditions(
        _In_ BSTR TargetCompositionEdition,
        _Out_ ITransmogrifyEditionCollection** ppTargetVirtualEditions) = 0;
};

MIDL_INTERFACE("57ab0039-5829-4ebf-b093-0b3a80867c24")
IDismMsiManager : IDispatch
{};

MIDL_INTERFACE("e1b47f29-955c-49a5-b0a5-5018207fde69")
IDismIBSSupport : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE SetFirstBootCommandLine(
        _In_ DISM_FIRST_BOOT_PHASE phase,
        _In_ BSTR commandLine) = 0;
};

MIDL_INTERFACE("40e0e56d-3d1d-4f5c-b325-0121423677c6")
IDismAppxPackageInfo : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Name(
        _Out_ LPBSTR Value) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_PublisherId(
        _Out_ LPBSTR Value) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Version(
        _Out_ unsigned __int64* Value) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Architecture(
        _Out_ LPDWORD Value) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ResourceId(
        _Out_ LPBSTR Value) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_PackageMoniker(
        _Out_ LPBSTR Value) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InstallLocation(
        _Out_ LPBSTR Value) = 0;
};

MIDL_INTERFACE("75cae0d9-716d-4e62-931d-bdfa3dbbd42e")
IDismAppxPackageInfoCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG Count) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismAppxPackageInfo** appxPackageInfo) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** newEnum) = 0;
};

MIDL_INTERFACE("54103f66-7cb7-41ba-ad1a-2e124d01c08c")
IDismAppxManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE AddAllUserAppxLooseFiles(
        _In_ BSTR appPath,
        _In_ BSTR customDataPath,
        _In_ VARIANT_BOOL skipLicenseCheck) = 0;
    virtual HRESULT STDMETHODCALLTYPE AddAllUserAppxPackage(
        _In_ BSTR mainPackagePath,
        _In_ IDismStringCollection* dependencyPackagePaths,
        _In_ BSTR customDataPath,
        _In_ BSTR licensePath,
        _In_ VARIANT_BOOL skipLicenseCheck) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAllUserAppx(
        _Out_ IDismAppxPackageInfoCollection** packageCollection) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveAllUserAppx(
        _In_ BSTR mainPackageMoniker) = 0;
};

MIDL_INTERFACE("80f63941-9464-4495-bac3-51a49b20ce80")
IDismAppxManager2 : IDismAppxManager
{
    virtual HRESULT STDMETHODCALLTYPE AddProvisionedAppxDataFile(
        _In_ BSTR customDataPath,
        _In_ BSTR packageFullName) = 0;
};

MIDL_INTERFACE("6674fe44-de28-4b7e-978d-ab6a51ce0a5b")
IDismAppxManager3 : IDismAppxManager2
{
    virtual HRESULT STDMETHODCALLTYPE AddAllUserAppxPackageWithOptionalPackages(
        _In_ BSTR mainPackagePath,
        _In_ IDismStringCollection* dependencyPackagePaths,
        _In_ IDismStringCollection* optionalPackagePaths,
        _In_ BSTR customDataPath,
        _In_ IDismStringCollection* licensePaths,
        _In_ VARIANT_BOOL skipLicenseCheck) = 0;
};

MIDL_INTERFACE("7446ff66-999d-497a-b712-a26059730c5d")
IDismAppxManager4 : IDismAppxManager3
{
    virtual HRESULT STDMETHODCALLTYPE OptimizeAllUserAppx() = 0;
};

MIDL_INTERFACE("9b31f137-5668-441f-8bc3-d21f8d3c70db")
IDismAppxManager5 : IDismAppxManager4
{
    virtual HRESULT STDMETHODCALLTYPE AddAllUserAppxPackageWithOptionalPackagesAndRegion(
        _In_ BSTR mainPackagePath,
        _In_ IDismStringCollection* dependencyPackagePaths,
        _In_ IDismStringCollection* optionalPackagePaths,
        _In_ BSTR customDataPath,
        _In_ IDismStringCollection* licensePaths,
        _In_ VARIANT_BOOL skipLicenseCheck,
        _In_ BSTR region) = 0;
};

MIDL_INTERFACE("b83f048e-2309-4e52-965b-4ff56b3f5ab7")
IDismAppxManager6 : IDismAppxManager5
{
    virtual HRESULT STDMETHODCALLTYPE AddAppxUninstallBlocklistEntry(
        _In_ BSTR packageFamilyName) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveAppxUninstallBlocklistEntry(
        _In_ BSTR packageFamilyName) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetNonRemovableAppPolicy(
        _Out_ IDismStringCollection** packageFamilyNames) = 0;
};

MIDL_INTERFACE("5367d43f-aa59-4a81-9068-36c7a360fe72")
IDismAppxPackageInfo2 : IDismAppxPackageInfo
{
    virtual HRESULT STDMETHODCALLTYPE get_Regions(
        _Out_ LPBSTR Value) = 0;
};

MIDL_INTERFACE("dced1a68-143b-4c8c-8451-f155bde821db")
IDismAssocSupport : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE ExportAppAssoc(
        _In_ BSTR bstrFile) = 0;
    virtual HRESULT STDMETHODCALLTYPE ImportAppAssoc(
        _In_ BSTR bstrFile) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAppAssoc(
        _Out_ LPBSTR pbstrOut) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveAppAssoc() = 0;
};

MIDL_INTERFACE("ed08f15d-d5f2-40bf-b08b-73a8456ca62d")
IDismKcacheManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE GetStringValue(
        _In_ BSTR bstrName,
        _Out_ LPBSTR pbstrVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDwordValue(
        _In_ BSTR bstrName,
        _Out_ LPDWORD pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetBinaryValue(
        _In_ BSTR bstrName,
        _Out_ SAFEARRAY** pbVal) = 0;
};

MIDL_INTERFACE("16fcf50b-1eb0-4ed3-9a5c-8a33c80f0d83")
IDismSetMachineName : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE SetMachineName(
        _In_ BSTR machineName) = 0;
};

MIDL_INTERFACE("0ab3ac34-c644-4903-aa5f-5b5d8f15d969")
IDismGenericManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE Run(
        _In_ IDismTokenCollection* Tokens) = 0;
};

MIDL_INTERFACE("db46267b-2fea-4bb5-9f14-e4e582e4777f")
IDismSysprep : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE Stage() = 0;
    virtual HRESULT STDMETHODCALLTYPE Cleanup(
        _In_ SYSPREP_RESEAL_TARGET Reseal,
        _In_ BSTR Mode) = 0;
    virtual HRESULT STDMETHODCALLTYPE Generalize(
        _In_ SYSPREP_RESEAL_TARGET Reseal,
        _In_ BSTR Mode,
        _In_ BSTR UnattendPath) = 0;
    virtual HRESULT STDMETHODCALLTYPE Specialize(
        _In_ BSTR UnattendPath) = 0;
};

MIDL_INTERFACE("8e65fbac-20ad-4075-bd7b-377112ff1f69")
IDismSysprepManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE Run(
        _In_ IDismTokenCollection* Tokens) = 0;
};

MIDL_INTERFACE("53219e4f-44c6-4ed4-83f9-065fdee95795")
IDismSetupPlatform : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE InitiateUninstall(
        _In_ ULONG UninstallReason) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveUninstall() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetUninstallWindow(
        _Out_ LPDWORD UninstallWindow) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetUninstallWindow(
        _In_ ULONG UninstallWindow) = 0;
};

MIDL_INTERFACE("aa668c08-675c-49cb-a127-2cd57a173c04")
IDismSetupPlatformManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE Run(
        _In_ IDismTokenCollection* Tokens) = 0;
};

MIDL_INTERFACE("924d876e-17f7-4a5e-ac33-918c2bea0fd5")
IDismFfuProvider : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE ApplyImage(
        _In_ BSTR ImagePath,
        _In_ BSTR ApplyPath,
        _In_ BSTR PartPath) = 0;
    virtual HRESULT STDMETHODCALLTYPE SplitImage(
        _In_ BSTR ImagePath,
        _In_ BSTR PartPath,
        _In_ unsigned __int64 PartSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE CaptureImage(
        _In_ BSTR CapturePath,
        _In_ BSTR ImagePath,
        _In_ BSTR Name,
        _In_ BSTR Description,
        _In_ BSTR PlatformIds,
        _In_ unsigned short CompressionAlgorithm) = 0;
};

MIDL_INTERFACE("a192440b-5e7b-4538-a48b-3dfe9e043086")
IDismFfuProvider2 : IDismFfuProvider
{
    virtual HRESULT STDMETHODCALLTYPE OptimizeImage(
        _In_ BSTR ImagePath,
        _In_ int PartitionNumber) = 0;
};

MIDL_INTERFACE("f7a983d3-e787-40a8-9572-cd04ff99d001")
IDismProvPackageElement : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Type(
        _Out_ DISM_ELEMENT_TYPE * pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDismPropertyCollection(
        _Out_ IDismPropertyCollection** PropertyCollection) = 0;
};

MIDL_INTERFACE("3ca3a1b2-ff8c-487a-a0bc-95327ccabd7b")
IDismProvPackageElementCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismProvPackageElement** ppPackage) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("446e7afc-05e3-4eea-9e28-d8d6b324a8c7")
IDismProvisioningSupport : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE GetElementCollection(
        _In_ BSTR bstrPackagePath,
        _Out_ IDismProvPackageElementCollection** ppElementCollection) = 0;
    virtual HRESULT STDMETHODCALLTYPE ApplyProvPackage(
        _In_ BSTR bstrPackagePath) = 0;
};

MIDL_INTERFACE("a0efd55a-17a8-413d-a53a-3c43aef96c0e")
_IDismEvents : IDispatch
{};

MIDL_INTERFACE("be33a527-42fe-43a4-ac5e-c1d059fc705f")
DismManager;
// interface IDismConfiguration
// interface IDismConfiguration2
// [ default ] interface IDismManager
// interface IDismManagerPrivate
// interface IDismEventManager
// [ default, source ] dispinterface _IDismEvents

MIDL_INTERFACE("dee9a285-b7f6-4c16-b651-7a13af90f42c")
IDismPackage : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Identity(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ReleaseType(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_State(
        _Out_ DISM_INSTALL_STATE* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Name(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Description(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_RestartRequired(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ProductName(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ProductVersion(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Company(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Copyright(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_SupportInformation(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_CreationTime(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_LastUpdateTime(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InstallTime(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InstallPackageName(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InstallLocation(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InstallClient(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InstallUserName(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_IsApplicable(
        _Out_ VARIANT_BOOL* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE Install() = 0;
    virtual HRESULT STDMETHODCALLTYPE Remove() = 0;
    virtual HRESULT STDMETHODCALLTYPE Stage() = 0;
    virtual HRESULT STDMETHODCALLTYPE OpenFeature(
        _In_ BSTR UpdateName,
        _Out_ IDismPackageFeature** PackageFeature) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPackageFeatureCollection(
        _Out_ IDismPackageFeatureCollection** ppPackageFeatures) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDismPropertyCollection(
        _Out_ IDismPropertyCollection** PropertyCollection) = 0;
};

MIDL_INTERFACE("5f81a121-c3c5-4e94-b7ae-22b532f9f3a8")
IDismPackage2 : IDismPackage
{
    virtual HRESULT STDMETHODCALLTYPE get_FullyOfflineInstallable(
        _Out_ DISM_PACKAGE_FULLY_OFFLINE_INSTALLABLE_STATE * pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ScavengeSequence(
        _Out_ LPUINT piScavengeSequence) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_CreationFileTime(
        _Out_ LPBSTR pFileTime) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_LastUpdateFileTime(
        _Out_ LPBSTR pFileTime) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InstallFileTime(
        _Out_ LPBSTR pFileTime) = 0;
};

MIDL_INTERFACE("ec622249-ba0c-405b-a83c-4a31bc7f0c4d")
IDismPackage3 : IDismPackage2
{
    virtual HRESULT STDMETHODCALLTYPE get_CapabilityId(
        _Out_ LPBSTR pId) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_IsSSU(
        _Out_ VARIANT_BOOL* pVal) = 0;
};

MIDL_INTERFACE("a27155b5-6831-4427-b0d0-972d58f20263")
IDismPackage4 : IDismPackage3
{
    virtual HRESULT STDMETHODCALLTYPE InstallEx(
        _In_ DISM_PACKAGE_INSTALL_OPTION InstallOption) = 0;
};

MIDL_INTERFACE("d80d838a-0d57-428c-b898-265a7d244a67")
IDismPackageCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismPackage** ppPackage) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("3bc3752e-74c5-46f0-b50c-12ff269d5bb1")
IDismPackageFeature : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Name(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_State(
        _Out_ DISM_INSTALL_STATE* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DisplayName(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Description(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DisplayFile(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Restart(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_PsfName(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DownloadSize(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_SetMembership(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_RawParentPackage(
        _Out_ IDismPackage** pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE Enable() = 0;
    virtual HRESULT STDMETHODCALLTYPE Disable() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetParentFeatureCollection(
        _Out_ IDismPackageFeatureCollection** pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDismPropertyCollection(
        _Out_ IDismPropertyCollection** PropertyCollection) = 0;
};

MIDL_INTERFACE("63e0af82-28e4-4145-ae9a-aadd715c1251")
IDismPackageFeatureCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismPackageFeature** ppDriver) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("1c551557-21c2-4907-be82-35d7eef5bb13")
IDismPackageFeature2 : IDismPackageFeature
{
    virtual HRESULT STDMETHODCALLTYPE EnableAll() = 0;
    virtual HRESULT STDMETHODCALLTYPE DisableEx(
        _In_ unsigned int Options) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_CustomPropertyXMLBlob(
        _Out_ LPBSTR pVal) = 0;
};

MIDL_INTERFACE("1754aa4f-f0e2-4692-a802-d51871085f34")
IDismPackageManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_IsCompressionEnabled(
        _Out_ VARIANT_BOOL * pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_IsImageUnserviceable(
        _Out_ VARIANT_BOOL* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE OpenPackageByPath(
        _In_ BSTR PackageLocation,
        _Out_ IDismPackage** Package) = 0;
    virtual HRESULT STDMETHODCALLTYPE OpenPackageByName(
        _In_ BSTR PackageName,
        _Out_ IDismPackage** Package) = 0;
    virtual HRESULT STDMETHODCALLTYPE OpenFoundationPackage(
        _Out_ IDismPackage** Package) = 0;
    virtual HRESULT STDMETHODCALLTYPE OpenMsuPackage(
        _In_ BSTR PackageLocation,
        _Out_ IDismMsuPackage** Package) = 0;
    virtual HRESULT STDMETHODCALLTYPE OpenPublicFeature(
        _In_ BSTR FeatureName,
        _Out_ IDismPackageFeature** Feature) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPackageCollection(
        _Out_ IDismPackageCollection** ppPackages) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPublicFeatureCollection(
        _Out_ IDismPackageFeatureCollection** ppPackageFeatures) = 0;
    virtual HRESULT STDMETHODCALLTYPE ProcessChanges() = 0;
    virtual HRESULT STDMETHODCALLTYPE ProcessChangesWithOptions(
        _In_ unsigned int Options) = 0;
    virtual HRESULT STDMETHODCALLTYPE ProcessChangesPostReboot() = 0;
    virtual HRESULT STDMETHODCALLTYPE Scavenge() = 0;
    virtual HRESULT STDMETHODCALLTYPE RevertPendingActions() = 0;
};

MIDL_INTERFACE("5cc9ed95-cf41-4685-beaa-70531031fec4")
IDismPackageManager2 : IDismPackageManager
{
    virtual HRESULT STDMETHODCALLTYPE get_IsStoreCorrupt(
        _Out_ VARIANT_BOOL * pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_LastCBSSessionID(
        _Out_ LPBSTR pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_CBSSessionState(
        _In_ BSTR Id,
        _Out_ VARIANT_BOOL* Completed,
        _Out_ HRESULT* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_CleanupSize(
        _Out_ LPDWORD pulSizeInMB) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_PreventPending(
        _In_ VARIANT_BOOL _arg1) = 0;
    virtual HRESULT STDMETHODCALLTYPE AddSourcePath(
        BSTR Path) = 0;
    virtual HRESULT STDMETHODCALLTYPE ClearSourcePaths() = 0;
    virtual HRESULT STDMETHODCALLTYPE ScanHealth() = 0;
    virtual HRESULT STDMETHODCALLTYPE RestoreHealth(
        _In_ VARIANT_BOOL LimitAccess) = 0;
    virtual HRESULT STDMETHODCALLTYPE StartComponentCleanup(
        _In_ VARIANT_BOOL ResetBase) = 0;
    virtual HRESULT STDMETHODCALLTYPE WindowsUpdateCleanup() = 0;
};

MIDL_INTERFACE("e40dd35a-89e1-4619-ad21-ef97d29b5d00")
IDismPackageManager3 : IDismPackageManager2
{
    virtual HRESULT STDMETHODCALLTYPE AnalyzeComponentStore(
        _Out_ IDismComponentStoreReport** ppReport) = 0;
};

MIDL_INTERFACE("4dfd752a-1e5e-4c70-94ff-9c91b5f61335")
IDismPackageManager4 : IDismPackageManager3
{
    virtual HRESULT STDMETHODCALLTYPE GetCapabilityCollection(
        _In_ DISM_ON_DEMAND_SOURCE_FILTER SourceFilter,
        _Out_ IDismCapabilityCollection** ppCapabilities) = 0;
    virtual HRESULT STDMETHODCALLTYPE StartComponentCleanupEx(
        _In_ VARIANT_BOOL ResetBase,
        _In_ DISM_RESETBASE_OPTION ResetBaseOption) = 0;
};

MIDL_INTERFACE("bc8687e4-4b69-4749-878d-6def2746fc36")
IDismPackageManager5 : IDismPackageManager4
{
    virtual HRESULT STDMETHODCALLTYPE SetProperty(
        DISM_CONFIGURABLE_PROPERTY Property,
        BSTR PropertyValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE PerformOperation(
        _In_ unsigned int Options,
        _In_ DISM_CBS_OPERATION_TYPE OperationType) = 0;
};

MIDL_INTERFACE("6a064b65-3cec-491b-a2d4-8d1d4da6decc")
IDismDriverManager : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE GetDriverPackageCollection(
        _In_ DRIVER_COLL_OUTPUT CollType,
        _Out_ IDismDriverPackageCollection** DriverPackages) = 0;
    virtual HRESULT STDMETHODCALLTYPE OpenPackageByFile(
        _In_ BSTR PackageInf,
        _Out_ IDismDriverPackage** DriverPackage) = 0;
    virtual HRESULT STDMETHODCALLTYPE OpenPackageByFolder(
        _In_ BSTR PackageFolder,
        _In_ VARIANT_BOOL SearchSubFolders,
        _Out_ IDismErrorCollection** Errors,
        _Out_ IDismDriverPackageCollection** DriverPackages) = 0;
    virtual HRESULT STDMETHODCALLTYPE OpenPackageInStore(
        _In_ BSTR PackageName,
        _Out_ IDismDriverPackage** DriverPackage) = 0;
};

MIDL_INTERFACE("45554af5-6058-4913-8e5f-c9b79d5995cf")
IDismDriverPackageCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismDriverPackage** Driver) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("1af61d22-d513-4417-8838-61f8724edc4f")
IDismDriverPackage : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE GetDriverCollection(
        _In_ LONG Architecture,
        _Out_ IDismDriverCollection** Drivers) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_LocaleName(
        _Out_ LPBSTR LocaleName) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InfProviderName(
        _Out_ LPBSTR Name) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InBox(
        _Out_ VARIANT_BOOL* InBox) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_CatalogFile(
        _Out_ LPBSTR CatalogFile) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InfPath(
        _Out_ LPBSTR InfPath) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_PublishedInfName(
        _Out_ LPBSTR InfName) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ClassGuid(
        _Out_ LPBSTR ClassGuid) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ClassName(
        _Out_ LPBSTR ClassName) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ClassDescription(
        _Out_ LPBSTR ClassDesc) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_BootCritical(
        _Out_ VARIANT_BOOL* BootCritical) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_SignatureStatus(
        _Out_ DRIVER_SIG_STATUS* SigStatus) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDate(
        _Out_ LPLONG Year,
        _Out_ LPLONG Month,
        _Out_ LPLONG Day) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetVersion(
        _Out_ LPLONG Major,
        _Out_ LPLONG Minor,
        _Out_ LPLONG Build,
        _Out_ LPLONG Revision) = 0;
    virtual HRESULT STDMETHODCALLTYPE Install() = 0;
    virtual HRESULT STDMETHODCALLTYPE Remove() = 0;
};

MIDL_INTERFACE("3b6bbfa2-f817-423b-a83c-103ad54615d4")
IDismDriver : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_ParentPackage(
        _Out_ IDismDriverPackage** Package) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Manufacturer(
        _Out_ LPBSTR Manufacturer) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Description(
        _Out_ LPBSTR Description) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_HardwareId(
        _Out_ LPBSTR HardwareId) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Architecture(
        _Out_ LPLONG Architecture) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ServiceName(
        _Out_ LPBSTR HardwareId) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_CompatibleIds(
        _Out_ IDismDeviceIdCollection** DeviceIds) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ExcludeIds(
        _Out_ IDismDeviceIdCollection** DeviceIds) = 0;
};

MIDL_INTERFACE("4e1cb72f-c47d-42ee-8cca-4ceb35b3fe17")
IDismDriverCollection : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE get_Count(
        _Out_ LPLONG pnCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Item(
        _In_ LONG n,
        _Out_ IDismDriver** Driver) = 0;
    virtual HRESULT STDMETHODCALLTYPE get__NewEnum(
        _Out_ IUnknown** ppEnum) = 0;
};

MIDL_INTERFACE("671fcaaf-cf96-4b46-ac3d-7b968fbbcc3f")
IDismDriverPackage2 : IDismDriverPackage
{
    virtual HRESULT STDMETHODCALLTYPE InstallEx(
        _In_ DRIVER_INSTALL_FLAGS Flags) = 0;
};

MIDL_INTERFACE("04f36a61-a37d-406d-a320-0708444f3e26")
IDismDriverPackage3 : IDismDriverPackage2
{
    virtual HRESULT STDMETHODCALLTYPE InstallEx2(
        _In_ DRIVER_INSTALL_FLAGS Flags,
        _Inout_ IDismDriverPackage** StorePackage) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetProperty(
        _In_ struct DEVPROPKEY* PropertyKey,
        _In_ ULONG PropertyType,
        _In_ SAFEARRAY* PropertyBuffer,
        _In_ ULONG Flags) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetProperty(
        _In_ struct DEVPROPKEY* PropertyKey,
        _Inout_ LPDWORD PropertyType,
        _Inout_ SAFEARRAY** PropertyBuffer,
        _In_ ULONG Flags) = 0;
};

MIDL_INTERFACE("6d9208e3-17b2-46c2-b902-2433cc8cf0fd")
IDismDriverPackage4 : IDismDriverPackage3
{
    virtual HRESULT STDMETHODCALLTYPE get_TargetProcessorArchitecture(
        _Out_ LPLONG Architecture) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_TargetProcessorArchitecture(
        _In_ LONG Architecture) = 0;
};

EXTERN_GUID(
    IID_IDismLogger,
    0xbd7fc505, 0x69bc, 0x49cb, 0x8c, 0x58, 0x1d, 0x1d, 0xa5, 0x47, 0xb7, 0xe2);
EXTERN_GUID(
    IID_IDismObject,
    0xd8066238, 0xfdd5, 0x4195, 0x81, 0x55, 0x23, 0x71, 0xe0, 0xe6, 0x1c, 0xc5);
EXTERN_GUID(
    IID_IDismProvider,
    0x33f93018, 0xedb7, 0x4092, 0x97, 0x8a, 0x4b, 0x37, 0xd5, 0xdf, 0xb1, 0xc0);
EXTERN_GUID(
    IID_IDismProviderCollection,
    0x722de115, 0x1404, 0x4bf8, 0xbc, 0xee, 0xde, 0x0b, 0x7b, 0x58, 0x06, 0x71);
EXTERN_GUID(
    IID_IDismImage,
    0x9bb62bf9, 0x01a0, 0x49a7, 0xbe, 0x40, 0xfb, 0x54, 0x45, 0xb2, 0x9b, 0xef);
EXTERN_GUID(
    IID_IDismProviderStore,
    0x5df7b1b0, 0x93a5, 0x4f03, 0x8b, 0xcb, 0xf3, 0x93, 0x96, 0x89, 0x94, 0x43);
EXTERN_GUID(
    IID_IDismImageSession,
    0xa08538a6, 0xb9ff, 0x47c2, 0xb2, 0x28, 0xb9, 0x36, 0x12, 0xef, 0x11, 0x4b);
EXTERN_GUID(
    IID_IDismManager,
    0x4d4fce31, 0xf767, 0x45e4, 0xb2, 0xe9, 0x54, 0x00, 0xe3, 0xe1, 0x38, 0x9a);
EXTERN_GUID(
    IID_IDismManagerPrivate,
    0x8817cb2a, 0xe80a, 0x480b, 0xa7, 0xb4, 0x22, 0x32, 0x44, 0x3a, 0x9e, 0x4b);
EXTERN_GUID(
    IID_IDismEventManager,
    0xd9894135, 0x1aa7, 0x49af, 0xad, 0x11, 0x27, 0xe7, 0x84, 0xca, 0xc9, 0x31);
EXTERN_GUID(
    IID_IDismError,
    0x18caabcd, 0xeb4c, 0x4df3, 0x9f, 0xf1, 0xb5, 0xd2, 0x4e, 0x65, 0x5f, 0x33);
EXTERN_GUID(
    IID_IDismErrorCollection,
    0x74d82b92, 0xae58, 0x4962, 0x91, 0x33, 0x37, 0x01, 0xe1, 0x51, 0x0a, 0x81);
EXTERN_GUID(
    IID_IDismImageInfo,
    0x7970de73, 0x63e8, 0x43e9, 0x9f, 0x61, 0x81, 0xbd, 0xc7, 0x48, 0xf2, 0x9b);
EXTERN_GUID(
    IID_IDismImageInfoCollection,
    0x572f3374, 0x7dae, 0x47ab, 0x88, 0xaf, 0xbd, 0x6c, 0xa2, 0x92, 0x52, 0xfe);
EXTERN_GUID(
    IID_IDismMountedImageInfo,
    0xee9b6624, 0x8acc, 0x440c, 0x95, 0x64, 0xf5, 0xd7, 0x56, 0x8d, 0x6e, 0x01);
EXTERN_GUID(
    IID_IDismMountedImageInfoCollection,
    0x931fc35d, 0x0976, 0x48b8, 0x8b, 0xde, 0xb5, 0x95, 0x4a, 0xad, 0xd3, 0xf2);
EXTERN_GUID(
    IID_IDismImageManager,
    0xa654ff2f, 0xd9fa, 0x4df1, 0xaa, 0xbf, 0x98, 0x2c, 0x0a, 0x3c, 0x5f, 0x52);
EXTERN_GUID(
    IID_IDismImageSessionPrivate,
    0xff0e7a8b, 0x7b7c, 0x411b, 0x96, 0x8b, 0x11, 0xaa, 0x89, 0x8b, 0xb0, 0x3a);
EXTERN_GUID(
    IID_IDismImageSessionPrivate2,
    0x13a81f76, 0x50e2, 0x11e0, 0xb7, 0x44, 0x00, 0x12, 0x3f, 0x3f, 0xc6, 0xdd);
EXTERN_GUID(
    IID_IDismConfigObject,
    0x705a28e2, 0xa26f, 0x4a43, 0x8d, 0x82, 0xa5, 0x69, 0x41, 0xb5, 0xc2, 0x50);
EXTERN_GUID(
    IID_IDismConfiguration,
    0x3f92cf95, 0x462b, 0x4bda, 0xa1, 0xd4, 0xb6, 0xcb, 0x74, 0xce, 0xfb, 0x5c);
EXTERN_GUID(
    IID_IDismConfiguration2,
    0x59891552, 0x9ac8, 0x4c89, 0xb2, 0xa7, 0xc6, 0x4b, 0x39, 0x62, 0x35, 0x4e);
EXTERN_GUID(
    IID_IDismProperty,
    0x20599df3, 0x1a1b, 0x4eaa, 0xa5, 0x3f, 0x77, 0xf3, 0x94, 0x38, 0xb5, 0x42);
EXTERN_GUID(
    IID_IDismPropertyCollection,
    0x88d4518a, 0x41a8, 0x49b0, 0xa0, 0x87, 0xa3, 0xd6, 0xfc, 0x1d, 0x29, 0x8e);
EXTERN_GUID(
    IID_IDismStringCollection,
    0x7e6eb080, 0x3384, 0x4155, 0xa4, 0xa7, 0x5e, 0x43, 0xbb, 0xaf, 0xb2, 0xf3);
EXTERN_GUID(
    IID_IDismProviderPrivate,
    0x2114daec, 0x42fd, 0x4847, 0x8a, 0x04, 0x5f, 0x2d, 0xde, 0x32, 0x76, 0xa3);
EXTERN_GUID(
    IID_IDismRegistry,
    0x8e1c59a6, 0x272f, 0x4212, 0xa6, 0xd3, 0x8d, 0xc6, 0xcd, 0xdf, 0x81, 0x3a);
EXTERN_GUID(
    IID_IDismOSServiceManager,
    0x0b49a069, 0x56a5, 0x48eb, 0x9d, 0x9f, 0xe9, 0xe4, 0x24, 0x78, 0x14, 0x9a);
EXTERN_GUID(
    IID_IDismServicingProvider,
    0xc8292388, 0x8d1d, 0x4c34, 0x9a, 0x41, 0x87, 0x63, 0xfc, 0xd3, 0x04, 0xf7);
EXTERN_GUID(
    IID_IDismItemCollection,
    0xe2c60889, 0x8e0f, 0x4a21, 0x9f, 0x5b, 0xe8, 0x0f, 0x0b, 0x13, 0xc8, 0xd8);
EXTERN_GUID(
    IID_IDismItemManager,
    0xf316031b, 0x3c98, 0x4f20, 0xb9, 0xcb, 0xa3, 0xb8, 0x70, 0x2d, 0xdd, 0xd8);
EXTERN_GUID(
    IID_IDismSupportUnattend,
    0xdadad7bb, 0x2b1d, 0x4edb, 0x90, 0x14, 0xb1, 0x91, 0x2e, 0x1a, 0x2e, 0x3d);
EXTERN_GUID(
    IID_IDismToken,
    0x5bed6686, 0xeeb7, 0x40b7, 0x97, 0xf9, 0xfa, 0xbc, 0xc5, 0xef, 0xae, 0xc2);
EXTERN_GUID(
    IID_IDismTokenCollection,
    0x407c0f0f, 0x0540, 0x4900, 0xaf, 0xd2, 0x52, 0x92, 0x52, 0x46, 0xdc, 0xb3);
EXTERN_GUID(
    IID_IDismCommand,
    0xc112610a, 0x5d2a, 0x4d52, 0x80, 0x82, 0xed, 0xcd, 0x5a, 0x5e, 0xb1, 0x1d);
EXTERN_GUID(
    IID_IDismCommandCollection,
    0x8f609999, 0x5d3c, 0x48a8, 0xaa, 0xdf, 0x5f, 0x96, 0xcb, 0x29, 0x72, 0x79);
EXTERN_GUID(
    IID_IDismHelpItem,
    0xe3f237d6, 0x8f3c, 0x4443, 0x92, 0x5a, 0xab, 0xf4, 0x48, 0x07, 0x69, 0x0c);
EXTERN_GUID(
    IID_IDismHelpItemCollection,
    0x14e23a62, 0xcd9f, 0x4e41, 0xab, 0xd5, 0x58, 0xd5, 0x34, 0xbf, 0x37, 0x4e);
EXTERN_GUID(
    IID_IDismSupportCommands,
    0x7234ec82, 0xc68b, 0x4c4e, 0xba, 0x1d, 0xd3, 0x33, 0xf5, 0xb8, 0x7b, 0x37);
EXTERN_GUID(
    IID_IDismProviderStorePrivate,
    0xfe104c14, 0x65e6, 0x43b3, 0xaa, 0x56, 0xf0, 0x7b, 0x5e, 0x57, 0xd8, 0x1f);
EXTERN_GUID(
    IID_IDismHostManager,
    0xc9e39092, 0x53df, 0x42b6, 0x89, 0xb4, 0xfb, 0xa1, 0x26, 0x62, 0xea, 0xd1);
EXTERN_GUID(
    IID_IDismCapability,
    0x6a7dee63, 0x129c, 0x41e8, 0xb6, 0xa4, 0xbd, 0x1e, 0xb1, 0xd8, 0x5e, 0x65);
EXTERN_GUID(
    IID_IDismCapabilityCollection,
    0x9b4784da, 0xe322, 0x4bf0, 0x80, 0x0b, 0x28, 0xba, 0xe3, 0x57, 0xec, 0x71);
EXTERN_GUID(
    IID_IDismMsuPackage,
    0xf8ef44d0, 0x94f1, 0x4dfb, 0xba, 0xd8, 0x7b, 0x13, 0x83, 0x0b, 0xa0, 0x6e);
EXTERN_GUID(
    IID_IDismComponentStoreReport,
    0x46ce4564, 0xe88a, 0x4eae, 0xbd, 0x7c, 0xda, 0x22, 0x1f, 0x61, 0xe0, 0xc7);
EXTERN_GUID(
    IID_IDismServicePackScavenge,
    0xa457b170, 0xc9c3, 0x499b, 0x85, 0xbc, 0x59, 0x25, 0x9e, 0x92, 0xae, 0x4d);
EXTERN_GUID(
    IID_IDismXmlPackageManager,
    0xc1817aef, 0x4351, 0x4721, 0x9a, 0x62, 0x5d, 0xed, 0xa9, 0xda, 0x24, 0x6c);
EXTERN_GUID(
    IID_IDismDeviceIdCollection,
    0xa3ccf844, 0x82c9, 0x431e, 0x8d, 0x29, 0x5b, 0xd0, 0x9f, 0x8c, 0x9b, 0x70);
EXTERN_GUID(
    IID_IUnattend,
    0xe7627187, 0xc11b, 0x42fc, 0x91, 0xdb, 0x4f, 0x46, 0xbd, 0x54, 0x53, 0x74);
EXTERN_GUID(
    IID_IUnattendManager,
    0x517cdc5a, 0x7a77, 0x4356, 0x81, 0xca, 0xd3, 0x9f, 0x50, 0x65, 0x87, 0x26);
EXTERN_GUID(
    IID_IUnattendSettings,
    0xf8a8aef3, 0x1322, 0x4e1d, 0xbb, 0xf1, 0x0a, 0x22, 0x8c, 0x6f, 0xe1, 0x93);
EXTERN_GUID(
    IID_IDismCompatManager,
    0x092187e4, 0x09b8, 0x46a3, 0x87, 0xd5, 0x1c, 0x9b, 0xc6, 0xed, 0x8b, 0x5d);
EXTERN_GUID(
    IID_IDismIntlManager,
    0xeced57df, 0x5d67, 0x440a, 0xa3, 0xcd, 0xb4, 0xf1, 0xb8, 0xb3, 0x9d, 0x1a);
EXTERN_GUID(
    IID_IDismIntl,
    0x574fd8f3, 0xbc64, 0x455e, 0xa0, 0xd8, 0xbc, 0xf9, 0x5d, 0x88, 0x19, 0xb1);
EXTERN_GUID(
    IID_IPEImageManager,
    0x30eb8593, 0xfa91, 0x4948, 0xad, 0xf5, 0xdc, 0x6a, 0x35, 0x4f, 0x18, 0x73);
EXTERN_GUID(
    IID_ITransmogrifyEditionCollection,
    0xb629eef0, 0x2060, 0x43a0, 0xa1, 0x80, 0x7d, 0x04, 0x35, 0x1c, 0x39, 0x03);
EXTERN_GUID(
    IID_ITransmogrify,
    0x05bd25af, 0xf5d6, 0x4246, 0x9f, 0x91, 0x94, 0xb1, 0x87, 0xbc, 0x2b, 0xf4);
EXTERN_GUID(
    IID_ITransmogrify2,
    0xc6abc167, 0x33b8, 0x4a00, 0xbe, 0x58, 0x12, 0xec, 0x5b, 0x01, 0x35, 0x98);
EXTERN_GUID(
    IID_ITransmogrify3,
    0x50f2ddd7, 0x90ed, 0x4db1, 0x8e, 0xba, 0x3c, 0x99, 0x54, 0x86, 0xaa, 0x0c);
EXTERN_GUID(
    IID_ITransmogrify4,
    0xe6757b45, 0x14fc, 0x42c1, 0x99, 0x43, 0xac, 0x63, 0x0a, 0x8b, 0x0e, 0x1b);
EXTERN_GUID(
    IID_ITransmogrify5,
    0x3f483e2e, 0xf737, 0x481e, 0xa3, 0xa5, 0xc6, 0x4e, 0x53, 0xe4, 0x93, 0xae);
EXTERN_GUID(
    IID_IDismMsiManager,
    0x57ab0039, 0x5829, 0x4ebf, 0xb0, 0x93, 0x0b, 0x3a, 0x80, 0x86, 0x7c, 0x24);
EXTERN_GUID(
    IID_IDismIBSSupport,
    0xe1b47f29, 0x955c, 0x49a5, 0xb0, 0xa5, 0x50, 0x18, 0x20, 0x7f, 0xde, 0x69);
EXTERN_GUID(
    IID_IDismAppxPackageInfo,
    0x40e0e56d, 0x3d1d, 0x4f5c, 0xb3, 0x25, 0x01, 0x21, 0x42, 0x36, 0x77, 0xc6);
EXTERN_GUID(
    IID_IDismAppxPackageInfoCollection,
    0x75cae0d9, 0x716d, 0x4e62, 0x93, 0x1d, 0xbd, 0xfa, 0x3d, 0xbb, 0xd4, 0x2e);
EXTERN_GUID(
    IID_IDismAppxManager,
    0x54103f66, 0x7cb7, 0x41ba, 0xad, 0x1a, 0x2e, 0x12, 0x4d, 0x01, 0xc0, 0x8c);
EXTERN_GUID(
    IID_IDismAppxManager2,
    0x80f63941, 0x9464, 0x4495, 0xba, 0xc3, 0x51, 0xa4, 0x9b, 0x20, 0xce, 0x80);
EXTERN_GUID(
    IID_IDismAppxManager3,
    0x6674fe44, 0xde28, 0x4b7e, 0x97, 0x8d, 0xab, 0x6a, 0x51, 0xce, 0x0a, 0x5b);
EXTERN_GUID(
    IID_IDismAppxManager4,
    0x7446ff66, 0x999d, 0x497a, 0xb7, 0x12, 0xa2, 0x60, 0x59, 0x73, 0x0c, 0x5d);
EXTERN_GUID(
    IID_IDismAppxManager5,
    0x9b31f137, 0x5668, 0x441f, 0x8b, 0xc3, 0xd2, 0x1f, 0x8d, 0x3c, 0x70, 0xdb);
EXTERN_GUID(
    IID_IDismAppxManager6,
    0xb83f048e, 0x2309, 0x4e52, 0x96, 0x5b, 0x4f, 0xf5, 0x6b, 0x3f, 0x5a, 0xb7);
EXTERN_GUID(
    IID_IDismAppxPackageInfo2,
    0x5367d43f, 0xaa59, 0x4a81, 0x90, 0x68, 0x36, 0xc7, 0xa3, 0x60, 0xfe, 0x72);
EXTERN_GUID(
    IID_IDismAssocSupport,
    0xdced1a68, 0x143b, 0x4c8c, 0x84, 0x51, 0xf1, 0x55, 0xbd, 0xe8, 0x21, 0xdb);
EXTERN_GUID(
    IID_IDismKcacheManager,
    0xed08f15d, 0xd5f2, 0x40bf, 0xb0, 0x8b, 0x73, 0xa8, 0x45, 0x6c, 0xa6, 0x2d);
EXTERN_GUID(
    IID_IDismSetMachineName,
    0x16fcf50b, 0x1eb0, 0x4ed3, 0x9a, 0x5c, 0x8a, 0x33, 0xc8, 0x0f, 0x0d, 0x83);
EXTERN_GUID(
    IID_IDismGenericManager,
    0x0ab3ac34, 0xc644, 0x4903, 0xaa, 0x5f, 0x5b, 0x5d, 0x8f, 0x15, 0xd9, 0x69);
EXTERN_GUID(
    IID_IDismSysprep,
    0xdb46267b, 0x2fea, 0x4bb5, 0x9f, 0x14, 0xe4, 0xe5, 0x82, 0xe4, 0x77, 0x7f);
EXTERN_GUID(
    IID_IDismSysprepManager,
    0x8e65fbac, 0x20ad, 0x4075, 0xbd, 0x7b, 0x37, 0x71, 0x12, 0xff, 0x1f, 0x69);
EXTERN_GUID(
    IID_IDismSetupPlatform,
    0x53219e4f, 0x44c6, 0x4ed4, 0x83, 0xf9, 0x06, 0x5f, 0xde, 0xe9, 0x57, 0x95);
EXTERN_GUID(
    IID_IDismSetupPlatformManager,
    0xaa668c08, 0x675c, 0x49cb, 0xa1, 0x27, 0x2c, 0xd5, 0x7a, 0x17, 0x3c, 0x04);
EXTERN_GUID(
    IID_IDismFfuProvider,
    0x924d876e, 0x17f7, 0x4a5e, 0xac, 0x33, 0x91, 0x8c, 0x2b, 0xea, 0x0f, 0xd5);
EXTERN_GUID(
    IID_IDismFfuProvider2,
    0xa192440b, 0x5e7b, 0x4538, 0xa4, 0x8b, 0x3d, 0xfe, 0x9e, 0x04, 0x30, 0x86);
EXTERN_GUID(
    IID_IDismProvPackageElement,
    0xf7a983d3, 0xe787, 0x40a8, 0x95, 0x72, 0xcd, 0x04, 0xff, 0x99, 0xd0, 0x01);
EXTERN_GUID(
    IID_IDismProvPackageElementCollection,
    0x3ca3a1b2, 0xff8c, 0x487a, 0xa0, 0xbc, 0x95, 0x32, 0x7c, 0xca, 0xbd, 0x7b);
EXTERN_GUID(
    IID_IDismProvisioningSupport,
    0x446e7afc, 0x05e3, 0x4eea, 0x9e, 0x28, 0xd8, 0xd6, 0xb3, 0x24, 0xa8, 0xc7);
EXTERN_GUID(
    DIID__IDismEvents,
    0xa0efd55a, 0x17a8, 0x413d, 0xa5, 0x3a, 0x3c, 0x43, 0xae, 0xf9, 0x6c, 0x0e);
EXTERN_GUID(
    CLSID_DismManager,
    0xbe33a527, 0x42fe, 0x43a4, 0xac, 0x5e, 0xc1, 0xd0, 0x59, 0xfc, 0x70, 0x5f);
EXTERN_GUID(
    IID_IDismPackage,
    0xdee9a285, 0xb7f6, 0x4c16, 0xb6, 0x51, 0x7a, 0x13, 0xaf, 0x90, 0xf4, 0x2c);
EXTERN_GUID(
    IID_IDismPackage2,
    0x5f81a121, 0xc3c5, 0x4e94, 0xb7, 0xae, 0x22, 0xb5, 0x32, 0xf9, 0xf3, 0xa8);
EXTERN_GUID(
    IID_IDismPackage3,
    0xec622249, 0xba0c, 0x405b, 0xa8, 0x3c, 0x4a, 0x31, 0xbc, 0x7f, 0x0c, 0x4d);
EXTERN_GUID(
    IID_IDismPackage4,
    0xa27155b5, 0x6831, 0x4427, 0xb0, 0xd0, 0x97, 0x2d, 0x58, 0xf2, 0x02, 0x63);
EXTERN_GUID(
    IID_IDismPackageCollection,
    0xd80d838a, 0x0d57, 0x428c, 0xb8, 0x98, 0x26, 0x5a, 0x7d, 0x24, 0x4a, 0x67);
EXTERN_GUID(
    IID_IDismPackageFeature,
    0x3bc3752e, 0x74c5, 0x46f0, 0xb5, 0x0c, 0x12, 0xff, 0x26, 0x9d, 0x5b, 0xb1);
EXTERN_GUID(
    IID_IDismPackageFeatureCollection,
    0x63e0af82, 0x28e4, 0x4145, 0xae, 0x9a, 0xaa, 0xdd, 0x71, 0x5c, 0x12, 0x51);
EXTERN_GUID(
    IID_IDismPackageFeature2,
    0x1c551557, 0x21c2, 0x4907, 0xbe, 0x82, 0x35, 0xd7, 0xee, 0xf5, 0xbb, 0x13);
EXTERN_GUID(
    IID_IDismPackageManager,
    0x1754aa4f, 0xf0e2, 0x4692, 0xa8, 0x02, 0xd5, 0x18, 0x71, 0x08, 0x5f, 0x34);
EXTERN_GUID(
    IID_IDismPackageManager2,
    0x5cc9ed95, 0xcf41, 0x4685, 0xbe, 0xaa, 0x70, 0x53, 0x10, 0x31, 0xfe, 0xc4);
EXTERN_GUID(
    IID_IDismPackageManager3,
    0xe40dd35a, 0x89e1, 0x4619, 0xad, 0x21, 0xef, 0x97, 0xd2, 0x9b, 0x5d, 0x00);
EXTERN_GUID(
    IID_IDismPackageManager4,
    0x4dfd752a, 0x1e5e, 0x4c70, 0x94, 0xff, 0x9c, 0x91, 0xb5, 0xf6, 0x13, 0x35);
EXTERN_GUID(
    IID_IDismPackageManager5,
    0xbc8687e4, 0x4b69, 0x4749, 0x87, 0x8d, 0x6d, 0xef, 0x27, 0x46, 0xfc, 0x36);
EXTERN_GUID(
    IID_IDismDriverManager,
    0x6a064b65, 0x3cec, 0x491b, 0xa2, 0xd4, 0x8d, 0x1d, 0x4d, 0xa6, 0xde, 0xcc);
EXTERN_GUID(
    IID_IDismDriverPackageCollection,
    0x45554af5, 0x6058, 0x4913, 0x8e, 0x5f, 0xc9, 0xb7, 0x9d, 0x59, 0x95, 0xcf);
EXTERN_GUID(
    IID_IDismDriverPackage,
    0x1af61d22, 0xd513, 0x4417, 0x88, 0x38, 0x61, 0xf8, 0x72, 0x4e, 0xdc, 0x4f);
EXTERN_GUID(
    IID_IDismDriver,
    0x3b6bbfa2, 0xf817, 0x423b, 0xa8, 0x3c, 0x10, 0x3a, 0xd5, 0x46, 0x15, 0xd4);
EXTERN_GUID(
    IID_IDismDriverCollection,
    0x4e1cb72f, 0xc47d, 0x42ee, 0x8c, 0xca, 0x4c, 0xeb, 0x35, 0xb3, 0xfe, 0x17);
EXTERN_GUID(
    IID_IDismDriverPackage2,
    0x671fcaaf, 0xcf96, 0x4b46, 0xac, 0x3d, 0x7b, 0x96, 0x8f, 0xbb, 0xcc, 0x3f);
EXTERN_GUID(
    IID_IDismDriverPackage3,
    0x04f36a61, 0xa37d, 0x406d, 0xa3, 0x20, 0x07, 0x08, 0x44, 0x4f, 0x3e, 0x26);
EXTERN_GUID(
    IID_IDismDriverPackage4,
    0x6d9208e3, 0x17b2, 0x46c2, 0xb9, 0x02, 0x24, 0x33, 0xcc, 0x8c, 0xf0, 0xfd);

#endif // !DISM_CORE_H
