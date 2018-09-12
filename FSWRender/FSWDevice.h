/*
 * ------------------------------------------------------------------------
 *  Name:   FSWDevice.h
 *  Desc:   本文件定义了一个图形设备对象。
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


/** 图形设备
*/
class FSWDevice
{
    typedef std::vector<FSWPostEffect*> PostEffectChain;

private:
    HWND m_hWnd;                        // 目标窗口的句柄
    bool m_bRendering;                  // 是否正在渲染
	bool m_bFullscreen;					// 是否全屏模式
	DWORD m_dwWndStyle;					// 记录全屏前窗口的风格

    FSWSurface* m_pBackBuffer;          // 后台缓冲区
    FSWSurface* m_pZBuffer;             // 深度缓冲区

    Matrix44 m_mTMatrix[FTS_COUNT];     // 设备的变换矩阵列表
    SMaterial m_Material;               // 当前的材质数据
    SLight m_Light;                     // 当前的光照数据
    bool m_bLightEnable;                // 是否开启光照

    SRenderState m_RenderState;         // 当前设备的渲染状态
    FSWPainter* m_pActivePainter;       // 当前的活动绘制器
    FSWPainter* m_Painters[PAINT_NUM];  // 当前的绘制器列表

    std::vector<SVertex2D> m_2DVerts;   // 经过变换后的顶点数据(屏幕空间)
    std::vector<SVertex3D> m_ClippedVerts;  // 用于暂时存储剪裁后的顶点列表
    std::vector<WORD> m_ClippedIndis;   // 用于暂时存储剪裁后的索引列表

    PostEffectChain m_PostEffects;      // 后期特效链
    FSWSurface* m_pFinalBF;             // 设备最终的后台缓冲区

public:
    FSWDevice();
    ~FSWDevice();

	// 初始化渲染设备
	bool Init( HWND hWnd, UINT nWidth, UINT nHeight, bool bFullscreen );

    // 创建一个渲染目标
    FSWSurface* CreateSurface( UINT nWidth,UINT nHeight );

    // 创建一个Z缓冲区
    FSWSurface* CreateZBuffer( UINT nWidth,UINT nHeight );

    // 获取后台缓冲区
    FSWSurface* GetBackBuffer() const;

    // 获取Z缓冲区
    FSWSurface* GetZBuffer() const;

    // 获取当前渲染表面
    FSWSurface* GetRenderTarget() const;

    // 设置渲染目标
    void SetRenderTarget( FSWSurface* pSurface );

    // 控制绘制流程
    void BeginScene(void);
    void Clear( bool bColor,bool bZBuffer,const Color& clr=Color(0,0,0),float z=1.0f );
    void EndScene(void);
    void Present( const RECT* pSrcRect,const RECT* pDestRect );

    // 绘制指定的图元
    void DrawPrimitive( EPrimitiveType primType,const void* pVB,UINT nVertCount,
        const WORD* pIB,UINT nPrimCount );

    // 保存当前屏幕到文件
    void PrintScreen( const char* filename );

	///////////////////////////////////////////////////////////////////////

	bool AttachPostEffect(EPostEffectType Type);
	void DetachPostEffect(EPostEffectType Type);
	size_t GetNumPostEffects() const { return m_PostEffects.size(); }
	FSWPostEffect* GetPostEffect(size_t idx) { return idx < m_PostEffects.size() ? m_PostEffects[idx] : NULL; }
	void ApplyPostEffect();

    ///////////////////////////////////////////////////////////////////////

    // 设置设备的变换矩阵
    void SetTransform( ETransformType ts,const Matrix44& m );

    // 设置设备的着色模式
    void SetShadeMode( EShadeMode mode );

    // 设置当前的背面剔除模式
    void SetCullMode( ECullMode mode );

    // 设置设备的填充模式
    void SetFillMode( EFillMode mode );

    // 设置纹理的过滤模式
    void SetTextureFilter( ETextureFilter filter );

    // 设置当前的纹理数据
    void SetTexture( FSWTexture* pTexture );

    // 设置材质数据
    void SetMaterial( const SMaterial& material );

    // 设置光照数据
    void SetLight( const SLight& light );

    // 开启/关闭光照
    void SetLightEnable( bool bEnable );

    // 开启/关闭半透明混合
    void SetAlphaBlend( bool bEnable );

    ///////////////////////////////////////////////////////////////////////

    // 获取设备的变换矩阵
    void GetTransform( ETransformType ts,Matrix44& m );

	// 获取设备的宽高比
	float GetAspectRatio() const;

    // 获取设备的着色模式
    EShadeMode GetShadeMode(void) const { return m_RenderState.ShadeMode; }

    // 获取设备的背面剔除模式
    ECullMode GetCullMode(void) const { return m_RenderState.CullMode; }

    // 获取设备的填充模式
    EFillMode GetFillMode(void) const { return m_RenderState.FillMode; }

    // 获取当前的纹理数据
    FSWTexture* GetTexture() const { return m_RenderState.pTexture; }

    // 获取当前的纹理过滤模式
    ETextureFilter GetTextureFilter() const { return m_RenderState.TexFilter; }

    // 获取当前的材质数据
    void GetMaterial( SMaterial& material ) const { memcpy(&material, &m_Material, sizeof(SMaterial)); }

    // 获取当前的光照数据
    void GetLight( SLight& light ) const { memcpy(&light, &m_Light, sizeof(SLight)); }

    // 当前是否开启光照
    bool IsLightEnable() const { return m_bLightEnable; }

    // 当前是否开启半透明混合
    bool IsAlphaEnable() const { return m_RenderState.bAlphaBlend; }

    ///////////////////////////////////////////////////////////////////////

	// 相应窗口大小变化
	bool ResetDevice(int width, int height, bool bFullscreen);

	// 当前是否为全屏模式
	bool IsFullscreen() const { return m_bFullscreen; }

private:

    // 设置指定的像素颜色(不写深度缓冲区)
    void DrawPixel( UINT x,UINT y,const Color& clr );

    // 绘制二维线(不写深度缓冲区)
    void Draw2DLine( const POINT& vStart,const POINT& vEnd,const Color& clr );

    // 绘制三维线
    void Draw3DLine( const SVertex3D* from,const SVertex3D* to );

    // 本函数用于提出不可见的直线
    void DrawClippedLineList( const SVertex3D* pVerts, UINT nVertCount,
        const WORD* pIndis, UINT nPrimCount );

    // 本函数用于剔除不可见的三角形
    void DrawClippedTriangleList( const SVertex3D* pVerts,UINT nVertCount,
        const WORD* pIndis,UINT triangleCount );

    // 改变设备的绘制器
    void UpdatePainter();

	// 处理全屏模式
	void ToggleFullscreen(bool bFullscreen);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWDEVICE_H__