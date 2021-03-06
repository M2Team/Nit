#include "VL_Stream.h"

namespace vl
{
	namespace stream
	{

/*********************************************************************************************************
VL_FileStream
*********************************************************************************************************/

	VInt VL_FileStream::_Position()
	{
		if(!FStream)return -1;
		fpos_t p;
		fgetpos(FStream,&p);
		return (VInt)p;
	}

	VInt VL_FileStream::_Size()
	{
		fpos_t tp,p;
		fgetpos(FStream,&tp);
		fseek(FStream,0,SEEK_END);
		fgetpos(FStream,&p);
		VInt Result=(VInt)p;
		fsetpos(FStream,&tp);
		return Result;
	}

	void VL_FileStream::_MoveTo(VInt Pos)
	{
		fpos_t p=Pos;
		fsetpos(FStream,&p);
	}

	void VL_FileStream::_MoveOffset(VInt Offset , VLE_StreamPosition BasePos)
	{
		VInt Origin=0;
		switch(BasePos)
		{
		case vspBegin:
			Origin=SEEK_SET;
			break;
		case vspEnd:
			Origin=SEEK_END;
			break;
		case vspCurrent:
			Origin=SEEK_CUR;
			break;
		};
		fseek(FStream,Offset,Origin);
	}

	void VL_FileStream::_Close()
	{
		if(FStream)
		{
			fclose(FStream);
			FStream=0;
		}
	}

	VL_FileStream::VL_FileStream()
	{
		FStream=0;
	}

	VL_FileStream::~VL_FileStream()
	{
		_Close();
	}

	VBool VL_FileStream::Available()
	{
		return FStream!=0;
	}

/*********************************************************************************************************
VL_FileInputStream
*********************************************************************************************************/

	VL_FileInputStream::VL_FileInputStream(VMbcsString FileName)
	{
		FStream=fopen(FileName.Buffer(),"rb");
	}

	VL_FileInputStream::VL_FileInputStream(VUnicodeString FileName)
	{
		FStream=_wfopen(FileName.Buffer(),L"rb");
	}

	VInt VL_FileInputStream::Position()
	{
		return _Position();
	}

	VInt VL_FileInputStream::Size()
	{
		return _Size();
	}

	void VL_FileInputStream::MoveTo(VInt Pos)
	{
		return _MoveTo(Pos);
	}

	void VL_FileInputStream::MoveOffset(VInt Offset , VLE_StreamPosition BasePos)
	{
		return _MoveOffset(Offset,BasePos);
	}

	void VL_FileInputStream::Close()
	{
		return _Close();
	}

	VInt VL_FileInputStream::Read(VPointer Data , VInt Count)
	{
		if(!FStream)return -1;
		return (VInt)fread(Data,1,Count,FStream);
	}

	VBool VL_FileInputStream::IsEnd()
	{
		if(!FStream)return true;
		return feof(FStream)!=0;
	}

/*********************************************************************************************************
VL_CachedFileInputStream
*********************************************************************************************************/

	void VL_CachedFileInputStream::FillCache()
	{
		FCacheSize=VL_FileInputStream::Read(FCache,FDefaultCacheSize);
		FUnreadSize=FCacheSize;
		FReadPoint=FCache;
	}

	void VL_CachedFileInputStream::Init(VInt CacheSize)
	{
		FDefaultCacheSize=CacheSize;
		FCache=new VByte[FDefaultCacheSize];
		FillCache();
	}

	VInt VL_CachedFileInputStream::_Position()
	{
		return VL_FileInputStream::_Position()+FCacheSize-FUnreadSize;
	}

	void VL_CachedFileInputStream::_MoveTo(VInt Pos)
	{
		VL_FileInputStream::_MoveTo(Pos);
		FillCache();
	}

	void VL_CachedFileInputStream::_MoveOffset(VInt Offset , VLE_StreamPosition BasePos)
	{
		switch(BasePos)
		{
		case vspCurrent:
			{
				VInt NewPos=FCacheSize-FUnreadSize+Offset;
				if(NewPos>=0&&NewPos<FCacheSize)
				{
					FUnreadSize-=Offset;
					FReadPoint+=Offset;
				}
				else
				{
					VL_FileInputStream::_MoveOffset(FCacheSize-FUnreadSize+Offset,BasePos);
					FillCache();
				}
			}
			break;
		default:
			VL_FileInputStream::_MoveOffset(Offset,BasePos);
			FillCache();
		};
	}

