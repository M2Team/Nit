#include "VL_System.h"

namespace vl
{
	namespace system
	{

/*********************************************************************************************************
VFileName
*********************************************************************************************************/

	VUnicodeString VFileName::_GetPath()
	{
		VInt Index=FFileName.Length()-1;
		while(Index>=0)
		{
			if(FFileName[Index]==L'\\')
			{
				break;
			}
			Index--;
		}
		return FFileName.Left(Index+1);
	}

	VUnicodeString VFileName::_GetName()
	{
		VInt Index=FFileName.Length()-1;
		while(Index>=0)
		{
			if(FFileName[Index]==L'\\')
			{
				break;
			}
			Index--;
		}
		return FFileName.Right(FFileName.Length()-1-Index);
	}

	VFileName::VFileName(VMbcsString FileName)
	{
		FFileName=ToUnicode(FileName);
	}

	VFileName::VFileName(VUnicodeString FileName)
	{
		FFileName=FileName;
	}

	VFileName::VFileName(PChar FileName)
	{
		FFileName=ToUnicode(FileName);
	}

	VFileName::VFileName(PWChar FileName)
	{
		FFileName=FileName;
	}

	VFileName& VFileName::operator = (VFileName FileName)
	{
		FFileName=FileName.FFileName;
		return *this;
	}

	VFileName& VFileName::operator = (VUnicodeString FileName)
	{
		FFileName=FileName;
		return *this;
	}

	VFileName& VFileName::operator = (VMbcsString FileName)
	{
		FFileName=ToUnicode(FileName);
		return *this;
	}

	VFileName VFileName::MakeRelative(VFileName Dest)
	{
		VFileName Refer=FFileName;
		VUnicodeString Backs;
		while(Refer.FFileName!=Dest.FFileName.Left(Refer.FFileName.Length()))
		{
			Backs+=L"..\\";
			Refer.Back(1);
		}
		return Backs+Dest.FFileName.Right(Dest.FFileName.Length()-Refer.FFileName.Length());
	}

	VFileName VFileName::MakeAbsolute(VFileName Dest)
	{
		VFileName Refer=FFileName;
		while(Dest.FFileName.Left(3)==L"..\\")
		{
			Refer.Back(1);
			Dest.FFileName.Delete(0,3);
		}
		return Refer.FFileName+Dest.FFileName;
	}

	void VFileName::Back(VInt Count)
	{
		for(VInt i=0;i<Count;i++)
		{
			FFileName=_GetPath();
			if(FFileName.Length())
			{
				FFileName.Delete(FFileName.Length()-1,1);
				FFileName=_GetPath();
			}
		}
	}

	void VFileName::Join(VFileName Part)
	{
		FFileName+=Part.GetStrW();
	}

	VFileName VFileName::GetPath()
	{
		return _GetPath();
	}

	VFileName VFileName::GetName()
	{
		return _GetName();
	}

	VMbcsString VFileName::GetStrA()
	{
		return ToMbcs(FFileName);
	}

	VUnicodeString VFileName::GetStrW()
	{
		return FFileName;
	}

/*********************************************************************************************************
VTime
*********************************************************************************************************/

	VTime::VTime(time_t SecTime)
	{
		operator=(SecTime);
	}

	VTime::VTime(tm CrtTime)
	{
		operator=(CrtTime);
	}

	VTime::VTime(FILETIME FileTime)
	{
		operator=(FileTime);
	}

	VTime::VTime(SYSTEMTIME SystemTime)
	{
		operator=(SystemTime);
	}

	VTime::VTime(VTime& Time)
	{
		operator=(Time);
	}

	VTime::VTime()
	{
	}

	VTime& VTime::operator=(time_t SecTime)
	{
		tm t;
		localtime_s(&t,&SecTime);
		operator=(t);
		return *this;
	}

