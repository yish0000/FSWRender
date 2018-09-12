/*
 * -----------------------------------------------------------------------
 *  Name:   FSWGouraudPainter.cpp
 *  Desc:   ���ļ�������һ�����޵���ɫ����
 *  Author: Yish
 *  Date:   2011/7/24
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FSWGouraudPainter.h"
#include "FSWDeviceHelper.h"


/** ���캯��
*/
FSWGouraudPainter::FSWGouraudPainter( SRenderState* pRS )
    : FSWPainter(PAINT_GOURAUD, pRS)
{
}

/** ��������
*/
FSWGouraudPainter::~FSWGouraudPainter()
{
}

/** �����������б�
*/
void FSWGouraudPainter::DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,
                                          UINT nTriangleCount )
{
    SVertex2D *v1,*v2,*v3;  // Ҫ���Ƶ������εĶ���
    RECT rcTriangle; // Ҫ���Ƶ������εİ�Χ����
    int height; // �����εĸ߶�
    float longest;  // ��������ĺ���
    float tmpDiv;   // ������ʱ�洢����
    int span,spanEnd;	// ��Y���ϣ���span��ʼ��spanEnd�������Զ���ɨ���߽������
    float leftdeltaxf,rightdeltaxf;	// ÿ��һ��ɨ���ߣ�ɨ���ߵ����Ҷ˵�����ĳ���
    float leftxf,rightxf;			// ��ǰɨ�����ϣ����Ҷ˵������
    int leftx,rightx;				// ��ǰɨ�����ϣ����Ҷ˵������
    int leftR,leftG,leftB,rightR,rightG,rightB;	// ��ǰɨ�����ϣ����Ҷ˵����ɫֵ
    int leftStepR,leftStepG,leftStepB;	// ÿ��һ��ɨ���ߣ�ɨ������˵���ɫ���ӵĴ�С
    int rightStepR,rightStepG,rightStepB;	// ÿ��һ��ɨ���ߣ�ɨ�����Ҷ˵���ɫ���ӵĴ�С
    float leftZValue,rightZValue;		// ��ǰɨ�����ϣ����Ҷ˵�����
    float leftZStep,rightZStep;			// ÿ��һ��ɨ���ߣ�ɨ�������Ҷ˵�Zֵ���ӵĴ�С
    int spanR,spanG,spanB;				// ��ɨ����ʱ�����ص���ɫ
    int spanStepR,spanStepG,spanStepB;	// ��ɨ����ʱ����һ�����أ���ɫֵҪ���ӵĴ�С
    float spanZValue,spanZStep;			// ��ɨ����ʱ�����ص����ֵ����һ���������ֵ���ӵĴ�С
    DWORD *surfacePtr,*spanBeginPtr,*spanEndPtr;	// ��ɨ����ʱ�����������ں�̨�������е���ʼ�ͽ���λ��
    float *zBufferPtr,*spanZPtr;		// ��ɨ����ʱ����ǰ��������Ȼ�������λ��

    // ��ȡ��̨��������Z������ָ��
    DWORD* lockedSurface = (DWORD*)m_pRS->pBackBuffer->GetBufferData();
    float* lockedZBuffer = (float*)m_pRS->pZBuffer->GetBufferData();

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

		// ���濪ʼ���������Σ������Ժ���������߽������η�Ϊ��������
		// �Ȼ����ϰ벿��
        spanEnd = v2->pos.y;
        span = v1->pos.y;
        leftxf = (float)v1->pos.x;
        rightxf = (float)v1->pos.x;

        leftZValue = v1->z;
        rightZValue = v1->z;

        leftR = rightR = _GetRed(v1->color);
        leftG = rightG = _GetGreen(v1->color);
        leftB = rightB = _GetBlue(v1->color);

		// ��������������ָ���Ƶ���ǰɨ���ߵ�λ��
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

		// ���������Σ������λ���
        for (int triangleHalf=0; triangleHalf<2; ++triangleHalf)
        {
			// ���ɨ���߽���λ�ô�����Ļ�ײ��������ʣ���ɨ����
            if (spanEnd > m_pRS->rcViewport.bottom )
                spanEnd = m_pRS->rcViewport.bottom;

            // ���ɨ���߿�ʼλ��С����Ļ����������Զ������ϵ�ɨ����
            if (span < m_pRS->rcViewport.top)
            {
                // ��leftx��ʱ��¼����Ծ��ɨ���߸���
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

				// �ı���ʼɨ���ߣ��Լ���˵��ֵ
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

				// ���¶�λ��̨����������Ȼ������ĵ�ǰλ��
				surfacePtr += m_pRS->nWidth*leftx;
				zBufferPtr += m_pRS->nWidth*leftx;
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
                spanStepR = (rightR - leftR) * tmpDiv;
                spanStepG = (rightG - leftG) * tmpDiv;
                spanStepB = (rightB - leftB) * tmpDiv;

                // ��������һЩ���ڼ���
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

                // ����ǰɨ����
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

				// ��ʼ������һ��
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

			// ����������������ˣ�����ѭ��
            if( triangleHalf > 0 )
                break;

			// ׼�����������εĵڶ�������
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