	VL_CachedFileInputStream::VL_CachedFileInputStream(VMbcsString FileName , VInt CacheSize):VL_FileInputStream(FileName)
	{
		Init(CacheSize);
	}

	VL_CachedFileInputStream::VL_CachedFileInputStream(VUnicodeString FileName , VInt CacheSize):VL_FileInputStream(FileName)
	{
		Init(CacheSize);
	}

	VL_CachedFileInputStream::~VL_CachedFileInputStream()
	{
		delete[] FCache;
	}

	VInt VL_CachedFileInputStream::Read(VPointer Data , VInt Count)
	{
		if(Count<=FUnreadSize)
		{
			memcpy(Data,FReadPoint,Count);
			FReadPoint+=Count;
			FUnreadSize-=Count;
			return Count;
		}
		else
		{
			VInt ReadSize=FUnreadSize;
			memcpy(Data,FReadPoint,FUnreadSize);
			if(FCacheSize<FDefaultCacheSize)
			{
				FUnreadSize=0;
				return ReadSize;
			}
			else
			{
				VInt ToreadSize=Count-ReadSize;
				if(ToreadSize<FDefaultCacheSize)
				{
					VInt ReturnSize=ReadSize+Read((VBuffer)Data+ReadSize,Count-ReadSize);
					FillCache();
					return ReturnSize;
				}
				else
				{
					ReadSize+=VL_FileInputStream::Read((VBuffer)Data+ReadSize,Count-ReadSize);
					FillCache();
					return ReadSize;
				}
			}
		}
	}

/*********************************************************************************************************
VL_FileOutputStream
*********************************************************************************************************/

	VL_FileOutputStream::VL_FileOutputStream(VMbcsString FileName , VBool Append)
	{
		FStream=fopen(FileName.Buffer(),Append?"ab":"wb");
	}

	VL_FileOutputStream::VL_FileOutputStream(VUnicodeString FileName , VBool Append)
	{
		FStream=_wfopen(FileName.Buffer(),Append?L"ab":L"wb");
	}

	VInt VL_FileOutputStream::Position()
	{
		return _Position();
	}

	VInt VL_FileOutputStream::Size()
	{
		return _Size();
	}

	void VL_FileOutputStream::MoveTo(VInt Pos)
	{
		return _MoveTo(Pos);
	}

	void VL_FileOutputStream::MoveOffset(VInt Offset , VLE_StreamPosition BasePos)
	{
		return _MoveOffset(Offset,BasePos);
	}

	void VL_FileOutputStream::Close()
	{
		return _Close();
	}

	void VL_FileOutputStream::Write(VPointer Data , VInt Count)
	{
		if(!FStream)return;
		fwrite(Data,1,Count,FStream);
	}

/*********************************************************************************************************
VL_CachedFileOutputStream
*********************************************************************************************************/

	void VL_CachedFileOutputStream::FlushCache()
	{
		VL_FileOutputStream::Write(FCache,FDefaultCacheSize-FUnwriteSize);
		FUnwriteSize=FDefaultCacheSize;
		FWritePoint=FCache;
	}

	void VL_CachedFileOutputStream::Init(VInt CacheSize)
	{
		FDefaultCacheSize=CacheSize;
		FCache=new VByte[FDefaultCacheSize];
		FUnwriteSize=FDefaultCacheSize;
		FWritePoint=FCache;
	}

	VInt VL_CachedFileOutputStream::_Position()
	{
		return VL_FileOutputStream::_Position()+FDefaultCacheSize-FUnwriteSize;
	}

	void VL_CachedFileOutputStream::_MoveTo(VInt Pos)
	{
		VL_FileOutputStream::Write(FCache,FDefaultCacheSize-FUnwriteSize);
		FUnwriteSize=FDefaultCacheSize;
		FWritePoint=FCache;
		VL_FileOutputStream::_MoveTo(Pos);
	}

	void VL_CachedFileOutputStream::_MoveOffset(VInt Offset , VLE_StreamPosition BasePos)
	{
		VL_FileOutputStream::Write(FCache,FDefaultCacheSize-FUnwriteSize);
		FUnwriteSize=FDefaultCacheSize;
		FWritePoint=FCache;
		VL_FileOutputStream::_MoveOffset(Offset,BasePos);
	}

	void VL_CachedFileOutputStream::_Close()
	{
		FlushCache();
		delete[] FCache;
	}