	VTime& VTime::operator=(tm CrtTime)
	{
		Year=CrtTime.tm_year+1900;
		Month=CrtTime.tm_mon+1;
		Day=CrtTime.tm_mday;
		Hour=CrtTime.tm_hour;
		Minute=CrtTime.tm_min;
		Second=CrtTime.tm_sec;
		WeekDay=CrtTime.tm_wday;
		MSec=0;
		return *this;
	}

	VTime& VTime::operator=(FILETIME FileTime)
	{
		SYSTEMTIME SystemTime;
		FileTimeToSystemTime(&FileTime,&SystemTime);
		operator=(SystemTime);
		return *this;
	}

	VTime& VTime::operator=(SYSTEMTIME SystemTime)
	{
		Year=SystemTime.wYear;
		Month=SystemTime.wMonth;
		Day=SystemTime.wDay;
		Hour=SystemTime.wHour;
		Minute=SystemTime.wMinute;
		Second=SystemTime.wSecond;
		WeekDay=SystemTime.wDayOfWeek;
		MSec=SystemTime.wMilliseconds;
		return *this;
	}

	VTime& VTime::operator=(VTime& Time)
	{
		Year=Time.Year;
		Month=Time.Month;
		Day=Time.Day;
		Hour=Time.Hour;
		Minute=Time.Minute;
		Second=Time.Second;
		WeekDay=Time.WeekDay;
		MSec=Time.MSec;
		return *this;
	}

	void VTime::PickTime()
	{
		SYSTEMTIME dt;
		GetLocalTime(&dt);
		operator=(dt);
	}

	VUnicodeString VTime::DateString()
	{
		return VUnicodeString(Year)+L"年"+VUnicodeString(Month)+L"月"+VUnicodeString(Day)+L"日";
	}

	VUnicodeString VTime::TimeString(VBool UseMSec)
	{
		return VUnicodeString(Hour)+L"时"+VUnicodeString(Minute)+L"分"+VUnicodeString(Second)+L"秒"+(UseMSec?VUnicodeString(MSec)+L"毫秒":L"");
	}

	VUnicodeString VTime::WeekString()
	{
		PWChar Weeks[7]={L"星期天",L"星期一",L"星期二",L"星期三",L"星期四",L"星期五",L"星期六"};
		return Weeks[WeekDay];
	}

	namespace synchronization
	{

/*********************************************************************************************************
VL_SynCriticalSection
*********************************************************************************************************/
		
	VL_SynCriticalSection::VL_SynCriticalSection()
	{
		InitializeCriticalSection(&FCriticalSection);
	}

	VL_SynCriticalSection::~VL_SynCriticalSection()
	{
		DeleteCriticalSection(&FCriticalSection);
	}

	void VL_SynCriticalSection::Enter()
	{
		EnterCriticalSection(&FCriticalSection);
	}

	VBool VL_SynCriticalSection::TryEnter()
	{
		return TryEnterCriticalSection(&FCriticalSection)!=0;
	}

	void VL_SynCriticalSection::Leave()
	{
		LeaveCriticalSection(&FCriticalSection);
	}

/*********************************************************************************************************
VL_SynObject
*********************************************************************************************************/

	VL_SynObject::AccessResult VL_SynObject::CreateObject(VPointer Parameter , VUnicodeString Name)
	{
		Close();
		HANDLE Handle=CreateHandle(Parameter,(Name.Length()?Name.Buffer():NULL));
		if(Handle)
		{
			FHandle=Handle;
			FNeedToClose=true;
			Name;
			if(GetLastError()==ERROR_ALREADY_EXISTS)
			{
				FAlreadyExists=true;
				return VL_SynObject::arAlreadyExists;
			}
			else
			{
				return VL_SynObject::arSucceed;
			}
		}
		else
		{
			return VL_SynObject::arFail;
		}
	}

	VL_SynObject::AccessResult VL_SynObject::OpenObject(VPointer Parameter , VUnicodeString Name)
	{
		Close();
		HANDLE Handle=OpenHandle(Parameter,Name.Buffer());
		if(Handle)
		{
			FHandle=Handle;
			FAlreadyExists=true;
			FNeedToClose=true;
			Name;
			return VL_SynObject::arAlreadyExists;
		}
		else
		{
			return VL_SynObject::arFail;
		}
	}

