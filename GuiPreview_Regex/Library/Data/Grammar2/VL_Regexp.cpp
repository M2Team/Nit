#include "VL_Regexp.h"

#include "..\Data\VL_Data_Map.h"

namespace vl
{
	namespace grammar
	{

/*********************************************************************************************************
VL_RegExpBase
*********************************************************************************************************/

	VL_RegExpBase::VL_RegExpBase()
	{
	}

/*********************************************************************************************************
VL_RegExpCharset
*********************************************************************************************************/

	VL_RegExpCharset::VL_RegExpCharset()
	{
		Type=vrecCharSet;
		Reverse=false;
	}

	void VL_RegExpCharset::Visit(IVL_RegExpRecursiveAlgorithm* Algorithm)
	{
		VBool VisitChildren=true;
		VBool RunEnd=true;
		Algorithm->VisitBegin(this,VisitChildren,RunEnd);
		if(RunEnd)
		{
			Algorithm->VisitEnd(this);
		}
	}

/*********************************************************************************************************
VL_RegExpLoop
*********************************************************************************************************/

	VL_RegExpLoop::VL_RegExpLoop()
	{
		Begin=0;
		End=0;
		Finite=true;
		Short=false;
	}

	void VL_RegExpLoop::Visit(IVL_RegExpRecursiveAlgorithm* Algorithm)
	{
		VBool VisitChildren=true;
		VBool RunEnd=true;
		Algorithm->VisitBegin(this,VisitChildren,RunEnd);
		if(VisitChildren && Expression)
		{
			Expression->Visit(Algorithm);
		}
		if(RunEnd)
		{
			Algorithm->VisitEnd(this);
		}
	}

/*********************************************************************************************************
VL_RegExpFunc
*********************************************************************************************************/

	VL_RegExpFunc::VL_RegExpFunc()
	{
		Function=vrefUncatch;
		Content=vrecNormal;
		ReferenceSource=0;
	}

	void VL_RegExpFunc::Visit(IVL_RegExpRecursiveAlgorithm* Algorithm)
	{
		VBool VisitChildren=true;
		VBool RunEnd=true;
		Algorithm->VisitBegin(this,VisitChildren,RunEnd);
		if(VisitChildren && Expression)
		{
			Expression->Visit(Algorithm);
		}
		if(RunEnd)
		{
			Algorithm->VisitEnd(this);
		}
	}

/*********************************************************************************************************
VL_RegExpSection
*********************************************************************************************************/

	VL_RegExpSection::VL_RegExpSection()
	{
		Type=vresSequence;
	}

	void VL_RegExpSection::Visit(IVL_RegExpRecursiveAlgorithm* Algorithm)
	{
		VBool VisitChildren=true;
		VBool RunEnd=true;
		Algorithm->VisitBegin(this,VisitChildren,RunEnd);
		if(VisitChildren)
		{
			for(VInt i=0;i<Sections.GetCount();i++)
			{
				if(Sections[i])
				{
					Sections[i]->Visit(Algorithm);
				}
			}
		}
		if(RunEnd)
		{
			Algorithm->VisitEnd(this);
		}
	}

/*********************************************************************************************************
VL_RegExpResult
*********************************************************************************************************/

	VL_RegExpResult::VL_RegExpResult()
	{
		Error=false;
		ErrorPosition=-1;
	}

/*********************************************************************************************************
CharRangeToString
*********************************************************************************************************/

	VUnicodeString CharRangeToString(VL_RegCharRange Range)
	{
		if(Range.Key==Range.Value)
		{
			return	L"【"+VUnicodeString(Range.Key)+L","+VUnicodeString(VInt(Range.Key))+L"】";
		}
		else
		{
			return	L"【"+VUnicodeString(Range.Key)+L","+VUnicodeString(VInt(Range.Key))+
					L"】-【"+VUnicodeString(Range.Value)+L","+VUnicodeString(VInt(Range.Value))+L"】";
		}
	}

/*********************************************************************************************************
CharSetToString
*********************************************************************************************************/

	VUnicodeString CharSetToString(VL_RegCharSet& CharSet)
	{
		VUnicodeString Result;
		for(VInt i=0;i<CharSet.GetCount();i++)
		{
			VL_RegCharRange Range=CharSet[i];
			if(i)
			{
				Result+=L",";
			}
			Result+=CharRangeToString(Range);
		}
		return Result;
	}

/*********************************************************************************************************
RegularExpressionAnalysis
*********************************************************************************************************/

	class RegexpError
	{
	public:
		VInt			Position;
		VUnicodeString	Message;
	};

	enum RegexpCharType
	{
		rctCharSet,
		rctStringBegin,
		rctStringEnd,
		rctAny,
		rctSome,
		rctOption
	};

