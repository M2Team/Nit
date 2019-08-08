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

#include <comdef.h>

#ifndef DEVPROPKEY_DEFINED
#define DEVPROPKEY_DEFINED

typedef GUID  DEVPROPGUID, * PDEVPROPGUID;
typedef ULONG DEVPROPID, * PDEVPROPID;

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

struct __declspec(uuid("4d4fce31-f767-45e4-b2e9-5400e3e1389a"))
/* dual interface */ IDismManager;
struct __declspec(uuid("bd7fc505-69bc-49cb-8c58-1d1da547b7e2"))
/* dual interface */ IDismLogger;
struct __declspec(uuid("5df7b1b0-93a5-4f03-8bcb-f39396899443"))
/* dual interface */ IDismProviderStore;
struct __declspec(uuid("d8066238-fdd5-4195-8155-2371e0e61cc5"))
/* interface */ IDismObject;
struct __declspec(uuid("33f93018-edb7-4092-978a-4b37d5dfb1c0"))
/* dual interface */ IDismProvider;
struct __declspec(uuid("722de115-1404-4bf8-bcee-de0b7b580671"))
/* dual interface */ IDismProviderCollection;
struct __declspec(uuid("9bb62bf9-01a0-49a7-be40-fb5445b29bef"))
/* dual interface */ IDismImage;
struct __declspec(uuid("a08538a6-b9ff-47c2-b228-b93612ef114b"))
/* dual interface */ IDismImageSession;
struct __declspec(uuid("8817cb2a-e80a-480b-a7b4-2232443a9e4b"))
/* interface */ IDismManagerPrivate;
struct __declspec(uuid("d9894135-1aa7-49af-ad11-27e784cac931"))
/* interface */ IDismEventManager;
struct __declspec(uuid("18caabcd-eb4c-4df3-9ff1-b5d24e655f33"))
/* dual interface */ IDismError;
struct __declspec(uuid("74d82b92-ae58-4962-9133-3701e1510a81"))
/* dual interface */ IDismErrorCollection;
struct __declspec(uuid("a654ff2f-d9fa-4df1-aabf-982c0a3c5f52"))
/* dual interface */ IDismImageManager;
struct __declspec(uuid("572f3374-7dae-47ab-88af-bd6ca29252fe"))
/* dual interface */ IDismImageInfoCollection;
struct __declspec(uuid("7970de73-63e8-43e9-9f61-81bdc748f29b"))
/* dual interface */ IDismImageInfo;
struct __declspec(uuid("931fc35d-0976-48b8-8bde-b5954aadd3f2"))
/* dual interface */ IDismMountedImageInfoCollection;
struct __declspec(uuid("ee9b6624-8acc-440c-9564-f5d7568d6e01"))
/* dual interface */ IDismMountedImageInfo;
struct __declspec(uuid("ff0e7a8b-7b7c-411b-968b-11aa898bb03a"))
/* interface */ IDismImageSessionPrivate;
struct __declspec(uuid("13a81f76-50e2-11e0-b744-00123f3fc6dd"))
/* interface */ IDismImageSessionPrivate2;
struct __declspec(uuid("705a28e2-a26f-4a43-8d82-a56941b5c250"))
/* dual interface */ IDismConfigObject;
struct __declspec(uuid("3f92cf95-462b-4bda-a1d4-b6cb74cefb5c"))
/* interface */ IDismConfiguration;
struct __declspec(uuid("59891552-9ac8-4c89-b2a7-c64b3962354e"))
/* interface */ IDismConfiguration2;
struct __declspec(uuid("20599df3-1a1b-4eaa-a53f-77f39438b542"))
/* interface */ IDismProperty;
struct __declspec(uuid("88d4518a-41a8-49b0-a087-a3d6fc1d298e"))
/* interface */ IDismPropertyCollection;
struct __declspec(uuid("7e6eb080-3384-4155-a4a7-5e43bbafb2f3"))
/* dual interface */ IDismStringCollection;
struct __declspec(uuid("2114daec-42fd-4847-8a04-5f2dde3276a3"))
/* interface */ IDismProviderPrivate;
struct __declspec(uuid("c8292388-8d1d-4c34-9a41-8763fcd304f7"))
/* dual interface */ IDismServicingProvider;
struct __declspec(uuid("0b49a069-56a5-48eb-9d9f-e9e42478149a"))
/* dual interface */ IDismOSServiceManager;
struct __declspec(uuid("8e1c59a6-272f-4212-a6d3-8dc6cddf813a"))
/* dual interface */ IDismRegistry;
struct __declspec(uuid("f316031b-3c98-4f20-b9cb-a3b8702dddd8"))
/* dual interface */ IDismItemManager;
struct __declspec(uuid("e2c60889-8e0f-4a21-9f5b-e80f0b13c8d8"))
/* dual interface */ IDismItemCollection;
struct __declspec(uuid("dadad7bb-2b1d-4edb-9014-b1912e1a2e3d"))
/* dual interface */ IDismSupportUnattend;
struct __declspec(uuid("7234ec82-c68b-4c4e-ba1d-d333f5b87b37"))
/* interface */ IDismSupportCommands;
struct __declspec(uuid("407c0f0f-0540-4900-afd2-52925246dcb3"))
/* dual interface */ IDismTokenCollection;
struct __declspec(uuid("5bed6686-eeb7-40b7-97f9-fabcc5efaec2"))
/* dual interface */ IDismToken;
struct __declspec(uuid("8f609999-5d3c-48a8-aadf-5f96cb297279"))
/* dual interface */ IDismCommandCollection;
struct __declspec(uuid("c112610a-5d2a-4d52-8082-edcd5a5eb11d"))
/* interface */ IDismCommand;
struct __declspec(uuid("14e23a62-cd9f-4e41-abd5-58d534bf374e"))
/* dual interface */ IDismHelpItemCollection;
struct __declspec(uuid("e3f237d6-8f3c-4443-925a-abf44807690c"))
/* interface */ IDismHelpItem;
struct __declspec(uuid("fe104c14-65e6-43b3-aa56-f07b5e57d81f"))
/* interface */ IDismProviderStorePrivate;
struct __declspec(uuid("c9e39092-53df-42b6-89b4-fba12662ead1"))
/* dual interface */ IDismHostManager;
struct __declspec(uuid("dee9a285-b7f6-4c16-b651-7a13af90f42c"))
/* dual interface */ IDismPackage;
struct __declspec(uuid("3bc3752e-74c5-46f0-b50c-12ff269d5bb1"))
/* dual interface */ IDismPackageFeature;
struct __declspec(uuid("63e0af82-28e4-4145-ae9a-aadd715c1251"))
/* dual interface */ IDismPackageFeatureCollection;
struct __declspec(uuid("5f81a121-c3c5-4e94-b7ae-22b532f9f3a8"))
/* dual interface */ IDismPackage2;
struct __declspec(uuid("ec622249-ba0c-405b-a83c-4a31bc7f0c4d"))
/* dual interface */ IDismPackage3;
struct __declspec(uuid("a27155b5-6831-4427-b0d0-972d58f20263"))
/* dual interface */ IDismPackage4;
struct __declspec(uuid("d80d838a-0d57-428c-b898-265a7d244a67"))
/* dual interface */ IDismPackageCollection;
struct __declspec(uuid("1c551557-21c2-4907-be82-35d7eef5bb13"))
/* dual interface */ IDismPackageFeature2;
struct __declspec(uuid("6a7dee63-129c-41e8-b6a4-bd1eb1d85e65"))
/* dual interface */ IDismCapability;
struct __declspec(uuid("9b4784da-e322-4bf0-800b-28bae357ec71"))
/* dual interface */ IDismCapabilityCollection;
struct __declspec(uuid("1754aa4f-f0e2-4692-a802-d51871085f34"))
/* dual interface */ IDismPackageManager;
struct __declspec(uuid("f8ef44d0-94f1-4dfb-bad8-7b13830ba06e"))
/* dual interface */ IDismMsuPackage;
struct __declspec(uuid("5cc9ed95-cf41-4685-beaa-70531031fec4"))
/* dual interface */ IDismPackageManager2;
struct __declspec(uuid("e40dd35a-89e1-4619-ad21-ef97d29b5d00"))
/* dual interface */ IDismPackageManager3;
struct __declspec(uuid("46ce4564-e88a-4eae-bd7c-da221f61e0c7"))
/* dual interface */ IDismComponentStoreReport;
struct __declspec(uuid("4dfd752a-1e5e-4c70-94ff-9c91b5f61335"))
/* dual interface */ IDismPackageManager4;
struct __declspec(uuid("bc8687e4-4b69-4749-878d-6def2746fc36"))
/* dual interface */ IDismPackageManager5;
struct __declspec(uuid("a457b170-c9c3-499b-85bc-59259e92ae4d"))
/* dual interface */ IDismServicePackScavenge;
struct __declspec(uuid("c1817aef-4351-4721-9a62-5deda9da246c"))
/* dual interface */ IDismXmlPackageManager;
struct __declspec(uuid("6a064b65-3cec-491b-a2d4-8d1d4da6decc"))
/* dual interface */ IDismDriverManager;
struct __declspec(uuid("45554af5-6058-4913-8e5f-c9b79d5995cf"))
/* dual interface */ IDismDriverPackageCollection;
struct __declspec(uuid("1af61d22-d513-4417-8838-61f8724edc4f"))
/* dual interface */ IDismDriverPackage;
struct __declspec(uuid("4e1cb72f-c47d-42ee-8cca-4ceb35b3fe17"))
/* dual interface */ IDismDriverCollection;
struct __declspec(uuid("3b6bbfa2-f817-423b-a83c-103ad54615d4"))
/* dual interface */ IDismDriver;
struct __declspec(uuid("a3ccf844-82c9-431e-8d29-5bd09f8c9b70"))
/* dual interface */ IDismDeviceIdCollection;
struct __declspec(uuid("671fcaaf-cf96-4b46-ac3d-7b968fbbcc3f"))
/* dual interface */ IDismDriverPackage2;
struct __declspec(uuid("04f36a61-a37d-406d-a320-0708444f3e26"))
/* dual interface */ IDismDriverPackage3;
struct __declspec(uuid("6d9208e3-17b2-46c2-b902-2433cc8cf0fd"))
/* dual interface */ IDismDriverPackage4;
struct __declspec(uuid("e7627187-c11b-42fc-91db-4f46bd545374"))
/* dual interface */ IUnattend;
struct __declspec(uuid("517cdc5a-7a77-4356-81ca-d39f50658726"))
/* dual interface */ IUnattendManager;
struct __declspec(uuid("f8a8aef3-1322-4e1d-bbf1-0a228c6fe193"))
/* dual interface */ IUnattendSettings;
struct __declspec(uuid("092187e4-09b8-46a3-87d5-1c9bc6ed8b5d"))
/* dual interface */ IDismCompatManager;
struct __declspec(uuid("eced57df-5d67-440a-a3cd-b4f1b8b39d1a"))
/* dual interface */ IDismIntlManager;
struct __declspec(uuid("574fd8f3-bc64-455e-a0d8-bcf95d8819b1"))
/* dual interface */ IDismIntl;
struct __declspec(uuid("30eb8593-fa91-4948-adf5-dc6a354f1873"))
/* dual interface */ IPEImageManager;
struct __declspec(uuid("05bd25af-f5d6-4246-9f91-94b187bc2bf4"))
/* dual interface */ ITransmogrify;
struct __declspec(uuid("b629eef0-2060-43a0-a180-7d04351c3903"))
/* dual interface */ ITransmogrifyEditionCollection;
struct __declspec(uuid("c6abc167-33b8-4a00-be58-12ec5b013598"))
/* dual interface */ ITransmogrify2;
struct __declspec(uuid("50f2ddd7-90ed-4db1-8eba-3c995486aa0c"))
/* dual interface */ ITransmogrify3;
struct __declspec(uuid("e6757b45-14fc-42c1-9943-ac630a8b0e1b"))
/* dual interface */ ITransmogrify4;
struct __declspec(uuid("3f483e2e-f737-481e-a3a5-c64e53e493ae"))
/* dual interface */ ITransmogrify5;
struct __declspec(uuid("57ab0039-5829-4ebf-b093-0b3a80867c24"))
/* dual interface */ IDismMsiManager;
struct __declspec(uuid("e1b47f29-955c-49a5-b0a5-5018207fde69"))
/* dual interface */ IDismIBSSupport;
struct __declspec(uuid("54103f66-7cb7-41ba-ad1a-2e124d01c08c"))
/* dual interface */ IDismAppxManager;
struct __declspec(uuid("75cae0d9-716d-4e62-931d-bdfa3dbbd42e"))
/* dual interface */ IDismAppxPackageInfoCollection;
struct __declspec(uuid("40e0e56d-3d1d-4f5c-b325-0121423677c6"))
/* dual interface */ IDismAppxPackageInfo;
struct __declspec(uuid("80f63941-9464-4495-bac3-51a49b20ce80"))
/* dual interface */ IDismAppxManager2;
struct __declspec(uuid("6674fe44-de28-4b7e-978d-ab6a51ce0a5b"))
/* dual interface */ IDismAppxManager3;
struct __declspec(uuid("7446ff66-999d-497a-b712-a26059730c5d"))
/* dual interface */ IDismAppxManager4;
struct __declspec(uuid("9b31f137-5668-441f-8bc3-d21f8d3c70db"))
/* dual interface */ IDismAppxManager5;
struct __declspec(uuid("b83f048e-2309-4e52-965b-4ff56b3f5ab7"))
/* dual interface */ IDismAppxManager6;
struct __declspec(uuid("5367d43f-aa59-4a81-9068-36c7a360fe72"))
/* dual interface */ IDismAppxPackageInfo2;
struct __declspec(uuid("dced1a68-143b-4c8c-8451-f155bde821db"))
/* dual interface */ IDismAssocSupport;
struct __declspec(uuid("ed08f15d-d5f2-40bf-b08b-73a8456ca62d"))
/* dual interface */ IDismKcacheManager;
struct __declspec(uuid("16fcf50b-1eb0-4ed3-9a5c-8a33c80f0d83"))
/* dual interface */ IDismSetMachineName;
struct __declspec(uuid("0ab3ac34-c644-4903-aa5f-5b5d8f15d969"))
/* dual interface */ IDismGenericManager;
struct __declspec(uuid("db46267b-2fea-4bb5-9f14-e4e582e4777f"))
/* dual interface */ IDismSysprep;
struct __declspec(uuid("8e65fbac-20ad-4075-bd7b-377112ff1f69"))
/* dual interface */ IDismSysprepManager;
struct __declspec(uuid("53219e4f-44c6-4ed4-83f9-065fdee95795"))
/* dual interface */ IDismSetupPlatform;
struct __declspec(uuid("aa668c08-675c-49cb-a127-2cd57a173c04"))
/* dual interface */ IDismSetupPlatformManager;
struct __declspec(uuid("924d876e-17f7-4a5e-ac33-918c2bea0fd5"))
/* dual interface */ IDismFfuProvider;
struct __declspec(uuid("a192440b-5e7b-4538-a48b-3dfe9e043086"))
/* dual interface */ IDismFfuProvider2;
struct __declspec(uuid("446e7afc-05e3-4eea-9e28-d8d6b324a8c7"))
/* dual interface */ IDismProvisioningSupport;
struct __declspec(uuid("3ca3a1b2-ff8c-487a-a0bc-95327ccabd7b"))
/* dual interface */ IDismProvPackageElementCollection;
struct __declspec(uuid("f7a983d3-e787-40a8-9572-cd04ff99d001"))
/* dual interface */ IDismProvPackageElement;
struct __declspec(uuid("a0efd55a-17a8-413d-a53a-3c43aef96c0e"))
/* dispinterface */ _IDismEvents;
struct /* coclass */ DismManager;

