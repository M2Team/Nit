#include "VL_RegSim.h"

namespace vl
{
	namespace grammar
	{

/*********************************************************************************************************
VL_RegPureSimulator
*********************************************************************************************************/

	VL_RegPureSimulator::VL_RegPureSimulator(VL_RegDFA::Status* DFA , VL_RegExpCharClass* CharClass)
	{
		{
			RegDFAClosureAlgorithm Algorithm;
			Algorithm.ClosureTest=RegDFAClosureAlgorithm::ctAll;
			DFA->Visit(&Algorithm);
			FStatusList=Algorithm.Path;
		}
		FCharClass=CharClass;
		/*构造状态转移函数*/
		FTransitionFunction=new VInt*[FStatusList.GetCount()];
		for(VInt i=0;i<FStatusList.GetCount();i++)
		{
			FTransitionFunction[i]=new VInt[FCharClass->CharRange.GetCount()];
			VL_RegDFA::Status* CurrentStatus=FStatusList[i];
			/*填充转移表*/
			for(VInt j=0;j<FCharClass->CharRange.GetCount();j++)
			{
				FTransitionFunction[i][j]=-1;
			}
			for(VInt j=0;j<CurrentStatus->OutEdges.GetCount();j++)
			{
				VL_RegDFA::Edge* Edge=CurrentStatus->OutEdges[j];
				if(Edge->Data.Action==vretCharset)
				{
					FTransitionFunction[i][Edge->Data.Parameter]=FStatusList.IndexOf(Edge->End);
				}
			}
		}
	}

	VL_RegPureSimulator::~VL_RegPureSimulator()
	{
		for(VInt i=0;i<FStatusList.GetCount();i++)
		{
			delete[] FTransitionFunction[i];
		}
		delete[] FTransitionFunction;
	}

	void VL_RegPureSimulator::Match(PCWChar Text , Result& MatchResult , VBool PreferLongMatch)
	{
		/*设置起始状态*/
		VInt				Status=0;
		VInt				LastMatchedLength=-1;
		VInt				LastFiniteStatus=-1;
		VInt				MatchedLength=0;

		MatchResult.Succeeded=true;
		MatchResult.MatchedLength=0;
		MatchResult.FiniteStatus=0;
		while(true)
		{
			/*检查是否到达终结状态*/
			if(FStatusList[Status]->Data.FinalStatus)
			{
				if(PreferLongMatch)
				{
					LastMatchedLength=MatchedLength;
					LastFiniteStatus=Status;
				}
				else
				{
					MatchResult.Succeeded=true;
					MatchResult.MatchedLength=MatchedLength;
					MatchResult.FiniteStatus=FStatusList[Status];
					break;
				}
			}
			/*进行状态转移*/
			VBool Transited=false;
			VWChar CurrentChar=*Text++;
			if(CurrentChar)
			{
				/*检查输入字符*/
				VInt CharClass=FCharClass->CharClass[CurrentChar];
				if(CharClass>=0)
				{
					/*如果该字符的字符类存在，则进行状态转移*/
					Status=FTransitionFunction[Status][CharClass];
					/*如果成功转移则标记*/
					if(Status>=0)
					{
						Transited=true;
						MatchedLength++;
					}
				}
			}
			/*转移失败则检查是否曾经经过终结状态*/
			if(!Transited)
			{
				if(LastFiniteStatus==-1)
				{
					MatchResult.Succeeded=false;
					MatchResult.MatchedLength=MatchedLength;
				}
				else
				{
					MatchResult.Succeeded=true;
					MatchResult.MatchedLength=LastMatchedLength;
					MatchResult.FiniteStatus=FStatusList[LastFiniteStatus];
				}
				break;
			}
		}
	}

/*********************************************************************************************************
扩展正则表达式状态跟踪器
*********************************************************************************************************/

	/*字符串片段*/
	struct RegStringPart
	{
		PWChar					Start;
		VInt					Length;
	};
	typedef VL_MultiMap<VInt , RegStringPart , true>			RegStringPartListMap;
	typedef RegStringPartListMap::ValueList						RegStringPartList;

	/*循环结果记录*/
	struct RegReplicator
	{
		VLE_RegEdgeReplicator	Type;						/*循环类型*/
		VInt					ID;							/*循环ID*/
		RegStringPart			StringPart;					/*循环包含的字符串片段*/
	};
	typedef VL_List<RegReplicator , true>						RegReplicatorList;

	/*路径记录*/
	struct RegPath
	{
		RegPath*				PrevPath;
		RegStringPartList		CatchedStrings;				/*捕获到的字符串*/
		RegStringPartListMap	StorageStrings;				/*命名存储区的字符串*/
		RegReplicatorList		Replicators;				/*循环结果表*/

