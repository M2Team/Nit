#ifndef VL_DATA_BINTREE
#define VL_DATA_BINTREE

#include "VL_Data_CustomBintree.h"

namespace vl
{
	namespace collection
	{
/*
		template<typename _Type>
		class VL_Bintree:public VL_CustomBintree<_Type>
		{
		public:
			class VL_obNode;
			typedef IVL_Iterator<VL_obNode> IVL_BintreeNodeIterator;
			typedef IVL_Iterator<_Type> IVL_BintreeDataIterator;
			typedef VL_cobNodeIterator<VL_obNode> VL_BintreeNodeIterator;
			typedef VL_cobDataIterator<VL_obNode> VL_BintreeDataIterator;
		public:
			class VL_obNode:public VL_cobNode
			{
			public:
				VL_obNode();
				VL_obNode(_Type Data);
				VL_obNode*				GetLeft();									获得左节点
				VL_obNode*				SetLeft(VL_obNode* Node);					设置左节点
				VL_obNode*				GetRight();									获得右节点
				VL_obNode*				SetRight(VL_obNode* Node);					设置右节点
				VL_obNode*				GetParent();								获得父节点
				VL_obNode*				GetChild(VBool Left);						获得子节点
			};
		public:
			VL_obNode*					GetRoot();									获得根节点，空树返回0
			VL_obNode*					SetRoot(VL_obNode* Root);					设置根节点
			IVL_BintreeNodeIterator*	GetPreorderNodeIterator(VBool LeftFirst);	获得前序节点迭代器
			IVL_BintreeNodeIterator*	GetInorderNodeIterator(VBool LeftFirst);	获得中序节点迭代器
			IVL_BintreeNodeIterator*	GetPostorderNodeIterator(VBool LeftFirst);	获得后序节点迭代器
			IVL_BintreeDataIterator*	GetPreorderDataIterator(VBool LeftFirst);	获得前序元素迭代器
			IVL_BintreeDataIterator*	GetInorderDataIterator(VBool LeftFirst);	获得中序元素迭代器
			IVL_BintreeDataIterator*	GetPostorderDataIterator(VBool LeftFirst);	获得后序元素迭代器
		};
*/
		template<typename _Type>
		class VL_Bintree:public VL_CustomBintree<_Type>
		{
		public:
			class VL_obNode;
			typedef IVL_Iterator<VL_obNode> IVL_BintreeNodeIterator;
			typedef IVL_Iterator<_Type> IVL_BintreeDataIterator;
			typedef VL_cobNodeIterator<VL_obNode> VL_BintreeNodeIterator;
			typedef VL_cobDataIterator<VL_obNode> VL_BintreeDataIterator;
		public:
			class VL_obNode:public VL_cobNode
			{
			public:
				VL_obNode()
				{
				}
				VL_obNode(_Type Data)
				{
					FData=Data;
				}
				VL_obNode* GetLeft()
				{
					return (VL_obNode*)(FLeft);
				}
				VL_obNode* SetLeft(VL_obNode* Node)
				{
					return (VL_obNode*)(_SetLeft(Node));
				}
				VL_obNode* GetRight()
				{
					return (VL_obNode*)(FRight);
				}
				VL_obNode* SetRight(VL_obNode* Node)
				{
					return (VL_obNode*)(_SetRight(Node));
				}
				VL_obNode* GetParent()
				{
					return (VL_obNode*)FParent;
				}
				VL_obNode* GetChild(VBool Left)
				{
					return (VL_obNode*)(Left?FLeft:FRight);
				}
			};
		public:
			VL_obNode* GetRoot()
			{
				if(FRoot)
				{
					return (VL_obNode*)FRoot;
				}
				else
				{
					return 0;
				}
			}
			VL_obNode* SetRoot(VL_obNode* Root)
			{
				return (VL_obNode*)(_SetRoot(Root));
			}

			IVL_BintreeNodeIterator* GetPreorderNodeIterator(VBool LeftFirst)
			{
				VL_BintreeNodeIterator* Iterator=new VL_BintreeNodeIterator(this,vikPreorder,LeftFirst);
				FIteratorManager.Mark(Iterator);
				return Iterator;
			}
			IVL_BintreeNodeIterator* GetInorderNodeIterator(VBool LeftFirst)
			{
				VL_BintreeNodeIterator* Iterator=new VL_BintreeNodeIterator(this,vikInorder,LeftFirst);
				FIteratorManager.Mark(Iterator);
				return Iterator;
			}
			IVL_BintreeNodeIterator* GetPostorderNodeIterator(VBool LeftFirst)
			{
				VL_BintreeNodeIterator* Iterator=new VL_BintreeNodeIterator(this,vikPostorder,LeftFirst);
				FIteratorManager.Mark(Iterator);
				return Iterator;
			}

			IVL_BintreeDataIterator* GetPreorderDataIterator(VBool LeftFirst)
			{
				VL_BintreeDataIterator* Iterator=new VL_BintreeDataIterator(this,vikPreorder,LeftFirst);
				FIteratorManager.Mark(Iterator);
				return Iterator;
			}
			IVL_BintreeDataIterator* GetInorderDataIterator(VBool LeftFirst)
			{
				VL_BintreeDataIterator* Iterator=new VL_BintreeDataIterator(this,vikInorder,LeftFirst);
				FIteratorManager.Mark(Iterator);
				return Iterator;
			}
			IVL_BintreeDataIterator* GetPostorderDataIterator(VBool LeftFirst)
			{
				VL_BintreeDataIterator* Iterator=new VL_BintreeDataIterator(this,vikPostorder,LeftFirst);
				FIteratorManager.Mark(Iterator);
				return Iterator;
			}
		};

	}
}

#endif