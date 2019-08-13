#ifndef GENERALREGEX
#define GENERALREGEX

#include "..\..\..\Library\Data\Grammar2\VL_RegTools.h"

using namespace vl;
using namespace vl::grammar;

class GeneralMatch : public IVL_Interface
{
public:
	virtual VBool				IsSuccess()=0;
	virtual VInt				Position()=0;
	virtual VInt				Line()=0;
	virtual VUnicodeString		Text()=0;
	virtual VInt				GetCatchedCount()=0;
	virtual VUnicodeString		GetCatched(VInt Index)=0;
	virtual VInt				GetStorageCount()=0;
	virtual VUnicodeString		GetStorageName(VInt Index)=0;
	virtual VInt				GetStorageItemCount(VUnicodeString Name)=0;
	virtual VUnicodeString		GetStorageItem(VUnicodeString Name , VInt Index)=0;
};

class GeneralMatchList : public IVL_Interface
{
public:
	virtual VInt				GetCount()=0;
	virtual GeneralMatch*		GetItem(VInt Index)=0;
};

class GeneralRegex : public IVL_Interface
{
public:
	virtual GeneralMatchList*	Match(VUnicodeString Text)=0;
	virtual GeneralMatchList*	MatchHead(VUnicodeString Text)=0;
	virtual GeneralMatchList*	MatchWhole(VUnicodeString Text)=0;
	virtual GeneralMatchList*	Search(VUnicodeString Text , VBool Multiline)=0;
	virtual GeneralMatchList*	Split(VUnicodeString Text , VBool Multiline , VBool KeepEmptyPart)=0;
	virtual GeneralMatchList*	Cut(VUnicodeString Text , VBool Multiline , VBool KeepEmptyPart)=0;
};

enum GeneralInsType
{
	gitGetText,
	gitGetCatched,
	gitGetStorage
};

class GeneralIns : public VL_Base
{
public:
	VUnicodeString				Name;
	GeneralInsType				InsType;
	VInt						Index;
	VInt						MatchIndex;

	GeneralIns(VUnicodeString aName , GeneralInsType aInsType , VInt aIndex , VInt aMatchIndex);
	GeneralIns();
	
	VUnicodeString				GetText(GeneralMatchList* Match);
};

extern GeneralRegex* CreatePureRegex(VUnicodeString Regex);
extern GeneralRegex* CreateSafeRegex(VUnicodeString Regex);
extern GeneralRegex* CreateGreedRegex(VUnicodeString Regex);

#endif