	VWChar HexAnalysis(PWChar Expression)
	{
		VWChar Result=0;
		for(VInt i=0;i<4;i++)
		{
			VWChar Current=*Expression++;
			if(Current>=L'0' && Current<=L'9')
			{
				Result=Result*16+(Current-L'0');
			}
			else if(Current>=L'A' && Current<=L'F')
			{
				Result=Result*16+(Current-L'F'+10);
			}
			else
			{
				return 0;
			}
		}
		return Result;
	}

	VWChar SingleTransformAnalysis(PWChar Expression , VInt Index , VInt& Length)
	{
		Length=1;
		switch(Expression[Index])
		{
		case L'{':case L'}':case L'[':case L']':case L'(':case L')':case L'\\':case L'^':
		case L'?':case L'+':case L'-':case L'*':case L'|':case L'<':case L'>':case L'$':
			return Expression[Index];
		case L'x':
			{
				VWChar Char=HexAnalysis(&Expression[Index+1]);
				if(Char)
				{
					Length=5;
					return Char;
				}
				else
				{
					RegexpError Error;
					Error.Position=Index;
					Error.Message=L"错误的16进制转义字符。";
					throw Error;
				}
			}
			break;
		case L'r':
			Length=1;
			return L'\r';
			break;
		case L'n':
			Length=1;
			return L'\n';
			break;
		case L't':
			Length=1;
			return L'\t';
			break;
		case L'\0':
			{
				RegexpError Error;
				Error.Position=Index;
				Error.Message=L"正则表达式过早结束。";
				throw Error;
			}
			break;
		default:
			{
				RegexpError Error;
				Error.Position=Index;
				Error.Message=L"错误的字符集合转义字符\"\\"+VUnicodeString(Expression[Index])+L"\"。";
				throw Error;
			}
		}
	}

	void FullTransformAnalysis(PWChar Expression , VInt Index , VInt& Length , VBool& Reverse , VL_RegCharSet& CharSet)
	{
		Reverse=false;
		switch(Expression[Index])
		{
		case L'D':
			Reverse=true;
		case L'd':
			{
				VL_RegCharRange Range;
				Range.Key=L'0';
				Range.Value=L'9';
				CharSet.Add(Range);
				Length=1;
			}
			break;
		case L'L':
			Reverse=true;
		case L'l':
			{
				VL_RegCharRange Range;
				Range.Key=L'a';
				Range.Value=L'z';
				CharSet.Add(Range);
				Range.Key=L'A';
				Range.Value=L'Z';
				CharSet.Add(Range);
				Length=1;
			}
			break;
		case L'W':
			Reverse=true;
		case L'w':
			{
				VL_RegCharRange Range;
				Range.Key=L'a';
				Range.Value=L'z';
				CharSet.Add(Range);
				Range.Key=L'A';
				Range.Value=L'Z';
				CharSet.Add(Range);
				Range.Key=L'0';
				Range.Value=L'9';
				CharSet.Add(Range);
				Range.Key=L'_';
				Range.Value=L'_';
				CharSet.Add(Range);
				Length=1;
			}
			break;
		case L'S':
			Reverse=true;
		case L's':
			{
				VL_RegCharRange Range;
				Range.Key=L'\r';
				Range.Value=L'\r';
				CharSet.Add(Range);
				Range.Key=L'\n';
				Range.Value=L'\n';
				CharSet.Add(Range);
				Range.Key=L'\t';
				Range.Value=L'\t';
				CharSet.Add(Range);
				Range.Key=L' ';
				Range.Value=L' ';
				CharSet.Add(Range);
				Length=1;
			}
			break;
		case L'.':
			{
				Reverse=true;
				Length=1;
			}
			break;
		case L'{':case L'}':case L'[':case L']':case L'(':case L')':case L'\\':case L'^':
		case L'?':case L'+':case L'-':case L'*':case L'|':case L'<':case L'>':case L'$':
			{
				VL_RegCharRange Range;
				Range.Key=Expression[Index];
				Range.Value=Expression[Index];
				CharSet.Add(Range);
				Length=1;
			}
			break;
		case L'r':
			{
				VL_RegCharRange Range;
				Range.Key=L'\r';
				Range.Value=L'\r';
				CharSet.Add(Range);
				Length=1;
			}
			break;
		case L'n':
			{
				VL_RegCharRange Range;
				Range.Key=L'\n';
				Range.Value=L'\n';
				CharSet.Add(Range);
				Length=1;
			}
			break;
		case L't':
			{
				VL_RegCharRange Range;
				Range.Key=L'\t';
				Range.Value=L'\t';
				CharSet.Add(Range);
				Length=1;
			}
			break;
		case L'x':
			{
				VWChar Char=HexAnalysis(&Expression[Index+1]);
				if(Char)
				{
					VL_RegCharRange Range;
					Range.Key=Char;
					Range.Value=Char;
				CharSet.Add(Range);
					Length=5;
				}
				else
				{
					RegexpError Error;
					Error.Position=Index;
					Error.Message=L"错误的16进制转义字符。";
					throw Error;
				}
			}
			break;
		case L'\0':
			{
				RegexpError Error;
				Error.Position=Index;
				Error.Message=L"正则表达式过早结束。";
				throw Error;
			}
		default:
			{
				RegexpError Error;
				Error.Position=Index;
				Error.Message=L"错误的转义字符\"\\"+VUnicodeString(Expression[Index])+L"\"。";
				throw Error;
			}
		}
	}

