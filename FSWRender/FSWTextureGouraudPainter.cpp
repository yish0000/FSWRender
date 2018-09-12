/*
 * -----------------------------------------------------------------------
 *  Name:   FSWTextureGouraudPainter.cpp
 *  Desc:   ���ļ�������һ��������ĸ��޵���ɫ����
 *  Author: Yish
 *  Date:   2011/7/24
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FSWTextureGouraudPainter.h"
#include "FSWDeviceHelper.h"


/** ���캯��
*/
FSWTextureGouraudPainter::FSWTextureGouraudPainter( SRenderState* pRS )
    : FSWPainter(PAINT_TEXTURE, pRS)
{
}

/** ��������
*/
FSWTextureGouraudPainter::~FSWTextureGouraudPainter()
{
}

/** �����������б�
*/
void FSWTextureGouraudPainter::DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,
                                                 UINT nTriangleCount )
{
    SVertex2D *v1,*v2,*v3;  // Ҫ���Ƶ������εĶ���
    Vector2 vTex1,vTex2,vTex3;
    RECT rcTriangle; // Ҫ���Ƶ������εİ�Χ����
    int height; // �����εĸ߶�
    float longest;  // ��������ĺ���
    float tmpDiv;   // ������ʱ�洢����
    int span,spanEnd;	// ��Y���ϣ���span��ʼ��spanEnd�������Զ���ɨ���߽������
    float leftdeltaxf,rightdeltaxf;	// ÿ��һ��ɨ���ߣ�ɨ���ߵ����Ҷ˵�����ĳ���
    float leftxf,rightxf;	// ��ǰɨ�����ϣ����Ҷ˵��X����
    int leftx,rightx;		// ��ǰɨ�����ϣ����Ҷ˵��X����
    int leftR,leftG,leftB,rightR,rightG,rightB;	// ��ǰɨ�����ϣ����Ҷ˵����ɫֵ
    int leftStepR,leftStepG,leftStepB;	// ÿ��һ��ɨ���ߣ���˵���ɫ���ӵĴ�С
    int rightStepR,rightStepG,rightStepB;	// ÿ��һ��ɨ���ߣ��Ҷ˵���ɫ���ӵĴ�С
    float leftZValue,rightZValue;	// ��ǰɨ�����ϣ����Ҷ˵�����ֵ
    float leftZStep,rightZStep;		// ÿ��һ��ɨ���ߣ�ɨ�������Ҷ˵�Zֵ�����Ӵ�С
    int spanR,spanG,spanB;			// ��ɨ���ߵ�ʱ�����ص���ɫ
    int spanStepR,spanStepG,spanStepB;	// ��ɨ���ߵ�ʱ����һ�����أ���ɫֵҪ���ӵĴ�С
    float spanZValue,spanZStep;		// ��ɨ���ߵ�ʱ�����ص����ֵ����һ���������ֵ���ӵĴ�С
    DWORD *surfacePtr,*spanBeginPtr,*spanEndPtr;	// ��ɨ����ʱ�����������ں�̨�������е���ʼ������λ��
    float *zBufferPtr,*spanZPtr;	// ��ɨ����ʱ��������������Ȼ������е�λ��

    float leftW,rightW;		// ��ɨ�赱ǰ��ʱ�����Ҷ˵�����ֵ(1.0f/w)
    float leftStepW,rightStepW;	// ÿ��һ��ɨ���ߣ�ɨ�������Ҷ˵�����ֵ(t/w)
    float spanW,spanStepW;		// ��ɨ����ʱ�����(1.0f/w)�Ĳ�ֵ����һ������Ҫ���ӵĴ�С
    float leftTu,leftTv,rightTu,rightTv;	// ��ǰɨ�����ϣ����Ҷ˵����������
    float leftStepTu,leftStepTv,rightStepTu,rightStepTv;	// ÿ��һ��ɨ���ߣ����Ҷ˵������������ӵĴ�С
    float spanTu,spanTv,spanStepTu,spanStepTv;	// ��ɨ����ʱ�����ص������������һ��������������Ҫ���ӵĴ�С

    // ��ȡ��̨��������Z������ָ��
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

        // �������ν��б����޳�
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

        // ����������ڽ�ƽ��֮�����޳�
        if( v1->z < 0 && v2->z < 0 && v3->z < 0 )
            continue;

        // ���������εİ�Χ����
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

        // �ж������εİ�Χ�����Ƿ�����Ļ֮��
        if( !IsRectCollided(rcTriangle,m_pRS->rcViewport) )
            continue;

        // ���������εĸ߶�
        height = v3->pos.y - v1->pos.y;
        if( !height ) continue;

        // ������������ĺ���
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

		// ȡ����������ȵĵ���
        float invW1 = 1.0f / v1->w;
        float invW2 = 1.0f / v2->w;
        float invW3 = 1.0f / v3->w;

        // �������������
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

		// ���������Σ������λ���
        for( int triangleHalf=0; triangleHalf<2; ++triangleHalf )
        {
			// ���ɨ���߽���λ�ô�����Ļ�ײ��������ʣ���ɨ����
            if( spanEnd > m_pRS->rcViewport.bottom )
                spanEnd = m_pRS->rcViewport.bottom;

			// ���ɨ���߿�ʼλ��С����Ļ����������Զ������ϵ�ɨ����
            if (span < m_pRS->rcViewport.top)
            {
				// ��leftx��ʱ��¼����Ծ��ɨ��������
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

            // ����ÿ��ɨ����
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

                // ��������һЩ���ڼ���
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
							float ww = 1.0f / spanW;	// ����͸�ӽ���

							// д�����ֵ
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

				// ��ʼ������һ��
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

			// ����������������ˣ�����ѭ��
            if( triangleHalf > 0 )
                break;

			// ׼�����Ƶڶ�������
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