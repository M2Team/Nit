#include "VL_WinContainers.h"

namespace vl
{
	namespace windows
	{
		const VInt TABPAGE_TEXT_BUFFER_SIZE=1024;

/*********************************************************************************************************
VL_WinGroup
*********************************************************************************************************/

	VL_WinGroup::VL_WinGroup(VL_WinContainer* Parent)
	{
		_CreateWindow(
			WS_EX_CONTROLPARENT,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
			WC_BUTTON,
			Parent);
	}

	VL_WinGroup::~VL_WinGroup()
	{
	}

/*********************************************************************************************************
VL_WinTabPage
*********************************************************************************************************/

	void FillTabPageItem(TCITEM* Item , UINT Mask)
	{
		memset(Item,0,sizeof(*Item));
		Item->mask=Mask;
	}

	VL_WinTabPage::VL_WinTabPage(HWND Handle , VInt Index , VL_WinContainer* Container)
	{
		FHandle=Handle;
		FIndex=Index;
		FContainer=Container;
	}

	VL_WinTabPage::VL_WinTabPage()
	{
		FHandle=0;
		FIndex=0;
		FContainer=0;
	}

	VL_WinTabPage::VL_WinTabPage(const VL_WinTabPage& Page)
	{
		FHandle=Page.FHandle;
		FIndex=Page.FIndex;
		FContainer=Page.FContainer;
	}

	VL_WinTabPage& VL_WinTabPage::operator=(const VL_WinTabPage& Page)
	{
		FHandle=Page.FHandle;
		FIndex=Page.FIndex;
		FContainer=Page.FContainer;
		return *this;
	}

	VL_WinTabPage::operator VBool()
	{
		return FHandle!=0;
	}

	VL_WinTabPage::operator VL_WinContainer*()
	{
		return FContainer;
	}

	VUnicodeString VL_WinTabPage::GetText()
	{
		VWChar Buffer[TABPAGE_TEXT_BUFFER_SIZE]={0};
		TCITEM Item;
		FillTabPageItem(&Item,TCIF_TEXT);
		Item.pszText=Buffer;
		Item.cchTextMax=TABPAGE_TEXT_BUFFER_SIZE;
		TabCtrl_GetItem(FHandle,FIndex,&Item);
		return Buffer;
	}

	void VL_WinTabPage::SetText(VUnicodeString Value)
	{
		TCITEM Item;
		FillTabPageItem(&Item,TCIF_TEXT);
		Item.pszText=Value.Buffer();
		TabCtrl_SetItem(FHandle,FIndex,&Item);
	}

	VInt VL_WinTabPage::GetImageIndex()
	{
		TCITEM Item;
		FillTabPageItem(&Item,TCIF_IMAGE);
		TabCtrl_GetItem(FHandle,FIndex,&Item);
		return Item.iImage;
	}

	void VL_WinTabPage::SetImageIndex(VInt Value)
	{
		TCITEM Item;
		FillTabPageItem(&Item,TCIF_IMAGE);
		Item.iImage=Value;
		TabCtrl_SetItem(FHandle,FIndex,&Item);
	}

	VPointer VL_WinTabPage::GetCustomData()
	{
		TCITEM Item;
		FillTabPageItem(&Item,TCIF_PARAM);
		TabCtrl_GetItem(FHandle,FIndex,&Item);
		return (VPointer)Item.lParam;
	}

	void VL_WinTabPage::SetCustomData(VPointer Value)
	{
		TCITEM Item;
		FillTabPageItem(&Item,TCIF_PARAM);
		Item.lParam=(LPARAM)Value;
		TabCtrl_SetItem(FHandle,FIndex,&Item);
	}

	VBool VL_WinTabPage::GetHighlighted()
	{
		TCITEM Item;
		FillTabPageItem(&Item,TCIF_STATE);
		Item.dwStateMask=TCIS_HIGHLIGHTED;
		TabCtrl_GetItem(FHandle,FIndex,&Item);
		return Item.dwState!=0;
	}

