#include "..\..\..\Library\Windows\VL_WinGUI.h"
#include "GeneralRegex.h"
#include "RegexHelper.h"

using namespace vl;
using namespace vl::windows;
using namespace vl::windows::placement;
using namespace vl::grammar;

/*窗口标题*/
PCWChar WindowTitle=L"Vczh Regular Expression Debugger [GUI Framework Preview Demo]";

/*主窗口*/
class RegForm : public VL_WinForm
{
protected:
	enum RegexStatus
	{
		rsAnalysing,
		rsModified,
		rsCorrect,
		rsWrong,
		rsSearching
	};

	/*控件*/
	VL_WinEdit*						txtRegex;
	VL_WinComboBox*					cbRegex;
	VL_WinStatic*					lbErrorMessage;
	VL_WinButton*					btnGo;
	VL_WinTreeView*					tvSyntaxTree;
	VL_WinTab*						tabDebugger;
	VL_WinStatus*					stStatus;
	/*信息页控件*/
	VL_WinStatic*					lbInfo;
	VL_WinComboBox*					cbInfo;
	VL_WinEdit*						txtInfo;
	/*匹配页控件*/
	VL_WinComboBox*					cbMatchMethod;
	VL_WinCheck*					chkMultiline;
	VL_WinCheck*					chkKeepEmptyPart;
	VL_WinButton*					btnMatch;
	VL_WinEdit*						txtMatchInput;
	VL_WinEdit*						txtMatchOutput;
	VL_WinTreeView*					tvMatchStructure;
	/*正则表达式数据*/
	VUnicodeString					InfoSyntaxTree;
	VUnicodeString					InfoEpsilonNFA;
	VUnicodeString					InfoNFA;
	VUnicodeString					InfoDFA;
	VL_RegExpResult					RegexResult;
	RegexStatus						Status;
	VL_AutoPtr<GeneralRegex>		RegexEngine;
	VL_AutoPtr<GeneralMatchList>	RegexMatchList;
	VL_List<GeneralIns , false>		RegexIns;

	/*设置正在执行任务信息*/
	void SetRunningTaskText(VUnicodeString Text)
	{
		stStatus->GetItem(0).SetTextLeft(Text);
	}

	/*强制设置状态*/
	void SetStatus(RegexStatus aStatus)
	{
		switch(Status=aStatus)
		{
		case rsAnalysing:
			stStatus->GetItem(1).SetTextLeft(L"分析");
			break;
		case rsModified:
			stStatus->GetItem(1).SetTextLeft(L"修改");
			break;
		case rsCorrect:
			stStatus->GetItem(1).SetTextLeft(L"正确");
			break;
		case rsWrong:
			stStatus->GetItem(1).SetTextLeft(L"错误");
			break;
		case rsSearching:
			stStatus->GetItem(1).SetTextLeft(L"搜索");
			break;
		}
		SetRunningTaskText(L"就绪");
	}

	/*设置状态*/
	void ChangeStatus(RegexStatus aStatus)
	{
		if(Status!=aStatus)
		{
			SetStatus(aStatus);
		}
	}

	/*设置信息页文本框内容*/
	void ResetInfoPageContent()
	{
		switch(cbInfo->GetSelectedIndex())
		{
		case 0:
			{
				VL_WinTreeViewItem Item=tvSyntaxTree->GetCurrentSelectedItem();
				if(Item)
				{
					txtInfo->SetText(GetNodeInfo(((VL_RegExpBase*)Item.GetCustomData())));
				}
				else
				{
					txtInfo->SetText(L"没有选中的节点");
				}
			}
			break;
		case 1:
			txtInfo->SetText(InfoSyntaxTree);
			break;
		case 2:
			txtInfo->SetText(InfoEpsilonNFA);
			break;
		case 3:
			txtInfo->SetText(InfoNFA);
			break;
		case 4:
			txtInfo->SetText(InfoDFA);
			break;
		}
	}

