#ifndef VL_DATA_LIST
#define VL_DATA_LIST

#include "VL_Data_Iterator.h"

namespace vl
{
	namespace collection
	{
/*
		template<typename _Type , VBool _SolidType , typename _ExtType=_Type>
		class VL_CustomList abstract :public VL_Base
		{
		public:
			VL_CustomList();
			~VL_CustomList();
			VBool	IsLocked();									列表内容是否被锁定
			VInt	GetCount();									获得列表元素数量
			_Type&	operator [] (VInt Index);					获得列表元素引用
			void	Delete(VInt Index);							删除指定元素
			void	Delete(VInt Index , VInt Count);			删除指定元素段
			_Type	Fetch(VInt Index=-1);						删除指定元素并返回
			void	Clear();									清除元素
			_Type*	Buffer();									返回内部缓存
		};
*/
		template<typename _Type , VBool _SolidType , typename _ExtType=_Type>
		class VL_CustomList abstract :public VL_Base
		{
			typedef VL_CustomList<_Type , _SolidType , _ExtType>		_List;
		protected:
			VInt			FCount;
			VInt			FCapacity;
			_Type*			FItems;

			void Grow(VInt Count)
			{
				if(Count<8)
				{
					Count=8;
				}
				_Type* NewItems=new _Type[FCapacity+Count];
				if(FItems)
				{
					for(VInt i=0;i<FCount;i++)
					{
						NewItems[i]=FItems[i];
					}
					delete[] FItems;
				}
				FItems=NewItems;
				FCapacity+=Count;
			}
			VInt CalculateGrow(VInt Capacity)
			{
				return Capacity>=16?Capacity/2:16;
			}
			void InsertSpace(VInt Count)
			{
				VInt Capacity=FCapacity;
				while(Capacity<FCount+Count)
				{
					Capacity+=CalculateGrow(Capacity);
				}
				if(Capacity>FCapacity)
				{
					Grow(Capacity-FCapacity);
				}
			}
			void Move(VInt Index , VInt Count , VInt Offset)
			{
				if(Offset)
				{
					if(Offset>0)
					{
						if(_SolidType)
						{
							memmove(FItems+Index+Offset,FItems+Index,sizeof(_Type)*Count);
						}
						else
						{
							for(VInt i=Count-1;i>=0;i--)
							{
								FItems[Index+i+Offset]=FItems[Index+i];
							}
						}
					}
					else
					{
						if(_SolidType)
						{
							memcpy(FItems+Index+Offset,FItems+Index,sizeof(_Type)*Count);
						}
						else
						{
							for(VInt i=0;i<Count;i++)
							{
								FItems[Index+i+Offset]=FItems[Index+i];
							}
						}
					}
				}
			}

			
			void _Add(const _Type* Items , VInt Count)
			{
				InsertSpace(Count);
				if(_SolidType)
				{
					memmove(FItems+FCount,Items,sizeof(_Type)*Count);
					FCount+=Count;
				}
				else
				{
					for(VInt i=0;i<Count;i++)
					{
						FItems[FCount++]=Items[i];
					}
				}
			}
			void _Insert(VInt Index , const _Type* Items , VInt Count)
			{
				if(Index<0||Index>FCount)
				{
					return;
				}
				InsertSpace(Count);
				Move(Index,FCount-Index,Count);
				if(_SolidType)
				{
					memmove(FItems+Index,Items,sizeof(_Type)*Count);
				}
				else
				{
					for(VInt i=0;i<Count;i++)
					{
						FItems[Index+i]=Items[i];
					}
				}
				FCount+=Count;
			}
			void _Delete(VInt Index , VInt Count , VBool DestroyData)
			{
				if(Index<0||Index+Count>FCount||Count<0)
				{
					return;
				}
				Move(Index+Count,FCount-Index-Count,-Count);

				if(DestroyData)
				{
					for(VInt i=0;i<Count;i++)
					{
						FItems[FCount-1-i]=_Type();
					}
				}

				FCount-=Count;
			}
		public:
			VL_CustomList()
			{
				FCount=0;
				FCapacity=0;
				FItems=0;
			}
			~VL_CustomList()
			{
				if(FItems)
				{
					delete[] FItems;
				}
			}

			VInt GetCount()const
			{
				return FCount;
			}
			_Type& operator [] (VInt Index)
			{
				return FItems[Index];
			}

			void Delete(VInt Index)
			{
				_Delete(Index,1,true);
			}
			void Delete(VInt Index , VInt Count)
			{
				_Delete(Index,Count,true);
			}
			_Type Fetch(VInt Index=-1)
			{
				if(Index<-1||Index>=FCount)
				{
					return _Type();
				}
				else if(Index==-1)
				{
					Index=FCount-1;
				}
				_Type Item=FItems[Index];
				_Delete(Index,1,true);
				return Item;
			}
			void Clear()
			{
				FCount=0;
				//_Delete(0,FCount,true);
			}

