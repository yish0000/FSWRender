/*
 * -----------------------------------------------------------------------
 *  Name:   Vector.cpp
 *  Desc:   本文件实现了引擎所需的矢量类。
 *  Author: Yish
 *  Date:   11/12/2010
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#include "Vector.h"
#include "Matrix.h"


/** 获取矢量的长度
*/
float Vector2::GetLength(void) const
{
    return sqrtf(x*x+y*y);
}

/** 获取矢量长度的平方
*/
float Vector2::GetSqrLength(void) const
{
    return x*x+y*y;
}

/** 单位化矢量
*/
void Vector2::Normalize(void)
{
    float f = 1.0f / GetLength();
    x *= f;
    y *= f;
}

/** 获取两个矢量的点积
*/
float Vector2::DotProduct( const Vector2& v )
{
    return x*v.x+y*v.y;
}

//////////////////////////////////////////////////////////////////////////

// 三维矢量的几个常量
const Vector3 Vector3::ZERO( 0.0f,0.0f,0.0f );
const Vector3 Vector3::UNIT_X( 1.0f,0.0f,0.0f );
const Vector3 Vector3::UNIT_Y( 0.0f,1.0f,0.0f );
const Vector3 Vector3::UNIT_Z( 0.0f,0.0f,1.0f );

/** 获取矢量的长度
*/
float Vector3::GetLength(void) const
{
    return (float)sqrt( x*x + y*y + z*z );
}

/** 获取矢量的长度平方
*/
float Vector3::GetSqrLength(void) const
{
    return (x*x + y*y + z*z);
}

/** 为矢量取反
*/
void Vector3::Negate(void)
{
    x = -x;
    y = -y;
    z = -z;
}

/** 计算两个矢量之间的夹角
*/
float Vector3::AngleWith( const Vector3& v ) const
{
    return (float)acos( ((*this) * v)/(this->GetLength()*v.GetLength()) );
}

/** 单位化矢量
*/
void Vector3::Normalize(void)
{
    float len = (float)sqrt( x*x+y*y+z*z );

    if( len > 1e-08 )
    {
        float invLen = 1.0f / len;

        x *= invLen;
        y *= invLen;
        z *= invLen;
    }
}

/** 获取两个向量之间的差值
*/
void Vector3::Difference( const Vector3& v1,const Vector3& v2 )
{
    x = v2.x - v1.x;
    y = v2.y - v1.y;
    z = v2.z - v1.z;
}

/** 获取两个向量之间的叉积
*/
void Vector3::Cross( const Vector3& v1,const Vector3& v2 )
{
    x = v1.y * v2.z - v1.z * v2.y;
    y = v1.z * v2.x - v1.x * v2.z;
    z = v1.x * v2.y - v1.y * v2.x;
}

/** 获取两个向量之间的点积
*/
float Vector3::DotProduct( const Vector3& v ) const
{
    return x*v.x+y*v.y+z*v.z;
}

/** 判断两个矢量是否相等
*/
bool Vector3::operator == ( const Vector3& v ) const
{
    return (x == v.x && y == v.y && z == v.z);
}

/** 判断两个矢量是否不等
*/
bool Vector3::operator != ( const Vector3& v ) const
{
    return (x != v.x || y != v.y || z != v.z);
}

/** 将一个矢量与本身相加
*/
Vector3& Vector3::operator += ( const Vector3& v )
{
	x += v.x;
	y += v.y;
	z += v.z;

    return *this;
}

/** 取两个矢量的和
*/
Vector3 Vector3::operator + ( const Vector3& v ) const
{
	return Vector3(
        x+v.x,y+v.y,z+v.z
        );
}

/** 将本身减去一个指定的向量
*/
Vector3& Vector3::operator -= ( const Vector3& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

    return *this;
}

/** 取两个向量之间的差
*/
Vector3 Vector3::operator - ( const Vector3& v ) const
{
	return Vector3(
        x-v.x,y-v.y,z-v.z
        );
}

/** 将本身与一个浮点数相乘
*/
Vector3& Vector3::operator *= ( const float f )
{
	x *= f;
	y *= f;
	z *= f;

    return *this;
}

/** 将本身与一个浮点数相乘
*/
Vector3& Vector3::operator /= ( const float f )
{
    assert( f != 0.0f );
    float fInv = 1.0f / f;

	x *= fInv;
	y *= fInv;
	z *= fInv;

    return *this;
}

/** 计算矢量与一个浮点数的乘绩
*/
Vector3 Vector3::operator * ( const float f ) const
{
	return Vector3(
        x*f,y*f,z*f
        );
}

/** 计算矢量除以一个浮点数所得的商
*/
Vector3 Vector3::operator / ( const float f ) const
{
    assert( f != 0.0f );
    float fInv = 1.0f / f;

	return Vector3(
        x*fInv,y*fInv,z*fInv
        );
}

/** 将本身加上一个浮点数
*/
Vector3& Vector3::operator += ( const float f )
{
	x += f;
	y += f;
	z += f;

    return *this;
}

/** 将本身减去一个浮点数
*/
Vector3& Vector3::operator -= ( const float f )
{
	x -= f;
	y -= f;
	z -= f;

    return *this;
}

/** 计算矢量与一个浮点数的和
*/
Vector3 Vector3::operator + ( const float f ) const
{
	return Vector3(
        x+f,y+f,z+f
        );
}

/** 计算矢量与一个浮点数的差
*/
Vector3 Vector3::operator - ( const float f ) const
{
	return Vector3(
        x-f,y-f,z-f
        );
}

/** 计算两个矢量之间的点积
*/
float Vector3::operator * ( const Vector3& v ) const
{
	return (v.x*x + v.y*y + v.z*z);
}

/** 计算一个矢量和一个矩阵的乘绩
*/
Vector3 Vector3::operator * ( const Matrix44& m ) const
{
	Vector3 vcResult;

	vcResult.x = x*m._11 + y*m._21 + z*m._31 + m._41;
	vcResult.y = x*m._12 + y*m._22 + z*m._32 + m._42;
	vcResult.z = x*m._13 + y*m._23 + z*m._33 + m._43;

	return vcResult;
}

/** 将三维矢量转换为四维矢量
*/
Vector4 Vector3::ToVector4(void) const
{
    return Vector4( x,y,z,1.0f );
}

/** 将四维矢量转换为三维矢量
*/
Vector3 Vector4::ToVector3(void) const
{
    float f = 1.0f / w;
    return Vector3( x*f,y*f,z*f );
}