struct __declspec(uuid("bd7fc505-69bc-49cb-8c58-1d1da547b7e2"))
IDismLogger : IDispatch
{
      virtual HRESULT __stdcall Initialize (
        /*[in]*/ BSTR logFile,
        /*[in]*/ DISM_LOGLEVEL_TYPE logLevel ) = 0;
      virtual HRESULT __stdcall WriteEntry (
        /*[in]*/ DISM_LOGLEVEL_TYPE level,
        /*[in]*/ long providerID,
        /*[in]*/ BSTR providerName,
        /*[in]*/ BSTR message ) = 0;
      virtual HRESULT __stdcall get_LogFilePath (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_MaxLevelLogged (
        /*[out,retval]*/ DISM_LOGLEVEL_TYPE * pVal ) = 0;
      virtual HRESULT __stdcall get_Enabled (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall get_WdsCoreFilePath (
        /*[out,retval]*/ BSTR * pVal ) = 0;
};

struct __declspec(uuid("d8066238-fdd5-4195-8155-2371e0e61cc5"))
IDismObject : IDispatch
{
      virtual HRESULT __stdcall OnConnect (
        /*[in]*/ IUnknown * Parent,
        /*[in]*/ struct IDismLogger * DismLogger ) = 0;
      virtual HRESULT __stdcall OnDisconnect ( ) = 0;
      virtual HRESULT __stdcall get_Parent (
        /*[out,retval]*/ IUnknown * * Parent ) = 0;
};

struct __declspec(uuid("33f93018-edb7-4092-978a-4b37d5dfb1c0"))
IDismProvider : IDismObject
{
      virtual HRESULT __stdcall get_Name (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_Version (
        /*[out,retval]*/ BSTR * pVal ) = 0;
};

struct __declspec(uuid("722de115-1404-4bf8-bcee-de0b7b580671"))
IDismProviderCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismProvider * * ppProvider ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("9bb62bf9-01a0-49a7-be40-fb5445b29bef"))
IDismImage : IDispatch
{
      virtual HRESULT __stdcall get_PhysicalLocation (
        /*[out,retval]*/ BSTR * PhysicalLocation ) = 0;
      virtual HRESULT __stdcall Save (
        unsigned long ulFlags ) = 0;
      virtual HRESULT __stdcall Discard ( ) = 0;
      virtual HRESULT __stdcall get_IsClosed (
        /*[out,retval]*/ VARIANT_BOOL * IsClosed ) = 0;
      virtual HRESULT __stdcall get_ProviderStorePath (
        /*[out,retval]*/ BSTR * ProviderStorePath ) = 0;
      virtual HRESULT __stdcall SetProviderStorePath (
        /*[in]*/ BSTR ProviderStorePath ) = 0;
      virtual HRESULT __stdcall Unmount (
        /*[in]*/ unsigned long ulFlags ) = 0;
      virtual HRESULT __stdcall ValidateStatus ( ) = 0;
      virtual HRESULT __stdcall Remount ( ) = 0;
};

struct __declspec(uuid("5df7b1b0-93a5-4f03-8bcb-f39396899443"))
IDismProviderStore : IDismObject
{
      virtual HRESULT __stdcall GetProvider (
        /*[in]*/ BSTR providerName,
        /*[out,retval]*/ struct IDismProvider * * Provider ) = 0;
      virtual HRESULT __stdcall GetExternalProvider (
        /*[in]*/ BSTR providerName,
        /*[out,retval]*/ struct IDismProvider * * Provider ) = 0;
      virtual HRESULT __stdcall AddProvider (
        /*[in]*/ BSTR RelativeProviderLocation ) = 0;
      virtual HRESULT __stdcall RemoveProvider (
        /*[in]*/ BSTR providerName ) = 0;
      virtual HRESULT __stdcall GetProviderCollection (
        /*[out,retval]*/ struct IDismProviderCollection * * Providers ) = 0;
      virtual HRESULT __stdcall get_DismImage (
        /*[out,retval]*/ struct IDismImage * * DismImage ) = 0;
};

struct __declspec(uuid("a08538a6-b9ff-47c2-b228-b93612ef114b"))
IDismImageSession : IDismObject
{
      virtual HRESULT __stdcall get_ProviderStore (
        /*[out,retval]*/ struct IDismProviderStore * * ProviderStore ) = 0;
      virtual HRESULT __stdcall get_ImageState (
        /*[out,retval]*/ DISM_SESSION_STATE * State ) = 0;
      virtual HRESULT __stdcall get_DismImage (
        /*[out,retval]*/ struct IDismImage * * DismImage ) = 0;
      virtual HRESULT __stdcall get_SessionType (
        /*[out,retval]*/ DISM_SESSION_TYPE * SessionType ) = 0;
      virtual HRESULT __stdcall get_SessionVersion (
        /*[out,retval]*/ BSTR * SessionVersion ) = 0;
};

struct __declspec(uuid("4d4fce31-f767-45e4-b2e9-5400e3e1389a"))
IDismManager : IDispatch
{
      virtual HRESULT __stdcall get_Logger (
        /*[out,retval]*/ struct IDismLogger * * pVal ) = 0;
      virtual HRESULT __stdcall GetLocalProviderStore (
        /*[out,retval]*/ struct IDismProviderStore * * ppUnknown ) = 0;
      virtual HRESULT __stdcall CreateImageSession (
        /*[in]*/ struct IDismImage * pImage,
        /*[out,retval]*/ struct IDismImageSession * * ppUnknown ) = 0;
      virtual HRESULT __stdcall CloseImageSession (
        /*[in]*/ struct IDismImageSession * pImageSession,
        /*[out,retval]*/ DISM_SESSION_STATE * State ) = 0;
      virtual HRESULT __stdcall Close ( ) = 0;
      virtual HRESULT __stdcall ValidateDismTarget (
        /*[in]*/ struct IDismImage * pImage,
        /*[out,retval]*/ VARIANT_BOOL * pvbDismTargetValid ) = 0;
};

struct __declspec(uuid("8817cb2a-e80a-480b-a7b4-2232443a9e4b"))
IDismManagerPrivate : IUnknown
{};

struct __declspec(uuid("d9894135-1aa7-49af-ad11-27e784cac931"))
IDismEventManager : IUnknown
{
      virtual HRESULT __stdcall OnInitiate (
        /*[in]*/ BSTR providerName,
        /*[in]*/ BSTR ActionName,
        /*[in]*/ struct IDismImage * Image,
        /*[in]*/ BSTR message,
        /*[in,out]*/ int * Response ) = 0;
      virtual HRESULT __stdcall OnError (
        /*[in]*/ BSTR providerName,
        /*[in]*/ BSTR ActionName,
        /*[in]*/ struct IDismImage * Image,
        /*[in]*/ SCODE errorCode,
        /*[in]*/ BSTR message,
        /*[in,out]*/ int * Response ) = 0;
      virtual HRESULT __stdcall OnProgress (
        /*[in]*/ BSTR providerName,
        /*[in]*/ BSTR ActionName,
        /*[in]*/ struct IDismImage * Image,
        /*[in]*/ long ticks,
        /*[in]*/ long totalTicks,
        /*[in]*/ BSTR message,
        /*[in,out]*/ int * Response ) = 0;
      virtual HRESULT __stdcall OnTerminate (
        /*[in]*/ BSTR providerName,
        /*[in]*/ BSTR ActionName,
        /*[in]*/ struct IDismImage * Image,
        /*[in]*/ BSTR message ) = 0;
      virtual HRESULT __stdcall OnCLIOutput (
        /*[in]*/ BSTR providerName,
        /*[in]*/ BSTR ActionName,
        /*[in]*/ struct IDismImage * Image,
        /*[in]*/ BSTR message,
        /*[in,out]*/ int * Response ) = 0;
};

struct __declspec(uuid("18caabcd-eb4c-4df3-9ff1-b5d24e655f33"))
IDismError : IDispatch
{
      virtual HRESULT __stdcall get_Hresult (
        /*[out,retval]*/ HRESULT * hr ) = 0;
      virtual HRESULT __stdcall get_Source (
        /*[out,retval]*/ BSTR * Source ) = 0;
      virtual HRESULT __stdcall get_Description (
        /*[out,retval]*/ BSTR * Description ) = 0;
};

struct __declspec(uuid("74d82b92-ae58-4962-9133-3701e1510a81"))
IDismErrorCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismError * * Error ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("7970de73-63e8-43e9-9f61-81bdc748f29b"))
IDismImageInfo : IDispatch
{
      virtual HRESULT __stdcall get_Name (
        /*[out,retval]*/ BSTR * pbszName ) = 0;
      virtual HRESULT __stdcall put_Name (
        /*[in]*/ BSTR pbszName ) = 0;
      virtual HRESULT __stdcall get_Description (
        /*[out,retval]*/ BSTR * pbszDescription ) = 0;
      virtual HRESULT __stdcall put_Description (
        /*[in]*/ BSTR pbszDescription ) = 0;
      virtual HRESULT __stdcall get_Index (
        /*[out,retval]*/ unsigned long * pulIndex ) = 0;
      virtual HRESULT __stdcall GetProperty (
        /*[in]*/ BSTR bszName,
        /*[out,retval]*/ BSTR * pbszValue ) = 0;
      virtual HRESULT __stdcall GetPropertyEx (
        /*[in]*/ BSTR bszName,
        /*[in]*/ unsigned long ulIndex,
        /*[out,retval]*/ BSTR * pbszValue ) = 0;
      virtual HRESULT __stdcall SetProperty (
        /*[in]*/ BSTR bszName,
        /*[in]*/ BSTR bszValue ) = 0;
      virtual HRESULT __stdcall Mount (
        /*[in]*/ BSTR bszMountPath,
        /*[in]*/ unsigned long ulFlags,
        /*[out,retval]*/ struct IDismImage * * ppDISMImage ) = 0;
};

struct __declspec(uuid("572f3374-7dae-47ab-88af-bd6ca29252fe"))
IDismImageInfoCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismImageInfo * * ppDismImageInfo ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("ee9b6624-8acc-440c-9564-f5d7568d6e01"))
IDismMountedImageInfo : IDispatch
{
      virtual HRESULT __stdcall get_MountDir (
        /*[out,retval]*/ BSTR * pbszMountDir ) = 0;
      virtual HRESULT __stdcall get_ImagePath (
        /*[out,retval]*/ BSTR * pbszImagePath ) = 0;
      virtual HRESULT __stdcall get_Index (
        /*[out,retval]*/ unsigned long * pulIndex ) = 0;
      virtual HRESULT __stdcall get_ReadWritable (
        /*[out,retval]*/ VARIANT_BOOL * pvbReadWritable ) = 0;
      virtual HRESULT __stdcall get_Status (
        /*[out,retval]*/ unsigned long * pulStatus ) = 0;
};

