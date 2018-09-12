/*
 * -----------------------------------------------------------------------
 *  Name:   FSWCamera.cpp
 *  Desc:   ���ļ�ʵ����һ���������
 *  Author: Yish
 *  Date:   2011/7/29
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FSWTypes.h"
#include "FSWDevice.h"
#include "FSWCamera.h"


/** ���캯��
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

/** ��������
*/
FSWCamera::~FSWCamera(void)
{
}

/** ��ȡ��ǰ����۲����
*/
const Matrix44& FSWCamera::GetViewMatrix(void)
{
    // �����������
    if( m_bUpdateView )
    {
        m_mViewMatrix.MakeView3D( m_vRight,m_vUp,m_vDir,m_vPos );
        m_bUpdateView = false;
    }

    return m_mViewMatrix;
}

/** ���������λ��
*/
void FSWCamera::SetPosition( const Vector3& vPos )
{
    m_vPos = vPos;
    m_bUpdateView = true;
}

/** ���������λ��
*/
void FSWCamera::SetPosition( float fX,float fY,float fZ )
{
    m_vPos.x = fX;
    m_vPos.y = fY;
    m_vPos.z = fZ;

    m_bUpdateView = true;
}

/** ���������������ƶ�
@Param ����ƶ�������
*/
void FSWCamera::MoveWorld( const Vector3& vec )
{
    m_vPos += vec;
    m_bUpdateView = true;
}

/** ���������X���ƶ�
@Param Ҫ�ƶ��ľ���
*/
void FSWCamera::MoveWorldX( float fX )
{
    m_vPos.x += fX;
    m_bUpdateView = true;
}

/** ���������Y���ƶ�
@Param Ҫ�ƶ��ľ���
*/
void FSWCamera::MoveWorldY( float fY )
{
    m_vPos.y += fY;
    m_bUpdateView = true;
}

/** ���������Z���ƶ�
@Param Ҫ�ƶ��ľ���
*/
void FSWCamera::MoveWorldZ( float fZ )
{
    m_vPos.z += fZ;
    m_bUpdateView = true;
}

/** ��������ı����������ƶ�
@Param ����ƶ�������
*/
void FSWCamera::MoveLocal( const Vector3& vec )
{
    m_vPos += m_mOrientation * vec;
    m_bUpdateView = true;
}

/** ��������ı���X���ƶ�
@Param Ҫ�ƶ��Ĵ�С
*/
void FSWCamera::MoveLocalX( float fX )
{
    MoveLocal( Vector3(fX,0.0f,0.0f) );
}

/** ��������ı���Y���ƶ�
@Param Ҫ�ƶ��Ĵ�С
*/
void FSWCamera::MoveLocalY( float fY )
{
    MoveLocal( Vector3(0.0f,fY,0.0f) );
}

/** ��������ı���Z���ƶ�
@Param Ҫ�ƶ��Ĵ�С
*/
void FSWCamera::MoveLocalZ( float fZ )
{
    MoveLocal( Vector3(0.0f,0.0f,fZ) );
}

/** ���ű��ص�X�������ת
@Param Ҫ��ת�ĽǶ�
*/
void FSWCamera::Pitch( float fAngle )
{
    Vector3 vAxis;
    vAxis = m_mOrientation * Vector3(1.0f,0.0f,0.0f);

    Rotate( vAxis,fAngle );
}

/** ���ű��ص�Y�������ת
@Param Ҫ��ת�ĽǶ�
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

/** ���ű��ص�Z�������ת
@Param Ҫ��ת�ĽǶ�
*/
void FSWCamera::Roll( float fAngle )
{
    Vector3 vAxis;
    vAxis = m_mOrientation * Vector3(0.0f,0.0f,1.0f);

    Rotate( vAxis,fAngle );
}

/** ����ָ�����������ת
@Param ָ����������
@Param Ҫ��ת�ĽǶ�
*/
void FSWCamera::Rotate( const Vector3& vAxis,float fAngle )
{
    Matrix44 mat;
    mat.RotateAxis( vAxis,fAngle );

    Rotate( mat );
}

/** ʹ����Ԫ�������������ת
@Param ��ת��Ԫ��
*/
void FSWCamera::Rotate( const Matrix44& rot )
{
    m_mOrientation = m_mOrientation * rot;

    m_vRight = m_mOrientation * Vector3( 1.0f,0.0f,0.0f );
    m_vUp = m_mOrientation * Vector3( 0.0f,1.0f,0.0f );
    m_vDir = m_mOrientation * Vector3( 0.0f,0.0f,1.0f );

    m_bUpdateView = true;
}

/** ʹ�������ָ���ĵ�
*/
void FSWCamera::LookAt( float x,float y,float z )
{
    LookAt( Vector3(x,y,z) );
}

/** ʹ�������ָ���ĵ�
*/
void FSWCamera::LookAt( const Vector3& vec )
{
    m_bUpdateView = true;
}

/** Ϊ������ù̶�����ֱ��ת��
@Param ��ֱ��ת���Ƿ�̶�
@Param ��ֱ��ת��ʸ��
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

// ������׶����Ұ��Χ(͸��ͶӰ)
void FSWCamera::SetFOV( float fFOV )
{
	m_fFOV = fFOV;
	m_bUpdateProj = true;
}

// ������׶�Ľ�ƽ���Զƽ��
void FSWCamera::SetViewPlanes( float fNear,float fFar )
{
	m_fNearClip = fNear;
	m_fFarClip = fFar;
	m_bUpdateProj = true;
}

// ��ȡͶӰ����
const Matrix44& FSWCamera::GetProjectionMatrix()
{
	if( m_bUpdateProj )
	{
		m_mProjection.MakePerspProj(m_fFOV, m_pDevice->GetAspectRatio(), m_fNearClip, m_fFarClip);
		m_bUpdateView = false;
	}

	return m_mProjection;
}