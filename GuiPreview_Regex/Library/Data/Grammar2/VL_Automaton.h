/*******************************************************************************
Vczh Library++ 2.0
语法工具::自动机模板库
开发者：陈梓瀚

接口：
  VL_Automaton::IBFSAlgorithm		：基于广度优先搜索的自动机算法接口
类：
  VL_Automaton						：自动机类
  VL_Automaton::Edge				：自动机边
  VL_Automaton::Status				：自动机状态
函数：
*******************************************************************************/

#ifndef VL_AUTOMATON
#define VL_AUTOMATON

#include "..\Data\VL_Data_Pool.h"

namespace vl
{
	namespace grammar
	{
		using namespace pool;
		using namespace collection;

		template<typename _EdgeData , typename _StatusData>
		class VL_Automaton : public VL_Base
		{
		public:
			class Edge;
			class Status;

			class IBFSAlgorithm : public IVL_Interface
			{
			public:
				VL_List<Status* , true>		Path;
				virtual VBool EdgeTest(Edge* aEdge)
				{
					return true;
				}

				virtual void VisitBegin(Status* aStatus , VBool& VisitConnected , VBool& RunEnd)
				{
				}
				virtual void VisitEnd(Status* aStatus)
				{
				}
			};
		protected:
			class Initer : public VL_Base
			{
				friend class VL_Automaton;
			protected:
				VL_Automaton*				Automaton;
			public:
				void Init(Edge* Data)
				{
					Automaton->Init(Data);
				}
				void Init(Status* Data)
				{
					Automaton->Init(Data);
				}
			};
		public:
			typedef VL_List<Edge* , true>	_EdgeList;

			class Edge : public VL_Base
			{
			public:
				Status*						Start;
				Status*						End;
				_EdgeData					Data;
			};

			class Status : public VL_Base
			{
			public:
				_StatusData					Data;
				_EdgeList					OutEdges;
				_EdgeList					InEdges;

				void Visit(IBFSAlgorithm* Algorithm)
				{
					/*初始化路径，填入自身*/
					VL_List<Status* , true>& Stack=Algorithm->Path;
					Stack.Clear();
					Stack.Add(this);
					/*进行宽度优先搜索*/
					for(VInt i=0;i<Stack.GetCount();i++)
					{
						/*取出当前状态*/
						Status* Current=Stack[i];
						VBool VisitConnected=true;
						VBool RunEnd=true;
						/*访问状态*/
						Algorithm->VisitBegin(Current,VisitConnected,RunEnd);
						if(VisitConnected)
						{
							/*取出当前状态的每一条边*/
							for(VInt j=0;j<Current->OutEdges.GetCount();j++)
							{
								Edge* CurrentEdge=Current->OutEdges[j];
								/*如果边的目标尚未访问，而且边通过测试*/
								if(!Stack.Exists(CurrentEdge->End) && Algorithm->EdgeTest(CurrentEdge))
								{
									/*在路径上加入边的目标，以待将来访问*/
									Stack.Add(CurrentEdge->End);
								}
							}
						}
						if(RunEnd)
						{
							/*结束访问状态*/
							Algorithm->VisitEnd(Current);
						}
					}
				}
			};
		public:
			VL_Pool<Edge , Initer>			EdgePool;
			VL_Pool<Status , Initer>		StatusPool;

			VL_Automaton(VInt PackSize):EdgePool(PackSize),StatusPool(PackSize)
			{
				EdgePool.GetInitializer()->Automaton=this;
				StatusPool.GetInitializer()->Automaton=this;
			}

			virtual void Init(Edge* Data)
			{
			}

			virtual void Init(Status* Data)
			{
			}

			Edge* Connect(Status* Start , Status* End)
			{
				Edge* NewEdge=EdgePool.Alloc();
				NewEdge->Start=Start;
				NewEdge->End=End;
				Start->OutEdges.Add(NewEdge);
				End->InEdges.Add(NewEdge);
				return NewEdge;
			}

			void Disconnect(Status* Start , Edge* Edge)
			{
				VInt Index=Start->OutEdges.IndexOf(Edge);
				if(Index>=0)
				{
					Start->OutEdges.Delete(Index);
					Edge->End->InEdges.Delete(Edge->End->InEdges.IndexOf(Edge));
					EdgePool.Free(Edge);
				}
			}

			void Disconnect(Status* Start)
			{
				/*删除与该状态连接的所有边*/
				for(VInt i=Start->InEdges.GetCount()-1;i>=0;i--)
				{
					Disconnect(Start->InEdges[i]->Start,Start->InEdges[i]);
				}
				for(VInt i=Start->OutEdges.GetCount()-1;i>=0;i--)
				{
					Disconnect(Start,Start->OutEdges[i]);
				}
				/*删除该状态*/
				StatusPool.Free(Start);
			}
		};
	}
}

#endif