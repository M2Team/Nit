/*******************************************************************************
Vczh Library++ 2.0
Windows界面::Graphic Device Interface
开发者：陈梓瀚

接口：
类：
  VL_WinRegion			：图形区域
  VL_WinTransform		：坐标变换
  VL_WinBrush			：图形画刷
  VL_WinPen				：图形画笔
  VL_WinFont			：字体
  VL_WinDC				：设备上下文
  VL_WinControlDC		：控件DC
  VL_WinImageDC			：图形DC
  VL_WinProxyDC			：代理DC
  VL_WinDIB				：设备无关位图
  VL_WinMetaFileBuilder	：图形元文件编辑器
  VL_WinMetaFile		：图形元文件
  VL_WinBitmap			：位图
函数：
备注：
  VL_WinDC::PolyDraw
    Points		：点数组
	Actions		：一个说明点数组中点的作用的数组，可以是如下内容：
		PT_MOVETO	：消耗1个点执行MoveTo
		PT_LINETO	：消耗1个点执行LineTo
		PT_BEZIERTO	：消耗3个点完成Bezier曲线
		其中后两个值可以跟PT_CLOSEFIGURE进行or混合，以便让这个点跟最后一个PT_MOVETO或MoveTo的点连线组成封闭曲线
	PointCount	：点数组中点的数量
  Pen：
	Style：
		PS_SOLID、PS_DASH、PS_DOT、PS_DASHDOT、PS_DASHDOTDOT、PS_USERSTYLE(Geometric画笔有效)
	EndCap：
		PS_ENDCAP_ROUND、PS_ENDCAP_SQUARE、PS_ENDCAP_FLAT
	Join：
		PS_JOIN_BEVEL、PS_JOIN_MITER、PS_JOIN_ROUND
  Brush：
	Hatch：
		HS_BDIAGONAL、HS_CROSS、HS_DIAGCROSS、HS_FDIAGONAL、HS_HORIZONTAL、HS_VERTICAL
  Region：
	Combine：
		RGN_AND、RGN_OR、RGN_XOR、RGN_DIFF、RGN_COPY
  ImageCopy：
	Draw ROP：
		BLACKNESS、DSTINVERT、MERGECOPY、MERGEPAINT、NOTSRCCOPY、NOTSRCERASE、
		PATCOPY、PATINVERT、PATPAINT、SRCAND、SRCCOPY、SRCERASE、SRCINVERT、SRCPAINT、WHITENESS
  VL_WinDIB：
	TransformAlphaChannel()：	用于在修改非alpha图片的alpha通道后转换为alpha图片
	Generate×××()：			用于自动生成非alpha图片的alpha通道值
	被VL_WinDC的Draw绘制的时候，alpha图片和非alpha图片会有不同的效果
*******************************************************************************/

#ifndef VL_WINGDI
#define VL_WINGDI

#include "..\Data\Data\VL_Data_Basic.h"
#include "..\Data\Data\VL_Data_String.h"

#include<windows.h>

namespace vl
{
	namespace windows
	{

/*********************************************************************************************************
图形
*********************************************************************************************************/

		class VL_WinRegion : public VL_Base
		{
		public:
			typedef VL_AutoPtr<VL_WinRegion>	Ptr;

			friend VBool IsEqual(VL_WinRegion::Ptr Region1 , VL_WinRegion::Ptr Region2);
		protected:
			HRGN				FHandle;
		public:
			VL_WinRegion(VInt Left , VInt Top , VInt Right , VInt Bottom , VBool Rectangle);
			VL_WinRegion(RECT Rect , VBool Rectangle);
			VL_WinRegion(VInt Left , VInt Top , VInt Right , VInt Bottom , VInt EllipseWidth , VInt EllipseHeight);
			VL_WinRegion(POINT* Points , VInt Count , VBool Alternate);
			VL_WinRegion(VL_WinRegion::Ptr Region);
			VL_WinRegion(VL_WinRegion::Ptr Region1 , VL_WinRegion::Ptr Region2 , VInt CombineMode);
			VL_WinRegion(HRGN RegionHandle);
			~VL_WinRegion();

