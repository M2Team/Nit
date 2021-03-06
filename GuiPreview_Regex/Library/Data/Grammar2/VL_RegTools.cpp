#include "VL_RegTools.h"
#include "VL_RegSim.h"
#include "..\VL_Uniop.h"

#include "..\Data\VL_Data_Map.h"

namespace vl
{
	namespace grammar
	{
		using namespace uniop;

/*********************************************************************************************************
VL_RegMatch
*********************************************************************************************************/

	namespace regexp_internal
	{
		class Match
		{
		public:
			VL_RegFAInfo			Info;
			VL_RegNFA				NFA;
			VL_RegDFA				DFA;
			VL_RegPureSimulator*	Simulator;

			Match(VL_RegExpPtr Expression):Info(Expression)
			{
				RegularExpressionToNFA(&NFA,&Info,true);
				VL_RegDFA::Status* InitStatus=CompressedNFAToDFA(&DFA,Info.MainNFA);
				Simulator=new VL_RegPureSimulator(InitStatus,&Info.CharClass);
			};

			~Match()
			{
				delete Simulator;
			}
		};
	}

	VL_RegMatch::ResultArray VL_RegMatch::MakeArray(ResultList& List)
	{
		ResultArray Array=NewArray[List.GetCount()];
		for(VInt i=0;i<List.GetCount();i++)
		{
			Array[i]=List[i];
		}
		return Array;
	}

	void VL_RegMatch::MatchAll(VUnicodeString Input , ResultList& Results , VBool KeepEmptyPart , VBool KeepMatched , VBool KeepUnmatched)
	{
		PWChar Buffer=Input.Buffer();
		PWChar LastEnd=Buffer;
		VL_RegPureSimulator::Result SimulatorResult;
		while(*Buffer)
		{
			/*进行一次匹配*/
			FInternal->Simulator->Match(Buffer,SimulatorResult,true);
			if(SimulatorResult.Succeeded && SimulatorResult.MatchedLength>0)
			{
				/*保存空隙*/
				if((LastEnd!=Buffer || KeepEmptyPart) && KeepUnmatched)
				{
					VL_RegMatch::Result MatchResult;
					MatchResult.Matched=false;
					MatchResult.Position=(VInt)(LastEnd-Input.Buffer());
					MatchResult.Text=VUnicodeString(LastEnd,VInt(Buffer-LastEnd));
					Results.Add(MatchResult);
				}
				/*保存匹配结果*/
				if(KeepMatched)
				{
					VL_RegMatch::Result MatchResult;
					MatchResult.Matched=true;
					MatchResult.Position=(VInt)(Buffer-Input.Buffer());
					MatchResult.Text=VUnicodeString(Buffer,SimulatorResult.MatchedLength);
					Results.Add(MatchResult);
				}
				Buffer+=SimulatorResult.MatchedLength;
				LastEnd=Buffer;
			}
			else
			{
				Buffer++;
			}
		}
		/*保存结尾的空隙*/
		if((LastEnd!=Buffer || KeepEmptyPart) && KeepUnmatched)
		{
			VL_RegMatch::Result MatchResult;
			MatchResult.Matched=false;
			MatchResult.Position=(VInt)(LastEnd-Input.Buffer());
			MatchResult.Text=VUnicodeString(LastEnd,VInt(Buffer-LastEnd));
			Results.Add(MatchResult);
		}
	}

	VL_RegMatch::VL_RegMatch(VUnicodeString Expression)
	{
		FInternal=0;
		VL_RegExpResult Result=RegularExpressionAnalysis(Expression,false);
		if(Result.Error)
		{
			VL_RegError Error;
			Error.Expression=Expression;
			Error.Message=Result.ErrorMessage;
			Error.Position=Result.ErrorPosition;
			throw Error;
		}
		FInternal=new regexp_internal::Match(Result.Expression);
	}

	VL_RegMatch::~VL_RegMatch()
	{
		if(FInternal)
		{
			delete FInternal;
		}
	}

	VL_RegMatch::Result VL_RegMatch::Match(VUnicodeString Input)
	{
		VL_RegPureSimulator::Result	SimulatorResult;
		VL_RegMatch::Result			MatchResult;

		for(VInt i=0;i<Input.Length();i++)
		{
			FInternal->Simulator->Match(Input.Buffer()+i,SimulatorResult,true);
			if(SimulatorResult.Succeeded)
			{
				MatchResult.Matched=true;
				MatchResult.Position=i;
				MatchResult.Text=Input.SubString(i,SimulatorResult.MatchedLength);
				return MatchResult;
			}
		}

		MatchResult.Matched=false;
		MatchResult.Position=0;
		return MatchResult;
	}

