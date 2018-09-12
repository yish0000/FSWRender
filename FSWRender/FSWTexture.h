/*
 * -----------------------------------------------------------------------
 *  Name:   FSWTexture.h
 *  Desc:   ���ļ�������һ���������
 *  Author: Yish
 *  Date:   2011/7/28
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FSWTEXTURE_H__
#define __FSWTEXTURE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FSWSurface.h"

//////////////////////////////////////////////////////////////////////////


/** �������
*/
class FSWTexture
{
private:
    bool m_bAlpha;              // �Ƿ��͸��
    FSWSurface* m_pSurface;     // �洢�������ݵ�ָ��

public:
    FSWTexture(void);
    ~FSWTexture(void);

    // �����������
    bool Create( UINT nWidth, UINT nHeight );

    // ��λͼ�м���
    bool LoadFromBitmap( const char* filename );

    // �������ݵ�λͼ
    void SaveToBitmap( const char* filename );

    // ��ȡ��������
	DWORD* GetBufferData(void) { return m_pSurface ? (DWORD*)m_pSurface->GetBufferData() : NULL; }

    // ��ȡ����Ŀ�Ⱥ͸߶�
	UINT GetWidth(void) const { return m_pSurface ? m_pSurface->GetWidth() : 0; }
	UINT GetHeight(void) const { return m_pSurface ? m_pSurface->GetHeight() : 0; }

    // ��ȡ�����Surface����
    FSWSurface* _GetSurface() const { return m_pSurface; }

    // �����ص������Ƿ���Alpha��Ϣ
    bool HaveAlpha() const { return m_bAlpha; }

    // ����ָ������Ⱦ����
    static void SaveSurfaceToFile( FSWSurface* pSurface, const char* filename );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWTEXTURE_H__