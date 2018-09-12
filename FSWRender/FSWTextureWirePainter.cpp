/*
 * ------------------------------------------------------------------------
 *  Name:   FSWTextureWirePainter.cpp
 *  Desc:   本文件实现了一个纹理线框模式的高罗德着色器。
 *  Author: Yish
 *  Date:   2013/3/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FSWTextureWirePainter.h"
#include "FSWDeviceHelper.h"


/** 构造函数
*/
FSWTextureWirePainter::FSWTextureWirePainter( SRenderState* pRS )
    : FSWPainter(PAINT_TEXTURE_WIRE, pRS)
{
}

/** 析构函数
*/
FSWTextureWirePainter::~FSWTextureWirePainter()
{
}

/** 绘制三角形列表
*/
void FSWTextureWirePainter::DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,
                                              UINT nTriangleCount )
{
	SVertex2D *from, *to;
	SVertex2D *v[3];
	int steps;
	float incx, incy, x, y;
	int startR, startG, startB;
	int stepR, stepG, stepB;
	float startZ, stepZ;
	float tmpDiv;
	float startTu, startTv;
	float stepTu, stepTv;
	bool bY = false;

	// 获取后台缓冲区和深度缓冲区指针
	DWORD* lockedSurface = (DWORD*)m_pRS->pBackBuffer->GetBufferData();
	float* lockedZBuffer = (float*)m_pRS->pZBuffer->GetBufferData();
	DWORD* lockedTexture = (DWORD*)m_pRS->pTexture->GetBufferData();
	UINT textureWidth = m_pRS->pTexture->GetWidth();
	UINT textureHeight = m_pRS->pTexture->GetHeight();

	for(UINT i=0; i<nTriangleCount; ++i)
	{
		v[0] = &pVerts[*pIndis++];
		v[1] = &pVerts[*pIndis++];
		v[2] = &pVerts[*pIndis++];

		// 对三角形进行背面剪裁
		if( m_pRS->CullMode == FCM_CCW )
		{
			int z = (v[2]->pos.x - v[0]->pos.x) * (v[2]->pos.y - v[1]->pos.y) -
				(v[2]->pos.y - v[0]->pos.y) * (v[2]->pos.x - v[1]->pos.x);
			if( z < 0 ) continue;
		}
		else if( m_pRS->CullMode == FCM_CW )
		{
			int z = (v[2]->pos.x - v[0]->pos.x) * (v[2]->pos.y - v[1]->pos.y) -
				(v[2]->pos.y - v[0]->pos.y) * (v[2]->pos.x - v[1]->pos.x);
			if( z > 0 ) continue;
		}

		// 绘制三角形的三条边
		for( UINT l=0; l<3; ++l )
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

			if( abs(to->pos.x - from->pos.x) > abs(to->pos.y - from->pos.y) )
			{
				steps = abs(to->pos.x - from->pos.x);
				bY = false;
			}
			else
			{
				steps = abs(to->pos.y - from->pos.y);
				bY = true;
			}

			tmpDiv = 1.0f / (float)steps;
			incx = (float)(to->pos.x - from->pos.x) * tmpDiv;
			incy = (float)(to->pos.y - from->pos.y) * tmpDiv;

			startR = _GetRed(from->color);
			startG = _GetGreen(from->color);
			startB = _GetBlue(from->color);
			stepR = (_GetRed(to->color) - startR) * tmpDiv;
			stepG = (_GetGreen(to->color) - startG) * tmpDiv;
			stepB = (_GetBlue(to->color) - startB) * tmpDiv;

			startZ = from->z;
			stepZ = (to->z - from->z) * tmpDiv;
			startTu = from->tex.x;
			startTv = from->tex.y;
			stepTu = (to->tex.x - startTu) * tmpDiv;
			stepTv = (to->tex.y - startTv) * tmpDiv;

			POINT newV1, newV2;
			if( Clip2DLine(m_pRS->rcViewport, from->pos, to->pos, newV1, newV2) )
			{
				x = (float)newV1.x;
				y = (float)newV1.y;

				// 检测剪裁后，跳过多少个像素
				int tmp1 = bY ? abs(newV1.y - from->pos.y) : abs(newV1.x - from->pos.x);
				int tmp2 = bY ? abs(newV2.y - to->pos.y) : abs(newV2.x - to->pos.x);

				// 计算新的起始数据
				startR += tmp1 * stepR;
				startG += tmp1 * stepG;
				startB += tmp1 * stepB;
				startZ += tmp1 * stepZ;
				startTu += stepTu * tmp1;
				startTv += stepTv * tmp1;

				steps -= tmp1 + tmp2;
				while( steps-- > 0 )
				{
					UINT nx = (UINT)x;
					UINT ny = (UINT)y;

					if( startZ < lockedZBuffer[ny*m_pRS->nWidth+nx] )
					{
						lockedZBuffer[ny*m_pRS->nWidth+nx] = startZ;

						lockedSurface[ny*m_pRS->nWidth+nx] = _Modulate(
							_MakeColor(startR, startG, startB, 0xff00),
							_Sampler2D_Point(lockedTexture, textureWidth, textureHeight, startTu, startTv)
							);
					}

					x += incx;
					y += incy;

					startR += stepR;
					startG += stepG;
					startB += stepB;
					startZ += stepZ;
					startTu += stepTu;
					startTv += stepTv;
				}
			}
		}
	}
}