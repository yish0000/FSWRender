/*
 * -----------------------------------------------------------------------
 *  Name:   FSWCamera.cpp
 *  Desc:   本文件实现了一个相机对象。
 *  Author: Yish
 *  Date:   2011/7/29
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FSWTypes.h"
#include "FSWDevice.h"
#include "FSWCamera.h"


/** 构造函数
*/
FSWCamera::FSWCamera(FSWDevice* pDevice) : m_pDevice(pDevice),
	m_vPos(0.0f,0.0f,0.0f),m_vRight(1.0f,0.0f,0.0f),
    m_vUp(0.0f,1.0f,0.0f),m_vDir(0.0f,0.0f,1.0f),m_bYawFixed(false),
	m_vYawAxis(0.0f,0.0f,0.0f),m_bUpdateView(false),m_fFOV(FSW_PI/4.0f),
	m_fNearClip(1.0f),m_fFarClip(1000.0f),m_bUpdateProj(false)
{
    m_mOrientation.Identity();
    m_mViewMatrix.Identity();
	m_mProjection.MakePerspProj(m_fFOV, 1.333333f, m_fNearClip, m_fFarClip);
}

/** 析构函数
*/
FSWCamera::~FSWCamera(void)
{
}

/** 获取当前相机观察矩阵
*/
const Matrix44& FSWCamera::GetViewMatrix(void)
{
    // 更新相机矩阵
    if( m_bUpdateView )
    {
        m_mViewMatrix.MakeView3D( m_vRight,m_vUp,m_vDir,m_vPos );
        m_bUpdateView = false;
    }

    return m_mViewMatrix;
}

/** 设置相机的位置
*/
void FSWCamera::SetPosition( const Vector3& vPos )
{
    m_vPos = vPos;
    m_bUpdateView = true;
}

/** 设置相机的位置
*/
void FSWCamera::SetPosition( float fX,float fY,float fZ )
{
    m_vPos.x = fX;
    m_vPos.y = fY;
    m_vPos.z = fZ;

    m_bUpdateView = true;
}

/** 沿着世界坐标轴移动
@Param 相机移动的向量
*/
void FSWCamera::MoveWorld( const Vector3& vec )
{
    m_vPos += vec;
    m_bUpdateView = true;
}

/** 沿着世界的X轴移动
@Param 要移动的距离
*/
void FSWCamera::MoveWorldX( float fX )
{
    m_vPos.x += fX;
    m_bUpdateView = true;
}

/** 沿着世界的Y轴移动
@Param 要移动的距离
*/
void FSWCamera::MoveWorldY( float fY )
{
    m_vPos.y += fY;
    m_bUpdateView = true;
}

/** 沿着世界的Z轴移动
@Param 要移动的距离
*/
void FSWCamera::MoveWorldZ( float fZ )
{
    m_vPos.z += fZ;
    m_bUpdateView = true;
}

/** 沿着相机的本地坐标轴移动
@Param 相机移动的向量
*/
void FSWCamera::MoveLocal( const Vector3& vec )
{
    m_vPos += m_mOrientation * vec;
    m_bUpdateView = true;
}

/** 沿着相机的本地X轴移动
@Param 要移动的大小
*/
void FSWCamera::MoveLocalX( float fX )
{
    MoveLocal( Vector3(fX,0.0f,0.0f) );
}

/** 沿着相机的本地Y轴移动
@Param 要移动的大小
*/
void FSWCamera::MoveLocalY( float fY )
{
    MoveLocal( Vector3(0.0f,fY,0.0f) );
}

/** 沿着相机的本地Z轴移动
@Param 要移动的大小
*/
void FSWCamera::MoveLocalZ( float fZ )
{
    MoveLocal( Vector3(0.0f,0.0f,fZ) );
}

/** 绕着本地的X轴进行旋转
@Param 要旋转的角度
*/
void FSWCamera::Pitch( float fAngle )
{
    Vector3 vAxis;
    vAxis = m_mOrientation * Vector3(1.0f,0.0f,0.0f);

    Rotate( vAxis,fAngle );
}

/** 绕着本地的Y轴进行旋转
@Param 要旋转的角度
*/
void FSWCamera::Yaw( float fAngle )
{
    Vector3 vAxis;

    if( m_bYawFixed )
        vAxis = m_vYawAxis;
    else
        vAxis = m_mOrientation * Vector3(0.0f,1.0f,0.0f);

    Rotate( vAxis,fAngle );
}

/** 绕着本地的Z轴进行旋转
@Param 要旋转的角度
*/
void FSWCamera::Roll( float fAngle )
{
    Vector3 vAxis;
    vAxis = m_mOrientation * Vector3(0.0f,0.0f,1.0f);

    Rotate( vAxis,fAngle );
}

/** 绕着指定的轴进行旋转
@Param 指定的坐标轴
@Param 要旋转的角度
*/
void FSWCamera::Rotate( const Vector3& vAxis,float fAngle )
{
    Matrix44 mat;
    mat.RotateAxis( vAxis,fAngle );

    Rotate( mat );
}

/** 使用四元数对相机进行旋转
@Param 旋转四元数
*/
void FSWCamera::Rotate( const Matrix44& rot )
{
    m_mOrientation = m_mOrientation * rot;

    m_vRight = m_mOrientation * Vector3( 1.0f,0.0f,0.0f );
    m_vUp = m_mOrientation * Vector3( 0.0f,1.0f,0.0f );
    m_vDir = m_mOrientation * Vector3( 0.0f,0.0f,1.0f );

    m_bUpdateView = true;
}

/** 使相机面向指定的点
*/
void FSWCamera::LookAt( float x,float y,float z )
{
    LookAt( Vector3(x,y,z) );
}

/** 使相机面向指定的点
*/
void FSWCamera::LookAt( const Vector3& vec )
{
    m_bUpdateView = true;
}

/** 为相机设置固定的竖直旋转轴
@Param 竖直旋转轴是否固定
@Param 竖直旋转轴矢量
*/
void FSWCamera::SetYawFixed( bool bFixed,const Vector3& vYawAxis )
{
    if( !bFixed )
    {
        m_bYawFixed = false;
    }
    else
    {
        m_bYawFixed = true;
        m_vYawAxis = vYawAxis;
    }
}

// 设置视锥的视野范围(透视投影)
void FSWCamera::SetFOV( float fFOV )
{
	m_fFOV = fFOV;
	m_bUpdateProj = true;
}

// 设置视锥的近平面和远平面
void FSWCamera::SetViewPlanes( float fNear,float fFar )
{
	m_fNearClip = fNear;
	m_fFarClip = fFar;
	m_bUpdateProj = true;
}

// 获取投影矩阵
const Matrix44& FSWCamera::GetProjectionMatrix()
{
	if( m_bUpdateProj )
	{
		m_mProjection.MakePerspProj(m_fFOV, m_pDevice->GetAspectRatio(), m_fNearClip, m_fFarClip);
		m_bUpdateView = false;
	}

	return m_mProjection;
}