			HRGN				GetHandle();
			VBool				ContainPoint(POINT Point);
			VBool				ContainRect(RECT Rect);
			RECT				GetBoundRect();
			void				Move(VInt OffsetX , VInt OffsetY);
		};

		class VL_WinTransform : public VL_Base
		{
		protected:
			XFORM				FTransform;
		public:
			VL_WinTransform(XFORM Transform);
			VL_WinTransform(const VL_WinTransform& Transform);

			VL_WinTransform&	operator=(const VL_WinTransform& Transform);
			VL_WinTransform		operator*(const VL_WinTransform& Transform);
			const XFORM*		GetHandle()const;

			static VL_WinTransform		Translate(VFloat OffsetX , VFloat OffsetY);
			static VL_WinTransform		Scale(VFloat ScaleX , VFloat ScaleY);
			static VL_WinTransform		Rotate(VFloat Angle);
			static VL_WinTransform		Rotate(VFloat Cos , VFloat Sin);
			static VL_WinTransform		ReflectX();
			static VL_WinTransform		ReflectY();
			static VL_WinTransform		Reflect(VFloat VectorX , VFloat VectorY);
			static VL_WinTransform		Reflect(VFloat OriginX , VFloat OriginY , VFloat VectorX , VFloat VectorY);
			static VL_WinTransform		AxisV(VFloat Xx , VFloat Xy , VFloat Yx , VFloat Yy);
			static VL_WinTransform		AxisA(VFloat AngleX , VFloat LenX , VFloat AngleY , VFloat LenY);
		};
		
/*********************************************************************************************************
图像
*********************************************************************************************************/

		class VL_WinDC;
		class VL_WinControlDC;
		class VL_WinProxyDC;
		class VL_WinImageDC;

		class VL_WinMetaFileBuilder : public VL_Base
		{
			friend class VL_WinMetaFile;
		protected:
			VInt				FWidth;
			VInt				FHeight;
			VL_WinProxyDC*		FDC;

			void				Create(VInt Width , VInt Height);
			void				Draw(HENHMETAFILE Handle);
			void				Destroy();
		public:

			VL_WinMetaFileBuilder(VInt Width , VInt Height);
			~VL_WinMetaFileBuilder();

			void				LoadFrom(VL_WinMetaFile* File);
			void				SaveTo(VL_WinMetaFile* File);
			void				LoadFrom(VUnicodeString FileName);
			void				SaveTo(VUnicodeString FileName);
			VL_WinDC*			GetWinDC();
			VInt				GetWidth();
			VInt				GetHeight();
		};

		class VL_WinMetaFile : public VL_Base
		{
			friend class VL_WinMetaFileBuilder;
		protected:
			HENHMETAFILE		FHandle;
			VInt				FWidth;
			VInt				FHeight;
		public:
			VL_WinMetaFile(VUnicodeString FileName);
			VL_WinMetaFile(VL_WinMetaFileBuilder* Builder);
			~VL_WinMetaFile();

			HENHMETAFILE		GetHandle();
			VInt				GetWidth();
			VInt				GetHeight();
		};

		class VL_WinBitmap : public VL_Base
		{
		public:
			typedef VL_AutoPtr<VL_WinBitmap>		Ptr;
			enum BitmapBits
			{
				vbb2Bits,
				vbb24Bits,
				vbb32Bits
			};
		protected:
			BitmapBits			FBits;
			VInt				FWidth;
			VInt				FHeight;
			VL_WinImageDC*		FDC;
			HBITMAP				FHandle;
			BYTE**				FScanLines;
			VBool				FAlphaChannelBuilt;