	VL_RegMatch::Result VL_RegMatch::MatchHead(VUnicodeString Input)
	{
		VL_RegPureSimulator::Result	SimulatorResult;
		VL_RegMatch::Result			MatchResult;

		FInternal->Simulator->Match(Input.Buffer(),SimulatorResult,true);
		if(SimulatorResult.Succeeded)
		{
			MatchResult.Matched=true;
			MatchResult.Position=0;
			MatchResult.Text=Input.Left(SimulatorResult.MatchedLength);
		}
		else
		{
			MatchResult.Matched=false;
			MatchResult.Position=0;
		}
		return MatchResult;
	}

	VL_RegMatch::Result VL_RegMatch::MatchWhole(VUnicodeString Input)
	{
		VL_RegMatch::Result MatchResult=MatchHead(Input);
		if(MatchResult.Text!=Input)
		{
			MatchResult.Matched=false;
			MatchResult.Text=L"";
		}
		return MatchResult;
	}

	VInt VL_RegMatch::MatchBuffer(PCWChar Input)
	{
		VL_RegPureSimulator::Result	SimulatorResult;
		FInternal->Simulator->Match(Input,SimulatorResult,true);
		if(SimulatorResult.Succeeded)
		{
			return SimulatorResult.MatchedLength;
		}
		else
		{
			return -1;
		}
	}

	void VL_RegMatch::Search(VUnicodeString Input , ResultList& Results)
	{
		Results.Clear();
		MatchAll(Input,Results,true,true,false);
	}

	VL_RegMatch::ResultArray VL_RegMatch::Search(VUnicodeString Input)
	{
		ResultList Results;
		Search(Input,Results);
		return MakeArray(Results);
	}

	void VL_RegMatch::Split(VUnicodeString Input , ResultList& Results , VBool KeepEmptyPart)
	{
		Results.Clear();
		MatchAll(Input,Results,KeepEmptyPart,false,true);
	}

	VL_RegMatch::ResultArray VL_RegMatch::Split(VUnicodeString Input , VBool KeepEmptyPart)
	{
		ResultList Results;
		Split(Input,Results,KeepEmptyPart);
		return MakeArray(Results);
	}

	void VL_RegMatch::Cut(VUnicodeString Input , ResultList& Results , VBool KeepEmptyPart)
	{
		Results.Clear();
		MatchAll(Input,Results,KeepEmptyPart,true,true);
	}

	VL_RegMatch::ResultArray VL_RegMatch::Cut(VUnicodeString Input , VBool KeepEmptyPart)
	{
		ResultList Results;
		Cut(Input,Results,KeepEmptyPart);
		return MakeArray(Results);
	}

/*********************************************************************************************************
VL_RegExp
*********************************************************************************************************/

	namespace regexp_internal
	{
		typedef VL_AutoPtr<SimulatorResult>					MatchResult;
		typedef VL_List<VUnicodeString , false>				StringList;
		class ExpResult
		{
		public:
			MatchResult						Result;
			VUnicodeString					InitString;
			StringList*						StorageIDs;
			SimulatorResult::SimString		MatchedString;
			VBool							Success;

			ExpResult(StringList* aStorageIDs , VUnicodeString aInitString)
			{
				StorageIDs=aStorageIDs;
				InitString=aInitString;
			}
		};

		class Exp
		{
		public:
			VL_RegFAInfo			Info;
			VL_RegNFA				NFA;
			VL_RegDFA				DFA;
			VL_RegRichSimulator*	RichSimulator;
			VL_RegGreedSimulator*	GreedSimulator;

			Exp(VL_RegExpPtr Expression , VBool Greed):Info(Expression)
			{
				RichSimulator=0;
				GreedSimulator=0;
				if(Greed)
				{
					RegularExpressionToNFA(&NFA,&Info,true);

					VL_RegDFAStatusList SubDFAs;
					for(VInt i=0;i<Info.SubNFAs.GetCount();i++)
					{
						SubDFAs.Add(CompressedNFAToDFA(&DFA,Info.SubNFAs[i]));
					}

					GreedSimulator=new VL_RegGreedSimulator(Info.MainNFA,SubDFAs,&Info.CharClass);
				}
				else
				{
					RegularExpressionToNFA(&NFA,&Info,true);
					VL_RegDFA::Status* MainDFA=CompressedNFAToDFA(&DFA,Info.MainNFA);

					VL_RegDFAStatusList SubDFAs;
					for(VInt i=0;i<Info.SubNFAs.GetCount();i++)
					{
						SubDFAs.Add(CompressedNFAToDFA(&DFA,Info.SubNFAs[i]));
					}

					RichSimulator=new VL_RegRichSimulator(MainDFA,SubDFAs,&Info.CharClass);
				}
			}

			~Exp()
			{
				if(RichSimulator)delete RichSimulator;
				if(GreedSimulator)delete GreedSimulator;
			}