		VBool IsEmpty()
		{
			return CatchedStrings.GetCount()==0 && StorageStrings.KeyCount()==0 && Replicators.GetCount()==0;
		}
	};
	typedef VL_List<RegPath* , true>							RegPathPtrList;
	/*结果记录*/
	struct RegResult
	{
		RegPath*				Path;
		VInt					MatchedLength;
		VL_RegDFA::Status*		FiniteStatus;
	};
	typedef VL_List<RegResult , true>							RegResultList;

	/*指令记录*/
	struct RegIns
	{
		VLS_RegEdgeInstruction	Instruction;				/*指令内容*/
		PWChar					Start;						/*指令开始时字符串的位置*/
	};
	typedef VL_List<RegIns , true>								RegInsList;

	/*路径跟踪器*/
	struct RegTracker
	{
		RegTracker*				PrevTracker;
		RegTracker*				NextTracker;

		RegPath*				Path;						/*跟踪器跟踪的路径片段*/
		RegInsList				UnprocessedInstructions;	/*未完成指令栈*/
		VL_RegNFA::Status*		CurrentNFAStatus;			/*跟踪器的NFA状态*/
		VInt					DFAStatusIndex;				/*跟踪器的DFA状态*/
	};

	/*状态记录*/
	struct RegStatus
	{
		VL_RegDFA::Status*		DFAStatus;					/*DFA状态*/
		PWChar					Buffer;						/*DFA状态对应的字符串位置*/

		VBool operator==(RegStatus& Status)
		{
			return DFAStatus==Status.DFAStatus && Buffer==Status.Buffer;
		}
	};
	typedef VL_List<RegStatus , true>							RegStatusList;

	class RegPool : public VL_Base
	{
	protected:
		class Initer : public VL_Base
		{
		public:
			void Init(RegPath* Path)
			{
				Path->PrevPath=0;
				Path->CatchedStrings.Clear();
				Path->StorageStrings.Clear();
				Path->Replicators.Clear();
			}

			void Init(RegTracker* Tracker)
			{
				Tracker->PrevTracker=0;
				Tracker->NextTracker=0;
				Tracker->Path=0;
				Tracker->UnprocessedInstructions.Clear();
				Tracker->CurrentNFAStatus=0;
				Tracker->DFAStatusIndex=-1;
			}
		};

		typedef VL_Pool<RegPath , Initer>		_PathPool;
		typedef VL_Pool<RegTracker , Initer>	_TrackerPool;
	public:
		_PathPool				PathPool;
		_TrackerPool			TrackerPool;

		RegPool():PathPool(1024),TrackerPool(1024)
		{
		}
	};

	class RegSimulator : public VL_Base
	{
	protected:
		RegStatusList			FStatusList1;
		RegStatusList			FStatusList2;

		RegPool					FPool;						/*对象池*/
		RegPathPtrList			FActivePaths;				/*活动的路径片段*/
		RegTracker*				FTrackerHead;
		RegTracker*				FTrackerTail;
		PWChar					FInitialBuffer;				/*字符串起始位置*/
		PWChar					FStartBuffer;				/*字符串搜索位置*/
		VL_RegDFA::Status*		FInitialStatus;				/*起始状态*/
		RegResultList			FSucceededPaths;			/*成功到达终结状态的路径*/
		RegStatusList*			FCurrentStatusList;			/*活动的状态表*/
		RegStatusList*			FBackupStatusList;			/*备用的状态表*/

		RegPath* AllocatePath(RegPath* PreviousPath)
		{
			RegPath* Path=FPool.PathPool.Alloc();
			Path->PrevPath=PreviousPath;
			FActivePaths.Add(Path);
			return Path;
		}

		void InsertTracker(RegTracker* Tracker , RegTracker* NextTracker)
		{
			if(!NextTracker)
			{
				NextTracker=FTrackerHead;
			}
			if(NextTracker)
			{
				if(NextTracker==FTrackerHead)
				{
					Tracker->NextTracker=FTrackerHead;
					FTrackerHead->PrevTracker=Tracker;
					FTrackerHead=Tracker;
				}
				else
				{
					Tracker->NextTracker=NextTracker;
					Tracker->PrevTracker=NextTracker->PrevTracker;
					Tracker->PrevTracker->NextTracker=Tracker;
					Tracker->NextTracker->PrevTracker=Tracker;
				}
			}
			else
			{
				FTrackerHead=FTrackerTail=Tracker;
			}
		}

		void RemoveTracker(RegTracker* Tracker)
		{
			if(Tracker==FTrackerHead)
			{
				if(FTrackerHead==FTrackerTail)
				{
					FTrackerHead=FTrackerTail=0;
				}
				else
				{
					Tracker->NextTracker->PrevTracker=0;
					FTrackerHead=Tracker->NextTracker;
				}
			}
			else if(Tracker==FTrackerTail)
			{
				Tracker->PrevTracker->NextTracker=0;
				FTrackerTail=Tracker->PrevTracker;
			}
			else
			{
				Tracker->NextTracker->PrevTracker=Tracker->PrevTracker;
				Tracker->PrevTracker->NextTracker=Tracker->NextTracker;
			}
			Tracker->PrevTracker=0;
			Tracker->NextTracker=0;
		}

