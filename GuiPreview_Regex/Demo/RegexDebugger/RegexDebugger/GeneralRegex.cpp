#include "GeneralRegex.h"

/********************************************************************************
纯正则表达式
********************************************************************************/

class PureMatch : public GeneralMatch
{
public:
	VL_RegMatch::ResultList*	List;
	VInt						Index;

	VBool IsSuccess()
	{
		return (*List)[Index].Matched;
	}

	VInt Position()
	{
		return (*List)[Index].Position;
	}

	VUnicodeString Text()
	{
		return (*List)[Index].Text;
	}

	VInt Line()
	{
		return 0;
	}

	VInt GetCatchedCount()
	{
		return 0;
	}

	VUnicodeString GetCatched(VInt Index)
	{
		return L"";
	}

	VInt GetStorageCount()
	{
		return 0;
	}

	VUnicodeString GetStorageName(VInt Index)
	{
		return L"";
	}

	VInt GetStorageItemCount(VUnicodeString Name)
	{
		return 0;
	}

	VUnicodeString GetStorageItem(VUnicodeString Name , VInt Index)
	{
		return L"";
	}
};

class PureMatchList : public GeneralMatchList
{
public:
	VL_RegMatch::ResultList			ResultList;
	VL_List<PureMatch* , true>		MatchList;

	~PureMatchList()
	{
		for(VInt i=0;i<MatchList.GetCount();i++)
		{
			delete MatchList[i];
		}
	}

	void Build()
	{
		for(VInt i=0;i<ResultList.GetCount();i++)
		{
			PureMatch* Match=new PureMatch;
			Match->List=&ResultList;
			Match->Index=i;
			MatchList.Add(Match);
		}
	}

	VInt GetCount()
	{
		return MatchList.GetCount();
	}

	GeneralMatch* GetItem(VInt Index)
	{
		return MatchList[Index];
	}
};

class PureRegex : public GeneralRegex
{
public:
	VL_RegMatch*		RegEngine;

	PureRegex(VUnicodeString Regex)
	{
		RegEngine=new VL_RegMatch(Regex);
	}

	~PureRegex()
	{
		delete RegEngine;
	}

	GeneralMatchList* Match(VUnicodeString Text)
	{
		PureMatchList* List=new PureMatchList;
		List->ResultList.Add(RegEngine->Match(Text));
		List->Build();
		return List;
	}

	GeneralMatchList* MatchHead(VUnicodeString Text)
	{
		PureMatchList* List=new PureMatchList;
		List->ResultList.Add(RegEngine->MatchHead(Text));
		List->Build();
		return List;
	}

	GeneralMatchList* MatchWhole(VUnicodeString Text)
	{
		PureMatchList* List=new PureMatchList;
		List->ResultList.Add(RegEngine->MatchWhole(Text));
		List->Build();
		return List;
	}

	GeneralMatchList* Search(VUnicodeString Text , VBool Multiline)
	{
		PureMatchList* List=new PureMatchList;
		RegEngine->Search(Text,List->ResultList);
		List->Build();
		return List;
	}

	GeneralMatchList* Split(VUnicodeString Text , VBool Multiline , VBool KeepEmptyPart)
	{
		PureMatchList* List=new PureMatchList;
		RegEngine->Split(Text,List->ResultList,KeepEmptyPart);
		List->Build();
		return List;
	}

	GeneralMatchList* Cut(VUnicodeString Text , VBool Multiline , VBool KeepEmptyPart)
	{
		PureMatchList* List=new PureMatchList;
		RegEngine->Cut(Text,List->ResultList,KeepEmptyPart);
		List->Build();
		return List;
	}
};

/********************************************************************************
扩展正则表达式
********************************************************************************/

class ExpMatch : public GeneralMatch
{
public:
	VL_RegExp::ResultList*		List;
	VInt						Index;

	VBool IsSuccess()
	{
		return (*List)[Index]->IsMatched();
	}

	VInt Position()
	{
		return (*List)[Index]->GetPosition();
	}

	VUnicodeString Text()
	{
		return (*List)[Index]->GetText();
	}

	VInt Line()
	{
		return (*List)[Index]->GetLine();
	}

	VInt GetCatchedCount()
	{
		return (*List)[Index]->GetCatchedCount();
	}

	VUnicodeString GetCatched(VInt aIndex)
	{
		return (*List)[Index]->GetCatched(aIndex);
	}