	VL_CachedFileOutputStream::VL_CachedFileOutputStream(VMbcsString FileName, VBool Append , VInt CacheSize):VL_FileOutputStream(FileName,Append)
	{
		Init(CacheSize);
	}

	VL_CachedFileOutputStream::VL_CachedFileOutputStream(VUnicodeString FileName, VBool Append , VInt CacheSize):VL_FileOutputStream(FileName,Append)
	{
		Init(CacheSize);
	}

	VL_CachedFileOutputStream::~VL_CachedFileOutputStream()
	{
		_Close();
	}

	void VL_CachedFileOutputStream::Write(VPointer Data , VInt Count)
	{
		if(Count<FUnwriteSize)
		{
			memcpy(FWritePoint,Data,Count);
			FWritePoint+=Count;
			FUnwriteSize-=Count;
		}
		else
		{
			VInt WriteSize=FUnwriteSize;
			memcpy(FWritePoint,Data,WriteSize);
			FlushCache();
			VInt TowriteSize=Count-WriteSize;
			if(TowriteSize<FUnwriteSize)
			{
				Write((VBuffer)Data+WriteSize,TowriteSize);
			}
			else
			{
				VL_FileOutputStream::Write((VBuffer)Data+WriteSize,TowriteSize);
			}
		}
	}

/*********************************************************************************************************
VL_MemoryShellStream
*********************************************************************************************************/

	VL_MemoryShellStream::VL_MemoryShellStream(VPointer Data , VInt Size , VBool MakeEmpty)
	{
		FData=Data;
		FMaxSize=Size;
		FSize=MakeEmpty?0:FMaxSize;
		FPosition=0;
	}

	VL_MemoryShellStream::~VL_MemoryShellStream()
	{
	}

	VInt VL_MemoryShellStream::Position()
	{
		return FPosition;
	}

	VInt VL_MemoryShellStream::Size()
	{
		return FSize;
	}

	void VL_MemoryShellStream::MoveTo(VInt Pos)
	{
		if(Pos<0)
		{
			FPosition=0;
		}
		else if(Pos>FSize)
		{
			FPosition=FSize;
		}
		else
		{
			FPosition=Pos;
		}
	}

	void VL_MemoryShellStream::MoveOffset(VInt Offset , VLE_StreamPosition BasePos)
	{
		VInt Base=FPosition;
		switch(BasePos)
		{
		case vspBegin:
			Base=0;
			break;
		case vspEnd:
			Base=Size();
			break;
		}
		MoveTo(Base+Offset);
	}

	void VL_MemoryShellStream::Close()
	{
	}

	void VL_MemoryShellStream::Write(VPointer Data , VInt Count)
	{
		VInt Left=FMaxSize-FPosition;
		if(Count>Left)
		{
			Count=Left;
		}
		memcpy((VBuffer)FData+FPosition,Data,Count);
		FPosition+=Count;
		if(FPosition>FSize)
		{
			FSize=FPosition;
		}
	}

	VInt VL_MemoryShellStream::Read(VPointer Data , VInt Count)
	{
		VInt Left=FSize-FPosition;
		if(Count>Left)
		{
			Count=Left;
		}
		memcpy(Data,(VBuffer)FData+FPosition,Count);
		FPosition+=Count;
		return Count;
	}

	VBool VL_MemoryShellStream::IsEnd()
	{
		return FPosition>=FSize;
	}

/*********************************************************************************************************
VL_MemoryStream
*********************************************************************************************************/

	VL_MemoryStream::VLS_MemoryNode::VLS_MemoryNode(VL_MemoryStream* Stream)
	{
		Buffer=new VByte[Stream->FBlockSize];
		Next=0;
		Usage=0;
	}

	VL_MemoryStream::VLS_MemoryNode::~VLS_MemoryNode()
	{
		delete[] Buffer;
	}

	void VL_MemoryStream::AddMemoryNode()
	{
		VLS_MemoryNode* Node=new VLS_MemoryNode(this);
		if(FBufferHead)
		{
			FBufferTail->Next=Node;
			FBufferTail=Node;
		}
		else
		{
			FBufferHead=Node;
			FBufferTail=Node;
		}
		FBufferList.Add(Node);
	}

	VL_MemoryStream::VL_MemoryStream(VInt BlockSize)
	{
		FBlockSize=BlockSize;
		FBufferHead=0;
		FBufferTail=0;
		FPosition=0;
	}

