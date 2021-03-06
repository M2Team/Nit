/*******************************************************************************
Vczh Library++ 2.0
Windows界面::文本
开发者：陈梓瀚

接口：
类：
  VL_WinEdit				：文本框
  VL_WinStatic				：标签框
  VL_WinListBox				：列表框
  VL_WinComboBox			：下拉框
  VL_WinStatus				：状态栏
函数：
*******************************************************************************/

#ifndef VL_WINTEXT
#define VL_WINTEXT

#include "..\VL_WinClass.h"
#include "commctrl.h"

namespace vl
{
	namespace windows
	{
		enum VLE_EditCharDisplay
		{
			vecdNone,
			vecdLowerCase,
			vecdUpperCase,
			vecdDigitOnly,
			vecdPassword
		};
		class VL_WinEdit : public VL_WinControl
		{
		public:
			VL_WinEdit(VL_WinContainer* Parent , VBool Multiline , VBool AlwaysShowSelection=false);
			~VL_WinEdit();

			LRESULT					ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure);

			VE_NotifyEvent			OnChanged;
			VE_NotifyEvent			OnHScroll;
			VE_NotifyEvent			OnVScroll;
			VE_NotifyEvent			OnMaxText;
			VE_NotifyEvent			OnUpdate;

			VLE_WinHAlign			GetTextAlign();
			void					SetTextAlign(VLE_WinHAlign Align);

			VBool					GetMultiline();
			void					SetMultiline(VBool Value);

			VLE_EditCharDisplay		GetCharDisplay();
			void					SetCharDisplay(VLE_EditCharDisplay Display);

			VWChar					GetPasswordChar();
			void					SetPasswordChar(VWChar Char);

			VBool					GetHScroll();
			void					SetHScroll(VBool Value);

			VBool					GetVScroll();
			void					SetVScroll(VBool Value);

			VBool					GetReadonly();
			void					SetReadonly(VBool Value);

			VBool					GetWantReturn();
			void					SetWantReturn(VBool Value);

			VBool					GetBorder();
			void					SetBorder(VBool Value);

			VInt					GetTextLimit();
			void					SetTextLimit(VInt Value);

			VBool					GetModified();
			void					SetModified(VBool Value);

			VUnicodeString			GetSel();
			void					SetSel(VUnicodeString Sel);
			VInt					GetSelStart();
			void					SetSelStart(VInt Value);
			VInt					GetSelLength();
			void					SetSelLength(VInt Value);
			void					Select(VInt Index , VInt Count);
			void					SelectAll();
			void					ClearSel();
			void					Copy();
			void					Cut();
			void					Paste();

			VBool					GetCanUndo();
			VInt					GetCharFromPos(VInt X , VInt Y);
			void					GetPosFromChar(VInt Index , VInt& X , VInt& Y);
			VInt					GetFirstVisible();
			VUnicodeString			GetLine(VInt LineIndex);
			VInt					GetLineCount();
			VInt					GetCurrentLine();
			VInt					GetLineIndex(VInt CharIndex);
			VInt					GetCharIndex(VInt LineIndex);

			void					ScrollToCaret();
			void					ClearUndoBuffer();
			void					Undo();
		};

		enum VLE_StatusEdge
		{
			vseNone,
			vseSunken,
			vseEtched
		};
		class VL_WinStatic : public VL_WinContainer
		{
		public:
			VL_WinStatic(VL_WinContainer* Parent);
			~VL_WinStatic();

			LRESULT					ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure);

			VE_NotifyEvent			OnClick;
			VE_NotifyEvent			OnDbClick;

			VLE_WinHAlign			GetTextAlign();
			void					SetTextAlign(VLE_WinHAlign Align);