			VInt				GetBitsFromBB(BitmapBits BB);
			VInt				GetLineBytes(VInt Width , BitmapBits BB);
			void				FillBitmapInfoHeader(VInt Width , VInt Height , BitmapBits Bits , BITMAPINFOHEADER* Header);
			HBITMAP				CreateDDB(VInt Width , VInt Height , BitmapBits Bits);
			HBITMAP				CreateDIB(VInt Width , VInt Height , BitmapBits Bits , BYTE**& ScanLines);
			void				Constructor(VInt Width , VInt Height , BitmapBits Bits , VBool DIBSections);
		public:
			VL_WinBitmap(VInt Width , VInt Height , BitmapBits Bits , VBool DIBSections);
			VL_WinBitmap(VUnicodeString FileName , VBool Use32Bits , VBool DIBSections);
			~VL_WinBitmap();

			void				SaveToFile(VUnicodeString FileName);

			VL_WinDC*			GetWinDC();
			VInt				GetWidth();
			VInt				GetHeight();
			VInt				GetLineBytes();
			BYTE**				GetScanLines();
			HBITMAP				GetBitmap();
			BitmapBits			GetBitmapBits();
			void				FillCompatibleHeader(BITMAPINFOHEADER* Header);

			VBool				CanBuildAlphaChannel();
			VBool				IsAlphaChannelBuilt();
			void				BuildAlphaChannel();
			void				GenerateTrans(COLORREF Color);
			void				GenerateAlpha(BYTE Alpha);
			void				GenerateTransAlpha(COLORREF Color , BYTE Alpha);
			void				GenerateLuminance();
			void				GenerateGrayLevel();
			void				Generate(BYTE(*Function)(COLORREF));
		};

/*********************************************************************************************************
材料
*********************************************************************************************************/

		class VL_WinBrush : public VL_Base
		{
		public:
			typedef VL_AutoPtr<VL_WinBrush>		Ptr;
		protected:
			HBRUSH				FHandle;
			VBuffer				FDIBMemory;
		public:
			VL_WinBrush(COLORREF Color);
			VL_WinBrush(VInt Hatch , COLORREF Color);
			VL_WinBrush(VL_WinBitmap::Ptr Bitmap);
			~VL_WinBrush();

			HBRUSH				GetHandle();
		};

		class VL_WinPen : public VL_Base
		{
		public:
			typedef VL_AutoPtr<VL_WinPen>		Ptr;
		protected:
			HPEN				FHandle;
			VBuffer				FDIBMemory;
		public:
			VL_WinPen(VInt Style , VInt Width , COLORREF Color);
			VL_WinPen(VInt Style , VInt EndCap , VInt Join , VInt Width , COLORREF Color);
			VL_WinPen(VInt Style , VInt EndCap , VInt Join , VInt Hatch , VInt Width , COLORREF Color);
			VL_WinPen(VL_WinBitmap::Ptr DIB , VInt Style , VInt EndCap , VInt Join , VInt Width);
			~VL_WinPen();

			HPEN				GetHandle();
		};

		class VL_WinFont : public VL_Base
		{
		public:
			typedef VL_AutoPtr<VL_WinFont>		Ptr;
		protected:
			LOGFONT				FFontInfo;
			HFONT				FHandle;
		public:
			VL_WinFont(VUnicodeString Name , VInt Height , VInt Width , VInt Escapement , VInt Orientation , VInt Weight , VBool Italic , VBool Underline , VBool StrikeOut , VBool Antialise);
			VL_WinFont(LOGFONT* FontInfo);
			~VL_WinFont();

			HFONT				GetHandle();
			LOGFONT*			GetInfo();
		};

/*********************************************************************************************************
设备上下文
*********************************************************************************************************/

		class VL_WinDC : public VL_Base
		{
		protected:
			HDC					FHandle;

			VL_WinPen::Ptr		FPen;
			VL_WinBrush::Ptr	FBrush;
			VL_WinFont::Ptr		FFont;

			HPEN				FOldPen;
			HBRUSH				FOldBrush;
			HFONT				FOldFont;

			void				Init();
		public:
			VL_WinDC();
			~VL_WinDC();

			HDC					GetHandle();

