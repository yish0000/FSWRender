/*
 * -----------------------------------------------------------------------
 *  Name:   Matrix.cpp
 *  Desc:   本文件用于实现引擎的矩阵类。
 *  Author: Yish
 *  Date:   11/12/2010
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#include "Matrix.h"


/** 单位化矩阵
*/
void Matrix44::Identity(void)
{
    memset( &_11,0,sizeof(Matrix44) );
    _11 = _22 = _33 = _44 = 1.0f;
}

/** 绕X轴旋转
*/
void Matrix44::RotateX( const float f )
{
	float fCos = cosf(f);
	float fSin = sinf(f);

	_22 =  fCos;
	_23 =  fSin;
	_32 = -fSin;
	_33 =  fCos;

	_11 = _44 = 1.0f;
	_12 = _13 = _14 = _21 = _24 = _31 = _34 = _41 = _42 = _43 = 0.0f;
}

/** 绕Y轴旋转
*/
void Matrix44::RotateY( const float f )
{
	float fCos = cosf(f);
	float fSin = sinf(f);

	_11 =  fCos;
	_13 = -fSin;
	_31 =  fSin;
	_33 =  fCos;

	_22 = _44 = 1.0f;
	_12 = _23 = _14 = _21 = _24 = _32 = _34 = _41 = _42 = _43 = 0.0f;
}

/** 绕Z轴旋转
*/
void Matrix44::RotateZ( const float f )
{
	float fCos = cosf(f);
	float fSin = sinf(f);

	_11  =  fCos;
	_12  =  fSin;
	_21  = -fSin;
	_22  =  fCos;

	_33 = _44 = 1.0f;
	_13 = _14 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0.0f;
}

/** 按照指定的轴旋转
*/
void Matrix44::RotateAxis( const Vector3& vAxis,float fAngle )
{
    float fCos = cosf(fAngle);
    float fSin = sinf(fAngle);
    float fSum = 1.0f - fCos;

    _11 = (vAxis.x * vAxis.x) * fSum + fCos;
    _12 = (vAxis.y * vAxis.x) * fSum + (vAxis.z * fSin);
    _13 = (vAxis.z * vAxis.x) * fSum - (vAxis.y * fSin);

    _21 = (vAxis.x * vAxis.y) * fSum - (vAxis.z * fSin);
    _22 = (vAxis.y * vAxis.y) * fSum + fCos ;
    _23 = (vAxis.z * vAxis.y) * fSum + (vAxis.x * fSin);

    _31 = (vAxis.x * vAxis.z) * fSum + (vAxis.y * fSin);
    _32 = (vAxis.y * vAxis.z) * fSum - (vAxis.x * fSin);
    _33 = (vAxis.z * vAxis.z) * fSum + fCos;

    _14 = _24 = _34 = _41 = _42 = _43 = 0.0f;
    _44 = 1.0f;
}

/** 平移一个矩阵
*/
void Matrix44::Translate( float dx,float dy,float dz )
{
    Identity();

    _41 = dx;
    _42 = dy;
    _43 = dz;
}

/** 缩放矩阵
*/
void Matrix44::Scale( float dx,float dy,float dz )
{
    Identity();

    _11 = dx;
    _22 = dy;
    _33 = dz;
}

/** 产生一个相机矩阵
*/
void Matrix44::LookAt( const Vector3& vPos,const Vector3& vLookAt,
                        const Vector3& vWorldUp/* =Vector3::UNIT_Y */ )
{
    Vector3 xAxis,yAxis,zAxis;
    zAxis = vLookAt - vPos;
    zAxis.Normalize();
    xAxis.Cross( vWorldUp,zAxis );
    xAxis.Normalize();
    yAxis.Cross( zAxis,xAxis );

    _14 = _24 = _34 = 0.0f;
    _44 = 1.0f;

    _11 = xAxis.x;
    _21 = xAxis.y;
    _31 = xAxis.z;
    _41 = -(xAxis * vPos);

    _12 = yAxis.x;
    _22 = yAxis.y;
    _32 = yAxis.z;
    _42 = -(yAxis * vPos);

    _13 = zAxis.x;
    _23 = zAxis.y;
    _33 = zAxis.z;
    _43 = -(zAxis * vPos);
}

