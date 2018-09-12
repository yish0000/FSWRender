/*
 * -----------------------------------------------------------------------
 *  Name:   FSWGouraudWirePainter.cpp
 *  Desc:   本文件定义了一个高罗德线框绘制器。
 *  Author: Yish
 *  Date:   2011/7/24
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FSWGouraudWirePainter.h"
#include "FSWDeviceHelper.h"


/** 构造函数
*/
FSWGouraudWirePainter::FSWGouraudWirePainter( SRenderState* pRS )
    : FSWPainter(PAINT_GOURAUD_WIRE, pRS)
{
}

/** 析构函数
*/
FSWGouraudWirePainter::~FSWGouraudWirePainter()
{
}

/** 绘制三角形列表
*/
void FSWGouraudWirePainter::DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,
                                              UINT nTriangleCount )
{
    SVertex2D *from,*to;
    SVertex2D *v[3];
    int steps;
    float incx,incy,x,y;
    int startR,startG,startB;
    int stepR,stepG,stepB;
    float startZ,stepZ;
    float tmpDiv;

    // 获取后台缓冲区和Z缓冲区指针
    DWORD* lockedSurface = (DWORD*)m_pRS->pBackBuffer->GetBufferData();
    float* lockedZBuffer = (float*)m_pRS->pZBuffer->GetBufferData();

    for( UINT i=0;i<nTriangleCount;++i )
    {
        v[0] = &pVerts[*pIndis++];
        v[1] = &pVerts[*pIndis++];
        v[2] = &pVerts[*pIndis++];

        // 对三角形进行背面剪裁
        if( m_pRS->CullMode == FCM_CCW )
        {
            int n = (v[2]->pos.x - v[0]->pos.x) * (v[2]->pos.y - v[1]->pos.y) -
                (v[2]->pos.y - v[0]->pos.y) * (v[2]->pos.x - v[1]->pos.x);
            if( n < 0 ) continue;
        }
        else if( m_pRS->CullMode == FCM_CW )
        {
            int n = (v[2]->pos.x - v[0]->pos.x) * (v[2]->pos.y - v[1]->pos.y) -
                (v[2]->pos.y - v[0]->pos.y) * (v[2]->pos.x - v[1]->pos.x);
            if( n > 0 ) continue;
        }

        for( UINT l=0;l<3;++l )
        {
            if( l == 2 )
            {
                from = v[0];
                to = v[2];
            }
            else
            {
                from = v[l];
                to = v[l+1];
            }

            POINT newV1,newV2;
            if( Clip2DLine(m_pRS->rcViewport,from->pos,to->pos,newV1,newV2) )
            {
                if( abs(newV2.x-newV1.x) > abs(newV2.y-newV1.y) )
                    steps = abs(newV2.x - newV1.x);
                else
                    steps = abs(newV2.y - newV1.y);

                tmpDiv = 1.0f / (float)steps;
                x = (float)newV1.x;
                y = (float)newV1.y;
                incx = (float)(newV2.x - newV1.x) * tmpDiv;
                incy = (float)(newV2.y - newV1.y) * tmpDiv;

                startR = _GetRed(from->color);
                startG = _GetGreen(from->color);
                startB = _GetBlue(from->color);
                stepR = (_GetRed(to->color) - startR) * tmpDiv;
                stepG = (_GetGreen(to->color) - startG) * tmpDiv;
                stepB = (_GetBlue(to->color) - startB) * tmpDiv;
                startZ = from->z;
                stepZ = (to->z - from->z) * tmpDiv;

                while( steps-- > 0 )
                {
                    UINT nx = (UINT)x;
                    UINT ny = (UINT)y;

                    if( startZ < lockedZBuffer[ny*m_pRS->nWidth+nx] )
                    {
                        lockedZBuffer[ny*m_pRS->nWidth+nx] = startZ;
                        lockedSurface[ny*m_pRS->nWidth+nx] = _MakeColor( startR,startG,startB,0xff00 );
                    }

                    x += incx;
                    y += incy;
                    startR += stepR;
                    startG += stepG;
                    startB += stepB;
                    startZ += stepZ;
                }
            }
        }
    }
}