		RegTracker* AllocateTracker(RegTracker* NextTracker)
		{
			RegTracker* Tracker=FPool.TrackerPool.Alloc();
			InsertTracker(Tracker,NextTracker);
			return Tracker;
		}

		void FreeTracker(RegTracker* Tracker)
		{
			RemoveTracker(Tracker);
			FPool.TrackerPool.Free(Tracker);
		}

		void SwapStatusList()
		{
			/*让FCurrentStatusList在FStatusList1和FStatusList2中切换*/
			if(FCurrentStatusList==&FStatusList1)
			{
				FCurrentStatusList=&FStatusList2;
				FBackupStatusList=&FStatusList1;
			}
			else
			{
				FCurrentStatusList=&FStatusList1;
				FBackupStatusList=&FStatusList2;
			}
		}

		void Initialize()
		{
			/*清除所有数据*/
			for(VInt i=0;i<FActivePaths.GetCount();i++)
			{
				FPool.PathPool.Free(FActivePaths[i]);
			}
			FActivePaths.Clear();
			while(FTrackerHead)
			{
				FreeTracker(FTrackerHead);
			}
			FCurrentStatusList->Clear();
			FSucceededPaths.Clear();
		}
	public:
		RegSimulator()
		{
			FTrackerHead=0;
			FTrackerTail=0;
			FInitialBuffer=0;
			FCurrentStatusList=&FStatusList1;
			FBackupStatusList=&FStatusList2;
		}

		RegResultList& GetSucceededPaths()
		{
			return FSucceededPaths;
		}

		PWChar GetInitBuffer()
		{
			return FInitialBuffer;
		}

		PWChar GetStartBuffer()
		{
			return FStartBuffer;
		}

		void Start(PWChar InitialBuffer , PWChar CurrentBuffer , VL_RegDFA::Status* Status)
		{
			/*清空对象记录，进行初始化*/
			Initialize();
			FInitialBuffer=InitialBuffer;
			FStartBuffer=CurrentBuffer;
			FInitialStatus=Status;

			/*创建一个DFA状态*/
			RegStatus CurrentStatus;
			CurrentStatus.DFAStatus=Status;
			CurrentStatus.Buffer=CurrentBuffer;
			FCurrentStatusList->Add(CurrentStatus);

			/*为当前DFA状态包含的所有NFA状态分别创建一个路径跟踪器*/
			for(VInt i=0;i<Status->Data.NFAStatus.GetCount();i++)
			{
				RegPath* Path=AllocatePath(0);
				RegTracker* Tracker=AllocateTracker(FTrackerHead);
				Tracker->Path=Path;
				Tracker->CurrentNFAStatus=Status->Data.NFAStatus[i];
				Tracker->DFAStatusIndex=0;
			}
		};

#define _USE_SIMULATION_RECORD

		struct SimulationRecord
		{
			VInt	MaxTrackers;		/*同时出现的Tracker的最高次数*/
			VInt	RoundCount;			/*进行的DFA转换次数*/
			VInt	TotalTrackers;		/*分配的Tracker的总次数*/
			VInt	ResultCount;		/*结果的个数*/

			SimulationRecord()
			{
				MaxTrackers=0;
				RoundCount=0;
				TotalTrackers=0;
				ResultCount=0;
			}
		};

