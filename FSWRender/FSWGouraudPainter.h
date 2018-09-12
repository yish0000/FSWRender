/*
 * -----------------------------------------------------------------------
 *  Name:   FSWGouraudPainter.h
 *  Desc:   ���ļ�������һ�����޵���ɫ����
 *  Author: Yish
 *  Date:   2011/7/24
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FSWGOURAUDPAINTER_H__
#define __FSWGOURAUDPAINTER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FSWPainter.h"

//////////////////////////////////////////////////////////////////////////


/** ���޵���ɫ��
*/
class FSWGouraudPainter : public FSWPainter
{
public:
    FSWGouraudPainter( SRenderState* pRS );
    ~FSWGouraudPainter();

    // �����������б�
    void DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,
        UINT nTriangleCount );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWGOURAUDPAINTER_H__