	VL_SynObject::VL_SynObject()
	{
		FHandle=false;
		FAlreadyExists=false;
		FNeedToClose=false;
	}

	VL_SynObject::~VL_SynObject()
	{
		Close();
	}

	HANDLE VL_SynObject::GetHandle()
	{
		return FHandle;
	}

	VL_SynObject::WaitResult VL_SynObject::WaitFor(DWORD Milliseconds)
	{
		switch(WaitForSingleObject(FHandle,Milliseconds))
		{
		case WAIT_OBJECT_0:
			return wrSignaled;
		case WAIT_TIMEOUT:
			return wrTimeOut;
		default:
			return wrAbandoned;
		}
	}

	VBool VL_SynObject::IsSignaled()
	{
		return WaitFor(0)==wrSignaled;
	}
	
	VBool VL_SynObject::IsAlreadyExists()
	{
		return FAlreadyExists;
	}

	VBool VL_SynObject::IsReady()
	{
		return FHandle!=NULL;
	}

	void VL_SynObject::Close()
	{
		if(FNeedToClose)
		{
			CloseHandle(FHandle);
		}
		FHandle=NULL;
		FAlreadyExists=false;
		FNeedToClose=false;
		FName=L"";
	}
	VUnicodeString VL_SynObject::GetName()
	{
		return FName;
	}

	void WaitForSynObjects(VL_SynObject::List& SynObjects)
	{
		HANDLE* Handles=new HANDLE[SynObjects.GetCount()];
		for(VInt i=0;i<SynObjects.GetCount();i++)
		{
			Handles[i]=SynObjects[i]->GetHandle();
		}
		WaitForMultipleObjects(SynObjects.GetCount(),Handles,TRUE,INFINITE);
		delete[] Handles;
	}

	VBool WaitForSynObjectsTimeOut(VL_SynObject::List& SynObjects , DWORD Milliseconds)
	{
		HANDLE* Handles=new HANDLE[SynObjects.GetCount()];
		for(VInt i=0;i<SynObjects.GetCount();i++)
		{
			Handles[i]=SynObjects[i]->GetHandle();
		}
		VBool TimeOut=WaitForMultipleObjects(SynObjects.GetCount(),Handles,TRUE,Milliseconds)==WAIT_TIMEOUT;
		delete[] Handles;
		return !TimeOut;
	}

/*********************************************************************************************************
VL_SynEvent
*********************************************************************************************************/

	class VL_SynEvent_CreateParameter
	{
	public:
		VBool Signal;
		VBool AutoReset;
	};

	HANDLE VL_SynEvent::CreateHandle(VPointer Parameter , PWChar Name)
	{
		VL_SynEvent_CreateParameter* P=(VL_SynEvent_CreateParameter*)Parameter;
		return CreateEvent(NULL,(P->AutoReset?FALSE:TRUE),(P->Signal?TRUE:FALSE),Name);
	}

	HANDLE VL_SynEvent::OpenHandle(VPointer Parameter , PWChar Name)
	{
		return OpenEvent(SYNCHRONIZE|EVENT_MODIFY_STATE,FALSE,Name);
	}

	VL_SynObject::AccessResult VL_SynEvent::Create(VBool Signal , VBool AutoReset , VUnicodeString Name)
	{
		VL_SynEvent_CreateParameter Parameter;
		Parameter.Signal=Signal;
		Parameter.AutoReset=AutoReset;
		return CreateObject(&Parameter,Name);
	}

	VL_SynObject::AccessResult VL_SynEvent::Open(VUnicodeString Name)
	{
		return OpenObject(NULL,Name);
	}

	VBool VL_SynEvent::Signal()
	{
		return SetEvent(FHandle)!=0;
	}

