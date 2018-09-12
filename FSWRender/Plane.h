/*
 * -----------------------------------------------------------------------
 *  Name:   Plane.h
 *  Desc:   本文件为引擎定义了一个平面类。
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

/** 平面类
*/
class Plane
{
public:
    Vector3 m_vN;   // 平面的法向量
    float m_fD;     // 到原点的距离

    // 构造函数
    Plane(void) {};
    Plane( const Vector3& vN,float fD ) : m_vN(vN),m_fD(fD) {}
    Plane( float x,float y,float z,float d ) : m_vN(x,y,z),m_fD(d) {}
    Plane( const Vector3& vN,const Vector3& vP )
    {
        m_vN = vN;
        m_vN.Normalize();
        m_fD = -(m_vN * vP);
    }

    // 由三个点确定一个平面
    Plane( const Vector3& v0,const Vector3& v1,const Vector3& v2 )
    {
        Vector3 vEdge1 = v1 - v0;
        Vector3 vEdge2 = v2 - v0;
        m_vN.Cross( vEdge1,vEdge2 );
        m_vN.Normalize();
        m_fD = -(m_vN * v0);
    }

    // 单位化该平面
    void Normalize(void);

    // 获取顶点到平面的距离（绝对值）
    float Distance( const Vector3& pt ) const;

    // 获取顶点到平面的距离
    float DotCoord( const Vector3& pt ) const;

    // 判断顶点与平面的关系
    int Classify( const Vector3& pt ) const;

    // 判断平面与三角形是否相交
    bool Intersect( const Vector3& v0,const Vector3& v1,const Vector3& v2 ) const;

    // 计算平面与线段的交点
    bool Intersect( const Vector3& v0,const Vector3& v1,Vector3& interPt ) const;

    // 计算平面与线段交点在线段中的位置
    float Intersect( const Vector3& v0,const Vector3& v1 ) const;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __PLANE_H__