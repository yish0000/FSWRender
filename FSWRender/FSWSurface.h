/*
 * ------------------------------------------------------------------------
 *  Name:   FSWSurface.h
 *  Desc:   ���ļ�������һ��ͼ�α������
 *  Author: Yish
 *  Date:   2011/7/20
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSWSURFACE_H__
#define __FSWSURFACE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWTypes.h"

///////////////////////////////////////////////////////////////////////////


/** ͼ�α������
*/
class FSWSurface
{
private:
    void* m_pData;          // ͼ�α��������
    EPixelFormat m_Format;  // ���صĸ�ʽ
    UINT m_nWidth;          // ͼ�α���Ŀ��
    UINT m_nHeight;         // ͼ�α���ĸ߶�

public:
    FSWSurface(void);
    ~FSWSurface(void);

    // ����������
    bool Create( UINT nWidth,UINT nHeight,EPixelFormat format );

    // ��ȡ����������ָ��
    void* GetBufferData(void) { return m_pData; }

    // ��ȡ��Ⱥ͸߶�
    UINT GetWidth(void) const { return m_nWidth; }
    UINT GetHeight(void) const { return m_nHeight; }

    // ��ȡ�����������ظ�ʽ
    EPixelFormat GetPixelFormat(void) const { return m_Format; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWSURFACE_H__