	VBool VL_SynEvent::Unsignal()
	{
		return ResetEvent(FHandle)!=0;
	}

/*********************************************************************************************************
VL_SynMutex
*********************************************************************************************************/

	class VL_SynMutex_CreateParameter
	{
	public:
		VBool Own;
	};

	HANDLE VL_SynMutex::CreateHandle(VPointer Parameter , PWChar Name)
	{
		VL_SynMutex_CreateParameter* P=(VL_SynMutex_CreateParameter*)Parameter;
		return CreateMutex(NULL,(P->Own?TRUE:FALSE),Name);
	}

	HANDLE VL_SynMutex::OpenHandle(VPointer Parameter , PWChar Name)
	{
		return OpenMutex(SYNCHRONIZE|MUTEX_MODIFY_STATE,FALSE,Name);
	}

	VL_SynObject::AccessResult VL_SynMutex::Create(VBool Own , VUnicodeString Name)
	{
		VL_SynMutex_CreateParameter Parameter;
		Parameter.Own=Own;
		return CreateObject(&Parameter,Name);
	}

	VL_SynObject::AccessResult VL_SynMutex::Open(VUnicodeString Name)
	{
		return OpenObject(NULL,Name);
	}

	VBool VL_SynMutex::IsOwned()
	{
		return WaitFor(0)==wrTimeOut;
	}

	VBool VL_SynMutex::Request()
	{
		return WaitFor()==wrSignaled;
	}

	void VL_SynMutex::Release()
	{
		ReleaseMutex(FHandle);
	}

/*********************************************************************************************************
VL_SynSemaphore
*********************************************************************************************************/

	class VL_SynSemaphore_CreateParameter
	{
	public:
		VInt Init;
		VInt Max;
	};

	HANDLE VL_SynSemaphore::CreateHandle(VPointer Parameter , PWChar Name)
	{
		VL_SynSemaphore_CreateParameter* P=(VL_SynSemaphore_CreateParameter*)Parameter;
		return CreateSemaphore(NULL,P->Init,P->Max,Name);
	}

	HANDLE VL_SynSemaphore::OpenHandle(VPointer Parameter , PWChar Name)
	{
		return OpenSemaphore(SYNCHRONIZE|SEMAPHORE_MODIFY_STATE,FALSE,Name);
	}

	VL_SynObject::AccessResult VL_SynSemaphore::Create(VInt InitCount , VInt MaxCount , VUnicodeString Name)
	{
		VL_SynSemaphore_CreateParameter Parameter;
		Parameter.Init=InitCount;
		Parameter.Max=MaxCount;
		return CreateObject(&Parameter,Name);
	}

	VL_SynObject::AccessResult VL_SynSemaphore::Open(VUnicodeString Name)
	{
		return OpenObject(NULL,Name);
	}

	VBool VL_SynSemaphore::Increase(VInt Count)
	{
		LONG Previous;
		return ReleaseSemaphore(FHandle,Count,&Previous)!=0;
	}

	VBool VL_SynSemaphore::Decrease()
	{
		return WaitFor()==wrSignaled;
	}

	}

/*********************************************************************************************************
VL_Thread
*********************************************************************************************************/

	DWORD WINAPI VL_Thread::ExecuteThread(LPVOID Parameter)
	{
		VL_Thread* Thread=(VL_Thread*)Parameter;
		Thread->Run();
		if(Thread->GetFreeOnTerminated())
		{
			delete Thread;
		}
		return 0;
	}

	VL_Thread::	VL_Thread(VBool CreateSuspended , VBool FreeOnTerminated)
	{
		FSuspended=CreateSuspended;
		FFreeOnTerminated=FreeOnTerminated;
		FNeedToTerminate=false;
		FTerminated=true;

		FHandle=CreateThread(NULL,0,&ExecuteThread,this,CREATE_SUSPENDED,NULL);
		if(!CreateSuspended)
		{
			Execute();
		}
	}

	VL_Thread::~VL_Thread()
	{
		CloseHandle(FHandle);
	}

