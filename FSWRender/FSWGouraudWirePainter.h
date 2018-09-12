/*
 * -----------------------------------------------------------------------
 *  Name:   FSWGouraudWirePainter.h
 *  Desc:   ���ļ�������һ�����޵���ɫ��(�߿�ģʽ)��
 *  Author: Yish
 *  Date:   2011/7/24
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FSWGOURAUDWIREPAINTER_H__
#define __FSWGOURAUDWIREPAINTER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FSWPainter.h"

//////////////////////////////////////////////////////////////////////////


/** ���޵��߿���ɫ��
*/
class FSWGouraudWirePainter : public FSWPainter
{
public:
    FSWGouraudWirePainter( SRenderState* pRS );
    ~FSWGouraudWirePainter();

    // �����������б�
    void DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,UINT nTriangleCount );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWGOURAUDWIREPAINTER_H__