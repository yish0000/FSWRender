/*
 * -----------------------------------------------------------------------
 *  Name:   FSWTextureGouraudPainter.cpp
 *  Desc:   本文件定义了一个带纹理的高罗德着色器。
 *  Author: Yish
 *  Date:   2011/7/24
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FSWTextureGouraudPainter.h"
#include "FSWDeviceHelper.h"


/** 构造函数
*/
FSWTextureGouraudPainter::FSWTextureGouraudPainter( SRenderState* pRS )
    : FSWPainter(PAINT_TEXTURE, pRS)
{
}

/** 析构函数
*/
FSWTextureGouraudPainter::~FSWTextureGouraudPainter()
{
}

/** 绘制三角形列表
*/
void FSWTextureGouraudPainter::DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,
                                                 UINT nTriangleCount )
{
    SVertex2D *v1,*v2,*v3;  // 要绘制的三角形的顶点
    Vector2 vTex1,vTex2,vTex3;
    RECT rcTriangle; // 要绘制的三角形的包围矩形
    int height; // 三角形的高度
    float longest;  // 三角形最长的横跨度
    float tmpDiv;   // 用于临时存储除数
    int span,spanEnd;	// 在Y轴上，从span开始，spanEnd结束，以多条扫描线进行填充
    float leftdeltaxf,rightdeltaxf;	// 每隔一行扫描线，扫描线的左右端点延伸的长度
    float leftxf,rightxf;	// 当前扫描线上，左右端点的X坐标
    int leftx,rightx;		// 当前扫描线上，左右端点的X坐标
    int leftR,leftG,leftB,rightR,rightG,rightB;	// 当前扫描线上，左右端点的颜色值
    int leftStepR,leftStepG,leftStepB;	// 每隔一行扫描线，左端点颜色增加的大小
    int rightStepR,rightStepG,rightStepB;	// 每隔一行扫描线，右端点颜色增加的大小
    float leftZValue,rightZValue;	// 当前扫描线上，左右端点的深度值
    float leftZStep,rightZStep;		// 每隔一行扫描线，扫描线左右端点Z值得增加大小
    int spanR,spanG,spanB;			// 画扫描线的时候，像素的颜色
    int spanStepR,spanStepG,spanStepB;	// 画扫描线的时候，下一个像素，颜色值要增加的大小
    float spanZValue,spanZStep;		// 画扫描线的时候，像素的深度值和下一个像素深度值增加的大小
    DWORD *surfacePtr,*spanBeginPtr,*spanEndPtr;	// 画扫描线时，本线像素在后台缓冲区中的起始、结束位置
    float *zBufferPtr,*spanZPtr;	// 画扫描线时，本线像素在深度缓冲区中的位置

    float leftW,rightW;		// 当扫描当前线时，左右端点的深度值(1.0f/w)
    float leftStepW,rightStepW;	// 每隔一条扫描线，扫描线左右端点的深度值(t/w)
    float spanW,spanStepW;		// 画扫描线时，深度(1.0f/w)的差值；下一个像素要增加的大小
    float leftTu,leftTv,rightTu,rightTv;	// 当前扫描线上，左右端点的纹理坐标
    float leftStepTu,leftStepTv,rightStepTu,rightStepTv;	// 每隔一条扫描线，左右端点纹理坐标增加的大小
    float spanTu,spanTv,spanStepTu,spanStepTv;	// 画扫描线时，像素的纹理坐标和下一个像素纹理坐标要增加的大小

    // 获取后台缓冲区和Z缓冲区指针
    DWORD* lockedSurface = (DWORD*)m_pRS->pBackBuffer->GetBufferData();
    float* lockedZBuffer = (float*)m_pRS->pZBuffer->GetBufferData();
    DWORD* lockedTexture = (DWORD*)m_pRS->pTexture->GetBufferData();
    UINT textureWidth = m_pRS->pTexture->GetWidth();
    UINT textureHeight = m_pRS->pTexture->GetHeight();

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

        spanEnd = v2->pos.y;
        span = v1->pos.y;
        leftxf = (float)v1->pos.x;
        rightxf = (float)v1->pos.x;

        leftZValue = v1->z;
        rightZValue = v1->z;

        leftR = rightR = _GetRed(v1->color);
        leftG = rightG = _GetGreen(v1->color);
        leftB = rightB = _GetBlue(v1->color);

		// 取三个顶点深度的倒数
        float invW1 = 1.0f / v1->w;
        float invW2 = 1.0f / v2->w;
        float invW3 = 1.0f / v3->w;

        // 顶点的纹理坐标
        vTex1 = v1->tex;
        vTex2 = v2->tex;
        vTex3 = v3->tex;

        leftW = rightW = invW1;
        leftTu = rightTu = vTex1.x * invW1;
        leftTv = rightTv = vTex1.y * invW1;

        surfacePtr = lockedSurface + span * m_pRS->nWidth;
        zBufferPtr = lockedZBuffer + span * m_pRS->nWidth;

        if( longest < 0.0f )
        {
            tmpDiv = 1.0f / (float)(v2->pos.y - v1->pos.y);
            rightdeltaxf = (v2->pos.x - v1->pos.x) * tmpDiv;
            rightZStep = (v2->z - v1->z) * tmpDiv;
            rightStepR = (int)((_GetRed(v2->color) - rightR) * tmpDiv);
            rightStepG = (int)((_GetGreen(v2->color) - rightG) * tmpDiv);
            rightStepB = (int)((_GetBlue(v2->color) - rightB) * tmpDiv);
            rightStepW = (invW2 - invW1) * tmpDiv;
            rightStepTu = (vTex2.x * invW2 - rightTu) * tmpDiv;
            rightStepTv = (vTex2.y * invW2 - rightTv) * tmpDiv;

            tmpDiv = 1.0f / (float)height;
            leftdeltaxf = (v3->pos.x - v1->pos.x) * tmpDiv;
            leftZStep = (v3->z - v1->z) * tmpDiv;
            leftStepR = (int)((_GetRed(v3->color) - leftR) * tmpDiv);
            leftStepG = (int)((_GetGreen(v3->color) - leftG) * tmpDiv);
            leftStepB = (int)((_GetBlue(v3->color) - leftB) * tmpDiv);
            leftStepW = (invW3 - invW1) * tmpDiv;
            leftStepTu = (vTex3.x * invW3 - leftTu) * tmpDiv;
            leftStepTv = (vTex3.y * invW3 - leftTv) * tmpDiv;
        }
        else
        {
            tmpDiv = 1.0f / (float)height;
            rightdeltaxf = (v3->pos.x - v1->pos.x) * tmpDiv;
            rightZStep = (v3->z - v1->z) * tmpDiv;
            rightStepR = (int)((_GetRed(v3->color) - rightR) * tmpDiv);
            rightStepG = (int)((_GetGreen(v3->color) - rightG) * tmpDiv);
            rightStepB = (int)((_GetBlue(v3->color) - rightB) * tmpDiv);
            rightStepW = (invW3 - rightW) * tmpDiv;
            rightStepTu = (vTex3.x * invW3 - rightTu) * tmpDiv;
            rightStepTv = (vTex3.y * invW3 - rightTv) * tmpDiv;

            tmpDiv = 1.0f / (float)(v2->pos.y - v1->pos.y);
            leftdeltaxf = (v2->pos.x - v1->pos.x) * tmpDiv;
            leftZStep = (v2->z - v1->z) * tmpDiv;
            leftStepR = (int)((_GetRed(v2->color) - leftR) * tmpDiv);
            leftStepG = (int)((_GetGreen(v2->color) - leftG) * tmpDiv);
            leftStepB = (int)((_GetBlue(v2->color) - leftB) * tmpDiv);
            leftStepW = (invW2 - leftW) * tmpDiv;
            leftStepTu = (vTex2.x * invW2 - leftTu) * tmpDiv;
            leftStepTv = (vTex2.y * invW2 - leftTv) * tmpDiv;
        }

		// 绘制三角形，分两次绘制
        for( int triangleHalf=0; triangleHalf<2; ++triangleHalf )
        {
			// 如果扫描线结束位置大于屏幕底部，则忽略剩余的扫描线
            if( spanEnd > m_pRS->rcViewport.bottom )
                spanEnd = m_pRS->rcViewport.bottom;

			// 如果扫描线开始位置小于屏幕顶部，则忽略顶部以上的扫描线
            if (span < m_pRS->rcViewport.top)
            {
				// 用leftx暂时记录所跳跃的扫描线条数
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

                leftxf += leftdeltaxf*leftx;
                rightxf += rightdeltaxf*leftx;
                surfacePtr += m_pRS->nWidth*leftx;
                zBufferPtr += m_pRS->nWidth*leftx;
                leftZValue += leftZStep*leftx;
                rightZValue += rightZStep*leftx;

                leftR += leftStepR*leftx;
                leftG += leftStepG*leftx;
                leftB += leftStepB*leftx;
                rightR += rightStepR*leftx;
                rightG += rightStepG*leftx;
                rightB += rightStepB*leftx;

                leftW += leftStepW*leftx;
                leftTu += leftStepTu*leftx;
                leftTv += leftStepTv*leftx;
                rightTu += rightStepTu*leftx;
                rightTv += rightStepTv*leftx;
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
                spanStepR = (int)((rightR - leftR) * tmpDiv);
                spanStepG = (int)((rightG - leftG) * tmpDiv);
                spanStepB = (int)((rightB - leftB) * tmpDiv);

                spanW = leftW;
                spanStepW = (rightW - leftW) * tmpDiv;
                spanTu = leftTu;
                spanTv = leftTv;
                spanStepTu = (rightTu - leftTu) * tmpDiv;
                spanStepTv = (rightTv - leftTv) * tmpDiv;

                // 对像素做一些窗口剪裁
                if( leftx < m_pRS->rcViewport.left )
                {
                    int tmp = m_pRS->rcViewport.left - leftx;
                    spanZValue += tmp * spanZStep;
                    spanR += spanStepR * tmp;
                    spanG += spanStepG * tmp;
                    spanB += spanStepB * tmp;
                    spanW += tmp * spanStepW;
                    spanTu += tmp * spanStepTu;
                    spanTv += tmp * spanStepTv;
                    leftx = m_pRS->rcViewport.left;
                }
                else if (leftx>m_pRS->rcViewport.right)
                    leftx = m_pRS->rcViewport.right;

                if (rightx<m_pRS->rcViewport.left)
                    rightx = m_pRS->rcViewport.left;
                else if (rightx>m_pRS->rcViewport.right)
                    rightx = m_pRS->rcViewport.right;

                if (rightx - leftx != 0)
                {
                    spanBeginPtr = surfacePtr + leftx;
                    spanEndPtr = surfacePtr + rightx;
                    spanZPtr = zBufferPtr + leftx;

                    while (spanBeginPtr < spanEndPtr)
                    {
                        if( spanZValue < *spanZPtr )
                        {
							DWORD verColor, texColor;
							float ww = 1.0f / spanW;	// 用于透视矫正

							// 写入深度值
                            *spanZPtr = spanZValue;

							verColor = _MakeColor(spanR, spanG, spanB, 0xff00);
							texColor = (m_pRS->TexFilter == FTF_LINEAR) ?
								_Sampler2D_Bilinear(lockedTexture, textureWidth, textureHeight, spanTu*ww, spanTv*ww) :
								_Sampler2D_Point(lockedTexture, textureWidth, textureHeight, spanTu*ww, spanTv*ww);

                            *spanBeginPtr = m_pRS->bAlphaBlend ?
								_AlphaBlend(_Modulate(verColor, texColor), *spanBeginPtr) :
								_Modulate(verColor, texColor);
                        }

                        spanR += spanStepR;
                        spanG += spanStepG;
                        spanB += spanStepB;

                        spanZValue += spanZStep;
                        spanW += spanStepW;
                        spanTu += spanStepTu;
                        spanTv += spanStepTv;
                        ++spanBeginPtr;
                        ++spanZPtr;
                    }
                }

				// 开始绘制下一行
                leftxf += leftdeltaxf;
                rightxf += rightdeltaxf;
                ++span;
                surfacePtr += m_pRS->nWidth;
                zBufferPtr += m_pRS->nWidth;
                leftZValue += leftZStep;
                rightZValue += rightZStep;

                leftR += leftStepR;
                leftG += leftStepG;
                leftB += leftStepB;
                rightR += rightStepR;
                rightG += rightStepG;
                rightB += rightStepB;

                leftW += leftStepW;
                rightW += rightStepW;
                leftTu += leftStepTu;
                leftTv += leftStepTv;
                rightTu += rightStepTu;
                rightTv += rightStepTv;
            }

			// 如果两个都绘制完了，跳出循环
            if( triangleHalf > 0 )
                break;

			// 准备绘制第二个部分
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
                rightStepR = (int)((_GetRed(v3->color) - rightR) * tmpDiv);
                rightStepG = (int)((_GetGreen(v3->color) - rightG) * tmpDiv);
                rightStepB = (int)((_GetBlue(v3->color) - rightB) * tmpDiv);

                rightW = invW2;
                rightStepW = (invW3 - invW2) * tmpDiv;

                rightTu = vTex2.x * invW2;
                rightTv = vTex2.y * invW2;
                rightStepTu = (vTex3.x * invW3 - rightTu) * tmpDiv;
                rightStepTv = (vTex3.y * invW3 - rightTv) * tmpDiv;
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
                leftStepR = (int)((_GetRed(v3->color) - leftR) * tmpDiv);
                leftStepG = (int)((_GetGreen(v3->color) - leftG) * tmpDiv);
                leftStepB = (int)((_GetBlue(v3->color) - leftB) * tmpDiv);

                leftW = invW2;
                leftStepW = (invW3 - invW2) * tmpDiv;
                leftTu = vTex2.x * invW2;
                leftTv = vTex2.y * invW2;
                leftStepTu = (vTex3.x * invW3 - leftTu) * tmpDiv;
                leftStepTv = (vTex3.y * invW3 - leftTv) * tmpDiv;
            }

            spanEnd = v3->pos.y;
        }
    }
}