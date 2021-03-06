#include "VL_WinMain.h"
#include "..\Data\VL_Stream.h"
using namespace vl::stream;

#pragma comment(lib,"Msimg32.lib")

namespace vl
{
	namespace windows
	{

/*********************************************************************************************************
VL_WinRegion
*********************************************************************************************************/

	VBool IsEqual(VL_WinRegion::Ptr Region1 , VL_WinRegion::Ptr Region2)
	{
		return EqualRgn(Region1->GetHandle(),Region2->GetHandle())!=0;
	}

	VL_WinRegion::VL_WinRegion(VInt Left , VInt Top , VInt Right , VInt Bottom , VBool Rectangle)
	{
		if(Rectangle)
		{
			FHandle=CreateRectRgn(Left,Top,Right,Bottom);
		}
		else
		{
			FHandle=CreateEllipticRgn(Left,Top,Right,Bottom);
		}
	}

	VL_WinRegion::VL_WinRegion(RECT Rect , VBool Rectangle)
	{
		if(Rectangle)
		{
			FHandle=CreateRectRgnIndirect(&Rect);
		}
		else
		{
			FHandle=CreateEllipticRgnIndirect(&Rect);
		}
	}

	VL_WinRegion::VL_WinRegion(VInt Left , VInt Top , VInt Right , VInt Bottom , VInt EllipseWidth , VInt EllipseHeight)
	{
		FHandle=CreateRoundRectRgn(Left,Top,Right,Bottom,EllipseWidth,EllipseHeight);
	}

	VL_WinRegion::VL_WinRegion(POINT* Points , VInt Count , VBool Alternate)
	{
		FHandle=CreatePolygonRgn(Points,Count,Alternate?ALTERNATE:WINDING);
	}

	VL_WinRegion::VL_WinRegion(VL_WinRegion::Ptr Region)
	{
		FHandle=CreateRectRgn(0,0,1,1);
		CombineRgn(FHandle,Region->GetHandle(),Region->GetHandle(),RGN_COPY);
	}

	VL_WinRegion::VL_WinRegion(VL_WinRegion::Ptr Region1 , VL_WinRegion::Ptr Region2 , VInt CombineMode)
	{
		FHandle=CreateRectRgn(0,0,1,1);
		CombineRgn(FHandle,Region1->GetHandle(),Region2->GetHandle(),CombineMode);
	}

	VL_WinRegion::VL_WinRegion(HRGN RegionHandle)
	{
		FHandle=RegionHandle;
	}

	VL_WinRegion::~VL_WinRegion()
	{
		DeleteObject(FHandle);
	}

	HRGN VL_WinRegion::GetHandle()
	{
		return FHandle;
	}

	VBool VL_WinRegion::ContainPoint(POINT Point)
	{
		return PtInRegion(FHandle,Point.x,Point.y)!=0;
	}

	VBool VL_WinRegion::ContainRect(RECT Rect)
	{
		return RectInRegion(FHandle,&Rect)!=0;
	}

	RECT VL_WinRegion::GetBoundRect()
	{
		RECT Rect={0,0,0,0};
		GetRgnBox(FHandle,&Rect);
		return Rect;
	}

	void VL_WinRegion::Move(VInt OffsetX , VInt OffsetY)
	{
		OffsetRgn(FHandle,OffsetX,OffsetY);
	}

/*********************************************************************************************************
VL_WinTransform
*********************************************************************************************************/
	
	VL_WinTransform::VL_WinTransform(XFORM Transform)
	{
		FTransform=Transform;
	}

	VL_WinTransform::VL_WinTransform(const VL_WinTransform& Transform)
	{
		FTransform=Transform.FTransform;
	}

	VL_WinTransform& VL_WinTransform::operator=(const VL_WinTransform& Transform)
	{
		FTransform=Transform.FTransform;
		return *this;
	}

	VL_WinTransform VL_WinTransform::operator*(const VL_WinTransform& Transform)
	{
		XFORM Result;
		CombineTransform(&Result,GetHandle(),Transform.GetHandle());
		return Result;
	}

	const XFORM* VL_WinTransform::GetHandle()const
	{
		return &FTransform;
	}

	/*------------------------------------------------------------------------------*/

	VL_WinTransform VL_WinTransform::Translate(VFloat OffsetX , VFloat OffsetY)
	{
		XFORM Transform;
		Transform.eM11=1.0f;
		Transform.eM12=0.0f;
		Transform.eM21=0.0f;
		Transform.eM22=1.0f;
		Transform.eDx=OffsetX;
		Transform.eDy=OffsetY;
		return Transform;
	}

	VL_WinTransform VL_WinTransform::Scale(VFloat ScaleX , VFloat ScaleY)
	{
		XFORM Transform;
		Transform.eM11=ScaleX;
		Transform.eM12=0.0f;
		Transform.eM21=0.0f;
		Transform.eM22=ScaleY;
		Transform.eDx=0.0f;
		Transform.eDy=0.0f;
		return Transform;
	}

	VL_WinTransform VL_WinTransform::Rotate(VFloat Angle)
	{
		XFORM Transform;
		Transform.eM11=cos(Angle);
		Transform.eM12=sin(Angle);
		Transform.eM21=-sin(Angle);
		Transform.eM22=cos(Angle);
		Transform.eDx=0.0f;
		Transform.eDy=0.0f;
		return Transform;
	}

	VL_WinTransform VL_WinTransform::Rotate(VFloat Cos , VFloat Sin)
	{
		XFORM Transform;
		Transform.eM11=Cos;
		Transform.eM12=Sin;
		Transform.eM21=-Sin;
		Transform.eM22=Cos;
		Transform.eDx=0.0f;
		Transform.eDy=0.0f;
		return Transform;
	}

	VL_WinTransform VL_WinTransform::ReflectX()
	{
		XFORM Transform;
		Transform.eM11=1.0f;
		Transform.eM12=0.0f;
		Transform.eM21=0.0f;
		Transform.eM22=-1.0f;
		Transform.eDx=0.0f;
		Transform.eDy=0.0f;
		return Transform;
	}

	VL_WinTransform VL_WinTransform::ReflectY()
	{
		XFORM Transform;
		Transform.eM11=-1.0f;
		Transform.eM12=0.0f;
		Transform.eM21=0.0f;
		Transform.eM22=1.0f;
		Transform.eDx=0.0f;
		Transform.eDy=0.0f;
		return Transform;
	}

	VL_WinTransform VL_WinTransform::Reflect(VFloat VectorX , VFloat VectorY)
	{
		VFloat Len=sqrt(VectorX*VectorX+VectorY*VectorY);
		VFloat Cos=VectorX/Len;
		VFloat Sin=VectorY/Len;

		return Rotate(Cos,-Sin)*ReflectX()*Rotate(Cos,Sin);
	}

	VL_WinTransform VL_WinTransform::Reflect(VFloat OriginX , VFloat OriginY , VFloat VectorX , VFloat VectorY)
	{
		VFloat Len=sqrt(VectorX*VectorX+VectorY*VectorY);
		VFloat Cos=VectorX/Len;
		VFloat Sin=VectorY/Len;

		return Translate(-OriginX,-OriginY)*Rotate(Cos,-Sin)*ReflectX()*Rotate(Cos,Sin)*Translate(OriginX,OriginY);
	}