			VBool Match(PWChar Text , PWChar Init , MatchResult& Result)
			{
				if(RichSimulator)
				{
					RichSimulator->Match(Text,Init);
					MatchResult r=RichSimulator->Filtrate(false);
					if(r)
					{
						Result=r;
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					VInt Length=GreedSimulator->Match(Text,Init);
					if(Length==-1)
					{
						return false;
					}
					else
					{
						if(!Result)
						{
							Result=new SimulatorResult;
						}
						GreedSimulator->FillResult(Result.Object());
						return true;
					}
				}
			}
		};
	}

	VL_RegExp::Result::Result(StringList* aStorageIDs , VUnicodeString InitString , VInt Line)
	{
		FInternal=new regexp_internal::ExpResult(aStorageIDs,InitString);
		FLine=Line;
	}

	VL_RegExp::Result::~Result()
	{
		delete FInternal;
	}

	void VL_RegExp::Result::GetCatched(StringList& List)
	{
		List.Clear();
		if(FInternal->Success)
		{
			for(VInt i=0;i<FInternal->Result->CatchedStrings.GetCount();i++)
			{
				List.Add(GetCatched(i));
			}
		}
	}

	VUnicodeString VL_RegExp::Result::GetCatched(VInt Index)
	{
		if(FInternal->Success && Index>=0 && Index<FInternal->Result->CatchedStrings.GetCount())
		{
			SimulatorResult::SimString& Str=FInternal->Result->CatchedStrings[Index];
			return FInternal->InitString.SubString(Str.Start,Str.Length);
		}
		else
		{
			return L"";
		}
	}

	VInt VL_RegExp::Result::GetCatchedCount()
	{
		if(FInternal->Success)
		{
			return FInternal->Result->CatchedStrings.GetCount();
		}
		else
		{
			return 0;
		}
	}

	void VL_RegExp::Result::GetStorage(VUnicodeString Name , StringList& List)
	{
		List.Clear();
		if(FInternal->Success)
		{
			VInt NameIndex=FInternal->StorageIDs->IndexOf(Name);
			if(NameIndex!=-1)
			{
				VInt Index=FInternal->Result->StorageStrings.IndexOfKey(NameIndex);
				if(Index!=-1)
				{
					SimulatorResult::StringList& sList=FInternal->Result->StorageStrings.ValueOfIndex(Index);
					for(VInt i=0;i<sList.GetCount();i++)
					{
						SimulatorResult::SimString& Str=sList[i];
						List.Add(FInternal->InitString.SubString(Str.Start,Str.Length));
					}
				}
			}
		}
	}

	VUnicodeString VL_RegExp::Result::GetStorage(VUnicodeString Name , VInt Index)
	{
		if(FInternal->Success)
		{
			VInt NameIndex=FInternal->StorageIDs->IndexOf(Name);
			if(NameIndex!=-1)
			{
				VInt StorageIndex=FInternal->Result->StorageStrings.IndexOfKey(NameIndex);
				if(StorageIndex!=-1)
				{
					SimulatorResult::StringList& List=FInternal->Result->StorageStrings.ValueOfIndex(StorageIndex);
					if(Index>=0 && Index<List.GetCount())
					{
						SimulatorResult::SimString& Str=List[Index];
						return FInternal->InitString.SubString(Str.Start,Str.Length);
					}
				}
			}
		}
		return L"";
	}

	VInt VL_RegExp::Result::GetStorageCount(VUnicodeString Name)
	{
		if(FInternal->Success)
		{
			VInt NameIndex=FInternal->StorageIDs->IndexOf(Name);
			if(NameIndex!=-1)
			{
				VInt StorageIndex=FInternal->Result->StorageStrings.IndexOfKey(NameIndex);
				if(StorageIndex!=-1)
				{
					return FInternal->Result->StorageStrings.ValueOfIndex(StorageIndex).GetCount();
				}
			}
		}
		return -1;
	}

	VBool VL_RegExp::Result::HasStorage(VUnicodeString Name)
	{
		if(FInternal->Success)
		{
			VInt Index=FInternal->StorageIDs->IndexOf(Name);
			return (Index>=0) && (FInternal->Result->StorageStrings.Exists(Index));
		}
		else
		{
			return false;
		}
	}

	void VL_RegExp::Result::GetAllStorages(StringList& Names)
	{
		Names.Clear();
		if(FInternal->Success)
		{
			for(VInt i=0;i<FInternal->Result->StorageStrings.KeyCount();i++)
			{
				Names.Add((*FInternal->StorageIDs)[FInternal->Result->StorageStrings.KeyOfIndex(i)]);
			}
		}
	}

	VInt VL_RegExp::Result::GetPosition()
	{
		if(FInternal->Success)
		{
			return FInternal->Result->MatchedString.Start;
		}
		else
		{
			return FInternal->MatchedString.Start;
		}
	}