		void Simulate(VL_RegPureSimulatorList& LookAheadSimulators , VL_RegExpCharClass* CharClass , SimulationRecord* SR)
		{
			VL_List<VL_RegDFA::Edge* , true> CurrentEdgeList;
			while(FTrackerHead)
			{
				#ifdef _USE_SIMULATION_RECORD
					SR->RoundCount++;
				#endif
				FBackupStatusList->Clear();
				for(VInt i=0;i<FCurrentStatusList->GetCount();i++)
				{
					RegStatus& CurrentRegStatus=(*FCurrentStatusList)[i];
					CurrentEdgeList.Clear();
					/*计算出所有可能符合条件的边
					  Epsilon一定通过
					  Match暂时通过
					  Positive、Negative、StringBegin、StringEnd和CharSet需要经过检查
					*/
					for(VInt j=0;j<CurrentRegStatus.DFAStatus->OutEdges.GetCount();j++)
					{
						VL_RegDFA::Edge* CurrentEdge=CurrentRegStatus.DFAStatus->OutEdges[j];
						VBool Pass=false;
						switch(CurrentEdge->Data.Action)
						{
						case vretEpsilon:case vretMatch:
							Pass=true;
							break;
						case vretPositive:
							{
								VL_RegPureSimulator::Result MatchResult;
								LookAheadSimulators[CurrentEdge->Data.Parameter]->Match(CurrentRegStatus.Buffer,MatchResult,false);
								Pass=MatchResult.Succeeded;
							}
							break;
						case vretNegative:
							{
								VL_RegPureSimulator::Result MatchResult;
								LookAheadSimulators[CurrentEdge->Data.Parameter]->Match(CurrentRegStatus.Buffer,MatchResult,false);
								Pass=!MatchResult.Succeeded;
							}
							break;
						case vretStringBegin:
							Pass=FInitialBuffer==CurrentRegStatus.Buffer;
							break;
						case vretStringEnd:
							Pass=*CurrentRegStatus.Buffer==L'\0';
							break;
						case vretCharset:
							Pass=CharClass->CharClass[*CurrentRegStatus.Buffer]==CurrentEdge->Data.Parameter;
							break;
						}
						if(Pass)
						{
							CurrentEdgeList.Add(CurrentEdge);
						}
					}
					/*寻找所有起源于当前DFA状态的跟踪器*/
					RegTracker* Tracker=FTrackerTail;
					while(Tracker && Tracker->DFAStatusIndex>=0)
					{
						RegTracker* PrevTracker=Tracker->PrevTracker;
						if(Tracker->DFAStatusIndex==i)
						{
							/*寻找从当前跟踪器出发的所有NFA边*/
							for(VInt j=0;j<CurrentEdgeList.GetCount();j++)
							{
								VL_RegDFA::Edge* DFAEdge=CurrentEdgeList[j];
								for(VInt k=0;k<DFAEdge->Data.NFAEdges.GetCount();k++)
								{
									VL_RegNFA::Edge* NFAEdge=DFAEdge->Data.NFAEdges[k];
									if(NFAEdge->Start==Tracker->CurrentNFAStatus)
									{
										VInt DFAStatusIndex=-1;
										{
											/*创建新的RegStatus*/
											RegStatus NewRegStatus;
											NewRegStatus.DFAStatus=DFAEdge->End;
											NewRegStatus.Buffer=CurrentRegStatus.Buffer;
											/*记录新的RegStatus在备份表中的位置*/
											DFAStatusIndex=FBackupStatusList->IndexOf(NewRegStatus);
											if(DFAStatusIndex==-1)
											{
												DFAStatusIndex=FBackupStatusList->GetCount();
												FBackupStatusList->Add(NewRegStatus);
											}
										}
										RegStatus& NewRegStatus=(*FBackupStatusList)[DFAStatusIndex];
										/*创建新的Tracker*/
										#ifdef _USE_SIMULATION_RECORD
											SR->TotalTrackers++;
										#endif
										RegPath* NewPath=AllocatePath(Tracker->Path->IsEmpty()?Tracker->Path->PrevPath:Tracker->Path);
										RegTracker* NewTracker=AllocateTracker(0);
										NewTracker->CurrentNFAStatus=NFAEdge->End;
										NewTracker->Path=NewPath;
										NewTracker->DFAStatusIndex=-1-DFAStatusIndex;
										NewTracker->UnprocessedInstructions=Tracker->UnprocessedInstructions;
										/*执行边的指令*/
										VBool Fail=false;
										for(VInt l=0;l<NFAEdge->Data.Instructions.GetCount() && !Fail;l++)
										{
											VLS_RegEdgeInstruction Ins=NFAEdge->Data.Instructions[l];
											switch(Ins.Instruction)
											{
											case vreiPass:
												NewRegStatus.Buffer+=Ins.Parameter;
												break;
											case vreiMatch:
												{
													Fail=true;
													VInt MatchLength=0;/*设置为0则不能接受空的Match*/
													if(NFAEdge->Data.Action==vretMatch)
													{
														/*如果当前边是Match则进行所有经过的Path的搜索*/
														RegPath* Path=NewTracker->Path;
														while(Path)
														{
															if(Path->StorageStrings.Exists(NFAEdge->Data.Parameter))
															{
																/*取出相应的字符串表*/
																RegStringPartList& Storage=Path->StorageStrings[NFAEdge->Data.Parameter];
																for(VInt m=0;m<Storage.GetCount();m++)
																{
																	/*如果匹配，则记录最长的匹配*/
																	if(wcsncmp(CurrentRegStatus.Buffer,Storage[m].Start,Storage[m].Length)==0)
																	{
																		Fail=false;
																		if(MatchLength<Storage[m].Length)
																		{
																			MatchLength=Storage[m].Length;
																		}
																	}
																}
															}
															Path=Path->PrevPath;
														}
													}
													if(!Fail)
													{
														NewRegStatus.Buffer+=MatchLength;
													}
												}
												break;
											case vreiCatch:case vreiStorage:case vreiLongLoop:case vreiShortLoop:
												{
													RegIns NewIns;
													NewIns.Instruction=Ins;
													NewIns.Start=NewRegStatus.Buffer;
													NewTracker->UnprocessedInstructions.Add(NewIns);
												}
												break;
											case vreiEnd:case vreiEndLoop:
												{
													RegIns OldIns=NewTracker->UnprocessedInstructions.Fetch();
													switch(OldIns.Instruction.Instruction)
													{
													case vreiCatch:
														{
															RegStringPart SP;
															SP.Start=OldIns.Start;
															SP.Length=(VInt)(NewRegStatus.Buffer-OldIns.Start);
															NewTracker->Path->CatchedStrings.Add(SP);
														}
														break;
													case vreiStorage:
														{
															RegStringPart SP;
															SP.Start=OldIns.Start;
															SP.Length=(VInt)(NewRegStatus.Buffer-OldIns.Start);
															NewTracker->Path->StorageStrings.Add(OldIns.Instruction.Parameter,SP);
														}
														break;
													case vreiLongLoop:case vreiShortLoop:
														{
															RegReplicator RR;
															RR.Type=OldIns.Instruction.Instruction==vreiLongLoop?vrerLong:vrerShort;
															RR.ID=OldIns.Instruction.Parameter;
															RR.StringPart.Start=OldIns.Start;
															RR.StringPart.Length=(VInt)(NewRegStatus.Buffer-OldIns.Start);
															NewTracker->Path->Replicators.Add(RR);
														}
														break;
													}
												}
												break;
											}
										}
										/*如果失败了则删掉当前的跟踪器*/
										if(Fail)
										{
											FActivePaths.Delete(FActivePaths.IndexOf(NewTracker->Path));
											FPool.PathPool.Free(NewTracker->Path);
											FreeTracker(NewTracker);
										}
									}
								}
							}
							if(Tracker->Path->IsEmpty())
							{
								FActivePaths.Delete(FActivePaths.IndexOf(Tracker->Path));
								FPool.PathPool.Free(Tracker->Path);
							}
							PrevTracker=Tracker->PrevTracker;
							FreeTracker(Tracker);
						}
						Tracker=PrevTracker;
					}
				}
				SwapStatusList();
				/*修正所有Tracker的DFAStatusIndex记录，并将已经到达终结状态的路径记录下来*/
				RegTracker* Tracker=FTrackerHead;
				#ifdef _USE_SIMULATION_RECORD
					VInt Count=0;
				#endif
				while(Tracker)
				{
					#ifdef _USE_SIMULATION_RECORD
						Count++;
					#endif
					Tracker->DFAStatusIndex=-1-Tracker->DFAStatusIndex;
					RegStatus& CurrentDFAStatus=(*FCurrentStatusList)[Tracker->DFAStatusIndex];
					VL_RegDFA::Status* DFAStatus=CurrentDFAStatus.DFAStatus;
					if(Tracker->CurrentNFAStatus->Data.FinalStatus && DFAStatus!=FInitialStatus)
					{
						#ifdef _USE_SIMULATION_RECORD
							SR->ResultCount++;
						#endif
						RegResult Result;
						Result.Path=Tracker->Path->IsEmpty()?Tracker->Path->PrevPath:Tracker->Path;
						Result.FiniteStatus=DFAStatus;
						Result.MatchedLength=(VInt)(CurrentDFAStatus.Buffer-FStartBuffer);
						FSucceededPaths.Add(Result);
					}
					Tracker=Tracker->NextTracker;
				}
				#ifdef _USE_SIMULATION_RECORD
					if(Count>SR->MaxTrackers)
					{
						SR->MaxTrackers=Count;
					}
				#endif
			}
		}
	};

/*********************************************************************************************************
SimulatorResult
*********************************************************************************************************/

