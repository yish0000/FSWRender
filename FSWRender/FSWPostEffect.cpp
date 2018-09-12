/*
 * ------------------------------------------------------------------------
 *  Name:   FSWPostEffect.cpp
 *  Desc:   本文件实现了后期特效基类。
 *  Author: Yish
 *  Date:   2013/3/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FSWPostEffect.h"

#include "FSWPostASCII.h"
#include "FSWPostBloom.h"
#include "FSWPostBW.h"
#include "FSWPostDither.h"
#include "FSWPostEmbossed.h"
#include "FSWPostGlass.h"
#include "FSWPostInvert.h"
#include "FSWPostMotionBlur.h"
#include "FSWPostNightVision.h"
#include "FSWPostOldMovie.h"
#include "FSWPostOldTV.h"
#include "FSWPostRadialBlur.h"
#include "FSWPostTiling.h"


/** 构造函数
*/
FSWPostEffect::FSWPostEffect( EPostEffectType Type, FSWSurface* pSurface )
    : m_Type(Type), m_pPrevSurface(pSurface)
{
    m_pNextSurface = new FSWSurface();
    m_pNextSurface->Create(pSurface->GetWidth(), pSurface->GetHeight(), pSurface->GetPixelFormat());
}

/** 析构函数
*/
FSWPostEffect::~FSWPostEffect()
{
    SAFE_DELETE( m_pNextSurface );
}

/** 重新创建surface
*/
void FSWPostEffect::OnResetDevice()
{
	if( m_pNextSurface )
		m_pNextSurface->Create(m_pPrevSurface->GetWidth(), m_pPrevSurface->GetHeight(), m_pPrevSurface->GetPixelFormat());
}

/** 创建指定类型的后期特效
*/
FSWPostEffect* FSWPostEffect::CreatePostEffect(EPostEffectType Type, FSWSurface* pSurface)
{
	switch( Type )
	{
	case POST_ASCII: return new FSWPostASCII(pSurface);
	case POST_BLOOM: return new FSWPostBloom(pSurface);
	case POST_BW: return new FSWPostBW(pSurface);
	case POST_DITHER: return new FSWPostDither(pSurface);
	case POST_EMBOSSED: return new FSWPostEmbossed(pSurface);
	case POST_GLASS: return new FSWPostGlass(pSurface);
	case POST_INVERT: return new FSWPostInvert(pSurface);
	case POST_MOTIONBLUR: return new FSWPostMotionBlur(pSurface);
	case POST_NIGHTVISION: return new FSWPostNightVision(pSurface);
	case POST_OLDMOVIE: return new FSWPostOldMovie(pSurface);
	case POST_OLDTV: return new FSWPostOldTV(pSurface);
	case POST_RADIALBLUR: return new FSWPostRadialBlur(pSurface);
	case POST_TILING: return new FSWPostTiling(pSurface);
	default: ASSERT(0); return NULL;
	}
}