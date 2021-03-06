/*******************************************************************************
Vczh Library++ 2.0
语法工具::正则表达式工具
开发者：陈梓瀚

接口：
类：
  VL_RegError			：正则表达式错误
  VL_RegMatch			：正则表达式匹配工具	（非扩展）
  VL_RegExp				：正则表达式引擎		（扩展）
  VL_RegLex				：词法分析器			（非扩展）
  VL_RegSpliter			：简单词法分析器		（非扩展）
函数：
注意事项：
  VL_RegMatch和VL_RegExp中
    Match				：返回最早的一个匹配
	MatchHead			：从字符串开始处匹配
	MatchWhole			：尝试匹配整个字符串
	Search				：返回字符串中所有的匹配
	Split				：返回字符串被所有匹配分割后的片段
	Cut					：把字符串分割为匹配和非匹配的片段

  VL_RegLex的Handler的正则表达式为空的时候，将被认为是错误记号的处理器
*******************************************************************************/

#ifndef VL_REGTOOLS
#define VL_REGTOOLS

#include "..\Data\VL_Data_String.h"
#include "..\Data\VL_Data_List.h"

namespace vl
{
	namespace grammar
	{
		using namespace collection;

		namespace regexp_internal
		{
			class Match;
			class Exp;
			class ExpResult;
			class Lex;
			class LexHandler;
		}

		class VL_RegError : public VL_Base
		{
		public:
			VUnicodeString				Expression;				/*表达式信息*/
			VUnicodeString				Message;				/*错误信息*/
			VInt						Position;				/*错误位置*/
		};

/*********************************************************************************************************
正则表达式匹配工具
*********************************************************************************************************/

		class VL_RegMatch : public VL_Base
		{
		protected:
			regexp_internal::Match*		FInternal;
		public:
			class Result : public VL_Base
			{
			public:
				VBool					Matched;				/*是否匹配得到*/
				VInt					Position;				/*匹配到的字符串的起始位置*/
				VUnicodeString			Text;					/*匹配到的字符串*/
			};
			typedef VL_List<Result , false>	ResultList;
			typedef VL_Array<Result , 1>	ResultArray;

			ResultArray					MakeArray(ResultList& List);
			void						MatchAll(VUnicodeString Input , ResultList& Results , VBool KeepEmptyPart , VBool KeepMatched , VBool KeepUnmatched);
		public:
			VL_RegMatch(VUnicodeString Expression);
			~VL_RegMatch();

			Result						Match(VUnicodeString Input);
			Result						MatchHead(VUnicodeString Input);
			Result						MatchWhole(VUnicodeString Input);
			VInt						MatchBuffer(PCWChar Input);

			void						Search	(VUnicodeString Input , ResultList& Results);
			ResultArray					Search	(VUnicodeString Input);
			void						Split	(VUnicodeString Input , ResultList& Results , VBool KeepEmptyPart);
			ResultArray					Split	(VUnicodeString Input , VBool KeepEmptyPart);
			void						Cut		(VUnicodeString Input , ResultList& Results , VBool KeepEmptyPart);
			ResultArray					Cut		(VUnicodeString Input , VBool KeepEmptyPart);
		};

/*********************************************************************************************************
正则表达式引擎
*********************************************************************************************************/

		class RegSimulator;

		class VL_RegExp : public VL_Base
		{
			typedef VL_List<VUnicodeString , false>		StringList;
		protected:
			regexp_internal::Exp*		FInternal;
		public:
			class Result : public VL_Base
			{
				friend class VL_RegExp;
			protected:
				regexp_internal::ExpResult*	FInternal;
				VInt					FLine;
			public:
				Result(StringList* aStorageIDs , VUnicodeString InitString , VInt Line);
				~Result();

				void					GetCatched(StringList& List);						/*获取的字符串*/
				VUnicodeString			GetCatched(VInt Index);								/*获取的字符串*/
				VInt					GetCatchedCount();									/*获得获取的字符串的数量*/
				void					GetStorage(VUnicodeString Name , StringList& List);	/*通过名字获取存储器中的字符串表*/
				VUnicodeString			GetStorage(VUnicodeString Name , VInt Index);		/*通过名字获取存储器中的字符串表*/
				VInt					GetStorageCount(VUnicodeString Name);				/*通过名字获取存储器中的字符串表的尺寸*/
				VBool					HasStorage(VUnicodeString Name);					/*判断一个命名存储器是否存在*/
				void					GetAllStorages(StringList& Names);					/*获取所有命名存储器*/
				VInt					GetPosition();										/*获得匹配的起始位置*/
				VUnicodeString			GetText();											/*匹配到的字符串*/
				VBool					IsMatched();										/*是否是一个匹配，在Cut中区分匹配项与字符串片段*/
				VInt					GetLine();											/*获得字符串开始的行数，仅在多行匹配中有效*/
			};
			typedef VL_AutoPtr<Result>			ResultPtr;
			typedef VL_List<ResultPtr , false>	ResultList;
			typedef VL_Array<ResultPtr , 1>		ResultArray;

