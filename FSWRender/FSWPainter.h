/*
 * ------------------------------------------------------------------------
 *  Name:   FSWPainter.h
 *  Desc:   ���ļ�������һ��ͼ�λ��������ࡣ
 *  Author: Yish
 *  Date:   2011/7/24
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSWPAINTER_H__
#define __FSWPAINTER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWTypes.h"
#include "FSWSurface.h"
#include "FSWTexture.h"

///////////////////////////////////////////////////////////////////////////


enum EPainterType
{
    PAINT_GOURAUD,          // ���޵���ɫ
    PAINT_GOURAUD_WIRE,     // ���޵���ɫ���߿�
    PAINT_TEXTURE,          // ������ĸ��޵���ɫ
    PAINT_TEXTURE_WIRE,     // ������ĸ��޵���ɫ���߿�

    PAINT_NUM,
};

struct SRenderState
{
    FSWSurface* pBackBuffer;        // ��̨������ָ��
    FSWSurface* pZBuffer;           // ��Ȼ�����ָ��
    UINT nWidth, nHeight;           // ��̨�������Ŀ��
    RECT rcViewport;                // ���ڲü�����
    FSWTexture* pTexture;           // ����ָ��
    EShadeMode ShadeMode;           // ��ɫģʽ
    ECullMode CullMode;             // �������ģʽ
    EFillMode FillMode;             // �豸�����������ģʽ
    ETextureFilter TexFilter;       // �������ģʽ
    bool bAlphaBlend;               // �Ƿ���Alpha���
};

/** ͼ�λ���������
*/
class FSWPainter
{
public:
    FSWPainter( EPainterType Type, SRenderState* pRS ) : m_Type(Type), m_pRS(pRS) {}

    // �����������б�
    virtual void DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,
        UINT nTriangleCount ) = 0;

    // ��ȡ������������
    EPainterType GetType() const { return m_Type; }

    // ��ȡ�豸��Ⱦ״̬
    SRenderState* GetRenderState() { return m_pRS; }

    // ����ָ���Ļ�����
    static FSWPainter* CreatePainter( EPainterType Type, SRenderState* pRS );

protected:
    EPainterType m_Type;        // ��ɫ��������
    SRenderState* m_pRS;        // ��ǰ�豸����Ⱦ״̬
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWPAINTER_H__