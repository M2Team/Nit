#ifndef VL_DATA_BALANCETREE
#define VL_DATA_BALANCETREE

#include "VL_Data_SearchTree.h"

namespace vl
{
	namespace collection
	{
/*
		template<typename _Type , typename _ExtType=_Type>
		class VL_BalTree : public VL_SearchTree<_Type , _ExtType>
		{
		public:
			class VL_obbNode : public VL_osbNode
			{
			protected:
				VInt	FHeight;
			public:
				VL_obbNode();
				VL_obbNode(_Type Data);
				VL_obbNode*	GetLeft();
				VL_obbNode*	GetRight();
				VL_obbNode*	GetParent();
				VL_obbNode*	GetChild(VBool Left);
				VInt		GetHeight();			获得节点高度
		public:
		};
*/
		template<typename _Type , typename _ExtType=_Type>
		class VL_BalTree : public VL_SearchTree<_Type , _ExtType>
		{
		public:
			class VL_obbNode : public VL_osbNode
			{
			protected:
				VInt	FHeight;
			public:
				VL_obbNode()
				{
					FHeight=1;
				}
				VL_obbNode(_Type Data)
				{
					FData=Data;
					FHeight=1;
				}
				VL_obbNode* GetLeft()
				{
					return (VL_obbNode*)(FLeft);
				}
				VL_obbNode* GetRight()
				{
					return (VL_obbNode*)(FRight);
				}
				VL_obbNode* GetParent()
				{
					return (VL_obbNode*)(FParent);
				}
				VL_obbNode* GetChild(VBool Left)
				{
					return (VL_obbNode*)(Left?FLeft:FRight);
				}
				VInt GetHeight()
				{
					return FHeight;
				}
			protected:
				VL_obbNode* RotateLeft(VL_cobNode*& Root)
				{
					VL_obbNode* OldRight=GetRight();
					VL_obbNode* NewRight=OldRight->GetLeft();
					if(FParent)
					{
						GetParent()->_Replace(this,OldRight);
					}
					else
					{
						Root=OldRight;
					}
					_SetRight(NewRight);
					OldRight->_SetLeft(this);
					CalculateHeight();
					OldRight->CalculateHeight();
					return OldRight;
				}
				VL_obbNode* RotateRight(VL_cobNode*& Root)
				{
					VL_obbNode* OldLeft=GetLeft();
					VL_obbNode* NewLeft=OldLeft->GetRight();
					if(FParent)
					{
						GetParent()->_Replace(this,OldLeft);
					}
					else
					{
						Root=OldLeft;
					}
					_SetLeft(NewLeft);
					OldLeft->_SetRight(this);
					CalculateHeight();
					OldLeft->CalculateHeight();
					return OldLeft;
				}
				void CalculateHeight()
				{
					VL_obbNode* Left=GetLeft();
					VL_obbNode* Right=GetRight();
					VInt LeftHeight=0;
					VInt RightHeight=0;
					if(Left)
					{
						LeftHeight=Left->FHeight;
					}
					if(Right)
					{
						RightHeight=Right->FHeight;
					}
					FHeight=(LeftHeight>RightHeight)?(LeftHeight+1):(RightHeight+1);
				}
				void CalculateHeight(VL_obbNode*& Left , VL_obbNode*& Right , VInt& LeftHeight , VInt& RightHeight)
				{
					Left=GetLeft();
					Right=GetRight();
					if(Left)
					{
						LeftHeight=Left->FHeight;
					}
					else
					{
						LeftHeight=0;
					}
					if(Right)
					{
						RightHeight=Right->FHeight;
					}
					else
					{
						RightHeight=0;
					}
					FHeight=(LeftHeight>RightHeight)?(LeftHeight+1):(RightHeight+1);
				}
				void FixBalance(VL_cobNode*& Root)
				{
					VL_obbNode* Current=this;
					VInt OldHeight=0;
					VInt LeftHeight=0;
					VInt RightHeight=0;
					VL_obbNode* Left=0;
					VL_obbNode* Right=0;

					while(Current)
					{
						OldHeight=Current->FHeight;
						Current->CalculateHeight(Left,Right,LeftHeight,RightHeight);
						if(LeftHeight==RightHeight+1||LeftHeight==RightHeight-1||LeftHeight==RightHeight)
						{
						}
						else if(LeftHeight-RightHeight==2)
						{
							VL_obbNode* LeftLeft=Left->GetLeft();
							VL_obbNode* LeftRight=Left->GetRight();
							VInt LeftLeftHeight=LeftLeft?LeftLeft->FHeight:0;
							VInt LeftRightHeight=LeftRight?LeftRight->FHeight:0;

							if(LeftLeftHeight>LeftRightHeight)
							{
								Current=Current->RotateRight(Root);
							}
							else
							{
								VL_obbNode* A=Current;
								VL_obbNode* B=Left;
								VL_obbNode* C=LeftRight;
								VL_obbNode* CLeft=C->GetLeft();
								VL_obbNode* CRight=C->GetRight();
								VL_obbNode* Parent=A->GetParent();
								if(Parent)
								{
									Parent->_Replace(A,C);
								}
								else
								{
									Root=C;
								}
								B->_SetRight(CLeft);
								A->_SetLeft(CRight);
								C->_SetLeft(B);
								C->_SetRight(A);
								A->CalculateHeight();
								B->CalculateHeight();
								C->CalculateHeight();
								Current=C;
							}
						}
						else if(RightHeight-LeftHeight==2)
						{
							VL_obbNode* RightLeft=Right->GetLeft();
							VL_obbNode* RightRight=Right->GetRight();
							VInt RightLeftHeight=RightLeft?RightLeft->FHeight:0;
							VInt RightRightHeight=RightRight?RightRight->FHeight:0;

							if(RightRightHeight>RightLeftHeight)
							{
								Current=Current->RotateLeft(Root);
							}
							else
							{
								VL_obbNode* A=Current;
								VL_obbNode* B=Right;
								VL_obbNode* C=RightLeft;
								VL_obbNode* CLeft=C->GetLeft();
								VL_obbNode* CRight=C->GetRight();
								VL_obbNode* Parent=A->GetParent();
								if(Parent)
								{
									Parent->_Replace(A,C);
								}
								else
								{
									Root=C;
								}
								B->_SetLeft(CRight);
								A->_SetRight(CLeft);
								C->_SetLeft(A);
								C->_SetRight(B);
								A->CalculateHeight();
								B->CalculateHeight();
								C->CalculateHeight();
								Current=C;
							}
						}
						if(OldHeight==Current->FHeight)
						{
							break;
						}
						else
						{
							Current=Current->GetParent();
						}
					}
				}
				void AfterAdd(VL_cobNode*& Root)
				{
					FixBalance(Root);
				}
				void AfterDelete(VL_cobNode*& Root)
				{
					FixBalance(Root);
				}
			};
		protected:
			virtual VL_osbNode* CreateNode(_Type Data)
			{
				return new VL_obbNode(Data);
			}
		public:
		};

	}
}

#endif