	SimulatorResult::SimString::SimString()
	{
		Start=-1;
		Length=-1;
	}

	SimulatorResult::SimString::SimString(VInt aStart , VInt aLength)
	{
		Start=aStart;
		Length=aLength;
	}

/*********************************************************************************************************
VL_RegRichSimulator
*********************************************************************************************************/

	VL_RegRichSimulator::VL_RegRichSimulator(VL_RegDFA::Status* MainDFA , VL_RegDFAStatusList& LookAheadDFAs , VL_RegExpCharClass* CharClass)
	{
		FMainDFA=MainDFA;
		FCharClass=CharClass;
		for(VInt i=0;i<LookAheadDFAs.GetCount();i++)
		{
			FLookAheadSimulatorList.Add(new VL_RegPureSimulator(LookAheadDFAs[i],FCharClass));
		}
		FInternalSimulator=new RegSimulator;
	}

	VL_RegRichSimulator::~VL_RegRichSimulator()
	{
		delete FInternalSimulator;
	}

	void VL_RegRichSimulator::Match(PWChar Text , PWChar InitText)
	{
		FInternalSimulator->Start(InitText,Text,FMainDFA);
		RegSimulator::SimulationRecord SR;
		FInternalSimulator->Simulate(FLookAheadSimulatorList,FCharClass,&SR);
	}