	VInt GetStorageCount()
	{
		VL_List<VUnicodeString , false> Names;
		(*List)[Index]->GetAllStorages(Names);
		return Names.GetCount();
	}

	VUnicodeString GetStorageName(VInt aIndex)
	{
		VL_List<VUnicodeString , false> Names;
		(*List)[Index]->GetAllStorages(Names);
		return Names[aIndex];
	}

	VInt GetStorageItemCount(VUnicodeString Name)
	{
		return (*List)[Index]->GetStorageCount(Name);
	}

	VUnicodeString GetStorageItem(VUnicodeString Name , VInt aIndex)
	{
		return (*List)[Index]->GetStorage(Name,aIndex);
	}
};

class ExpMatchList : public GeneralMatchList
{
public:
	VL_RegExp::ResultList			ResultList;
	VL_List<ExpMatch* , true>		MatchList;

	~ExpMatchList()
	{
		for(VInt i=0;i<MatchList.GetCount();i++)
		{
			delete MatchList[i];
		}
	}

	void Build()
	{
		for(VInt i=0;i<ResultList.GetCount();i++)
		{
			ExpMatch* Match=new ExpMatch;
			Match->List=&ResultList;
			Match->Index=i;
			MatchList.Add(Match);
		}
	}

	VInt GetCount()
	{
		return MatchList.GetCount();
	}

	GeneralMatch* GetItem(VInt Index)
	{
		return MatchList[Index];
	}
};

class ExpRegex : public GeneralRegex
{
public:
	VL_RegExp*			RegEngine;

	ExpRegex(VUnicodeString Regex , VBool Greed)
	{
		RegEngine=new VL_RegExp(Regex,Greed);
	}

	~ExpRegex()
	{
		delete RegEngine;
	}

	GeneralMatchList* Match(VUnicodeString Text)
	{
		ExpMatchList* List=new ExpMatchList;
		List->ResultList.Add(RegEngine->Match(Text));
		List->Build();
		return List;
	}

	GeneralMatchList* MatchHead(VUnicodeString Text)
	{
		ExpMatchList* List=new ExpMatchList;
		List->ResultList.Add(RegEngine->MatchHead(Text));
		List->Build();
		return List;
	}

	GeneralMatchList* MatchWhole(VUnicodeString Text)
	{
		ExpMatchList* List=new ExpMatchList;
		List->ResultList.Add(RegEngine->MatchWhole(Text));
		List->Build();
		return List;
	}

	GeneralMatchList* Search(VUnicodeString Text , VBool Multiline)
	{
		ExpMatchList* List=new ExpMatchList;
		RegEngine->Search(Text,Multiline,List->ResultList);
		List->Build();
		return List;
	}

	GeneralMatchList* Split(VUnicodeString Text , VBool Multiline , VBool KeepEmptyPart)
	{
		ExpMatchList* List=new ExpMatchList;
		RegEngine->Split(Text,Multiline,List->ResultList,KeepEmptyPart);
		List->Build();
		return List;
	}

	GeneralMatchList* Cut(VUnicodeString Text , VBool Multiline , VBool KeepEmptyPart)
	{
		ExpMatchList* List=new ExpMatchList;
		RegEngine->Cut(Text,Multiline,List->ResultList,KeepEmptyPart);
		List->Build();
		return List;
	}
};

/********************************************************************************
工具
********************************************************************************/

GeneralIns::GeneralIns(VUnicodeString aName , GeneralInsType aInsType , VInt aIndex , VInt aMatchIndex)
{
	Name=aName;
	InsType=aInsType;
	Index=aIndex;
	MatchIndex=aMatchIndex;
}

GeneralIns::GeneralIns()
{
}

VUnicodeString GeneralIns::GetText(GeneralMatchList* Match)
{
	switch(InsType)
	{
	case gitGetText:
		return Match->GetItem(MatchIndex)->Text();
	case gitGetCatched:
		return Match->GetItem(MatchIndex)->GetCatched(Index);
	case gitGetStorage:
		return Match->GetItem(MatchIndex)->GetStorageItem(Name,Index);
	default:
		return L"";
	}
}

GeneralRegex* CreatePureRegex(VUnicodeString Regex)
{
	return new PureRegex(Regex);
}

GeneralRegex* CreateSafeRegex(VUnicodeString Regex)
{
	return new ExpRegex(Regex,false);
}

GeneralRegex* CreateGreedRegex(VUnicodeString Regex)
{
	return new ExpRegex(Regex,true);
}