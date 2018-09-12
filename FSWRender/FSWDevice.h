/*
 * ------------------------------------------------------------------------
 *  Name:   FSWDevice.h
 *  Desc:   ���ļ�������һ��ͼ���豸����
 *  Author: Yish
 *  Date:   2011/7/20
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSWDEVICE_H__
#define __FSWDEVICE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWTypes.h"
#include "FSWSurface.h"
#include "FSWPainter.h"
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** ͼ���豸
*/
class FSWDevice
{
    typedef std::vector<FSWPostEffect*> PostEffectChain;

private:
    HWND m_hWnd;                        // Ŀ�괰�ڵľ��
    bool m_bRendering;                  // �Ƿ�������Ⱦ
	bool m_bFullscreen;					// �Ƿ�ȫ��ģʽ
	DWORD m_dwWndStyle;					// ��¼ȫ��ǰ���ڵķ��

    FSWSurface* m_pBackBuffer;          // ��̨������
    FSWSurface* m_pZBuffer;             // ��Ȼ�����

    Matrix44 m_mTMatrix[FTS_COUNT];     // �豸�ı任�����б�
    SMaterial m_Material;               // ��ǰ�Ĳ�������
    SLight m_Light;                     // ��ǰ�Ĺ�������
    bool m_bLightEnable;                // �Ƿ�������

    SRenderState m_RenderState;         // ��ǰ�豸����Ⱦ״̬
    FSWPainter* m_pActivePainter;       // ��ǰ�Ļ������
    FSWPainter* m_Painters[PAINT_NUM];  // ��ǰ�Ļ������б�

    std::vector<SVertex2D> m_2DVerts;   // �����任��Ķ�������(��Ļ�ռ�)
    std::vector<SVertex3D> m_ClippedVerts;  // ������ʱ�洢���ú�Ķ����б�
    std::vector<WORD> m_ClippedIndis;   // ������ʱ�洢���ú�������б�

    PostEffectChain m_PostEffects;      // ������Ч��
    FSWSurface* m_pFinalBF;             // �豸���յĺ�̨������

public:
    FSWDevice();
    ~FSWDevice();

	// ��ʼ����Ⱦ�豸
	bool Init( HWND hWnd, UINT nWidth, UINT nHeight, bool bFullscreen );

    // ����һ����ȾĿ��
    FSWSurface* CreateSurface( UINT nWidth,UINT nHeight );

    // ����һ��Z������
    FSWSurface* CreateZBuffer( UINT nWidth,UINT nHeight );

    // ��ȡ��̨������
    FSWSurface* GetBackBuffer() const;

    // ��ȡZ������
    FSWSurface* GetZBuffer() const;

    // ��ȡ��ǰ��Ⱦ����
    FSWSurface* GetRenderTarget() const;

    // ������ȾĿ��
    void SetRenderTarget( FSWSurface* pSurface );

    // ���ƻ�������
    void BeginScene(void);
    void Clear( bool bColor,bool bZBuffer,const Color& clr=Color(0,0,0),float z=1.0f );
    void EndScene(void);
    void Present( const RECT* pSrcRect,const RECT* pDestRect );

    // ����ָ����ͼԪ
    void DrawPrimitive( EPrimitiveType primType,const void* pVB,UINT nVertCount,
        const WORD* pIB,UINT nPrimCount );

    // ���浱ǰ��Ļ���ļ�
    void PrintScreen( const char* filename );

	///////////////////////////////////////////////////////////////////////

	bool AttachPostEffect(EPostEffectType Type);
	void DetachPostEffect(EPostEffectType Type);
	size_t GetNumPostEffects() const { return m_PostEffects.size(); }
	FSWPostEffect* GetPostEffect(size_t idx) { return idx < m_PostEffects.size() ? m_PostEffects[idx] : NULL; }
	void ApplyPostEffect();

    ///////////////////////////////////////////////////////////////////////

    // �����豸�ı任����
    void SetTransform( ETransformType ts,const Matrix44& m );

    // �����豸����ɫģʽ
    void SetShadeMode( EShadeMode mode );

    // ���õ�ǰ�ı����޳�ģʽ
    void SetCullMode( ECullMode mode );

    // �����豸�����ģʽ
    void SetFillMode( EFillMode mode );

    // ��������Ĺ���ģʽ
    void SetTextureFilter( ETextureFilter filter );

    // ���õ�ǰ����������
    void SetTexture( FSWTexture* pTexture );

    // ���ò�������
    void SetMaterial( const SMaterial& material );

    // ���ù�������
    void SetLight( const SLight& light );

    // ����/�رչ���
    void SetLightEnable( bool bEnable );

    // ����/�رհ�͸�����
    void SetAlphaBlend( bool bEnable );

    ///////////////////////////////////////////////////////////////////////

    // ��ȡ�豸�ı任����
    void GetTransform( ETransformType ts,Matrix44& m );

	// ��ȡ�豸�Ŀ�߱�
	float GetAspectRatio() const;

    // ��ȡ�豸����ɫģʽ
    EShadeMode GetShadeMode(void) const { return m_RenderState.ShadeMode; }

    // ��ȡ�豸�ı����޳�ģʽ
    ECullMode GetCullMode(void) const { return m_RenderState.CullMode; }

    // ��ȡ�豸�����ģʽ
    EFillMode GetFillMode(void) const { return m_RenderState.FillMode; }

    // ��ȡ��ǰ����������
    FSWTexture* GetTexture() const { return m_RenderState.pTexture; }

    // ��ȡ��ǰ���������ģʽ
    ETextureFilter GetTextureFilter() const { return m_RenderState.TexFilter; }

    // ��ȡ��ǰ�Ĳ�������
    void GetMaterial( SMaterial& material ) const { memcpy(&material, &m_Material, sizeof(SMaterial)); }

    // ��ȡ��ǰ�Ĺ�������
    void GetLight( SLight& light ) const { memcpy(&light, &m_Light, sizeof(SLight)); }

    // ��ǰ�Ƿ�������
    bool IsLightEnable() const { return m_bLightEnable; }

    // ��ǰ�Ƿ�����͸�����
    bool IsAlphaEnable() const { return m_RenderState.bAlphaBlend; }

    ///////////////////////////////////////////////////////////////////////

	// ��Ӧ���ڴ�С�仯
	bool ResetDevice(int width, int height, bool bFullscreen);

	// ��ǰ�Ƿ�Ϊȫ��ģʽ
	bool IsFullscreen() const { return m_bFullscreen; }

private:

    // ����ָ����������ɫ(��д��Ȼ�����)
    void DrawPixel( UINT x,UINT y,const Color& clr );

    // ���ƶ�ά��(��д��Ȼ�����)
    void Draw2DLine( const POINT& vStart,const POINT& vEnd,const Color& clr );

    // ������ά��
    void Draw3DLine( const SVertex3D* from,const SVertex3D* to );

    // ����������������ɼ���ֱ��
    void DrawClippedLineList( const SVertex3D* pVerts, UINT nVertCount,
        const WORD* pIndis, UINT nPrimCount );

    // �����������޳����ɼ���������
    void DrawClippedTriangleList( const SVertex3D* pVerts,UINT nVertCount,
        const WORD* pIndis,UINT triangleCount );

    // �ı��豸�Ļ�����
    void UpdatePainter();

	// ����ȫ��ģʽ
	void ToggleFullscreen(bool bFullscreen);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWDEVICE_H__