	void VL_Thread::Execute()
	{
		if(FHandle==NULL)return;
		if(FTerminated)
		{
			FTerminated=false;
			FSuspended=false;
			ResumeThread(FHandle);
			FTerminated=true;
			FSuspended=true;
		}
	}

	void VL_Thread::Suspend()
	{
		if(FHandle==NULL)return;
		if(!FTerminated && !FSuspended)
		{
			SuspendThread(FHandle);
			FSuspended=true;
		}
	}

	void VL_Thread::Resume()
	{
		if(FHandle==NULL)return;
		if(!FTerminated && FSuspended)
		{
			ResumeThread(FHandle);
			FSuspended=false;
		}
	}

	void VL_Thread::WaitFor()
	{
		if(FHandle==NULL)return;
		if(!FTerminated)
		{
			WaitForSingleObject(FHandle,INFINITE);
		}
	}

	VBool VL_Thread::GetFreeOnTerminated()
	{
		return FFreeOnTerminated;
	}

	void VL_Thread::SetFreeOnTerminated(VBool FreeOnTerminated)
	{
		FFreeOnTerminated=FreeOnTerminated;
	}

	VBool VL_Thread::GetNeedToTerminate()
	{
		return FNeedToTerminate;
	}

	void VL_Thread::SetNeedToTerminate(VBool NeedToTerminate)
	{
		FNeedToTerminate=NeedToTerminate;
	}

	VBool VL_Thread::IsSuspended()
	{
		return FSuspended;
	}

	VBool VL_Thread::IsTerminated()
	{
		return FTerminated;
	}

	HANDLE VL_Thread::GetHandle()
	{
		return FHandle;
	}

/*********************************************************************************************************
VL_ProcThread
*********************************************************************************************************/

	void VL_ProcThread::Run()
	{
		FProc(FParameter);
	}

	VL_ProcThread::VL_ProcThread(VLFP_ThreadProc Proc , VPointer Parameter , VBool CreateSuspended , VBool FreeOnTerminated):VL_Thread(CreateSuspended,FreeOnTerminated)
	{
		FProc=Proc;
		FParameter=Parameter;
	}

/*********************************************************************************************************
工具函数
*********************************************************************************************************/

	template<class ThreadList>
	void WaitForThreadList(ThreadList& Threads , VBool Execute)
	{
		HANDLE* Handles=new HANDLE[Threads.GetCount()];
		for(VInt i=0;i<Threads.GetCount();i++)
		{
			Handles[i]=Threads[i]->GetHandle();
		}
		if(Execute)
		{
			for(VInt i=0;i<Threads.GetCount();i++)
			{
				Threads[i]->Execute();
			}
		}
		WaitForMultipleObjects(Threads.GetCount(),Handles,TRUE,INFINITE);
		delete[] Handles;
	}

	void WaitFor(VL_ThreadList& Threads , VBool Execute)
	{
		WaitForThreadList(Threads,Execute);
	}

	void WaitFor(VL_ManagedThreadList& Threads , VBool Execute)
	{
		WaitForThreadList(Threads,Execute);
	}

/*********************************************************************************************************
File System
*********************************************************************************************************/

	VL_Array<VUnicodeString , 1> VFSO_GetLogicalDrives()
	{
		DWORD Size=GetLogicalDriveStrings(0,NULL)+1;
		ApiChar* Buffer=new ApiChar[Size];
		GetLogicalDriveStrings(Size,Buffer);

		ApiChar* Read=Buffer;
		VInt Count=0;
		while(*Read)
		{
			Count++;
			Read+=papilen(Read)+1;
		}
		VL_Array<VUnicodeString , 1> Drives=NewArray[Count];

		Read=Buffer;
		Count=0;
		while(*Read)
		{
			Drives[Count]=apitou(Read);
			Count++;
			Read+=papilen(Read)+1;
		}

		delete[] Buffer;
		return Drives;
	}