	VL_MemoryStream::~VL_MemoryStream()
	{
		Close();
	}

	VInt VL_MemoryStream::Position()
	{
		return FPosition;
	}

	VInt VL_MemoryStream::Size()
	{
		if(FBufferList.GetCount())
		{
			return (FBufferList.GetCount()-1)*FBlockSize+FBufferTail->Usage;
		}
		else
		{
			return 0;
		}
	}

	void VL_MemoryStream::MoveTo(VInt Pos)
	{
		if(Pos<0)
		{
			FPosition=0;
		}
		else
		{
			VInt s=Size();
			if(Pos>s)
			{
				FPosition=s;
			}
			else
			{
				FPosition=Pos;
			}
		}
	}

	void VL_MemoryStream::MoveOffset(VInt Offset , VLE_StreamPosition BasePos)
	{
		VInt Base=FPosition;
		switch(BasePos)
		{
		case vspBegin:
			Base=0;
			break;
		case vspEnd:
			Base=Size();
			break;
		}
		MoveTo(Base+Offset);
	}

	void VL_MemoryStream::Close()
	{
		VLS_MemoryNode* Node=FBufferHead;
		while(Node)
		{
			VLS_MemoryNode* Next=Node->Next;
			delete Node;
			Node=Next;
		}
		FBufferHead=0;
		FBufferTail=0;
		FPosition=0;
		FBufferList.Clear();
	}

	void VL_MemoryStream::Write(VPointer Data , VInt Count)
	{
		if(!Count)return;
		{
			VInt NewSize=FPosition+Count;
			VInt CurrentBlocks=FBufferList.GetCount();
			VInt NewBlocks=NewSize/FBlockSize+(NewSize%FBlockSize>0);
			VInt NewTailUsage=NewSize%FBlockSize;
			if(!NewTailUsage)
			{
				NewTailUsage=FBlockSize;
			}
			if(NewBlocks>CurrentBlocks)
			{
				if(FBufferTail)
				{
					FBufferTail->Usage=FBlockSize;
				}
				VInt More=NewBlocks-CurrentBlocks;
				while(More--)
				{
					AddMemoryNode();
					FBufferTail->Usage=FBlockSize;
				}
				FBufferTail->Usage=NewTailUsage;
			}
			else if(NewBlocks==CurrentBlocks)
			{
				if(FBufferTail->Usage<NewTailUsage)
				{
					FBufferTail->Usage=NewTailUsage;
				}
			}
		}
		{
			VBuffer _Data=(VBuffer)Data;
			VLS_MemoryNode* CurrentBlock=FBufferList[FPosition/FBlockSize];
			do
			{
				VInt Start=FPosition%FBlockSize;
				VInt BlockLeft=FBlockSize-Start;
				if(Count<BlockLeft)
				{
					memcpy(CurrentBlock->Buffer+Start,_Data,Count);
					_Data+=Count;
					FPosition+=Count;
					Count=0;
				}
				else
				{
					memcpy(CurrentBlock->Buffer+Start,_Data,BlockLeft);
					_Data+=BlockLeft;
					Count-=BlockLeft;
					FPosition+=BlockLeft;
					CurrentBlock=CurrentBlock->Next;
				}
			}while(Count);
		}
	}

	VInt VL_MemoryStream::Read(VPointer Data , VInt Count)
	{
		VLS_MemoryNode* CurrentBlock=FBufferList[FPosition/FBlockSize];
		VInt OldPosition=FPosition;
		VInt s=Size();
		VBuffer _Data=(VBuffer)Data;
		do
		{
			VInt Start=FPosition%FBlockSize;
			VInt BlockLeft=CurrentBlock->Usage-Start;
			if(Count<BlockLeft)
			{
				memcpy(_Data,CurrentBlock->Buffer+Start,Count);
				_Data+=Count;
				FPosition+=Count;
				Count=0;
			}
			else
			{
				memcpy(_Data,CurrentBlock->Buffer+Start,BlockLeft);
				_Data+=BlockLeft;
				Count-=BlockLeft;
				FPosition+=BlockLeft;
				CurrentBlock=CurrentBlock->Next;
			}
		}while(Count&&(FPosition<s));
		return FPosition-OldPosition;
	}

	VBool VL_MemoryStream::IsEnd()
	{
		return FPosition>=Size();
	}

/*********************************************************************************************************
VL_TextConverter
*********************************************************************************************************/