	VWChar CharsetCharAnalysis(PWChar Expression , VInt Index , VInt& Length)
	{
		switch(Expression[Index])
		{
		case L'\\':
			{
				Length=0;
				VWChar Result=SingleTransformAnalysis(Expression,Index+1,Length);
				Length++;
				return Result;
			}
		case L'\0':
			{
				RegexpError Error;
				Error.Position=Index;
				Error.Message=L"正则表达式过早结束。";
				throw Error;
			}
		case L'{':case L'}':case L'[':case L']':case L'(':case L')':case L'^':case L'$':
		case L'?':case L'+':case L'-':case L'*':case L'|':case L'<':case L'>':
			{
				RegexpError Error;
				Error.Position=Index;
				Error.Message=L"错误的功能字符。";
				throw Error;
			}
		default:
			Length=1;
			return Expression[Index];
		}
	}

	VInt IntegerAnalysis(PWChar Expression , VInt Index , VInt& Length)
	{
		Length=0;
		VInt Result=0;
		while(true)
		{
			VWChar Char=Expression[Index++];
			if(Char>=L'0' && Char<=L'9')
			{
				Length++;
				Result=Result*10+(Char-L'0');
			}
			else
			{
				break;
			}
		}
		if(Length)
		{
			return Result;
		}
		else
		{
			RegexpError Error;
			Error.Position=Index;
			Error.Message=L"此处需要数字。";
			throw Error;
		}
	}

