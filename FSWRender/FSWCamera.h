/*
 * -----------------------------------------------------------------------
 *  Name:   FSWCamera.h
 *  Desc:   本文件定义了一个相机对象。
 *  Author: Yish
 *  Date:   2011/7/29
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FSW_CAMERA_H__
#define __FSW_CAMERA_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Vector.h"
#include "Matrix.h"

//////////////////////////////////////////////////////////////////////////


class FSWDevice;

/** 相机对象
*/
class FSWCamera
{
protected:
	FSWDevice* m_pDevice;		// 设备指针

    Vector3 m_vPos;             // 相机对象的位置
    Matrix44 m_mOrientation;    // 相机对象的旋转数据
    Vector3 m_vRight;           // 相机的X轴
    Vector3 m_vUp;              // 相机的Y轴 
    Vector3 m_vDir;             // 相机的Z轴

    bool m_bYawFixed;           // 竖直旋转的轴是否固定
    Vector3 m_vYawAxis;         // 竖直旋转的轴

    Matrix44 m_mViewMatrix;     // 相机矩阵
    bool m_bUpdateView;         // 是否需要更新相机矩阵

	///////////////////////////////////////////////////////////////////////
	// 投影参数

	float m_fFOV;               // 视锥的视野范围
	float m_fFarClip;           // 视锥的近平面距离
	float m_fNearClip;          // 视锥的远平面距离

	Matrix44 m_mProjection;		// 视锥的投影矩阵
	bool m_bUpdateProj;         // 需要更新投影矩阵

public:
    FSWCamera(FSWDevice* pDevice);
    ~FSWCamera(void);

    // 设置相机的位置
    void SetPosition( const Vector3& vPos );
    void SetPosition( float fX,float fY,float fZ );

    // 按照世界坐标系移动相机
    void MoveWorld( const Vector3& vec );
    void MoveWorldX( float fX );
    void MoveWorldY( float fY );
    void MoveWorldZ( float fZ );

    // 按照本地坐标系移动相机
    void MoveLocal( const Vector3& vec );
    void MoveLocalX( float fX );
    void MoveLocalY( float fY );
    void MoveLocalZ( float fZ );

    // 获取相机在世界中的位置
    const Vector3& GetPosition(void) const { return m_vPos; }

    // 获取相机的方向向量
    const Vector3& GetDirection(void) const { return m_vDir; }

    // 获取相机的上方向量
    const Vector3& GetUp(void) const { return m_vUp; }

    // 获取相机的右方向量
    const Vector3& GetRight(void) const { return m_vRight; }

    // 沿着本地X轴进行旋转
    void Pitch( float fAngle );

    // 沿着本地Y轴进行旋转
    void Yaw( float fAngle );

    // 沿着本地Z轴进行旋转
    void Roll( float fAngle );

    // 沿着指定的任意轴进行旋转(轴必须单位化)
    void Rotate( const Vector3& vAxis,float fAngle );

    // 使用四元数对相机进行旋转
    void Rotate( const Matrix44& rot );

    // 对相机对象使用固定的竖直旋转轴
    void SetYawFixed( bool bFixed,const Vector3& vYawAxis=Vector3(0.0f,1.0f,0.0f) );

    // 面向指定的点
    void LookAt( const Vector3& vec );
    void LookAt( float x,float y,float z );

    // 获取相机矩阵
    const Matrix44& GetViewMatrix(void);

	///////////////////////////////////////////////////////////////////////

	// 设置视锥的视野范围(透视投影)
	void SetFOV( float fFOV );

	// 设置视锥的近平面和远平面
	void SetViewPlanes( float fNear,float fFar );

	// 获取视锥的投影矩阵
	const Matrix44& GetProjectionMatrix();

	// 获取视锥的视野范围
	float GetFOV() const { return m_fFOV; }

	// 获取近平面的距离
	float GetNearClip() const { return m_fNearClip; }

	// 获取远平面的距离
	float GetFarClip() const { return m_fFarClip; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSW_CAMERA_H__