	VL_TextConverter::VL_TextConverter()
	{
		FData=0;
		FCount=0;
	}

	VL_TextConverter::~VL_TextConverter()
	{
		if(FData)
		{
			delete[] FData;
		}
	}

	void VL_TextConverter::SetText(VPointer Data , VInt Count , VLE_CharEncode Encode , VBool MakeCopy)
	{
		if(FData)
		{
			delete[] FData;
		}
		FCount=Count;
		if(MakeCopy)
		{
			FData=new VByte[FCount];
			memcpy(FData,Data,FCount);
		}
		else
		{
			FData=(VBuffer)Data;
		}
		if((FEncode=Encode)==vceBOM)
		{
			FEncode=vceMbcs;
		}
	}

	void VL_TextConverter::Convert(VLE_CharEncode Encode)
	{
		if(!FData)return;
		if(!FCount)return;
		if(Encode==vceBOM)
		{
			Encode=vceMbcs;
		}
		if(FEncode==Encode)return;
		switch(FEncode)
		{
		case vceMbcs:
			switch(Encode)
			{
			case vceUtf8:
				Convert(vceUtf16);
				Convert(vceUtf8);
				break;
			case vceUtf16:
				{
					VMbcsString Source((VMbcsString::_TypeBuffer)FData,FCount);
					VUnicodeString Dest=ToUnicode(Source);
					SetText(Dest.Buffer(),Dest.Length()*sizeof(VUnicodeString::_Element),Encode,true);
				}
				break;
			case vceUtf16_be:
				Convert(vceUtf16);
				Convert(vceUtf16_be);
				break;
			}
			break;
		case vceUtf8:
			switch(Encode)
			{
			case vceMbcs:
				Convert(vceUtf16);
				Convert(vceMbcs);
				break;
			case vceUtf16:
				{
					VBuffer NewBuffer=new VByte[FCount*2];
					VBuffer CurRead=FData;
					VInt ReadBytes=0;
					VBuffer CurWrite=NewBuffer;
					while(ReadBytes<FCount)
					{
						if(*CurRead>=0xE0)
						{
							CurWrite[1]=(*CurRead<<4);
							CurRead++;
							CurWrite[1]|=(*CurRead&0x3C)>>2;

							CurWrite[0]=(*CurRead<<6);
							CurRead++;
							CurWrite[0]|=*CurRead&0x3F;
							CurRead++;

							ReadBytes+=3;
						}
						else if(*CurRead>=0xC0)
						{
							CurWrite[1]=((*CurRead&0x1C)>>2);

							CurWrite[0]=(*CurRead&0x03)<<6;
							CurRead++;
							CurWrite[0]|=(*CurRead)&0x3F;
							CurRead++;

							ReadBytes+=2;
						}
						else
						{
							CurWrite[0]=*CurRead;
							CurRead++;

							CurWrite[1]=0x00;

							ReadBytes+=1;
						}
						CurWrite+=2;
					}
					SetText(NewBuffer,(VInt)(CurWrite-NewBuffer),Encode,true);
					delete[] NewBuffer;
				}
				break;
			case vceUtf16_be:
				Convert(vceUtf16);
				Convert(vceUtf16_be);
				break;
			}
			break;
		case vceUtf16:
			switch(Encode)
			{
			case vceMbcs:
				{
					VUnicodeString Source((VUnicodeString::_TypeBuffer)FData,FCount/sizeof(VUnicodeString::_Element));
					VMbcsString Dest=ToMbcs(Source);
					SetText(Dest.Buffer(),Dest.Length(),Encode,true);
				}
				break;
			case vceUtf8:
				{
					VBuffer NewBuffer=new VByte[(FCount/2)*3];
					VBuffer CurRead=FData;
					VBuffer CurWrite=NewBuffer;
					while(CurRead-FData<FCount)
					{
						unsigned __int16 WideChar=*((unsigned __int16*)CurRead);
						if(WideChar<=0x007F)
						{
							*CurWrite=*CurRead;
							CurWrite++;
							CurRead+=2;
						}
						else if(WideChar<=0x07FF)
						{
							*CurWrite=0xC0|(CurRead[1]>>2);
							*CurWrite|=(CurRead[0]&0xC0)<<6;
							CurWrite++;

							*CurWrite=0x80|(CurRead[0]&0x3F);
							CurWrite++;

							CurRead+=2;
						}
						else
						{
							*CurWrite=0xE0|(CurRead[1]>>4);
							CurWrite++;

							*CurWrite=0x80|((CurRead[1]&0x0F)<<2);
							*CurWrite|=(CurRead[0]&0xC0)>>6;
							CurWrite++;

							*CurWrite=0x80|(CurRead[0]&0x3F);
							CurWrite++;

							CurRead+=2;
						}
					}
					SetText(NewBuffer,(VInt)(CurWrite-NewBuffer),Encode,true);
					delete[] NewBuffer;
				}
				break;
			case vceUtf16_be:
				{
					VBuffer First=FData;
					VBuffer Second=FData+1;
					for(VInt i=0;i<FCount/2;i++)
					{
						VByte Temp=*First;
						*First=*Second;
						*Second=Temp;
						First+=2;
						Second+=2;
					}
				}
				break;
			}
			break;
		case vceUtf16_be:
			switch(Encode)
			{
			case vceMbcs:
				Convert(vceUtf16);
				Convert(vceMbcs);
				break;
			case vceUtf8:
				Convert(vceUtf16);
				Convert(vceUtf8);
				break;
			case vceUtf16:
				{
					VBuffer First=FData;
					VBuffer Second=FData+1;
					for(VInt i=0;i<FCount/2;i++)
					{
						VByte Temp=*First;
						*First=*Second;
						*Second=Temp;
						First+=2;
						Second+=2;
					}
				}
				break;
			}
			break;
		}
		FEncode=Encode;
	}