	void VL_WinTabPage::SetHighlighted(VBool Value)
	{
		TabCtrl_HighlightItem(FHandle,FIndex,(Value?TRUE:FALSE));
	}

	VInt VL_WinTabPage::GetIndex()
	{
		return FIndex;
	}

	VL_Placement* VL_WinTabPage::GetPlacement()
	{
		return FContainer->GetPlacement();
	}

	void VL_WinTabPage::ApplyPlacement(placement::pBase::Ptr Placement)
	{
		FContainer->ApplyPlacement(Placement);
	}

	VInt VL_WinTabPage::GetControlCount()
	{
		return FContainer->GetControlCount();
	}

	VL_WinControl* VL_WinTabPage::GetControl(VInt Index)
	{
		return FContainer->GetControl(Index);
	}

	VInt VL_WinTabPage::GetClientWidth()
	{
		return FContainer->GetClientWidth();
	}

	VInt VL_WinTabPage::GetClientHeight()
	{
		return FContainer->GetClientHeight();
	}

/*********************************************************************************************************
VL_WinTab
*********************************************************************************************************/

	DWORD VL_WinTab::InternalGetExStyle()
	{
		return TabCtrl_GetExtendedStyle(FHandle);
	}

	void VL_WinTab::InternalSetExStyle(DWORD ExStyle)
	{
		TabCtrl_SetExtendedStyle(FHandle,ExStyle);
	}

	void VL_WinTab::GetClientArea(RECT* Rect)
	{
		Rect->left=0;
		Rect->top=0;
		Rect->right=GetWidth();
		Rect->bottom=GetHeight();
		TabCtrl_AdjustRect(FHandle,FALSE,Rect);
	}

	void VL_WinTab::ArrangeTabContainers()
	{
		RECT Rect;
		GetClientArea(&Rect);
		for(VInt i=0;i<FTabContainers.GetCount();i++)
		{
			FTabContainers[i]->Move(Rect.left,Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top);
		}
	}

	void VL_WinTab::ResetTopTabContainer()
	{
		VInt Index=TabCtrl_GetCurSel(FHandle);
		if(Index!=-1)
		{
			for(VInt i=0;i<FTabContainers.GetCount();i++)
			{
				FTabContainers[i]->SetVisible(false);
			}
			FTabContainers[Index]->SetVisible(true);
		}
	}

	VL_WinTab::VL_WinTab(VL_WinContainer* Parent)
	{
		FImageList=0;

		_CreateWindow(
			WS_EX_CONTROLPARENT,
			WS_TABSTOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_CHILD | TCS_FOCUSONBUTTONDOWN,
			WC_TABCONTROL,
			Parent);
		TabCtrl_SetUnicodeFormat(FHandle,TRUE);
	}

	VL_WinTab::~VL_WinTab()
	{
	}

	LRESULT VL_WinTab::ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure)
	{
		LRESULT Result=0;
		switch(Message)
		{
		case WM_NOTIFY_DISPATCHED:
			switch(((NMHDR*)lParam)->code)
			{
			case NM_CLICK:
				{
					OnClick(this);
				}
				break;
			case NM_DBLCLK:
				{
					OnDbClick(this);
				}
				break;
			case NM_RCLICK:
				{
					OnRightClick(this);
				}
				break;
			case NM_RDBLCLK:
				{
					OnRightDbClick(this);
				}
				break;
			case TCN_SELCHANGE:
				{
					ResetTopTabContainer();
					OnSelChanged(this);
				}
				break;
			}
			Result=VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
			break;
		case WM_SIZE:
			{
				ArrangeTabContainers();
				Result=VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
			}
			break;
		default:
			Result=VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
		}
		return Result;
	}

	VL_WinImageList* VL_WinTab::GetImageList()
	{
		return FImageList;
	}

	void VL_WinTab::SetImageList(VL_WinImageList* ImageList)
	{
		TabCtrl_SetImageList(FHandle,ImageList->GetHandle());
		FImageList=ImageList;
	}

	VBool VL_WinTab::GetButtonTab()
	{
		return GetStyle(TCS_BUTTONS);
	}