	void ConvertResult(PWChar InitBuffer , PWChar StartBuffer , RegResult& PathResult , SimulatorResult* SimulateResult)
	{
		RegPath* Path=PathResult.Path;
		while(Path)
		{
			for(VInt i=Path->CatchedStrings.GetCount()-1;i>=0;i--)
			{
				SimulateResult->CatchedStrings.Insert(0,SimulatorResult::SimString(Path->CatchedStrings[i].Start-InitBuffer,Path->CatchedStrings[i].Length));
			}
			for(VInt i=0;i<Path->StorageStrings.KeyCount();i++)
			{
				VInt Key=Path->StorageStrings.KeyOfIndex(i);
				RegStringPartList& SPs=Path->StorageStrings.ValueOfIndex(i);
				for(VInt j=SPs.GetCount()-1;j>=0;j--)
				{
					RegStringPart& SP=SPs[j];
					SimulateResult->StorageStrings.Insert(Key,0,SimulatorResult::SimString(SP.Start-InitBuffer,SP.Length));
				}
			}
			Path=Path->PrevPath;
		}
		SimulateResult->MatchedString=SimulatorResult::SimString(StartBuffer-InitBuffer,PathResult.MatchedLength);
		SimulateResult->FiniteStatus=PathResult.FiniteStatus;
	}

	void VL_RegRichSimulator::GetAllResults(RegSimulator* Simulator , SimulatorResult::List& List)
	{
		List.Clear();
		RegResultList& Paths=Simulator->GetSucceededPaths();
		for(VInt i=0;i<Paths.GetCount();i++)
		{
			SimulatorResult* PathResult=new SimulatorResult;
			ConvertResult(Simulator->GetInitBuffer(),Simulator->GetStartBuffer(),Paths[i],PathResult);
			List.Add(PathResult);
		}
	}

	/********************************************************/

	VInt GetMaxLength(RegResultList& Paths)
	{
		/*计算出最长的匹配长度*/
		VInt MaxLength=0;
		for(VInt i=0;i<Paths.GetCount();i++)
		{
			VInt Length=Paths[i].MatchedLength;
			if(Length>MaxLength)
			{
				MaxLength=Length;
			}
		}
		return MaxLength;
	}

	void FiltrateByLength(RegResultList& Paths)
	{
		VInt MaxLength=GetMaxLength(Paths);
		/*删除所有比最长匹配长度短的匹配*/
		for(VInt i=Paths.GetCount()-1;i>=0;i--)
		{
			if(Paths[i].MatchedLength<MaxLength)
			{
				Paths.Delete(i);
			}
		}
	}

	void GetReplicatorList(RegResult Result , RegReplicatorList& ReplicatorList)
	{
		RegPath* Path=Result.Path;
		/*降Path中的所有循环记录组合起来*/
		while(Path)
		{
			for(VInt i=Path->Replicators.GetCount()-1;i>=0;i--)
			{
				ReplicatorList.Insert(0,Path->Replicators[i]);
			}
			Path=Path->PrevPath;
		}
	}

	enum
	{
		regBetter,
		regUnknown,
		regWeak
	} Compare(RegReplicatorList& RepA , RegReplicatorList& RepB)
	{
		for(VInt i=0;i<RepA.GetCount();i++)
		{
			RegReplicator ItemA=RepA[i];
			for(VInt j=0;j<RepB.GetCount();j++)
			{
				RegReplicator ItemB=RepB[j];
				/*比较两个循环记录*/
				if((ItemA.ID==ItemB.ID) && (ItemA.StringPart.Start==ItemB.StringPart.Start))
				{
					/*如果属于同一个循环，则比较循环包含的字符串的长度*/
					switch(ItemA.Type)
					{
					case vrerLong:
						if(ItemA.StringPart.Length>ItemB.StringPart.Length)
						{
							return regBetter;
						}
						else if(ItemA.StringPart.Length<ItemB.StringPart.Length)
						{
							return regWeak;
						}
						break;
					case vrerShort:
						if(ItemA.StringPart.Length>ItemB.StringPart.Length)
						{
							return regWeak;
						}
						else if(ItemA.StringPart.Length<ItemB.StringPart.Length)
						{
							return regBetter;
						}
						break;
					}
				}
			}
		}
		return regUnknown;
	}

