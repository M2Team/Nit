#include "VL_Data_Iterator.h"

namespace vl
{

/*********************************************************************************************************
VL_MarkManagerNode
*********************************************************************************************************/

	VL_MarkManagerNode::VL_MarkManagerNode(VL_MarkManagerNode* Prev , VL_MarkManagerNode* Next)
	{
		if(FPrev=Prev)
		{
			FPrev->SetNext(this);
		}
		if(FNext=Next)
		{
			FNext->SetPrev(this);
		}
		FItem=0;
	}

	VL_MarkManagerNode::~VL_MarkManagerNode()
	{
		if(FPrev)
		{
			FPrev->FNext=FNext;
		}
		if(FNext)
		{
			FNext->FPrev=FPrev;
		}
	}

	VL_MarkManagerNode* VL_MarkManagerNode::GetPrev()
	{
		return FPrev;
	}

	VL_MarkManagerNode* VL_MarkManagerNode::GetNext()
	{
		return FNext;
	}

	void VL_MarkManagerNode::SetPrev(VL_MarkManagerNode* Prev)
	{
		FPrev=Prev;
	}

	void VL_MarkManagerNode::SetNext(VL_MarkManagerNode* Next)
	{
		FNext=Next;
	}

	void VL_MarkManagerNode::SetItem(VL_MarkManagerItem* Item)
	{
		if(FItem=Item)
		{
			FItem->SetMark(this);
		}
	}

	void VL_MarkManagerNode::DestroyItem()
	{
		if(FItem)
		{
			FItem->SetMark(0);
			delete FItem;
		}
		delete this;
	}

/*********************************************************************************************************
VL_MarkManager
*********************************************************************************************************/

	VL_MarkManager::VL_MarkManager()
	{
		FHead=FTail=0;
	}

	VL_MarkManager::~VL_MarkManager()
	{
		if(FHead)
		{
			while(!IsEmpty())
			{
				FHead->GetNext()->DestroyItem();
			}
			FHead->SetNext(0);
			FTail->SetPrev(0);
			delete FHead;
			delete FTail;
		}
	}

	void VL_MarkManager::Mark(VL_MarkManagerItem* Markee)
	{
		if(!FHead)
		{
			FHead=new VL_MarkManagerNode(0,0);
			FTail=new VL_MarkManagerNode(0,0);
			FHead->SetNext(FTail);
			FTail->SetPrev(FHead);
		}
		VL_MarkManagerNode* Item=new VL_MarkManagerNode(FHead,FHead->GetNext());
		Item->SetItem(Markee);
	}

	VBool VL_MarkManager::IsEmpty()
	{
		return FHead==0 || FHead->GetNext()==FTail;
	}
}