	VBool VFSO_GetVolumeInformation(VUnicodeString Path , VLS_VFSO_VolumeInformation& Information)
	{
		ApiChar VolumeFileBuffer[257];
		ApiChar FileSystemBuffer[257];
		DWORD FileSystemFlags;
		DWORD SerialNumber;
		DWORD MaximumComponentLength;
		BOOL Result=GetVolumeInformation(apistr(Path),VolumeFileBuffer,257,&SerialNumber,&MaximumComponentLength,&FileSystemFlags,FileSystemBuffer,257);
		if(Result)
		{
			Information.SerialNumber=SerialNumber;
			Information.MaximumComponentLength=MaximumComponentLength;
			Information.Label=apitou(VolumeFileBuffer);
			Information.FileSystem=apitou(FileSystemBuffer);

			Information.SupportNamedStream		=(FileSystemFlags&FILE_NAMED_STREAMS)		!=0;
			Information.ReadOnly				=(FileSystemFlags&FILE_READ_ONLY_VOLUME)	!=0;
			Information.CaseSensitive			=(FileSystemFlags&FS_CASE_SENSITIVE)		!=0;
			Information.SupportFileCompression	=(FileSystemFlags&FS_FILE_COMPRESSION)		!=0;
			Information.SupportFileEncryption	=(FileSystemFlags&FS_FILE_ENCRYPTION)		!=0;
			Information.SupportUnicodeFileName	=(FileSystemFlags&FS_UNICODE_STORED_ON_DISK)!=0;
			Information.VolumeCompressed		=(FileSystemFlags&FS_VOL_IS_COMPRESSED)		!=0;

			return true;
		}
		else
		{
			return false;
		}
	}

	VLE_VFSO_VolumeType VFSO_GetVolumeType(VUnicodeString Path)
	{
		switch(GetDriveType(apistr(Path)))
		{
		case DRIVE_NO_ROOT_DIR:
			return vvtNoRootDir;
		case DRIVE_REMOVABLE:
			return vvtRemovable;
		case DRIVE_FIXED:
			return vvtFixed;
		case DRIVE_REMOTE:
			return vvtRemote;
		case DRIVE_CDROM:
			return vvtCDRom;
		case DRIVE_RAMDISK:
			return vvtRamDisk;
		default:
			return vvtUnknown;
		}
	}

	VLE_VFSO_CreateDirectory VFSO_CreateDirectory(VUnicodeString Path)
	{
		if(CreateDirectory(apistr(Path),NULL))
		{
			return vcdSucceed;
		}
		else
		{
			if(GetLastError()==ERROR_ALREADY_EXISTS)
			{
				return vcdPathExists;
			}
			else
			{
				return vcdNotFound;
			}
		}
	}

	VBool VFSO_RemoveDirectory(VUnicodeString Path)
	{
		return RemoveDirectory(apistr(Path))!=0;
	}

	VBool VFSO_CopyFile(VUnicodeString OldFile , VUnicodeString NewFile , VBool FailIfExists)
	{
		return CopyFile(apistr(OldFile),apistr(NewFile),FailIfExists)!=0;
	}

	VBool VFSO_MoveFile(VUnicodeString OldFile , VUnicodeString NewFile)
	{
		return MoveFile(apistr(OldFile),apistr(NewFile))!=0;
	}

	VBool VFSO_DeleteFile(VUnicodeString File)
	{
		return DeleteFile(apistr(File))!=0;
	}

