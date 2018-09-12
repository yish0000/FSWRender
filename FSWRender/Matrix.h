/*
 * -----------------------------------------------------------------------
 *  Name:   Matrix.h
 *  Desc:   ���ļ���������������Ҫ�ľ����ࡣ
 *  Author: Yish
 *  Date:   11/12/2010
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __MATRIX_H__
#define __MATRIX_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Common.h"
#include "Vector.h"

//////////////////////////////////////////////////////////////////////////


/** 4x4����
*/
class Matrix44
{
public:
    union
    {
        struct
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };

        float m[4][4];
    };

    // ���캯��
    Matrix44(void) {};
    Matrix44( const float* ff ) { memcpy(&_11,ff,sizeof(Matrix44)); }
    Matrix44( const Matrix44& m ) { memcpy(&_11,&m,sizeof(Matrix44)); }

    // ��λ������
    void Identity(void);

    // �������ת
    void RotateX( const float f );
    void RotateY( const float f );
    void RotateZ( const float f );

    // ��ָ��������ת
    void RotateAxis( const Vector3& vAxis,float fAngle );

    // �����ƽ��
    void Translate( float dx,float dy,float dz );

    // ���������
    void Scale( float dx,float dy,float dz );
    void Scale( const Vector3& v ) { Scale(v.x, v.y, v.z); }

    // ��һ�������ת�þ���
    void TransposeOf( const Matrix44& m );

    // ��һ������������
    void InverseOf( const Matrix44& m );

    // ����һ���������
    void MakeView3D( const Vector3& vRight,const Vector3& vUp,
        const Vector3& vDir,const Vector3& vPos );

    // ����һ���������
    void LookAt( const Vector3& vPos,const Vector3& vLookAt,
        const Vector3& vWorldUp=Vector3::UNIT_Y );

    // ����һ������ͶӰ����
    void MakeOrthoProj( float l,float r,float b,float t,float n,float f );

    // ����һ��͸��ͶӰ����
    void MakePerspProj( float fov,float aspect,float n,float f );

    // ����ĳ˷�
    Matrix44 operator * ( const Matrix44& m ) const;

    // ��ʸ�����
    Vector3 operator * ( const Vector3& v ) const;
    Vector4 operator * ( const Vector4& v ) const;

    // ��ʸ�����б任
    void Transform( Vector3& v ) const;
    void Transform( Vector4& v ) const;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_MATRIX_H__