struct __declspec(uuid("931fc35d-0976-48b8-8bde-b5954aadd3f2"))
IDismMountedImageInfoCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismMountedImageInfo * * ppDismMountedImageInfo ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("a654ff2f-d9fa-4df1-aabf-982c0a3c5f52"))
IDismImageManager : IDismObject
{
      virtual HRESULT __stdcall GetImageInfoCollection (
        /*[in]*/ BSTR bszPath,
        /*[out,retval]*/ struct IDismImageInfoCollection * * ppImageInfoCollection ) = 0;
      virtual HRESULT __stdcall CreateDismImage (
        /*[in]*/ BSTR bszPath,
        /*[out,retval]*/ struct IDismImage * * ppDISMImage ) = 0;
      virtual HRESULT __stdcall GetMountedImageInfoCollection (
        /*[out,retval]*/ struct IDismMountedImageInfoCollection * * ppMountedImageInfoCollection ) = 0;
      virtual HRESULT __stdcall CleanupMountpoints ( ) = 0;
};

struct __declspec(uuid("ff0e7a8b-7b7c-411b-968b-11aa898bb03a"))
IDismImageSessionPrivate : IDispatch
{
      virtual HRESULT __stdcall put_HostManager (
        /*[in]*/ IUnknown * _arg1 ) = 0;
      virtual HRESULT __stdcall put_ImageState (
        /*[in]*/ DISM_SESSION_STATE _arg1 ) = 0;
      virtual HRESULT __stdcall put_DismImage (
        /*[in]*/ struct IDismImage * _arg1 ) = 0;
      virtual HRESULT __stdcall put_SessionType (
        /*[in]*/ DISM_SESSION_TYPE _arg1 ) = 0;
      virtual HRESULT __stdcall ReleaseHostManager ( ) = 0;
};

struct __declspec(uuid("13a81f76-50e2-11e0-b744-00123f3fc6dd"))
IDismImageSessionPrivate2 : IDismImageSessionPrivate
{
      virtual HRESULT __stdcall get_CanUnloadClient (
        /*[out,retval]*/ VARIANT_BOOL * pvbCanUnloadClient ) = 0;
      virtual HRESULT __stdcall put_CanUnloadClient (
        /*[in]*/ VARIANT_BOOL pvbCanUnloadClient ) = 0;
};

struct __declspec(uuid("705a28e2-a26f-4a43-8d82-a56941b5c250"))
IDismConfigObject : IDispatch
{
      virtual HRESULT __stdcall get_MaxProgress (
        /*[out,retval]*/ int * MaxTick ) = 0;
      virtual HRESULT __stdcall put_MaxProgress (
        /*[in]*/ int MaxTick ) = 0;
      virtual HRESULT __stdcall get_ScratchDir (
        /*[out,retval]*/ BSTR * DirectoryPath ) = 0;
      virtual HRESULT __stdcall put_ScratchDir (
        /*[in]*/ BSTR DirectoryPath ) = 0;
      virtual HRESULT __stdcall get_logFile (
        /*[out,retval]*/ BSTR * logFile ) = 0;
      virtual HRESULT __stdcall put_logFile (
        /*[in]*/ BSTR logFile ) = 0;
      virtual HRESULT __stdcall get_logLevel (
        /*[out,retval]*/ DISM_LOGLEVEL_TYPE * logLevel ) = 0;
      virtual HRESULT __stdcall put_logLevel (
        /*[in]*/ DISM_LOGLEVEL_TYPE logLevel ) = 0;
};

struct __declspec(uuid("3f92cf95-462b-4bda-a1d4-b6cb74cefb5c"))
IDismConfiguration : IUnknown
{
      virtual HRESULT __stdcall get_MaxProgress (
        /*[out,retval]*/ int * MaxTick ) = 0;
      virtual HRESULT __stdcall put_MaxProgress (
        /*[in]*/ int MaxTick ) = 0;
      virtual HRESULT __stdcall get_ScratchDir (
        /*[out,retval]*/ BSTR * DirectoryPath ) = 0;
      virtual HRESULT __stdcall put_ScratchDir (
        /*[in]*/ BSTR DirectoryPath ) = 0;
      virtual HRESULT __stdcall get_logFile (
        /*[out,retval]*/ BSTR * logFile ) = 0;
      virtual HRESULT __stdcall put_logFile (
        /*[in]*/ BSTR logFile ) = 0;
      virtual HRESULT __stdcall get_logLevel (
        /*[out,retval]*/ DISM_LOGLEVEL_TYPE * logLevel ) = 0;
      virtual HRESULT __stdcall put_logLevel (
        /*[in]*/ DISM_LOGLEVEL_TYPE logLevel ) = 0;
      virtual HRESULT __stdcall get_ImageFlags (
        /*[out,retval]*/ DISM_IMAGE_FLAGS * ImageFlags ) = 0;
      virtual HRESULT __stdcall put_ImageFlags (
        /*[in]*/ DISM_IMAGE_FLAGS ImageFlags ) = 0;
      virtual HRESULT __stdcall get_Display (
        /*[out,retval]*/ DISM_DISPLAY_TYPE * Display ) = 0;
      virtual HRESULT __stdcall put_Display (
        /*[in]*/ DISM_DISPLAY_TYPE Display ) = 0;
      virtual HRESULT __stdcall get_Format (
        /*[out,retval]*/ DISM_FORMAT_TYPE * Format ) = 0;
      virtual HRESULT __stdcall put_Format (
        /*[in]*/ DISM_FORMAT_TYPE Format ) = 0;
};

struct __declspec(uuid("59891552-9ac8-4c89-b2a7-c64b3962354e"))
IDismConfiguration2 : IDismConfiguration
{
      virtual HRESULT __stdcall get_ExternalStack (
        /*[out,retval]*/ BSTR * StackPath ) = 0;
      virtual HRESULT __stdcall put_ExternalStack (
        /*[in]*/ BSTR StackPath ) = 0;
};

struct __declspec(uuid("20599df3-1a1b-4eaa-a53f-77f39438b542"))
IDismProperty : IUnknown
{
      virtual HRESULT __stdcall get_Name (
        /*[out,retval]*/ BSTR * Name ) = 0;
      virtual HRESULT __stdcall get_Path (
        /*[out,retval]*/ BSTR * Path ) = 0;
      virtual HRESULT __stdcall get_Value (
        /*[out,retval]*/ BSTR * Value ) = 0;
};

struct __declspec(uuid("88d4518a-41a8-49b0-a087-a3d6fc1d298e"))
IDismPropertyCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismProperty * * ppProperty ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("7e6eb080-3384-4155-a4a7-5e43bbafb2f3"))
IDismStringCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ BSTR * pValue ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("2114daec-42fd-4847-8a04-5f2dde3276a3"))
IDismProviderPrivate : IDismObject
{};

struct __declspec(uuid("8e1c59a6-272f-4212-a6d3-8dc6cddf813a"))
IDismRegistry : IDispatch
{
      virtual HRESULT __stdcall GetKeyPath (
        /*[in]*/ DISM_REGISTRY RegHive,
        /*[out,retval]*/ BSTR * KeyPath ) = 0;
};

struct __declspec(uuid("0b49a069-56a5-48eb-9d9f-e9e42478149a"))
IDismOSServiceManager : IDispatch
{
      virtual HRESULT __stdcall get_State (
        /*[out,retval]*/ DISM_OS_STATE * State ) = 0;
      virtual HRESULT __stdcall get_OSVersion (
        /*[out,retval]*/ BSTR * Version ) = 0;
      virtual HRESULT __stdcall get_Architecture (
        /*[out,retval]*/ long * Architecture ) = 0;
      virtual HRESULT __stdcall get_DismImage (
        /*[out,retval]*/ struct IDismImage * * DismImage ) = 0;
      virtual HRESULT __stdcall get_ServicingStackDirectory (
        /*[out,retval]*/ BSTR * ServicingStackDirectory ) = 0;
      virtual HRESULT __stdcall get_WindowsDirectory (
        /*[out,retval]*/ BSTR * WindowsDirectory ) = 0;
      virtual HRESULT __stdcall get_ProgramFilesDirectory (
        /*[out,retval]*/ BSTR * ProgramFilesDirectory ) = 0;
      virtual HRESULT __stdcall get_BootDrive (
        /*[out,retval]*/ BSTR * BootDrive ) = 0;
      virtual HRESULT __stdcall get_SystemPath (
        /*[out,retval]*/ BSTR * SystemPath ) = 0;
      virtual HRESULT __stdcall SetWindowsDirectory (
        /*[in]*/ BSTR WindowsDirectory ) = 0;
      virtual HRESULT __stdcall SetSystemPath (
        /*[in]*/ BSTR SystemPath ) = 0;
      virtual HRESULT __stdcall MountRegistry (
        /*[out,retval]*/ struct IDismRegistry * * DismRegistry ) = 0;
};

struct __declspec(uuid("c8292388-8d1d-4c34-9a41-8763fcd304f7"))
IDismServicingProvider : IDismObject
{
      virtual HRESULT __stdcall Initialize (
        /*[in]*/ struct IDismOSServiceManager * OSServiceManager,
        /*[in]*/ BSTR ScratchDirectory ) = 0;
      virtual HRESULT __stdcall Finalize ( ) = 0;
      virtual HRESULT __stdcall ValidateServiceability ( ) = 0;
};

struct __declspec(uuid("e2c60889-8e0f-4a21-9f5b-e80f0b13c8d8"))
IDismItemCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * Count ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ IUnknown * * Item ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * Enum ) = 0;
};

struct __declspec(uuid("f316031b-3c98-4f20-b9cb-a3b8702dddd8"))
IDismItemManager : IDismServicingProvider
{
      virtual HRESULT __stdcall AddItem (
        /*[in]*/ BSTR ItemLocation ) = 0;
      virtual HRESULT __stdcall RemoveItemByName (
        /*[in]*/ BSTR ItemName ) = 0;
      virtual HRESULT __stdcall RemoveItem (
        /*[in]*/ IUnknown * Item ) = 0;
      virtual HRESULT __stdcall OpenItem (
        /*[in]*/ BSTR ItemLocation,
        /*[out,retval]*/ IUnknown * * Item ) = 0;
      virtual HRESULT __stdcall GetItems (
        /*[out,retval]*/ struct IDismItemCollection * * Item ) = 0;
};

struct __declspec(uuid("dadad7bb-2b1d-4edb-9014-b1912e1a2e3d"))
IDismSupportUnattend : IDispatch
{
      virtual HRESULT __stdcall Apply (
        /*[in]*/ BSTR UnattendPath ) = 0;
};

struct __declspec(uuid("5bed6686-eeb7-40b7-97f9-fabcc5efaec2"))
IDismToken : IDispatch
{
      virtual HRESULT __stdcall get_Option (
        /*[out,retval]*/ BSTR * Option ) = 0;
      virtual HRESULT __stdcall get_Argument (
        /*[out,retval]*/ BSTR * Argument ) = 0;
};

struct __declspec(uuid("407c0f0f-0540-4900-afd2-52925246dcb3"))
IDismTokenCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismToken * * ppToken ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("c112610a-5d2a-4d52-8082-edcd5a5eb11d"))
IDismCommand : IUnknown
{
      virtual HRESULT __stdcall get_Name (
        /*[out,retval]*/ BSTR * pbstrName ) = 0;
      virtual HRESULT __stdcall get_providerName (
        /*[out,retval]*/ BSTR * pbstrProviderName ) = 0;
};

struct __declspec(uuid("8f609999-5d3c-48a8-aadf-5f96cb297279"))
IDismCommandCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismCommand * * ppToken ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("e3f237d6-8f3c-4443-925a-abf44807690c"))
IDismHelpItem : IUnknown
{
      virtual HRESULT __stdcall get_IsCategory (
        /*[out,retval]*/ VARIANT_BOOL * pvbCategory ) = 0;
      virtual HRESULT __stdcall get_TopicCategory (
        /*[out,retval]*/ BSTR * pbstrTopicCategory ) = 0;
      virtual HRESULT __stdcall get_TopicName (
        /*[out,retval]*/ BSTR * pbstrTopicName ) = 0;
      virtual HRESULT __stdcall get_TopicDescription (
        /*[out,retval]*/ BSTR * pbstrTopicDescription ) = 0;
      virtual HRESULT __stdcall get_HeaderText (
        /*[out,retval]*/ BSTR * pbstrHeaderText ) = 0;
      virtual HRESULT __stdcall get_FooterText (
        /*[out,retval]*/ BSTR * pbstrFooterText ) = 0;
      virtual HRESULT __stdcall get_providerName (
        /*[out,retval]*/ BSTR * pbstrProviderName ) = 0;
};