/** 产生一个相机矩阵
*/
void Matrix44::MakeView3D( const Vector3& vRight,const Vector3& vUp,
                           const Vector3& vDir,const Vector3& vPos )
{
    this->_14 = this->_24 = this->_34 = 0.0f;
    this->_44 = 1.0f;

    this->_11 = vRight.x;
    this->_21 = vRight.y;
    this->_31 = vRight.z;
    this->_41 = -(vRight * vPos);

    this->_12 = vUp.x;
    this->_22 = vUp.y;
    this->_32 = vUp.z;
    this->_42 = -(vUp * vPos);

    this->_13 = vDir.x;
    this->_23 = vDir.y;
    this->_33 = vDir.z;
    this->_43 = -(vDir * vPos);
}

/** 求一个矩阵的转置矩阵
*/
void Matrix44::TransposeOf( const Matrix44& m )
{
    _11 = m._11;
    _21 = m._12;
    _31 = m._13;
    _41 = m._14;

    _12 = m._21;
    _22 = m._22;
    _32 = m._23;
    _42 = m._24;

    _13 = m._31;
    _23 = m._32;
    _33 = m._33;
    _43 = m._34;

    _14 = m._41;
    _24 = m._42;
    _34 = m._43;
    _44 = m._44;
}

/** 求一个矩阵的逆矩阵
*/
void Matrix44::InverseOf( const Matrix44& m )
{
    Matrix44 mTrans;
    float fTemp[12],fDet;

    mTrans.TransposeOf( m );

    fTemp[ 0]  = mTrans._33 * mTrans._44;
    fTemp[ 1]  = mTrans._34 * mTrans._43;
    fTemp[ 2]  = mTrans._32 * mTrans._44;
    fTemp[ 3]  = mTrans._34 * mTrans._42;
    fTemp[ 4]  = mTrans._32 * mTrans._43;
    fTemp[ 5]  = mTrans._33 * mTrans._42;
    fTemp[ 6]  = mTrans._31 * mTrans._44;
    fTemp[ 7]  = mTrans._34 * mTrans._41;
    fTemp[ 8]  = mTrans._31 * mTrans._43;
    fTemp[ 9]  = mTrans._33 * mTrans._41;
    fTemp[10]  = mTrans._31 * mTrans._42;
    fTemp[11]  = mTrans._32 * mTrans._41;

    this->_11  = fTemp[0]*mTrans._22 + fTemp[3]*mTrans._23 + fTemp[4] *mTrans._24;
    this->_11 -= fTemp[1]*mTrans._22 + fTemp[2]*mTrans._23 + fTemp[5] *mTrans._24;
    this->_12  = fTemp[1]*mTrans._21 + fTemp[6]*mTrans._23 + fTemp[9] *mTrans._24;
    this->_12 -= fTemp[0]*mTrans._21 + fTemp[7]*mTrans._23 + fTemp[8] *mTrans._24;
    this->_13  = fTemp[2]*mTrans._21 + fTemp[7]*mTrans._22 + fTemp[10]*mTrans._24;
    this->_13 -= fTemp[3]*mTrans._21 + fTemp[6]*mTrans._22 + fTemp[11]*mTrans._24;
    this->_14  = fTemp[5]*mTrans._21 + fTemp[8]*mTrans._22 + fTemp[11]*mTrans._23;
    this->_14 -= fTemp[4]*mTrans._21 + fTemp[9]*mTrans._22 + fTemp[10]*mTrans._23;
    this->_21  = fTemp[1]*mTrans._12 + fTemp[2]*mTrans._13 + fTemp[5] *mTrans._14;
    this->_21 -= fTemp[0]*mTrans._12 + fTemp[3]*mTrans._13 + fTemp[4] *mTrans._14;
    this->_22  = fTemp[0]*mTrans._11 + fTemp[7]*mTrans._13 + fTemp[8] *mTrans._14;
    this->_22 -= fTemp[1]*mTrans._11 + fTemp[6]*mTrans._13 + fTemp[9] *mTrans._14;
    this->_23  = fTemp[3]*mTrans._11 + fTemp[6]*mTrans._12 + fTemp[11]*mTrans._14;
    this->_23 -= fTemp[2]*mTrans._11 + fTemp[7]*mTrans._12 + fTemp[10]*mTrans._14;
    this->_24  = fTemp[4]*mTrans._11 + fTemp[9]*mTrans._12 + fTemp[10]*mTrans._13;
    this->_24 -= fTemp[5]*mTrans._11 + fTemp[8]*mTrans._12 + fTemp[11]*mTrans._13;

    fTemp[ 0]  = mTrans._13 * mTrans._24;
    fTemp[ 1]  = mTrans._14 * mTrans._23;
    fTemp[ 2]  = mTrans._12 * mTrans._24;
    fTemp[ 3]  = mTrans._14 * mTrans._22;
    fTemp[ 4]  = mTrans._12 * mTrans._23;
    fTemp[ 5]  = mTrans._13 * mTrans._22;
    fTemp[ 6]  = mTrans._11 * mTrans._24;
    fTemp[ 7]  = mTrans._14 * mTrans._21;
    fTemp[ 8]  = mTrans._11 * mTrans._23;
    fTemp[ 9]  = mTrans._13 * mTrans._21;
    fTemp[10]  = mTrans._11 * mTrans._22;
    fTemp[11]  = mTrans._12 * mTrans._21;

    this->_31  = fTemp[0] *mTrans._42 + fTemp[3] *mTrans._43 + fTemp[4] *mTrans._44;
    this->_31 -= fTemp[1] *mTrans._42 + fTemp[2] *mTrans._43 + fTemp[5] *mTrans._44;
    this->_32  = fTemp[1] *mTrans._41 + fTemp[6] *mTrans._43 + fTemp[9] *mTrans._44;
    this->_32 -= fTemp[0] *mTrans._41 + fTemp[7] *mTrans._43 + fTemp[8] *mTrans._44;
    this->_33  = fTemp[2] *mTrans._41 + fTemp[7] *mTrans._42 + fTemp[10]*mTrans._44;
    this->_33 -= fTemp[3] *mTrans._41 + fTemp[6] *mTrans._42 + fTemp[11]*mTrans._44;
    this->_34  = fTemp[5] *mTrans._41 + fTemp[8] *mTrans._42 + fTemp[11]*mTrans._43;
    this->_34 -= fTemp[4] *mTrans._41 + fTemp[9] *mTrans._42 + fTemp[10]*mTrans._43;
    this->_41  = fTemp[2] *mTrans._33 + fTemp[5] *mTrans._34 + fTemp[1] *mTrans._32;
    this->_41 -= fTemp[4] *mTrans._34 + fTemp[0] *mTrans._32 + fTemp[3] *mTrans._33;
    this->_42  = fTemp[8] *mTrans._34 + fTemp[0] *mTrans._31 + fTemp[7] *mTrans._33;
    this->_42 -= fTemp[6] *mTrans._33 + fTemp[9] *mTrans._34 + fTemp[1] *mTrans._31;
    this->_43  = fTemp[6] *mTrans._32 + fTemp[11]*mTrans._34 + fTemp[3] *mTrans._31;
    this->_43 -= fTemp[10]*mTrans._34 + fTemp[2] *mTrans._31 + fTemp[7] *mTrans._32;
    this->_44  = fTemp[10]*mTrans._33 + fTemp[4] *mTrans._31 + fTemp[9] *mTrans._32;
    this->_44 -= fTemp[8] *mTrans._32 + fTemp[11]*mTrans._33 + fTemp[5] *mTrans._31;

    fDet = mTrans._11*this->_11 + 
        mTrans._12*this->_12 + 
        mTrans._13*this->_13 +
        mTrans._14*this->_14;

    fDet = 1 / fDet;

    this->_11 *= fDet;
    this->_12 *= fDet;
    this->_13 *= fDet;
    this->_14 *= fDet;

    this->_21 *= fDet;
    this->_22 *= fDet;
    this->_23 *= fDet;
    this->_24 *= fDet;

    this->_31 *= fDet;
    this->_32 *= fDet;
    this->_33 *= fDet;
    this->_34 *= fDet;

    this->_41 *= fDet;
    this->_42 *= fDet;
    this->_43 *= fDet;
    this->_44 *= fDet;
}