	VL_RegExpPtr InternalRegexpAnalysis(PWChar Expression , VInt StartChar , VBool AllowExtension , VWChar TerminalChar , VInt& TerminalIndex)
	{
		VL_RegExpPtr		Result;
		VL_RegExpSection*	Branch=0;
		VL_RegExpSection*	Sequence=0;
		VInt				Index=StartChar;

		/*预先建立一个序列以供存储表达式*/
		Sequence=new VL_RegExpSection;
		Sequence->Type=vresSequence;
		Result=Sequence;

		while(true)
		{
			VWChar Current=Expression[Index];
			if(Current==TerminalChar)/*遇到结束字符*/
			{
				if(Index==StartChar)/*没有产生过任何正则表达式树节点*/
				{
					RegexpError Error;
					Error.Position=Index;
					Error.Message=L"正则表达式不可为空。";
					throw Error;
				}
				else if(Sequence->Sections.GetCount()==0)/*存储表达式的列表为空*/
				{
					RegexpError Error;
					Error.Position=Index;
					Error.Message=L"正则表达式缺少部件。";
					throw Error;
				}
				else
				{
					break;
				}
			}
			else if(Current==L'\0')/*遇到字符串结束*/
			{
				RegexpError Error;
				Error.Position=Index;
				Error.Message=L"正则表达式过早结束。";
				throw Error;
			}
			else/*正常情况*/
			{
				VL_RegExpPtr CurrentExpression;
				switch(Current)
				{
				case L'|':
					if(Sequence->Sections.GetCount()==0)/*存储表达式的列表为空*/
					{
						RegexpError Error;
						Error.Position=Index;
						Error.Message=L"正则表达式缺少部件。";
						throw Error;
					}
					else
					{
						if(!Branch)/*如果Branch尚未建立*/
						{
							/*建立一个Branch，并把结果表达式树的指针指向Branch*/
							Branch=new VL_RegExpSection;
							Branch->Type=vresBranch;
							Branch->Sections.Add(Result);
							Result=Branch;
						}
						/*建立一个新的序列以供存储表达式*/
						Sequence=new VL_RegExpSection;
						Sequence->Type=vresSequence;
						Branch->Sections.Add(Sequence);
					}
					break;
				case L'(':
					{
						if(Expression[Index+1]==L'?')
						{
							VL_RegExpFunc* Function=new VL_RegExpFunc;
							CurrentExpression=Function;

							/*检查功能类型*/
							switch(Expression[Index+2])
							{
							case L':':
								Function->Function=vrefUncatch;
								Index+=2;
								break;
							case L'=':
								Function->Function=vrefPositive;
								Index+=2;
								break;
							case L'!':
								Function->Function=vrefNegative;
								Index+=2;
								break;
							default:
								Function->Function=vrefCatch;
								Index+=1;
							}
							if(!AllowExtension && Function->Function!=vrefUncatch)
							{
								RegexpError Error;
								Error.Position=Index;
								Error.Message=L"正则表达式此处不可使用扩展语法。";
								throw Error;
							}

							/*检查内容类型*/
							if(Expression[Index+1]==L'<')
							{
								switch(Expression[Index+2])
								{
								case L'#':
									if(Function->Function==vrefPositive || Function->Function==vrefNegative)
									{
										RegexpError Error;
										Error.Position=Index+2;
										Error.Message=L"正则表达式此处不可使用扩展语法。";
										throw Error;
									}
									else
									{
										Function->Content=vrecStorage;
										Index+=3;
									}
									break;
								case L'$':
									if(Function->Function==vrefPositive || Function->Function==vrefNegative)
									{
										RegexpError Error;
										Error.Position=Index+2;
										Error.Message=L"正则表达式此处不可使用扩展语法。";
										throw Error;
									}
									else
									{
										Function->Content=vrecMatch;
										Index+=3;
									}
									break;
								default:
									Function->Content=vrecNamed;
									Index+=2;
									break;
								}
							}
							else
							{
								Function->Content=vrecNormal;
								Index+=1;
							}
							if(!AllowExtension && (Function->Content==vrecStorage || Function->Content==vrecMatch))
							{
								RegexpError Error;
								Error.Position=Index;
								Error.Message=L"此处不可使用扩展语法。";
								throw Error;
							}

							/*检查模式名称*/
							if(Function->Content!=vrecNormal)
							{
								VInt TempIndex=Index;
								while(true)
								{
									if(Expression[TempIndex]==L'\0')
									{
										RegexpError Error;
										Error.Position=TempIndex;
										Error.Message=L"正则表达式过早结束。";
										throw Error;
									}
									else if(Expression[TempIndex]==L'>')
									{
										Function->Pattern=VUnicodeString(&Expression[Index],TempIndex-Index);
										Index=TempIndex+1;
										if(Function->Pattern==L"")
										{
											RegexpError Error;
											Error.Position=TempIndex-1;
											Error.Message=L"此处缺少模式名称。";
											throw Error;
										}
										else
										{
											break;
										}
									}
									else
									{
										TempIndex++;
									}
								}
							}

							/*检查表达式*/
							if(Expression[Index]==L')')
							{
								if(Function->Content==vrecNormal || Function->Content==vrecStorage)
								{
									RegexpError Error;
									Error.Position=Index;
									Error.Message=L"此处缺少表达式。";
									throw Error;
								}
							}
							else
							{
								if(Function->Content==vrecMatch)
								{
									RegexpError Error;
									Error.Position=Index;
									Error.Message=L"此处不可出现表达式。";
									throw Error;
								}
								else if((Function->Function==vrefPositive) || (Function->Function==vrefNegative))
								{
									Function->Expression=InternalRegexpAnalysis(Expression,Index,false,L')',Index);
								}
								else
								{
									Function->Expression=InternalRegexpAnalysis(Expression,Index,AllowExtension,L')',Index);
								}
							}
						}
						else
						{
							CurrentExpression=InternalRegexpAnalysis(Expression,Index+1,AllowExtension,L')',Index);
						}
					}
					break;
				default:
					{
						VL_RegExpCharset* Charset=new VL_RegExpCharset;
						CurrentExpression=Charset;
						switch(Expression[Index])
						{
						case L'^':
							if(AllowExtension)
							{
								Charset->Type=vrecStringBegin;
							}
							else
							{
								RegexpError Error;
								Error.Position=Index+1;
								Error.Message=L"此处不可使用扩展语法。";
								throw Error;
							}
							break;
						case L'$':
							if(AllowExtension)
							{
								Charset->Type=vrecStringEnd;
							}
							else
							{
								RegexpError Error;
								Error.Position=Index+1;
								Error.Message=L"此处不可使用扩展语法。";
								throw Error;
							}
							break;
						case L'\\':
							{
								VInt Length=0;
								FullTransformAnalysis(Expression,Index+1,Length,Charset->Reverse,Charset->CharSet);
								Index+=Length;
							}
							break;
						case L'[':
							{
								Index++;
								if(Expression[Index]==L'^')
								{
									Charset->Reverse=true;
									Index++;
								}
								while(Expression[Index]!=L']')
								{
									VWChar Start=0;
									VWChar End=0;
									VInt Length=0;

									/*获得一个区间*/
									End=Start=CharsetCharAnalysis(Expression,Index,Length);
									Index+=Length;

									if(Expression[Index]==L'-')
									{
										Index++;
										End=CharsetCharAnalysis(Expression,Index,Length);
										Index+=Length;
									}
									else if(Expression[Index]==L'\0')
									{
										RegexpError Error;
										Error.Position=Index;
										Error.Message=L"正则表达式过早结束。";
										throw Error;
									}

									/*检查当前区间是否跟之前的区间定义冲突*/
									if(Start>End)
									{
										RegexpError Error;
										Error.Position=Index;
										Error.Message=L"错误的区间。";
										throw Error;
									}
									for(VInt i=0;i<Charset->CharSet.GetCount();i++)
									{
										VL_RegCharRange Range=Charset->CharSet[i];
										if(Start<=Range.Value && End>=Range.Key)
										{
											RegexpError Error;
											Error.Position=Index;
											Error.Message=L"错误的区间。";
											throw Error;
										}
									}
									VL_RegCharRange Range;
									Range.Key=Start;
									Range.Value=End;
									Charset->CharSet.Add(Range);
								}
							}
							break;
						case L'{':case L'}':case L']':case L'(':case L')':
						case L'?':case L'+':case L'-':case L'*':case L'|':case L'<':case L'>':
							{
								RegexpError Error;
								Error.Position=Index+1;
								Error.Message=L"错误的功能字符。";
								throw Error;
							}
						case L'\0':
							{
								RegexpError Error;
								Error.Position=Index;
								Error.Message=L"正则表达式过早结束。";
								throw Error;
							}
						default:
							{
								VL_RegCharRange Range;
								Range.Key=Expression[Index];
								Range.Value=Expression[Index];
								Charset->CharSet.Add(Range);
							}
						}
					}
				}
				Index++;
				/*检查循环项目*/
				VL_RegExpLoop* Loop=0;
				switch(Expression[Index])
				{
				case L'?':
					Loop=new VL_RegExpLoop;
					Loop->Expression=CurrentExpression;
					Loop->Begin=0;
					Loop->End=1;
					Loop->Finite=true;
					Sequence->Sections.Add(Loop);
					Index++;
					break;
				case L'+':
					Loop=new VL_RegExpLoop;
					Loop->Expression=CurrentExpression;
					Loop->Begin=1;
					Loop->Finite=false;
					Sequence->Sections.Add(Loop);
					Index++;
					break;
				case L'*':
					Loop=new VL_RegExpLoop;
					Loop->Expression=CurrentExpression;
					Loop->Begin=0;
					Loop->Finite=false;
					Sequence->Sections.Add(Loop);
					Index++;
					break;
				case L'{':
					Loop=new VL_RegExpLoop;
					Loop->Expression=CurrentExpression;
					Sequence->Sections.Add(Loop);
					{
						VInt Length=0;
						Index++;
						/*检查最低循环次数*/
						Loop->Begin=IntegerAnalysis(Expression,Index,Length);
						Index+=Length;
						/*检查最高循环次数*/
						if(Expression[Index]==L',')
						{
							Index++;
							if(Expression[Index]==L'}')
							{
								Loop->Finite=false;
							}
							else
							{
								Loop->Finite=true;
								Length=0;
								Loop->End=IntegerAnalysis(Expression,Index,Length);
								Index+=Length;
								if(Loop->End<Loop->Begin)
								{
									RegexpError Error;
									Error.Position=Index;
									Error.Message=L"循环的结束次数必须比起始次数大。";
									throw Error;
								}
							}
						}
						else
						{
							Loop->End=Loop->Begin;
						}
						if(Expression[Index]==L'}')
						{
							Index++;
						}
						else
						{
							RegexpError Error;
							Error.Position=Index;
							Error.Message=L"此处缺少\"}\"。";
							throw Error;
						}
					}
					break;
				default:
					if(CurrentExpression)
					{
						Sequence->Sections.Add(CurrentExpression);
					}
				}
				if(Loop && Expression[Index]==L'?')
				{
					if(AllowExtension)
					{
						Loop->Short=true;
						Index++;
					}
					else
					{
						RegexpError Error;
						Error.Position=Index+1;
						Error.Message=L"此处不可使用扩展语法。";
						throw Error;
					}
				}
			}
		}

		TerminalIndex=Index;
		return Result;
	}