	VInt VL_TextConverter::GetCount()
	{
		return FCount;
	}

	VPointer VL_TextConverter::GetData()
	{
		return FData;
	}

/*********************************************************************************************************
VL_TextInput
*********************************************************************************************************/

	const VByte BOM_Unicode[]={0xFF,0xFE};
	const VByte BOM_Unicode_BE[]={0xFE,0xFF};
	const VByte BOM_UTF8[]={0xEF,0xBB,0xBF};

	VL_TextInput::VL_TextInput(IVL_InputStream* Stream , VBool OwnStream , VLE_CharEncode Encode)
	{
		FStream=Stream;
		FOwnStream=OwnStream;
		if((FEncode=Encode)==vceBOM)
		{
			VByte Byte;
			Stream->Read(&Byte,1);
			switch(Byte)
			{
			case 0xFF:
				FEncode=vceUtf16;
				Stream->MoveOffset(1,vspCurrent);
				break;
			case 0xFE:
				FEncode=vceUtf16_be;
				Stream->MoveOffset(1,vspCurrent);
				break;
			case 0xEF:
				FEncode=vceUtf8;
				Stream->MoveOffset(2,vspCurrent);
				break;
			default:
				FEncode=vceMbcs;
				Stream->MoveOffset(-1,vspCurrent);
			}
		}
	}

	VL_TextInput::~VL_TextInput()
	{
		if(FStream&&FOwnStream)
		{
			delete FStream;
		}
	}

	VLE_CharEncode VL_TextInput::GetEncode()
	{
		return FEncode;
	}

	void VL_TextInput::Read(VMbcsString& String , VInt Count)
	{
		if(Count==-1)
		{
			Count=FStream->Size();
		}
		VBuffer Buffer=new VByte[Count];
		Count=FStream->Read(Buffer,Count);

		VL_TextConverter Converter;
		Converter.SetText(Buffer,Count,FEncode,false);
		Converter.Convert(vceMbcs);
		String=VMbcsString((VMbcsString::_ConstTypeBuffer)Converter.GetData(),Converter.GetCount());
	}

	void VL_TextInput::Read(VUnicodeString& String , VInt Count)
	{
		if(Count==-1)
		{
			Count=FStream->Size();
		}
		VBuffer Buffer=new VByte[Count];
		Count=FStream->Read(Buffer,Count);

		VL_TextConverter Converter;
		Converter.SetText(Buffer,Count,FEncode,false);
		Converter.Convert(vceUtf16);
		String=VUnicodeString((VUnicodeString::_ConstTypeBuffer)Converter.GetData(),Converter.GetCount()/sizeof(VUnicodeString::_Element));
	}

/*********************************************************************************************************
VL_TextOutput
*********************************************************************************************************/