	VL_AutoPtr<SimulatorResult> VL_RegRichSimulator::Filtrate(VBool TestLengthFirst)
	{
		RegResultList&														Paths=FInternalSimulator->GetSucceededPaths();
		VL_List<VL_AutoPtr<RegReplicatorList> , false , RegReplicatorList*>	PathReps;

		if(Paths.GetCount()>0)
		{
			/*如果指定了优先使用长度匹配则挑选出匹配长度最长的一组结果*/
			if(TestLengthFirst)
			{
				FiltrateByLength(Paths);
			}
			/*计算出所有结果的循环记录*/
			for(VInt i=0;i<Paths.GetCount();i++)
			{
				RegReplicatorList* List=new RegReplicatorList;
				GetReplicatorList(Paths[i],*List);
				PathReps.Add(List);
			}
			/*根据循环记录剔除比较差的结果*/
			for(VInt i=Paths.GetCount()-2;i>=0;i--)
			{
				VBool NeedContinuing=false;
				RegReplicatorList& RepA=*PathReps[i].Object();
				for(VInt j=Paths.GetCount()-1;j>i;j--)
				{
					RegReplicatorList& RepB=*PathReps[j].Object();
					/*比较两个循环的优劣*/
					switch(Compare(RepA,RepB))
					{
					case regBetter:
						Paths.Delete(j);
						PathReps.Delete(j);
						break;
					case regWeak:
						Paths.Delete(i);
						PathReps.Delete(i);
						NeedContinuing=true;
						break;
					}
					if(NeedContinuing)
					{
						break;
					}
				}
				if(NeedContinuing)
				{
					continue;
				}
			}
			/*如果没有指定优先使用长度匹配则挑选出匹配长度最长的一组结果*/
			if(!TestLengthFirst)
			{
				FiltrateByLength(Paths);
			}
			/*转换结果*/
			SimulatorResult* PathResult=new SimulatorResult;
			ConvertResult(FInternalSimulator->GetInitBuffer(),FInternalSimulator->GetStartBuffer(),Paths[0],PathResult);
			return PathResult;
		}
		else
		{
			return 0;
		}
	}

	VL_RegDFA::Status* VL_RegRichSimulator::GetDFA()
	{
		return FMainDFA;
	}

/*********************************************************************************************************
VL_RegGreedSimulator
*********************************************************************************************************/

	VL_RegGreedSimulator::VL_RegGreedSimulator(VL_RegNFA::Status* MainNFA , VL_RegDFAStatusList& LookAheadDFAs , VL_RegExpCharClass* CharClass):FStorages(128),FCommands(16384),FStatus(16384)
	{
		FMainNFA=MainNFA;
		FCharClass=CharClass;
		for(VInt i=0;i<LookAheadDFAs.GetCount();i++)
		{
			FLookAheadSimulatorList.Add(new VL_RegPureSimulator(LookAheadDFAs[i],FCharClass));
		}
		FInitText=0;
	}

	void VL_RegGreedSimulator::FillResult(SimulatorResult* Result)
	{
		Result->CatchedStrings.Clear();
		Result->StorageStrings.Clear();
		for(VInt i=0;i<FStorages.Count;i++)
		{
			Storage& s=FStorages.Items[i];
			if(s.StorageID==-1)
			{
				Result->CatchedStrings.Add(SimulatorResult::SimString(s.Start,s.Length));
			}
			else
			{
				Result->StorageStrings.Add(s.StorageID,SimulatorResult::SimString(s.Start,s.Length));
			}
		}
		Result->MatchedString=SimulatorResult::SimString(FStartText-FInitText,FMatchedLength);
		Result->FiniteStatus=0;
	}