			void DeleteUndestroy(VInt Index)
			{
				_Delete(Index,1,false);
			}
			void DeleteUndestroy(VInt Index , VInt Count)
			{
				_Delete(Index,Count,false);
			}
			_Type FetchUndestroy(VInt Index=-1)
			{
				if(Index<-1||Index>=FCount)
				{
					return _Type();
				}
				else if(Index==-1)
				{
					Index=FCount-1;
				}
				_Type Item=FItems[Index];
				_Delete(Index,1,false);
				return Item;
			}
			void ClearUndestroy()
			{
				_Delete(0,FCount,false);
			}

			_Type* Buffer()
			{
				return FItems;
			}
			VInt GetCapacity()const
			{
				return FCapacity;
			}
			void SetCapacity(VInt Capacity)
			{
				if(Capacity==0)
				{
					FCount=0;
					FCapacity=0;
					if(FItems)
					{
						delete[] FItems;
						FItems=0;
					}
				}
				else if(Capacity>0 && Capacity!=FCapacity)
				{
					VInt Min=Capacity<FCount?Capacity:FCount;
					_Type* Items=new _Type[Capacity];
					if(FItems)
					{
						if(_SolidType)
						{
							memcpy(Items,FItems,sizeof(_Type)*Min);
						}
						else
						{
							for(VInt i=0;i<Min;i++)
							{
								Items[i]=FItems[i];
							}
						}
						delete[] FItems;
						FItems=0;
					}
					FItems=Items;
					FCapacity=Capacity;
				}
			}
		};
/*
		template<typename _Type , VBool _SolidType , typename _ExtType=_Type>
		class VL_List :public VL_CustomList<_Type , _SolidType , _ExtType>
		{
			typedef VL_CustomList<_Type , _SolidType , _ExtType> _CustomList;
			typedef IVL_ModifiableIterator<_Type> IVL_ListIterator;
			typedef IVL_BiIterator<_Type> IVL_ListBiIterator;
		public:
			void				Add(_Type Item);									在列表尾部追加元素
			void				Add(Range r);										在列表尾部追加另一张表的一部分元素
			void				Insert(VInt Index , _Type Item);					在指定位置插入元素
			void				Insert(VInt Index , Range r);						在指定位置插入另一张表的一部分元素

			void				AddDistinct(_Type Item);							在列表尾部追加元素
			void				AddDistinct(Range r);								在列表尾部追加另一张表的一部分元素
			void				InsertDistinct(VInt Index , _Type Item);			在指定位置插入元素
			void				InsertDistinct(VInt Index , Range r);				在指定位置插入另一张表的一部分元素

			VInt				IndexOf(_ExtType Item);								获得元素的位置，找不到返回-1
			VBool				Exists(_ExtType Item);								返回某元素是否存在
			Range				Whole();											返回代表全部的表范围
			Range				Part(VInt Index , VInt Count);						宣布代表一部分的表范围
		};
*/
		template<typename _Type , VBool _SolidType , typename _ExtType=_Type>
		class VL_List :public VL_CustomList<_Type , _SolidType , _ExtType>
		{
			typedef VL_CustomList<_Type , _SolidType , _ExtType>	_CustomList;
			typedef VL_List<_Type , _SolidType , _ExtType>			_List;
		public:
			class Range
			{
				friend class VL_List;
			protected:
				VInt			Index;
				VInt			Count;
				const _List*	List;
			};

			VInt IndexOf_Type(_Type& Item)
			{
				for(VInt i=0;i<FCount;i++)
				{
					if(FItems[i]==Item)
					{
						return i;
					}
				}
				return -1;
			}
			VBool Exists_Type(_Type& Item)
			{
				return IndexOf_Type(Item)>-1;
			}
		public:
			VL_List()
			{
			}
			VL_List(const _List& List)
			{
				*this=List;
			}
			VL_List& operator =(const _List& List)
			{
				Clear();
				_Add(List.FItems,List.GetCount());
				return *this;
			}

			void Add(_Type Item)
			{
				_Add(&Item,1);
			}
			void Add(Range r)
			{
				_Add(r.List->FItems+r.Index,r.Count);
			}
			void Insert(VInt Index , _Type Item)
			{
				_Insert(Index,&Item,1);
			}
			void Insert(VInt Index , Range r)
			{
				_Insert(Index,r.List->FItems+r.Index,r.Count);
			}
			