/** 生成一个正交投影矩阵
@Param 左坐标
@Param 右坐标
@Param 底坐标
@Param 顶坐标
@Param 近坐标
@Param 远坐标
*/
void Matrix44::MakeOrthoProj( float l,float r,float b,float t,float n,float f )
{
    float x	= 2.0f / (r - l);
    float y = 2.0f / (t - b);
    float z = 2.0f / (f - n);
    float tx = -(r+l) / (r-l);
    float ty = -(t+b) / (t-b);
    float tz = -(f+n) / (f-n);

    memset( &_11,0,sizeof(Matrix44) );
    this->_11 = x;
    this->_22 = y;
    this->_33 = z;
    this->_44 = 1.0f;
    this->_41 = tx;
    this->_42 = ty;
    this->_43 = tz;
}

/** 生成一个透视投影矩阵
@Param 视野范围
@Param 屏幕的宽高比
@Param 近平面的距离
@Param 远平面的距离
*/
void Matrix44::MakePerspProj( float fov,float aspect,float n,float f )
{
    assert( f > n );

    float h = cosf(fov/2.0f) / sinf(fov/2.0f);
    float w = h / aspect;
    float q = f / (f - n);

    memset( &_11,0,sizeof(Matrix44) );
    this->_11 = w;
    this->_22 = h;
    this->_33 = q;
    this->_34 = 1.0f;
    this->_43 = -q * n;
}