	void WriteBOMToStream(IVL_OutputStream* Stream , VLE_CharEncode Encode)
	{
		switch(Encode)
		{
		case vceUtf8:
			Stream->Write((VPointer)BOM_UTF8,3);
			break;
		case vceUtf16:
			Stream->Write((VPointer)BOM_Unicode,2);
			break;
		case vceUtf16_be:
			Stream->Write((VPointer)BOM_Unicode_BE,2);
			break;
		}
	}

	VL_TextOutput::VL_TextOutput(IVL_OutputStream* Stream , VBool OwnStream , VLE_CharEncode Encode , VBool MakeBOM)
	{
		FStream=Stream;
		FOwnStream=OwnStream;
		FEncode=Encode;
		if(FEncode==vceBOM)
		{
			FEncode=vceMbcs;
		}
		if(MakeBOM)
		{
			WriteBOMToStream(FStream,Encode);
		}
	}

	VL_TextOutput::~VL_TextOutput()
	{
		if(FOwnStream)
		{
			delete FStream;
		}
	}

	void VL_TextOutput::Write(PCChar Buffer , VInt Length)
	{
		if(Length==-1)
		{
			Length=(VInt)strlen(Buffer);
		}
		VL_TextConverter Converter;
		Converter.SetText((VPointer)Buffer,Length,vceMbcs,true);
		Converter.Convert(FEncode);
		FStream->Write(Converter.GetData(),Converter.GetCount());
	}

	void VL_TextOutput::Write(PCWChar Buffer , VInt Length)
	{
		if(Length==-1)
		{
			Length=(VInt)wcslen(Buffer);
		}
		VL_TextConverter Converter;
		Converter.SetText((VPointer)Buffer,Length*sizeof(VCWChar),vceUtf16,true);
		Converter.Convert(FEncode);
		FStream->Write(Converter.GetData(),Converter.GetCount());
	}

	void VL_TextOutput::Write(VMbcsString String)
	{
		Write(String.Buffer(),String.Length());
	}

	void VL_TextOutput::Write(VUnicodeString String)
	{
		Write(String.Buffer(),String.Length());
	}

/*********************************************************************************************************
VL_BinaryInput
*********************************************************************************************************/

	VL_BinaryInput::VL_BinaryInput(IVL_InputStream* Stream , VBool OwnStream)
	{
		FStream=Stream;
		FOwnStream=OwnStream;
	}

	VL_BinaryInput::~VL_BinaryInput()
	{
		if(FOwnStream)
		{
			delete FStream;
		}
	}

	void VL_BinaryInput::Read(VBool& Data)
	{
		FStream->Read(&Data,sizeof(Data));
	}

	void VL_BinaryInput::Read(VInt8u& Data)
	{
		FStream->Read(&Data,sizeof(Data));
	}

	void VL_BinaryInput::Read(VInt8s& Data)
	{
		FStream->Read(&Data,sizeof(Data));
	}

	void VL_BinaryInput::Read(VInt16u& Data)
	{
		FStream->Read(&Data,sizeof(Data));
	}

	void VL_BinaryInput::Read(VInt16s& Data)
	{
		FStream->Read(&Data,sizeof(Data));
	}

	void VL_BinaryInput::Read(VInt32u& Data)
	{
		FStream->Read(&Data,sizeof(Data));
	}

	void VL_BinaryInput::Read(VInt32s& Data)
	{
		FStream->Read(&Data,sizeof(Data));
	}

	void VL_BinaryInput::Read(VInt64u& Data)
	{
		FStream->Read(&Data,sizeof(Data));
	}

	void VL_BinaryInput::Read(VInt64s& Data)
	{
		FStream->Read(&Data,sizeof(Data));
	}

	void VL_BinaryInput::Read(VFloat& Data)
	{
		FStream->Read(&Data,sizeof(Data));
	}

	void VL_BinaryInput::Read(VDouble& Data)
	{
		FStream->Read(&Data,sizeof(Data));
	}

	void VL_BinaryInput::Read(VMbcsString& Data , VInt Bytes)
	{
		if(Bytes==-1)
		{
			Read(Bytes);
		}
		VBuffer Buffer=new VByte[Bytes];
		FStream->Read(Buffer,Bytes);
		Data=VMbcsString((VMbcsString::_TypeBuffer)Buffer,Bytes/sizeof(VMbcsString::_Element));
		delete[] Buffer;
	}

