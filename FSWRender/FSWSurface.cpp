/*
 * -----------------------------------------------------------------------
 *  Name:   FSWSurface.cpp
 *  Desc:   ���ļ�������һ��ͼ�α������
 *  Author: Yish
 *  Date:   2011/7/23
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FSWSurface.h"


/** ���캯��
*/
FSWSurface::FSWSurface(void) : m_pData(NULL),m_nWidth(0),m_nHeight(0),
    m_Format(FPF_A8R8G8B8)
{
}

/** ��������
*/
FSWSurface::~FSWSurface(void)
{
    if( m_Format == FPF_A8R8G8B8 )
    {
        DWORD* ptr = (DWORD*)m_pData;
        SAFE_DELETE( ptr );
    }
    else
    {
        float* ptr = (float*)m_pData;
        SAFE_DELETE( ptr );
    }
}

/** ����һ��ָ���Ļ�����
*/
bool FSWSurface::Create( UINT nWidth,UINT nHeight,EPixelFormat format )
{
    assert( nWidth && nHeight );

    if( m_nWidth != nWidth || m_nHeight != nHeight || m_Format != format )
    {
		if( m_Format == FPF_A8R8G8B8 )
		{
			DWORD* ptr = (DWORD*)m_pData;
			SAFE_DELETE( ptr );
		}
		else
		{
			float* ptr = (float*)m_pData;
			SAFE_DELETE( ptr );
		}

        m_nWidth = nWidth;
        m_nHeight = nHeight;
        m_Format = format;

        if( m_Format == FPF_A8R8G8B8 )
            m_pData = new DWORD[m_nWidth*m_nHeight];
        else
            m_pData = new float[m_nWidth*m_nHeight];

        if( !m_pData )
			return false;
    }

    return true;
}