/*
 * ------------------------------------------------------------------------
 *  Name:   FSWPostEffect.h
 *  Desc:   ���ļ�������һ�����ڴ���Ч�����ࡣ
 *  Author: Yish
 *  Date:   2013/3/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSW_POSTEFFECT_H__
#define __FSW_POSTEFFECT_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "Common.h"
#include "FSWSurface.h"

///////////////////////////////////////////////////////////////////////////


enum EPostEffectType
{
    POST_BW,                // �ڰ�Ч��
    POST_INVERT,            // ��תЧ��
    POST_BLOOM,             // ���Ч��
    POST_MOTIONBLUR,        // �˶�ģ��
    POST_OLDTV,             // �Ͼɵ���Ч��
    POST_OLDMOVIE,          // �Ͼɵ�ӰЧ��
    POST_GLASS,             // �����˾�Ч��
    POST_CARTOON,           // ��ͨЧ��
    POST_ASCII,             // �ַ�ͼ��Ч��
    POST_HDR,               // �߶�̬����Ч��
    POST_TILING,            // ��״��ȾЧ��
    POST_NIGHTVISION,       // ҹ��Ч��
    POST_RADIALBLUR,        // ����ģ��
    POST_EMBOSSED,          // ����Ч��
    POST_DITHER,            // �ڰ׵�Ч��
    POST_HALFTONE,          // �ڰ׿�Ч��
};

/** ���ڴ�����Ч����
*/
class FSWPostEffect
{
public:
    FSWPostEffect( EPostEffectType Type, FSWSurface* pSurface );
    virtual ~FSWPostEffect();

    // Ӧ�ú��ڴ�����Ч
    virtual void ApplyEffect() = 0;

    FSWSurface* GetPrevSurface() { return m_pPrevSurface; }
    FSWSurface* GetNextSurface() { return m_pNextSurface; }

    // ����ǰһ��ͼ�λ���
    void SetPrevSurface( FSWSurface* pSurface ) { m_pPrevSurface = pSurface; }

    // ��ȡ������Ч������
    EPostEffectType GetEffectType() const { return m_Type; }

	// ���´���surface
	void OnResetDevice();

	// ����ָ�����͵ĺ�����Ч
	static FSWPostEffect* CreatePostEffect(EPostEffectType Type, FSWSurface* pSurface);

protected:
    EPostEffectType m_Type;         // ������Ч������
    FSWSurface* m_pPrevSurface;     // ��һ��ͼ�λ���
    FSWSurface* m_pNextSurface;     // ��һ��ͼ�λ���
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSW_POSTEFFECT_H__