	VL_WinTransform VL_WinTransform::AxisV(VFloat Xx , VFloat Xy , VFloat Yx , VFloat Yy)
	{
		XFORM Transform;
		Transform.eM11=Xx;
		Transform.eM12=Xy;
		Transform.eM21=Yx;
		Transform.eM22=Yy;
		Transform.eDx=0.0f;
		Transform.eDy=0.0f;
		return Transform;
	}

	VL_WinTransform VL_WinTransform::AxisA(VFloat AngleX , VFloat LenX , VFloat AngleY , VFloat LenY)
	{
		XFORM Transform;
		Transform.eM11=cos(AngleX)*LenX;
		Transform.eM12=sin(AngleX)*LenX;
		Transform.eM21=cos(AngleY)*LenY;
		Transform.eM22=sin(AngleY)*LenY;
		Transform.eDx=0.0f;
		Transform.eDy=0.0f;
		return Transform;
	}

/*********************************************************************************************************
VL_WinMetaFileBuilder
*********************************************************************************************************/

	void VL_WinMetaFileBuilder::Create(VInt Width , VInt Height)
	{
		HDC hdcRef=GetDC(NULL);
		VInt iWidthMM = GetDeviceCaps(hdcRef, HORZSIZE); 
		VInt iHeightMM = GetDeviceCaps(hdcRef, VERTSIZE); 
		VInt iWidthPels = GetDeviceCaps(hdcRef, HORZRES); 
		VInt iHeightPels = GetDeviceCaps(hdcRef, VERTRES); 
		ReleaseDC(NULL,hdcRef);

		RECT Rect;
		Rect.left=0;
		Rect.top=0;
		Rect.right = (Width*iWidthMM*100)/iWidthPels;
		Rect.bottom = (Height*iHeightMM*100)/iHeightPels;

		HDC Handle=CreateEnhMetaFile(NULL,NULL,&Rect,L"VczhLibrary++GDI\0Enhanced Meta File\0");
		FDC->Initialize(Handle);

		FWidth=Width;
		FHeight=Height;
	}

	void VL_WinMetaFileBuilder::Draw(HENHMETAFILE Handle)
	{
		RECT Rect;
		Rect.left=0;
		Rect.top=0;
		Rect.right=FWidth;
		Rect.bottom=FHeight;
		PlayEnhMetaFile(FDC->GetHandle(),Handle,&Rect);
	}

	void VL_WinMetaFileBuilder::Destroy()
	{
		DeleteEnhMetaFile(CloseEnhMetaFile(FDC->GetHandle()));
	}

	VL_WinMetaFileBuilder::VL_WinMetaFileBuilder(VInt Width , VInt Height)
	{
		FDC=new VL_WinProxyDC();
		Create(Width,Height);
	}

	VL_WinMetaFileBuilder::~VL_WinMetaFileBuilder()
	{
		Destroy();
		delete FDC;
	}

	void VL_WinMetaFileBuilder::LoadFrom(VL_WinMetaFile* File)
	{
		Destroy();
		Create(File->GetWidth(),File->GetHeight());
		Draw(File->GetHandle());
	}

	void VL_WinMetaFileBuilder::SaveTo(VL_WinMetaFile* File)
	{
		HENHMETAFILE Handle=CloseEnhMetaFile(FDC->GetHandle());
		if(File->FHandle)
		{
			DeleteEnhMetaFile(File->FHandle);
		}
		File->FHandle=Handle;
		File->FWidth=FWidth;
		File->FHeight=FHeight;
		Create(FWidth,FHeight);
		Draw(Handle);
	}

	void VL_WinMetaFileBuilder::LoadFrom(VUnicodeString FileName)
	{
		VL_WinMetaFile File(FileName);
		Destroy();
		Create(File.GetWidth(),File.GetHeight());
		Draw(File.GetHandle());
	}

	void VL_WinMetaFileBuilder::SaveTo(VUnicodeString FileName)
	{
		HENHMETAFILE Handle=CloseEnhMetaFile(FDC->GetHandle());
		HENHMETAFILE NewHandle=CopyEnhMetaFile(Handle,FileName.Buffer());
		DeleteEnhMetaFile(NewHandle);
		Create(FWidth,FHeight);
		Draw(Handle);
		DeleteEnhMetaFile(Handle);
	}

	VL_WinDC* VL_WinMetaFileBuilder::GetWinDC()
	{
		return FDC;
	}

	VInt VL_WinMetaFileBuilder::GetWidth()
	{
		return FWidth;
	}

	VInt VL_WinMetaFileBuilder::GetHeight()
	{
		return FHeight;
	}

/*********************************************************************************************************
VL_WinMetaFile
*********************************************************************************************************/

	VL_WinMetaFile::VL_WinMetaFile(VUnicodeString FileName)
	{
		FHandle=GetEnhMetaFile(FileName.Buffer());
		ENHMETAHEADER Header;
		GetEnhMetaFileHeader(FHandle,sizeof(Header),&Header);
		FWidth=(Header.rclFrame.right-Header.rclFrame.left)*Header.szlDevice.cx/(Header.szlMillimeters.cx*100);
		FHeight=(Header.rclFrame.bottom-Header.rclFrame.top)*Header.szlDevice.cy/(Header.szlMillimeters.cy*100);
	}

	VL_WinMetaFile::VL_WinMetaFile(VL_WinMetaFileBuilder* Builder)
	{
		FHandle=NULL;
		Builder->SaveTo(this);
	}

	VL_WinMetaFile::~VL_WinMetaFile()
	{
		DeleteEnhMetaFile(FHandle);
	}

	HENHMETAFILE VL_WinMetaFile::GetHandle()
	{
		return FHandle;
	}

	VInt VL_WinMetaFile::GetWidth()
	{
		return FWidth;
	}

	VInt VL_WinMetaFile::GetHeight()
	{
		return FHeight;
	}

/*********************************************************************************************************
VL_WinBitmap
*********************************************************************************************************/

	VInt VL_WinBitmap::GetBitsFromBB(BitmapBits BB)
	{
		switch(BB)
		{
		case vbb32Bits:
			return 32;
		case vbb24Bits:
			return 24;
		default:
			return 1;
		}
	}

	VInt VL_WinBitmap::GetLineBytes(VInt Width , BitmapBits BB)
	{
		VInt Bits=GetBitsFromBB(BB);
		VInt LineBits=Width*Bits;
		VInt AlignBits=sizeof(WORD)*8;
		LineBits+=(AlignBits-LineBits%AlignBits)%AlignBits;
		return LineBits/8;
	}

	void VL_WinBitmap::FillBitmapInfoHeader(VInt Width , VInt Height , BitmapBits Bits , BITMAPINFOHEADER* Header)
	{
		Header->biSize=sizeof(BITMAPINFOHEADER);
		Header->biWidth=Width;
		Header->biHeight=-Height;
		Header->biPlanes=1;
		Header->biBitCount=GetBitsFromBB(Bits);
		Header->biCompression=BI_RGB;
		Header->biSizeImage=0;
		Header->biXPelsPerMeter=0;
		Header->biYPelsPerMeter=0;
		Header->biClrUsed=(Bits==vbb2Bits?2:0);
		Header->biClrImportant=0;
	}