	VUnicodeString VL_RegExp::Result::GetText()
	{
		if(FInternal->Success)
		{
			return FInternal->InitString.SubString(FInternal->Result->MatchedString.Start,FInternal->Result->MatchedString.Length);
		}
		else
		{
			return FInternal->InitString.SubString(FInternal->MatchedString.Start,FInternal->MatchedString.Length);
		}
	}

	VBool VL_RegExp::Result::IsMatched()
	{
		return FInternal->Success;
	}

	VInt VL_RegExp::Result::GetLine()
	{
		return FLine;
	}

	/*********************************/

	VL_RegExp::ResultArray VL_RegExp::MakeArray(ResultList& List)
	{
		ResultArray Array=NewArray[List.GetCount()];
		for(VInt i=0;i<List.GetCount();i++)
		{
			Array[i]=List[i];
		}
		return Array;
	}

	void VL_RegExp::InternalMatchAll(VUnicodeString Input , ResultList& Results , VBool KeepEmptyPart , VBool KeepMatched , VBool KeepUnmatched , VInt Line)
	{
		PWChar Buffer=Input.Buffer();
		PWChar InitBuffer=Input.Buffer();
		PWChar LastEnd=Buffer;
		regexp_internal::MatchResult _Result;
		while(*Buffer)
		{
			/*进行一次匹配*/
			_Result=0;
			VBool Success=FInternal->Match(Buffer,Input.Buffer(),_Result);
			if(Success && _Result->MatchedString.Length!=0)
			{
				/*保存空隙*/
				if((LastEnd!=Buffer || KeepEmptyPart) && KeepUnmatched)
				{
					ResultPtr MatchResult=new Result(&FInternal->Info.StorageIDs,Input,Line);
					MatchResult->FInternal->MatchedString=SimulatorResult::SimString(LastEnd-InitBuffer,(VInt)(Buffer-LastEnd));
					MatchResult->FInternal->Success=false;
					Results.Add(MatchResult);
				}
				/*保存匹配结果*/
				if(KeepMatched)
				{
					ResultPtr MatchResult=new Result(&FInternal->Info.StorageIDs,Input,Line);
					MatchResult->FInternal->Result=_Result;
					MatchResult->FInternal->Success=true;
					Results.Add(MatchResult);
				}
				Buffer+=_Result->MatchedString.Length;
				LastEnd=Buffer;
			}
			else
			{
				Buffer++;
			}
		}
		/*保存结尾的空隙*/
		if((LastEnd!=Buffer || KeepEmptyPart) && KeepUnmatched)
		{
			ResultPtr MatchResult=new Result(&FInternal->Info.StorageIDs,Input,Line);
			MatchResult->FInternal->MatchedString=SimulatorResult::SimString(LastEnd-InitBuffer,(VInt)(Buffer-LastEnd));
			MatchResult->FInternal->Success=false;
			Results.Add(MatchResult);
		}
	}

	void VL_RegExp::MatchAll(VUnicodeString Input , ResultList& Results , VBool KeepEmptyPart , VBool KeepMatched , VBool KeepUnmatched , VBool Multiline)
	{
		if(Multiline)
		{
			VL_UniStrings Strings;
			Strings.SetText(Input);
			for(VInt i=0;i<Strings.GetCount();i++)
			{
				InternalMatchAll(Strings[i],Results,KeepEmptyPart,KeepMatched,KeepUnmatched,i);
			}
		}
		else
		{
			InternalMatchAll(Input,Results,KeepEmptyPart,KeepMatched,KeepUnmatched,0);
		}
	}

	void VL_RegExp::InternalMatch(VUnicodeString Input , VBool Head , VBool Whole , ResultPtr MatchResult)
	{
		/*开始分析*/
		PWChar Start=Input.Buffer();
		VBool Success=false;
		do
		{
			Success=FInternal->Match(Start,Input.Buffer(),MatchResult->FInternal->Result);
			if(Head)
			{
				break;
			}
		}while(!Success && *Start++);
		/*处理结果*/
		if(Whole && Success && 
			(
				MatchResult->FInternal->Result->MatchedString.Start!=0 || 
				MatchResult->FInternal->Result->MatchedString.Length!=Input.Length()
			)
		)
		{
			Success=false;
		}
		MatchResult->FInternal->Success=Success;
	}

	VL_RegExp::ResultPtr VL_RegExp::InternalMatch(VUnicodeString Input , VBool Head , VBool Whole)
	{
		/*建立一个结果对象*/
		ResultPtr MatchResult=new Result(&FInternal->Info.StorageIDs,Input,0);
		/*进行分析*/
		InternalMatch(Input,Head,Whole,MatchResult);
		return MatchResult;
	}

	VL_RegExp::VL_RegExp(VUnicodeString Expression , VBool Greed)
	{
		FInternal=0;
		VL_RegExpResult Result=RegularExpressionAnalysis(Expression,true);
		if(Result.Error)
		{
			VL_RegError Error;
			Error.Expression=Expression;
			Error.Message=Result.ErrorMessage;
			Error.Position=Result.ErrorPosition;
			throw Error;
		}
		FInternal=new regexp_internal::Exp(Result.Expression,Greed);
	}