	VBool VFSO_GetFileAttributes(VUnicodeString File , VLS_VFSO_FileAttributes& Attributes)
	{
		DWORD Result=GetFileAttributes(apistr(File));
		if(Result==INVALID_FILE_ATTRIBUTES)
		{
			return false;
		}
		else
		{
			Attributes.Archive		=(Result&FILE_ATTRIBUTE_ARCHIVE)		!=0;
			Attributes.Compressed	=(Result&FILE_ATTRIBUTE_COMPRESSED)		!=0;
			Attributes.Directory	=(Result&FILE_ATTRIBUTE_DIRECTORY)		!=0;
			Attributes.Encrypted	=(Result&FILE_ATTRIBUTE_ENCRYPTED)		!=0;
			Attributes.Hidden		=(Result&FILE_ATTRIBUTE_HIDDEN)			!=0;
			Attributes.Readonly		=(Result&FILE_ATTRIBUTE_READONLY)		!=0;
			Attributes.System		=(Result&FILE_ATTRIBUTE_SYSTEM)			!=0;
			Attributes.Temporary	=(Result&FILE_ATTRIBUTE_TEMPORARY)		!=0;
			return true;
		}
	}

	
	VBool VFSO_SetFileAttributes(VUnicodeString File , VLS_VFSO_FileAttributes& Attributes)
	{
		DWORD Att=0;
		if(Attributes.Archive)		Att|=FILE_ATTRIBUTE_ARCHIVE;
		if(Attributes.Hidden)		Att|=FILE_ATTRIBUTE_HIDDEN;
		if(Attributes.Readonly)		Att|=FILE_ATTRIBUTE_READONLY;
		if(Attributes.System)		Att|=FILE_ATTRIBUTE_SYSTEM;
		if(Attributes.Temporary)	Att|=FILE_ATTRIBUTE_TEMPORARY;
		if(Att==0)
		{
			Att=FILE_ATTRIBUTE_NORMAL;
		}
		return SetFileAttributes(apistr(File),Att)!=0;
	}

	VInt64s VFSO_GetFileSize(VUnicodeString File)
	{
		LARGE_INTEGER Size;
		if(GetFileSizeEx(apistr(File),&Size))
		{
			return Size.QuadPart;
		}
		else
		{
			return -1;
		}
	}

	VBool VFSO_DirectoryExists(VUnicodeString File)
	{
		if(File==L"")
		{
			return false;
		}
		if(File.Pos(L"*")>=0 || File.Pos(L"?")>=0)
		{
			return false;
		}
		if(File[File.Length()-1]==L'\\')
		{
			File=File.Left(File.Length()-1);
		}
		WIN32_FIND_DATA Data;
		HANDLE Result=FindFirstFile(apistr(File),&Data);
		if(Result==INVALID_HANDLE_VALUE)
		{
			return false;
		}
		else
		{
			FindClose(Result);
			return (Data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0;
		}
	}

	VBool VFSO_FileExists(VUnicodeString File)
	{
		if(File==L"")
		{
			return false;
		}
		if(File.Pos(L"*")>=0 || File.Pos(L"?")>=0)
		{
			return false;
		}
		WIN32_FIND_DATA Data;
		HANDLE Result=FindFirstFile(apistr(File),&Data);
		if(Result==INVALID_HANDLE_VALUE)
		{
			return false;
		}
		else
		{
			FindClose(Result);
			return (Data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0;
		}
	}

	VBool VFSO_EnumerateFiles(VUnicodeString Path , VL_List<VUnicodeString , false>& Files , VL_List<VUnicodeString , false>& Directories)
	{
		if(Path==L"")
		{
			return false;
		}
		if(Path.Pos(L"*")>=0 || Path.Pos(L"?")>=0)
		{
			return false;
		}
		if(Path[Path.Length()-1]!=L'\\')
		{
			Path+=L"\\*";
		}
		else
		{
			Path+=L"*";
		}
		Files.Clear();
		Directories.Clear();

		WIN32_FIND_DATA Data;
		HANDLE Result=FindFirstFile(apistr(Path),&Data);
		if(Result==INVALID_HANDLE_VALUE)
		{
			return false;
		}
		else
		{
			do
			{
				if((Data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)
				{
					Files.Add(Data.cFileName);
				}
				else
				{
					VUnicodeString Dir=Data.cFileName;
					if(Dir!=L"." && Dir!=L"..")
					{
						Directories.Add(Dir);
					}
				}
			}while(FindNextFile(Result,&Data));
			return true;
		}
	}

	}
}