	HBITMAP VL_WinBitmap::CreateDDB(VInt Width , VInt Height , BitmapBits Bits)
	{
		if(Bits==vbb2Bits)
		{
			return CreateBitmap(Width,Height,2,GetBitsFromBB(Bits),NULL);
		}
		else
		{
			VL_WinBitmap Bitmap(1,1,Bits,true);
			return CreateCompatibleBitmap(Bitmap.GetWinDC()->GetHandle(),Width,Height);
		}
	}

	HBITMAP VL_WinBitmap::CreateDIB(VInt Width , VInt Height , BitmapBits Bits , BYTE**& ScanLines)
	{
		BITMAPINFO* Info=(BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+2*sizeof(RGBQUAD));
		FillBitmapInfoHeader(Width,Height,Bits,&Info->bmiHeader);
		Info->bmiColors[0].rgbBlue=0;
		Info->bmiColors[0].rgbGreen=0;
		Info->bmiColors[0].rgbRed=0;
		Info->bmiColors[0].rgbReserved=0;
		Info->bmiColors[1].rgbBlue=255;
		Info->bmiColors[1].rgbGreen=255;
		Info->bmiColors[1].rgbRed=255;
		Info->bmiColors[1].rgbReserved=255;

		BYTE* FirstLine=0;
		HBITMAP Handle=CreateDIBSection(FDC->GetHandle(),Info,DIB_RGB_COLORS,(void**)&FirstLine,NULL,0);
		ScanLines=new BYTE*[Height];
		VInt LineBytes=GetLineBytes(Width,Bits);
		for(VInt i=0;i<Height;i++)
		{
			ScanLines[i]=FirstLine+LineBytes*i;
		}
		free(Info);
		return Handle;
	}

	void VL_WinBitmap::Constructor(VInt Width , VInt Height , BitmapBits Bits , VBool DIBSections)
	{
		FDC=new VL_WinImageDC();
		if(DIBSections)
		{
			FHandle=CreateDIB(Width,Height,Bits,FScanLines);
		}
		else
		{
			FHandle=CreateDDB(Width,Height,Bits);
			FScanLines=0;
		}
		FWidth=Width;
		FHeight=Height;
		FBits=Bits;
		FAlphaChannelBuilt=false;
		HGDIOBJ Object=SelectObject(FDC->GetHandle(),FHandle);
		if(Object)
		{
			DeleteObject(Object);
		}
	}
	
	VL_WinBitmap::VL_WinBitmap(VInt Width , VInt Height , BitmapBits Bits , VBool DIBSections)
	{
		Constructor(Width,Height,Bits,DIBSections);
	}