struct __declspec(uuid("14e23a62-cd9f-4e41-abd5-58d534bf374e"))
IDismHelpItemCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismHelpItem * * ppToken ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("7234ec82-c68b-4c4e-ba1d-d333f5b87b37"))
IDismSupportCommands : IUnknown
{
      virtual HRESULT __stdcall ExecuteCmdLine (
        /*[in]*/ struct IDismTokenCollection * Tokens ) = 0;
      virtual HRESULT __stdcall GetCommandCollection (
        /*[out,retval]*/ struct IDismCommandCollection * * ppCommandCollection ) = 0;
      virtual HRESULT __stdcall GetHelpItemCollection (
        /*[out,retval]*/ struct IDismHelpItemCollection * * ppHelpItemCollection ) = 0;
};

struct __declspec(uuid("fe104c14-65e6-43b3-aa56-f07b5e57d81f"))
IDismProviderStorePrivate : IDismObject
{
      virtual HRESULT __stdcall Initialize (
        /*[in]*/ struct IDismProviderStorePrivate * pStore ) = 0;
      virtual HRESULT __stdcall GetProviderLocation (
        /*[in]*/ BSTR providerName,
        /*[out,retval]*/ BSTR * ProviderLocation ) = 0;
      virtual HRESULT __stdcall RegisterProvider (
        /*[in]*/ struct IDismProvider * Provider ) = 0;
};

struct __declspec(uuid("c9e39092-53df-42b6-89b4-fba12662ead1"))
IDismHostManager : IDispatch
{
      virtual HRESULT __stdcall CreateObjectFromDLL (
        /*[in]*/ BSTR DLLPath,
        /*[in]*/ GUID * ClassIdentifier,
        /*[in]*/ GUID * riid,
        /*[out,retval]*/ IUnknown * * ppUnknown ) = 0;
      virtual HRESULT __stdcall put_ExtendedLogInfo (
        /*[in]*/ BSTR WdsCorePath,
        /*[in]*/ BSTR LogPath,
        /*[in]*/ DISM_LOGLEVEL_TYPE _arg3 ) = 0;
      virtual HRESULT __stdcall SetSynchronizationContext (
        /*[in]*/ BSTR SyncObjectName,
        /*[in]*/ HANDLE ProcessHandle ) = 0;
};

