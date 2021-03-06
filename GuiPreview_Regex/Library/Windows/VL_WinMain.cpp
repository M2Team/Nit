#include "VL_WinMain.h"
#include "..\Data\VL_Data.h"
#include "commctrl.h"

#pragma comment(lib,"comctl32.lib")
#pragma comment(linker,"/manifestdependency:\""		\
	"type='win32' "									\
	"name='Microsoft.Windows.Common-Controls' "		\
	"version='6.0.0.0' "							\
	"processorArchitecture='*' "					\
	"publicKeyToken='6595b64144ccf1df' "			\
	"language='*'\""								\
)

namespace vl
{
	namespace windows
	{
		VL_WinClass*		FDefaultClass=0;
		VL_WinApplication*	FApplication=0;
		WPARAM				FTerminatingValue=0;

/*********************************************************************************************************
VL_WinApplication
*********************************************************************************************************/

	VBool VL_WinApplication::ProcessMessage(VBool InMessageLoop)
	{
		MSG Message;
		VBool Process=false;
		VBool Result=false;
		if(InMessageLoop)
		{
			switch(GetMessage(&Message,NULL,0,0))
			{
			case 0:
				FTerminatingValue=Message.wParam;
				Process=false;
				Result=false;
				break;
			case -1:
				Process=false;
				Result=true;
				break;
			default:
				Process=true;
				Result=true;
			}
		}
		else
		{
			switch(PeekMessage(&Message,NULL,0,0,PM_NOREMOVE))
			{
			case 0:
				Process=false;
				Result=true;
				break;
			default:
				if(Message.message==WM_QUIT)
				{
					Process=false;
					Result=true;
				}
				else
				{
					PeekMessage(&Message,NULL,0,0,PM_REMOVE);
					Process=true;
					Result=true;
				}
			}
		}
		if(Process)
		{
			VL_AcceleratorManager* Manager=GetAcceleratorManager();
			if(Manager)
			{
				TranslateAccelerator(Message.hwnd,Manager->GetHandle(),&Message);
			}
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		return Result;
	}

	VBool VL_WinApplication::RegisterForm(VL_WinForm* Form)
	{
		FForms.AddDistinct(Form);
		if(FMainForm)
		{
			return false;
		}
		else
		{
			FMainForm=Form;
			return true;
		}
	}

	void VL_WinApplication::UnregisterForm(VL_WinForm* Form)
	{
		if(FForms.Exists(Form))
		{
			FForms.Delete(FForms.IndexOf(Form));
		}
	}

	void VL_WinApplication::RunModal(VL_WinForm* Form)
	{
		while(ProcessMessage(true))
		{
			if(Form->GetVisible()==false)
			{
				break;
			}
			Sleep(0);
		}
	}

	VL_WinApplication::VL_WinApplication(HINSTANCE hInstance)
	{
		FInstance=hInstance;
		FMainForm=0;
		
		FPen=new VL_WinPen(PS_SOLID,0,RGB(0,0,0));
		FBrush=new VL_WinBrush(RGB(255,255,255));
		FFont=new VL_WinFont(L"宋体",12,0,0,0,400,false,false,false,true);
	}

	HINSTANCE VL_WinApplication::GetInstance()
	{
		return FInstance;
	}

	VUnicodeString VL_WinApplication::GetAppName()
	{
		VWChar Buffer[1024];
		GetModuleFileName(FInstance,Buffer,sizeof(Buffer)/sizeof(*Buffer));
		return Buffer;
	}

	void VL_WinApplication::Run()
	{
		if(FForms.GetCount()>0)
		{
			while(ProcessMessage(true))
			{
				Sleep(0);
			}
		}
	}

	void VL_WinApplication::DoEvents()
	{
		ProcessMessage(false);
	}

	VL_WinControl* VL_WinApplication::GetControl(HWND Handle)
	{
		VInt Index=FControls.IndexOfKey(Handle);
		if(Index==-1)
		{
			return 0;
		}
		else
		{
			return FControls.ValueOfIndex(Index);
		}
	}

	void VL_WinApplication::Terminate()
	{
		for(VInt i=0;i<FForms.GetCount();i++)
		{
			VL_WinForm* Form=FForms[i];
			if(Form!=FMainForm)
			{
				Form->DestroyForm();
			}
		}
		if(FMainForm)
		{
			FMainForm->DestroyForm();
		}

		for(VInt i=FForms.GetCount()-1;i>=0;i--)
		{
			if(i<FForms.GetCount())
			{
				delete FForms.Fetch(i);
			}
		}
	}

	VL_WinForm* VL_WinApplication::GetMainForm()
	{
		return FMainForm;
	}

	VL_WinPen::Ptr VL_WinApplication::GetDefaultPen()
	{
		return FPen;
	}

	VL_WinBrush::Ptr VL_WinApplication::GetDefaultBrush()
	{
		return FBrush;
	}

	VL_WinFont::Ptr VL_WinApplication::GetDefaultFont()
	{
		return FFont;
	}

/*********************************************************************************************************
工具函数
*********************************************************************************************************/

	VL_WinClass* GetDefaultClass()
	{
		return FDefaultClass;
	}

	VL_WinApplication* GetApplication()
	{
		return FApplication;
	}

	extern void SetCommdlgService(VL_WinCommonDialogService* Service);
	extern void InitAccelerator();
	extern void DestroyAccelerator();

	}
}

using namespace vl;
using namespace vl::windows;

extern void main();

LRESULT CALLBACK SubclassProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam,
	UINT_PTR uIdSubclass,
	DWORD_PTR dwRefData)
{
	VBool CallDefaultProcedure=true;
	LRESULT Result=0;
	VL_WinControl* Control=GetApplication()->GetControl(hWnd);
	if(Control)
	{
		Result=Control->ProcessMessage(uMsg,wParam,lParam,CallDefaultProcedure);
	}
	if(CallDefaultProcedure)
	{
		Result=DefSubclassProc(hWnd,uMsg,wParam,lParam);
	}
	return Result;
}


LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	LRESULT Result=0;
	VBool CallDefaultProcedure=true;
	VL_WinControl* Control=GetApplication()->GetControl(hwnd);
	if(Control)
	{
		Result=Control->ProcessMessage(uMsg,wParam,lParam,CallDefaultProcedure);
	}
	if(CallDefaultProcedure)
	{
		Result=DefWindowProc(hwnd,uMsg,wParam,lParam);
	}
	return Result;
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	{
		InitCommonControls();
		VL_WinClass DefaultClass(L"VczhClass",false,false,WindowProc,hInstance);
		if(DefaultClass.IsAvailable())
		{
			VL_WinApplication			Application(hInstance);
			VL_WinCommonDialogService	CommonDialogService;

			FDefaultClass=&DefaultClass;
			FApplication=&Application;
			SetCommdlgService(&CommonDialogService);

			InitAccelerator();
			main();
			DestroyAccelerator();
		}
	}
	TestMemoryLeaks();
	return FTerminatingValue;
}