	VL_WinBitmap::VL_WinBitmap(VUnicodeString FileName , VBool Use32Bits , VBool DIBSections)
	{
		FBits=Use32Bits?vbb32Bits:vbb24Bits;
		
		HBITMAP	TempBmp=(HBITMAP)LoadImage(NULL,FileName.Buffer(),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		HDC		TempDC=CreateCompatibleDC(NULL);
		BITMAP	TempRec;

		GetObject(TempBmp,sizeof(BITMAP),&TempRec);
		DeleteObject(SelectObject(TempDC,TempBmp));

		Constructor(TempRec.bmWidth,TempRec.bmHeight,FBits,DIBSections);
		BitBlt(FDC->GetHandle(),0,0,TempRec.bmWidth,TempRec.bmHeight,TempDC,0,0,SRCCOPY);

		DeleteObject(TempDC);
		DeleteObject(TempBmp);
	}

	VL_WinBitmap::~VL_WinBitmap()
	{
		if(FScanLines)
		{
			delete[] FScanLines;
		}
		if(FHandle)
		{
			DeleteObject(FHandle);
		}
		delete FDC;
	}

	void VL_WinBitmap::SaveToFile(VUnicodeString FileName)
	{
		if(FScanLines)
		{
			BITMAPFILEHEADER Header1;
			BITMAPV5HEADER Header2;
			{
				Header1.bfType='M'*256+'B';
				Header1.bfSize=sizeof(Header1)+sizeof(Header2)+GetLineBytes()*FHeight;
				Header1.bfReserved1=0;
				Header1.bfReserved2=0;
				Header1.bfOffBits=sizeof(Header2)+sizeof(Header1);
			}
			{
				memset(&Header2,0,sizeof(Header2));
				Header2.bV5Size=sizeof(Header2);
				Header2.bV5Width=FWidth;
				Header2.bV5Height=-FHeight;
				Header2.bV5Planes=1;
				Header2.bV5BitCount=GetBitsFromBB(FBits);
				Header2.bV5Compression=BI_RGB;
				Header2.bV5CSType=LCS_sRGB;
				Header2.bV5Intent=LCS_GM_GRAPHICS;
			}
			VL_FileOutputStream Output(FileName,false);
			Output.Write(&Header1,sizeof(Header1));
			Output.Write(&Header2,sizeof(Header2));
			for(VInt i=0;i<FHeight;i++)
			{
				Output.Write(FScanLines[i],GetLineBytes());
			}
		}
		else
		{
			VL_WinBitmap Temp(FWidth,FHeight,FBits,true);
			Temp.GetWinDC()->Copy(0,0,FWidth,FHeight,FDC,0,0);
			Temp.SaveToFile(FileName);
		}
	}

	VL_WinDC* VL_WinBitmap::GetWinDC()
	{
		return FDC;
	}

	VInt VL_WinBitmap::GetWidth()
	{
		return FWidth;
	}

	VInt VL_WinBitmap::GetHeight()
	{
		return FHeight;
	}

	VInt VL_WinBitmap::GetLineBytes()
	{
		return GetLineBytes(FWidth,FBits);
	}

	BYTE** VL_WinBitmap::GetScanLines()
	{
		return FScanLines;
	}

	HBITMAP VL_WinBitmap::GetBitmap()
	{
		return FHandle;
	}

	VL_WinBitmap::BitmapBits VL_WinBitmap::GetBitmapBits()
	{
		return FBits;
	}
	
	void VL_WinBitmap::FillCompatibleHeader(BITMAPINFOHEADER* Header)
	{
		FillBitmapInfoHeader(FWidth,FHeight,FBits,Header);
	}

	VBool VL_WinBitmap::CanBuildAlphaChannel()
	{
		return FScanLines!=0 && FBits==vbb32Bits;
	}

	VBool VL_WinBitmap::IsAlphaChannelBuilt()
	{
		return FAlphaChannelBuilt;
	}

	void VL_WinBitmap::BuildAlphaChannel()
	{
		if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
		{
			FAlphaChannelBuilt=true;
			for(VInt i=0;i<FHeight;i++)
			{
				BYTE* Colors=FScanLines[i];
				VInt j=FWidth;
				while(j--)
				{
					BYTE Alpha=Colors[3];
					Colors[0]=Colors[0]*Alpha/255;
					Colors[1]=Colors[1]*Alpha/255;
					Colors[2]=Colors[2]*Alpha/255;
					Colors+=4;
				}
			}
		}
	}

	void VL_WinBitmap::GenerateTrans(COLORREF Color)
	{
		if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
		{
			for(VInt i=0;i<FHeight;i++)
			{
				COLORREF* Colors=(COLORREF*)FScanLines[i];
				VInt j=FWidth;
				while(j--)
				{
					COLORREF Dest=*Colors & 0x00FFFFFF;
					*Colors = Dest | (0xFF000000 * (Dest!=Color));
					Colors++;
				}
			}
		}
	}

	void VL_WinBitmap::GenerateAlpha(BYTE Alpha)
	{
		if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
		{
			for(VInt i=0;i<FHeight;i++)
			{
				BYTE* Colors=FScanLines[i];
				VInt j=FWidth;
				while(j--)
				{
					Colors[3]=Alpha;
					Colors+=4;
				}
			}
		}
	}

	void VL_WinBitmap::GenerateTransAlpha(COLORREF Color , BYTE Alpha)
	{
		if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
		{
			COLORREF A=Alpha<<24;
			for(VInt i=0;i<FHeight;i++)
			{
				COLORREF* Colors=(COLORREF*)FScanLines[i];
				VInt j=FWidth;
				while(j--)
				{
					COLORREF Dest=*Colors & 0x00FFFFFF;
					*Colors = Dest | (A * (Dest!=Color));
					Colors++;
				}
			}
		}
	}

	void VL_WinBitmap::GenerateLuminance()
	{
		if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
		{
			for(VInt i=0;i<FHeight;i++)
			{
				COLORREF* Colors=(COLORREF*)FScanLines[i];
				VInt j=FWidth;
				while(j--)
				{
					COLORREF Dest=*Colors & 0x00FFFFFF;
					*Colors = Dest | ((GetRValue(Dest)*77 + GetGValue(Dest)*151 + GetBValue(Dest)*28) & 0x0000FF00)<<16;
					Colors++;
				}
			}
		}
	}

	void VL_WinBitmap::GenerateGrayLevel()
	{
		if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
		{
			for(VInt i=0;i<FHeight;i++)
			{
				COLORREF* Colors=(COLORREF*)FScanLines[i];
				VInt j=FWidth;
				while(j--)
				{
					COLORREF Dest=*Colors & 0x00FFFFFF;
					*Colors = Dest | ((GetRValue(Dest)+GetGValue(Dest)+GetBValue(Dest))/3)<<24;
					Colors++;
				}
			}
		}
	}

	void VL_WinBitmap::Generate(BYTE(*Function)(COLORREF))
	{
		if(CanBuildAlphaChannel() && !FAlphaChannelBuilt)
		{
			for(VInt i=0;i<FHeight;i++)
			{
				COLORREF* Colors=(COLORREF*)FScanLines[i];
				VInt j=FWidth;
				while(j--)
				{
					COLORREF Dest= *Colors & 0x00FFFFFF;
					*Colors = Dest | Function(Dest)<<24;
					Colors++;
				}
			}
		}
	}
			
/*********************************************************************************************************
VL_WinBrush
*********************************************************************************************************/

	VL_WinBrush::VL_WinBrush(COLORREF Color)
	{
		FDIBMemory=0;
		FHandle=CreateSolidBrush(Color);
	}

	VL_WinBrush::VL_WinBrush(VInt Hatch , COLORREF Color)
	{
		FDIBMemory=0;
		FHandle=CreateHatchBrush(Hatch,Color);
	}

	VL_WinBrush::VL_WinBrush(VL_WinBitmap::Ptr DIB)
	{
		VL_WinBitmap Temp(DIB->GetWidth(),DIB->GetHeight(),VL_WinBitmap::vbb24Bits,true);
		Temp.GetWinDC()->Draw(0,0,DIB);
		VInt HeaderSize=sizeof(BITMAPINFOHEADER);
		FDIBMemory=new VByte[HeaderSize+DIB->GetHeight()*DIB->GetLineBytes()];
		Temp.FillCompatibleHeader((BITMAPINFOHEADER*)FDIBMemory);
		memcpy(FDIBMemory+HeaderSize,DIB->GetScanLines()[0],DIB->GetHeight()*DIB->GetLineBytes());

		FHandle=CreateDIBPatternBrushPt(FDIBMemory,DIB_RGB_COLORS);
		DWORD Error=GetLastError();
	}

	VL_WinBrush::~VL_WinBrush()
	{
		DeleteObject(FHandle);
		if(FDIBMemory)
		{
			delete[] FDIBMemory;
		}
	}

	HBRUSH VL_WinBrush::GetHandle()
	{
		return FHandle;
	}

/*********************************************************************************************************
VL_WinPen
*********************************************************************************************************/

	VL_WinPen::VL_WinPen(VInt Style , VInt Width , COLORREF Color)
	{
		FDIBMemory=0;
		FHandle=CreatePen(Style,Width,Color);
	}

	VL_WinPen::VL_WinPen(VInt Style , VInt EndCap , VInt Join , VInt Width , COLORREF Color)
	{
		FDIBMemory=0;
		LOGBRUSH Brush;
		Brush.lbColor=Color;
		Brush.lbStyle=BS_SOLID;
		Brush.lbHatch=0;
		FHandle=ExtCreatePen(PS_GEOMETRIC|Style|EndCap|Join,Width,&Brush,0,0);
	}

	VL_WinPen::VL_WinPen(VInt Style , VInt EndCap , VInt Join , VInt Hatch , VInt Width , COLORREF Color)
	{
		FDIBMemory=0;
		LOGBRUSH Brush;
		Brush.lbColor=Color;
		Brush.lbStyle=BS_HATCHED;
		Brush.lbHatch=Hatch;
		FHandle=ExtCreatePen(PS_GEOMETRIC|Style|EndCap|Join,Width,&Brush,0,0);
	}

	VL_WinPen::VL_WinPen(VL_WinBitmap::Ptr DIB , VInt Style , VInt EndCap , VInt Join , VInt Width)
	{
		VL_WinBitmap Temp(DIB->GetWidth(),DIB->GetHeight(),VL_WinBitmap::vbb24Bits,true);
		Temp.GetWinDC()->Draw(0,0,DIB);
		VInt HeaderSize=sizeof(BITMAPINFOHEADER);
		FDIBMemory=new VByte[HeaderSize+DIB->GetHeight()*DIB->GetLineBytes()];
		Temp.FillCompatibleHeader((BITMAPINFOHEADER*)FDIBMemory);
		memcpy(FDIBMemory+HeaderSize,DIB->GetScanLines()[0],DIB->GetHeight()*DIB->GetLineBytes());
		
		LOGBRUSH Brush;
		Brush.lbColor=RGB(0,0,0);
		Brush.lbStyle=BS_DIBPATTERNPT;
		Brush.lbHatch=(LONG)FDIBMemory;
		FHandle=ExtCreatePen(PS_GEOMETRIC|Style|EndCap|Join,Width,&Brush,0,0);
	}

	VL_WinPen::~VL_WinPen()
	{
		DeleteObject(FHandle);
		if(FDIBMemory)
		{
			delete[] FDIBMemory;
		}
	}

	HPEN VL_WinPen::GetHandle()
	{
		return FHandle;
	}

/*********************************************************************************************************
VL_WinFont
*********************************************************************************************************/

	VL_WinFont::VL_WinFont(VUnicodeString Name , VInt Height , VInt Width , VInt Escapement , VInt Orientation , VInt Weight , VBool Italic , VBool Underline , VBool StrikeOut , VBool Antialise)
	{
		FFontInfo.lfHeight=Height;
		FFontInfo.lfWidth=Width;
		FFontInfo.lfEscapement=Escapement;
		FFontInfo.lfOrientation=Orientation;
		FFontInfo.lfWeight=Weight;
		FFontInfo.lfItalic=Italic?TRUE:FALSE;
		FFontInfo.lfUnderline=Underline?TRUE:FALSE;
		FFontInfo.lfStrikeOut=StrikeOut?TRUE:FALSE;
		FFontInfo.lfCharSet=DEFAULT_CHARSET;
		FFontInfo.lfOutPrecision=OUT_DEFAULT_PRECIS;
		FFontInfo.lfClipPrecision=CLIP_DEFAULT_PRECIS;
		FFontInfo.lfQuality=Antialise?CLEARTYPE_QUALITY:NONANTIALIASED_QUALITY;
		FFontInfo.lfPitchAndFamily=DEFAULT_PITCH | FF_DONTCARE;
		wcsncpy(FFontInfo.lfFaceName,Name.Buffer(),LF_FACESIZE-1);
		FHandle=CreateFontIndirect(&FFontInfo);
	}

	VL_WinFont::VL_WinFont(LOGFONT* FontInfo)
	{
		FFontInfo=*FontInfo;
		FHandle=CreateFontIndirect(&FFontInfo);
	}

	VL_WinFont::~VL_WinFont()
	{
		DeleteObject(FHandle);
	}

	HFONT VL_WinFont::GetHandle()
	{
		return FHandle;
	}

	LOGFONT* VL_WinFont::GetInfo()
	{
		return &FFontInfo;
	}

/*********************************************************************************************************
VL_WinDC
*********************************************************************************************************/

	void VL_WinDC::Init()
	{
		FPen=GetApplication()->GetDefaultPen();
		FOldPen=(HPEN)SelectObject(FHandle,FPen->GetHandle());

		FBrush=GetApplication()->GetDefaultBrush();
		FOldBrush=(HBRUSH)SelectObject(FHandle,FBrush->GetHandle());

		FFont=GetApplication()->GetDefaultFont();
		FOldFont=(HFONT)SelectObject(FHandle,FFont->GetHandle());

		SetGraphicsMode(FHandle,GM_ADVANCED);
	}

	VL_WinDC::VL_WinDC()
	{
		FHandle=0;
		FOldPen=0;
		FOldBrush=0;
		FOldFont=0;
	}

	VL_WinDC::~VL_WinDC()
	{
		SelectObject(FHandle,FOldFont);
		SelectObject(FHandle,FOldBrush);
		SelectObject(FHandle,FOldPen);
	}

	HDC VL_WinDC::GetHandle()
	{
		return FHandle;
	}

	void VL_WinDC::SetPen(VL_WinPen::Ptr Pen)
	{
		SelectObject(FHandle,Pen->GetHandle());
		FPen=Pen;
	}

	void VL_WinDC::SetBrush(VL_WinBrush::Ptr Brush)
	{
		SelectObject(FHandle,Brush->GetHandle());
		FBrush=Brush;
	}

	void VL_WinDC::SetFont(VL_WinFont::Ptr Font)
	{
		SelectObject(FHandle,Font->GetHandle());
		FFont=Font;
	}

	COLORREF VL_WinDC::GetBackColor()
	{
		return GetBkColor(FHandle);
	}

	void VL_WinDC::SetBackColor(COLORREF Color)
	{
		SetBkColor(FHandle,Color);
	}

	COLORREF VL_WinDC::GetTextColor()
	{
		return ::GetTextColor(FHandle);
	}

	void VL_WinDC::SetTextColor(COLORREF Color)
	{
		::SetTextColor(FHandle,Color);
	}

	VBool VL_WinDC::GetBackTransparent()
	{
		return GetBkMode(FHandle)==TRANSPARENT;
	}

	void VL_WinDC::SetBackTransparent(VBool Transparent)
	{
		SetBkMode(FHandle,Transparent?TRANSPARENT:OPAQUE);
	}

	POINT VL_WinDC::GetBrushOrigin()
	{
		POINT Point;
		GetBrushOrgEx(FHandle,&Point);
		return Point;
	}

	void VL_WinDC::SetBrushOrigin(POINT Point)
	{
		SetBrushOrgEx(FHandle,Point.x,Point.y,NULL);
	}

	/*------------------------------------------------------------------------------*/

	void VL_WinDC::DrawString(VInt X , VInt Y , VUnicodeString Text)
	{
		TextOut(FHandle,X,Y,Text.Buffer(),Text.Length());
	}

	void VL_WinDC::DrawString(VInt X , VInt Y , VUnicodeString Text , VInt TabWidth , VInt TabOriginX)
	{
		TabbedTextOut(FHandle,X,Y,Text.Buffer(),Text.Length(),1,&TabWidth,TabOriginX);
	}

	void VL_WinDC::FillRegion(VL_WinRegion::Ptr Region)
	{
		FillRgn(FHandle,Region->GetHandle(),FBrush->GetHandle());
	}

	void VL_WinDC::FrameRegion(VL_WinRegion::Ptr Region , VInt BlockWidth , VInt BlockHeight)
	{
		FrameRgn(FHandle,Region->GetHandle(),FBrush->GetHandle(),BlockWidth,BlockHeight);
	}

	void VL_WinDC::MoveTo(VInt X , VInt Y)
	{
		::MoveToEx(FHandle,X,Y,NULL);
	}

	void VL_WinDC::LineTo(VInt X , VInt Y)
	{
		::LineTo(FHandle,X,Y);
	}

	void VL_WinDC::Rectangle(VInt Left , VInt Top , VInt Right , VInt Bottom)
	{
		::Rectangle(FHandle,Left,Top,Right,Bottom);
	}

	void VL_WinDC::Rectangle(RECT Rect)
	{
		::Rectangle(FHandle,Rect.left,Rect.top,Rect.right,Rect.bottom);
	}

	void VL_WinDC::FillRect(VInt Left , VInt Top , VInt Right , VInt Bottom)
	{
		RECT Rect;
		Rect.left=Left;
		Rect.top=Top;
		Rect.right=Right;
		Rect.bottom=Bottom;
		::FillRect(FHandle,&Rect,FBrush->GetHandle());
	}

	void VL_WinDC::FillRect(RECT Rect)
	{
		::FillRect(FHandle,&Rect,FBrush->GetHandle());
	}

	void VL_WinDC::Ellipse(VInt Left , VInt Top , VInt Right , VInt Bottom)
	{
		::Ellipse(FHandle,Left,Top,Right,Bottom);
	}

	void VL_WinDC::Ellipse(RECT Rect)
	{
		::Ellipse(FHandle,Rect.left,Rect.top,Rect.right,Rect.bottom);
	}

	void VL_WinDC::RoundRect(VInt Left , VInt Top , VInt Right , VInt Bottom , VInt EllipseWidth , VInt EllipseHeight)
	{
		::RoundRect(FHandle,Left,Top,Right,Bottom,EllipseWidth,EllipseHeight);
	}

	void VL_WinDC::RoundRect(RECT Rect , VInt EllipseWidth , VInt EllipseHeight)
	{
		::RoundRect(FHandle,Rect.left,Rect.top,Rect.right,Rect.bottom,EllipseWidth,EllipseHeight);
	}

	void VL_WinDC::PolyLine(const POINT* Points , VInt Count)
	{
		::Polyline(FHandle,Points,Count);
	}

	void VL_WinDC::PolyLineTo(const POINT* Points , VInt Count)
	{
		::PolylineTo(FHandle,Points,Count);
	}

	void VL_WinDC::PolyGon(const POINT* Points , VInt Count)
	{
		::Polygon(FHandle,Points,Count);
	}

	void VL_WinDC::PolyBezier(const POINT* Points , VInt Count)
	{
		::PolyBezier(FHandle,Points,Count);
	}

	void VL_WinDC::PolyBezierTo(const POINT* Points , VInt Count)
	{
		::PolyBezierTo(FHandle,Points,Count);
	}

	void VL_WinDC::PolyDraw(const POINT* Points , const BYTE* Actions , VInt PointCount)
	{
		::PolyDraw(FHandle,Points,Actions,PointCount);
	}

	void VL_WinDC::Arc(RECT Bound , POINT Start , POINT End)
	{
		::Arc(FHandle,Bound.left,Bound.top,Bound.right,Bound.bottom,Start.x,Start.y,End.x,End.y);
	}

	void VL_WinDC::Arc(VInt Left , VInt Top , VInt Right , VInt Bottom , VInt StartX , VInt StartY , VInt EndX , VInt EndY)
	{
		::Arc(FHandle,Left,Top,Right,Bottom,StartX,StartY,EndX,EndY);
	}

	void VL_WinDC::ArcTo(RECT Bound , POINT Start , POINT End)
	{
		::ArcTo(FHandle,Bound.left,Bound.top,Bound.right,Bound.bottom,Start.x,Start.y,End.x,End.y);
	}

	void VL_WinDC::ArcTo(VInt Left , VInt Top , VInt Right , VInt Bottom , VInt StartX , VInt StartY , VInt EndX , VInt EndY)
	{
		::ArcTo(FHandle,Left,Top,Right,Bottom,StartX,StartY,EndX,EndY);
	}

	void VL_WinDC::AngleArc(VInt X , VInt Y , VInt Radius , VFloat StartAngle , VFloat SweepAngle)
	{
		::AngleArc(FHandle,X,Y,Radius,StartAngle,SweepAngle);
	}

	void VL_WinDC::AngleArc(VInt X , VInt Y , VInt Radius , VDouble StartAngle , VDouble SweepAngle)
	{
		::AngleArc(FHandle,X,Y,Radius,(VFloat)StartAngle,(VFloat)SweepAngle);
	}

	void VL_WinDC::Chord(RECT Bound , POINT Start , POINT End)
	{
		::Chord(FHandle,Bound.left,Bound.top,Bound.right,Bound.bottom,Start.x,Start.y,End.x,End.y);
	}

	void VL_WinDC::Chord(VInt Left , VInt Top , VInt Right , VInt Bottom , VInt StartX , VInt StartY , VInt EndX , VInt EndY)
	{
		::Chord(FHandle,Left,Top,Right,Bottom,StartX,StartY,EndX,EndY);
	}

	void VL_WinDC::Pie(RECT Bound , POINT Start , POINT End)
	{
		::Pie(FHandle,Bound.left,Bound.top,Bound.right,Bound.bottom,Start.x,Start.y,End.x,End.y);
	}

	void VL_WinDC::Pie(VInt Left , VInt Top , VInt Right , VInt Bottom , VInt StartX , VInt StartY , VInt EndX , VInt EndY)
	{
		::Pie(FHandle,Left,Top,Right,Bottom,StartX,StartY,EndX,EndY);
	}

	void VL_WinDC::GradientTriangle(TRIVERTEX* Vertices , VInt VerticesCount , GRADIENT_TRIANGLE* Triangles , VInt TriangleCount)
	{
		GradientFill(FHandle,Vertices,VerticesCount,Triangles,TriangleCount,GRADIENT_FILL_TRIANGLE);
	}

	/*------------------------------------------------------------------------------*/

	void VL_WinDC::BeginPath()
	{
		::BeginPath(FHandle);
	}

	void VL_WinDC::EndPath()
	{
		::EndPath(FHandle);
	}

	void VL_WinDC::ClosePath()
	{
		::CloseFigure(FHandle);
	}

	void VL_WinDC::DiscardPath()
	{
		::AbortPath(FHandle);
	}

	void VL_WinDC::DrawPath()
	{
		::StrokePath(FHandle);
	}

	void VL_WinDC::FillPath()
	{
		::FillPath(FHandle);
	}

	void VL_WinDC::DrawAndFillPath()
	{
		::StrokeAndFillPath(FHandle);
	}

	VL_WinRegion::Ptr VL_WinDC::RegionFromPath()
	{
		return new VL_WinRegion(::PathToRegion(FHandle));
	}
	
	/*------------------------------------------------------------------------------*/

	VBool VL_WinDC::PointInClip(POINT Point)
	{
		return PtVisible(FHandle,Point.x,Point.y)==TRUE;
	}

	VBool VL_WinDC::RectInClip(RECT Rect)
	{
		return RectVisible(FHandle,&Rect)==TRUE;
	}

	void VL_WinDC::ClipPath(VInt CombineMode)
	{
		SelectClipPath(FHandle,CombineMode);
	}

	void VL_WinDC::ClipRegion(VL_WinRegion::Ptr Region)
	{
		SelectClipRgn(FHandle,Region->GetHandle());
	}

	void VL_WinDC::RemoveClip()
	{
		SelectClipRgn(FHandle,NULL);
	}

	void VL_WinDC::MoveClip(VInt OffsetX , VInt OffsetY)
	{
		OffsetClipRgn(FHandle,OffsetX,OffsetY);
	}

	void VL_WinDC::CombineClip(VL_WinRegion::Ptr Region , VInt CombineMode)
	{
		ExtSelectClipRgn(FHandle,Region->GetHandle(),CombineMode);
	}

	void VL_WinDC::IntersetClipRect(RECT Rect)
	{
		::IntersectClipRect(FHandle,Rect.left,Rect.top,Rect.right,Rect.bottom);
	}

	void VL_WinDC::ExcludeClipRect(RECT Rect)
	{
		::ExcludeClipRect(FHandle,Rect.left,Rect.top,Rect.right,Rect.bottom);
	}

	VL_WinRegion::Ptr VL_WinDC::GetClipRegion()
	{
		HRGN Handle=CreateRectRgn(0,0,1,1);
		GetClipRgn(FHandle,Handle);
		return new VL_WinRegion(Handle);
	}

	RECT VL_WinDC::GetClipBoundRect()
	{
		RECT Rect;
		GetClipBox(FHandle,&Rect);
		return Rect;
	}

	/*------------------------------------------------------------------------------*/

	VL_WinTransform VL_WinDC::GetTransform()
	{
		XFORM Transform;
		GetWorldTransform(FHandle,&Transform);
		return Transform;
	}

	void VL_WinDC::SetTransform(const VL_WinTransform& Transform)
	{
		SetWorldTransform(FHandle,Transform.GetHandle());
	}

	/*------------------------------------------------------------------------------*/

	void VL_WinDC::Copy(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinDC* Source , VInt srcX , VInt srcY , DWORD DrawROP)
	{
		HDC SourceHandle=Source?Source->GetHandle():0;
		BitBlt(FHandle,dstX,dstY,dstW,dstH,SourceHandle,srcX,srcY,DrawROP);
	}

	void VL_WinDC::Copy(RECT dstRect , VL_WinDC* Source , POINT srcPos , DWORD DrawROP)
	{
		HDC SourceHandle=Source?Source->GetHandle():0;
		BitBlt(FHandle,dstRect.left,dstRect.top,dstRect.right-dstRect.left,dstRect.bottom-dstRect.top,SourceHandle,srcPos.x,srcPos.y,DrawROP);
	}

	void VL_WinDC::Copy(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinDC* Source , VInt srcX , VInt srcY ,VInt srcW , VInt srcH , DWORD DrawROP)
	{
		HDC SourceHandle=Source?Source->GetHandle():0;
		StretchBlt(FHandle,dstX,dstY,dstW,dstH,SourceHandle,srcX,srcY,srcW,srcH,DrawROP);
	}

	void VL_WinDC::Copy(RECT dstRect , VL_WinDC* Source , RECT srcRect , DWORD DrawROP)
	{
		HDC SourceHandle=Source?Source->GetHandle():0;
		StretchBlt(	FHandle		,dstRect.left,dstRect.top,dstRect.right-dstRect.left,dstRect.bottom-dstRect.top,
					SourceHandle,srcRect.left,srcRect.top,srcRect.right-srcRect.left,srcRect.bottom-srcRect.top,
					DrawROP);
	}

	void VL_WinDC::Copy(POINT UpperLeft , POINT UpperRight , POINT LowerLeft , VL_WinDC* Source , VInt srcX , VInt srcY , VInt srcW , VInt srcH)
	{
		POINT Pt[3];
		Pt[0]=UpperLeft;
		Pt[1]=UpperRight;
		Pt[2]=LowerLeft;
		PlgBlt(FHandle,Pt,Source->GetHandle(),srcX,srcY,srcW,srcH,0,0,0);
	}

	void VL_WinDC::Copy(POINT UpperLeft , POINT UpperRight , POINT LowerLeft , VL_WinDC*Source , RECT srcRect)
	{
		POINT Pt[3];
		Pt[0]=UpperLeft;
		Pt[1]=UpperRight;
		Pt[2]=LowerLeft;
		PlgBlt(FHandle,Pt,Source->GetHandle(),srcRect.left,srcRect.top,srcRect.right-srcRect.left,srcRect.bottom-srcRect.top,0,0,0);
	}

	void VL_WinDC::CopyTrans(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinDC* Source , VInt srcX , VInt srcY ,VInt srcW , VInt srcH , COLORREF Color)
	{
		TransparentBlt(FHandle,dstX,dstY,dstW,dstH,Source->GetHandle(),srcX,srcY,srcW,srcH,Color);
	}

	void VL_WinDC::CopyTrans(RECT dstRect , VL_WinDC* Source , RECT srcRect , COLORREF Color)
	{
		TransparentBlt(	FHandle				,dstRect.left,dstRect.top,dstRect.right-dstRect.left,dstRect.bottom-dstRect.top,
						Source->GetHandle()	,srcRect.left,srcRect.top,srcRect.right-srcRect.left,srcRect.bottom-srcRect.top,
						Color);
	}

	/*------------------------------------------------------------------------------*/

	void VL_WinDC::Draw(VInt dstX , VInt dstY , VL_WinMetaFile* MetaFile)
	{
		Draw(dstX,dstY,MetaFile->GetWidth(),MetaFile->GetHeight(),MetaFile);
	}

	void VL_WinDC::Draw(POINT Pos , VL_WinMetaFile* MetaFile)
	{
		Draw(Pos.x,Pos.y,MetaFile->GetWidth(),MetaFile->GetHeight(),MetaFile);
	}

	void VL_WinDC::Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinMetaFile* MetaFile)
	{
		RECT Rect;
		Rect.left=dstX;
		Rect.top=dstY;
		Rect.right=dstX+dstW;
		Rect.bottom=dstY+dstH;
		Draw(Rect,MetaFile);
	}

