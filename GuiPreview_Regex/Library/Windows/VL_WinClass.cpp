#include "VL_WinMain.h"
#include <commctrl.h>

namespace vl
{
	namespace windows
	{

/*********************************************************************************************************
事件
*********************************************************************************************************/

	VLS_MouseStruct::VLS_MouseStruct(WPARAM wParam , LPARAM lParam , VBool WheelMessage)
	{
		if(WheelMessage)
		{
			Wheel=GET_WHEEL_DELTA_WPARAM(wParam);
			wParam=GET_KEYSTATE_WPARAM(wParam);
		}
		else
		{
			Wheel=0;
		}
		Ctrl=(wParam & MK_CONTROL)!=0;
		Shift=(wParam & MK_SHIFT)!=0;
		LeftButton=(wParam & MK_LBUTTON)!=0;
		MiddleButton=(wParam & MK_MBUTTON)!=0;
		RightButton=(wParam & MK_RBUTTON)!=0;
		POINTS Point=MAKEPOINTS(lParam);
		X=Point.x;
		Y=Point.y;
	}

	VLS_KeyStruct::VLS_KeyStruct(WPARAM wParam , LPARAM lParam)
	{
		KeyCode=wParam;
		RepeatCount=lParam%65536;
		Alt=(lParam>>29)%2==1;
		PreviousDown=(lParam>>30)%2==1;
	}

	VL_IMPLEMENT_VOID_EVENT	(VE_NotifyEvent		,(Sender)									,(VL_Base* Sender));
	VL_IMPLEMENT_VOID_EVENT	(VE_MovingEvent		,(Sender,Area)								,(VL_Base* Sender,LPRECT Area));
	VL_IMPLEMENT_VOID_EVENT	(VE_SizingEvent		,(Sender,Left,Right,Top,Bottom,Area)		,(VL_Base* Sender,VBool Left,VBool Right,VBool Top,VBool Bottom,LPRECT Area));
	VL_IMPLEMENT_VOID_EVENT	(VE_QueryEvent		,(Sender,Cancel)							,(VL_Base* Sender,VBool& Cancel));
	VL_IMPLEMENT_VOID_EVENT	(VE_MouseEvent		,(Sender,MouseStruct)						,(VL_Base* Sender,VLS_MouseStruct MouseStruct));
	VL_IMPLEMENT_VOID_EVENT	(VE_KeyEvent		,(Sender,KeyStruct)							,(VL_Base* Sender,VLS_KeyStruct KeyStruct));
	VL_IMPLEMENT_VOID_EVENT	(VE_CharEvent		,(Sender,Char)								,(VL_Base* Sender,VWChar& Char));
	VL_IMPLEMENT_VOID_EVENT	(VE_HotKeyEvent		,(Sender,ID)								,(VL_Base* Sender,VInt ID));
	VL_IMPLEMENT_VOID_EVENT	(VE_DropdownEvent	,(Sender,Rect)								,(VL_Base* Sender,RECT Rect));

/*********************************************************************************************************
加速键
*********************************************************************************************************/

	struct VLS_KeyNamePair
	{
		VInt				Code;
		VUnicodeString		Name;
	};
	VLS_KeyNamePair*		AcceleratorNames=0;
	VInt					AcceleratorCount=0;
	VL_AcceleratorManager*	AcceleratorManager=0;

	void AddAcceleratorName(VInt Code , VUnicodeString Name)
	{
		VLS_KeyNamePair& Pair=AcceleratorNames[AcceleratorCount++];
		Pair.Code=Code;
		Pair.Name=Name;
	}

	void InitAccelerator()
	{
		AcceleratorNames=new VLS_KeyNamePair[100];

		AddAcceleratorName(192	,L"`");
		AddAcceleratorName(27	,L"Esc");
		AddAcceleratorName(189	,L"-");
		AddAcceleratorName(187	,L"=");
		AddAcceleratorName(220	,L"\\");
		AddAcceleratorName(8	,L"Backspace");
		AddAcceleratorName(219	,L"[");
		AddAcceleratorName(221	,L"]");
		AddAcceleratorName(186	,L";");
		AddAcceleratorName(222	,L"\'");
		AddAcceleratorName(188	,L",");
		AddAcceleratorName(190	,L".");
		AddAcceleratorName(191	,L"/");
		AddAcceleratorName(45	,L"Ins");
		AddAcceleratorName(46	,L"Del");
		AddAcceleratorName(36	,L"Home");
		AddAcceleratorName(35	,L"End");
		AddAcceleratorName(33	,L"PageUp");
		AddAcceleratorName(34	,L"PageDown");
		for(VInt i=0;i<26;i++)
		{
			AddAcceleratorName(65+i,VUnicodeString((VWChar)(L'A'+i)));
		}
		for(VInt i=0;i<10;i++)
		{
			AddAcceleratorName(48+i,VUnicodeString(i));
		}
		for(VInt i=0;i<12;i++)
		{
			AddAcceleratorName(112+i,L"F"+VUnicodeString(i+1));
		}
		for(VInt i=0;i<10;i++)
		{
			AddAcceleratorName(96+i,L"Num"+VUnicodeString(i));
		}

		AcceleratorManager=new VL_AcceleratorManager;
	}

	void DestroyAccelerator()
	{
		delete AcceleratorManager;
		delete[] AcceleratorNames;
		AcceleratorManager=0;
		AcceleratorNames=0;
	}

	VL_AcceleratorManager* GetAcceleratorManager()
	{
		return AcceleratorManager;
	}

	VL_AcceleratorItem* Accel(VLE_AcceleratorKey SysKeys , VUnicodeString Key)
	{
		return GetAcceleratorManager()->GetAcceleratorItem(SysKeys,Key);
	}

	VInt KeyCodeFromName(VUnicodeString Name)
	{
		for(VInt i=0;i<AcceleratorCount;i++)
		{
			if(AcceleratorNames[i].Name==Name)
			{
				return AcceleratorNames[i].Code;
			}
		}
		return 0;
	}

/*********************************************************************************************************
IVL_AcceleratorReceiver
*********************************************************************************************************/

	VL_AcceleratorItem* IVL_AcceleratorReceiver::GetAcceleratorItem()
	{
		return FItem;
	}

	IVL_AcceleratorReceiver::IVL_AcceleratorReceiver()
	{
		FItem=0;
	}

	IVL_AcceleratorReceiver::~IVL_AcceleratorReceiver()
	{
		if(FItem)
		{
			FItem->Unbind();
			FItem=0;
		}
	}