	VL_RegExp::~VL_RegExp()
	{
		if(FInternal)
		{
			delete FInternal;
		}
	}

	VUnicodeString VL_RegExp::GetDescription()
	{
		if(FInternal->RichSimulator)
		{
			return DFAToString(&FInternal->Info,FInternal->RichSimulator->GetDFA());
		}
		else
		{
			return NFAToString(&FInternal->Info,FInternal->GreedSimulator->GetNFA());
		}
	}

	VL_RegExp::StringList& VL_RegExp::GetStorageNames()
	{
		return FInternal->Info.StorageIDs;
	}

	VL_RegExp::ResultPtr VL_RegExp::Match(VUnicodeString Input)
	{
		return InternalMatch(Input,false,false);
	}

	VL_RegExp::ResultPtr VL_RegExp::MatchHead(VUnicodeString Input)
	{
		return InternalMatch(Input,true,false);
	}

	VL_RegExp::ResultPtr VL_RegExp::MatchWhole(VUnicodeString Input)
	{
		return InternalMatch(Input,true,true);
	}

	VBool VL_RegExp::Match(VUnicodeString Input , ResultPtr& MatchResult)
	{
		if(!MatchResult)
		{
			MatchResult=new Result(&FInternal->Info.StorageIDs,Input,0);
		}
		else
		{
			MatchResult->FInternal->InitString=Input;
		}
		InternalMatch(Input,false,false,MatchResult);
		return MatchResult->IsMatched();
	}

	VBool VL_RegExp::MatchHead(VUnicodeString Input , ResultPtr& MatchResult)
	{
		if(!MatchResult)
		{
			MatchResult=new Result(&FInternal->Info.StorageIDs,Input,0);
		}
		else
		{
			MatchResult->FInternal->InitString=Input;
		}
		InternalMatch(Input,true,false,MatchResult);
		return MatchResult->IsMatched();
	}

	VBool VL_RegExp::MatchWhole(VUnicodeString Input , ResultPtr& MatchResult)
	{
		if(!MatchResult)
		{
			MatchResult=new Result(&FInternal->Info.StorageIDs,Input,0);
		}
		else
		{
			MatchResult->FInternal->InitString=Input;
		}
		InternalMatch(Input,true,true,MatchResult);
		return MatchResult->IsMatched();
	}

	void VL_RegExp::Search(VUnicodeString Input , VBool Multiline , ResultList& Results)
	{
		Results.Clear();
		MatchAll(Input,Results,true,true,false,Multiline);
	}

	VL_RegExp::ResultArray VL_RegExp::Search(VUnicodeString Input , VBool Multiline)
	{
		ResultList Results;
		Search(Input,Multiline,Results);
		return MakeArray(Results);
	}

	void VL_RegExp::Split(VUnicodeString Input , VBool Multiline , ResultList& Results , VBool KeepEmptyPart)
	{
		Results.Clear();
		MatchAll(Input,Results,KeepEmptyPart,false,true,Multiline);
	}

	VL_RegExp::ResultArray VL_RegExp::Split(VUnicodeString Input , VBool Multiline , VBool KeepEmptyPart)
	{
		ResultList Results;
		Split(Input,Multiline,Results,KeepEmptyPart);
		return MakeArray(Results);
	}

	void VL_RegExp::Cut(VUnicodeString Input , VBool Multiline , ResultList& Results , VBool KeepEmptyPart)
	{
		Results.Clear();
		MatchAll(Input,Results,KeepEmptyPart,true,true,Multiline);
	}

	VL_RegExp::ResultArray VL_RegExp::Cut(VUnicodeString Input , VBool Multiline , VBool KeepEmptyPart)
	{
		ResultList Results;
		Cut(Input,Multiline,Results,KeepEmptyPart);
		return MakeArray(Results);
	}

/*********************************************************************************************************
VL_RegLex
*********************************************************************************************************/

	typedef VL_List<VL_RegLex::Handler* , true>							ReglexHandlerList;
	typedef VL_List<VL_AutoPtr<VL_RegFAInfo> , false , VL_RegFAInfo*>	ReglexInfoList;
	typedef VL_AutoPtr<VL_RegExpCharClass>								ReglexCharClassPtr;
	typedef VL_ListedMap<VL_RegNFA::Status* , VL_RegLex::Handler*>		ReglexStatusHandlerMap;

	namespace regexp_internal
	{
		class LexHandler
		{
		public:
			VL_RegExpPtr			RegExpTree;
		};

		class Lex
		{
		protected:
			ReglexInfoList			FNFAList;
			ReglexCharClassPtr		FCharClass;
			VL_RegNFA*				FNFA;
			VL_RegDFA*				FDFA;
			VL_RegDFA::Status*		FAutomaton;
			VL_RegPureSimulator*	FSimulator;

