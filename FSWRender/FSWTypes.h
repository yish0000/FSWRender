/*
 * -----------------------------------------------------------------------
 *  Name:   FSWTypes.h
 *  Desc:   ���ļ�������ͼ���豸�����һЩ���͡�
 *  Author: Yish
 *  Date:   2011/7/24
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FSWTYPES_H__
#define __FSWTYPES_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Common.h"
#include "Vector.h"
#include "Matrix.h"
#include "Color.h"

//////////////////////////////////////////////////////////////////////////


// ����һЩ����
const float FSW_PI = 3.14159265358979f;

//////////////////////////////////////////////////////////////////////////
// �����ö������

enum EPixelFormat
{
    FPF_A8R8G8B8,       // 32λ��ɫ������
    FPF_D32,            // 32λ��Ȼ�����
};

enum EPrimitiveType
{
    FPT_LINELIST,       // �߱�
    FPT_TRIANGLELIST,   // �����α�
};

enum ETransformType
{
    FTS_WORLD,          // ����任����
    FTS_VIEW,           // �۲�任����
    FTS_PROJECTION,     // ͶӰ�任����
    FTS_TRANSFORM,      // ��Ϻ�ľ���
    FTS_COUNT,
};

enum EShadeMode
{
    FSM_FLAT,           // ƽ����ɫ
    FSM_GOURAUD,        // ���޵���ɫ
};

enum ECullMode
{
    FCM_NONE,           // �����б����޳�
    FCM_CW,             // ˳ʱ�뱳���޳�
    FCM_CCW,            // ��ʱ�뱳���޳�
};

enum EFillMode
{
    FFM_SOLID,          // ���ģʽ
    FFM_WIREFRAME,      // �߿�ģʽ
};

enum EDepthBufferType
{
    FDB_NONE,           // �ر�Z������
    FDB_READONLY,       // Z������ֻ��
    FDB_READWRITE,      // Z�������ɶ���д
};

enum ETextureFilter
{
    FTF_POINT,          // ��������
    FTF_LINEAR,         // ˫���Բ�ֵ����
};

enum EFrustumPlane
{
    FFP_LEFT,           // ��ƽ��
    FFP_RIGHT,          // ��ƽ��
    FFP_TOP,            // ��ƽ��
    FFP_BOTTOM,         // ��ƽ��
    FFP_NEAR,           // ��ƽ��
    FFP_FAR,            // Զƽ��
};

//////////////////////////////////////////////////////////////////////////

/** �����ʽ(��ά)
*/
struct SVertex3D
{
    Vector3 pos;        // λ��
    Vector3 normal;     // ����
    DWORD color;        // ������ɫ
    Vector2 tex;        // ��������
};

/** �����ʽ(�任����Ļ�Ķ�ά����)
*/
struct SVertex2D
{
    POINT pos;          // ��Ļ�ϵ�����
    DWORD color;        // ������ɫ
    float z;            // ��������ֵ
    float w;            // ����͸��У��
    Vector2 tex;        // �����ϵ�����
};

/** ������Ϣ
*/
struct SMaterial
{
    ColorF Ambient;     // ��������ɫ
    ColorF Diffuse;     // �������ɫ
};

/** ������Ϣ
*/
struct SLight
{
    ColorF Ambient;     // ��������ɫ
    ColorF Diffuse;     // �������ɫ
    Vector3 Direction;  // ���յķ���
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWTYPES_H__