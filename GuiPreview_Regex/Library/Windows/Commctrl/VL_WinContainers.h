/*******************************************************************************
Vczh Library++ 2.0
Windows界面::容器
开发者：陈梓瀚

接口：
类：
  VL_WinGroup				：聚类框
  VL_WinTab					：分页框
  VL_WinTabPage				：分页框页面
函数：
*******************************************************************************/

#ifndef VL_WINCONTAINERS
#define VL_WINCONTAINERS

#include "..\VL_WinClass.h"
#include "VL_WinText.h"

namespace vl
{
	namespace windows
	{

/*********************************************************************************************************
Group Box
*********************************************************************************************************/

		class VL_WinGroup : public VL_WinContainer
		{
		protected:
		public:
			VL_WinGroup(VL_WinContainer* Parent);
			~VL_WinGroup();
		};

/*********************************************************************************************************
Tab Control
*********************************************************************************************************/

		class VL_WinTabPage : public VL_Base
		{
			friend class VL_WinTab;
		protected:
			HWND					FHandle;
			VInt					FIndex;
			VL_WinContainer*		FContainer;

			VL_WinTabPage(HWND Handle , VInt Index , VL_WinContainer* Container);
		public:
			VL_WinTabPage();
			VL_WinTabPage(const VL_WinTabPage& Page);
			VL_WinTabPage& operator=(const VL_WinTabPage& Page);
			operator VBool();
			operator VL_WinContainer*();

			VUnicodeString			GetText();
			void					SetText(VUnicodeString Value);

			VInt					GetImageIndex();
			void					SetImageIndex(VInt Value);

			VPointer				GetCustomData();
			void					SetCustomData(VPointer Value);

			VBool					GetHighlighted();
			void					SetHighlighted(VBool Value);

			VInt					GetIndex();

			VL_Placement*			GetPlacement();
			void					ApplyPlacement(placement::pBase::Ptr Placement);
			VInt					GetControlCount();
			VL_WinControl*			GetControl(VInt Index);
			VInt					GetClientWidth();
			VInt					GetClientHeight();
		};

		class VL_WinTab : public VL_WinContainer
		{
			typedef VL_List<VL_WinContainer* , true>		_TabContainers;
		protected:
			VL_WinImageList*		FImageList;
			_TabContainers			FTabContainers;

			DWORD					InternalGetExStyle();
			void					InternalSetExStyle(DWORD ExStyle);
			void					GetClientArea(RECT* Rect);
			void					ArrangeTabContainers();
			void					ResetTopTabContainer();
		public:
			VL_WinTab(VL_WinContainer* Parent);
			~VL_WinTab();
			LRESULT					ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure);

			VE_NotifyEvent			OnClick;
			VE_NotifyEvent			OnDbClick;
			VE_NotifyEvent			OnRightClick;
			VE_NotifyEvent			OnRightDbClick;
			VE_NotifyEvent			OnSelChanged;

			VL_WinImageList*		GetImageList();
			void					SetImageList(VL_WinImageList* ImageList);

			VBool					GetButtonTab();
			void					SetButtonTab(VBool Value);

			VBool					GetFlatButton();
			void					SetFlatButton(VBool Value);

			VBool					GetFlatSeparator();
			void					SetFlatSeparator(VBool Value);

			VBool					GetMultiline();
			void					SetMultiline(VBool Value);

			VBool					GetTabAtLeftTop();
			void					SetTabAtLeftTop(VBool Value);

			VBool					GetVerticalTab();
			void					SetVerticalTab(VBool Value);

			VL_WinTabPage			AddPage(VUnicodeString Text , VL_WinContainer* Container=0);
			VL_WinTabPage			InsertPage(VInt Index , VUnicodeString Text , VL_WinContainer* Container=0);
			void					DeletePage(VInt Index);
			VL_WinTabPage			GetPage(VInt Index);
			VInt					GetPageCount();
			VL_WinTabPage			GetSelectedPage();
			void					SetSelectedPage(VL_WinTabPage Page);
			VInt					GetRowCount();
		};
	}
}

#endif