			void Clear()
			{
				StatusHandlerMap.Clear();
				FCharClass=0;
				if(FNFA)delete FNFA;
				if(FDFA)delete FDFA;
				if(FSimulator)delete FSimulator;
				FNFA=0;
				FDFA=0;
				FSimulator=0;
			}
		public:
			ReglexHandlerList		Handlers;
			ReglexHandlerList		EnabledHandlers;
			ReglexHandlerList		LimitedHandlers;
			ReglexHandlerList		ErrorHandlers;
			VBool					Modified;
			ReglexStatusHandlerMap	StatusHandlerMap;

			Lex()
			{
				FNFA=0;
				FDFA=0;
				FAutomaton=0;
				FSimulator=0;
			}

			~Lex()
			{
				Clear();
			}

			VL_RegPureSimulator* GetSimulator()
			{
				if(Modified)
				{
					return 0;
				}
				else
				{
					return FSimulator;
				}
			}

			VL_RegExpCharClass* GetCharClass()
			{
				return FCharClass.Object();
			}

			VL_RegDFA::Status* GetAutomaton()
			{
				return FAutomaton;
			}

			void BuildAnalyzer()
			{
				if(Handlers.GetCount()>0)
				{
					Modified=false;
					Clear();
					FNFA=new VL_RegNFA;
					FDFA=new VL_RegDFA;
					/*建立全局的CharClass*/
					{
						VL_RegExpCharClass* GlobalCharClass=new VL_RegExpCharClass;
						FCharClass=GlobalCharClass;
						for(VInt i=0;i<Handlers.GetCount();i++)
						{
							RegularExpressionCharsetNormalize(Handlers[i]->FInternal->RegExpTree,*GlobalCharClass);
						}
					}
					/*建立全局EpsilonNFA*/
					VL_RegNFA::EpsilonNFA EpsilonNFA;
					EpsilonNFA.InitStatus=FNFA->StatusPool.Alloc();
					EpsilonNFA.FinalStatus=FNFA->StatusPool.Alloc();
					EpsilonNFA.FinalStatus->Data.FinalStatus=true;
					/*将每一个表达式转换成压缩有的NFA，然后连接到全局的EpsilonNFA*/
					for(VInt i=0;i<Handlers.GetCount();i++)
					{
						VL_RegFAInfo* Info=new VL_RegFAInfo(Handlers[i]->FInternal->RegExpTree,FCharClass.Object());
						RegularExpressionToNFA(FNFA,Info,true);
						FNFAList.Add(Info);

						RegNFAClosureAlgorithm Algorithm;
						Algorithm.ClosureTest=RegNFAClosureAlgorithm::ctAll;
						Info->MainNFA->Visit(&Algorithm);
						for(VInt j=0;j<Algorithm.Path.GetCount();j++)
						{
							VL_RegNFA::Status* Status=Algorithm.Path[j];
							if(Status->Data.FinalStatus)
							{
								StatusHandlerMap.Add(Status,Handlers[i]);
								Status->Data.FinalStatus=false;
								FNFA->Connect(Status,EpsilonNFA.FinalStatus);
							}
						}
						FNFA->Connect(EpsilonNFA.InitStatus,Info->MainNFA);
					}
					/*把全局EpsilonNFA转换成DFA，然后构造模拟器*/
					FAutomaton=CompressedNFAToDFA(FDFA,CompressNFA(FNFA,EpsilonNFAToNFA(FNFA,EpsilonNFA)));
					FSimulator=new VL_RegPureSimulator(FAutomaton,FCharClass.Object());
				}
			}
		};
	}

	VL_RegLex::Handler::Handler(VUnicodeString Expression)
	{
		FExpression=Expression;
		if(FExpression.Length()>0)
		{
			FInternal=new regexp_internal::LexHandler;
			VL_RegExpResult Result=RegularExpressionAnalysis(Expression,false);
			if(Result.Error)
			{
				VL_RegError Error;
				Error.Expression=Expression;
				Error.Message=Result.ErrorMessage;
				Error.Position=Result.ErrorPosition;
				throw Error;
			}
			FInternal->RegExpTree=Result.Expression;
		}
		else
		{
			FInternal=0;
		}
	}

	VL_RegLex::Handler::~Handler()
	{
		if(FInternal)
		{
			delete FInternal;
		}
	}

	VUnicodeString VL_RegLex::Handler::GetExpression()
	{
		return FExpression;
	}

	VL_RegLex::VL_RegLex()
	{
		FInternal=new regexp_internal::Lex;
		FInternal->Modified=true;
	}

	VL_RegLex::~VL_RegLex()
	{
		delete FInternal;
	}

	VUnicodeString VL_RegLex::ToString()
	{
		return DFAToString(0,FInternal->GetCharClass(),FInternal->GetAutomaton());
	}