	void VL_WinDC::Draw(RECT Rect , VL_WinMetaFile* MetaFile)
	{
		PlayEnhMetaFile(FHandle,MetaFile->GetHandle(),&Rect);
	}

	/*------------------------------------------------------------------------------*/

	void VL_WinDC::Draw(VInt dstX , VInt dstY , VL_WinBitmap::Ptr Bitmap)
	{
		VInt dstW=Bitmap->GetWidth();
		VInt dstH=Bitmap->GetHeight();
		VInt srcX=0;
		VInt srcY=0;
		if(!Bitmap->IsAlphaChannelBuilt())
		{
			BitBlt(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,SRCCOPY);
		}
		else
		{
			VInt srcW=dstW;
			VInt srcH=dstH;
			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=255;
			Blend.AlphaFormat=AC_SRC_ALPHA;
			AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
		}
	}

	void VL_WinDC::Draw(POINT Pos , VL_WinBitmap::Ptr Bitmap)
	{
		VInt dstX=Pos.x;
		VInt dstY=Pos.y;
		VInt dstW=Bitmap->GetWidth();
		VInt dstH=Bitmap->GetHeight();
		VInt srcX=0;
		VInt srcY=0;
		if(!Bitmap->IsAlphaChannelBuilt())
		{
			BitBlt(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,SRCCOPY);
		}
		else
		{
			VInt srcW=dstW;
			VInt srcH=dstH;
			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=255;
			Blend.AlphaFormat=AC_SRC_ALPHA;
			AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
		}
	}