/** 求两个矩阵的乘积
*/
Matrix44 Matrix44::operator * ( const Matrix44& m ) const
{
    Matrix44 mRet;

    mRet._11 = _11*m._11 + _12*m._21 + _13*m._31 + _14*m._41;
    mRet._12 = _11*m._12 + _12*m._22 + _13*m._32 + _14*m._42;
    mRet._13 = _11*m._13 + _12*m._23 + _13*m._33 + _14*m._43;
    mRet._14 = _11*m._14 + _12*m._24 + _13*m._34 + _14*m._44;

    mRet._21 = _21*m._11 + _22*m._21 + _23*m._31 + _24*m._41;
    mRet._22 = _21*m._12 + _22*m._22 + _23*m._32 + _24*m._42;
    mRet._23 = _21*m._13 + _22*m._23 + _23*m._33 + _24*m._43;
    mRet._24 = _21*m._14 + _22*m._24 + _23*m._34 + _24*m._44;

    mRet._31 = _31*m._11 + _32*m._21 + _33*m._31 + _34*m._41;
    mRet._32 = _31*m._12 + _32*m._22 + _33*m._32 + _34*m._42;
    mRet._33 = _31*m._13 + _32*m._23 + _33*m._33 + _34*m._43;
    mRet._34 = _31*m._14 + _32*m._24 + _33*m._34 + _34*m._44;

    mRet._41 = _41*m._11 + _42*m._21 + _43*m._31 + _44*m._41;
    mRet._42 = _41*m._12 + _42*m._22 + _43*m._32 + _44*m._42;
    mRet._43 = _41*m._13 + _42*m._23 + _43*m._33 + _44*m._43;
    mRet._44 = _41*m._14 + _42*m._24 + _43*m._34 + _44*m._44;

    return mRet;
}

/** 求一个顶点和一个矩阵的乘绩
*/
Vector3 Matrix44::operator * ( const Vector3& v ) const
{
    return Vector3(
        v.x*_11 + v.y*_21 + v.z*_31 + _41,
        v.x*_12 + v.y*_22 + v.z*_32 + _42,
        v.x*_13 + v.y*_23 + v.z*_33 + _43
        );
}

/** 变换四维矢量
*/
Vector4 Matrix44::operator * ( const Vector4& v ) const
{
    return Vector4(
        v.x*_11 + v.y*_21 + v.z*_31 + v.w*_41,
        v.x*_12 + v.y*_22 + v.z*_32 + v.w*_42,
        v.x*_13 + v.y*_23 + v.z*_33 + v.w*_43,
        v.x*_14 + v.y*_24 + v.z*_34 + v.w*_44
        );
}

/** 变换三维矢量
*/
void Matrix44::Transform( Vector3& v ) const
{
    Vector3 vResult;
    vResult.x = v.x*_11 + v.y*_21 + v.z*_31 + _41;
    vResult.y = v.x*_12 + v.y*_22 + v.z*_32 + _42;
    vResult.z = v.x*_13 + v.y*_23 + v.z*_33 + _43;
    v = vResult;
}

/** 变换四维矢量
*/
void Matrix44::Transform( Vector4& v ) const
{
    Vector4 vResult;
    vResult.x = v.x*_11 + v.y*_21 + v.z*_31 + v.w*_41;
    vResult.y = v.x*_12 + v.y*_22 + v.z*_32 + v.w*_42;
    vResult.z = v.x*_13 + v.y*_23 + v.z*_33 + v.w*_43;
    vResult.w = v.x*_14 + v.y*_24 + v.z*_34 + v.w*_44;
    v = vResult;
}