	/*构造正则表达式状态机以及字符串描述*/
	void AnalysisRegexResult()
	{
		/*检查表达式语法错误，如果没有发现错误则计算语法树*/
		if(!RegexResult.Error)
		{
			GetRegexDiscription(RegexResult,InfoSyntaxTree,InfoEpsilonNFA,InfoNFA,InfoDFA);
		}
		else
		{
			InfoSyntaxTree=L"正则表达式语法错误。";
			InfoEpsilonNFA=L"正则表达式语法错误。";
			InfoNFA=L"正则表达式语法错误。";
			InfoDFA=L"正则表达式语法错误。";
		}
	}

	/*显示正则表达式语法树*/
	void BuildSyntaxTree()
	{
		FillTreeView(RegexResult.Expression.Object(),tvSyntaxTree);
	}

	/*显示错误信息*/
	void ResetErrorStatus(VL_RegExpResult& Result)
	{
		btnMatch->SetEnabled(!Result.Error);
		if(Result.Error)
		{
			lbErrorMessage->SetText(L"发现错误："+Result.ErrorMessage);
		}
		else
		{
			lbErrorMessage->SetText(L"正确的正则表达式。");
		}
	}

	/*构造正则表达式引擎*/
	void BuildRegexEngine()
	{
		switch(cbRegex->GetSelectedIndex())
		{
		case 0:
			RegexEngine=CreatePureRegex(txtRegex->GetText());
			break;
		case 1:
			RegexEngine=CreateSafeRegex(txtRegex->GetText());
			break;
		case 2:
			RegexEngine=CreateGreedRegex(txtRegex->GetText());
			break;
		}
	}

	/*分析文本框中的正则表达式*/
	void AnalysisRegex()
	{
		ChangeStatus(rsAnalysing);
		RegexEngine=0;
		SetRunningTaskText(L"正在分析正则表达式语法...");
		tvSyntaxTree->Clear();
		VUnicodeString Text=txtRegex->GetText();
		RegexResult=RegularExpressionAnalysis(Text,cbRegex->GetSelectedIndex()!=0);
		ResetErrorStatus(RegexResult);
		if(RegexResult.Error)
		{
			if(RegexResult.ErrorPosition!=-1)
			{
				txtRegex->Select(RegexResult.ErrorPosition,Text.Length()-RegexResult.ErrorPosition);
			}
			AnalysisRegexResult();
			ShowMessage(this,RegexResult.ErrorMessage);
			txtRegex->SetFocused();
			ChangeStatus(rsWrong);
		}
		else
		{
			SetRunningTaskText(L"正在生成正则表达式详细信息...");
			BuildSyntaxTree();
			AnalysisRegexResult();
			BuildRegexEngine();
			ChangeStatus(rsCorrect);
		}
		ResetInfoPageContent();
	}

	/*控制匹配复选按钮的可用性*/
	void SetCheckBoxEnabled()
	{
		chkMultiline->SetEnabled(cbMatchMethod->GetSelectedIndex()>=3 && cbRegex->GetSelectedIndex()!=0);
		chkKeepEmptyPart->SetEnabled(cbMatchMethod->GetSelectedIndex()>=4);
	}

	/*将状态转换为修改并对正则表达式做语法检查*/
	void RegexModified()
	{
		ChangeStatus(rsModified);
		VL_RegExpResult Result=RegularExpressionAnalysis(txtRegex->GetText(),cbRegex->GetSelectedIndex()!=0);
		ResetErrorStatus(Result);
	}

	/*初始化状态*/
	void InitStatus()
	{
		SetStatus(rsModified);
		/*设置示例表达式*/
		txtRegex->SetText(L"(?:<sec>(?:<#part>25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]\\d|[0-9]))(.(?:<sec>)){3}");
		AnalysisRegex();
		txtRegex->SelectAll();
		txtRegex->SetFocused();
		/*设置控件内容*/
		SetCheckBoxEnabled();
		ResetInfoPageContent();
	}

