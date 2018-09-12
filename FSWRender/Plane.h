/*
 * -----------------------------------------------------------------------
 *  Name:   Plane.h
 *  Desc:   ���ļ�Ϊ���涨����һ��ƽ���ࡣ
 *  Author: Yish
 *  Date:   11/13/2010
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __PLANE_H__
#define __PLANE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Common.h"
#include "Vector.h"

//////////////////////////////////////////////////////////////////////////


#define PLANE_ON    0
#define PLANE_FRONT 1
#define PLANE_BACK  2

/** ƽ����
*/
class Plane
{
public:
    Vector3 m_vN;   // ƽ��ķ�����
    float m_fD;     // ��ԭ��ľ���

    // ���캯��
    Plane(void) {};
    Plane( const Vector3& vN,float fD ) : m_vN(vN),m_fD(fD) {}
    Plane( float x,float y,float z,float d ) : m_vN(x,y,z),m_fD(d) {}
    Plane( const Vector3& vN,const Vector3& vP )
    {
        m_vN = vN;
        m_vN.Normalize();
        m_fD = -(m_vN * vP);
    }

    // ��������ȷ��һ��ƽ��
    Plane( const Vector3& v0,const Vector3& v1,const Vector3& v2 )
    {
        Vector3 vEdge1 = v1 - v0;
        Vector3 vEdge2 = v2 - v0;
        m_vN.Cross( vEdge1,vEdge2 );
        m_vN.Normalize();
        m_fD = -(m_vN * v0);
    }

    // ��λ����ƽ��
    void Normalize(void);

    // ��ȡ���㵽ƽ��ľ��루����ֵ��
    float Distance( const Vector3& pt ) const;

    // ��ȡ���㵽ƽ��ľ���
    float DotCoord( const Vector3& pt ) const;

    // �ж϶�����ƽ��Ĺ�ϵ
    int Classify( const Vector3& pt ) const;

    // �ж�ƽ�����������Ƿ��ཻ
    bool Intersect( const Vector3& v0,const Vector3& v1,const Vector3& v2 ) const;

    // ����ƽ�����߶εĽ���
    bool Intersect( const Vector3& v0,const Vector3& v1,Vector3& interPt ) const;

    // ����ƽ�����߶ν������߶��е�λ��
    float Intersect( const Vector3& v0,const Vector3& v1 ) const;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __PLANE_H__