	void VL_WinDC::Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinBitmap::Ptr Bitmap)
	{
		VInt srcX=0;
		VInt srcY=0;
		VInt srcW=Bitmap->GetWidth();
		VInt srcH=Bitmap->GetHeight();
		if(!Bitmap->IsAlphaChannelBuilt())
		{
			StretchBlt(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,SRCCOPY);
		}
		else
		{
			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=255;
			Blend.AlphaFormat=AC_SRC_ALPHA;
			AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
		}
	}

	void VL_WinDC::Draw(RECT Rect , VL_WinBitmap::Ptr Bitmap)
	{
		VInt dstX=Rect.left;
		VInt dstY=Rect.top;
		VInt dstW=Rect.right-Rect.left;
		VInt dstH=Rect.bottom-Rect.top;
		VInt srcX=0;
		VInt srcY=0;
		VInt srcW=Bitmap->GetWidth();
		VInt srcH=Bitmap->GetHeight();
		if(!Bitmap->IsAlphaChannelBuilt())
		{
			StretchBlt(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,SRCCOPY);
		}
		else
		{
			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=255;
			Blend.AlphaFormat=AC_SRC_ALPHA;
			AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
		}
	}

	void VL_WinDC::Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinBitmap::Ptr Bitmap , VInt srcX , VInt srcY)
	{
		if(!Bitmap->IsAlphaChannelBuilt())
		{
			BitBlt(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,SRCCOPY);
		}
		else
		{
			VInt srcW=dstW;
			VInt srcH=dstH;
			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=255;
			Blend.AlphaFormat=AC_SRC_ALPHA;
			AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
		}
	}

	void VL_WinDC::Draw(RECT Rect , VL_WinBitmap::Ptr Bitmap , POINT Pos)
	{
		VInt dstX=Rect.left;
		VInt dstY=Rect.top;
		VInt dstW=Rect.right-Rect.left;
		VInt dstH=Rect.bottom-Rect.top;
		VInt srcX=Pos.x;
		VInt srcY=Pos.y;
		if(!Bitmap->IsAlphaChannelBuilt())
		{
			BitBlt(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,SRCCOPY);
		}
		else
		{
			VInt srcW=dstW;
			VInt srcH=dstH;
			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=255;
			Blend.AlphaFormat=AC_SRC_ALPHA;
			AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
		}
	}

	void VL_WinDC::Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinBitmap::Ptr Bitmap , VInt srcX , VInt srcY , VInt srcW , VInt srcH)
	{
		if(!Bitmap->IsAlphaChannelBuilt())
		{
			StretchBlt(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,SRCCOPY);
		}
		else
		{
			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=255;
			Blend.AlphaFormat=AC_SRC_ALPHA;
			AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
		}
	}

	void VL_WinDC::Draw(RECT dstRect , VL_WinBitmap::Ptr Bitmap , RECT srcRect)
	{
		VInt dstX=dstRect.left;
		VInt dstY=dstRect.top;
		VInt dstW=dstRect.right-dstRect.left;
		VInt dstH=dstRect.bottom-dstRect.top;
		VInt srcX=srcRect.left;
		VInt srcY=srcRect.top;
		VInt srcW=srcRect.right-srcRect.left;
		VInt srcH=srcRect.bottom-srcRect.top;
		if(!Bitmap->IsAlphaChannelBuilt())
		{
			StretchBlt(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,SRCCOPY);
		}
		else
		{
			BLENDFUNCTION Blend;
			Blend.BlendOp=AC_SRC_OVER;
			Blend.BlendFlags=0;
			Blend.SourceConstantAlpha=255;
			Blend.AlphaFormat=AC_SRC_ALPHA;
			AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
		}
	}

	/*------------------------------------------------------------------------------*/

	void VL_WinDC::Draw(VInt dstX , VInt dstY , VL_WinBitmap::Ptr Bitmap , VByte Alpha)
	{
		VInt dstW=Bitmap->GetWidth();
		VInt dstH=Bitmap->GetHeight();
		VInt srcX=0;
		VInt srcY=0;
		VInt srcW=dstW;
		VInt srcH=dstH;

		BLENDFUNCTION Blend;
		Blend.BlendOp=AC_SRC_OVER;
		Blend.BlendFlags=0;
		Blend.SourceConstantAlpha=Alpha;
		Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
		AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
	}

	void VL_WinDC::Draw(POINT Pos , VL_WinBitmap::Ptr Bitmap , VByte Alpha)
	{
		VInt dstX=Pos.x;
		VInt dstY=Pos.y;
		VInt dstW=Bitmap->GetWidth();
		VInt dstH=Bitmap->GetHeight();
		VInt srcX=0;
		VInt srcY=0;
		VInt srcW=dstW;
		VInt srcH=dstH;

		BLENDFUNCTION Blend;
		Blend.BlendOp=AC_SRC_OVER;
		Blend.BlendFlags=0;
		Blend.SourceConstantAlpha=Alpha;
		Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
		AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
	}

	void VL_WinDC::Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinBitmap::Ptr Bitmap , VByte Alpha)
	{
		VInt srcX=0;
		VInt srcY=0;
		VInt srcW=Bitmap->GetWidth();
		VInt srcH=Bitmap->GetHeight();

		BLENDFUNCTION Blend;
		Blend.BlendOp=AC_SRC_OVER;
		Blend.BlendFlags=0;
		Blend.SourceConstantAlpha=Alpha;
		Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
		AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
	}

	void VL_WinDC::Draw(RECT Rect , VL_WinBitmap::Ptr Bitmap , VByte Alpha)
	{
		VInt dstX=Rect.left;
		VInt dstY=Rect.top;
		VInt dstW=Rect.right-Rect.left;
		VInt dstH=Rect.bottom-Rect.top;
		VInt srcX=0;
		VInt srcY=0;
		VInt srcW=Bitmap->GetWidth();
		VInt srcH=Bitmap->GetHeight();

		BLENDFUNCTION Blend;
		Blend.BlendOp=AC_SRC_OVER;
		Blend.BlendFlags=0;
		Blend.SourceConstantAlpha=Alpha;
		Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
		AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
	}

	void VL_WinDC::Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinBitmap::Ptr Bitmap , VInt srcX , VInt srcY , VByte Alpha)
	{
		VInt srcW=dstW;
		VInt srcH=dstH;

		BLENDFUNCTION Blend;
		Blend.BlendOp=AC_SRC_OVER;
		Blend.BlendFlags=0;
		Blend.SourceConstantAlpha=Alpha;
		Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
		AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
	}

	void VL_WinDC::Draw(RECT Rect , VL_WinBitmap::Ptr Bitmap , POINT Pos , VByte Alpha)
	{
		VInt dstX=Rect.left;
		VInt dstY=Rect.top;
		VInt dstW=Rect.right-Rect.left;
		VInt dstH=Rect.bottom-Rect.top;
		VInt srcX=Pos.x;
		VInt srcY=Pos.y;
		VInt srcW=dstW;
		VInt srcH=dstH;

		BLENDFUNCTION Blend;
		Blend.BlendOp=AC_SRC_OVER;
		Blend.BlendFlags=0;
		Blend.SourceConstantAlpha=Alpha;
		Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
		AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
	}

	void VL_WinDC::Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinBitmap::Ptr Bitmap , VInt srcX , VInt srcY , VInt srcW , VInt srcH , VByte Alpha)
	{
		BLENDFUNCTION Blend;
		Blend.BlendOp=AC_SRC_OVER;
		Blend.BlendFlags=0;
		Blend.SourceConstantAlpha=Alpha;
		Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
		AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
	}

	void VL_WinDC::Draw(RECT dstRect , VL_WinBitmap::Ptr Bitmap , RECT srcRect , VByte Alpha)
	{
		VInt dstX=dstRect.left;
		VInt dstY=dstRect.top;
		VInt dstW=dstRect.right-dstRect.left;
		VInt dstH=dstRect.bottom-dstRect.top;
		VInt srcX=srcRect.left;
		VInt srcY=srcRect.top;
		VInt srcW=srcRect.right-srcRect.left;
		VInt srcH=srcRect.bottom-srcRect.top;

		BLENDFUNCTION Blend;
		Blend.BlendOp=AC_SRC_OVER;
		Blend.BlendFlags=0;
		Blend.SourceConstantAlpha=Alpha;
		Blend.AlphaFormat=Bitmap->IsAlphaChannelBuilt()?AC_SRC_ALPHA:0;
		AlphaBlend(FHandle,dstX,dstY,dstW,dstH,Bitmap->GetWinDC()->GetHandle(),srcX,srcY,srcW,srcH,Blend);
	}

/*********************************************************************************************************
VL_WinControlDC
*********************************************************************************************************/

	VL_WinControlDC::VL_WinControlDC(HWND Handle)
	{
		FControlHandle=Handle;
		FHandle=GetDC(FControlHandle);
		Init();
	}

	VL_WinControlDC::~VL_WinControlDC()
	{
		ReleaseDC(FControlHandle,FHandle);
	}

/*********************************************************************************************************
VL_WinProxyDC
*********************************************************************************************************/

	VL_WinProxyDC::VL_WinProxyDC()
	{
		FHandle=NULL;
	}

	VL_WinProxyDC::~VL_WinProxyDC()
	{
	}

	void VL_WinProxyDC::Initialize(HDC Handle)
	{
		FHandle=Handle;
		Init();
	}

/*********************************************************************************************************
VL_WinImageDC
*********************************************************************************************************/

	VL_WinImageDC::VL_WinImageDC()
	{
		FHandle=CreateCompatibleDC(NULL);
		Init();
	}

	VL_WinImageDC::~VL_WinImageDC()
	{
		DeleteDC(FHandle);
	}

	}
}