			void AddDistinct(_Type Item)
			{
				if(Exists_Type(Item))
				{
					return;
				}
				_Add(&Item,1);
			}
			void AddDistinct(Range r)
			{
				for(VInt i=r.Index;i<r.Index+r.Count;i++)
				{
					if(!Exists_Type(r.List->FItems[i]))
					{
						_Add(r.List->FItems+i,1);
					}
				}
			}
			void InsertDistinct(VInt Index , _Type Item)
			{
				if(Exists_Type(Item))
				{
					return;
				}
				_Insert(Index,&Item,1);
			}
			void InsertDistinct(VInt Index , Range r)
			{
				for(VInt i=r.Index+r.Count-1;i>=r.Index;i--)
				{
					if(!Exists_Type(r.List->FItems[i]))
					{
						_Insert(Index,r.List->FItems+i,1);
					}
				}
			}
			void Clear()
			{
				_CustomList::Clear();
			}
			void Clear(VInt Count)
			{
				if(Count<0)
				{
					Count=0;
				}
				if(FItems)
				{
					delete[] FItems;
				}
				FItems=new _Type[Count];
				FCount=Count;
				FCapacity=Count;
			}

			VInt IndexOf(_ExtType Item)const
			{
				for(VInt i=0;i<FCount;i++)
				{
					if(FItems[i]==Item)
					{
						return i;
					}
				}
				return -1;
			}
			VBool Exists(_ExtType Item)const
			{
				return IndexOf(Item)>-1;
			}
			Range Whole()const
			{
				Range r;
				r.Index=0;
				r.Count=FCount;
				r.List=this;
				return r;
			}
			Range Part(VInt Index , VInt Count)const
			{
				if(Index<0 || Index+Count>FCount)
				{
					Index=0;
					Count=0;
				}
				Range r;
				r.Index=Index;
				r.Count=Count;
				r.List=this;
				return r;
			}
		};
/*
		template<typename _Type , VBool _SolidType , typename _ExtType=_Type>
		class VL_OrderedList :public VL_CustomList<_Type , _SolidType , _ExtType> , public IVL_OrderedContainer<_Type , _ExtType>
		{
			typedef VL_CustomList<_Type , _SolidType , _ExtType> _CustomList;
			typedef IVL_Iterator<_Type> IVL_ListIterator;
			typedef IVL_BiIterator<_Type> IVL_ListBiIterator;
		public:
			void				Add(_Type Item);							添加元素
			void				AddDistinct(_Type Item);					添加元素
			VInt				IndexOf(_ExtType Item);						获得元素位置，找不到返回-1
			_Type*				DataFrom(_ExtType Item);					获得元素
			VBool				Exists(_ExtType Item);						返回某元素是否存在
		};
*/
		template<typename _Type , VBool _SolidType , typename _ExtType=_Type>
		class VL_OrderedList :public VL_CustomList<_Type , _SolidType , _ExtType>
		{
			typedef VL_CustomList<_Type , _SolidType , _ExtType>	_CustomList;
			typedef VL_OrderedList<_Type , _SolidType , _ExtType>	_OrderedList;
		protected:
			template<typename _CompairType>
			VInt FindInsertPosition(_CompairType& Item)const
			{
				if(FCount)
				{
					VInt Begin=0;
					VInt End=FCount;
					for(;;)
					{
						VInt Pos=(Begin+End)/2;
						if(FItems[Pos]>Item)
						{
							End=Pos;
							if(Begin==End)
							{
								return Pos;
							}
						}
						else if(FItems[Pos]<Item)
						{
							Begin=Pos+1;
							if(Begin==End)
							{
								return Begin;
							}
							else if(Begin>End)
							{
								return End;
							}
						}
						else if(Pos==0)
						{
							return 0;
						}
						else if(FItems[Pos-1]<=Item)
						{
							return Pos;
						}
						else
						{
							if(Begin==End)
							{
								return Pos;
							}
							else
							{
								return Pos;
							}
						}
					}
				}
				else
				{
					return 0;
				}
			}
		public:
			VL_OrderedList()
			{
			}
			VL_OrderedList(const _OrderedList& List)
			{
				*this=List;
			}
			VL_OrderedList& operator =(const _OrderedList& List)
			{
				Clear();
				_Add(List.FItems,List.GetCount());
				return *this;
			}
			void Add(_Type Item)
			{
				VInt Index=FindInsertPosition(Item);
				_Insert(Index,&Item,1);
			}
			void AddDistinct(_Type Item)
			{
				VInt Index=FindInsertPosition(Item);
				if(Index<FCount && FItems[Index]==Item)
				{
					return;
				}
				_Insert(Index,&Item,1);
			}
			VInt IndexOf(_ExtType Item)const
			{
				if(FCount)
				{
					VInt Pos=FindInsertPosition(Item);
					if(Pos==FCount)
					{
						return -1;
					}
					else if(FItems[Pos]==Item)
					{
						return Pos;
					}
					else
					{
						return -1;
					}
				}
				else
				{
					return -1;
				}
			}
			VBool Exists(_ExtType Item)const
			{
				return IndexOf(Item)>-1;
			}
		};

	}
}

#endif