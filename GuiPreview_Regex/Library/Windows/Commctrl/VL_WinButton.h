/*******************************************************************************
Vczh Library++ 2.0
Windows界面::按钮
开发者：陈梓瀚

接口：
类：
  VL_WinCustomButton				：按钮基类
  VL_WinCustomChecked				：通用可选按钮

  VL_WinButton						：命令按钮
  VL_WinCommandLink					：命令链接按钮
  VL_WinCheck						：复选按钮
  VL_WinRadio						：单选按钮
  VL_WinSplitButton					：分割按钮
函数：
*******************************************************************************/

#ifndef VL_WINBUTTON
#define VL_WINBUTTON

#include "..\VL_WinClass.h"
#include "commctrl.h"

namespace vl
{
	namespace windows
	{

/*********************************************************************************************************
结构控件
*********************************************************************************************************/

		class VL_WinCustomButton : public VL_WinControl
		{
		protected:
		public:
			VE_NotifyEvent			OnClick;

			VL_WinCustomButton(VL_WinContainer* Parent , DWORD Style);

			LRESULT					ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure);

			VLE_Align				GetTextVAlign();
			void					SetTextVAlign(VLE_Align Value);

			VLE_Align				GetTextHAlign();
			void					SetTextHAlign(VLE_Align Value);
		};

		class VL_WinCustomChecked : public VL_WinCustomButton
		{
		protected:
		public:
			VL_WinCustomChecked(VL_WinContainer* Parent , DWORD Style);

			VBool					GetChecked();
			virtual void			SetChecked(VBool Value);

			VBool					GetLeftText();
			void					SetLeftText(VBool Value);

			VBool					GetPushLike();
			void					SetPushLike(VBool Value);
		};

/*********************************************************************************************************
可用控件
*********************************************************************************************************/

		class VL_WinButton : public VL_WinCustomButton
		{
		protected:
		public:
			VL_WinButton(VL_WinContainer* Parent);
		};

		class VL_WinCommandLink : public VL_WinCustomButton
		{
		protected:
		public:
			VL_WinCommandLink(VL_WinContainer* Parent);

			VUnicodeString			GetNote();
			void					SetNote(VUnicodeString Note);
		};

		class VL_WinCheck : public VL_WinCustomChecked
		{
		protected:
		public:
			VL_WinCheck(VL_WinContainer* Parent);
		};

		class VL_WinRadio : public VL_WinCustomChecked
		{
		protected:
			VInt					FGroupNumber;
		public:
			VL_WinRadio(VL_WinContainer* Parent);

			LRESULT					ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure);

			void					SetChecked(VBool Value);

			VInt					GetGroupNumber();
			void					SetGroupNumber(VInt Value);
		};

		class VL_WinSplitButton : public VL_WinCustomButton
		{
		protected:
		public:
			VE_DropdownEvent		OnDropDown;

			VL_WinSplitButton(VL_WinContainer* Parent);

			LRESULT					ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure);
		};
	}
}

#endif