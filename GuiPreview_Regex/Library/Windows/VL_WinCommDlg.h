/*******************************************************************************
Vczh Library++ 2.0
Windows界面::Common Dialog Library
开发者：陈梓瀚

接口：
类：
  VL_WinColorDialog						：颜色对话框
  VL_WinOpenFileDialog					：打开文件对话框
  VL_WinSaveFileDialog					：保存文件对话框
  VL_WinFontDialog						：字体对话框
函数：
*******************************************************************************/

#ifndef VL_WINCOMMDLG
#define VL_WINCOMMDLG

#include "..\Data\Data\VL_Data_Map.h"
#include "VL_WinGDI.h"

namespace vl
{
	namespace windows
	{

/*********************************************************************************************************
事件
*********************************************************************************************************/

/*********************************************************************************************************
对话框
*********************************************************************************************************/

		class VL_WinColorDialog : public VL_Base
		{
			friend class VL_WinCommonDialogService;
		protected:
			HWND						FHandle;
			VBool						FOpening;
		public:
			COLORREF					Color;
			COLORREF					CustomColors[16];

			VE_NotifyEvent				OnInit;
			VE_QueryEvent				OnOK;

			VL_WinColorDialog();

			VBool						IsOpening();
			VBool						Execute(VL_WinForm* Form);
		};

		class VL_WinFileDialog : public VL_Base
		{
			friend class VL_WinCommonDialogService;
			friend	UINT_PTR CALLBACK Service_PFNHookProc(HWND hdlg , UINT uiMsg , WPARAM wParam , LPARAM lParam);
		protected:
			HWND						FHandle;
			VBool						FOpening;

			VBool						FMultiSelection;
			VBool						FAddToRecent;
			VBool						FEnableSizing;
			VBool						FShowHidden;
			VBool						FDereferenceLink;
			VBool						FNetworkButton;
			VUnicodeString				FTitle;
			VUnicodeString				FInitDir;
			VUnicodeString				FFilter;
			VUnicodeString				FDefaultExt;

			PWChar						FFilterBuffer;
			PWChar						FFileBuffer;
			PWChar						FTitleBuffer;
			PWChar						FInitDirBuffer;
			PWChar						FDefExtBuffer;

			void						Initialize();
			void						Finalize();
			void						FillStruct(OPENFILENAME& Struct , DWORD ExtraFlag);
			virtual void				FillResult(OPENFILENAME& Struct);
		public:
			VE_NotifyEvent				OnInit;
			VE_QueryEvent				OnOK;

			VL_WinFileDialog();
			~VL_WinFileDialog();

			VL_List<VUnicodeString , false>	FileNames;

			VBool						GetMultiSelection();
			void						SetMultiSelection(VBool Value);

			VBool						GetAddToRecent();
			void						SetAddToRecent(VBool Value);

			VBool						GetEnableSizing();
			void						SetEnableSizing(VBool Value);

			VBool						GetShowHidden();
			void						SetShowHidden(VBool Value);

			VBool						GetDereferenceLink();
			void						SetDereferenceLink(VBool Value);

			VBool						GetNetworkButton();
			void						SetNetworkButton(VBool Value);

			VUnicodeString				GetTitle();
			void						SetTitle(VUnicodeString Value);

			VUnicodeString				GetInitDir();
			void						SetInitDir(VUnicodeString Value);

			VUnicodeString				GetFilter();
			void						SetFilter(VUnicodeString Value);

			VUnicodeString				GetDefaultExt();
			void						SetDefaultExt(VUnicodeString Value);
		};

		class VL_WinOpenFileDialog : public VL_WinFileDialog
		{
		protected:
			VBool						FHideReadonly;
			VBool						FReadonly;
			VBool						FFileMustExist;

			void						FillResult(OPENFILENAME& Struct);
		public:
			VL_WinOpenFileDialog();
			~VL_WinOpenFileDialog();

			VBool						GetHideReadonly();
			void						SetHideReadonly(VBool Value);

			VBool						GetReadonly();
			void						SetReadonly(VBool Value);

			VBool						GetFileMustExist();
			void						SetFileMustExist(VBool Value);

			VBool						IsOpening();
			VBool						Execute(VL_WinForm* Form);
		};

		class VL_WinSaveFileDialog : public VL_WinFileDialog
		{
		protected:
			VBool						FOverridePrompt;
		public:
			VL_WinSaveFileDialog();
			~VL_WinSaveFileDialog();

			VBool						GetOverridePrompt();
			void						SetOverridePrompt(VBool Value);

			VBool						IsOpening();
			VBool						Execute(VL_WinForm* Form);
		};

		class VL_WinFontDialog : public VL_Base
		{
			friend class VL_WinCommonDialogService;
			friend UINT_PTR CALLBACK Service_CFHookProc(HWND hdlg , UINT uiMsg , WPARAM wParam , LPARAM lParam);
		protected:
			LOGFONT						FFontInfo;
			HWND						FHandle;
			VBool						FOpening;

			VBool						FTrueTypeOnly;
			VBool						FExtended;
			VBool						FFontMustExist;
		public:
			VL_WinFontDialog();
			~VL_WinFontDialog();

			VL_WinFont::Ptr				Font;
			COLORREF					Color;

			VBool						GetTrueTypeOnly();
			void						SetTrueTypeOnly(VBool Value);

			VBool						GetExtended();
			void						SetExtended(VBool Value);

			VBool						GetFontMustExist();
			void						SetFontMustExist(VBool Value);

			VBool						IsOpening();
			VBool						Execute(VL_WinForm* Form);
		};

/*********************************************************************************************************
通用对话框服务
*********************************************************************************************************/

		class VL_WinCommonDialogService : public VL_Base
		{
			typedef VL_ListedMap<HWND , VL_WinColorDialog*>		ColorDialogMap;
			typedef VL_ListedMap<HWND , VL_WinFileDialog*>		FileDialogMap;
			typedef VL_ListedMap<HWND , VL_WinFontDialog*>		FontDialogMap;

			friend 	UINT_PTR CALLBACK Service_CCHookProc(HWND hdlg , UINT uiMsg , WPARAM wParam , LPARAM lParam);
			friend	UINT_PTR CALLBACK Service_PFNHookProc(HWND hdlg , UINT uiMsg , WPARAM wParam , LPARAM lParam);
			friend UINT_PTR CALLBACK Service_CFHookProc(HWND hdlg , UINT uiMsg , WPARAM wParam , LPARAM lParam);
		protected:
			UINT						FMessage_ColorOnOK;

			ColorDialogMap				FColorDialogs;
			FileDialogMap				FFileDialogs;
			FontDialogMap				FFontDialogs;
		public:
			VL_WinCommonDialogService();

			void						RegisterColorDialog(HWND Handle , VL_WinColorDialog* Dialog);
			void						UnregisterColorDialog(HWND Handle);
			void						RegisterFileDialog(HWND Handle , VL_WinFileDialog* Dialog);
			void						UnregisterFileDialog(HWND Handle);
			void						RegisterFontDialog(HWND Handle , VL_WinFontDialog* Dialog);
			void						UnregisterFontDialog(HWND Handle);
		};

		extern VL_WinCommonDialogService* GetCommdlgService();

	}
}

#endif