#ifndef REGEXHELPER
#define REGEXHELPER

#include "..\..\..\Library\Windows\VL_WinGUI.h"
#include "..\..\..\Library\Data\Grammar2\VL_RegExp.h"
#include "..\..\..\Library\Data\Grammar2\VL_RegFA.h"

using namespace vl;
using namespace vl::grammar;
using namespace vl::windows;

extern VUnicodeString GetNodeText(VL_RegExpBase* Node);
extern VUnicodeString GetNodeInfo(VL_RegExpBase* Node);
extern void FillTreeView(VL_RegExpBase* Node , VL_WinTreeView* TreeView);
extern void GetRegexDiscription(VL_RegExpResult& RegexResult , VUnicodeString& InfoSyntaxTree , VUnicodeString& InfoEpsilonNFA , VUnicodeString& InfoNFA , VUnicodeString& InfoDFA);

#endif