	class RegexpSemanticAnalysisAlgorithm : public IVL_RegExpRecursiveAlgorithm
	{
		typedef VL_MultiMap<VUnicodeString , VUnicodeString , false>	StringListMap;
		typedef VL_MultiMap<VUnicodeString , VL_RegExpFunc* , false>	NamedRefMap;
		typedef StringListMap::ValueList								StringList;
	protected:

		VL_ListedMap<VUnicodeString , VL_RegExpBase*>	FNamedExpression;
		NamedRefMap										FNamedReferences;
		StringListMap									FRefGraph;
		StringList										FNamedStack;
		StringList										FTopNamedExpressions;
		VInt											FLevelCounter;

		void IncCounter()
		{
			FLevelCounter++;
		}

		void DecCounter()
		{
			FLevelCounter--;
			if(FLevelCounter==0)
			{
				for(VInt i=0;i<FNamedExpression.KeyCount();i++)
				{
					if(FNamedExpression.ValueOfIndex(i)==0)
					{
						RegexpError Error;
						Error.Position=-1;
						Error.Message=L"找不到命名子表达式\""+FNamedExpression.KeyOfIndex(i)+L"\"的表达式定义。";
						throw Error;
					}
				}
				for(VInt i=0;i<FTopNamedExpressions.GetCount();i++)
				{
					StringList RefStack;
					RefStack.Add(FTopNamedExpressions[i]);
					for(VInt j=0;j<RefStack.GetCount();j++)
					{
						VUnicodeString Current=RefStack[j];
						if(FRefGraph.Exists(Current))
						{
							StringList& Children=FRefGraph[Current];
							for(VInt k=0;k<Children.GetCount();k++)
							{
								VUnicodeString& Child=Children[k];
								if(RefStack.Exists(Child))
								{
									RegexpError Error;
									Error.Position=-1;
									Error.Message=L"命名子表达式\""+Child+L"\"出现递归引用。";
									throw Error;
								}
								else
								{
									RefStack.Add(Child);
								}
							}
						}
					}
				}
				for(VInt i=0;i<FNamedReferences.KeyCount();i++)
				{
					NamedRefMap::ValueList& Values=FNamedReferences.ValueOfIndex(i);
					VL_RegExpBase* Node=FNamedExpression[FNamedReferences.KeyOfIndex(i)];
					for(VInt j=0;j<Values.GetCount();j++)
					{
						Values[j]->ReferenceSource=Node;
					}
				}
			}
		}
	public:
		RegexpSemanticAnalysisAlgorithm()
		{
			FLevelCounter=0;
		}

