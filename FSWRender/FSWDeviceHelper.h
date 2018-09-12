/*
 * ------------------------------------------------------------------------
 *  Name:   FSWDeviceHelper.h
 *  Desc:   本文件定义了一些图形设备的辅助函数。
 *  Author: Yish
 *  Date:   2011/7/23
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSWDEVICEHELPER_H__
#define __FSWDEVICEHELPER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "Common.h"
#include "Color.h"
#include "Matrix.h"
#include "Plane.h"

///////////////////////////////////////////////////////////////////////////


/** 对2D直线进行剪裁
*/
bool Clip2DLine( const RECT& rcClip,const POINT& from,const POINT& to,POINT& v1,POINT& v2 );

/** 将指定的世界坐标转换为屏幕坐标
*/
void WorldToScreen( const Vector3& vPos,const Matrix44& wvp,UINT w,UINT h,POINT& pt,
                    float& Z,float& W );

/** 获取视锥的六个平面
*/
void GetFrustumPlane( const Matrix44& vp,Plane planes[] );

/** 判断两个矩形是否相交
*/
bool IsRectCollided( const RECT& rc1,const RECT& rc2 );

/** 检测全屏分辨率是否有效
@remarks
	全屏支持分辨率：640x480, 800x600, 1024x768, 1152x864, 1280x720, 1280x768,
	1280x800, 1280x960, 1280x1024, 1360x768, 1440x900, 1600x900, 1680x1050
*/
bool CheckResolution(int width, int height);


///////////////////////////////////////////////////////////////////////////
//  
//  Inline Function
//  
///////////////////////////////////////////////////////////////////////////

// 获取颜色中的RED通道
inline int _GetRed( DWORD color ) { return (color & 0x00ff0000) >> 8; }

// 获取颜色中的GREEN通道
inline int _GetGreen( DWORD color ) { return (color & 0x0000ff00); }

// 获取颜色中的BLUE通道
inline int _GetBlue( DWORD color ) { return (color & 0x000000ff) << 8; }

// 获取颜色中的ALPHA通道
inline int _GetAlpha( DWORD color ) { return (color & 0xff000000) >> 16; }

// 生成颜色值（各个通道都是16位）
inline DWORD _MakeColor( int r,int g,int b,int a )
{
	return (DWORD)(((a>>8) << 24) | ((r>>8) << 16) | ((g>>8) << 8) | (b>>8));
}

/** 设置指定的像素
*/
inline void _WritePixel( DWORD* buf,UINT pitch,UINT x,UINT y,const Color& clr )
{
	buf[y*pitch+x] = clr.val;
}

// 对两种颜色进行插值
inline DWORD _Lerp( DWORD c1, DWORD c2, float f )
{
	BYTE ret[4];
	BYTE* byc1 = (BYTE*)&c1;
	BYTE* byc2 = (BYTE*)&c2;
	ret[0] = (BYTE)(byc1[0] + (byc2[0] - byc1[0]) * f);
	ret[1] = (BYTE)(byc1[1] + (byc2[1] - byc1[1]) * f);
	ret[2] = (BYTE)(byc1[2] + (byc2[2] - byc1[2]) * f);
	ret[3] = (BYTE)(byc1[3] + (byc2[3] - byc1[3]) * f);
	return (DWORD)(ret[0] | (ret[1] << 8) | (ret[2] << 16) | (ret[3] << 24));
}

// 对两种颜色进行融合
inline DWORD _Modulate( DWORD c1, DWORD c2 )
{
	DWORD ret[4];
	BYTE* bc1 = (BYTE*)&c1;
	BYTE* bc2 = (BYTE*)&c2;
	ret[0] = (bc1[0] * bc2[0]) >> 8;
	ret[1] = (bc1[1] * bc2[1]) >> 8;
	ret[2] = (bc1[2] * bc2[2]) >> 8;
	ret[3] = (bc1[3] * bc2[3]) >> 8;
	return (DWORD)((ret[3] << 24) | (ret[2] << 16) | (ret[1] << 8) | ret[0]);
}

