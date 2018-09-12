/*
 * -----------------------------------------------------------------------
 *  Name:   Plane.cpp
 *  Desc:   本文件用于实现引擎的平面类。
 *  Author: Yish
 *  Date:   11/13/2010
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#include "Plane.h"


/** 单位化平面
*/
void Plane::Normalize(void)
{
    float fL = m_vN.GetLength();

    if( fL > 0.0f )
    {
        float fInvLen = 1.0f / fL;
        m_vN *= fInvLen;
        m_fD *= fInvLen;
    }
}

/** 获取顶点到该平面的距离(绝对值)
*/
float Plane::Distance( const Vector3& pt ) const
{
    return fabs( (m_vN*pt) + m_fD );
}

/** 获取顶点到该平面的距离
*/
float Plane::DotCoord( const Vector3& pt ) const
{
    return m_vN*pt + m_fD;
}

/** 判断顶点与该平面之间的关系
*/
int Plane::Classify( const Vector3& pt ) const
{
	float f = (pt * m_vN) + m_fD;

	if( f > 0.0f ) return PLANE_FRONT;
	if( f < 0.0f ) return PLANE_BACK;

	return PLANE_ON;
}

/** 检测该平面是否与三角形相交
*/
bool Plane::Intersect( const Vector3& v0,const Vector3& v1,const Vector3& v2 ) const
{
	int n = Classify( v0 );

	if( (n == Classify(v1)) && (n == Classify(v2)) )
		return false;

	return true;
}

/** 计算平面与线段交点在线段中的位置
*/
float Plane::Intersect( const Vector3& v0,const Vector3& v1 ) const
{
    Vector3 vec = v1 - v0;
    float f = m_vN.DotProduct( vec );
    return -((m_vN.DotProduct(v0) + m_fD) / f);
}