		void VisitBegin (VL_RegExpCharset* Node , VBool& VisitChildren , VBool& RunEnd)
		{
			IncCounter();
		}

		void VisitBegin (VL_RegExpLoop* Node , VBool& VisitChildren , VBool& RunEnd)
		{
			IncCounter();
		}

		void VisitBegin (VL_RegExpFunc* Node , VBool& VisitChildren , VBool& RunEnd)
		{
			IncCounter();
			if(Node->Content==vrecNamed)
			{
				if(FNamedStack.Exists(Node->Pattern))
				{
					RegexpError Error;
					Error.Position=-1;
					Error.Message=L"命名子表达式\""+Node->Pattern+L"\"出现递归引用。";
					throw Error;
				}
				else
				{
					if(FNamedStack.GetCount())
					{
						VUnicodeString CurrentPattern=FNamedStack[FNamedStack.GetCount()-1];
						FRefGraph.AddDistinct(CurrentPattern,Node->Pattern);
					}
					else
					{
						FTopNamedExpressions.Add(Node->Pattern);
					}
					FNamedStack.Add(Node->Pattern);
					VBool HasExpression=Node->Expression;
					if(FNamedExpression.Exists(Node->Pattern))
					{
						if(FNamedExpression[Node->Pattern]!=0 && HasExpression)
						{
							RegexpError Error;
							Error.Position=-1;
							Error.Message=L"命名子表达式\""+Node->Pattern+L"\"重复声明。";
							throw Error;
						}
						if(HasExpression)
						{
							FNamedExpression[Node->Pattern]=Node->Expression.Object();
						}
						else
						{
							FNamedReferences.Add(Node->Pattern,Node);
						}
					}
					else
					{
						if(HasExpression)
						{
							FNamedExpression.Add(Node->Pattern,Node->Expression.Object());
						}
						else
						{
							FNamedExpression.Add(Node->Pattern,0);
							FNamedReferences.Add(Node->Pattern,Node);
						}
					}
				}
			}
		}

		void VisitBegin (VL_RegExpSection* Node , VBool& VisitChildren , VBool& RunEnd)
		{
			IncCounter();
		}

		void VisitEnd (VL_RegExpCharset* Node)
		{
			DecCounter();
		}

		void VisitEnd (VL_RegExpLoop* Node)
		{
			DecCounter();
		}

		void VisitEnd (VL_RegExpFunc* Node)
		{
			if(Node->Content==vrecNamed)
			{
				FNamedStack.Fetch();
			}
			DecCounter();
		}

		void VisitEnd (VL_RegExpSection* Node)
		{
			DecCounter();
		}
	};

	VL_RegExpResult RegularExpressionAnalysis(VUnicodeString Expression , VBool AllowExtension)
	{
		try
		{
			VInt TerminalIndex=0;
			VL_RegExpResult Result;
			Result.Expression=InternalRegexpAnalysis(Expression.Buffer(),0,AllowExtension,L'\0',TerminalIndex);
			RegexpSemanticAnalysisAlgorithm Algorithm;
			Result.Expression->Visit(&Algorithm);
			return Result;
		}
		catch(RegexpError Error)
		{
			VL_RegExpResult Result;
			Result.Error=true;
			Result.ErrorPosition=Error.Position;
			Result.ErrorMessage=Error.Message;
			return Result;
		}
	}

/*********************************************************************************************************
RegularExpressionToString
*********************************************************************************************************/

	class RegexpToStringAlgorithm : public IVL_RegExpRecursiveAlgorithm
	{
	protected:
		VUnicodeString Prefix;

		void AppendPrefix()
		{
			Prefix+=L"  ";
		}

		void ReducePrefix()
		{
			if(Prefix.Length()>0)
			{
				Prefix=Prefix.Left(Prefix.Length()-2);
			}
		}
	public:
		VUnicodeString Result;

