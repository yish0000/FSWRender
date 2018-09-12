/*
 * -----------------------------------------------------------------------
 *  Name:   Vector.cpp
 *  Desc:   ���ļ�ʵ�������������ʸ���ࡣ
 *  Author: Yish
 *  Date:   11/12/2010
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#include "Vector.h"
#include "Matrix.h"


/** ��ȡʸ���ĳ���
*/
float Vector2::GetLength(void) const
{
    return sqrtf(x*x+y*y);
}

/** ��ȡʸ�����ȵ�ƽ��
*/
float Vector2::GetSqrLength(void) const
{
    return x*x+y*y;
}

/** ��λ��ʸ��
*/
void Vector2::Normalize(void)
{
    float f = 1.0f / GetLength();
    x *= f;
    y *= f;
}

/** ��ȡ����ʸ���ĵ��
*/
float Vector2::DotProduct( const Vector2& v )
{
    return x*v.x+y*v.y;
}

//////////////////////////////////////////////////////////////////////////

// ��άʸ���ļ�������
const Vector3 Vector3::ZERO( 0.0f,0.0f,0.0f );
const Vector3 Vector3::UNIT_X( 1.0f,0.0f,0.0f );
const Vector3 Vector3::UNIT_Y( 0.0f,1.0f,0.0f );
const Vector3 Vector3::UNIT_Z( 0.0f,0.0f,1.0f );

/** ��ȡʸ���ĳ���
*/
float Vector3::GetLength(void) const
{
    return (float)sqrt( x*x + y*y + z*z );
}

/** ��ȡʸ���ĳ���ƽ��
*/
float Vector3::GetSqrLength(void) const
{
    return (x*x + y*y + z*z);
}

/** Ϊʸ��ȡ��
*/
void Vector3::Negate(void)
{
    x = -x;
    y = -y;
    z = -z;
}

/** ��������ʸ��֮��ļн�
*/
float Vector3::AngleWith( const Vector3& v ) const
{
    return (float)acos( ((*this) * v)/(this->GetLength()*v.GetLength()) );
}

/** ��λ��ʸ��
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

/** ��ȡ��������֮��Ĳ�ֵ
*/
void Vector3::Difference( const Vector3& v1,const Vector3& v2 )
{
    x = v2.x - v1.x;
    y = v2.y - v1.y;
    z = v2.z - v1.z;
}

/** ��ȡ��������֮��Ĳ��
*/
void Vector3::Cross( const Vector3& v1,const Vector3& v2 )
{
    x = v1.y * v2.z - v1.z * v2.y;
    y = v1.z * v2.x - v1.x * v2.z;
    z = v1.x * v2.y - v1.y * v2.x;
}

/** ��ȡ��������֮��ĵ��
*/
float Vector3::DotProduct( const Vector3& v ) const
{
    return x*v.x+y*v.y+z*v.z;
}

/** �ж�����ʸ���Ƿ����
*/
bool Vector3::operator == ( const Vector3& v ) const
{
    return (x == v.x && y == v.y && z == v.z);
}

/** �ж�����ʸ���Ƿ񲻵�
*/
bool Vector3::operator != ( const Vector3& v ) const
{
    return (x != v.x || y != v.y || z != v.z);
}

/** ��һ��ʸ���뱾�����
*/
Vector3& Vector3::operator += ( const Vector3& v )
{
	x += v.x;
	y += v.y;
	z += v.z;

    return *this;
}

/** ȡ����ʸ���ĺ�
*/
Vector3 Vector3::operator + ( const Vector3& v ) const
{
	return Vector3(
        x+v.x,y+v.y,z+v.z
        );
}

/** �������ȥһ��ָ��������
*/
Vector3& Vector3::operator -= ( const Vector3& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

    return *this;
}

/** ȡ��������֮��Ĳ�
*/
Vector3 Vector3::operator - ( const Vector3& v ) const
{
	return Vector3(
        x-v.x,y-v.y,z-v.z
        );
}

/** ��������һ�����������
*/
Vector3& Vector3::operator *= ( const float f )
{
	x *= f;
	y *= f;
	z *= f;

    return *this;
}

/** ��������һ�����������
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

/** ����ʸ����һ���������ĳ˼�
*/
Vector3 Vector3::operator * ( const float f ) const
{
	return Vector3(
        x*f,y*f,z*f
        );
}

/** ����ʸ������һ�����������õ���
*/
Vector3 Vector3::operator / ( const float f ) const
{
    assert( f != 0.0f );
    float fInv = 1.0f / f;

	return Vector3(
        x*fInv,y*fInv,z*fInv
        );
}

/** ���������һ��������
*/
Vector3& Vector3::operator += ( const float f )
{
	x += f;
	y += f;
	z += f;

    return *this;
}

/** �������ȥһ��������
*/
Vector3& Vector3::operator -= ( const float f )
{
	x -= f;
	y -= f;
	z -= f;

    return *this;
}

/** ����ʸ����һ���������ĺ�
*/
Vector3 Vector3::operator + ( const float f ) const
{
	return Vector3(
        x+f,y+f,z+f
        );
}

/** ����ʸ����һ���������Ĳ�
*/
Vector3 Vector3::operator - ( const float f ) const
{
	return Vector3(
        x-f,y-f,z-f
        );
}

/** ��������ʸ��֮��ĵ��
*/
float Vector3::operator * ( const Vector3& v ) const
{
	return (v.x*x + v.y*y + v.z*z);
}

/** ����һ��ʸ����һ������ĳ˼�
*/
Vector3 Vector3::operator * ( const Matrix44& m ) const
{
	Vector3 vcResult;

	vcResult.x = x*m._11 + y*m._21 + z*m._31 + m._41;
	vcResult.y = x*m._12 + y*m._22 + z*m._32 + m._42;
	vcResult.z = x*m._13 + y*m._23 + z*m._33 + m._43;

	return vcResult;
}

/** ����άʸ��ת��Ϊ��άʸ��
*/
Vector4 Vector3::ToVector4(void) const
{
    return Vector4( x,y,z,1.0f );
}

/** ����άʸ��ת��Ϊ��άʸ��
*/
Vector3 Vector4::ToVector3(void) const
{
    float f = 1.0f / w;
    return Vector3( x*f,y*f,z*f );
}