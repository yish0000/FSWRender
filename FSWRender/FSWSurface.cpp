/*
 * -----------------------------------------------------------------------
 *  Name:   FSWSurface.cpp
 *  Desc:   本文件定义了一个图形表面对象。
 *  Author: Yish
 *  Date:   2011/7/23
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FSWSurface.h"


/** 构造函数
*/
FSWSurface::FSWSurface(void) : m_pData(NULL),m_nWidth(0),m_nHeight(0),
    m_Format(FPF_A8R8G8B8)
{
}

/** 析构函数
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

/** 创建一个指定的缓冲区
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