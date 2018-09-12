/*
 * -----------------------------------------------------------------------
 *  Name:   FSWDeviceHelper.cpp
 *  Desc:   本文件定义了一些图形设备的辅助函数。
 *  Author: Yish
 *  Date:   2011/7/23
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FSWDeviceHelper.h"


#define CLIP_NONE       0
#define CLIP_LEFT       1
#define CLIP_TOP        2
#define CLIP_RIGHT      4
#define CLIP_BOTTOM     8

/** 获取指定点与剪裁矩形的位置关系
*/
static UINT _GetClipCode( const RECT& rcClip,const POINT& pt )
{
    UINT code = CLIP_NONE;

    if( pt.x < rcClip.left )
        code |= CLIP_LEFT;
    else if( pt.x > rcClip.right )
        code |= CLIP_RIGHT;

    if( pt.y < rcClip.top )
        code |= CLIP_TOP;
    else if( pt.y > rcClip.bottom )
        code |= CLIP_BOTTOM;

    return code;
}

/** 剪裁指定的二维线段
@remarks
    如果线段完全不可见则返回false
*/
bool Clip2DLine( const RECT& rcClip,const POINT& from,const POINT& to,
                 POINT& v1,POINT& v2 )
{
    UINT code,code1,code2;

    v1 = from;
    v2 = to;

    code1 = _GetClipCode( rcClip,v1 );
    code2 = _GetClipCode( rcClip,v2 );

    while( code1 | code2 )
    {
        int x = 0;
        int y = 0;

        if( code1 & code2 )
            return false;

        if( code1 )
            code = code1;
        else
            code = code2;

        if( code & CLIP_LEFT )
        {
            x = rcClip.left;
            y = v1.y + (x - v1.x) * (v2.y - v1.y) / (v2.x - v1.x);
        }
        else if( code & CLIP_RIGHT )
        {
            x = rcClip.right;
            y = v1.y + (x - v1.x) * (v2.y - v1.y) / (v2.x - v1.x);
        }
        else if( code & CLIP_TOP )
        {
            y = rcClip.top;
            x = v1.x + (y - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
        }
        else if( code & CLIP_BOTTOM )
        {
            y = rcClip.bottom;
            x = v1.x + (y - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
        }

        if( code == code1 )
        {
            v1.x = x;
            v1.y = y;
            code1 = _GetClipCode( rcClip,v1 );
        }
        else
        {
            v2.x = x;
            v2.y = y;
            code2 = _GetClipCode( rcClip,v2 );
        }
    }

    return true;
}

/** 将指定的世界坐标转换为屏幕坐标
*/
void WorldToScreen( const Vector3& vPos,const Matrix44& wvp,UINT w,UINT h,POINT& pt,float& Z,float& W )
{
    Vector4 vec(vPos);
    wvp.Transform( vec );
    float f = 1.0f / vec.w;
    vec.x *= f;
    vec.y *= -f;
    vec.x = (vec.x + 1.0f) * 0.5f;
    vec.y = (vec.y + 1.0f) * 0.5f;
    pt.x = (LONG)(vec.x * w);
    pt.y = (LONG)(vec.y * h);
    Z = vec.z * f;
    W = vec.w;
}

/** 获取视锥的六个平面
*/
void GetFrustumPlane( const Matrix44& vp,Plane planes[] )
{
    // 左平面
    planes[0].m_vN.x = ( vp._11 + vp._14 );
    planes[0].m_vN.y = ( vp._21 + vp._24 );
    planes[0].m_vN.z = ( vp._31 + vp._34 );
    planes[0].m_fD   = ( vp._41 + vp._44 );
    planes[0].Normalize();

    // 右平面
    planes[1].m_vN.x = ( vp._14 - vp._11 );
    planes[1].m_vN.y = ( vp._24 - vp._21 );
    planes[1].m_vN.z = ( vp._34 - vp._31 );
    planes[1].m_fD   = ( vp._44 - vp._41 );
    planes[1].Normalize();

    // 顶平面
    planes[2].m_vN.x = ( vp._14 - vp._12 );
    planes[2].m_vN.y = ( vp._24 - vp._22 );
    planes[2].m_vN.z = ( vp._34 - vp._32 );
    planes[2].m_fD   = ( vp._44 - vp._42 );
    planes[2].Normalize();

    // 底平面
    planes[3].m_vN.x = ( vp._12 + vp._14 );
    planes[3].m_vN.y = ( vp._22 + vp._24 );
    planes[3].m_vN.z = ( vp._32 + vp._34 );
    planes[3].m_fD   = ( vp._42 + vp._44 );
    planes[3].Normalize();

    // 近平面
    planes[4].m_vN.x = vp._13;
    planes[4].m_vN.y = vp._23;
    planes[4].m_vN.z = vp._33;
    planes[4].m_fD   = vp._43;
    planes[4].Normalize();

    // 远平面
    planes[5].m_vN.x = ( vp._14 - vp._13 );
    planes[5].m_vN.y = ( vp._24 - vp._23 );
    planes[5].m_vN.z = ( vp._34 - vp._33 );
    planes[5].m_fD   = ( vp._44 - vp._43 );
    planes[5].Normalize();
}

/** 判断两个矩形是否相交
*/
bool IsRectCollided( const RECT& rc1,const RECT& rc2 )
{
    return (rc1.left < rc2.right && rc1.top < rc2.bottom &&
        rc1.right > rc2.left && rc1.bottom > rc2.top);
}

/** 检测全屏分辨率是否支持
*/
bool CheckResolution(int width, int height)
{
	const union {
		struct { short width, height; };
		int val;
	} RESOLUTION[] = {
		{ 640, 480 }, { 800, 600 }, { 1024, 768 }, { 1152, 864 }, { 1280, 720 },
		{ 1280, 768 }, { 1280, 800 }, { 1280, 960 }, { 1280, 1024 }, { 1360, 768 },
		{ 1366, 768 }, { 1440, 900 }, { 1600, 900 }, { 1680, 1050 }
	};

	bool bFound = false;
	int res_val = (height << 16) | width;
	for( int i=0;i<sizeof(RESOLUTION)/sizeof(int);i++ )
	{
		if( RESOLUTION[i].val == res_val )
		{
			bFound = true;
			break;
		}
	}

	return bFound;
}