	void VL_RegLex::AddHandler(Handler* TokenHandler)
	{
		if(TokenHandler->FInternal)
		{
			if(!FInternal->Handlers.Exists(TokenHandler))
			{
				FInternal->Handlers.Add(TokenHandler);
				FInternal->EnabledHandlers.Add(TokenHandler);
				FInternal->LimitedHandlers.Add(TokenHandler);
				FInternal->Modified=true;
			}
		}
		else
		{
			FInternal->ErrorHandlers.AddDistinct(TokenHandler);
		}
	}

	void VL_RegLex::RemoveHandler(Handler* TokenHandler)
	{
		if(FInternal->Handlers.Exists(TokenHandler))
		{
			FInternal->Handlers.Delete(FInternal->Handlers.IndexOf(TokenHandler));
			if(FInternal->EnabledHandlers.Exists(TokenHandler))
			{
				FInternal->EnabledHandlers.Delete(FInternal->EnabledHandlers.IndexOf(TokenHandler));
			}
			if(FInternal->LimitedHandlers.Exists(TokenHandler))
			{
				FInternal->LimitedHandlers.Delete(FInternal->LimitedHandlers.IndexOf(TokenHandler));
			}
			FInternal->Modified=true;
		}
		else if(FInternal->ErrorHandlers.Exists(TokenHandler))
		{
			FInternal->ErrorHandlers.Delete(FInternal->ErrorHandlers.IndexOf(TokenHandler));
		}
	}

	void VL_RegLex::BuildAnalyzer()
	{
		FInternal->BuildAnalyzer();
	}

	void VL_RegLex::Parse(VUnicodeString Input)
	{
		VL_RegPureSimulator* Simulator=FInternal->GetSimulator();
		VBool* Processed=new VBool[FInternal->Handlers.GetCount()];
		if(Simulator)
		{
			VL_RegPureSimulator::Result Result;
			PWChar Buffer=Input.Buffer();

			while(*Buffer)
			{
				Simulator->Match(Buffer,Result,true);
				VBool HandlerInvoked=false;
				if(!Result.Succeeded)
				{
					if(Result.MatchedLength==0)
					{
						Result.MatchedLength=1;
					}
				}
				if(Result.Succeeded)
				{
					/*清除处理记录*/
					for(VInt i=0;i<FInternal->Handlers.GetCount();i++)
					{
						Processed[i]=false;
					}
					VInt MinIndex=FInternal->Handlers.GetCount();
					/*寻找优先级最高的处理器*/
					for(VInt i=0;i<Result.FiniteStatus->Data.NFAStatus.GetCount();i++)
					{
						VL_RegNFA::Status* NFAStatus=Result.FiniteStatus->Data.NFAStatus[i];
						if(NFAStatus->Data.FinalStatus)
						{
							Handler* TokenHandler=FInternal->StatusHandlerMap[Result.FiniteStatus->Data.NFAStatus[i]];
							VInt Index=FInternal->Handlers.IndexOf(TokenHandler);
							if(Index<MinIndex)
							{
								MinIndex=Index;
							}
							/*如果当前处理器不处于优先级限制中，则立即执行*/
							if(!FInternal->LimitedHandlers.Exists(TokenHandler) && !Processed[Index])
							{
								if(IsEnabled(TokenHandler))
								{
									TokenHandler->Handle(VUnicodeString(Buffer,Result.MatchedLength),(VInt)(Buffer-Input.Buffer()),Buffer);
									Processed[Index]=true;
									HandlerInvoked=true;
								}
							}
						}
					}
					/*使用优先级最高的处理器处理*/
					if(!Processed[MinIndex])
					{
						Handler* MinHandler=FInternal->Handlers[MinIndex];
						if(IsEnabled(MinHandler))
						{
							MinHandler->Handle(VUnicodeString(Buffer,Result.MatchedLength),(VInt)(Buffer-Input.Buffer()),Buffer);
							HandlerInvoked=true;
						}
					}
				}
				/*检查该记号有没有被处理，没有的话则调用错误处理器*/
				if(!HandlerInvoked)
				{
					for(VInt i=0;i<FInternal->ErrorHandlers.GetCount();i++)
					{
						FInternal->ErrorHandlers[i]->Handle(VUnicodeString(Buffer,Result.MatchedLength),(VInt)(Buffer-Input.Buffer()),Buffer);
					}
				}
				if(Result.MatchedLength==0)
				{
					Buffer++;
				}
				else
				{
					Buffer+=Result.MatchedLength;
				}
			}
		}
		delete[] Processed;
	}

