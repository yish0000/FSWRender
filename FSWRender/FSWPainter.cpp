/*
 * ------------------------------------------------------------------------
 *  Name:   FSWPainter.cpp
 *  Desc:   ���ļ�ʵ������Ⱦ�豸�Ļ��������ࡣ
 *  Author: Yish
 *  Date:   2013/3/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FSWPainter.h"

#include "FSWGouraudPainter.h"
#include "FSWGouraudWirePainter.h"
#include "FSWTextureGouraudPainter.h"
#include "FSWTextureWirePainter.h"


/** ����ָ���Ļ�����
*/
FSWPainter* FSWPainter::CreatePainter( EPainterType Type, SRenderState* pRS )
{
    switch( Type )
    {
    case PAINT_GOURAUD:         return new FSWGouraudPainter(pRS);
    case PAINT_GOURAUD_WIRE:    return new FSWGouraudWirePainter(pRS);
    case PAINT_TEXTURE:         return new FSWTextureGouraudPainter(pRS);
    case PAINT_TEXTURE_WIRE:    return new FSWTextureWirePainter(pRS);
    default: ASSERT(0);         return NULL;
    }
}