/*
 * -----------------------------------------------------------------------
 *  Name:   Vector.h
 *  Desc:   本文件定义了一个引擎所需的矢量类。
 *  Author: Yish
 *  Date:   11/12/2010
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __VECTOR_H__
#define __VECTOR_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Common.h"

//////////////////////////////////////////////////////////////////////////


class Vector2;
class Vector3;
class Vector4;
class Matrix44;

/** 二维矢量
*/
class Vector2
{
public:
    union
    {
        struct { float x,y; };
        float m[2];
    };

    // 构造函数
    Vector2(void) {};
    Vector2( float fx,float fy ) : x(fx),y(fy) {}
    Vector2( const Vector2& v ) : x(v.x),y(v.y) {}

    // 获取矢量的长度
    float GetLength(void) const;

    // 获取矢量长度的平方
    float GetSqrLength(void) const;

    // 单位化向量
    void Normalize(void);

    // 两个矢量的点积
    float DotProduct( const Vector2& v );

    Vector2& operator += ( const Vector2& v )
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2& operator -= ( const Vector2& v )
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2 operator + ( const Vector2& v ) const { return Vector2(x+v.x,y+v.y); }
    Vector2 operator - ( const Vector2& v ) const { return Vector2(x-v.x,y-v.y); }

    Vector2& operator += ( const float f )
    {
        x += f;
        y += f;
        return *this;
    }

    Vector2& operator -= ( const float f )
    {
        x -= f;
        y -= f;
        return *this;
    }

    Vector2& operator *= ( const float f )
    {
        x *= f;
        y *= f;
        return *this;
    }

    Vector2& operator /= ( const float f )
    {
        x /= f;
        y /= f;
        return *this;
    }

    Vector2 operator + ( const float f ) const { return Vector2(x+f,y+f); }
    Vector2 operator - ( const float f ) const { return Vector2(x-f,y-f); }
    Vector2 operator * ( const float f ) const { return Vector2(x*f,y*f); }
    Vector2 operator / ( const float f ) const { return Vector2(x/f,y/f); }

    bool operator == ( const Vector2& v ) const { return (x==v.x) && (y==v.y); }
    bool operator != ( const Vector2& v ) const { return (x!=v.x) || (y!=v.y); }
};

/** 三维矢量
*/
class Vector3
{
public:
    union
    {
        struct { float x,y,z; };
        float m[3];
    };

    // 构造函数
    Vector3(void) {};
    Vector3( float _x,float _y,float _z ) : x(_x),y(_y),z(_z) {}
    Vector3( const Vector3& vec ) : x(vec.x),y(vec.y),z(vec.z) {}

    // 获取矢量的长度
    float GetLength(void) const;

    // 获取矢量长度的平方
    float GetSqrLength(void) const;

    // 取矢量的反方向
    void Negate(void);

    // 单位化矢量
    void Normalize(void);

    // 获取两个矢量的夹角
    float AngleWith( const Vector3& v ) const;

    // 获取两个矢量的差值(从v1到v2)
    void Difference( const Vector3& v1,const Vector3& v2 );

    // 获取两个矢量的叉积
    void Cross( const Vector3& v1,const Vector3& v2 );

    // 获取与另一个矢量的点积
    float DotProduct( const Vector3& v ) const;

    //////////////////////////////////////////////////////////////////////
    // 重载一些运算符

    Vector3& operator += ( const Vector3& v );
    Vector3& operator -= ( const Vector3& v );
    Vector3 operator + ( const Vector3& v ) const;
    Vector3 operator - ( const Vector3& v ) const;

    Vector3& operator += ( const float f );
    Vector3& operator -= ( const float f );
    Vector3& operator *= ( const float f );
    Vector3& operator /= ( const float f );
    Vector3 operator + ( const float f ) const;
    Vector3 operator - ( const float f ) const;
    Vector3 operator * ( const float f ) const;
    Vector3 operator / ( const float f ) const;

    bool operator == ( const Vector3& v ) const;
    bool operator != ( const Vector3& v ) const;

    // 获取两个矢量之间的点积
    float operator * ( const Vector3& v ) const;

    // 项矢量与一个矩阵相乘
    Vector3 operator * ( const Matrix44& m ) const;

    // 转换为四维矢量
    Vector4 ToVector4(void) const;

    // 定义一些常用的常量
    static const Vector3 ZERO;
    static const Vector3 UNIT_X;
    static const Vector3 UNIT_Y;
    static const Vector3 UNIT_Z;
};

/** 四维矢量
*/
class Vector4
{
public:
    union
    {
        struct { float x,y,z,w; };
        float m[4];
    };

    Vector4(void) {};
    Vector4( float fx,float fy,float fz,float fw=1.0f )
        : x(fx),y(fy),z(fz),w(fw) {}
    Vector4( const Vector3& v,float fw=1.0f ) : x(v.x),y(v.y),z(v.z),w(fw) {}
    Vector4( const Vector4& v ) : x(v.x),y(v.y),z(v.z),w(v.w) {}

    Vector3 ToVector3(void) const;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_VECTOR_H__