	/*控件初始化函数*/
	void InitControls()
	{
		/*初始化控件*/
		txtRegex=new VL_WinEdit(this,false);
		cbRegex=new VL_WinComboBox(this,true);
		cbRegex->AddString(L"纯正则表达式");
		cbRegex->AddString(L"扩展正则表达式");
		cbRegex->AddString(L"贪婪正则表达式");
		cbRegex->SetSelectedIndex(2);
		lbErrorMessage=new VL_WinStatic(this);
		btnGo=new VL_WinButton(this);
		btnGo->SetText(L"分析");
		tvSyntaxTree=new VL_WinTreeView(this);
		tabDebugger=new VL_WinTab(this);
		{
			/*初始化信息页*/
			VL_WinTabPage Page=tabDebugger->AddPage(L"信息");
			lbInfo=new VL_WinStatic(Page);
			lbInfo->SetText(L"信息内容：");
			cbInfo=new VL_WinComboBox(Page,true);
			cbInfo->AddString(L"节点详细信息");
			cbInfo->AddString(L"语法树");
			cbInfo->AddString(L"ε-NFA");
			cbInfo->AddString(L"NFA");
			cbInfo->AddString(L"DFA");
			cbInfo->SetSelectedIndex(0);
			txtInfo=new VL_WinEdit(Page,true);
			txtInfo->SetHScroll(true);
			txtInfo->SetVScroll(true);
			txtInfo->SetReadonly(true);
			/*设置信息页空间排版*/
			Page.ApplyPlacement(
				pHorzFix1(10,10,
					pVertFix1(0,10,
						pControl(lbInfo,60,20),
						pVertFix1(0,10,
							pControl(cbInfo,120,20),
							pBlank(0,0)
						)
					),
					pControl(txtInfo)
				)
			);
		}
		{
			VL_WinTabPage Page=tabDebugger->AddPage(L"匹配");
			cbMatchMethod=new VL_WinComboBox(Page,true);
			cbMatchMethod->AddString(L"匹配第一处");
			cbMatchMethod->AddString(L"匹配字符串开头");
			cbMatchMethod->AddString(L"匹配整个字符串");
			cbMatchMethod->AddString(L"搜索所有匹配");
			cbMatchMethod->AddString(L"分离字符串");
			cbMatchMethod->AddString(L"切割字符串");
			cbMatchMethod->SetSelectedIndex(3);
			chkMultiline=new VL_WinCheck(Page);
			chkMultiline->SetText(L"多行匹配");
			chkKeepEmptyPart=new VL_WinCheck(Page);
			chkKeepEmptyPart->SetText(L"保留空白切割");
			btnMatch=new VL_WinButton(Page);
			btnMatch->SetText(L"匹配");
			txtMatchInput=new VL_WinEdit(Page,true,true);
			txtMatchInput->SetHScroll(true);
			txtMatchInput->SetVScroll(true);
			txtMatchInput->SetText(L"在这里输入被匹配的字符串。");
			txtMatchOutput=new VL_WinEdit(Page,true);
			txtMatchOutput->SetHScroll(true);
			txtMatchOutput->SetVScroll(true);
			txtMatchOutput->SetReadonly(true);
			tvMatchStructure=new VL_WinTreeView(Page);
			Page.ApplyPlacement(
				pHorzFix1(10,10,
					pVertFix1(0,10,
						pVertFix1(0,10,
							pVertFix1(0,10,
								pControl(chkMultiline,70,0),
								pControl(chkKeepEmptyPart,100,0)
							),
							pVertFix1(0,10,
								pControl(cbMatchMethod,100,21),
								pControl(btnMatch,40,0)
							)
						),
						pBlank(0,0)
					),
					pHorzScale(0,10,0.4,
						pControl(txtMatchInput),
						pVertScale(0,10,0.4,
							pControl(tvMatchStructure),
							pControl(txtMatchOutput)
						)
					)
				)
			);
		}
		stStatus=new VL_WinStatus(this);
		stStatus->AddItem().SetWidth(200);
		stStatus->AddItem().SetWidth(50);
		stStatus->AddItem().SetTextRight(L"开发者：陈梓瀚(vczh) http://www.cppblog.com/vczh   ");
		/*设置控件排版，在窗口尺寸修改的时候自动调整控件尺寸*/
		ApplyPlacement(
			pHorzFix2(0,0,
				pHorzFix1(10,10,
					pVertFix1(0,10,
						pControl(cbRegex,120,21),
						pVertFix2(0,10,
							pControl(txtRegex),
							pControl(btnGo,40,0)
						)
					),
					pHorzFix1(0,10,
						pControl(lbErrorMessage,0,20),
						pVertFix1(0,10,
							pControl(tvSyntaxTree,300,300),
							pControl(tabDebugger,400,300)
						)
					)
				),
				pControl(stStatus,0,20)
			)
		);
		/*设置控件事件*/
		cbRegex->OnSelChanged.Bind(this,&RegForm::cbRegex_OnSelChanged);
		txtRegex->OnChanged.Bind(this,&RegForm::txtRegex_OnChanged);
		txtRegex->OnKeyDown.Bind(this,&RegForm::txtRegex_OnKeyDown);
		btnGo->OnClick.Bind(this,&RegForm::btnGo_OnClick);
		tvSyntaxTree->OnItemSelected.Bind(this,&RegForm::tvSyntaxTree_OnItemSelected);
		cbInfo->OnSelChanged.Bind(this,&RegForm::cbInfo_OnSelChanged);
		cbMatchMethod->OnSelChanged.Bind(this,&RegForm::cbMatchMethod_OnSelChanged);
		btnMatch->OnClick.Bind(this,&RegForm::btnMatch_OnClick);
		tvMatchStructure->OnItemSelected.Bind(this,&RegForm::tvMatchStructure_OnItemSelected);
	}