// 对两种颜色进行Alpha混合
inline DWORD _AlphaBlend( DWORD src, DWORD dest )
{
	BYTE* srcClr = (BYTE*)&src;
	BYTE* destClr = (BYTE*)&dest;
	DWORD alpha = (src & 0xff000000) >> 24;
	destClr[3] = (BYTE)alpha;
	srcClr[0] = (BYTE)((srcClr[0] * alpha) >> 8);
	srcClr[1] = (BYTE)((srcClr[1] * alpha) >> 8);
	srcClr[2] = (BYTE)((srcClr[2] * alpha) >> 8);
	alpha = 255 - alpha;
	destClr[0] = (BYTE)((destClr[0] * alpha) >> 8);
	destClr[1] = (BYTE)((destClr[1] * alpha) >> 8);
	destClr[2] = (BYTE)((destClr[2] * alpha) >> 8);
	destClr[0] = srcClr[0] + destClr[0];
	destClr[1] = srcClr[1] + destClr[1];
	destClr[2] = srcClr[2] + destClr[2];
	return dest;
}

// 优化浮点数到整形的转换
inline int _FloatToInt( float f )
{
	int CtrlwdHolder;
	int CtrlwdSetter;
	int RetVal;

	__asm
	{
		fld     f
		fnstcw  CtrlwdHolder
		movzx   eax, CtrlwdHolder
		and     eax, 0xFFFFF3FF
		or      eax, 0x00000C00
		mov     CtrlwdSetter, eax
		fldcw   CtrlwdSetter
		fistp   RetVal
		fldcw   CtrlwdHolder
	}

	return RetVal;
}

inline DWORD _Sampler2D_Point( DWORD* pTexture, UINT w, UINT h, float tu, float tv )
{
	int x, y;
	x = _FloatToInt(tu);
	y = _FloatToInt(tv);
	x = x % w;
	y = y % h;
	if( x < 0 ) x += w;
	if( y < 0 ) y += h;
	return pTexture[y*w+x];
}

inline DWORD _Sampler2D_Bilinear( DWORD* pTexture, UINT w, UINT h, float tu, float tv )
{
	int x1, y1, x2, y2;
	DWORD c1, c2, c3, c4;
	DWORD c12, c34;
	float fx, fy;

	// 计算离目标最近的四个点
	x1 = _FloatToInt(tu);
	y1 = _FloatToInt(tv);
	fx = (x1 > 0) ? tu - x1 : x1 - tu;
	fy = (y1 > 0) ? tv - y1 : y1 - tv;
	x1 = x1 % w;
	y1 = y1 % h;
	if( x1 < 0 ) x1 += w;
	if( y1 < 0 ) y1 += h;
	x2 = x1 + 1;
	y2 = y1 + 1;

	// 如果像素在右边线
	if( x1 == w - 1 )
	{
		if( y1 == h - 1 )
		{
			// 取右下角像素
			return pTexture[y1*w+x1];
		}
		else
		{
			// 对右边线进行线性差值
			c1 = pTexture[y1 * w + x1];
			c2 = pTexture[y2 * w + x1];
			return _Lerp(c1, c2, fy);
		}
	}
	else if( y1 == h - 1 )
	{
		// 如果像素在底线上
		c1 = pTexture[y1 * w + x1];
		c2 = pTexture[y1 * w + x2];
		return _Lerp(c1, c2, fx);
	}
	else
	{
		// 像素在纹理的内部
		c1 = pTexture[y1 * w + x1];
		c2 = pTexture[y1 * w + x2];
		c3 = pTexture[y2 * w + x1];
		c4 = pTexture[y2 * w + x2];
		c12 = _Lerp(c1, c2, fx);
		c34 = _Lerp(c3, c4, fx);
		return _Lerp(c12, c34, fy);
	}
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWDEVICEHELPER_H__