	void VL_BinaryInput::Read(VUnicodeString& Data , VInt Bytes)
	{
		if(Bytes==-1)
		{
			Read(Bytes);
		}
		VBuffer Buffer=new VByte[Bytes];
		FStream->Read(Buffer,Bytes);
		Data=VUnicodeString((VUnicodeString::_TypeBuffer)Buffer,Bytes/sizeof(VUnicodeString::_Element));
		delete[] Buffer;
	}

	void VL_BinaryInput::ReadEncoded(VMbcsString& Data)
	{
		VInt Bytes;
		Read(Bytes);
		VL_TextInput TextInput(FStream,false,vceBOM);
		TextInput.Read(Data,Bytes);
	}

	void VL_BinaryInput::ReadEncoded(VUnicodeString& Data)
	{
		VInt Bytes;
		Read(Bytes);
		VL_TextInput TextInput(FStream,false,vceBOM);
		TextInput.Read(Data,Bytes);
	}

/*********************************************************************************************************
VL_BinaryOutput
*********************************************************************************************************/

	VL_BinaryOutput::VL_BinaryOutput(IVL_OutputStream* Stream , VBool OwnStream)
	{
		FStream=Stream;
		FOwnStream=OwnStream;
	}
	VL_BinaryOutput::~VL_BinaryOutput()
	{
		if(FOwnStream)
		{
			delete FStream;
		}
	}

	void VL_BinaryOutput::Write(VBool Data)
	{
		FStream->Write(&Data,sizeof(Data));
	}

	void VL_BinaryOutput::Write(VInt8u Data)
	{
		FStream->Write(&Data,sizeof(Data));
	}

	void VL_BinaryOutput::Write(VInt8s Data)
	{
		FStream->Write(&Data,sizeof(Data));
	}

	void VL_BinaryOutput::Write(VInt16u Data)
	{
		FStream->Write(&Data,sizeof(Data));
	}

	void VL_BinaryOutput::Write(VInt16s Data)
	{
		FStream->Write(&Data,sizeof(Data));
	}

	void VL_BinaryOutput::Write(VInt32u Data)
	{
		FStream->Write(&Data,sizeof(Data));
	}

	void VL_BinaryOutput::Write(VInt32s Data)
	{
		FStream->Write(&Data,sizeof(Data));
	}

	void VL_BinaryOutput::Write(VInt64u Data)
	{
		FStream->Write(&Data,sizeof(Data));
	}

	void VL_BinaryOutput::Write(VInt64s Data)
	{
		FStream->Write(&Data,sizeof(Data));
	}

	void VL_BinaryOutput::Write(VFloat Data)
	{
		FStream->Write(&Data,sizeof(Data));
	}

	void VL_BinaryOutput::Write(VDouble Data)
	{
		FStream->Write(&Data,sizeof(Data));
	}

	void VL_BinaryOutput::Write(VMbcsString Data , VBool WriteCount)
	{
		VInt Bytes=Data.Length()*sizeof(VMbcsString::_Element);
		if(WriteCount)
		{
			Write(Bytes);
		}
		FStream->Write(Data.Buffer(),Bytes);
	}

	void VL_BinaryOutput::Write(VUnicodeString Data , VBool WriteCount)
	{
		VInt Bytes=Data.Length()*sizeof(VUnicodeString::_Element);
		if(WriteCount)
		{
			Write(Bytes);
		}
		FStream->Write(Data.Buffer(),Bytes);
	}

	void VL_BinaryOutput::WriteEncoded(VMbcsString Data , VLE_CharEncode Encode)
	{
		VL_TextConverter Converter;
		if(Encode==vceBOM)
		{
			Encode=vceMbcs;
		}
		Converter.SetText(Data.Buffer(),Data.Length()*sizeof(VMbcsString::_Element),vceMbcs,true);
		Converter.Convert(Encode);
		Write(Converter.GetCount());
		WriteBOMToStream(FStream,Encode);
		FStream->Write(Converter.GetData(),Converter.GetCount());
	}

	void VL_BinaryOutput::WriteEncoded(VUnicodeString Data , VLE_CharEncode Encode)
	{
		VL_TextConverter Converter;
		if(Encode==vceBOM)
		{
			Encode=vceMbcs;
		}
		Converter.SetText(Data.Buffer(),Data.Length()*sizeof(VUnicodeString::_Element),vceUtf16,true);
		Converter.Convert(Encode);
		Write(Converter.GetCount());
		WriteBOMToStream(FStream,Encode);
		FStream->Write(Converter.GetData(),Converter.GetCount());
	}

	}
}