	VL_RegLex::Handler* VL_RegLex::ParseOne(PWChar Input , VInt& MatchedLength)
	{
		/*检查模拟器是否有效*/
		VL_RegPureSimulator* Simulator=FInternal->GetSimulator();
		if(Simulator)
		{
			/*进行一次匹配并检查是否成功*/
			VL_RegPureSimulator::Result Result;
			Simulator->Match(Input,Result,true);
			if(Result.Succeeded)
			{
				/*寻找优先级最高的处理器*/
				VInt MinIndex=FInternal->Handlers.GetCount();
				for(VInt i=0;i<Result.FiniteStatus->Data.NFAStatus.GetCount();i++)
				{
					VL_RegNFA::Status* NFAStatus=Result.FiniteStatus->Data.NFAStatus[i];
					if(NFAStatus->Data.FinalStatus)
					{
						Handler* TokenHandler=FInternal->StatusHandlerMap[Result.FiniteStatus->Data.NFAStatus[i]];
						VInt Index=FInternal->Handlers.IndexOf(TokenHandler);
						if(Index<MinIndex)
						{
							MinIndex=Index;
						}
					}
				}
				/*返回结果*/
				if(MinIndex==FInternal->Handlers.GetCount())
				{
					MatchedLength=Result.MatchedLength;
					return 0;
				}
				else
				{
					MatchedLength=Result.MatchedLength;
					return FInternal->Handlers[MinIndex];
				}
			}
			else
			{
				MatchedLength=Result.MatchedLength;
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}

	void VL_RegLex::Enable(Handler* TokenHandler)
	{
		if(FInternal->Handlers.Exists(TokenHandler))
		{
			FInternal->EnabledHandlers.AddDistinct(TokenHandler);
		}
	}

	void VL_RegLex::Disable(Handler* TokenHandler)
	{
		if(FInternal->Handlers.Exists(TokenHandler))
		{
			if(FInternal->EnabledHandlers.Exists(TokenHandler))
			{
				FInternal->EnabledHandlers.Delete(FInternal->EnabledHandlers.IndexOf(TokenHandler));
			}
		}
	}

	VBool VL_RegLex::IsEnabled(Handler* TokenHandler)
	{
		return FInternal->EnabledHandlers.Exists(TokenHandler);
	}

	void VL_RegLex::EnterPriorityLimitation(Handler* TokenHandler)
	{
		if(FInternal->Handlers.Exists(TokenHandler))
		{
			FInternal->LimitedHandlers.AddDistinct(TokenHandler);
		}
	}

	void VL_RegLex::LeavePriorityLimitation(Handler* TokenHandler)
	{
		if(FInternal->Handlers.Exists(TokenHandler))
		{
			if(FInternal->LimitedHandlers.Exists(TokenHandler))
			{
				FInternal->LimitedHandlers.Delete(FInternal->LimitedHandlers.IndexOf(TokenHandler));
			}
		}
	}

	VBool VL_RegLex::IsInPriorityLimitation(Handler* TokenHandler)
	{
		return FInternal->LimitedHandlers.Exists(TokenHandler);
	}

/*********************************************************************************************************
VL_RegSpliter
*********************************************************************************************************/
	
	VL_RegSpliter::Handler::Handler(VUnicodeString Expression , VInt Type):VL_RegLex::Handler(Expression)
	{
		FType=Type;
		FList=0;
	}

	void VL_RegSpliter::Handler::SetResultList(Result::List* List)
	{
		FList=List;
	}

	VUnicodeString VL_RegSpliter::Handler::GetID()
	{
		return FType;
	}

	void VL_RegSpliter::Handler::Handle(VUnicodeString MatchedText , VInt Position , PWChar CurrentBuffer)
	{
		Result::Ptr r=new Result;
		r->Type=FType;
		r->Position=Position;
		r->Text=MatchedText;
		FList->Add(r);
	}

	VL_RegSpliter::VL_RegSpliter()
	{
		FBuilt=false;
		Handler* H=new Handler(L"",-1);
		FHandlers.Add(H);
		FLex.AddHandler(H);
	}

	VL_RegSpliter::~VL_RegSpliter()
	{
	}

	VInt VL_RegSpliter::AddType(VUnicodeString Expression)
	{
		if(FBuilt)
		{
			return -1;
		}
		else
		{
			VInt ID=FHandlers.GetCount();
			Handler* H=new Handler(Expression,ID);
			FHandlers.Add(H);
			FLex.AddHandler(H);
			return ID;
		}
	}

	VInt VL_RegSpliter::TypeCount()
	{
		return FHandlers.GetCount();
	}

	void VL_RegSpliter::BuildAnalyzer()
	{
		if(!FBuilt)
		{
			FBuilt=true;
			FLex.BuildAnalyzer();
		}
	}

	VBool VL_RegSpliter::Parse(VUnicodeString Input , Result::List& List)
	{
		if(FBuilt)
		{
			List.Clear();
			for(VInt i=0;i<FHandlers.GetCount();i++)
			{
				FHandlers[i]->SetResultList(&List);
			}
			FLex.Parse(Input);
		}
		return FBuilt;
	}

	}
}