	VInt VL_RegGreedSimulator::Match(PWChar Start , PWChar InitText)
	{
		/*初始化环境，推入初始节点*/
		FInitText=InitText;
		FStartText=Start;
		FMatchedLength=-1;
		FFinalStatus=0;
		FStorages.Clear();
		FCommands.Clear();
		FStatus.Clear();
		{
			Status& s=FStatus.Items[0];;
			s.NFAStatus=FMainNFA;
			s.CommandDepth=0;
			s.EdgeIndex=-1;
			s.Start=FStartText-FInitText;
			FStatus.Count=1;
		}
		/*每次回溯并走尽可能多的边，一直到堆栈为空或者找到结果为止*/
		while(FStatus.Count && FMatchedLength==-1)
		{
			/*获得栈顶，回溯*/
			Status& Top=FStatus.Top();
			FCommands.Count=Top.CommandDepth;
			VL_RegNFA::_EdgeList* Edges=&Top.NFAStatus->OutEdges;
			if(Top.EdgeIndex<Edges->GetCount()-1)
			{
				VInt Start=Top.EdgeIndex+1;
				PWChar Buffer=FInitText+Top.Start;
				while(Edges)
				{
					/*查看当前状态中可以通过的第一条边*/
					VL_RegNFA::Edge* Edge=0;
					VInt EdgeIndex=0;
					for(VInt ei=Start;ei<Edges->GetCount();ei++)
					{
						Edge=(*Edges)[ei];
						/*检查边是否通过，不通过则将Edge标记为0，Match边暂时通过，待执行命令的时候检查*/
						switch(Edge->Data.Action)
						{
						case vretCharset:
							if(FCharClass->CharClass[*Buffer]!=Edge->Data.Parameter)
							{
								Edge=0;
							}
							break;
						case vretStringBegin:
							if(Buffer!=FInitText)
							{
								Edge=0;
							}
							break;
						case vretStringEnd:
							if(*Buffer!=L'\0')
							{
								Edge=0;
							}
							break;
						case vretPositive:
							{
								VL_RegPureSimulator::Result Result;
								FLookAheadSimulatorList[Edge->Data.Parameter]->Match(Buffer,Result,false);
								if(!Result.Succeeded)
								{
									Edge=0;
								}
							}
							break;
						case vretNegative:
							{
								VL_RegPureSimulator::Result Result;
								FLookAheadSimulatorList[Edge->Data.Parameter]->Match(Buffer,Result,false);
								if(Result.Succeeded)
								{
									Edge=0;
								}
							}
							break;
						}
						/*找到则尝试执行指令，执行不成功（Match失败）则放弃本边*/
						if(Edge)
						{
							/*记录当前堆栈位置及字符指针*/
							VInt StorageCount=FStorages.Count;
							VInt CommandCount=FCommands.Count;
							PWChar CurrentBuffer=Buffer;
							/*执行边的指令*/
							for(VInt i=0;Edge && i<Edge->Data.Instructions.GetCount();i++)
							{
								VInt Parameter=Edge->Data.Instructions[i].Parameter;
								VLE_RegEdgeInstruction Instruction=Edge->Data.Instructions[i].Instruction;
								VInt Previous=-1;
								if(FCommands.Count)
								{
									switch(FCommands.Top().Instruction)
									{
									case vreiCatch:case vreiStorage:
										Previous=FCommands.Count-1;
										break;
									default:
										Previous=FCommands.Top().Previous;
									}
								}
								switch(Instruction)
								{
								case vreiMatch:/*匹配命名获取*/
									{
										VBool Found=false;
										for(VInt i=0;i<FStorages.Count;i++)
										{
											Storage& st=FStorages.Items[i];
											if(st.StorageID==Edge->Data.Parameter && st.Length>0)
											{
												if(!wcsncmp(FInitText+st.Start,CurrentBuffer,st.Length))
												{
													CurrentBuffer+=st.Length;
													Found=true;
													break;
												}
											}
										}
										if(!Found)
										{
											Edge=0;
										}
									}
									break;
								case vreiCatch:case vreiStorage:/*开始获取*/
									{
										Command& c=FCommands.Push();
										c.Instruction=Instruction;
										c.Parameter=Parameter;
										c.Previous=Previous;
										c.Start=CurrentBuffer-FInitText;
									}
									break;
								case vreiPass:/*通过字符*/
									CurrentBuffer+=Parameter;
									break;
								case vreiEnd:/*结束获取*/
									{
										Command& c=FCommands.Items[Previous];
										VInt StorageID=-1;
										switch(c.Instruction)
										{
										case vreiCatch:
											StorageID=-1;
											break;
										case vreiStorage:
											StorageID=c.Parameter;
											break;
										}
										Storage& s=FStorages.Push();
										s.StorageID=StorageID;
										s.Start=c.Start;
										s.Length=CurrentBuffer-(FInitText+s.Start);
										s.Depth=FStatus.Count+1;
									}
									{
										Command& c=FCommands.Push();
										c.Instruction=vreiEnd;
										c.Parameter=Parameter;
										c.Previous=FCommands.Items[Previous].Previous;
									}
									break;
								}
							}
							if(Edge)
							{
								/*指令执行成功则接受更改*/
								EdgeIndex=ei;
								Buffer=CurrentBuffer;
								break;
							}
							else
							{
								/*否则恢复状态*/
								FStorages.Count=StorageCount;
								FCommands.Count=CommandCount;
							}
						}
					}
					if(Edge)
					{
						FStatus.Top().EdgeIndex=EdgeIndex;
						/*建立新的堆栈对象*/
						Status& s=FStatus.Push();
						s.CommandDepth=FCommands.Count;
						s.EdgeIndex=-1;
						s.NFAStatus=Edge->End;
						s.Start=Buffer-FInitText;
						/*更改Start和Edges以便继续检查*/
						Start=0;
						Edges=&s.NFAStatus->OutEdges;
					}
					else
					{
						/*如果没找到合适的边则检查自己是否终结状态，如果是则结束，否则将栈顶标记为无效节点*/
						Status& Top=FStatus.Top();
						if(Top.NFAStatus->Data.FinalStatus)
						{
							FFinalStatus=Top.NFAStatus;
							FMatchedLength=Top.Start-(FStartText-FInitText);
							Edges=0;
						}
						else
						{
							Top.EdgeIndex=Edges->GetCount();
							Edges=0;
						}
					}
				}
			}
			else
			{
				/*回溯的时候消除必要的存储*/
				VInt TopCount=FStorages.Count;
				for(VInt i=FStorages.Count-1;i>=0;i--)
				{
					if(FStorages.Items[i].Depth>=FStatus.Count)
					{
						TopCount--;
					}
				}
				FStorages.Count=TopCount;
				/*回溯*/
				FStatus.Pop();
			}
		}
		return FMatchedLength;
	}

	VL_RegNFA::Status* VL_RegGreedSimulator::GetNFA()
	{
		return FMainNFA;
	}

	}
}