			void				SetPen(VL_WinPen::Ptr Pen);
			void				SetBrush(VL_WinBrush::Ptr Brush);
			void				SetFont(VL_WinFont::Ptr Font);
			COLORREF			GetBackColor();
			void				SetBackColor(COLORREF Color);
			COLORREF			GetTextColor();
			void				SetTextColor(COLORREF Color);
			VBool				GetBackTransparent();
			void				SetBackTransparent(VBool Transparent);
			POINT				GetBrushOrigin();
			void				SetBrushOrigin(POINT Point);

			void				DrawString(VInt X , VInt Y , VUnicodeString Text);
			void				DrawString(VInt X , VInt Y , VUnicodeString Text , VInt TabWidth , VInt TabOriginX);

			void				FillRegion(VL_WinRegion::Ptr Region);
			void				FrameRegion(VL_WinRegion::Ptr Region , VInt BlockWidth , VInt BlockHeight);

			void				MoveTo(VInt X , VInt Y);
			void				LineTo(VInt X , VInt Y);
			void				Rectangle(VInt Left , VInt Top , VInt Right , VInt Bottom);
			void				Rectangle(RECT Rect);
			void				FillRect(VInt Left , VInt Top , VInt Right , VInt Bottom);
			void				FillRect(RECT Rect);
			void				Ellipse(VInt Left , VInt Top , VInt Right , VInt Bottom);
			void				Ellipse(RECT Rect);
			void				RoundRect(VInt Left , VInt Top , VInt Right , VInt Bottom , VInt EllipseWidth , VInt EllipseHeight);
			void				RoundRect(RECT Rect , VInt EllipseWidth , VInt EllipseHeight);
			void				PolyLine(const POINT* Points , VInt Count);
			void				PolyLineTo(const POINT* Points , VInt Count);
			void				PolyGon(const POINT* Points , VInt Count);
			void				PolyBezier(const POINT* Points , VInt Count);
			void				PolyBezierTo(const POINT* Points , VInt Count);
			void				PolyDraw(const POINT* Points , const BYTE* Actions , VInt PointCount);
			void				Arc(RECT Bound , POINT Start , POINT End);
			void				Arc(VInt Left , VInt Top , VInt Right , VInt Bottom , VInt StartX , VInt StartY , VInt EndX , VInt EndY);
			void				ArcTo(RECT Bound , POINT Start , POINT End);
			void				ArcTo(VInt Left , VInt Top , VInt Right , VInt Bottom , VInt StartX , VInt StartY , VInt EndX , VInt EndY);
			void				AngleArc(VInt X , VInt Y , VInt Radius , VFloat StartAngle , VFloat SweepAngle);
			void				AngleArc(VInt X , VInt Y , VInt Radius , VDouble StartAngle , VDouble SweepAngle);
			void				Chord(RECT Bound , POINT Start , POINT End);
			void				Chord(VInt Left , VInt Top , VInt Right , VInt Bottom , VInt StartX , VInt StartY , VInt EndX , VInt EndY);
			void				Pie(RECT Bound , POINT Start , POINT End);
			void				Pie(VInt Left , VInt Top , VInt Right , VInt Bottom , VInt StartX , VInt StartY , VInt EndX , VInt EndY);
			void				GradientTriangle(TRIVERTEX* Vertices , VInt VerticesCount , GRADIENT_TRIANGLE* Triangles , VInt TriangleCount);

			void				BeginPath();
			void				EndPath();
			void				ClosePath();
			void				DiscardPath();
			void				DrawPath();
			void				FillPath();
			void				DrawAndFillPath();
			VL_WinRegion::Ptr	RegionFromPath();

			VBool				PointInClip(POINT Point);
			VBool				RectInClip(RECT Rect);
			void				ClipPath(VInt CombineMode);
			void				ClipRegion(VL_WinRegion::Ptr Region);
			void				RemoveClip();
			void				MoveClip(VInt OffsetX , VInt OffsetY);
			void				CombineClip(VL_WinRegion::Ptr Region , VInt CombineMode);
			void				IntersetClipRect(RECT Rect);
			void				ExcludeClipRect(RECT Rect);
			VL_WinRegion::Ptr	GetClipRegion();
			RECT				GetClipBoundRect();

