/*
 * -----------------------------------------------------------------------
 *  Name:   Matrix.h
 *  Desc:   本文件定义了引擎所需要的矩阵类。
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


/** 4x4矩阵
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

    // 构造函数
    Matrix44(void) {};
    Matrix44( const float* ff ) { memcpy(&_11,ff,sizeof(Matrix44)); }
    Matrix44( const Matrix44& m ) { memcpy(&_11,&m,sizeof(Matrix44)); }

    // 单位化矩阵
    void Identity(void);

    // 矩阵的旋转
    void RotateX( const float f );
    void RotateY( const float f );
    void RotateZ( const float f );

    // 按指定的轴旋转
    void RotateAxis( const Vector3& vAxis,float fAngle );

    // 矩阵的平移
    void Translate( float dx,float dy,float dz );

    // 矩阵的缩放
    void Scale( float dx,float dy,float dz );
    void Scale( const Vector3& v ) { Scale(v.x, v.y, v.z); }

    // 求一个矩阵的转置矩阵
    void TransposeOf( const Matrix44& m );

    // 求一个矩阵的逆矩阵
    void InverseOf( const Matrix44& m );

    // 产生一个相机矩阵
    void MakeView3D( const Vector3& vRight,const Vector3& vUp,
        const Vector3& vDir,const Vector3& vPos );

    // 产生一个相机矩阵
    void LookAt( const Vector3& vPos,const Vector3& vLookAt,
        const Vector3& vWorldUp=Vector3::UNIT_Y );

    // 生成一个正交投影矩阵
    void MakeOrthoProj( float l,float r,float b,float t,float n,float f );

    // 生成一个透视投影矩阵
    void MakePerspProj( float fov,float aspect,float n,float f );

    // 矩阵的乘法
    Matrix44 operator * ( const Matrix44& m ) const;

    // 与矢量相乘
    Vector3 operator * ( const Vector3& v ) const;
    Vector4 operator * ( const Vector4& v ) const;

    // 对矢量进行变换
    void Transform( Vector3& v ) const;
    void Transform( Vector4& v ) const;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_MATRIX_H__