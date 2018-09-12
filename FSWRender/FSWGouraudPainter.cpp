/*
 * -----------------------------------------------------------------------
 *  Name:   FSWGouraudPainter.cpp
 *  Desc:   本文件定义了一个高罗德着色器。
 *  Author: Yish
 *  Date:   2011/7/24
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FSWGouraudPainter.h"
#include "FSWDeviceHelper.h"


/** 构造函数
*/
FSWGouraudPainter::FSWGouraudPainter( SRenderState* pRS )
    : FSWPainter(PAINT_GOURAUD, pRS)
{
}

/** 析构函数
*/
FSWGouraudPainter::~FSWGouraudPainter()
{
}

/** 绘制三角形列表
*/
void FSWGouraudPainter::DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,
                                          UINT nTriangleCount )
{
    SVertex2D *v1,*v2,*v3;  // 要绘制的三角形的顶点
    RECT rcTriangle; // 要绘制的三角形的包围矩形
    int height; // 三角形的高度
    float longest;  // 三角形最长的横跨度
    float tmpDiv;   // 用于临时存储除数
    int span,spanEnd;	// 在Y轴上，从span开始，spanEnd结束，以多条扫描线进行填充
    float leftdeltaxf,rightdeltaxf;	// 每隔一行扫描线，扫描线的左右端点延伸的长度
    float leftxf,rightxf;			// 当前扫描线上，左右端点的坐标
    int leftx,rightx;				// 当前扫描线上，左右端点的坐标
    int leftR,leftG,leftB,rightR,rightG,rightB;	// 当前扫描线上，左右端点的颜色值
    int leftStepR,leftStepG,leftStepB;	// 每隔一行扫描线，扫描线左端点颜色增加的大小
    int rightStepR,rightStepG,rightStepB;	// 每隔一行扫描线，扫描线右端点颜色增加的大小
    float leftZValue,rightZValue;		// 当前扫描线上，左右端点的深度
    float leftZStep,rightZStep;			// 每隔一行扫描线，扫描线左右端点Z值增加的大小
    int spanR,spanG,spanB;				// 画扫描线时，像素的颜色
    int spanStepR,spanStepG,spanStepB;	// 画扫描线时，下一个像素，颜色值要增加的大小
    float spanZValue,spanZStep;			// 画扫描线时，像素的深度值和下一个像素深度值增加的大小
    DWORD *surfacePtr,*spanBeginPtr,*spanEndPtr;	// 画扫描线时，本线像素在后台缓冲区中的起始和结束位置
    float *zBufferPtr,*spanZPtr;		// 画扫描线时，当前像素在深度缓冲区的位置

    // 获取后台缓冲区和Z缓冲区指针
    DWORD* lockedSurface = (DWORD*)m_pRS->pBackBuffer->GetBufferData();
    float* lockedZBuffer = (float*)m_pRS->pZBuffer->GetBufferData();

    for( UINT i=0;i<nTriangleCount;++i )
    {
        v1 = &pVerts[*pIndis++];
        v2 = &pVerts[*pIndis++];
        v3 = &pVerts[*pIndis++];

        // 对三角形进行背面剔除
        if( m_pRS->CullMode == FCM_CCW )
        {
            int n = (v3->pos.x - v1->pos.x) * (v3->pos.y - v2->pos.y) -
                (v3->pos.y - v1->pos.y) * (v3->pos.x - v2->pos.x);
            if( n < 0 ) continue;
        }
        else if( m_pRS->CullMode == FCM_CW )
        {
            int n = (v3->pos.x - v1->pos.x) * (v3->pos.y - v2->pos.y) -
                (v3->pos.y - v1->pos.y) * (v3->pos.x - v2->pos.x);
            if( n > 0 ) continue;
        }

        // 如果三角形在近平面之外则剔除
        if( v1->z < 0 && v2->z < 0 && v3->z < 0 )
            continue;

        // 计算三角形的包围矩形
        if( v1->pos.x > v2->pos.x ) std::swap( v1,v2 );
        if( v1->pos.x > v3->pos.x ) std::swap( v1,v3 );
        if( v2->pos.x > v3->pos.x ) std::swap( v2,v3 );
        rcTriangle.left = v1->pos.x;
        rcTriangle.right = v3->pos.x;
        if( v3->pos.x - v1->pos.x == 0 ) continue;
        if( v1->pos.y > v2->pos.y ) std::swap( v1,v2 );
        if( v1->pos.y > v3->pos.y ) std::swap( v1,v3 );
        if( v2->pos.y > v3->pos.y ) std::swap( v2,v3 );
        rcTriangle.top = v1->pos.y;
        rcTriangle.bottom = v3->pos.y;

        // 判断三角形的包围矩形是否在屏幕之外
        if( !IsRectCollided(rcTriangle,m_pRS->rcViewport) )
            continue;

        // 计算三角形的高度
        height = v3->pos.y - v1->pos.y;
        if( !height ) continue;

        // 计算三角形最长的横跨度
        longest = (v2->pos.y - v1->pos.y) / (float)height * (v3->pos.x - v1->pos.x)
            + (v1->pos.x - v2->pos.x);

		// 下面开始绘制三角形，我们以横向最大跨度线将三角形分为两个部分
		// 先绘制上半部分
        spanEnd = v2->pos.y;
        span = v1->pos.y;
        leftxf = (float)v1->pos.x;
        rightxf = (float)v1->pos.x;

        leftZValue = v1->z;
        rightZValue = v1->z;

        leftR = rightR = _GetRed(v1->color);
        leftG = rightG = _GetGreen(v1->color);
        leftB = rightB = _GetBlue(v1->color);

		// 将两个缓冲区的指针移到当前扫描线的位置
        surfacePtr = lockedSurface + span * m_pRS->nWidth;
        zBufferPtr = lockedZBuffer + span * m_pRS->nWidth;

        if (longest < 0.0f)
        {
            tmpDiv = 1.0f / (float)(v2->pos.y - v1->pos.y);
            rightdeltaxf = (v2->pos.x - v1->pos.x) * tmpDiv;
            rightZStep = (v2->z - v1->z) * tmpDiv;
            rightStepR = (_GetRed(v2->color) - rightR) * tmpDiv;
            rightStepG = (_GetGreen(v2->color) - rightG) * tmpDiv;
            rightStepB = (_GetBlue(v2->color) - rightB) * tmpDiv;

            tmpDiv = 1.0f / (float)height;
            leftdeltaxf = (v3->pos.x - v1->pos.x) * tmpDiv;
            leftZStep = (v3->z - v1->z) * tmpDiv;
            leftStepR = (_GetRed(v3->color) - leftR) * tmpDiv;
            leftStepG = (_GetGreen(v3->color) - leftG) * tmpDiv;
            leftStepB = (_GetBlue(v3->color) - leftB) * tmpDiv;
        }
        else
        {
            tmpDiv = 1.0f / (float)height;
            rightdeltaxf = (v3->pos.x - v1->pos.x) * tmpDiv;
            rightZStep = (v3->z - v1->z) * tmpDiv;
            rightStepR = (_GetRed(v3->color) - rightR) * tmpDiv;
            rightStepG = (_GetGreen(v3->color) - rightG) * tmpDiv;
            rightStepB = (_GetBlue(v3->color) - rightB) * tmpDiv;

            tmpDiv = 1.0f / (float)(v2->pos.y - v1->pos.y);
            leftdeltaxf = (v2->pos.x - v1->pos.x) * tmpDiv;
            leftZStep = (v2->z - v1->z) * tmpDiv;
            leftStepR = (_GetRed(v2->color) - leftR) * tmpDiv;
            leftStepG = (_GetGreen(v2->color) - leftG) * tmpDiv;
            leftStepB = (_GetBlue(v2->color) - leftB) * tmpDiv;
        }

		// 绘制三角形，分两次绘制
        for (int triangleHalf=0; triangleHalf<2; ++triangleHalf)
        {
			// 如果扫描线结束位置大于屏幕底部，则忽略剩余的扫描线
            if (spanEnd > m_pRS->rcViewport.bottom )
                spanEnd = m_pRS->rcViewport.bottom;

            // 如果扫描线开始位置小于屏幕顶部，则忽略顶部以上的扫描线
            if (span < m_pRS->rcViewport.top)
            {
                // 用leftx暂时记录所跳跃的扫描线个数
                if (spanEnd < m_pRS->rcViewport.top)
                {
                    leftx = spanEnd - span;
                    span = spanEnd;
                }
                else
                {
                    leftx = m_pRS->rcViewport.top - span; 
                    span = m_pRS->rcViewport.top;
                }

				// 改变起始扫描线，以及其端点的值
                leftxf += leftdeltaxf*leftx;
                rightxf += rightdeltaxf*leftx;
                leftZValue += leftZStep*leftx;
                rightZValue += rightZStep*leftx;
                leftR += leftStepR*leftx;
                leftG += leftStepG*leftx;
                leftB += leftStepB*leftx;
                rightR += rightStepR*leftx;
                rightG += rightStepG*leftx;
                rightB += rightStepB*leftx;

				// 重新定位后台缓冲区和深度缓冲区的当前位置
				surfacePtr += m_pRS->nWidth*leftx;
				zBufferPtr += m_pRS->nWidth*leftx;
            }


            // 绘制每条扫描线
            while (span < spanEnd)
            {
                leftx = (int)(leftxf);
                rightx = (int)(rightxf + 0.5f);

                tmpDiv = 1.0f / (rightx - leftx);
                spanZValue = leftZValue;
                spanZStep = (rightZValue - leftZValue) * tmpDiv;

                spanR = leftR;
                spanG = leftG;
                spanB = leftB;
                spanStepR = (rightR - leftR) * tmpDiv;
                spanStepG = (rightG - leftG) * tmpDiv;
                spanStepB = (rightB - leftB) * tmpDiv;

                // 对像素做一些窗口剪裁
                if (leftx<m_pRS->rcViewport.left)
                {
                    int tmp = m_pRS->rcViewport.left - leftx;
                    spanZValue += tmp * spanZStep;
                    spanR += spanStepR * tmp;
                    spanG += spanStepG * tmp;
                    spanB += spanStepB * tmp;
                    leftx = m_pRS->rcViewport.left;
                }
                else if (leftx>m_pRS->rcViewport.right)
                    leftx = m_pRS->rcViewport.right;

                if (rightx<m_pRS->rcViewport.left)
                    rightx = m_pRS->rcViewport.left;
                else if (rightx>m_pRS->rcViewport.right)
                    rightx = m_pRS->rcViewport.right;

                // 画当前扫描线
                if (rightx - leftx != 0)
                {
                    spanBeginPtr = surfacePtr + leftx;
                    spanEndPtr = surfacePtr + rightx;
                    spanZPtr = zBufferPtr + leftx;

                    while (spanBeginPtr < spanEndPtr)
                    {
                        if (spanZValue < *spanZPtr)
                        {
                            *spanZPtr = spanZValue;
                            *spanBeginPtr = _MakeColor(spanR,spanG,spanB,0xff00);
                        }

                        spanR += spanStepR;
                        spanG += spanStepG;
                        spanB += spanStepB;

                        spanZValue += spanZStep;
                        ++spanBeginPtr;
                        ++spanZPtr;
                    }
                }

				// 开始绘制下一行
                leftxf += leftdeltaxf;
                rightxf += rightdeltaxf;
                leftZValue += leftZStep;
                rightZValue += rightZStep;
                leftR += leftStepR;
                leftG += leftStepG;
                leftB += leftStepB;
                rightR += rightStepR;
                rightG += rightStepG;
                rightB += rightStepB;

				surfacePtr += m_pRS->nWidth;
				zBufferPtr += m_pRS->nWidth;
				++span;
            }

			// 如果两个都绘制完了，跳出循环
            if( triangleHalf > 0 )
                break;

			// 准备绘制三角形的第二个部分
            if( longest < 0.0f )
            {
                tmpDiv = 1.0f / (v3->pos.y - v2->pos.y);

                rightdeltaxf = (v3->pos.x - v2->pos.x) * tmpDiv;
                rightxf = (float)v2->pos.x;

                rightZValue = v2->z;
                rightZStep = (v3->z - v2->z) * tmpDiv;

                rightR = _GetRed(v2->color);
                rightG = _GetGreen(v2->color);
                rightB = _GetBlue(v2->color);
                rightStepR = (_GetRed(v3->color) - rightR) * tmpDiv;
                rightStepG = (_GetGreen(v3->color) - rightG) * tmpDiv;
                rightStepB = (_GetBlue(v3->color) - rightB) * tmpDiv;
            }
            else
            {
                tmpDiv = 1.0f / (v3->pos.y - v2->pos.y);

                leftdeltaxf = (v3->pos.x - v2->pos.x) * tmpDiv;
                leftxf = (float)v2->pos.x;

                leftZValue = v2->z;
                leftZStep = (v3->z - v2->z) * tmpDiv;

                leftR = _GetRed(v2->color);
                leftG = _GetGreen(v2->color);
                leftB = _GetBlue(v2->color);
                leftStepR = (_GetRed(v3->color) - leftR) * tmpDiv;
                leftStepG = (_GetGreen(v3->color) - leftG) * tmpDiv;
                leftStepB = (_GetBlue(v3->color) - leftB) * tmpDiv;
            }

            spanEnd = v3->pos.y;
        }
    }
}