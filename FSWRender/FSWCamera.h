/*
 * -----------------------------------------------------------------------
 *  Name:   FSWCamera.h
 *  Desc:   ���ļ�������һ���������
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

/** �������
*/
class FSWCamera
{
protected:
	FSWDevice* m_pDevice;		// �豸ָ��

    Vector3 m_vPos;             // ��������λ��
    Matrix44 m_mOrientation;    // ����������ת����
    Vector3 m_vRight;           // �����X��
    Vector3 m_vUp;              // �����Y�� 
    Vector3 m_vDir;             // �����Z��

    bool m_bYawFixed;           // ��ֱ��ת�����Ƿ�̶�
    Vector3 m_vYawAxis;         // ��ֱ��ת����

    Matrix44 m_mViewMatrix;     // �������
    bool m_bUpdateView;         // �Ƿ���Ҫ�����������

	///////////////////////////////////////////////////////////////////////
	// ͶӰ����

	float m_fFOV;               // ��׶����Ұ��Χ
	float m_fFarClip;           // ��׶�Ľ�ƽ�����
	float m_fNearClip;          // ��׶��Զƽ�����

	Matrix44 m_mProjection;		// ��׶��ͶӰ����
	bool m_bUpdateProj;         // ��Ҫ����ͶӰ����

public:
    FSWCamera(FSWDevice* pDevice);
    ~FSWCamera(void);

    // ���������λ��
    void SetPosition( const Vector3& vPos );
    void SetPosition( float fX,float fY,float fZ );

    // ������������ϵ�ƶ����
    void MoveWorld( const Vector3& vec );
    void MoveWorldX( float fX );
    void MoveWorldY( float fY );
    void MoveWorldZ( float fZ );

    // ���ձ�������ϵ�ƶ����
    void MoveLocal( const Vector3& vec );
    void MoveLocalX( float fX );
    void MoveLocalY( float fY );
    void MoveLocalZ( float fZ );

    // ��ȡ����������е�λ��
    const Vector3& GetPosition(void) const { return m_vPos; }

    // ��ȡ����ķ�������
    const Vector3& GetDirection(void) const { return m_vDir; }

    // ��ȡ������Ϸ�����
    const Vector3& GetUp(void) const { return m_vUp; }

    // ��ȡ������ҷ�����
    const Vector3& GetRight(void) const { return m_vRight; }

    // ���ű���X�������ת
    void Pitch( float fAngle );

    // ���ű���Y�������ת
    void Yaw( float fAngle );

    // ���ű���Z�������ת
    void Roll( float fAngle );

    // ����ָ���������������ת(����뵥λ��)
    void Rotate( const Vector3& vAxis,float fAngle );

    // ʹ����Ԫ�������������ת
    void Rotate( const Matrix44& rot );

    // ���������ʹ�ù̶�����ֱ��ת��
    void SetYawFixed( bool bFixed,const Vector3& vYawAxis=Vector3(0.0f,1.0f,0.0f) );

    // ����ָ���ĵ�
    void LookAt( const Vector3& vec );
    void LookAt( float x,float y,float z );

    // ��ȡ�������
    const Matrix44& GetViewMatrix(void);

	///////////////////////////////////////////////////////////////////////

	// ������׶����Ұ��Χ(͸��ͶӰ)
	void SetFOV( float fFOV );

	// ������׶�Ľ�ƽ���Զƽ��
	void SetViewPlanes( float fNear,float fFar );

	// ��ȡ��׶��ͶӰ����
	const Matrix44& GetProjectionMatrix();

	// ��ȡ��׶����Ұ��Χ
	float GetFOV() const { return m_fFOV; }

	// ��ȡ��ƽ��ľ���
	float GetNearClip() const { return m_fNearClip; }

	// ��ȡԶƽ��ľ���
	float GetFarClip() const { return m_fFarClip; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSW_CAMERA_H__