		void VisitBegin (VL_RegExpCharset* Node , VBool& VisitChildren , VBool& RunEnd)
		{
			switch(Node->Type)
			{
			case vrecStringBegin:
				Result+=Prefix+L"StringBegin\r\n";
				break;
			case vrecStringEnd:
				Result+=Prefix+L"StringEnd\r\n";
				break;
			case vrecCharSet:
				Result+=Prefix+L"CharSet["+(Node->Reverse?L"Reverse":L"Normal")+L"]:"+CharSetToString(Node->CharSet)+L"\r\n";
				break;
			}
		}

		void VisitBegin (VL_RegExpLoop* Node , VBool& VisitChildren , VBool& RunEnd)
		{
			Result+=Prefix+L"Loop["+(Node->Short?L"Short":L"Long")+L"]{"+VUnicodeString(Node->Begin)+L","+(Node->Finite?VUnicodeString(Node->End):L"")+L"} {\r\n";
			AppendPrefix();
		}

		void VisitBegin (VL_RegExpFunc* Node , VBool& VisitChildren , VBool& RunEnd)
		{
			VUnicodeString Function;
			VUnicodeString Content;
			switch(Node->Function)
			{
			case vrefCatch:
				Function=L"Catch";
				break;
			case vrefUncatch:
				Function=L"Uncatch";
				break;
			case vrefPositive:
				Function=L"Positive";
				break;
			case vrefNegative:
				Function=L"Negative";
				break;
			}
			switch(Node->Content)
			{
			case vrecNamed:
				Content=L"Named:"+Node->Pattern;
				break;
			case vrecStorage:
				Content=L"Storage:"+Node->Pattern;
				break;
			case vrecMatch:
				Content=L"Match:"+Node->Pattern;
				break;
			case vrecNormal:
				Function=L"Normal";
				break;
			}
			Result+=Prefix+L"Function["+Function+L"]["+Content+L"] {\r\n";
			AppendPrefix();
		}

		void VisitBegin (VL_RegExpSection* Node , VBool& VisitChildren , VBool& RunEnd)
		{
			Result+=Prefix+(Node->Type==vresSequence?L"Sequence {\r\n":L"Branch {\r\n");
			AppendPrefix();
		}

		void VisitEnd (VL_RegExpCharset* Node)
		{
		}

		void VisitEnd (VL_RegExpLoop* Node)
		{
			ReducePrefix();
			Result+=Prefix+L"}\r\n";
		}

		void VisitEnd (VL_RegExpFunc* Node)
		{
			ReducePrefix();
			Result+=Prefix+L"}\r\n";
		}

		void VisitEnd (VL_RegExpSection* Node)
		{
			ReducePrefix();
			Result+=Prefix+L"}\r\n";
		}
	};

	VUnicodeString RegularExpressionToString(VL_RegExpPtr Expression)
	{
		if(Expression)
		{
			RegexpToStringAlgorithm Algorithm;
			Algorithm.Result=L"";
			Expression->Visit(&Algorithm);
			return Algorithm.Result;
		}
		else
		{
			return L"";
		}
	}

/*********************************************************************************************************
RegularExpressionCharsetNormalize
*********************************************************************************************************/
	
	class RegexpNormalizeAlgorithm : public IVL_RegExpRecursiveAlgorithm
	{
	public:
	protected:
		VInt				FLevelCounter;

		void IncCounter()
		{
			FLevelCounter++;
		}

		void DecCounter()
		{
			FLevelCounter--;
			if(FLevelCounter==0)
			{
				CharClass->CharClass[0]=-2;
				for(VInt i=1;i<65536;i++)
				{
					CharClass->CharClass[i]=-1;
				}
				for(VInt i=0;i<CharClass->CharRange.GetCount();i++)
				{
					VL_RegCharRange Range=CharClass->CharRange[i];
					for(VInt j=Range.Key;j<=Range.Value;j++)
					{
						CharClass->CharClass[j]=i;
					}
				}
			}
		}