	VBool IVL_AcceleratorReceiver::Bind(VL_AcceleratorItem* Item)
	{
		if(Item)
		{
			if(Item->Bind(this))
			{
				Unbind();
				FItem=Item;
				OnAttachAcceleratorItem();
				return false;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	void IVL_AcceleratorReceiver::Unbind()
	{
		if(FItem)
		{
			FItem->Unbind();
			FItem=0;
			OnDetachAcceleratorItem();
		}
	}

/*********************************************************************************************************
VL_AcceleratorItem
*********************************************************************************************************/

	VL_AcceleratorItem::VL_AcceleratorItem(VUnicodeString Name)
	{
		FName=Name;
		FReceiver=0;
	}

	VL_AcceleratorItem::~VL_AcceleratorItem()
	{
		if(FReceiver)
		{
			FReceiver->Unbind();
		}
	}

	VBool VL_AcceleratorItem::Bind(IVL_AcceleratorReceiver* Receiver)
	{
		if(FReceiver)
		{
			return false;
		}
		else
		{
			FReceiver=Receiver;
			return true;
		}
	}

	void VL_AcceleratorItem::Unbind()
	{
		FReceiver=0;
	}

	void VL_AcceleratorItem::Execute()
	{
		if(FReceiver)
		{
			FReceiver->OnExecuteAcceleratorItem();
		}
	}

	VUnicodeString VL_AcceleratorItem::GetName()
	{
		return FName;
	}

/*********************************************************************************************************
VL_AcceleratorManager
*********************************************************************************************************/

	VL_AcceleratorManager::VL_AcceleratorManager()
	{
		FHandle=CreateAcceleratorTable(FAcceleratorList.Buffer(),FAcceleratorList.GetCount());
	}

	VL_AcceleratorManager::~VL_AcceleratorManager()
	{
		DestroyAcceleratorTable(FHandle);
	}

	HACCEL VL_AcceleratorManager::GetHandle()
	{
		return FHandle;
	}

	VL_AcceleratorItem* VL_AcceleratorManager::GetAcceleratorItem(VLE_AcceleratorKey SysKeys , VUnicodeString Key)
	{
		ACCEL Info;
		Info.fVirt=FVIRTKEY;
		Info.key=0;
		Info.cmd=0;
		if(SysKeys & vakAlt)	Info.fVirt|=FALT;
		if(SysKeys & vakShift)	Info.fVirt|=FSHIFT;
		if(SysKeys & vakCtrl)	Info.fVirt|=FCONTROL;
		Info.key=KeyCodeFromName(Key);
		if(Info.key==0)
		{
			return false;
		}

		for(VInt i=0;i<FAcceleratorList.GetCount();i++)
		{
			ACCEL& Old=FAcceleratorList[i];
			if((Old.fVirt==Info.fVirt) && (Old.key==Info.key))
			{
				return FAcceleratorItems[i].Object();
			}
		}

		Info.cmd=FAcceleratorList.GetCount();
		if(SysKeys & vakAlt)	Key=L"Alt+"+Key;
		if(SysKeys & vakShift)	Key=L"Shift+"+Key;
		if(SysKeys & vakCtrl)	Key=L"Ctrl+"+Key;
		VL_AcceleratorItem* Item=new VL_AcceleratorItem(Key);
		FAcceleratorItems.Add(Item);

		DestroyAcceleratorTable(FHandle);
		FAcceleratorList.Add(Info);
		FHandle=CreateAcceleratorTable(FAcceleratorList.Buffer(),FAcceleratorList.GetCount());
		return Item;
	}

	VL_AcceleratorItem* VL_AcceleratorManager::GetAcceleratorItem(VInt ID)
	{
		if(ID>=0 && ID<FAcceleratorItems.GetCount())
		{
			return FAcceleratorItems[ID].Object();
		}
		else
		{
			return 0;
		}
	}

/*********************************************************************************************************
VL_Placement
*********************************************************************************************************/

	void VL_Placement::Initialize()
	{
		FContainer=0;
		FParent=0;
		FControl=0;
		FPlacement1=0;
		FPlacement2=0;
		FSpliterDirection=vpdHorizontal;
		FSpliterPosition=0;
		FSpliterSize=0;
		FSpliterScale=0;
		FBehavior=vpbFixScale;
		FBorderSize=0;
		FMinClientWidth=0;
		FMinClientHeight=0;
		FDisableResize=false;
	}
	
	VL_Placement::VL_Placement(VL_Placement* Parent)
	{
		Initialize();
		FParent=Parent;
	}

	VL_Placement::VL_Placement(VL_WinContainer* Container)
	{
		Initialize();
		FContainer=Container;
		FContainer->OnSizing.Bind(this,&VL_Placement::OnSizing);
		FContainer->OnSized.Bind(this,&VL_Placement::OnSized);
	}

	VL_Placement::~VL_Placement()
	{
		if(FPlacement1)delete FPlacement1;
		if(FPlacement2)delete FPlacement2;
		if(FContainer)
		{
			FContainer->OnSizing.Unbind(this,&VL_Placement::OnSizing);
			FContainer->OnSized.Unbind(this,&VL_Placement::OnSized);
		}
	}

	void VL_Placement::OnSizing(VL_Base* Sender , VBool Left , VBool Right , VBool Top , VBool Bottom , LPRECT Area)
	{
		if(FDisableResize)return;
		RECT Min=GetMinSize();
		VInt MinWidth=Min.right+(FContainer->GetWidth()-FContainer->GetClientWidth());
		VInt MinHeight=Min.bottom+(FContainer->GetHeight()-FContainer->GetClientHeight());
		VInt Width=Area->right-Area->left;
		VInt Height=Area->bottom-Area->top;
		if(Width<MinWidth)
		{
			if(Left)
			{
				Area->left=Area->right-MinWidth;
			}
			else
			{
				Area->right=Area->left+MinWidth;
			}
		}
		if(Height<MinHeight)
		{
			if(Top)
			{
				Area->top=Area->bottom-MinHeight;
			}
			else
			{
				Area->bottom=Area->top+MinHeight;
			}
		}
	}

	void VL_Placement::OnSized(VL_Base* Sender)
	{
		if(FDisableResize)return;
		Resize();
	}

	void VL_Placement::Resize()
	{
		if(FContainer)
		{
			FDisableResize=true;
			RECT MinSize=GetMinSize();
			if(FContainer->GetClientWidth()<MinSize.right)
			{
				FContainer->SetClientWidth(MinSize.right);
			}
			if(FContainer->GetClientHeight()<MinSize.bottom)
			{
				FContainer->SetClientHeight(MinSize.bottom);
			}
			FDisableResize=false;
		}
		RECT Area=GetArea();
		if(FControl)
		{
			FControl->SetLeft(Area.left);
			FControl->SetTop(Area.top);
			FControl->SetWidth(Area.right-Area.left);
			FControl->SetHeight(Area.bottom-Area.top);
		}
		if(FBehavior==vpbFixScale)
		{
			if(FSpliterDirection==vpdHorizontal)
			{
				FSpliterPosition=(VInt)((Area.bottom-Area.top)*FSpliterScale);
			}
			else
			{
				FSpliterPosition=(VInt)((Area.right-Area.left)*FSpliterScale);
			}
		}
		if(FPlacement1)FPlacement1->Resize();
		if(FPlacement2)FPlacement2->Resize();
	}

	void VL_Placement::UnregisterControl(VL_WinControl* Control)
	{
		if(FControl==Control)FControl=0;
		if(FPlacement1)FPlacement1->UnregisterControl(Control);
		if(FPlacement2)FPlacement2->UnregisterControl(Control);
	}

	VInt VL_Placement::GetSpliterPosition()
	{
		return FSpliterPosition;
	}

	void VL_Placement::SetSpliterPosition(VInt Position)
	{
		FSpliterPosition=Position;
		Resize();
	}

	VLE_PlacementDirection VL_Placement::GetSpliterDirection()
	{
		return FSpliterDirection;
	}

	void VL_Placement::SetSpliterDirection(VLE_PlacementDirection Direction)
	{
		FSpliterDirection=Direction;
		Resize();
	}

	VLE_PlacementBehavior VL_Placement::GetBehavior()
	{
		return FBehavior;
	}

	void VL_Placement::SetBehavior(VLE_PlacementBehavior Behavior)
	{
		FBehavior=Behavior;
	}

	VInt VL_Placement::GetBorderSize()
	{
		return FBorderSize;
	}

	void VL_Placement::SetBorderSize(VInt Value)
	{
		FBorderSize=Value;
		Resize();
	}

	VInt VL_Placement::GetSpliterSize()
	{
		return FSpliterSize;
	}

	void VL_Placement::SetSpliterSize(VInt Size)
	{
		FSpliterSize=Size;
		Resize();
	}

	VDouble VL_Placement::GetSpliterScale()
	{
		return FSpliterScale;
	}

	void VL_Placement::SetSpliterScale(VDouble Scale)
	{
		FSpliterScale=Scale;
		Resize();
	}

	VL_WinControl* VL_Placement::GetControl()
	{
		return FControl;
	}

	void VL_Placement::SetControl(VL_WinControl* Control)
	{
		FControl=Control;
		Resize();
	}

	VInt VL_Placement::GetMinClientWidth()
	{
		return FMinClientWidth;
	}

	void VL_Placement::SetMinClientWidth(VInt Value)
	{
		FMinClientWidth=Value;
		Resize();
	}

	VInt VL_Placement::GetMinClientHeight()
	{
		return FMinClientHeight;
	}

	void VL_Placement::SetMinClientHeight(VInt Value)
	{
		FMinClientHeight=Value;
		Resize();
	}

	VL_Placement* VL_Placement::GetPlacement1()
	{
		if(!FPlacement1)
		{
			FPlacement1=new VL_Placement(this);
		}
		return FPlacement1;
	}

	VL_Placement* VL_Placement::GetPlacement2()
	{
		if(!FPlacement2)
		{
			FPlacement2=new VL_Placement(this);
		}
		return FPlacement2;
	}

	RECT VL_Placement::GetArea()
	{
		RECT Area;
		if(FParent)
		{
			Area=FParent->GetArea();
			if(FParent->FPlacement1==this)
			{
				if(FParent->FSpliterDirection==vpdHorizontal)
				{
					Area.bottom=Area.top+FParent->GetPlacement1Position();
				}
				else
				{
					Area.right=Area.left+FParent->GetPlacement1Position();
				}
			}
			else
			{
				if(FParent->FSpliterDirection==vpdHorizontal)
				{
					Area.top=Area.bottom-FParent->GetPlacement2Position();
				}
				else
				{
					Area.left=Area.right-FParent->GetPlacement2Position();
				}
			}
		}
		else
		{
			Area.left=0;
			Area.top=0;
			Area.right=FContainer->GetClientWidth();
			Area.bottom=FContainer->GetClientHeight();
		}
		Area.left+=FBorderSize;
		Area.top+=FBorderSize;
		Area.right-=FBorderSize;
		Area.bottom-=FBorderSize;
		if(Area.right<Area.left)Area.right=Area.left;
		if(Area.bottom<Area.top)Area.bottom=Area.top;
		return Area;
	}

	RECT VL_Placement::GetMinSize()
	{
		RECT Min1,Min2,Min;
		Min1.left=0;
		Min1.top=0;
		Min1.right=0;
		Min1.bottom=0;
		Min2.left=0;
		Min2.top=0;
		Min2.right=0;
		Min2.bottom=0;
		Min.left=0;
		Min.top=0;
		Min.right=0;
		Min.bottom=0;

		if(FPlacement1)Min1=FPlacement1->GetMinSize();
		if(FPlacement2)Min2=FPlacement2->GetMinSize();
		VInt MaxWidth=Min1.right>Min2.right?Min1.right:Min2.right;
		VInt MaxHeight=Min1.bottom>Min2.bottom?Min1.bottom:Min2.bottom;
		if(FSpliterDirection==vpdHorizontal)
		{
			Min.right=MaxWidth+FBorderSize*2;
			Min.bottom=Min1.bottom+Min2.bottom+FBorderSize*2+FSpliterSize;
		}
		else
		{
			Min.right=Min1.right+Min2.right+FBorderSize*2+FSpliterSize;
			Min.bottom=MaxHeight+FBorderSize*2;
		}

		VInt MinWidth=FMinClientWidth+2*FBorderSize;
		VInt MinHeight=FMinClientHeight+2*FBorderSize;
		if(Min.right<MinWidth)Min.right=MinWidth;
		if(Min.bottom<MinHeight)Min.bottom=MinHeight;
		return Min;
	}

	VInt VL_Placement::GetPlacement1Position()
	{
		if(FBehavior==vpbFixPlacement2)
		{
			RECT Area=GetArea();
			if(FSpliterDirection==vpdHorizontal)
			{
				return Area.bottom-Area.top-FSpliterSize-FSpliterPosition;
			}
			else
			{
				return Area.right-Area.left-FSpliterSize-FSpliterPosition;
			}
		}
		else
		{
			return FSpliterPosition;
		}
	}

	void VL_Placement::SetPlacement1Position(VInt Value)
	{
		if(FBehavior==vpbFixPlacement2)
		{
			RECT Area=GetArea();
			if(FSpliterDirection==vpdHorizontal)
			{
				SetSpliterPosition(Area.bottom-Area.top-FSpliterSize-Value);
			}
			else
			{
				SetSpliterPosition(Area.right-Area.left-FSpliterSize-Value);
			}
		}
		else
		{
			SetSpliterPosition(Value);
		}
	}

	VInt VL_Placement::GetPlacement2Position()
	{
		if(FBehavior==vpbFixPlacement2)
		{
			return FSpliterPosition;
		}
		else
		{
			RECT Area=GetArea();
			if(FSpliterDirection==vpdHorizontal)
			{
				return Area.bottom-Area.top-FSpliterSize-FSpliterPosition;
			}
			else
			{
				return Area.right-Area.left-FSpliterSize-FSpliterPosition;
			}
		}
	}

	void VL_Placement::SetPlacement2Position(VInt Value)
	{
		if(FBehavior==vpbFixPlacement2)
		{
			SetSpliterPosition(Value);
		}
		else
		{
			RECT Area=GetArea();
			if(FSpliterDirection==vpdHorizontal)
			{
				SetSpliterPosition(Area.bottom-Area.top-FSpliterSize-Value);
			}
			else
			{
				SetSpliterPosition(Area.right-Area.left-FSpliterSize-Value);
			}
		}
	}

	namespace placement
	{
		class _pControl : public pBase
		{
		protected:
			VL_WinControl*		FControl;
			VInt				FBorder;
			VInt				FMinWidth;
			VInt				FMinHeight;
		public:
			_pControl(VL_WinControl* Control , VInt Border , VInt MinWidth , VInt MinHeight)
			{
				FControl=Control;
				FBorder=Border;
				FMinWidth=MinWidth;
				FMinHeight=MinHeight;
			}

			void Apply(VL_Placement* Placement)
			{
				Placement->SetBorderSize(FBorder);
				Placement->SetMinClientWidth(FMinWidth);
				Placement->SetMinClientHeight(FMinHeight);
				Placement->SetControl(FControl);
			}
		};

		class _pPart : public pBase
		{
		public:
			enum Behavior
			{
				Fix1,
				Fix2,
				Scale
			};
		protected:
			VBool				FHorizontal;
			VInt				FSpliterSize;
			VDouble				FSpliterScale;
			VInt				FBorder;
			Behavior			FBehavior;
			pBase::Ptr			FPlacement1;
			pBase::Ptr			FPlacement2;
		public:
			_pPart(VBool Horizontal , VInt SpliterSize , VDouble SpliterScale , VInt Border , Behavior PlacementBehavior , pBase::Ptr Placement1 , pBase::Ptr Placement2)
			{
				FHorizontal=Horizontal;
				FSpliterSize=SpliterSize;
				FSpliterScale=SpliterScale;
				FBorder=Border;
				FBehavior=PlacementBehavior;
				FPlacement1=Placement1;
				FPlacement2=Placement2;
			}

			void Apply(VL_Placement* Placement)
			{
				Placement->SetSpliterDirection(FHorizontal?vpdHorizontal:vpdVertical);
				Placement->SetSpliterSize(FSpliterSize);
				Placement->SetBorderSize(FBorder);
				switch(FBehavior)
				{
				case Fix1:
					Placement->SetBehavior(vpbFixPlacement1);
					break;
				case Fix2:
					Placement->SetBehavior(vpbFixPlacement2);
					break;
				case Scale:
					Placement->SetBehavior(vpbFixScale);
					Placement->SetSpliterScale(FSpliterScale);
					break;
				}
				FPlacement1->Apply(Placement->GetPlacement1());
				FPlacement2->Apply(Placement->GetPlacement2());
				RECT MinSize1=Placement->GetPlacement1()->GetMinSize();
				RECT MinSize2=Placement->GetPlacement2()->GetMinSize();
				VInt Min1=FHorizontal?(MinSize1.bottom-MinSize1.top):(MinSize1.right-MinSize1.left);
				VInt Min2=FHorizontal?(MinSize2.bottom-MinSize2.top):(MinSize2.right-MinSize2.left);
				switch(FBehavior)
				{
				case Fix1:
					Placement->SetPlacement1Position(Min1);
					break;
				case Fix2:
					Placement->SetPlacement2Position(Min2);
					break;
				case Scale:
					if(Min1)
					{
						Placement->SetPlacement1Position(Min1);
					}
					else
					{
						Placement->SetPlacement2Position(Min2);
					}
					break;
				}
			}
		};

		pBlank::pBlank(VInt MinWidth , VInt MinHeight)
		{
			FMinWidth=MinWidth;
			FMinHeight=MinHeight;
		}

		pBlank::operator pBase::Ptr()
		{
			return new _pControl(0,0,FMinWidth,FMinHeight);
		}

		pControl::pControl(VL_WinControl* Control)
		{
			FControl=Control;
			FBorder=0;
			FMinWidth=0;
			FMinHeight=0;
		}

		pControl::pControl(VL_WinControl* Control , VInt Border)
		{
			FControl=Control;
			FBorder=Border;
			FMinWidth=0;
			FMinHeight=0;
		}

		pControl::pControl(VL_WinControl* Control , VInt MinWidth , VInt MinHeight)
		{
			FControl=Control;
			FBorder=0;
			FMinWidth=MinWidth;
			FMinHeight=MinHeight;
		}

		pControl::pControl(VL_WinControl* Control , VInt Border , VInt MinWidth , VInt MinHeight)
		{
			FControl=Control;
			FBorder=Border;
			FMinWidth=MinWidth;
			FMinHeight=MinHeight;
		}

		pControl::operator pBase::Ptr()
		{
			return new _pControl(FControl,FBorder,FMinWidth,FMinHeight);
		}

		pVertFix1::pVertFix1(VInt Border , VInt SpliterSize , pBase::Ptr Placement1 , pBase::Ptr Placement2)
		{
			FBorder=Border;
			FSpliterSize=SpliterSize;
			FPlacement1=Placement1;
			FPlacement2=Placement2;
		}

		pVertFix1::operator pBase::Ptr()
		{
			return new _pPart(false,FSpliterSize,0,FBorder,_pPart::Fix1,FPlacement1,FPlacement2);
		}

		pVertFix2::pVertFix2(VInt Border , VInt SpliterSize , pBase::Ptr Placement1 , pBase::Ptr Placement2)
		{
			FBorder=Border;
			FSpliterSize=SpliterSize;
			FPlacement1=Placement1;
			FPlacement2=Placement2;
		}

		pVertFix2::operator pBase::Ptr()
		{
			return new _pPart(false,FSpliterSize,0,FBorder,_pPart::Fix2,FPlacement1,FPlacement2);
		}

		pVertScale::pVertScale(VInt Border , VInt SpliterSize , VDouble Scale , pBase::Ptr Placement1 , pBase::Ptr Placement2)
		{
			FBorder=Border;
			FSpliterSize=SpliterSize;
			FScale=Scale;
			FPlacement1=Placement1;
			FPlacement2=Placement2;
		}

		pVertScale::operator pBase::Ptr()
		{
			return new _pPart(false,FSpliterSize,FScale,FBorder,_pPart::Scale,FPlacement1,FPlacement2);
		}

		pHorzFix1::pHorzFix1(VInt Border , VInt SpliterSize , pBase::Ptr Placement1 , pBase::Ptr Placement2)
		{
			FBorder=Border;
			FSpliterSize=SpliterSize;
			FPlacement1=Placement1;
			FPlacement2=Placement2;
		}

		pHorzFix1::operator pBase::Ptr()
		{
			return new _pPart(true,FSpliterSize,0,FBorder,_pPart::Fix1,FPlacement1,FPlacement2);
		}

		pHorzFix2::pHorzFix2(VInt Border , VInt SpliterSize , pBase::Ptr Placement1 , pBase::Ptr Placement2)
		{
			FBorder=Border;
			FSpliterSize=SpliterSize;
			FPlacement1=Placement1;
			FPlacement2=Placement2;
		}

		pHorzFix2::operator pBase::Ptr()
		{
			return new _pPart(true,FSpliterSize,0,FBorder,_pPart::Fix2,FPlacement1,FPlacement2);
		}

		pHorzScale::pHorzScale(VInt Border , VInt SpliterSize , VDouble Scale , pBase::Ptr Placement1 , pBase::Ptr Placement2)
		{
			FBorder=Border;
			FSpliterSize=SpliterSize;
			FScale=Scale;
			FPlacement1=Placement1;
			FPlacement2=Placement2;
		}

		pHorzScale::operator pBase::Ptr()
		{
			return new _pPart(true,FSpliterSize,FScale,FBorder,_pPart::Scale,FPlacement1,FPlacement2);
		}
	}

/*********************************************************************************************************
VL_WinClass
*********************************************************************************************************/

	VL_WinClass::VL_WinClass(VUnicodeString Name , VBool Shadow , VBool OwnDC , WNDPROC Procedure , HINSTANCE hInstance)
	{
		FName=Name;

		FWindowClass.cbSize=sizeof(FWindowClass);
		FWindowClass.style=CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | (Shadow?CS_DROPSHADOW:0) | (OwnDC?CS_OWNDC:0);
		FWindowClass.lpfnWndProc=Procedure;
		FWindowClass.cbClsExtra=0;
		FWindowClass.cbWndExtra=0;
		FWindowClass.hInstance=hInstance;
		FWindowClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
		FWindowClass.hCursor=LoadCursor(NULL,IDC_ARROW);
		FWindowClass.hbrBackground=GetSysColorBrush(COLOR_BTNFACE);
		FWindowClass.lpszMenuName=NULL;
		FWindowClass.lpszClassName=Name.Buffer();
		FWindowClass.hIconSm=NULL;

		FClassAtom=RegisterClassEx(&FWindowClass);
	}

	VBool VL_WinClass::IsAvailable()
	{
		return FClassAtom!=0;
	}

	VUnicodeString VL_WinClass::GetName()
	{
		if(IsAvailable())
		{
			return FName;
		}
		else
		{
			return L"";
		}
	}

	ATOM VL_WinClass::GetClassAtom()
	{
		return FClassAtom;
	}

/*********************************************************************************************************
VL_WinControl
*********************************************************************************************************/

	VBool VL_WinControl::_CreateWindow(DWORD ExStyle , DWORD Style , PWChar Class , VL_WinContainer* Parent)
	{
		if(FHandle==NULL)
		{
			FHandle=CreateWindowEx(
				ExStyle,
				Class,
				L"",
				Style,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				Parent?Parent->GetHandle():NULL,
				NULL,
				GetApplication()->GetInstance(),
				NULL);
			if(FHandle!=NULL)
			{
				GetApplication()->FControls.Add(FHandle,this);
				if(Class!=GetDefaultClass()->GetName())
				{
					SetWindowSubclass(FHandle,SubclassProc,0,0);
				}
				if(Parent)
				{
					SetParent(Parent);
				}
				SetFont(GetApplication()->GetDefaultFont());
				return true;
			}
			else
			{
				DWORD Error=GetLastError();
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	DWORD VL_WinControl::InternalGetExStyle()
	{
		return GetWindowLongPtr(FHandle,GWL_EXSTYLE);
	}

	void VL_WinControl::InternalSetExStyle(DWORD ExStyle)
	{
		SetWindowLongPtr(FHandle,GWL_EXSTYLE,ExStyle);
		SetWindowPos(FHandle,0,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	VBool VL_WinControl::GetExStyle(DWORD ExStyle)
	{
		LONG_PTR Long=InternalGetExStyle();
		return (Long & ExStyle) != 0;
	}

	void VL_WinControl::SetExStyle(DWORD ExStyle , VBool Available)
	{
		LONG_PTR Long=InternalGetExStyle();
		if(Available)
		{
			Long|=ExStyle;
		}
		else
		{
			Long&=~ExStyle;
		}
		InternalSetExStyle(Long);
	}

	VBool VL_WinControl::GetStyle(DWORD Style)
	{
		LONG_PTR Long=GetWindowLongPtr(FHandle,GWL_STYLE);
		return (Long & Style) != 0;
	}

	void VL_WinControl::SetStyle(DWORD Style , VBool Available)
	{
		LONG_PTR Long=GetWindowLongPtr(FHandle,GWL_STYLE);
		if(Available)
		{
			Long|=Style;
		}
		else
		{
			Long&=~Style;
		}
		SetWindowLongPtr(FHandle,GWL_STYLE,Long);
		SetWindowPos(FHandle,0,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	void VL_WinControl::TrackMouse(VBool Enable , VInt HoverTime)
	{
		TRACKMOUSEEVENT Event;
		Event.cbSize=sizeof(Event);
		Event.hwndTrack=FHandle;
		Event.dwFlags=(Enable?0:TME_CANCEL) | TME_HOVER | TME_LEAVE;
		Event.dwHoverTime=HoverTime==-1?HOVER_DEFAULT:HoverTime;
		TrackMouseEvent(&Event);
	}

	void VL_WinControl::Destroy()
	{
		if(FParent)
		{
			FParent->UnregisterChild(this);
			FParent=0;
		}
		else if(FHandle)
		{
			if(FSubClassed)
			{
				RemoveWindowSubclass(FHandle,SubclassProc,0);
			}
			DestroyWindow(FHandle);
		}
		if(FHandle)
		{
			GetApplication()->FControls.Delete(FHandle);
			FHandle=0;
		}
	}

	void VL_WinControl::GetWindowPosSize(VInt& l , VInt& t , VInt& w , VInt & h)
	{
		RECT Rect;
		GetWindowRect(FHandle,&Rect);
		if(FParent)
		{
			POINT Point;
			Point.x=Rect.left;
			Point.y=Rect.top;
			MapWindowPoints(NULL,FParent->FHandle,&Point,1);
			l=Point.x;
			t=Point.y;
		}
		else
		{
			l=Rect.left;
			t=Rect.top;
		}
		w=Rect.right-Rect.left;
		h=Rect.bottom-Rect.top;
	}

	void VL_WinControl::SetWindowPosSize(VInt& l , VInt& t , VInt& w , VInt & h)
	{
		MoveWindow(FHandle,l,t,w,h,TRUE);
	}

	VL_WinControl::VL_WinControl()
	{
		FHandle=NULL;
		FMouseEntered=false;
		FHoverOnce=true;
		FEnableHover=false;
		FHoverTime=-1;
		FLastX=-1;
		FLastY=-1;
		FParent=0;
		FSubClassed=false;
	}

	VL_WinControl::~VL_WinControl()
	{
		Destroy();
	}

	LRESULT VL_WinControl::ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure)
	{
		LRESULT Result=0;
		switch(Message)
		{
		case WM_GETPOINTER_DISPATCHED:
			{
				Result=(LRESULT)this;
				CallDefaultProcedure=false;
			}
			break;
		case WM_ENABLE:
			if(wParam==TRUE)	/*窗口有效*/
				OnEnabled(this);
			else				/*窗口无效*/
				OnDisabled(this);
			break;
		case WM_MOVE:			/*窗口被移动*/
			/*
			X=LOWORD(lParam)
			Y=HIWORD(lParam)
			*/
			OnMoved(this);
			break;
		case WM_MOVING:			/*窗口正在移动*/
			/*
			lParam==LPRECT
			被处理则返回TRUE
			*/
			OnMoving(this,(LPRECT)lParam);
			Result=TRUE;
			break;
		case WM_SHOWWINDOW:
			if(wParam==TRUE)	/*窗口被显示*/
				OnShow(this);
			else				/*窗口被隐藏*/
				OnHide(this);
			break;
		case WM_SIZE:			/*窗口尺寸被修改*/
			OnSized(this);
			break;
		case WM_SIZING:			/*窗口尺寸正在修改*/
			/*
			lParam==LPRECT
			被处理则返回TRUE
			*/
			{
				VBool Left=false;
				VBool Right=false;
				VBool Top=false;
				VBool Bottom=false;
				switch(wParam)
				{
				case WMSZ_BOTTOM:
					Bottom=true;
					break;
				case WMSZ_BOTTOMLEFT:
					Left=true;
					Bottom=true;
					break;
				case WMSZ_BOTTOMRIGHT:
					Right=true;
					Bottom=true;
					break;
				case WMSZ_LEFT:
					Left=true;
					break;
				case WMSZ_RIGHT:
					Right=true;
					break;
				case WMSZ_TOP:
					Top=true;
					break;
				case WMSZ_TOPLEFT:
					Left=true;
					Top=true;
					break;
				case WMSZ_TOPRIGHT:
					Right=true;
					Top=true;
					break;
				}
				OnSizing(this,Left,Right,Top,Bottom,(LPRECT)lParam);
				Result=TRUE;
			}
			break;
		case WM_SETFOCUS:		/*获得焦点*/
			OnGotFocus(this);
			break;
		case WM_KILLFOCUS:		/*失去焦点*/
			OnLostFocus(this);
			break;
		case WM_PAINT:
			OnPaint(this);
			break;
		case WM_MOUSEMOVE:
			{
				VLS_MouseStruct MouseStruct(wParam,lParam,false);
				if((FLastX!=MouseStruct.X) || (FLastY!=MouseStruct.Y))
				{
					FLastX=MouseStruct.X;
					FLastY=MouseStruct.Y;
					OnMouseMove(this,MouseStruct);
					if(!FMouseEntered)
					{
						FMouseEntered=true;
						OnMouseEnter(this);
						TrackMouse(true,FHoverTime);
					}
					else if(FHoverOnce)
					{
						TrackMouse(true,FHoverTime);
					}
				}
			}
			break;
		case WM_LBUTTONDOWN:
			OnLeftButtonDown(this,VLS_MouseStruct(wParam,lParam,false));
			break;
		case WM_LBUTTONUP:
			OnLeftButtonUp(this,VLS_MouseStruct(wParam,lParam,false));
			break;
		case WM_LBUTTONDBLCLK:
			OnLeftButtonDbClick(this,VLS_MouseStruct(wParam,lParam,false));
			break;
		case WM_RBUTTONDOWN:
			OnRightButtonDown(this,VLS_MouseStruct(wParam,lParam,false));
			break;
		case WM_RBUTTONUP:
			OnRightButtonUp(this,VLS_MouseStruct(wParam,lParam,false));
			break;
		case WM_RBUTTONDBLCLK:
			OnRightButtonDbClick(this,VLS_MouseStruct(wParam,lParam,false));
			break;
		case WM_MBUTTONDOWN:
			OnMiddleButtonDown(this,VLS_MouseStruct(wParam,lParam,false));
			break;
		case WM_MBUTTONUP:
			OnMiddleButtonUp(this,VLS_MouseStruct(wParam,lParam,false));
			break;
		case WM_MBUTTONDBLCLK:
			OnMiddleButtonDbClick(this,VLS_MouseStruct(wParam,lParam,false));
			break;
		case WM_MOUSEHWHEEL:
			OnMouseHWheel(this,VLS_MouseStruct(wParam,lParam,true));
			break;
		case WM_MOUSEWHEEL:
			OnMouseVWheel(this,VLS_MouseStruct(wParam,lParam,true));
			break;
		case WM_MOUSEHOVER:
			if(FEnableHover)
			{
				OnMouseHover(this,VLS_MouseStruct(wParam,lParam,false));
				if(!FHoverOnce)
				{
					TrackMouse(true,FHoverTime);
				}
			}
			break;
		case WM_MOUSELEAVE:
			OnMouseLeave(this);
			FMouseEntered=false;
			break;
		case WM_KEYDOWN:
			OnKeyDown(this,VLS_KeyStruct(wParam,lParam));
			break;
		case WM_KEYUP:
			OnKeyUp(this,VLS_KeyStruct(wParam,lParam));
			break;
		case WM_SYSKEYDOWN:
			OnSysKeyDown(this,VLS_KeyStruct(wParam,lParam));
			break;
		case WM_SYSKEYUP:
			OnSysKeyUp(this,VLS_KeyStruct(wParam,lParam));
			break;
		case WM_CHAR:
			{
				VWChar Char=wParam;
				OnChar(this,Char);
				wParam=Char;
				if(wParam==0)
				{
					CallDefaultProcedure=false;
				}
			}
			break;
		case WM_COMMAND:
			if(lParam!=0)
			{
				VL_WinControl* Control=GetApplication()->GetControl((HWND)lParam);
				if(Control)
				{
					Result=Control->ProcessMessage(WM_COMMAND_DISPATCHED,wParam,lParam,CallDefaultProcedure);
				}
			}
			break;
		case WM_NOTIFY:
			{
				NMHDR* hdr=(NMHDR*)lParam;
				VL_WinControl* Control=GetApplication()->GetControl(hdr->hwndFrom);
				if(Control)
				{
					Result=Control->ProcessMessage(WM_NOTIFY_DISPATCHED,wParam,lParam,CallDefaultProcedure);
				}
			}
			break;
		case WM_VSCROLL:
			{
				VL_WinControl* Control=GetApplication()->GetControl((HWND)lParam);
				if(Control)
				{
					Result=Control->ProcessMessage(WM_VSCROLL_DISPATCHED,wParam,lParam,CallDefaultProcedure);
				}
			}
			break;
		case WM_HSCROLL:
			{
				VL_WinControl* Control=GetApplication()->GetControl((HWND)lParam);
				if(Control)
				{
					Result=Control->ProcessMessage(WM_HSCROLL_DISPATCHED,wParam,lParam,CallDefaultProcedure);
				}
			}
			break;
		}
		return Result;
	}

	HWND VL_WinControl::GetHandle()
	{
		return FHandle;
	}

	VL_WinContainer* VL_WinControl::GetParent()
	{
		return FParent;
	}

	void VL_WinControl::SetParent(VL_WinContainer* Parent)
	{
		HWND NewParent=Parent?Parent->GetHandle():NULL;
		if(::SetParent(FHandle,NewParent)!=NULL)
		{
			if(FParent)
			{
				FParent->UnregisterChild(this);
				SetWindowPos(FParent->GetHandle(),NULL,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			FParent=Parent;
			if(FParent)
			{
				FParent->RegisterChild(this);
				SetWindowPos(FParent->GetHandle(),NULL,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
	}

	VBool VL_WinControl::GetEnableHover()
	{
		return FEnableHover;
	}

	void VL_WinControl::SetEnableHover(VBool Value)
	{
		FEnableHover=Value;
		if(FEnableHover)
		{
			TrackMouse(true,FHoverTime);
		}
	}

	VBool VL_WinControl::GetHoverOnce()
	{
		return FHoverOnce;
	}

	void VL_WinControl::SetHoverOnce(VBool Value)
	{
		FHoverOnce=Value;
		if(!FHoverOnce && FEnableHover)
		{
			TrackMouse(true,FHoverTime);
		}
	}

	VInt VL_WinControl::GetHoverTime()
	{
		return FHoverTime;
	}

	void VL_WinControl::SetHoverTime(VInt Value)
	{
		if(Value>=0)
		{
			FHoverTime=Value;
		}
		else
		{
			FHoverTime=-1;
		}
	}

	VInt VL_WinControl::GetLeft()
	{
		VInt l,t,w,h;
		GetWindowPosSize(l,t,w,h);
		return l;
	}

	void VL_WinControl::SetLeft(VInt Value)
	{
		VInt l,t,w,h;
		GetWindowPosSize(l,t,w,h);
		l=Value;
		SetWindowPosSize(l,t,w,h);
	}

	VInt VL_WinControl::GetTop()
	{
		VInt l,t,w,h;
		GetWindowPosSize(l,t,w,h);
		return t;
	}

	void VL_WinControl::SetTop(VInt Value)
	{
		VInt l,t,w,h;
		GetWindowPosSize(l,t,w,h);
		t=Value;
		SetWindowPosSize(l,t,w,h);
	}

	VInt VL_WinControl::GetWidth()
	{
		VInt l,t,w,h;
		GetWindowPosSize(l,t,w,h);
		return w;
	}

	void VL_WinControl::SetWidth(VInt Value)
	{
		VInt l,t,w,h;
		GetWindowPosSize(l,t,w,h);
		w=Value;
		SetWindowPosSize(l,t,w,h);
	}

	VInt VL_WinControl::GetHeight()
	{
		VInt l,t,w,h;
		GetWindowPosSize(l,t,w,h);
		return h;
	}

	void VL_WinControl::SetHeight(VInt Value)
	{
		VInt l,t,w,h;
		GetWindowPosSize(l,t,w,h);
		h=Value;
		SetWindowPosSize(l,t,w,h);
	}

	void VL_WinControl::Move(VInt Left , VInt Top , VInt Width , VInt Height)
	{
		MoveWindow(FHandle,Left,Top,Width,Height,TRUE);
	}

	VUnicodeString VL_WinControl::GetText()
	{
		VInt Length=GetWindowTextLength(FHandle);
		PWChar Buffer=new VWChar[Length+1];
		GetWindowText(FHandle,Buffer,Length+1);
		VUnicodeString Text=Buffer;
		delete[] Buffer;
		return Text;
	}

	void VL_WinControl::SetText(VUnicodeString Value)
	{
		SetWindowText(FHandle,Value.Buffer());
	}

	VBool VL_WinControl::GetEnabled()
	{
		return IsWindowEnabled(FHandle)!=0;
	}

	void VL_WinControl::SetEnabled(VBool Value)
	{
		EnableWindow(FHandle,Value?TRUE:FALSE);
	}

	VBool VL_WinControl::GetVisible()
	{
		return IsWindowVisible(FHandle)!=0;
	}

	void VL_WinControl::SetVisible(VBool Value)
	{
		ShowWindow(FHandle,Value?SW_SHOW:SW_HIDE);
	}

	VBool VL_WinControl::GetFocused()
	{
		return GetFocus()==FHandle;
	}

	void VL_WinControl::SetFocused()
	{
		SetFocus(FHandle);
	}

	VBool VL_WinControl::GetAcceptFiles()
	{
		return GetExStyle(WS_EX_ACCEPTFILES);
	}

	void VL_WinControl::SetAcceptFiles(VBool Value)
	{
		SetExStyle(WS_EX_ACCEPTFILES,Value);
	}

	VL_WinFont::Ptr VL_WinControl::GetFont()
	{
		return FFont;
	}

	void VL_WinControl::SetFont(VL_WinFont::Ptr Font)
	{
		FFont=Font;
		SendMessage(FHandle,WM_SETFONT,(WPARAM)FFont->GetHandle(),TRUE);
	}

/*********************************************************************************************************
VL_WinContainer
*********************************************************************************************************/

	void VL_WinContainer::RegisterChild(VL_WinControl* Control)
	{
		FControls.AddDistinct(Control);
	}

	void VL_WinContainer::UnregisterChild(VL_WinControl* Control)
	{
		if(FPlacement)
		{
			FPlacement->UnregisterControl(Control);
		}
		VInt Index=FControls.IndexOf(Control);
		if(Index!=-1)
		{
			FControls.Delete(Index);
		}
	}

	void VL_WinContainer::Destroy()
	{
		if(FPlacement)
		{
			delete FPlacement;
			FPlacement=0;
		}
		for(VInt i=FControls.GetCount()-1;i>=0;i--)
		{
			if(i<FControls.GetCount())
			{
				delete FControls.Fetch(i);
			}
		}
		VL_WinControl::Destroy();
	}

	void VL_WinContainer::GetClientArea(RECT* Rect)
	{
		GetClientRect(FHandle,Rect);
	}

	VL_WinContainer::VL_WinContainer()
	{
		FPlacement=0;
	}

	VL_WinContainer::~VL_WinContainer()
	{
		Destroy();
	}

	VL_Placement* VL_WinContainer::GetPlacement()
	{
		if(!FPlacement)
		{
			FPlacement=new VL_Placement(this);
		}
		return FPlacement;
	}

	void VL_WinContainer::ApplyPlacement(placement::pBase::Ptr Placement)
	{
		Placement->Apply(GetPlacement());
	}

	VInt VL_WinContainer::GetControlCount()
	{
		return FControls.GetCount();
	}

	VL_WinControl* VL_WinContainer::GetControl(VInt Index)
	{
		if(Index>=0 && Index<FControls.GetCount())
		{
			return FControls[Index];
		}
		else
		{
			return 0;
		}
	}

	VBool VL_WinContainer::GetClipChildren()
	{
		return GetStyle(WS_CLIPCHILDREN);
	}

	void VL_WinContainer::SetClipChildren(VBool Value)
	{
		SetStyle(WS_CLIPCHILDREN,Value);
	}

	VBool VL_WinContainer::GetClipSiblings()
	{
		return GetStyle(WS_CLIPSIBLINGS);
	}

	void VL_WinContainer::SetClipSiblings(VBool Value)
	{
		SetStyle(WS_CLIPSIBLINGS,Value);
	}

	VInt VL_WinContainer::GetClientWidth()
	{
		RECT Rect;
		GetClientArea(&Rect);
		return Rect.right;
	}

	void VL_WinContainer::SetClientWidth(VInt Value)
	{
		SetWidth(Value+GetWidth()-GetClientWidth());
	}

	VInt VL_WinContainer::GetClientHeight()
	{
		RECT Rect;
		GetClientArea(&Rect);
		return Rect.bottom;
	}

	void VL_WinContainer::SetClientHeight(VInt Value)
	{
		SetHeight(Value+GetHeight()-GetClientHeight());
	}

/*********************************************************************************************************
VL_WinForm
*********************************************************************************************************/

	VInt VL_WinForm::RegisterOwned(VL_WinTimer* Timer)
	{
		static VInt TimerHandle=1;
		FTimerList.AddDistinct(Timer);
		return TimerHandle++;
	}

	void VL_WinForm::UnregisterOwned(VL_WinTimer* Timer)
	{
		VInt Index=FTimerList.IndexOf(Timer);
		if(Index!=-1)
		{
			FTimerList.Delete(Index);
		}
	}

	void VL_WinForm::RegisterOwned(VL_WinMenu* Menu)
	{
		FMenuList.AddDistinct(Menu);
	}

	void VL_WinForm::UnregisterOwned(VL_WinMenu* Menu)
	{
		VInt Index=FMenuList.IndexOf(Menu);
		if(Index!=-1)
		{
			FMenuList.Delete(Index);
		}
	}

	VInt VL_WinForm::GenerateMenuItemHandle()
	{
		if(FFreeMenuItemHandles.GetCount())
		{
			return FFreeMenuItemHandles.Fetch();
		}
		else
		{
			return FUsedMenuItemHandle++;
		}
	}

	void VL_WinForm::RegisterMenuItem(VInt Handle , VL_WinMenuItem* Item)
	{
		FMenuItemMap.Add(Handle,Item);
	}

	void VL_WinForm::UnregisterMenuItem(VInt Handle)
	{
		FMenuItemMap.Delete(Handle);
		FFreeMenuItemHandles.Add(Handle);
	}

	void VL_WinForm::InitComponents()
	{
	}

	void VL_WinForm::CreateForm()
	{
		if(!FCreated)
		{
			if(_CreateWindow(
				WS_EX_APPWINDOW | WS_EX_CONTROLPARENT,
				WS_BORDER | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
				GetDefaultClass()->GetName().Buffer(),
				0)
				)
			{
				FIsMainForm=GetApplication()->RegisterForm(this);
				InitComponents();
				OnCreate(this);
			}
			else
			{
				FIsMainForm=false;
			}
			FCreated=true;
		}
	}

	VL_WinForm::VL_WinForm(VBool Create)
	{
		FCreated=false;
		FIsMainForm=false;
		FFormMenu=0;
		FUsedMenuItemHandle=1;
		if(Create)
		{
			CreateForm();
		}
	}

	VL_WinForm::~VL_WinForm()
	{
		GetApplication()->UnregisterForm(this);
	}

	LRESULT VL_WinForm::ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure)
	{
		LRESULT Result=0;
		switch(Message)
		{
		case WM_ACTIVATEAPP:
			if(wParam==TRUE)	/*窗口被激活*/
				OnActivateApp(this);
			else				/*窗口被取消激活*/
				OnDeactivateApp(this);
		case WM_ACTIVATE:
			if(wParam==WA_ACTIVE || wParam==WA_CLICKACTIVE)	/*窗口被激活*/
				OnActivate(this);
			else				/*窗口被取消激活*/
				OnDeactivate(this);
			break;
		case WM_CLOSE:			/*需要关闭窗口*/
			{
				VBool Cancel=false;
				OnQueryClose(this,Cancel);
				if(!Cancel)
				{
					OnClose(this);
					if(FIsMainForm)
					{
						GetApplication()->Terminate();
					}
				}
				else
				{
					CallDefaultProcedure=false;
				}
			}
			break;
		case WM_COMPACTING:		/*内存太少*/
			OnCompacting(this);
			break;
		case WM_CREATE:			/*创建窗口*/
			break;
		case WM_DESTROY:		/*释放窗口*/
			OnDestroy(this);
			{
				/*删除Timer*/
				for(VInt i=FTimerList.GetCount()-1;i>=0;i--)
				{
					if(i<FTimerList.GetCount())
					{
						delete FTimerList.Fetch(i);
					}
				}
				/*删除菜单*/
				for(VInt i=FMenuList.GetCount()-1;i>=0;i--)
				{
					if(i<FMenuList.GetCount())
					{
						delete FMenuList.Fetch(i);
					}
				}
				/*注销热键*/
				for(VInt i=FHotKeys.GetCount()-1;i>=0;i--)
				{
					UnregisterGlobalHotKey(FHotKeys[i]);
				}
			}
			if(FIsMainForm)
			{
				PostQuitMessage(0);
			}
			break;
		case WM_ENTERSIZEMOVE:	/*进入修改尺寸状态*/
			OnEnterSizeMove(this);
			break;
		case WM_EXITSIZEMOVE:	/*退出修改尺寸状态*/
			OnLeaveSizeMove(this);
			break;
		case WM_NCACTIVATE:
			if(wParam==TRUE)	/*非客户区被激活*/
				;
			else				/*非客户区被取消激活*/
				;
			Result=wParam;
			break;
		case WM_HOTKEY:			/*热键*/
			OnHotKey(this,wParam);
			break;
		case WM_TIMER:			/*定时器*/
			for(VInt i=0;i<FTimerList.GetCount();i++)
			{
				VL_WinTimer* Timer=FTimerList[i];
				if(Timer->GetEnabled() && Timer->GetHandle()==wParam)
				{
					Timer->OnTimer(Timer);
				}
			}
			break;
		case WM_COMMAND:
			if(lParam!=0)
			{
				Result=VL_WinContainer::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
			}
			else
			{
				switch(HIWORD(wParam))
				{
				case 0:				/*菜单*/
					{
						VL_WinMenuItem* Item=MenuItemOfHandle(LOWORD(wParam));
						if(Item)
						{
							Item->OnClick(Item);
						}
					}
					break;
				case 1:				/*加速键*/
					{
						VL_AcceleratorItem* Item=GetAcceleratorManager()->GetAcceleratorItem(LOWORD(wParam));
						if(Item)
						{
							Item->Execute();
						}
					}
					break;
				default:
					Result=VL_WinContainer::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
				}
			}
			break;
		case WM_MENURBUTTONUP:	/*菜单右键消息*/
			{
				VInt Handle=GetMenuItemID((HMENU)lParam,wParam);
				VL_WinMenuItem* Item=MenuItemOfHandle(Handle);
				if(Item)
				{
					Item->OnRightClick(Item);
				}
			}
			break;
		default:
			Result=VL_WinContainer::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
		}
		return Result;
	}

	VBool VL_WinForm::RegisterGlobalHotKey(VInt ID , VBool Alt , VBool Ctrl , VBool Shift , VBool Win , VInt KeyCode)
	{
		UINT Modifiers=0;
		if(Alt)		Modifiers|=MOD_ALT;
		if(Ctrl)	Modifiers|=MOD_CONTROL;
		if(Shift)	Modifiers|=MOD_SHIFT;
		if(Win)		Modifiers|=MOD_WIN;
		if(RegisterHotKey(FHandle,ID,Modifiers,KeyCode)!=0)
		{
			FHotKeys.Add(ID);
			return true;
		}
		else
		{
			return false;
		}
	}

	void VL_WinForm::UnregisterGlobalHotKey(VInt ID)
	{
		VInt Index=FHotKeys.IndexOf(ID);
		if(Index!=-1)
		{
			UnregisterHotKey(FHandle,FHotKeys.Fetch(Index));
		}
	}

	VL_WinMenuItem* VL_WinForm::MenuItemOfHandle(VInt Handle)
	{
		VInt Index=FMenuItemMap.IndexOfKey(Handle);
		if(Index==-1)
		{
			return 0;
		}
		else
		{
			return FMenuItemMap.ValueOfIndex(Index);
		}
	}

	VLE_WinFormBorder VL_WinForm::GetBorder()
	{
		if(GetStyle(WS_SIZEBOX))
		{
			return vwfbSizable;
		}
		else if(GetStyle(WS_CAPTION))
		{
			return vwfbSingle;
		}
		else if(GetStyle(WS_BORDER))
		{
			return vwfbBorder;
		}
		else
		{
			return vwfbNone;
		}
	}

	void VL_WinForm::SetBorder(VLE_WinFormBorder Value)
	{
		switch(Value)
		{
		case vwfbNone:
			SetStyle(WS_BORDER,false);
			SetStyle(WS_CAPTION,false);
			SetStyle(WS_SIZEBOX,false);
			break;
		case vwfbBorder:
			SetStyle(WS_BORDER,true);
			SetStyle(WS_CAPTION,false);
			SetStyle(WS_SIZEBOX,false);
			break;
		case vwfbSingle:
			SetStyle(WS_BORDER,true);
			SetStyle(WS_CAPTION,true);
			SetStyle(WS_SIZEBOX,false);
			break;
		case vwfbSizable:
			SetStyle(WS_BORDER,true);
			SetStyle(WS_CAPTION,true);
			SetStyle(WS_SIZEBOX,true);
			break;
		}
	}

	VBool VL_WinForm::GetMaximizeBox()
	{
		return GetStyle(WS_MAXIMIZEBOX);
	}

	void VL_WinForm::SetMaximizeBox(VBool Value)
	{
		SetStyle(WS_MAXIMIZEBOX,Value);
	}

	VBool VL_WinForm::GetMinimizeBox()
	{
		return GetStyle(WS_MINIMIZEBOX);
	}

	void VL_WinForm::SetMinimizeBox(VBool Value)
	{
		SetStyle(WS_MINIMIZEBOX,Value);
	}

	VBool VL_WinForm::GetIconAvailable()
	{
		return GetStyle(WS_SYSMENU);
	}

	void VL_WinForm::SetIconAvailable(VBool Value)
	{
		SetStyle(WS_SYSMENU,Value);
	}

	VBool VL_WinForm::GetAppearInTaskBar()
	{
		return GetExStyle(WS_EX_APPWINDOW);
	}

	void VL_WinForm::SetAppearInTaskBar(VBool Value)
	{
		SetExStyle(WS_EX_APPWINDOW,Value);
	}

	VBool VL_WinForm::GetTopMost()
	{
		return GetExStyle(WS_EX_TOPMOST);
	}

	void VL_WinForm::SetTopMost(VBool Value)
	{
		SetExStyle(WS_EX_TOPMOST,Value);
		SetWindowPos(FHandle,Value?HWND_TOPMOST:HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_DRAWFRAME);
	}

	VBool VL_WinForm::GetTransparent()
	{
		return GetExStyle(WS_EX_TRANSPARENT);
	}

	void VL_WinForm::SetTransparent(VBool Value)
	{
		SetExStyle(WS_EX_TRANSPARENT,Value);
	}

	VBool VL_WinForm::GetWindowEdge()
	{
		return GetExStyle(WS_EX_WINDOWEDGE);
	}

	void VL_WinForm::SetWindowEdge(VBool Value)
	{
		SetExStyle(WS_EX_WINDOWEDGE,Value);
	}

	VBool VL_WinForm::GetToolWindow()
	{
		return GetExStyle(WS_EX_TOOLWINDOW);
	}

	void VL_WinForm::SetToolWindow(VBool Value)
	{
		SetExStyle(WS_EX_TOOLWINDOW,Value);
	}

	VBool VL_WinForm::GetActived()
	{
		return GetActiveWindow()==FHandle;
	}

	void VL_WinForm::SetActived()
	{
		SetActiveWindow(FHandle);
	}

	void VL_WinForm::Show()
	{
		ShowWindow(FHandle,SW_SHOWNORMAL);
	}

	void VL_WinForm::ShowRestored()
	{
		ShowWindow(FHandle,SW_RESTORE);
	}

	void VL_WinForm::ShowMinimized()
	{
		ShowWindow(FHandle,SW_SHOWMINIMIZED);
	}

	void VL_WinForm::ShowMaximized()
	{
		ShowWindow(FHandle,SW_SHOWMAXIMIZED);
	}

	void VL_WinForm::ShowModal()
	{
		ShowWindow(FHandle,SW_SHOWNORMAL);
		GetApplication()->RunModal(this);
	}

	void VL_WinForm::MoveCenter()
	{
		RECT Area,Size;
		GetWindowRect(FHandle,&Size);
		SystemParametersInfo(SPI_GETWORKAREA,0,&Area,0);
		VInt Width=Size.right-Size.left;
		VInt Height=Size.bottom-Size.top;
		VInt ScreenWidth=Area.right-Area.left;
		VInt ScreenHeight=Area.bottom-Area.top;
		MoveWindow(FHandle,Area.left+(ScreenWidth-Width)/2,Area.top+(ScreenHeight-Height)/2,Width,Height,GetVisible()?TRUE:FALSE);
	}

	void VL_WinForm::Close()
	{
		CloseWindow(FHandle);
	}

	void VL_WinForm::DestroyForm()
	{
		Destroy();
	}

	VL_WinFormMenu* VL_WinForm::GetFormMenu()
	{
		return FFormMenu;
	}

	void VL_WinForm::SetFormMenu(VL_WinFormMenu* Value)
	{
		if(FFormMenu)
		{
			FFormMenu->FParent=0;
			FFormMenu->FAssociated=false;
		}
		FFormMenu=Value;
		if(FFormMenu)
		{
			FFormMenu->FParent=this;
			FFormMenu->FAssociated=true;
			SetMenu(FHandle,FFormMenu->GetHandle());
		}
		else
		{
			SetMenu(FHandle,NULL);
		}
	}

/*********************************************************************************************************
VL_WinTimer
*********************************************************************************************************/

	VL_WinTimer::VL_WinTimer(VL_WinForm* Owner)
	{
		FInterval=1000;
		FEnabled=false;
		FOwner=Owner;
		FHandle=FOwner->RegisterOwned(this);
	}

	VL_WinTimer::~VL_WinTimer()
	{
		FOwner->UnregisterOwned(this);
		if(FEnabled)
		{
			KillTimer(FOwner->GetHandle(),FHandle);
		}
	}

	UINT_PTR VL_WinTimer::GetHandle()
	{
		return FHandle;
	}

	VL_WinForm* VL_WinTimer::GetOwner()
	{
		return FOwner;
	}

	VInt VL_WinTimer::GetInterval()
	{
		return FInterval;
	}

	void VL_WinTimer::SetInterval(VInt Value)
	{
		FInterval=Value;
		if(FEnabled)
		{
			SetTimer(FOwner->GetHandle(),FHandle,FInterval,NULL);
		}
	}

	VBool VL_WinTimer::GetEnabled()
	{
		return FEnabled;
	}

	void VL_WinTimer::SetEnabled(VBool Value)
	{
		if(FEnabled!=Value)
		{
			FEnabled=Value;
			if(FEnabled)
			{
				SetTimer(FOwner->GetHandle(),FHandle,FInterval,NULL);
			}
			else
			{
				KillTimer(FOwner->GetHandle(),FHandle);
			}
		}
	}

/*********************************************************************************************************
VL_WinImageList
*********************************************************************************************************/

	void ModifyBitmap(VL_WinBitmap::Ptr Bitmap , COLORREF Color)
	{
		Bitmap->GenerateTrans(Color);
		return;
		if(Color!=RGB(0,0,0))
		{
			for(VInt Y=0;Y<Bitmap->GetHeight();Y++)
			{
				COLORREF* Buffer=(COLORREF*)(Bitmap->GetScanLines()[Y]);
				for(VInt X=Bitmap->GetWidth();X!=0;X--)
				{
					COLORREF Current=*Buffer | 0xFF000000;
					if(Current==0)
					{
						*Buffer=RGB(1,1,1);
					}
					else if(Current==Color)
					{
						*Buffer=RGB(0,0,0);
					}
					Buffer++;
				}
			}
		}
	}

	VL_WinBitmap::Ptr VL_WinImageList::CheckBitmap(VL_WinBitmap::Ptr Bitmap)
	{
		if(Bitmap)
		{
			VL_WinBitmap::Ptr DIB=0;
			VInt w=GetImageWidth();
			VInt h=GetImageHeight();
			if(Bitmap->GetWidth()==w && Bitmap->GetHeight()==h && Bitmap->GetBitmapBits()==VL_WinBitmap::vbb32Bits)
			{
				DIB=Bitmap;
			}
			else
			{
				DIB=new VL_WinBitmap(w,h,VL_WinBitmap::vbb32Bits,true);
				DIB->GetWinDC()->Draw(0,0,w,h,Bitmap);
				DIB->GenerateTrans(GetPixel(DIB->GetWinDC()->GetHandle(),0,0));
			}
			return DIB;
		}
		else
		{
			return 0;
		}
	}

	VL_WinImageList::VL_WinImageList(VInt Width , VInt Height)
	{
		FHandle=ImageList_Create(Width,Height,ILC_COLOR24 | ILC_MASK,8,8);
	}

	VL_WinImageList::VL_WinImageList(VL_WinImageList* ImageList)
	{
		FHandle=ImageList_Duplicate(ImageList->FHandle);
	}

	VL_WinImageList::~VL_WinImageList()
	{
		ImageList_Destroy(FHandle);
	}

	HIMAGELIST VL_WinImageList::GetHandle()
	{
		return FHandle;
	}

	void VL_WinImageList::Add(VL_WinBitmap::Ptr Bitmap)
	{
		VL_WinBitmap::Ptr Dest=CheckBitmap(Bitmap);
		if(GetCount()==0)
		{
			SetBackColor(GetPixel(Dest->GetWinDC()->GetHandle(),0,0));
		}
		ModifyBitmap(Dest,GetBackColor());
		VInt Index=ImageList_Add(FHandle,Dest->GetBitmap(),NULL);
	}

	void VL_WinImageList::Replace(VInt Index , VL_WinBitmap::Ptr Bitmap)
	{
		VL_WinBitmap::Ptr Dest=CheckBitmap(Bitmap);
		ModifyBitmap(Dest,GetBackColor());
		ImageList_Replace(FHandle,Index,Dest->GetBitmap(),NULL);
	}

	void VL_WinImageList::Remove(VInt Index)
	{
		ImageList_Remove(FHandle,Index);
	}

	VInt VL_WinImageList::GetCount()
	{
		return ImageList_GetImageCount(FHandle);
	}

	VInt VL_WinImageList::GetImageWidth()
	{
		VInt X,Y;
		ImageList_GetIconSize(FHandle,&X,&Y);
		return X;
	}

	VInt VL_WinImageList::GetImageHeight()
	{
		VInt X,Y;
		ImageList_GetIconSize(FHandle,&X,&Y);
		return Y;
	}

	COLORREF VL_WinImageList::GetBackColor()
	{
		COLORREF Color=ImageList_GetBkColor(FHandle);
		if(Color==CLR_NONE)
		{
			Color=0;
		}
		return Color;
	}

	void VL_WinImageList::SetBackColor(COLORREF Color)
	{
		ImageList_SetBkColor(FHandle,Color);
	}

	VBool VL_WinImageList::GetUseBackColor()
	{
		return ImageList_GetBkColor(FHandle)!=CLR_NONE;
	}

	void VL_WinImageList::SetUseBackColor(VBool Use)
	{
		if(Use!=GetUseBackColor())
		{
			ImageList_SetBkColor(FHandle,Use?0:CLR_NONE);
		}
	}

	void VL_WinImageList::Draw(VL_WinDC* DC , VInt Index , VInt X , VInt Y , UINT Style)
	{
		ImageList_Draw(FHandle,Index,DC->GetHandle(),X,Y,Style);
	}

/*********************************************************************************************************
VL_WinMenu
*********************************************************************************************************/

	void VL_WinMenu::ItemChanged()
	{
	}

	VL_WinMenu::VL_WinMenu(VL_WinForm* Owner)
	{
		FHandle=0;
		FOwner=Owner;
		FMessageDispatcher=FOwner;
		FAssociated=false;
		FAssociatedMenuItem=0;

		if(FOwner)
		{
			FOwner->RegisterOwned(this);
		}
	}

	VL_WinMenu::~VL_WinMenu()
	{
		if(FOwner)
		{
			FOwner->UnregisterOwned(this);
		}
		for(VInt i=FMenuItems.GetCount()-1;i>=0;i--)
		{
			if(i<FMenuItems.GetCount())
			{
				delete FMenuItems[i];
			}
		}
		if(!FAssociated)
		{
			DestroyMenu(FHandle);
		}
	}

	HMENU VL_WinMenu::GetHandle()
	{
		return FHandle;
	}

	VInt VL_WinMenu::GetCount()
	{
		return FMenuItems.GetCount();
	}

	VL_WinMenuItem* VL_WinMenu::GetItem(VInt Index)
	{
		if(Index>=0 && Index<FMenuItems.GetCount())
		{
			return FMenuItems[Index];
		}
		else
		{
			return  0;
		}
	}

	VBool VL_WinMenu::GetAssociated()
	{
		return FAssociated;
	}

	VL_WinMenuItem* VL_WinMenu::GetAssociatedMenuItem()
	{
		return FAssociatedMenuItem;
	}

	VL_WinMenuItem* VL_WinMenu::Append()
	{
		return Insert(FMenuItems.GetCount());
	}

	VL_WinMenuItem* VL_WinMenu::Insert(VInt Index)
	{
		VL_WinMenuItem* MenuItem=new VL_WinMenuItem(this,FMessageDispatcher->GenerateMenuItemHandle());
		FMenuItems.Insert(Index,MenuItem);

		MENUITEMINFO Info;
		MenuItem->FillStruct(&Info);
		InsertMenuItem(FHandle,Index,TRUE,&Info);

		ItemChanged();
		return MenuItem;
	}

	void VL_WinMenu::Delete(VInt Index)
	{
		if(Index>=0 && Index<FMenuItems.GetCount())
		{
			delete FMenuItems.Fetch(Index);
		}
		ItemChanged();
	}

	void VL_WinMenu::Delete(VL_WinMenuItem* MenuItem)
	{
		Delete(FMenuItems.IndexOf(MenuItem));
	}

/*********************************************************************************************************
VL_WinMenuItem
*********************************************************************************************************/

	void VL_WinMenuItem::OnAttachAcceleratorItem()
	{
		RefreshProperties();
	}

	void VL_WinMenuItem::OnDetachAcceleratorItem()
	{
		RefreshProperties();
	}

	void VL_WinMenuItem::OnExecuteAcceleratorItem()
	{
		OnClick(this);
	}

	void VL_WinMenuItem::FillStruct(MENUITEMINFO* Info)
	{
		if(GetAcceleratorItem())
		{
			FMenuText=FText+L"\t"+GetAcceleratorItem()->GetName();
		}
		else
		{
			FMenuText=FText;
		}
		Info->cbSize=sizeof(*Info);
		Info->fMask=MIIM_CHECKMARKS | MIIM_BITMAP | MIIM_DATA | MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING | MIIM_SUBMENU;
		Info->fType=0;
		{
			if(FSeparator)
			{
				Info->fType|=MFT_SEPARATOR;
			}
			else
			{
				Info->fType|=MFT_STRING;
			}
			if(FRadioCheck)
			{
				Info->fType|=MFT_RADIOCHECK;
			}
		}
		Info->fState=0;
		{
			if(FChecked)
			{
				Info->fState|=MFS_CHECKED;
			}
			else
			{
				Info->fState|=MFS_UNCHECKED;
			}
			if(FEnabled)
			{
				Info->fState|=MFS_ENABLED;
			}
			else
			{
				Info->fState|=MFS_DISABLED;
			}
			if(FHighlighted)
			{
				Info->fState|=MFS_HILITE;
			}
			else
			{
				Info->fState|=MFS_UNHILITE;
			}
		}
		Info->wID=FHandle;
		if(FSubMenu)
		{
			Info->hSubMenu=FSubMenu->GetHandle();
		}
		else
		{
			Info->hSubMenu=NULL;
		}
		Info->hbmpChecked=NULL;
		Info->hbmpUnchecked=NULL;
		Info->dwItemData=(DWORD)this;
		Info->dwTypeData=FMenuText.Buffer();
		Info->cch=FText.Length();
		Info->hbmpItem=FCheckedBitmap?FCheckedBitmap->GetBitmap():0;
	}

	void VL_WinMenuItem::RefreshProperties()
	{
		MENUITEMINFO Info;
		FillStruct(&Info);
		SetMenuItemInfo(FOwner->GetHandle(),FHandle,FALSE,&Info);

		HBITMAP Checked=FCheckedBitmap?FCheckedBitmap->GetBitmap():NULL;
		HBITMAP Unchecked=FUncheckedBitmap?FUncheckedBitmap->GetBitmap():NULL;
		SetMenuItemBitmaps(FOwner->GetHandle(),FHandle,MF_BYCOMMAND,Checked,Unchecked);
		FOwner->ItemChanged();
		VInt Index=GetSystemMetrics(SM_CXMENUCHECK);
	}

	VL_WinBitmap::Ptr VL_WinMenuItem::CheckBitmap(VL_WinBitmap::Ptr Bitmap)
	{
		if(Bitmap)
		{
			VInt w=GetSystemMetrics(SM_CXMENUCHECK);
			VInt h=GetSystemMetrics(SM_CYMENUCHECK);
			if(Bitmap->GetWidth()==w && Bitmap->GetHeight())
			{
				return Bitmap;
			}
			else
			{
				VL_WinBitmap::Ptr DIB=new VL_WinBitmap(w,h,VL_WinBitmap::vbb32Bits,false);
				DIB->GetWinDC()->Draw(0,0,w,h,Bitmap);
				return DIB;
			}
		}
		else
		{
			return 0;
		}
	}

	VL_WinMenuItem::VL_WinMenuItem(VL_WinMenu* Owner , VInt Handle)
	{
		FOwner=Owner;
		FSubMenu=0;
		FHandle=Handle;

		FChecked=false;
		FSeparator=false;
		FRadioCheck=false;
		FRadioGroup=-1;
		FEnabled=true;
		FHighlighted=false;

		FOwner->FMessageDispatcher->RegisterMenuItem(FHandle,this);
	}

	VL_WinMenuItem::~VL_WinMenuItem()
	{
		FOwner->FMessageDispatcher->UnregisterMenuItem(FHandle);
		if(FSubMenu)
		{
			delete FSubMenu;
		}
	}

	VL_WinMenu* VL_WinMenuItem::GetOwner()
	{
		return FOwner;
	}

	VBool VL_WinMenuItem::GetChecked()
	{
		return FChecked;
	}

	void VL_WinMenuItem::SetChecked(VBool Value)
	{
		FChecked=Value;
		if(FChecked && FRadioCheck)
		{
			for(VInt i=0;i<FOwner->GetCount();i++)
			{
				VL_WinMenuItem* Item=FOwner->GetItem(i);
				if(Item->FChecked && Item->FRadioCheck && Item->FRadioGroup==FRadioGroup && Item!=this)
				{
					Item->SetChecked(false);
				}
			}
		}
		RefreshProperties();
	}

	VBool VL_WinMenuItem::GetSeparator()
	{
		return FSeparator;
	}

	void VL_WinMenuItem::SetSeparator(VBool Value)
	{
		FSeparator=Value;
		RefreshProperties();
	}

	VBool VL_WinMenuItem::GetRadioCheck()
	{
		return FRadioCheck;
	}

	void VL_WinMenuItem::SetRadioCheck(VBool Value)
	{
		FRadioCheck=Value;
		RefreshProperties();
	}

	VInt VL_WinMenuItem::GetRadioGroup()
	{
		return FRadioGroup;
	}

	void VL_WinMenuItem::SetRadioGroup(VInt Value)
	{
		FRadioGroup=Value;
		RefreshProperties();
	}

	VBool VL_WinMenuItem::GetEnabled()
	{
		return FEnabled;
	}

	void VL_WinMenuItem::SetEnabled(VBool Value)
	{
		FEnabled=Value;
		RefreshProperties();
	}

	VBool VL_WinMenuItem::GetHighlighted()
	{
		return FHighlighted;
	}

	void VL_WinMenuItem::SetHighlighted(VBool Value)
	{
		FHighlighted=Value;
		RefreshProperties();
	}

	VUnicodeString VL_WinMenuItem::GetText()
	{
		return FText;
	}

	void VL_WinMenuItem::SetText(VUnicodeString Text)
	{
		FText=Text;
		RefreshProperties();
	}

	VL_WinBitmap::Ptr VL_WinMenuItem::GetCheckedBitmap()
	{
		return FCheckedBitmap;
	}

	void VL_WinMenuItem::SetCheckedBitmap(VL_WinBitmap::Ptr Bitmap)
	{
		FCheckedBitmap=CheckBitmap(Bitmap);
		RefreshProperties();
	}

	VL_WinBitmap::Ptr VL_WinMenuItem::GetUncheckedBitmap()
	{
		return FUncheckedBitmap;
	}

	void VL_WinMenuItem::SetUncheckedBitmap(VL_WinBitmap::Ptr Bitmap)
	{
		FUncheckedBitmap=CheckBitmap(Bitmap);
		RefreshProperties();
	}

	VL_WinPopupMenu* VL_WinMenuItem::GetSubMenu()
	{
		return FSubMenu;
	}

	VL_WinPopupMenu* VL_WinMenuItem::CreateSubMenu()
	{
		if(!FSubMenu)
		{
			FSubMenu=new VL_WinPopupMenu(0);
			FSubMenu->FMessageDispatcher=FOwner->FMessageDispatcher;
			FSubMenu->FAssociated=true;
			FSubMenu->FAssociatedMenuItem=this;
			RefreshProperties();
		}
		return FSubMenu;
	}

	void VL_WinMenuItem::DestroySubMenu()
	{
		if(FSubMenu)
		{
			FSubMenu->FAssociated=false;
			delete FSubMenu;
			FSubMenu=0;
			RefreshProperties();
		}
	}

/*********************************************************************************************************
VL_WinFormMenu
*********************************************************************************************************/

	void VL_WinFormMenu::ItemChanged()
	{
		if(FParent)
		{
			DrawMenuBar(FParent->GetHandle());
		}
	}

	VL_WinFormMenu::VL_WinFormMenu(VL_WinForm* Owner):VL_WinMenu(Owner)
	{
		FParent=0;
		FHandle=CreateMenu();
	}

	VL_WinFormMenu::~VL_WinFormMenu()
	{
	}

/*********************************************************************************************************
VL_WinPopupMenu
*********************************************************************************************************/

	VL_WinPopupMenu::VL_WinPopupMenu(VL_WinForm* Owner):VL_WinMenu(Owner)
	{
		FHandle=CreatePopupMenu();
	}

	VL_WinPopupMenu::~VL_WinPopupMenu()
	{
	}
	
	VL_WinMenuItem* VL_WinPopupMenu::Popup(VInt X , VInt Y , VBool KeepPrevMenu)
	{
		BOOL ID=TrackPopupMenuEx(
					FHandle,
					TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD | (KeepPrevMenu?TPM_RECURSE:0),
					X,
					Y,
					FMessageDispatcher->GetHandle(),
					NULL);
		VL_WinMenuItem* Item=FMessageDispatcher->MenuItemOfHandle(ID);
		if(Item)
		{
			Item->OnClick(Item);
		}
		return Item;
	}

	VL_WinMenuItem* VL_WinPopupMenu::Popup(VBool KeepPrevMenu)
	{
		POINT Point;
		GetCursorPos(&Point);
		return Popup(Point.x,Point.y,KeepPrevMenu);
	}

/*********************************************************************************************************
工具函数
*********************************************************************************************************/

	VLE_MsgboxResult VL_WinMsgbox(
		VL_WinForm* Caller ,
		VUnicodeString Text , 
		VUnicodeString Caption , 
		VLE_MsgboxButton Button , 
		VLE_MsgboxIcon Icon, 
		VLE_MsgboxDefaultButton DefaultButton
		)
	{
		UINT Type=0;
		switch(Button)
		{
		case vmbAbortRetryIgnore:
			Type=MB_ABORTRETRYIGNORE;
			break;
		case vmbOK:
			Type=MB_OK;
			break;
		case vmbOKCancel:
			Type=MB_OKCANCEL;
			break;
		case vmbRetryCancel:
			Type=MB_RETRYCANCEL;
			break;
		case vmbYesNo:
			Type=MB_YESNO;
			break;
		case vmbYesNoCancel:
			Type=MB_YESNOCANCEL;
			break;
		}
		switch(Icon)
		{
		case vmiExclamation:
			Type|=MB_ICONEXCLAMATION;
			break;
		case vmiInformation:
			Type|=MB_ICONINFORMATION;
			break;
		case vmiQuestion:
			Type|=MB_ICONQUESTION;
			break;
		case vmiError:
			Type|=MB_ICONERROR;
			break;
		}
		switch(DefaultButton)
		{
		case vmdbButton1:
			Type|=MB_DEFBUTTON1;
			break;
		case vmdbButton2:
			Type|=MB_DEFBUTTON2;
			break;
		case vmdbButton3:
			Type|=MB_DEFBUTTON3;
			break;
		case vmdbButton4:
			Type|=MB_DEFBUTTON4;
			break;
		}
		Type|=MB_APPLMODAL;
		switch(MessageBox(Caller?Caller->GetHandle():NULL,Text.Buffer(),Caption.Buffer(),Type))
		{
		case IDABORT:
			return vmrAbort;
		case IDIGNORE:
			return vmrIgnore;
		case IDRETRY:
			return vmrRetry;
		case IDOK:
			return vmrOK;
		case IDCANCEL:
			return vmrCancel;
		case IDYES:
			return vmrYes;
		case IDNO:
			return vmrNo;
		default:
			return vmrOK;
		}
	}

	void ShowMessage(VL_WinForm* Caller , VUnicodeString Message)
	{
		VL_WinMsgbox(Caller,Message,GetApplication()->GetMainForm()->GetText(),vmbOK);
	}

	}
}