	void VL_WinTab::SetButtonTab(VBool Value)
	{
		SetStyle(TCS_BUTTONS,Value);
	}

	VBool VL_WinTab::GetFlatButton()
	{
		return GetStyle(TCS_FLATBUTTONS);
	}

	void VL_WinTab::SetFlatButton(VBool Value)
	{
		SetStyle(TCS_FLATBUTTONS,Value);
	}

	VBool VL_WinTab::GetFlatSeparator()
	{
		return GetExStyle(TCS_EX_FLATSEPARATORS);
	}

	void VL_WinTab::SetFlatSeparator(VBool Value)
	{
		return SetExStyle(TCS_EX_FLATSEPARATORS,Value);
	}

	VBool VL_WinTab::GetMultiline()
	{
		return GetStyle(TCS_MULTILINE);
	}

	void VL_WinTab::SetMultiline(VBool Value)
	{
		SetStyle(TCS_MULTILINE,Value);
		ArrangeTabContainers();
	}

	VBool VL_WinTab::GetTabAtLeftTop()
	{
		return !GetStyle(TCS_BOTTOM);
	}

	void VL_WinTab::SetTabAtLeftTop(VBool Value)
	{
		SetStyle(TCS_BOTTOM,!Value);
		ArrangeTabContainers();
	}

	VBool VL_WinTab::GetVerticalTab()
	{
		return GetStyle(TCS_VERTICAL);
	}

	void VL_WinTab::SetVerticalTab(VBool Value)
	{
		SetStyle(TCS_VERTICAL,Value);
		ArrangeTabContainers();
	}

	VL_WinTabPage VL_WinTab::AddPage(VUnicodeString Text , VL_WinContainer* Container)
	{
		return InsertPage(GetPageCount(),Text,Container);
	}

	VL_WinTabPage VL_WinTab::InsertPage(VInt Index , VUnicodeString Text , VL_WinContainer* Container)
	{
		TCITEM Item;
		memset(&Item,0,sizeof(Item));
		Item.mask=TCIF_TEXT;
		Item.pszText=Text.Buffer();
		Index=TabCtrl_InsertItem(FHandle,Index,&Item);
		if(Index==-1)
		{
			return VL_WinTabPage();
		}
		else
		{
			RECT Rect;
			GetClientArea(&Rect);
			if(Container)
			{
				Container->SetParent(this);
			}
			else
			{
				Container=new VL_WinStatic(this);
			}
			FTabContainers.Insert(Index,Container);
			ResetTopTabContainer();
			ArrangeTabContainers();
			return VL_WinTabPage(FHandle,Index,Container);
		}
	}

	void VL_WinTab::DeletePage(VInt Index)
	{
		if(Index>=0 && Index<GetPageCount())
		{
			if(TabCtrl_DeleteItem(FHandle,Index)==TRUE)
			{
				delete FTabContainers.Fetch(Index);
				ArrangeTabContainers();
			}
		}
	}

	VL_WinTabPage VL_WinTab::GetPage(VInt Index)
	{
		if(Index>=0 && Index<GetPageCount())
		{
			return VL_WinTabPage(FHandle,Index,FTabContainers[Index]);
		}
		else
		{
			return VL_WinTabPage();
		}
	}

	VInt VL_WinTab::GetPageCount()
	{
		return TabCtrl_GetItemCount(FHandle);
	}

	VL_WinTabPage VL_WinTab::GetSelectedPage()
	{
		VInt Index=TabCtrl_GetCurSel(FHandle);
		if(Index==-1)
		{
			return VL_WinTabPage();
		}
		else
		{
			return VL_WinTabPage(FHandle,Index,FTabContainers[Index]);
		}
	}

	void VL_WinTab::SetSelectedPage(VL_WinTabPage Page)
	{
		if(Page)
		{
			TabCtrl_SetCurSel(FHandle,Page.FIndex);
			ResetTopTabContainer();
			OnSelChanged(this);
		}
	}

	VInt VL_WinTab::GetRowCount()
	{
		return TabCtrl_GetRowCount(FHandle);
	}

	}
}