		void AddRange(VL_RegCharRange Range)
		{
			for(VInt i=0;i<CharClass->CharRange.GetCount();i++)
			{
				VL_RegCharRange Old=CharClass->CharRange[i];
				if(Old.Key<Range.Key)
				{
					if(Old.Value<Range.Key)
					{
						/*
						[  Old  ]
						          [  New  ]
						*/
					}
					else if(Old.Value<Range.Value)
					{
						/*
						[  Old       ]
						          [  New  ]
						*/
						VL_RegCharRange Mid;
						CharClass->CharRange.Delete(CharClass->CharRange.IndexOf(Old));
						Mid.Key=Range.Key;
						Mid.Value=Old.Value;
						Range.Key=Mid.Value+1;
						Old.Value=Mid.Key-1;
						CharClass->CharRange.Add(Old);
						CharClass->CharRange.Add(Mid);
						i++;
					}
					else if(Old.Value==Range.Value)
					{
						/*
						[       Old       ]
						          [  New  ]
						*/
						CharClass->CharRange.Delete(CharClass->CharRange.IndexOf(Old));
						Old.Value=Range.Key-1;
						CharClass->CharRange.Add(Range);
						CharClass->CharRange.Add(Old);
						return;
					}
					else
					{
						/*
						[        Old        ]
						          [  New  ]
						*/
						VL_RegCharRange Left,Right;
						CharClass->CharRange.Delete(CharClass->CharRange.IndexOf(Old));
						Left.Key=Old.Key;
						Left.Value=Range.Key-1;
						Right.Key=Range.Value+1;
						Right.Value=Old.Value;
						CharClass->CharRange.Add(Left);
						CharClass->CharRange.Add(Range);
						CharClass->CharRange.Add(Right);
						return;
					}
				}
				else if(Old.Key==Range.Key)
				{
					if(Old.Value<Range.Value)
					{
						/*
						[  Old  ]
						[   New   ]
						*/
						Range.Key=Old.Value+1;
					}
					else if(Old.Value==Range.Value)
					{
						/*
						[  Old  ]
						[  New  ]
						*/
						return;
					}
					else
					{
						/*
						[   Old   ]
						[  New  ]
						*/
						CharClass->CharRange.Delete(CharClass->CharRange.IndexOf(Old));
						Old.Key=Range.Value+1;
						CharClass->CharRange.Add(Range);
						CharClass->CharRange.Add(Old);
						return;
					}
				}
				else if(Old.Key<=Range.Value)
				{
					if(Old.Value<Range.Value)
					{
						/*
						  [  Old  ]
						[   New     ]
						*/
						VL_RegCharRange Left;
						Left.Key=Range.Key;
						Left.Value=Old.Key-1;
						Range.Key=Old.Value+1;
						CharClass->CharRange.Add(Left);
						i++;
					}
					else if(Old.Value==Range.Value)
					{
						/*
						  [  Old  ]
						[  New    ]
						*/
						Range.Value=Old.Key-1;
						CharClass->CharRange.Add(Range);
						return;
					}
					else
					{
						/*
						  [   Old   ]
						[  New  ]
						*/
						VL_RegCharRange Mid;
						CharClass->CharRange.Delete(CharClass->CharRange.IndexOf(Old));
						Mid.Key=Old.Key;
						Mid.Value=Range.Value;
						Range.Value=Mid.Key-1;
						Old.Key=Mid.Value+1;
						CharClass->CharRange.Add(Old);
						CharClass->CharRange.Add(Mid);
						CharClass->CharRange.Add(Range);
						return;
					}
				}
			}
			CharClass->CharRange.Add(Range);
		}
	public:
		VL_RegExpCharClass*	CharClass;

		RegexpNormalizeAlgorithm()
		{
			FLevelCounter=0;
			CharClass=0;
		}

		void VisitBegin (VL_RegExpCharset* Node , VBool& VisitChildren , VBool& RunEnd)
		{
			IncCounter();
			if(Node->Reverse)
			{
				Node->Reverse=false;
				VL_RegCharRange Old;
				Old.Key=L'\0';
				Old.Value=L'\0';
				VL_RegCharSet Set;

				for(VInt i=0;i<Node->CharSet.GetCount();i++)
				{
					VL_RegCharRange New=Node->CharSet[i];
					if(New.Key-Old.Value>1)
					{
						VL_RegCharRange Mid;
						Mid.Key=Old.Value+1;
						Mid.Value=New.Key-1;
						Set.Add(Mid);
						Old=New;
					}
				}
				if(Old.Value<65535)
				{
					VL_RegCharRange Mid;
					Mid.Key=Old.Value+1;
					Mid.Value=65535;
					Set.Add(Mid);
				}
				Node->CharSet=Set;
			}
			for(VInt i=0;i<Node->CharSet.GetCount();i++)
			{
				AddRange(Node->CharSet[i]);
			}
		}

		void VisitBegin (VL_RegExpLoop* Node , VBool& VisitChildren , VBool& RunEnd)
		{
			IncCounter();
		}

		void VisitBegin (VL_RegExpFunc* Node , VBool& VisitChildren , VBool& RunEnd)
		{
			IncCounter();
		}

		void VisitBegin (VL_RegExpSection* Node , VBool& VisitChildren , VBool& RunEnd)
		{
			IncCounter();
		}

		void VisitEnd (VL_RegExpCharset* Node)
		{
			DecCounter();
		}

		void VisitEnd (VL_RegExpLoop* Node)
		{
			DecCounter();
		}

		void VisitEnd (VL_RegExpFunc* Node)
		{
			DecCounter();
		}

		void VisitEnd (VL_RegExpSection* Node)
		{
			DecCounter();
		}
	};

	void RegularExpressionCharsetNormalize(VL_RegExpPtr Expression , VL_RegExpCharClass& CharClass)
	{
		if(Expression)
		{
			RegexpNormalizeAlgorithm Algorithm;
			Algorithm.CharClass=&CharClass;
			Expression->Visit(&Algorithm);
		}
	}

	}
}