			VL_WinTransform		GetTransform();
			void				SetTransform(const VL_WinTransform& Transform);

			void				Copy(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinDC* Source , VInt srcX , VInt srcY , DWORD DrawROP=SRCCOPY);
			void				Copy(RECT dstRect , VL_WinDC* Source , POINT srcPos , DWORD DrawROP=SRCCOPY);
			void				Copy(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinDC* Source , VInt srcX , VInt srcY ,VInt srcW , VInt srcH , DWORD DrawROP=SRCCOPY);
			void				Copy(RECT dstRect , VL_WinDC* Source , RECT srcRect , DWORD DrawROP=SRCCOPY);
			void				Copy(POINT UpperLeft , POINT UpperRight , POINT LowerLeft , VL_WinDC* Source , VInt srcX , VInt srcY , VInt srcW , VInt srcH);
			void				Copy(POINT UpperLeft , POINT UpperRight , POINT LowerLeft , VL_WinDC* Source , RECT srcRect);
			void				CopyTrans(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinDC* Source , VInt srcX , VInt srcY ,VInt srcW , VInt srcH , COLORREF Color);
			void				CopyTrans(RECT dstRect , VL_WinDC* Source , RECT srcRect , COLORREF Color);

			void				Draw(VInt dstX , VInt dstY , VL_WinMetaFile* MetaFile);
			void				Draw(POINT Pos , VL_WinMetaFile* MetaFile);
			void				Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinMetaFile* MetaFile);
			void				Draw(RECT Rect , VL_WinMetaFile* MetaFile);

			void				Draw(VInt dstX , VInt VL_WinBitmap , VL_WinBitmap::Ptr Bitmap);
			void				Draw(POINT Pos , VL_WinBitmap::Ptr Bitmap);
			void				Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinBitmap::Ptr Bitmap);
			void				Draw(RECT Rect , VL_WinBitmap::Ptr Bitmap);
			void				Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinBitmap::Ptr Bitmap , VInt srcX , VInt srcY);
			void				Draw(RECT Rect , VL_WinBitmap::Ptr Bitmap , POINT Pos);
			void				Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinBitmap::Ptr Bitmap , VInt srcX , VInt srcY , VInt srcW , VInt srcH);
			void				Draw(RECT dstRect , VL_WinBitmap::Ptr Bitmap , RECT srcRect);

			void				Draw(VInt dstX , VInt dstY , VL_WinBitmap::Ptr Bitmap , VByte Alpha);
			void				Draw(POINT Pos , VL_WinBitmap::Ptr Bitmap , VByte Alpha);
			void				Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinBitmap::Ptr Bitmap , VByte Alpha);
			void				Draw(RECT Rect , VL_WinBitmap::Ptr Bitmap , VByte Alpha);
			void				Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinBitmap::Ptr Bitmap , VInt srcX , VInt srcY , VByte Alpha);
			void				Draw(RECT Rect , VL_WinBitmap::Ptr Bitmap , POINT Pos , VByte Alpha);
			void				Draw(VInt dstX , VInt dstY , VInt dstW , VInt dstH , VL_WinBitmap::Ptr Bitmap , VInt srcX , VInt srcY , VInt srcW , VInt srcH , VByte Alpha);
			void				Draw(RECT dstRect , VL_WinBitmap::Ptr Bitmap , RECT srcRect , VByte Alpha);
		};

		class VL_WinControlDC : public VL_WinDC
		{
		protected:
			HWND				FControlHandle;
		public:
			VL_WinControlDC(HWND Handle);
			~VL_WinControlDC();
		};

		class VL_WinProxyDC : public VL_WinDC
		{
		public:
			VL_WinProxyDC();
			~VL_WinProxyDC();

			void				Initialize(HDC Handle);
		};

		class VL_WinImageDC : public VL_WinDC
		{
		public:
			VL_WinImageDC();
			~VL_WinImageDC();
		};
	}
}

#endif