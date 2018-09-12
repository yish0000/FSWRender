/*
 * -----------------------------------------------------------------------
 *  Name:   Vector.h
 *  Desc:   ���ļ�������һ�����������ʸ���ࡣ
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

/** ��άʸ��
*/
class Vector2
{
public:
    union
    {
        struct { float x,y; };
        float m[2];
    };

    // ���캯��
    Vector2(void) {};
    Vector2( float fx,float fy ) : x(fx),y(fy) {}
    Vector2( const Vector2& v ) : x(v.x),y(v.y) {}

    // ��ȡʸ���ĳ���
    float GetLength(void) const;

    // ��ȡʸ�����ȵ�ƽ��
    float GetSqrLength(void) const;

    // ��λ������
    void Normalize(void);

    // ����ʸ���ĵ��
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

/** ��άʸ��
*/
class Vector3
{
public:
    union
    {
        struct { float x,y,z; };
        float m[3];
    };

    // ���캯��
    Vector3(void) {};
    Vector3( float _x,float _y,float _z ) : x(_x),y(_y),z(_z) {}
    Vector3( const Vector3& vec ) : x(vec.x),y(vec.y),z(vec.z) {}

    // ��ȡʸ���ĳ���
    float GetLength(void) const;

    // ��ȡʸ�����ȵ�ƽ��
    float GetSqrLength(void) const;

    // ȡʸ���ķ�����
    void Negate(void);

    // ��λ��ʸ��
    void Normalize(void);

    // ��ȡ����ʸ���ļн�
    float AngleWith( const Vector3& v ) const;

    // ��ȡ����ʸ���Ĳ�ֵ(��v1��v2)
    void Difference( const Vector3& v1,const Vector3& v2 );

    // ��ȡ����ʸ���Ĳ��
    void Cross( const Vector3& v1,const Vector3& v2 );

    // ��ȡ����һ��ʸ���ĵ��
    float DotProduct( const Vector3& v ) const;

    //////////////////////////////////////////////////////////////////////
    // ����һЩ�����

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

    // ��ȡ����ʸ��֮��ĵ��
    float operator * ( const Vector3& v ) const;

    // ��ʸ����һ���������
    Vector3 operator * ( const Matrix44& m ) const;

    // ת��Ϊ��άʸ��
    Vector4 ToVector4(void) const;

    // ����һЩ���õĳ���
    static const Vector3 ZERO;
    static const Vector3 UNIT_X;
    static const Vector3 UNIT_Y;
    static const Vector3 UNIT_Z;
};

/** ��άʸ��
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