			ResultArray					MakeArray(ResultList& List);
			void						InternalMatchAll(VUnicodeString Input , ResultList& Results , VBool KeepEmptyPart , VBool KeepMatched , VBool KeepUnmatched , VInt Line);
			void						MatchAll(VUnicodeString Input , ResultList& Results , VBool KeepEmptyPart , VBool KeepMatched , VBool KeepUnmatched , VBool Multiline);
			void						InternalMatch(VUnicodeString Input , VBool Head , VBool Whole , ResultPtr MatchResult);
			ResultPtr					InternalMatch(VUnicodeString Input , VBool Head , VBool Whole);
		public:
			VL_RegExp(VUnicodeString Expression , VBool Greed);
			~VL_RegExp();

			VUnicodeString				GetDescription();
			StringList&					GetStorageNames();

			ResultPtr					Match(VUnicodeString Input);
			ResultPtr					MatchHead(VUnicodeString Input);
			ResultPtr					MatchWhole(VUnicodeString Input);

			VBool						Match(VUnicodeString Input , ResultPtr& MatchResult);
			VBool						MatchHead(VUnicodeString Input , ResultPtr& MatchResult);
			VBool						MatchWhole(VUnicodeString Input , ResultPtr& MatchResult);

			void						Search	(VUnicodeString Input , VBool Multiline , ResultList& Results);
			ResultArray					Search	(VUnicodeString Input , VBool Multiline);
			void						Split	(VUnicodeString Input , VBool Multiline , ResultList& Results , VBool KeepEmptyPart);
			ResultArray					Split	(VUnicodeString Input , VBool Multiline , VBool KeepEmptyPart);
			void						Cut		(VUnicodeString Input , VBool Multiline , ResultList& Results , VBool KeepEmptyPart);
			ResultArray					Cut		(VUnicodeString Input , VBool Multiline , VBool KeepEmptyPart);
		};

/*********************************************************************************************************
词法分析器
*********************************************************************************************************/

		class VL_RegLex : public VL_Base
		{
		protected:
			regexp_internal::Lex*		FInternal;
		public:
			class Handler : public VL_Base
			{
				friend class regexp_internal::Lex;
				friend class VL_RegLex;
			private:
				VUnicodeString					FExpression;
				regexp_internal::LexHandler*	FInternal;
			public:
				Handler(VUnicodeString Expression);
				~Handler();

				VUnicodeString					GetExpression();

				virtual VUnicodeString			GetID()=0;
				virtual void					Handle(VUnicodeString MatchedText , VInt Position , PWChar CurrentBuffer)=0;
			};
		public:
			VL_RegLex();
			~VL_RegLex();

			/*获得确定性有穷状态自动机的描述*/
			VUnicodeString				ToString();
			/*添加一个处理器*/
			void						AddHandler(Handler* TokenHandler);
			/*删除一个处理器*/
			void						RemoveHandler(Handler* TokenHandler);
			/*构造词法分析器*/
			void						BuildAnalyzer();
			/*进行词法分析*/
			void						Parse(VUnicodeString Input);
			Handler*					ParseOne(PWChar Input , VInt& MatchedLength);

			/*启用一个处理器*/
			void						Enable(Handler* TokenHandler);
			/*禁用一个处理器*/
			void						Disable(Handler* TokenHandler);
			/*返回指定的处理器是否被禁用*/
			VBool						IsEnabled(Handler* TokenHandler);

			/*把一个处理器归入优先级限制*/
			void						EnterPriorityLimitation(Handler* TokenHandler);
			/*把一个处理器移出优先级限制*/
			void						LeavePriorityLimitation(Handler* TokenHandler);
			/*返回指定的处理器是否处于优先级限制状态*/
			VBool						IsInPriorityLimitation(Handler* TokenHandler);
		};

		class VL_RegSpliter : public VL_Base
		{
		public:
			class Result
			{
			public:
				typedef VL_AutoPtr<Result>				Ptr;
				typedef VL_List<Ptr , false , Result*>	List;

				VInt					Position;				/*位置*/
				VInt					Type;					/*类型，-1为匹配失败*/
				VUnicodeString			Text;					/*记号内容*/
			};
		protected:
			class Handler : public VL_RegLex::Handler
			{
			protected:
				VInt					FType;
				Result::List*			FList;
			public:
				typedef VL_AutoPtr<Handler>				Ptr;
				typedef VL_List<Ptr , false , Handler*>	List;

				Handler(VUnicodeString Expression , VInt Type);

				void					SetResultList(Result::List* List);

				VUnicodeString			GetID();
				void					Handle(VUnicodeString MatchedText , VInt Position , PWChar CurrentBuffer);
			};

			Handler::List				FHandlers;
			VL_RegLex					FLex;
			VBool						FBuilt;
		public:
			VL_RegSpliter();
			~VL_RegSpliter();

			VInt						AddType(VUnicodeString Expression);
			VInt						TypeCount();
			void						BuildAnalyzer();
			VBool						Parse(VUnicodeString Input , Result::List& List);
		};
	}
}

#endif