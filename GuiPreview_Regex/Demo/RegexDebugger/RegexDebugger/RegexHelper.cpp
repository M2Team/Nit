#include "RegexHelper.h"

/*将正则表达式语法树节点转换为字符串信息的算法*/
class SyntaxTreeInfoProvider : public IVL_RegExpRecursiveAlgorithm
{
public:
	VUnicodeString		Text;
	VUnicodeString		Information;

	void VisitBegin(VL_RegExpCharset* Node , VBool& VisitChildren , VBool& RunEnd)
	{
		Text=L"";
		Information=L"";
		switch(Node->Type)
		{
		case vrecCharSet:
			Text=(Node->Reverse?L"反转：":L"")+CharSetToString(Node->CharSet);
			break;
		case vrecStringBegin:
			Text=L"字符串开头";
			break;
		case vrecStringEnd:
			Text=L"字符串结尾";
			break;
		}
		Information+=L"节点类型：字符串匹配\r\n";
		Information+=L"匹配目标："+Text+L"\r\n";
		VisitChildren=false;
		RunEnd=false;
	}

	void VisitBegin(VL_RegExpLoop* Node , VBool& VisitChildren , VBool& RunEnd)
	{
		Text=L"";
		Information=L"";
		if(Node->Finite)
		{
			Text=L"{"+VUnicodeString(Node->Begin)+L","+VUnicodeString(Node->End)+L"}";
		}
		else
		{
			Text=L"{"+VUnicodeString(Node->Begin)+L",}";
		}
		if(Node->Short)
		{
			Text+=L"?";
		}
		Information+=L"节点类型：循环\r\n";
		Information+=L"最小循环次数："+VUnicodeString(Node->Begin)+L"\r\n";
		Information+=L"最大循环次数："+VUnicodeString(Node->End)+L"\r\n";
		Information+=L"无限循环："+VUnicodeString(Node->Finite?L"否":L"是")+L"\r\n";
		Information+=L"长优先："+VUnicodeString(Node->Short?L"否":L"是")+L"\r\n";
		VisitChildren=false;
		RunEnd=false;
	}

	void VisitBegin(VL_RegExpFunc* Node , VBool& VisitChildren , VBool& RunEnd)
	{
		Text=L"";
		Information=L"";
		Information+=L"节点类型：功能节点\r\n";
		switch(Node->Function)
		{
		case vrefCatch:
			Text+=L"[获取]";
			Information+=L"功能：获取\r\n";
			break;
		case vrefUncatch:
			Text+=L"[不获取]";
			Information+=L"功能：不获取\r\n";
			break;
		case vrefPositive:
			Text+=L"[正向预查]";
			Information+=L"功能：正向预查\r\n";
			break;
		case vrefNegative:
			Text+=L"[反向预查]";
			Information+=L"功能：反向预查\r\n";
			break;
		}
		switch(Node->Content)
		{
		case vrecNamed:
			Text+=L"[命名表达式]";
			Information+=L"内容：命名表达式\r\n";
			break;
		case vrecStorage:
			Text+=L"[指定管道存储]";
			Information+=L"内容：指定管道存储\r\n";
			break;
		case vrecMatch:
			Text+=L"[指定管道匹配]";
			Information+=L"内容：指定管道匹配\r\n";
			break;
		case vrecNormal:
			Text+=L"[普通]";
			Information+=L"内容：普通\r\n";
			break;
		}
		Text+=L"["+Node->Pattern+L"]";
		Information+=L"模式名称："+Node->Pattern+L"\r\n";
		VisitChildren=false;
		RunEnd=false;
	}

	void VisitBegin(VL_RegExpSection* Node , VBool& VisitChildren , VBool& RunEnd)
	{
		Text=L"";
		Information=L"";
		switch(Node->Type)
		{
		case vresSequence:
			Text=L"序列";
			break;
		case vresBranch:
			Text=L"分支";
			break;
		}
		Information+=L"节点类型："+Text+L"\r\n";
		VisitChildren=false;
		RunEnd=false;
	}

	void VisitEnd(VL_RegExpCharset* Node)
	{
	}

	void VisitEnd(VL_RegExpLoop* Node)
	{
	}

	void VisitEnd(VL_RegExpFunc* Node)
	{
	}

	void VisitEnd(VL_RegExpSection* Node)
	{
	}
};

/*将正则表达式语法树转换为TreeView的算法*/
class SyntaxTreeBuilder : public IVL_RegExpRecursiveAlgorithm
{
public:
	VL_WinTreeView*							TreeView;
	VL_List<VL_WinTreeViewItem , false>		ItemStack;

	SyntaxTreeBuilder(VL_WinTreeView* aTreeView)
	{
		TreeView=aTreeView;
	}

	void MakeNode(VL_RegExpBase* Node)
	{
		VL_WinTreeViewItem Item;
		if(ItemStack.GetCount())
		{
			Item=ItemStack[ItemStack.GetCount()-1].InsertAtLast(GetNodeText(Node));
		}
		else
		{
			Item=TreeView->AddRootItem(GetNodeText(Node));
		}
		Item.SetCustomData(Node);
		ItemStack.Add(Item);
	}

