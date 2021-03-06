/*******************************************************************************
Vczh Library++ 2.0
语法工具::有穷状态自动机模拟器
开发者：陈梓瀚

接口：
类：
  VL_RegPureSimulator			：非扩展正则表达式模拟器
  VL_RegRichSimulator			：扩展正则表达式模拟器
  VL_RegGreedSimulator			：贪婪正则表达式模拟器
函数：
注意事项：
  使用PureSimulator的时候，字符串开始，字符串结束和匹配存储区的功能都将失效
*******************************************************************************/

#ifndef VL_REGSIM
#define VL_REGSIM

#include "VL_RegFA.h"

namespace vl
{
	namespace grammar
	{

/*********************************************************************************************************
非扩展正则表达式模拟器
*********************************************************************************************************/

		class VL_RegPureSimulator : public VL_Base
		{
			typedef VL_List<VL_RegDFA::Status* , true>		DFAList;
		protected:
			DFAList						FStatusList;			/*DFA状态表*/
			VL_RegExpCharClass*			FCharClass;				/*字符类*/
			VInt**						FTransitionFunction;	/*转移函数：Status×CharClass->Status*/
		public:

			class Result : public VL_Base
			{
			public:
				VBool					Succeeded;				/*是否成功*/
				VInt					MatchedLength;			/*成功匹配的长度*/
				VL_RegDFA::Status*		FiniteStatus;			/*匹配完成所在的终结状态*/
			};

		public:
			VL_RegPureSimulator(VL_RegDFA::Status* DFA , VL_RegExpCharClass* CharClass);
			~VL_RegPureSimulator();

			void						Match(PCWChar Text , Result& MatchResult , VBool PreferLongMatch);
		};

		typedef VL_AutoPtr<VL_RegPureSimulator>									VL_RegPureSimulatorPtr;
		typedef VL_List<VL_RegPureSimulatorPtr , false , VL_RegPureSimulator*>	VL_RegPureSimulatorList;

/*********************************************************************************************************
正则表达死匹配结果
*********************************************************************************************************/

		class SimulatorResult
		{
		public:
			class SimString
			{
			public:
				VInt					Start;
				VInt					Length;

				SimString();
				SimString(VInt aStart , VInt aLength);
			};

			typedef VL_MultiMap<VInt , SimString , false>							StringListMap;
			typedef StringListMap::ValueList										StringList;
			typedef VL_List<VL_AutoPtr<SimulatorResult> , false , SimulatorResult*>	List;
		public:
			SimString				MatchedString;			/*成功匹配的字符串*/
			VL_RegDFA::Status*		FiniteStatus;			/*匹配完成所在的终结状态*/
			StringList				CatchedStrings;			/*捕获到全局存储区的子字符串*/
			StringListMap			StorageStrings;			/*捕获到命名存储区的子字符串*/
		};

/*********************************************************************************************************
扩展正则表达式模拟器
*********************************************************************************************************/

		typedef VL_List<VL_RegDFA::Status* , true>	VL_RegDFAStatusList;
		class RegSimulator;

		class VL_RegRichSimulator : public VL_Base
		{
		protected:
			VL_RegDFA::Status*			FMainDFA;
			VL_RegPureSimulatorList		FLookAheadSimulatorList;
			VL_RegExpCharClass*			FCharClass;
			RegSimulator*				FInternalSimulator;
		public:
			VL_RegRichSimulator(VL_RegDFA::Status* MainDFA , VL_RegDFAStatusList& LookAheadDFAs , VL_RegExpCharClass* CharClass);
			~VL_RegRichSimulator();

			void						Match(PWChar Text , PWChar InitText);
			void						GetAllResults(RegSimulator* Simulator , SimulatorResult::List& List);
			VL_AutoPtr<SimulatorResult>	Filtrate(VBool TestLengthFirst);
			VL_RegDFA::Status*			GetDFA();
		};

/*********************************************************************************************************
贪婪正则表达式模拟器
*********************************************************************************************************/

		template<typename _Type>
		class VL_RegGreedStack : public VL_Base
		{
		public:
			_Type*						Items;
			VInt						Capacity;
			VInt						Count;

			/*使用一个基量构造堆栈*/
			VL_RegGreedStack(VInt aCapacity)
			{
				if(aCapacity<16)
				{
					aCapacity=16;
				}
				Count=0;
				Capacity=aCapacity;
				Items=new _Type[Capacity];
			}

			~VL_RegGreedStack()
			{
				delete[] Items;
			}

			/*推入数据*/
			_Type& Push()
			{
				if(Count==Capacity)
				{
					Capacity*=2;
					_Type* NewItems=new _Type[Capacity];
					memcpy(NewItems,Items,sizeof(_Type)*Count);
					delete[] Items;
					Items=NewItems;
				}
				return Items[Count++];
			}

			/*弹出数据*/
			_Type& Pop()
			{
				if(Count>0)
				{
					Count--;
				}
				return Items[Count];
			}

			/*获得栈顶*/
			_Type& Top()
			{
				return Items[Count-1];
			}

			/*清除数据*/
			void Clear()
			{
				Count=0;
			}
		};

		class VL_RegGreedSimulator : public VL_Base
		{
		public:
			class Storage
			{
			public:
				typedef VL_RegGreedStack<Storage>		Stack;

				VInt					StorageID;		/*-1为匿名 捕获*/
				VInt					Depth;			/*有效状态堆栈深度*/
				VInt					Start;			/*起始位置*/
				VInt					Length;			/*长度*/
			};

			class Command
			{
			public:
				typedef VL_RegGreedStack<Command>		Stack;

				VLE_RegEdgeInstruction	Instruction;	/*指令*/
				VInt					Parameter;		/*参数*/
				VInt					Previous;		/*前一个未终结指令*/
				VInt					Start;			/*字符串起始*/
			};

			class Status
			{
			public:
				typedef VL_RegGreedStack<Status>		Stack;

				VL_RegNFA::Status*		NFAStatus;		/*状态*/
				VInt					EdgeIndex;		/*边序号*/
				VInt					CommandDepth;	/*命令堆栈深度*/
				VInt					Start;			/*字符串起始*/
			};
		protected:
			Storage::Stack				FStorages;
			Command::Stack				FCommands;
			Status::Stack				FStatus;
			PWChar						FInitText;
			PWChar						FStartText;
			VInt						FMatchedLength;
			VL_RegNFA::Status*			FFinalStatus;

			VL_RegNFA::Status*			FMainNFA;
			VL_RegPureSimulatorList		FLookAheadSimulatorList;
			VL_RegExpCharClass*			FCharClass;
		public:

			VL_RegGreedSimulator(VL_RegNFA::Status* MainNFA , VL_RegDFAStatusList& LookAheadDFAs , VL_RegExpCharClass* CharClass);

			void						FillResult(SimulatorResult* Result);
			VInt						Match(PWChar Start , PWChar InitText);
			VL_RegNFA::Status*			GetNFA();
		};

	}
}

#endif