/*
 * -----------------------------------------------------------------------
 *  Name:   Plane.cpp
 *  Desc:   ���ļ�����ʵ�������ƽ���ࡣ
 *  Author: Yish
 *  Date:   11/13/2010
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#include "Plane.h"


/** ��λ��ƽ��
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

/** ��ȡ���㵽��ƽ��ľ���(����ֵ)
*/
float Plane::Distance( const Vector3& pt ) const
{
    return fabs( (m_vN*pt) + m_fD );
}

/** ��ȡ���㵽��ƽ��ľ���
*/
float Plane::DotCoord( const Vector3& pt ) const
{
    return m_vN*pt + m_fD;
}

/** �ж϶������ƽ��֮��Ĺ�ϵ
*/
int Plane::Classify( const Vector3& pt ) const
{
	float f = (pt * m_vN) + m_fD;

	if( f > 0.0f ) return PLANE_FRONT;
	if( f < 0.0f ) return PLANE_BACK;

	return PLANE_ON;
}

/** ����ƽ���Ƿ����������ཻ
*/
bool Plane::Intersect( const Vector3& v0,const Vector3& v1,const Vector3& v2 ) const
{
	int n = Classify( v0 );

	if( (n == Classify(v1)) && (n == Classify(v2)) )
		return false;

	return true;
}

/** ����ƽ�����߶ν������߶��е�λ��
*/
float Plane::Intersect( const Vector3& v0,const Vector3& v1 ) const
{
    Vector3 vec = v1 - v0;
    float f = m_vN.DotProduct( vec );
    return -((m_vN.DotProduct(v0) + m_fD) / f);
}