	/*控件事件*/

	void cbRegex_OnSelChanged(VL_Base* Sender)
	{
		SetCheckBoxEnabled();
		RegexModified();
	}

	void txtRegex_OnChanged(VL_Base* Sender)
	{
		RegexModified();
	}

	void txtRegex_OnKeyDown(VL_Base* Sender,VLS_KeyStruct KeyStruct)
	{
		if(KeyStruct.KeyCode==VK_RETURN)
		{
			AnalysisRegex();
		}
	}

	void btnGo_OnClick(VL_Base* Sender)
	{
		AnalysisRegex();
	}

	void tvSyntaxTree_OnItemSelected(VL_Base* Sender , VL_WinTreeViewItem Item)
	{
		if(cbInfo->GetSelectedIndex()==0)
		{
			ResetInfoPageContent();
		}
	}

	void cbInfo_OnSelChanged(VL_Base* Sender)
	{
		ResetInfoPageContent();
	}

	void cbMatchMethod_OnSelChanged(VL_Base* Sender)
	{
		SetCheckBoxEnabled();
	}

	void btnMatch_OnClick(VL_Base* Sender)
	{
		RegexStatus aStatus=Status;
		ChangeStatus(rsSearching);
		SetRunningTaskText(L"正在匹配...");
		if(Status!=rsCorrect)
		{
			BuildRegexEngine();
		}
		switch(cbMatchMethod->GetSelectedIndex())
		{
		case 0:
			RegexMatchList=RegexEngine->Match(txtMatchInput->GetText());
			break;
		case 1:
			RegexMatchList=RegexEngine->MatchHead(txtMatchInput->GetText());
			break;
		case 2:
			RegexMatchList=RegexEngine->MatchWhole(txtMatchInput->GetText());
			break;
		case 3:
			RegexMatchList=RegexEngine->Search(txtMatchInput->GetText(),chkMultiline->GetChecked());
			break;
		case 4:
			RegexMatchList=RegexEngine->Split(txtMatchInput->GetText(),chkMultiline->GetChecked(),chkKeepEmptyPart->GetChecked());
			break;
		case 5:
			RegexMatchList=RegexEngine->Cut(txtMatchInput->GetText(),chkMultiline->GetChecked(),chkKeepEmptyPart->GetChecked());
			break;
		}
		SetRunningTaskText(L"正在生成匹配结果...");
		tvMatchStructure->Clear();
		if(RegexMatchList->GetCount())
		{
			for(VInt i=0;i<RegexMatchList->GetCount();i++)
			{
				VUnicodeString Text;
				GeneralMatch* Match=RegexMatchList->GetItem(i);
				if(Match->IsSuccess())
				{
					Text=L"匹配["+VUnicodeString(i)+L"]："+Match->Text();
				}
				else
				{
					Text=L"文本["+VUnicodeString(i)+L"]："+Match->Text();
				}
				VL_WinTreeViewItem _Match=tvMatchStructure->AddRootItem(Text);
				_Match.SetCustomData((VPointer)RegexIns.GetCount());
				RegexIns.Add(GeneralIns(L"",gitGetText,0,i));
				if(Match->GetCatchedCount())
				{
					VL_WinTreeViewItem _Catch=_Match.InsertAtLast(L"匿名捕获");
					_Catch.SetCustomData((VPointer)-1);
					for(VInt j=0;j<Match->GetCatchedCount();j++)
					{
						VL_WinTreeViewItem _CatchItem=_Catch.InsertAtLast(Match->GetCatched(j));
						_CatchItem.SetCustomData((VPointer)RegexIns.GetCount());
						RegexIns.Add(GeneralIns(L"",gitGetCatched,j,i));
					}
				}
				VInt Count=Match->GetStorageCount();
				if(Count)
				{
					for(VInt k=0;k<Count;k++)
					{
						VUnicodeString Name=Match->GetStorageName(k);
						VL_WinTreeViewItem _Catch=_Match.InsertAtLast(L"命名捕获["+Name+L"]");
						_Catch.SetCustomData((VPointer)-1);
						for(VInt j=0;j<Match->GetStorageItemCount(Name);j++)
						{
							VL_WinTreeViewItem _CatchItem=_Catch.InsertAtLast(Match->GetStorageItem(Name,j));
							_CatchItem.SetCustomData((VPointer)RegexIns.GetCount());
							RegexIns.Add(GeneralIns(Name,gitGetStorage,j,i));
						}
					}
				}
			}
		}
		else
		{
			tvMatchStructure->AddRootItem(L"找不到任何满足要求的匹配").SetCustomData((VPointer)-1);
		}
		SetRunningTaskText(L"就绪");
		ChangeStatus(aStatus);
	}

	void tvMatchStructure_OnItemSelected(VL_Base* Sender , VL_WinTreeViewItem Item)
	{
		if(Status!=rsSearching)
		{
			VInt Index=(VInt)Item.GetCustomData();
			if(Index!=-1)
			{
				txtMatchOutput->SetText(RegexIns[Index].GetText(RegexMatchList.Object()));
				GeneralMatch* Match=RegexMatchList->GetItem(RegexIns[Index].MatchIndex);
				VInt Line=Match->Line();
				VInt Start=txtMatchInput->GetCharIndex(Line)+Match->Position();
				txtMatchInput->Select(Start,Match->Text().Length());
				txtMatchInput->ScrollToCaret();
			}
		}
	}

public:
	RegForm():VL_WinForm(true)
	{
		SetText(WindowTitle);
		InitControls();
		InitStatus();
		MoveCenter();
	}
};

void main()
{
	(new RegForm())->ShowMaximized();
	GetApplication()->Run();
}