	void VisitBegin(VL_RegExpCharset* Node , VBool& VisitChildren , VBool& RunEnd)
	{
		MakeNode(Node);
	}

	void VisitBegin(VL_RegExpLoop* Node , VBool& VisitChildren , VBool& RunEnd)
	{
		MakeNode(Node);
	}

	void VisitBegin(VL_RegExpFunc* Node , VBool& VisitChildren , VBool& RunEnd)
	{
		MakeNode(Node);
	}

	void VisitBegin(VL_RegExpSection* Node , VBool& VisitChildren , VBool& RunEnd)
	{
		MakeNode(Node);
	}

	void VisitEnd(VL_RegExpCharset* Node)
	{
		ItemStack.Fetch().Expand();
	}

	void VisitEnd(VL_RegExpLoop* Node)
	{
		ItemStack.Fetch().Expand();
	}

	void VisitEnd(VL_RegExpFunc* Node)
	{
		ItemStack.Fetch().Expand();
	}

	void VisitEnd(VL_RegExpSection* Node)
	{
		ItemStack.Fetch().Expand();
	}
};

VUnicodeString GetNodeText(VL_RegExpBase* Node)
{
	SyntaxTreeInfoProvider Algorithm;
	Node->Visit(&Algorithm);
	return Algorithm.Text;
}

VUnicodeString GetNodeInfo(VL_RegExpBase* Node)
{
	SyntaxTreeInfoProvider Algorithm;
	Node->Visit(&Algorithm);
	return Algorithm.Information;
}

void FillTreeView(VL_RegExpBase* Node , VL_WinTreeView* TreeView)
{
	SyntaxTreeBuilder Algorithm(TreeView);
	Node->Visit(&Algorithm);
}

void GetRegexDiscription(VL_RegExpResult& RegexResult , VUnicodeString& InfoSyntaxTree , VUnicodeString& InfoEpsilonNFA , VUnicodeString& InfoNFA , VUnicodeString& InfoDFA)
{
	InfoSyntaxTree=L"";
	InfoEpsilonNFA=L"";
	InfoNFA=L"";
	InfoDFA=L"";
	/*得到语法树的字符串描述*/
	InfoSyntaxTree=RegularExpressionToString(RegexResult.Expression);
	/*创建自动机*/
	VL_RegNFA						NFAAutomaton;
	VL_RegDFA						DFAAutomaton;
	VL_AutoPtr<VL_RegFAInfo>		FAInfo=new VL_RegFAInfo(RegexResult.Expression);
	/*计算epsilon-NFA*/
	VL_RegFAInfo::EpsilonNFABag		eNFA=RegularExpressionToEpsilonNFA(&NFAAutomaton,FAInfo.Object());
	InfoEpsilonNFA+=L"--------【主状态机】--------\r\n";
	InfoEpsilonNFA+=NFAToString(FAInfo.Object(),eNFA.MainEpsilonNFA.InitStatus);
	for(VInt i=0;i<eNFA.SubEpsilonNFAs.GetCount();i++)
	{
		InfoEpsilonNFA+=L"--------【附状态机"+VUnicodeString(i)+L"】--------\r\n";
		InfoEpsilonNFA+=NFAToString(FAInfo.Object(),eNFA.SubEpsilonNFAs[i].InitStatus);
	}
	/*计算NFA*/
	FAInfo->MainNFA=CompressNFA(&NFAAutomaton,EpsilonNFAToNFA(&NFAAutomaton,eNFA.MainEpsilonNFA));
	for(VInt i=0;i<eNFA.SubEpsilonNFAs.GetCount();i++)
	{
		FAInfo->SubNFAs.Add(CompressNFA(&NFAAutomaton,EpsilonNFAToNFA(&NFAAutomaton,eNFA.SubEpsilonNFAs[i])));
	}
	InfoNFA+=L"--------【主状态机】--------\r\n";
	InfoNFA+=NFAToString(FAInfo.Object(),FAInfo->MainNFA);
	for(VInt i=0;i<FAInfo->SubNFAs.GetCount();i++)
	{
		InfoNFA+=L"--------【附状态机"+VUnicodeString(i)+L"】--------\r\n";
		InfoNFA+=NFAToString(FAInfo.Object(),FAInfo->SubNFAs[i]);
	}
	/*计算DFA*/
	InfoDFA+=L"--------【主状态机】--------\r\n";
	InfoDFA+=DFAToString(FAInfo.Object(),CompressedNFAToDFA(&DFAAutomaton,FAInfo->MainNFA));
	for(VInt i=0;i<FAInfo->SubNFAs.GetCount();i++)
	{
		InfoDFA+=L"--------【附状态机"+VUnicodeString(i)+L"】--------\r\n";
		InfoDFA+=DFAToString(FAInfo.Object(),CompressedNFAToDFA(&DFAAutomaton,FAInfo->SubNFAs[i]));
	}
}