			VLE_StatusEdge			GetBorderStyle();
			void					SetBorderStyle(VLE_StatusEdge Value);
		};

		enum VLE_ListBoxStyle
		{
			vlbsNormal,
			vlbsMultiSelection,
			vlbsCheckSelection,
			vlbsMultiAndCheck
		};
		class VL_WinListBox : public VL_WinControl
		{
		protected:

			VBool					IsMultiSelection();

		public:
			VL_WinListBox(VL_WinContainer* Parent , VLE_ListBoxStyle Style=vlbsNormal);
			~VL_WinListBox();

			LRESULT					ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure);

			VE_NotifyEvent			OnDbClick;
			VE_NotifyEvent			OnSelChanged;

			VInt					GetFocusedIndex();
			void					SetFocusedIndex(VInt Value);

			VInt					GetSelectedIndex();
			void					SetSelectedIndex(VInt Value);

			VInt					GetItemHeight(VInt Index);
			void					SetItemHeight(VInt Index , VInt Value);

			VInt					GetFirstVisibleIndex();
			void					SetFirstVisibleIndex(VInt Value);

			VBool					GetSelected(VInt Index);
			void					SetSelected(VInt Index , VBool Value);

			void					SelectAll(VBool Selected);
			void					GetSelectedIndices(VL_List<VInt , true>& Indices);
			void					AddString(VUnicodeString String);
			void					InsertString(VInt Index , VUnicodeString String);
			void					DeleteString(VInt Index);
			VInt					GetCount();
			VUnicodeString			GetString(VInt Index);
			void					SetString(VInt Index , VUnicodeString String);
			void					Clear();

			VInt					FindStringPrefix(VUnicodeString String);
			VInt					FindString(VUnicodeString String);
			void					SelectPrefix(VUnicodeString String);
		};

		class VL_WinComboBox : public VL_WinControl
		{
		public:
			VL_WinComboBox(VL_WinContainer* Parent , VBool ReadOnly);
			~VL_WinComboBox();
			LRESULT					ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure);

			VE_NotifyEvent			OnCloseUp;
			VE_NotifyEvent			OnDbClick;
			VE_NotifyEvent			OnDropDown;
			VE_NotifyEvent			OnChanged;
			VE_NotifyEvent			OnUpdate;
			VE_NotifyEvent			OnSelChanged;
			VE_NotifyEvent			OnRejectSel;
			VE_NotifyEvent			OnAcceptSel;

			VBool					GetDisabledScroll();
			void					SetDiabledScroll(VBool Value);

			VInt					GetSelStart();
			void					SetSelStart(VInt Value);

			VInt					GetSelLength();
			void					SetSelLength(VInt Value);

			void					Select(VInt Index , VInt Length);

			VInt					GetMinVisible();
			void					SetMinVisible(VInt Value);

			void					SelectAll();

			void					ClearSel();

			VInt					GetSelectedIndex();
			void					SetSelectedIndex(VInt Index);

			VInt					GetItemHeight();
			void					SetItemHeight(VInt Value);

			VInt					GetFirstVisibleIndex();
			void					SetFirstVisibleIndex(VInt Index);

			VBool					IsOpened();
			void					AddString(VUnicodeString String);
			void					InsertString(VInt Index , VUnicodeString String);
			void					DeleteString(VInt Index);
			VInt					GetCount();
			VUnicodeString			GetString(VInt Index);
			void					SetString(VInt Index , VUnicodeString String);
			void					Clear();
			VInt					FindStringPrefix(VUnicodeString String);
			VInt					FindString(VUnicodeString String);
			void					SelectPrefix(VUnicodeString String);
			void					OpenList();
			void					CloseList();
		};

		class VL_WinStatusItem : public VL_Base
		{
			friend class VL_WinStatus;
		protected:
			VL_WinStatus*			FStatus;
			VInt					FIndex;

			VL_WinStatusItem(VL_WinStatus* Status , VInt Index);
		public:
			VL_WinStatusItem();
			VL_WinStatusItem(const VL_WinStatusItem& Item);
			~VL_WinStatusItem();
			VL_WinStatusItem& operator=(const VL_WinStatusItem& Item);
			operator VBool();

			VUnicodeString			GetTextLeft();
			void					SetTextLeft(VUnicodeString Value);

			VUnicodeString			GetTextCenter();
			void					SetTextCenter(VUnicodeString Value);

			VUnicodeString			GetTextRight();
			void					SetTextRight(VUnicodeString Value);

			VInt					GetWidth();
			void					SetWidth(VInt Value);

			VBool					GetBorder();
			void					SetBorder(VBool Value);

			VBool					GetRaise();
			void					SetRaise(VBool Value);
		};
		
		VL_DEFINE_VOID_EVENT	(VE_StatusItemEvent	,(Sender , Item)					,(VL_Base* Sender , VInt Item));

		class VL_WinStatus : public VL_WinControl
		{
			friend class VL_WinStatusItem;
		protected:

			class ItemRecord
			{
			public:
				typedef VL_List<ItemRecord , false>		List;

				VUnicodeString		TextLeft;
				VUnicodeString		TextCenter;
				VUnicodeString		TextRight;
				VInt				Width;
				VBool				Border;
				VBool				Raise;

				ItemRecord();
			};
		protected:
			ItemRecord::List		FItems;

			void					RefreshItem(VInt Index);
			void					RefreshItems();
		public:
			VL_WinStatus(VL_WinContainer* Parent);
			~VL_WinStatus();
			LRESULT					ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure);

			VE_StatusItemEvent		OnClick;
			VE_StatusItemEvent		OnDbClick;
			VE_StatusItemEvent		OnRightClick;
			VE_StatusItemEvent		OnRightDbClick;

			VL_WinStatusItem		AddItem();
			VL_WinStatusItem		InsertItem(VInt Index);
			VL_WinStatusItem		GetItem(VInt Index);
			VInt					GetItemCount();
			void					DeleteItem(VInt Index);
		};
	}
}

#endif