struct __declspec(uuid("6a7dee63-129c-41e8-b6a4-bd1eb1d85e65"))
IDismCapability : IDispatch
{
      virtual HRESULT __stdcall get_Name (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_Id (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_State (
        /*[out,retval]*/ DISM_INSTALL_STATE * pVal ) = 0;
      virtual HRESULT __stdcall get_DownloadSize (
        /*[out,retval]*/ unsigned long * pVal ) = 0;
      virtual HRESULT __stdcall get_InstallSize (
        /*[out,retval]*/ unsigned long * pVal ) = 0;
      virtual HRESULT __stdcall get_DisplayName (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_Description (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall Install ( ) = 0;
      virtual HRESULT __stdcall Uninstall ( ) = 0;
};

struct __declspec(uuid("9b4784da-e322-4bf0-800b-28bae357ec71"))
IDismCapabilityCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismCapability * * ppCapability ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("f8ef44d0-94f1-4dfb-bad8-7b13830ba06e"))
IDismMsuPackage : IDispatch
{
      virtual HRESULT __stdcall get_Path (
        /*[out,retval]*/ BSTR * Path ) = 0;
      virtual HRESULT __stdcall Install ( ) = 0;
};

struct __declspec(uuid("46ce4564-e88a-4eae-bd7c-da221f61e0c7"))
IDismComponentStoreReport : IDispatch
{
      virtual HRESULT __stdcall get_WinSxSVolatileSize (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_WinSxSNonVolatileSize (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_WinSxSSharedWithWindowsSize (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_WinSxSAccordingToExplorerSize (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_LastScavengeDateTime (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_SupersededPackageCount (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_ComponentCleanupRecommended (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_ActualSize (
        /*[out,retval]*/ BSTR * pVal ) = 0;
};

struct __declspec(uuid("a457b170-c9c3-499b-85bc-59259e92ae4d"))
IDismServicePackScavenge : IDispatch
{
      virtual HRESULT __stdcall SPScavenge (
        /*[in]*/ VARIANT_BOOL bHideSpSpecified ) = 0;
      virtual HRESULT __stdcall SPScavengeable (
        /*[out]*/ VARIANT_BOOL * bScavenge,
        /*[out]*/ long * plNumberOfSupersededSP ) = 0;
      virtual HRESULT __stdcall IsSystemPending (
        /*[out,retval]*/ VARIANT_BOOL * pbPending ) = 0;
      virtual HRESULT __stdcall GetCurrentEdition (
        /*[out]*/ BSTR * pszCurrentEdition,
        /*[out]*/ long * plEditionsCount ) = 0;
};

struct __declspec(uuid("c1817aef-4351-4721-9a62-5deda9da246c"))
IDismXmlPackageManager : IDispatch
{
      virtual HRESULT __stdcall CheckXmlPackageApplicability (
        /*[in]*/ BSTR MetadataBlob,
        /*[out]*/ VARIANT_BOOL * bApplicable,
        /*[out]*/ VARIANT_BOOL * bInstalled ) = 0;
};

struct __declspec(uuid("a3ccf844-82c9-431e-8d29-5bd09f8c9b70"))
IDismDeviceIdCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("e7627187-c11b-42fc-91db-4f46bd545374"))
IUnattend : IDispatch
{
      virtual HRESULT __stdcall get_Path (
        /*[out,retval]*/ BSTR * pVal ) = 0;
};

struct __declspec(uuid("517cdc5a-7a77-4356-81ca-d39f50658726"))
IUnattendManager : IDispatch
{
      virtual HRESULT __stdcall OpenUnattend (
        /*[in]*/ BSTR UnattendPath,
        /*[out,retval]*/ struct IUnattend * * ppUnattend ) = 0;
      virtual HRESULT __stdcall Validate (
        /*[in]*/ struct IUnattend * pUnattend ) = 0;
      virtual HRESULT __stdcall Apply (
        /*[in]*/ struct IUnattend * pUnattend,
        /*[in]*/ unsigned long Flags ) = 0;
};

struct __declspec(uuid("f8a8aef3-1322-4e1d-bbf1-0a228c6fe193"))
IUnattendSettings : IDispatch
{
      virtual HRESULT __stdcall get_Settings (
        /*[out,retval]*/ unsigned long * pSettings ) = 0;
};

struct __declspec(uuid("092187e4-09b8-46a3-87d5-1c9bc6ed8b5d"))
IDismCompatManager : IDispatch
{
      virtual HRESULT __stdcall CompatExecuteCmdLine (
        /*[in]*/ struct IDismTokenCollection * Tokens,
        /*[in]*/ BSTR bszPkgMgrExe,
        /*[in]*/ BSTR bszImagePath,
        /*[in]*/ BSTR bszWindir,
        /*[in]*/ VARIANT_BOOL vbQuiet,
        /*[in]*/ VARIANT_BOOL vbNoRestart ) = 0;
};

struct __declspec(uuid("eced57df-5d67-440a-a3cd-b4f1b8b39d1a"))
IDismIntlManager : IDispatch
{};

struct __declspec(uuid("574fd8f3-bc64-455e-a0d8-bcf95d8819b1"))
IDismIntl : IDispatch
{
      virtual HRESULT __stdcall GetEffectiveSystemUILanguage (
        /*[out,retval]*/ BSTR * pbstrLanguage ) = 0;
      virtual HRESULT __stdcall GetInstallLanguage (
        /*[out,retval]*/ BSTR * pbstrLanguage ) = 0;
      virtual HRESULT __stdcall SetAllIntlSettings (
        /*[in]*/ BSTR bstrLanguage ) = 0;
      virtual HRESULT __stdcall SetSkuIntlDefaults (
        /*[in]*/ BSTR bstrLanguage ) = 0;
};

struct __declspec(uuid("30eb8593-fa91-4948-adf5-dc6a354f1873"))
IPEImageManager : IDispatch
{
      virtual HRESULT __stdcall get_ScratchSpace (
        /*[out,retval]*/ unsigned long * pulScratchSpace ) = 0;
      virtual HRESULT __stdcall put_ScratchSpace (
        /*[in]*/ unsigned long pulScratchSpace ) = 0;
      virtual HRESULT __stdcall get_InstallRoot (
        /*[out,retval]*/ BSTR * pInstallRoot ) = 0;
      virtual HRESULT __stdcall put_InstallRoot (
        /*[in]*/ BSTR pInstallRoot ) = 0;
      virtual HRESULT __stdcall get_Profiling (
        /*[out,retval]*/ ProfileStatus * pStatus ) = 0;
      virtual HRESULT __stdcall put_Profiling (
        /*[in]*/ ProfileStatus pStatus ) = 0;
      virtual HRESULT __stdcall Prep ( ) = 0;
      virtual HRESULT __stdcall ApplyProfile (
        /*[in]*/ BSTR Profiles ) = 0;
};

struct __declspec(uuid("b629eef0-2060-43a0-a180-7d04351c3903"))
ITransmogrifyEditionCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * plCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long lIndex,
        /*[out,retval]*/ BSTR * pbstrEditionId ) = 0;
      virtual HRESULT __stdcall get_Index (
        /*[in]*/ BSTR bstrEditionId,
        /*[out,retval]*/ long * plCount ) = 0;
      virtual HRESULT __stdcall Add (
        /*[in]*/ BSTR bstrEditionId ) = 0;
};

struct __declspec(uuid("05bd25af-f5d6-4246-9f91-94b187bc2bf4"))
ITransmogrify : IDispatch
{
      virtual HRESULT __stdcall CanTransmogrify (
        /*[in]*/ BSTR EditionId,
        /*[in]*/ BSTR ProductKey ) = 0;
      virtual HRESULT __stdcall DoTransmogrify (
        /*[in]*/ BSTR EditionId,
        /*[in]*/ BSTR ProductKey ) = 0;
      virtual HRESULT __stdcall GetCurrentEdition (
        /*[out,retval]*/ BSTR * pEditionId ) = 0;
      virtual HRESULT __stdcall CreateEditionCollection (
        /*[out,retval]*/ struct ITransmogrifyEditionCollection * * ppEditionCollection ) = 0;
      virtual HRESULT __stdcall GetTargetEditions (
        /*[out,retval]*/ struct ITransmogrifyEditionCollection * * ppTargetEditions ) = 0;
      virtual HRESULT __stdcall GetStagedEditions (
        /*[out,retval]*/ struct ITransmogrifyEditionCollection * * ppStagedEditions ) = 0;
      virtual HRESULT __stdcall RemoveStagedEditions (
        /*[in]*/ struct ITransmogrifyEditionCollection * pStagedEditions ) = 0;
};

struct __declspec(uuid("c6abc167-33b8-4a00-be58-12ec5b013598"))
ITransmogrify2 : ITransmogrify
{
      virtual HRESULT __stdcall SetProductKey (
        /*[in]*/ BSTR ProductKey ) = 0;
};

struct __declspec(uuid("50f2ddd7-90ed-4db1-8eba-3c995486aa0c"))
ITransmogrify3 : ITransmogrify2
{
      virtual HRESULT __stdcall GetProductKeyInfo (
        /*[in]*/ BSTR ProductKey,
        /*[out]*/ BSTR * pEditionId,
        /*[out,retval]*/ BSTR * pProductKeyType ) = 0;
      virtual HRESULT __stdcall ValidateProductKey (
        /*[in]*/ BSTR ProductKey ) = 0;
};

struct __declspec(uuid("e6757b45-14fc-42c1-9943-ac630a8b0e1b"))
ITransmogrify4 : ITransmogrify3
{
      virtual HRESULT __stdcall DoFootprintCleanup ( ) = 0;
};

struct __declspec(uuid("3f483e2e-f737-481e-a3a5-c64e53e493ae"))
ITransmogrify5 : ITransmogrify4
{
      virtual HRESULT __stdcall DoTransmogrify2 (
        /*[in]*/ BSTR EditionId,
        /*[in]*/ BSTR Channel ) = 0;
      virtual HRESULT __stdcall GetTargetCompositionEditions (
        /*[out,retval]*/ struct ITransmogrifyEditionCollection * * ppTargetCompositionEditions ) = 0;
      virtual HRESULT __stdcall GetTargetVirtualEditions (
        /*[in]*/ BSTR TargetCompositionEdition,
        /*[out,retval]*/ struct ITransmogrifyEditionCollection * * ppTargetVirtualEditions ) = 0;
};

struct __declspec(uuid("57ab0039-5829-4ebf-b093-0b3a80867c24"))
IDismMsiManager : IDispatch
{};

struct __declspec(uuid("e1b47f29-955c-49a5-b0a5-5018207fde69"))
IDismIBSSupport : IDispatch
{
      virtual HRESULT __stdcall SetFirstBootCommandLine (
        /*[in]*/ DISM_FIRST_BOOT_PHASE phase,
        /*[in]*/ BSTR commandLine ) = 0;
};

struct __declspec(uuid("40e0e56d-3d1d-4f5c-b325-0121423677c6"))
IDismAppxPackageInfo : IDispatch
{
      virtual HRESULT __stdcall get_Name (
        /*[out,retval]*/ BSTR * Value ) = 0;
      virtual HRESULT __stdcall get_PublisherId (
        /*[out,retval]*/ BSTR * Value ) = 0;
      virtual HRESULT __stdcall get_Version (
        /*[out,retval]*/ unsigned __int64 * Value ) = 0;
      virtual HRESULT __stdcall get_Architecture (
        /*[out,retval]*/ unsigned long * Value ) = 0;
      virtual HRESULT __stdcall get_ResourceId (
        /*[out,retval]*/ BSTR * Value ) = 0;
      virtual HRESULT __stdcall get_PackageMoniker (
        /*[out,retval]*/ BSTR * Value ) = 0;
      virtual HRESULT __stdcall get_InstallLocation (
        /*[out,retval]*/ BSTR * Value ) = 0;
};

struct __declspec(uuid("75cae0d9-716d-4e62-931d-bdfa3dbbd42e"))
IDismAppxPackageInfoCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * Count ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismAppxPackageInfo * * appxPackageInfo ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * newEnum ) = 0;
};

struct __declspec(uuid("54103f66-7cb7-41ba-ad1a-2e124d01c08c"))
IDismAppxManager : IDispatch
{
      virtual HRESULT __stdcall AddAllUserAppxLooseFiles (
        /*[in]*/ BSTR appPath,
        /*[in]*/ BSTR customDataPath,
        /*[in]*/ VARIANT_BOOL skipLicenseCheck ) = 0;
      virtual HRESULT __stdcall AddAllUserAppxPackage (
        /*[in]*/ BSTR mainPackagePath,
        /*[in]*/ struct IDismStringCollection * dependencyPackagePaths,
        /*[in]*/ BSTR customDataPath,
        /*[in]*/ BSTR licensePath,
        /*[in]*/ VARIANT_BOOL skipLicenseCheck ) = 0;
      virtual HRESULT __stdcall GetAllUserAppx (
        /*[out]*/ struct IDismAppxPackageInfoCollection * * packageCollection ) = 0;
      virtual HRESULT __stdcall RemoveAllUserAppx (
        /*[in]*/ BSTR mainPackageMoniker ) = 0;
};

struct __declspec(uuid("80f63941-9464-4495-bac3-51a49b20ce80"))
IDismAppxManager2 : IDismAppxManager
{
      virtual HRESULT __stdcall AddProvisionedAppxDataFile (
        /*[in]*/ BSTR customDataPath,
        /*[in]*/ BSTR packageFullName ) = 0;
};

struct __declspec(uuid("6674fe44-de28-4b7e-978d-ab6a51ce0a5b"))
IDismAppxManager3 : IDismAppxManager2
{
      virtual HRESULT __stdcall AddAllUserAppxPackageWithOptionalPackages (
        /*[in]*/ BSTR mainPackagePath,
        /*[in]*/ struct IDismStringCollection * dependencyPackagePaths,
        /*[in]*/ struct IDismStringCollection * optionalPackagePaths,
        /*[in]*/ BSTR customDataPath,
        /*[in]*/ struct IDismStringCollection * licensePaths,
        /*[in]*/ VARIANT_BOOL skipLicenseCheck ) = 0;
};

struct __declspec(uuid("7446ff66-999d-497a-b712-a26059730c5d"))
IDismAppxManager4 : IDismAppxManager3
{
      virtual HRESULT __stdcall OptimizeAllUserAppx ( ) = 0;
};

struct __declspec(uuid("9b31f137-5668-441f-8bc3-d21f8d3c70db"))
IDismAppxManager5 : IDismAppxManager4
{
      virtual HRESULT __stdcall AddAllUserAppxPackageWithOptionalPackagesAndRegion (
        /*[in]*/ BSTR mainPackagePath,
        /*[in]*/ struct IDismStringCollection * dependencyPackagePaths,
        /*[in]*/ struct IDismStringCollection * optionalPackagePaths,
        /*[in]*/ BSTR customDataPath,
        /*[in]*/ struct IDismStringCollection * licensePaths,
        /*[in]*/ VARIANT_BOOL skipLicenseCheck,
        /*[in]*/ BSTR region ) = 0;
};

struct __declspec(uuid("b83f048e-2309-4e52-965b-4ff56b3f5ab7"))
IDismAppxManager6 : IDismAppxManager5
{
      virtual HRESULT __stdcall AddAppxUninstallBlocklistEntry (
        /*[in]*/ BSTR packageFamilyName ) = 0;
      virtual HRESULT __stdcall RemoveAppxUninstallBlocklistEntry (
        /*[in]*/ BSTR packageFamilyName ) = 0;
      virtual HRESULT __stdcall GetNonRemovableAppPolicy (
        /*[out]*/ struct IDismStringCollection * * packageFamilyNames ) = 0;
};

struct __declspec(uuid("5367d43f-aa59-4a81-9068-36c7a360fe72"))
IDismAppxPackageInfo2 : IDismAppxPackageInfo
{
      virtual HRESULT __stdcall get_Regions (
        /*[out,retval]*/ BSTR * Value ) = 0;
};

struct __declspec(uuid("dced1a68-143b-4c8c-8451-f155bde821db"))
IDismAssocSupport : IDispatch
{
      virtual HRESULT __stdcall ExportAppAssoc (
        /*[in]*/ BSTR bstrFile ) = 0;
      virtual HRESULT __stdcall ImportAppAssoc (
        /*[in]*/ BSTR bstrFile ) = 0;
      virtual HRESULT __stdcall GetAppAssoc (
        /*[out]*/ BSTR * pbstrOut ) = 0;
      virtual HRESULT __stdcall RemoveAppAssoc ( ) = 0;
};

struct __declspec(uuid("ed08f15d-d5f2-40bf-b08b-73a8456ca62d"))
IDismKcacheManager : IDispatch
{
      virtual HRESULT __stdcall GetStringValue (
        /*[in]*/ BSTR bstrName,
        /*[out,retval]*/ BSTR * pbstrVal ) = 0;
      virtual HRESULT __stdcall GetDwordValue (
        /*[in]*/ BSTR bstrName,
        /*[out,retval]*/ unsigned long * pVal ) = 0;
      virtual HRESULT __stdcall GetBinaryValue (
        /*[in]*/ BSTR bstrName,
        /*[out,retval]*/ SAFEARRAY * * pbVal ) = 0;
};

struct __declspec(uuid("16fcf50b-1eb0-4ed3-9a5c-8a33c80f0d83"))
IDismSetMachineName : IDispatch
{
      virtual HRESULT __stdcall SetMachineName (
        /*[in]*/ BSTR machineName ) = 0;
};

struct __declspec(uuid("0ab3ac34-c644-4903-aa5f-5b5d8f15d969"))
IDismGenericManager : IDispatch
{
      virtual HRESULT __stdcall Run (
        /*[in]*/ struct IDismTokenCollection * Tokens ) = 0;
};

struct __declspec(uuid("db46267b-2fea-4bb5-9f14-e4e582e4777f"))
IDismSysprep : IDispatch
{
      virtual HRESULT __stdcall Stage ( ) = 0;
      virtual HRESULT __stdcall Cleanup (
        /*[in]*/ SYSPREP_RESEAL_TARGET Reseal,
        /*[in]*/ BSTR Mode ) = 0;
      virtual HRESULT __stdcall Generalize (
        /*[in]*/ SYSPREP_RESEAL_TARGET Reseal,
        /*[in]*/ BSTR Mode,
        /*[in]*/ BSTR UnattendPath ) = 0;
      virtual HRESULT __stdcall Specialize (
        /*[in]*/ BSTR UnattendPath ) = 0;
};

struct __declspec(uuid("8e65fbac-20ad-4075-bd7b-377112ff1f69"))
IDismSysprepManager : IDispatch
{
      virtual HRESULT __stdcall Run (
        /*[in]*/ struct IDismTokenCollection * Tokens ) = 0;
};

struct __declspec(uuid("53219e4f-44c6-4ed4-83f9-065fdee95795"))
IDismSetupPlatform : IDispatch
{
      virtual HRESULT __stdcall InitiateUninstall (
        /*[in]*/ unsigned long UninstallReason ) = 0;
      virtual HRESULT __stdcall RemoveUninstall ( ) = 0;
      virtual HRESULT __stdcall GetUninstallWindow (
        /*[out]*/ unsigned long * UninstallWindow ) = 0;
      virtual HRESULT __stdcall SetUninstallWindow (
        /*[in]*/ unsigned long UninstallWindow ) = 0;
};

struct __declspec(uuid("aa668c08-675c-49cb-a127-2cd57a173c04"))
IDismSetupPlatformManager : IDispatch
{
      virtual HRESULT __stdcall Run (
        /*[in]*/ struct IDismTokenCollection * Tokens ) = 0;
};

struct __declspec(uuid("924d876e-17f7-4a5e-ac33-918c2bea0fd5"))
IDismFfuProvider : IDispatch
{
      virtual HRESULT __stdcall ApplyImage (
        /*[in]*/ BSTR ImagePath,
        /*[in]*/ BSTR ApplyPath,
        /*[in]*/ BSTR PartPath ) = 0;
      virtual HRESULT __stdcall SplitImage (
        /*[in]*/ BSTR ImagePath,
        /*[in]*/ BSTR PartPath,
        /*[in]*/ unsigned __int64 PartSize ) = 0;
      virtual HRESULT __stdcall CaptureImage (
        /*[in]*/ BSTR CapturePath,
        /*[in]*/ BSTR ImagePath,
        /*[in]*/ BSTR Name,
        /*[in]*/ BSTR Description,
        /*[in]*/ BSTR PlatformIds,
        /*[in]*/ unsigned short CompressionAlgorithm ) = 0;
};

struct __declspec(uuid("a192440b-5e7b-4538-a48b-3dfe9e043086"))
IDismFfuProvider2 : IDismFfuProvider
{
      virtual HRESULT __stdcall OptimizeImage (
        /*[in]*/ BSTR ImagePath,
        /*[in]*/ int PartitionNumber ) = 0;
};

struct __declspec(uuid("f7a983d3-e787-40a8-9572-cd04ff99d001"))
IDismProvPackageElement : IDispatch
{
      virtual HRESULT __stdcall get_Type (
        /*[out,retval]*/ DISM_ELEMENT_TYPE * pVal ) = 0;
      virtual HRESULT __stdcall GetDismPropertyCollection (
        /*[out,retval]*/ struct IDismPropertyCollection * * PropertyCollection ) = 0;
};

struct __declspec(uuid("3ca3a1b2-ff8c-487a-a0bc-95327ccabd7b"))
IDismProvPackageElementCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismProvPackageElement * * ppPackage ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("446e7afc-05e3-4eea-9e28-d8d6b324a8c7"))
IDismProvisioningSupport : IDispatch
{
      virtual HRESULT __stdcall GetElementCollection (
        /*[in]*/ BSTR bstrPackagePath,
        /*[out,retval]*/ struct IDismProvPackageElementCollection * * ppElementCollection ) = 0;
      virtual HRESULT __stdcall ApplyProvPackage (
        /*[in]*/ BSTR bstrPackagePath ) = 0;
};

struct __declspec(uuid("a0efd55a-17a8-413d-a53a-3c43aef96c0e"))
_IDismEvents : IDispatch
{};

struct __declspec(uuid("be33a527-42fe-43a4-ac5e-c1d059fc705f"))
DismManager;
    // interface IDismConfiguration
    // interface IDismConfiguration2
    // [ default ] interface IDismManager
    // interface IDismManagerPrivate
    // interface IDismEventManager
    // [ default, source ] dispinterface _IDismEvents

struct __declspec(uuid("dee9a285-b7f6-4c16-b651-7a13af90f42c"))
IDismPackage : IDispatch
{
      virtual HRESULT __stdcall get_Identity (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_ReleaseType (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_State (
        /*[out,retval]*/ DISM_INSTALL_STATE * pVal ) = 0;
      virtual HRESULT __stdcall get_Name (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_Description (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_RestartRequired (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_ProductName (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_ProductVersion (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_Company (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_Copyright (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_SupportInformation (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_CreationTime (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_LastUpdateTime (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_InstallTime (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_InstallPackageName (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_InstallLocation (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_InstallClient (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_InstallUserName (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_IsApplicable (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall Install ( ) = 0;
      virtual HRESULT __stdcall Remove ( ) = 0;
      virtual HRESULT __stdcall Stage ( ) = 0;
      virtual HRESULT __stdcall OpenFeature (
        /*[in]*/ BSTR UpdateName,
        /*[out,retval]*/ struct IDismPackageFeature * * PackageFeature ) = 0;
      virtual HRESULT __stdcall GetPackageFeatureCollection (
        /*[out,retval]*/ struct IDismPackageFeatureCollection * * ppPackageFeatures ) = 0;
      virtual HRESULT __stdcall GetDismPropertyCollection (
        /*[out,retval]*/ struct IDismPropertyCollection * * PropertyCollection ) = 0;
};

struct __declspec(uuid("5f81a121-c3c5-4e94-b7ae-22b532f9f3a8"))
IDismPackage2 : IDismPackage
{
      virtual HRESULT __stdcall get_FullyOfflineInstallable (
        /*[out,retval]*/ DISM_PACKAGE_FULLY_OFFLINE_INSTALLABLE_STATE * pVal ) = 0;
      virtual HRESULT __stdcall get_ScavengeSequence (
        /*[out,retval]*/ unsigned int * piScavengeSequence ) = 0;
      virtual HRESULT __stdcall get_CreationFileTime (
        /*[out,retval]*/ BSTR * pFileTime ) = 0;
      virtual HRESULT __stdcall get_LastUpdateFileTime (
        /*[out,retval]*/ BSTR * pFileTime ) = 0;
      virtual HRESULT __stdcall get_InstallFileTime (
        /*[out,retval]*/ BSTR * pFileTime ) = 0;
};

struct __declspec(uuid("ec622249-ba0c-405b-a83c-4a31bc7f0c4d"))
IDismPackage3 : IDismPackage2
{
      virtual HRESULT __stdcall get_CapabilityId (
        /*[out,retval]*/ BSTR * pId ) = 0;
      virtual HRESULT __stdcall get_IsSSU (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
};

struct __declspec(uuid("a27155b5-6831-4427-b0d0-972d58f20263"))
IDismPackage4 : IDismPackage3
{
      virtual HRESULT __stdcall InstallEx (
        /*[in]*/ DISM_PACKAGE_INSTALL_OPTION InstallOption ) = 0;
};

struct __declspec(uuid("d80d838a-0d57-428c-b898-265a7d244a67"))
IDismPackageCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismPackage * * ppPackage ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("3bc3752e-74c5-46f0-b50c-12ff269d5bb1"))
IDismPackageFeature : IDispatch
{
      virtual HRESULT __stdcall get_Name (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_State (
        /*[out,retval]*/ DISM_INSTALL_STATE * pVal ) = 0;
      virtual HRESULT __stdcall get_DisplayName (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_Description (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_DisplayFile (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_Restart (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_PsfName (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_DownloadSize (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_SetMembership (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_RawParentPackage (
        /*[out,retval]*/ struct IDismPackage * * pVal ) = 0;
      virtual HRESULT __stdcall Enable ( ) = 0;
      virtual HRESULT __stdcall Disable ( ) = 0;
      virtual HRESULT __stdcall GetParentFeatureCollection (
        /*[out,retval]*/ struct IDismPackageFeatureCollection * * pVal ) = 0;
      virtual HRESULT __stdcall GetDismPropertyCollection (
        /*[out,retval]*/ struct IDismPropertyCollection * * PropertyCollection ) = 0;
};

struct __declspec(uuid("63e0af82-28e4-4145-ae9a-aadd715c1251"))
IDismPackageFeatureCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismPackageFeature * * ppDriver ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("1c551557-21c2-4907-be82-35d7eef5bb13"))
IDismPackageFeature2 : IDismPackageFeature
{
      virtual HRESULT __stdcall EnableAll ( ) = 0;
      virtual HRESULT __stdcall DisableEx (
        /*[in]*/ unsigned int Options ) = 0;
      virtual HRESULT __stdcall get_CustomPropertyXMLBlob (
        /*[out,retval]*/ BSTR * pVal ) = 0;
};

struct __declspec(uuid("1754aa4f-f0e2-4692-a802-d51871085f34"))
IDismPackageManager : IDispatch
{
      virtual HRESULT __stdcall get_IsCompressionEnabled (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall get_IsImageUnserviceable (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall OpenPackageByPath (
        /*[in]*/ BSTR PackageLocation,
        /*[out,retval]*/ struct IDismPackage * * Package ) = 0;
      virtual HRESULT __stdcall OpenPackageByName (
        /*[in]*/ BSTR PackageName,
        /*[out,retval]*/ struct IDismPackage * * Package ) = 0;
      virtual HRESULT __stdcall OpenFoundationPackage (
        /*[out,retval]*/ struct IDismPackage * * Package ) = 0;
      virtual HRESULT __stdcall OpenMsuPackage (
        /*[in]*/ BSTR PackageLocation,
        /*[out,retval]*/ struct IDismMsuPackage * * Package ) = 0;
      virtual HRESULT __stdcall OpenPublicFeature (
        /*[in]*/ BSTR FeatureName,
        /*[out,retval]*/ struct IDismPackageFeature * * Feature ) = 0;
      virtual HRESULT __stdcall GetPackageCollection (
        /*[out,retval]*/ struct IDismPackageCollection * * ppPackages ) = 0;
      virtual HRESULT __stdcall GetPublicFeatureCollection (
        /*[out,retval]*/ struct IDismPackageFeatureCollection * * ppPackageFeatures ) = 0;
      virtual HRESULT __stdcall ProcessChanges ( ) = 0;
      virtual HRESULT __stdcall ProcessChangesWithOptions (
        /*[in]*/ unsigned int Options ) = 0;
      virtual HRESULT __stdcall ProcessChangesPostReboot ( ) = 0;
      virtual HRESULT __stdcall Scavenge ( ) = 0;
      virtual HRESULT __stdcall RevertPendingActions ( ) = 0;
};

struct __declspec(uuid("5cc9ed95-cf41-4685-beaa-70531031fec4"))
IDismPackageManager2 : IDismPackageManager
{
      virtual HRESULT __stdcall get_IsStoreCorrupt (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall get_LastCBSSessionID (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall get_CBSSessionState (
        /*[in]*/ BSTR Id,
        /*[out]*/ VARIANT_BOOL * Completed,
        /*[out,retval]*/ HRESULT * pVal ) = 0;
      virtual HRESULT __stdcall get_CleanupSize (
        /*[out,retval]*/ unsigned long * pulSizeInMB ) = 0;
      virtual HRESULT __stdcall put_PreventPending (
        /*[in]*/ VARIANT_BOOL _arg1 ) = 0;
      virtual HRESULT __stdcall AddSourcePath (
        BSTR Path ) = 0;
      virtual HRESULT __stdcall ClearSourcePaths ( ) = 0;
      virtual HRESULT __stdcall ScanHealth ( ) = 0;
      virtual HRESULT __stdcall RestoreHealth (
        /*[in]*/ VARIANT_BOOL LimitAccess ) = 0;
      virtual HRESULT __stdcall StartComponentCleanup (
        /*[in]*/ VARIANT_BOOL ResetBase ) = 0;
      virtual HRESULT __stdcall WindowsUpdateCleanup ( ) = 0;
};

struct __declspec(uuid("e40dd35a-89e1-4619-ad21-ef97d29b5d00"))
IDismPackageManager3 : IDismPackageManager2
{
      virtual HRESULT __stdcall AnalyzeComponentStore (
        /*[out,retval]*/ struct IDismComponentStoreReport * * ppReport ) = 0;
};

struct __declspec(uuid("4dfd752a-1e5e-4c70-94ff-9c91b5f61335"))
IDismPackageManager4 : IDismPackageManager3
{
      virtual HRESULT __stdcall GetCapabilityCollection (
        /*[in]*/ DISM_ON_DEMAND_SOURCE_FILTER SourceFilter,
        /*[out,retval]*/ struct IDismCapabilityCollection * * ppCapabilities ) = 0;
      virtual HRESULT __stdcall StartComponentCleanupEx (
        /*[in]*/ VARIANT_BOOL ResetBase,
        /*[in]*/ DISM_RESETBASE_OPTION ResetBaseOption ) = 0;
};

struct __declspec(uuid("bc8687e4-4b69-4749-878d-6def2746fc36"))
IDismPackageManager5 : IDismPackageManager4
{
      virtual HRESULT __stdcall SetProperty (
        DISM_CONFIGURABLE_PROPERTY Property,
        BSTR PropertyValue ) = 0;
      virtual HRESULT __stdcall PerformOperation (
        /*[in]*/ unsigned int Options,
        /*[in]*/ DISM_CBS_OPERATION_TYPE OperationType ) = 0;
};

struct __declspec(uuid("6a064b65-3cec-491b-a2d4-8d1d4da6decc"))
IDismDriverManager : IDispatch
{
      virtual HRESULT __stdcall GetDriverPackageCollection (
        /*[in]*/ DRIVER_COLL_OUTPUT CollType,
        /*[out,retval]*/ struct IDismDriverPackageCollection * * DriverPackages ) = 0;
      virtual HRESULT __stdcall OpenPackageByFile (
        /*[in]*/ BSTR PackageInf,
        /*[out,retval]*/ struct IDismDriverPackage * * DriverPackage ) = 0;
      virtual HRESULT __stdcall OpenPackageByFolder (
        /*[in]*/ BSTR PackageFolder,
        /*[in]*/ VARIANT_BOOL SearchSubFolders,
        /*[out]*/ struct IDismErrorCollection * * Errors,
        /*[out,retval]*/ struct IDismDriverPackageCollection * * DriverPackages ) = 0;
      virtual HRESULT __stdcall OpenPackageInStore (
        /*[in]*/ BSTR PackageName,
        /*[out,retval]*/ struct IDismDriverPackage * * DriverPackage ) = 0;
};

struct __declspec(uuid("45554af5-6058-4913-8e5f-c9b79d5995cf"))
IDismDriverPackageCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismDriverPackage * * Driver ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("1af61d22-d513-4417-8838-61f8724edc4f"))
IDismDriverPackage : IDispatch
{
      virtual HRESULT __stdcall GetDriverCollection (
        /*[in]*/ long Architecture,
        /*[out,retval]*/ struct IDismDriverCollection * * Drivers ) = 0;
      virtual HRESULT __stdcall get_LocaleName (
        /*[out,retval]*/ BSTR * LocaleName ) = 0;
      virtual HRESULT __stdcall get_InfProviderName (
        /*[out,retval]*/ BSTR * Name ) = 0;
      virtual HRESULT __stdcall get_InBox (
        /*[out,retval]*/ VARIANT_BOOL * InBox ) = 0;
      virtual HRESULT __stdcall get_CatalogFile (
        /*[out,retval]*/ BSTR * CatalogFile ) = 0;
      virtual HRESULT __stdcall get_InfPath (
        /*[out,retval]*/ BSTR * InfPath ) = 0;
      virtual HRESULT __stdcall get_PublishedInfName (
        /*[out,retval]*/ BSTR * InfName ) = 0;
      virtual HRESULT __stdcall get_ClassGuid (
        /*[out,retval]*/ BSTR * ClassGuid ) = 0;
      virtual HRESULT __stdcall get_ClassName (
        /*[out,retval]*/ BSTR * ClassName ) = 0;
      virtual HRESULT __stdcall get_ClassDescription (
        /*[out,retval]*/ BSTR * ClassDesc ) = 0;
      virtual HRESULT __stdcall get_BootCritical (
        /*[out,retval]*/ VARIANT_BOOL * BootCritical ) = 0;
      virtual HRESULT __stdcall get_SignatureStatus (
        /*[out,retval]*/ DRIVER_SIG_STATUS * SigStatus ) = 0;
      virtual HRESULT __stdcall GetDate (
        /*[out]*/ long * Year,
        /*[out]*/ long * Month,
        /*[out]*/ long * Day ) = 0;
      virtual HRESULT __stdcall GetVersion (
        /*[out]*/ long * Major,
        /*[out]*/ long * Minor,
        /*[out]*/ long * Build,
        /*[out]*/ long * Revision ) = 0;
      virtual HRESULT __stdcall Install ( ) = 0;
      virtual HRESULT __stdcall Remove ( ) = 0;
};

struct __declspec(uuid("3b6bbfa2-f817-423b-a83c-103ad54615d4"))
IDismDriver : IDispatch
{
      virtual HRESULT __stdcall get_ParentPackage (
        /*[out,retval]*/ struct IDismDriverPackage * * Package ) = 0;
      virtual HRESULT __stdcall get_Manufacturer (
        /*[out,retval]*/ BSTR * Manufacturer ) = 0;
      virtual HRESULT __stdcall get_Description (
        /*[out,retval]*/ BSTR * Description ) = 0;
      virtual HRESULT __stdcall get_HardwareId (
        /*[out,retval]*/ BSTR * HardwareId ) = 0;
      virtual HRESULT __stdcall get_Architecture (
        /*[out,retval]*/ long * Architecture ) = 0;
      virtual HRESULT __stdcall get_ServiceName (
        /*[out,retval]*/ BSTR * HardwareId ) = 0;
      virtual HRESULT __stdcall get_CompatibleIds (
        /*[out,retval]*/ struct IDismDeviceIdCollection * * DeviceIds ) = 0;
      virtual HRESULT __stdcall get_ExcludeIds (
        /*[out,retval]*/ struct IDismDeviceIdCollection * * DeviceIds ) = 0;
};

struct __declspec(uuid("4e1cb72f-c47d-42ee-8cca-4ceb35b3fe17"))
IDismDriverCollection : IDispatch
{
      virtual HRESULT __stdcall get_Count (
        /*[out,retval]*/ long * pnCount ) = 0;
      virtual HRESULT __stdcall get_Item (
        /*[in]*/ long n,
        /*[out,retval]*/ struct IDismDriver * * Driver ) = 0;
      virtual HRESULT __stdcall get__NewEnum (
        /*[out,retval]*/ IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("671fcaaf-cf96-4b46-ac3d-7b968fbbcc3f"))
IDismDriverPackage2 : IDismDriverPackage
{
      virtual HRESULT __stdcall InstallEx (
        /*[in]*/ DRIVER_INSTALL_FLAGS Flags ) = 0;
};

struct __declspec(uuid("04f36a61-a37d-406d-a320-0708444f3e26"))
IDismDriverPackage3 : IDismDriverPackage2
{
      virtual HRESULT __stdcall InstallEx2 (
        /*[in]*/ DRIVER_INSTALL_FLAGS Flags,
        /*[in,out]*/ struct IDismDriverPackage * * StorePackage ) = 0;
      virtual HRESULT __stdcall SetProperty (
        /*[in]*/ struct DEVPROPKEY * PropertyKey,
        /*[in]*/ unsigned long PropertyType,
        /*[in]*/ SAFEARRAY * PropertyBuffer,
        /*[in]*/ unsigned long Flags ) = 0;
      virtual HRESULT __stdcall GetProperty (
        /*[in]*/ struct DEVPROPKEY * PropertyKey,
        /*[in,out]*/ unsigned long * PropertyType,
        /*[in,out]*/ SAFEARRAY * * PropertyBuffer,
        /*[in]*/ unsigned long Flags ) = 0;
};

struct __declspec(uuid("6d9208e3-17b2-46c2-b902-2433cc8cf0fd"))
IDismDriverPackage4 : IDismDriverPackage3
{
      virtual HRESULT __stdcall get_TargetProcessorArchitecture (
        /*[out,retval]*/ long * Architecture ) = 0;
      virtual HRESULT __stdcall put_TargetProcessorArchitecture (
        /*[in]*/ long Architecture ) = 0;
};

extern "C" const GUID __declspec(selectany) IID_IDismLogger =
    {0xbd7fc505,0x69bc,0x49cb,{0x8c,0x58,0x1d,0x1d,0xa5,0x47,0xb7,0xe2}};
extern "C" const GUID __declspec(selectany) IID_IDismObject =
    {0xd8066238,0xfdd5,0x4195,{0x81,0x55,0x23,0x71,0xe0,0xe6,0x1c,0xc5}};
extern "C" const GUID __declspec(selectany) IID_IDismProvider =
    {0x33f93018,0xedb7,0x4092,{0x97,0x8a,0x4b,0x37,0xd5,0xdf,0xb1,0xc0}};
extern "C" const GUID __declspec(selectany) IID_IDismProviderCollection =
    {0x722de115,0x1404,0x4bf8,{0xbc,0xee,0xde,0x0b,0x7b,0x58,0x06,0x71}};
extern "C" const GUID __declspec(selectany) IID_IDismImage =
    {0x9bb62bf9,0x01a0,0x49a7,{0xbe,0x40,0xfb,0x54,0x45,0xb2,0x9b,0xef}};
extern "C" const GUID __declspec(selectany) IID_IDismProviderStore =
    {0x5df7b1b0,0x93a5,0x4f03,{0x8b,0xcb,0xf3,0x93,0x96,0x89,0x94,0x43}};
extern "C" const GUID __declspec(selectany) IID_IDismImageSession =
    {0xa08538a6,0xb9ff,0x47c2,{0xb2,0x28,0xb9,0x36,0x12,0xef,0x11,0x4b}};
extern "C" const GUID __declspec(selectany) IID_IDismManager =
    {0x4d4fce31,0xf767,0x45e4,{0xb2,0xe9,0x54,0x00,0xe3,0xe1,0x38,0x9a}};
extern "C" const GUID __declspec(selectany) IID_IDismManagerPrivate =
    {0x8817cb2a,0xe80a,0x480b,{0xa7,0xb4,0x22,0x32,0x44,0x3a,0x9e,0x4b}};
extern "C" const GUID __declspec(selectany) IID_IDismEventManager =
    {0xd9894135,0x1aa7,0x49af,{0xad,0x11,0x27,0xe7,0x84,0xca,0xc9,0x31}};
extern "C" const GUID __declspec(selectany) IID_IDismError =
    {0x18caabcd,0xeb4c,0x4df3,{0x9f,0xf1,0xb5,0xd2,0x4e,0x65,0x5f,0x33}};
extern "C" const GUID __declspec(selectany) IID_IDismErrorCollection =
    {0x74d82b92,0xae58,0x4962,{0x91,0x33,0x37,0x01,0xe1,0x51,0x0a,0x81}};
extern "C" const GUID __declspec(selectany) IID_IDismImageInfo =
    {0x7970de73,0x63e8,0x43e9,{0x9f,0x61,0x81,0xbd,0xc7,0x48,0xf2,0x9b}};
extern "C" const GUID __declspec(selectany) IID_IDismImageInfoCollection =
    {0x572f3374,0x7dae,0x47ab,{0x88,0xaf,0xbd,0x6c,0xa2,0x92,0x52,0xfe}};
extern "C" const GUID __declspec(selectany) IID_IDismMountedImageInfo =
    {0xee9b6624,0x8acc,0x440c,{0x95,0x64,0xf5,0xd7,0x56,0x8d,0x6e,0x01}};
extern "C" const GUID __declspec(selectany) IID_IDismMountedImageInfoCollection =
    {0x931fc35d,0x0976,0x48b8,{0x8b,0xde,0xb5,0x95,0x4a,0xad,0xd3,0xf2}};
extern "C" const GUID __declspec(selectany) IID_IDismImageManager =
    {0xa654ff2f,0xd9fa,0x4df1,{0xaa,0xbf,0x98,0x2c,0x0a,0x3c,0x5f,0x52}};
extern "C" const GUID __declspec(selectany) IID_IDismImageSessionPrivate =
    {0xff0e7a8b,0x7b7c,0x411b,{0x96,0x8b,0x11,0xaa,0x89,0x8b,0xb0,0x3a}};
extern "C" const GUID __declspec(selectany) IID_IDismImageSessionPrivate2 =
    {0x13a81f76,0x50e2,0x11e0,{0xb7,0x44,0x00,0x12,0x3f,0x3f,0xc6,0xdd}};
extern "C" const GUID __declspec(selectany) IID_IDismConfigObject =
    {0x705a28e2,0xa26f,0x4a43,{0x8d,0x82,0xa5,0x69,0x41,0xb5,0xc2,0x50}};
extern "C" const GUID __declspec(selectany) IID_IDismConfiguration =
    {0x3f92cf95,0x462b,0x4bda,{0xa1,0xd4,0xb6,0xcb,0x74,0xce,0xfb,0x5c}};
extern "C" const GUID __declspec(selectany) IID_IDismConfiguration2 =
    {0x59891552,0x9ac8,0x4c89,{0xb2,0xa7,0xc6,0x4b,0x39,0x62,0x35,0x4e}};
extern "C" const GUID __declspec(selectany) IID_IDismProperty =
    {0x20599df3,0x1a1b,0x4eaa,{0xa5,0x3f,0x77,0xf3,0x94,0x38,0xb5,0x42}};
extern "C" const GUID __declspec(selectany) IID_IDismPropertyCollection =
    {0x88d4518a,0x41a8,0x49b0,{0xa0,0x87,0xa3,0xd6,0xfc,0x1d,0x29,0x8e}};
extern "C" const GUID __declspec(selectany) IID_IDismStringCollection =
    {0x7e6eb080,0x3384,0x4155,{0xa4,0xa7,0x5e,0x43,0xbb,0xaf,0xb2,0xf3}};
extern "C" const GUID __declspec(selectany) IID_IDismProviderPrivate =
    {0x2114daec,0x42fd,0x4847,{0x8a,0x04,0x5f,0x2d,0xde,0x32,0x76,0xa3}};
extern "C" const GUID __declspec(selectany) IID_IDismRegistry =
    {0x8e1c59a6,0x272f,0x4212,{0xa6,0xd3,0x8d,0xc6,0xcd,0xdf,0x81,0x3a}};
extern "C" const GUID __declspec(selectany) IID_IDismOSServiceManager =
    {0x0b49a069,0x56a5,0x48eb,{0x9d,0x9f,0xe9,0xe4,0x24,0x78,0x14,0x9a}};
extern "C" const GUID __declspec(selectany) IID_IDismServicingProvider =
    {0xc8292388,0x8d1d,0x4c34,{0x9a,0x41,0x87,0x63,0xfc,0xd3,0x04,0xf7}};
extern "C" const GUID __declspec(selectany) IID_IDismItemCollection =
    {0xe2c60889,0x8e0f,0x4a21,{0x9f,0x5b,0xe8,0x0f,0x0b,0x13,0xc8,0xd8}};
extern "C" const GUID __declspec(selectany) IID_IDismItemManager =
    {0xf316031b,0x3c98,0x4f20,{0xb9,0xcb,0xa3,0xb8,0x70,0x2d,0xdd,0xd8}};
extern "C" const GUID __declspec(selectany) IID_IDismSupportUnattend =
    {0xdadad7bb,0x2b1d,0x4edb,{0x90,0x14,0xb1,0x91,0x2e,0x1a,0x2e,0x3d}};
extern "C" const GUID __declspec(selectany) IID_IDismToken =
    {0x5bed6686,0xeeb7,0x40b7,{0x97,0xf9,0xfa,0xbc,0xc5,0xef,0xae,0xc2}};
extern "C" const GUID __declspec(selectany) IID_IDismTokenCollection =
    {0x407c0f0f,0x0540,0x4900,{0xaf,0xd2,0x52,0x92,0x52,0x46,0xdc,0xb3}};
extern "C" const GUID __declspec(selectany) IID_IDismCommand =
    {0xc112610a,0x5d2a,0x4d52,{0x80,0x82,0xed,0xcd,0x5a,0x5e,0xb1,0x1d}};
extern "C" const GUID __declspec(selectany) IID_IDismCommandCollection =
    {0x8f609999,0x5d3c,0x48a8,{0xaa,0xdf,0x5f,0x96,0xcb,0x29,0x72,0x79}};
extern "C" const GUID __declspec(selectany) IID_IDismHelpItem =
    {0xe3f237d6,0x8f3c,0x4443,{0x92,0x5a,0xab,0xf4,0x48,0x07,0x69,0x0c}};
extern "C" const GUID __declspec(selectany) IID_IDismHelpItemCollection =
    {0x14e23a62,0xcd9f,0x4e41,{0xab,0xd5,0x58,0xd5,0x34,0xbf,0x37,0x4e}};
extern "C" const GUID __declspec(selectany) IID_IDismSupportCommands =
    {0x7234ec82,0xc68b,0x4c4e,{0xba,0x1d,0xd3,0x33,0xf5,0xb8,0x7b,0x37}};
extern "C" const GUID __declspec(selectany) IID_IDismProviderStorePrivate =
    {0xfe104c14,0x65e6,0x43b3,{0xaa,0x56,0xf0,0x7b,0x5e,0x57,0xd8,0x1f}};
extern "C" const GUID __declspec(selectany) IID_IDismHostManager =
    {0xc9e39092,0x53df,0x42b6,{0x89,0xb4,0xfb,0xa1,0x26,0x62,0xea,0xd1}};
extern "C" const GUID __declspec(selectany) IID_IDismCapability =
    {0x6a7dee63,0x129c,0x41e8,{0xb6,0xa4,0xbd,0x1e,0xb1,0xd8,0x5e,0x65}};
extern "C" const GUID __declspec(selectany) IID_IDismCapabilityCollection =
    {0x9b4784da,0xe322,0x4bf0,{0x80,0x0b,0x28,0xba,0xe3,0x57,0xec,0x71}};
extern "C" const GUID __declspec(selectany) IID_IDismMsuPackage =
    {0xf8ef44d0,0x94f1,0x4dfb,{0xba,0xd8,0x7b,0x13,0x83,0x0b,0xa0,0x6e}};
extern "C" const GUID __declspec(selectany) IID_IDismComponentStoreReport =
    {0x46ce4564,0xe88a,0x4eae,{0xbd,0x7c,0xda,0x22,0x1f,0x61,0xe0,0xc7}};
extern "C" const GUID __declspec(selectany) IID_IDismServicePackScavenge =
    {0xa457b170,0xc9c3,0x499b,{0x85,0xbc,0x59,0x25,0x9e,0x92,0xae,0x4d}};
extern "C" const GUID __declspec(selectany) IID_IDismXmlPackageManager =
    {0xc1817aef,0x4351,0x4721,{0x9a,0x62,0x5d,0xed,0xa9,0xda,0x24,0x6c}};
extern "C" const GUID __declspec(selectany) IID_IDismDeviceIdCollection =
    {0xa3ccf844,0x82c9,0x431e,{0x8d,0x29,0x5b,0xd0,0x9f,0x8c,0x9b,0x70}};
extern "C" const GUID __declspec(selectany) IID_IUnattend =
    {0xe7627187,0xc11b,0x42fc,{0x91,0xdb,0x4f,0x46,0xbd,0x54,0x53,0x74}};
extern "C" const GUID __declspec(selectany) IID_IUnattendManager =
    {0x517cdc5a,0x7a77,0x4356,{0x81,0xca,0xd3,0x9f,0x50,0x65,0x87,0x26}};
extern "C" const GUID __declspec(selectany) IID_IUnattendSettings =
    {0xf8a8aef3,0x1322,0x4e1d,{0xbb,0xf1,0x0a,0x22,0x8c,0x6f,0xe1,0x93}};
extern "C" const GUID __declspec(selectany) IID_IDismCompatManager =
    {0x092187e4,0x09b8,0x46a3,{0x87,0xd5,0x1c,0x9b,0xc6,0xed,0x8b,0x5d}};
extern "C" const GUID __declspec(selectany) IID_IDismIntlManager =
    {0xeced57df,0x5d67,0x440a,{0xa3,0xcd,0xb4,0xf1,0xb8,0xb3,0x9d,0x1a}};
extern "C" const GUID __declspec(selectany) IID_IDismIntl =
    {0x574fd8f3,0xbc64,0x455e,{0xa0,0xd8,0xbc,0xf9,0x5d,0x88,0x19,0xb1}};
extern "C" const GUID __declspec(selectany) IID_IPEImageManager =
    {0x30eb8593,0xfa91,0x4948,{0xad,0xf5,0xdc,0x6a,0x35,0x4f,0x18,0x73}};
extern "C" const GUID __declspec(selectany) IID_ITransmogrifyEditionCollection =
    {0xb629eef0,0x2060,0x43a0,{0xa1,0x80,0x7d,0x04,0x35,0x1c,0x39,0x03}};
extern "C" const GUID __declspec(selectany) IID_ITransmogrify =
    {0x05bd25af,0xf5d6,0x4246,{0x9f,0x91,0x94,0xb1,0x87,0xbc,0x2b,0xf4}};
extern "C" const GUID __declspec(selectany) IID_ITransmogrify2 =
    {0xc6abc167,0x33b8,0x4a00,{0xbe,0x58,0x12,0xec,0x5b,0x01,0x35,0x98}};
extern "C" const GUID __declspec(selectany) IID_ITransmogrify3 =
    {0x50f2ddd7,0x90ed,0x4db1,{0x8e,0xba,0x3c,0x99,0x54,0x86,0xaa,0x0c}};
extern "C" const GUID __declspec(selectany) IID_ITransmogrify4 =
    {0xe6757b45,0x14fc,0x42c1,{0x99,0x43,0xac,0x63,0x0a,0x8b,0x0e,0x1b}};
extern "C" const GUID __declspec(selectany) IID_ITransmogrify5 =
    {0x3f483e2e,0xf737,0x481e,{0xa3,0xa5,0xc6,0x4e,0x53,0xe4,0x93,0xae}};
extern "C" const GUID __declspec(selectany) IID_IDismMsiManager =
    {0x57ab0039,0x5829,0x4ebf,{0xb0,0x93,0x0b,0x3a,0x80,0x86,0x7c,0x24}};
extern "C" const GUID __declspec(selectany) IID_IDismIBSSupport =
    {0xe1b47f29,0x955c,0x49a5,{0xb0,0xa5,0x50,0x18,0x20,0x7f,0xde,0x69}};
extern "C" const GUID __declspec(selectany) IID_IDismAppxPackageInfo =
    {0x40e0e56d,0x3d1d,0x4f5c,{0xb3,0x25,0x01,0x21,0x42,0x36,0x77,0xc6}};
extern "C" const GUID __declspec(selectany) IID_IDismAppxPackageInfoCollection =
    {0x75cae0d9,0x716d,0x4e62,{0x93,0x1d,0xbd,0xfa,0x3d,0xbb,0xd4,0x2e}};
extern "C" const GUID __declspec(selectany) IID_IDismAppxManager =
    {0x54103f66,0x7cb7,0x41ba,{0xad,0x1a,0x2e,0x12,0x4d,0x01,0xc0,0x8c}};
extern "C" const GUID __declspec(selectany) IID_IDismAppxManager2 =
    {0x80f63941,0x9464,0x4495,{0xba,0xc3,0x51,0xa4,0x9b,0x20,0xce,0x80}};
extern "C" const GUID __declspec(selectany) IID_IDismAppxManager3 =
    {0x6674fe44,0xde28,0x4b7e,{0x97,0x8d,0xab,0x6a,0x51,0xce,0x0a,0x5b}};
extern "C" const GUID __declspec(selectany) IID_IDismAppxManager4 =
    {0x7446ff66,0x999d,0x497a,{0xb7,0x12,0xa2,0x60,0x59,0x73,0x0c,0x5d}};
extern "C" const GUID __declspec(selectany) IID_IDismAppxManager5 =
    {0x9b31f137,0x5668,0x441f,{0x8b,0xc3,0xd2,0x1f,0x8d,0x3c,0x70,0xdb}};
extern "C" const GUID __declspec(selectany) IID_IDismAppxManager6 =
    {0xb83f048e,0x2309,0x4e52,{0x96,0x5b,0x4f,0xf5,0x6b,0x3f,0x5a,0xb7}};
extern "C" const GUID __declspec(selectany) IID_IDismAppxPackageInfo2 =
    {0x5367d43f,0xaa59,0x4a81,{0x90,0x68,0x36,0xc7,0xa3,0x60,0xfe,0x72}};
extern "C" const GUID __declspec(selectany) IID_IDismAssocSupport =
    {0xdced1a68,0x143b,0x4c8c,{0x84,0x51,0xf1,0x55,0xbd,0xe8,0x21,0xdb}};
extern "C" const GUID __declspec(selectany) IID_IDismKcacheManager =
    {0xed08f15d,0xd5f2,0x40bf,{0xb0,0x8b,0x73,0xa8,0x45,0x6c,0xa6,0x2d}};
extern "C" const GUID __declspec(selectany) IID_IDismSetMachineName =
    {0x16fcf50b,0x1eb0,0x4ed3,{0x9a,0x5c,0x8a,0x33,0xc8,0x0f,0x0d,0x83}};
extern "C" const GUID __declspec(selectany) IID_IDismGenericManager =
    {0x0ab3ac34,0xc644,0x4903,{0xaa,0x5f,0x5b,0x5d,0x8f,0x15,0xd9,0x69}};
extern "C" const GUID __declspec(selectany) IID_IDismSysprep =
    {0xdb46267b,0x2fea,0x4bb5,{0x9f,0x14,0xe4,0xe5,0x82,0xe4,0x77,0x7f}};
extern "C" const GUID __declspec(selectany) IID_IDismSysprepManager =
    {0x8e65fbac,0x20ad,0x4075,{0xbd,0x7b,0x37,0x71,0x12,0xff,0x1f,0x69}};
extern "C" const GUID __declspec(selectany) IID_IDismSetupPlatform =
    {0x53219e4f,0x44c6,0x4ed4,{0x83,0xf9,0x06,0x5f,0xde,0xe9,0x57,0x95}};
extern "C" const GUID __declspec(selectany) IID_IDismSetupPlatformManager =
    {0xaa668c08,0x675c,0x49cb,{0xa1,0x27,0x2c,0xd5,0x7a,0x17,0x3c,0x04}};
extern "C" const GUID __declspec(selectany) IID_IDismFfuProvider =
    {0x924d876e,0x17f7,0x4a5e,{0xac,0x33,0x91,0x8c,0x2b,0xea,0x0f,0xd5}};
extern "C" const GUID __declspec(selectany) IID_IDismFfuProvider2 =
    {0xa192440b,0x5e7b,0x4538,{0xa4,0x8b,0x3d,0xfe,0x9e,0x04,0x30,0x86}};
extern "C" const GUID __declspec(selectany) IID_IDismProvPackageElement =
    {0xf7a983d3,0xe787,0x40a8,{0x95,0x72,0xcd,0x04,0xff,0x99,0xd0,0x01}};
extern "C" const GUID __declspec(selectany) IID_IDismProvPackageElementCollection =
    {0x3ca3a1b2,0xff8c,0x487a,{0xa0,0xbc,0x95,0x32,0x7c,0xca,0xbd,0x7b}};
extern "C" const GUID __declspec(selectany) IID_IDismProvisioningSupport =
    {0x446e7afc,0x05e3,0x4eea,{0x9e,0x28,0xd8,0xd6,0xb3,0x24,0xa8,0xc7}};
extern "C" const GUID __declspec(selectany) DIID__IDismEvents =
    {0xa0efd55a,0x17a8,0x413d,{0xa5,0x3a,0x3c,0x43,0xae,0xf9,0x6c,0x0e}};
extern "C" const GUID __declspec(selectany) CLSID_DismManager =
    {0xbe33a527,0x42fe,0x43a4,{0xac,0x5e,0xc1,0xd0,0x59,0xfc,0x70,0x5f}};
extern "C" const GUID __declspec(selectany) IID_IDismPackage =
    {0xdee9a285,0xb7f6,0x4c16,{0xb6,0x51,0x7a,0x13,0xaf,0x90,0xf4,0x2c}};
extern "C" const GUID __declspec(selectany) IID_IDismPackage2 =
    {0x5f81a121,0xc3c5,0x4e94,{0xb7,0xae,0x22,0xb5,0x32,0xf9,0xf3,0xa8}};
extern "C" const GUID __declspec(selectany) IID_IDismPackage3 =
    {0xec622249,0xba0c,0x405b,{0xa8,0x3c,0x4a,0x31,0xbc,0x7f,0x0c,0x4d}};
extern "C" const GUID __declspec(selectany) IID_IDismPackage4 =
    {0xa27155b5,0x6831,0x4427,{0xb0,0xd0,0x97,0x2d,0x58,0xf2,0x02,0x63}};
extern "C" const GUID __declspec(selectany) IID_IDismPackageCollection =
    {0xd80d838a,0x0d57,0x428c,{0xb8,0x98,0x26,0x5a,0x7d,0x24,0x4a,0x67}};
extern "C" const GUID __declspec(selectany) IID_IDismPackageFeature =
    {0x3bc3752e,0x74c5,0x46f0,{0xb5,0x0c,0x12,0xff,0x26,0x9d,0x5b,0xb1}};
extern "C" const GUID __declspec(selectany) IID_IDismPackageFeatureCollection =
    {0x63e0af82,0x28e4,0x4145,{0xae,0x9a,0xaa,0xdd,0x71,0x5c,0x12,0x51}};
extern "C" const GUID __declspec(selectany) IID_IDismPackageFeature2 =
    {0x1c551557,0x21c2,0x4907,{0xbe,0x82,0x35,0xd7,0xee,0xf5,0xbb,0x13}};
extern "C" const GUID __declspec(selectany) IID_IDismPackageManager =
    {0x1754aa4f,0xf0e2,0x4692,{0xa8,0x02,0xd5,0x18,0x71,0x08,0x5f,0x34}};
extern "C" const GUID __declspec(selectany) IID_IDismPackageManager2 =
    {0x5cc9ed95,0xcf41,0x4685,{0xbe,0xaa,0x70,0x53,0x10,0x31,0xfe,0xc4}};
extern "C" const GUID __declspec(selectany) IID_IDismPackageManager3 =
    {0xe40dd35a,0x89e1,0x4619,{0xad,0x21,0xef,0x97,0xd2,0x9b,0x5d,0x00}};
extern "C" const GUID __declspec(selectany) IID_IDismPackageManager4 =
    {0x4dfd752a,0x1e5e,0x4c70,{0x94,0xff,0x9c,0x91,0xb5,0xf6,0x13,0x35}};
extern "C" const GUID __declspec(selectany) IID_IDismPackageManager5 =
    {0xbc8687e4,0x4b69,0x4749,{0x87,0x8d,0x6d,0xef,0x27,0x46,0xfc,0x36}};
extern "C" const GUID __declspec(selectany) IID_IDismDriverManager =
    {0x6a064b65,0x3cec,0x491b,{0xa2,0xd4,0x8d,0x1d,0x4d,0xa6,0xde,0xcc}};
extern "C" const GUID __declspec(selectany) IID_IDismDriverPackageCollection =
    {0x45554af5,0x6058,0x4913,{0x8e,0x5f,0xc9,0xb7,0x9d,0x59,0x95,0xcf}};
extern "C" const GUID __declspec(selectany) IID_IDismDriverPackage =
    {0x1af61d22,0xd513,0x4417,{0x88,0x38,0x61,0xf8,0x72,0x4e,0xdc,0x4f}};
extern "C" const GUID __declspec(selectany) IID_IDismDriver =
    {0x3b6bbfa2,0xf817,0x423b,{0xa8,0x3c,0x10,0x3a,0xd5,0x46,0x15,0xd4}};
extern "C" const GUID __declspec(selectany) IID_IDismDriverCollection =
    {0x4e1cb72f,0xc47d,0x42ee,{0x8c,0xca,0x4c,0xeb,0x35,0xb3,0xfe,0x17}};
extern "C" const GUID __declspec(selectany) IID_IDismDriverPackage2 =
    {0x671fcaaf,0xcf96,0x4b46,{0xac,0x3d,0x7b,0x96,0x8f,0xbb,0xcc,0x3f}};
extern "C" const GUID __declspec(selectany) IID_IDismDriverPackage3 =
    {0x04f36a61,0xa37d,0x406d,{0xa3,0x20,0x07,0x08,0x44,0x4f,0x3e,0x26}};
extern "C" const GUID __declspec(selectany) IID_IDismDriverPackage4 =
    {0x6d9208e3,0x17b2,0x46c2,{0xb9,0x02,0x24,0x33,0xcc,0x8c,0xf0,0xfd}};

#pragma pack(pop)

#endif // !DISM_CORE_H
