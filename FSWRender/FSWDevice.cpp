/*
 * -----------------------------------------------------------------------
 *  Name:   FSWDevice.cpp
 *  Desc:   本文件定义了一个图形设备对象。
 *  Author: Yish
 *  Date:   2011/7/23
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FSWDevice.h"
#include "FSWDeviceHelper.h"
#include "LogFile.h"


/** 构造函数
*/
FSWDevice::FSWDevice() : m_hWnd(NULL), m_bRendering(false), m_bFullscreen(false),
	m_dwWndStyle(0), m_pBackBuffer(NULL), m_pZBuffer(NULL), m_pFinalBF(NULL),
	m_bLightEnable(false), m_pActivePainter(NULL)
{
}

/** 析构函数
*/
FSWDevice::~FSWDevice(void)
{
	size_t i;
	for( i=0;i<m_PostEffects.size();i++ ) SAFE_DELETE(m_PostEffects[i]);
    for( i=0;i<PAINT_NUM;i++ ) SAFE_DELETE(m_Painters[i]);
    SAFE_DELETE( m_pBackBuffer );
    SAFE_DELETE( m_pZBuffer );
}

/** 初始化渲染设备
@Param 窗口句柄
@Param 窗口的宽度
@Param 窗口的高度
@Param 是否是全屏模式
*/
bool FSWDevice::Init( HWND hWnd, UINT nWidth, UINT nHeight, bool bFullscreen )
{
	m_hWnd = hWnd;
	m_bRendering = false;
	m_bFullscreen = bFullscreen;
	m_dwWndStyle = GetWindowLong(hWnd, GWL_STYLE);

	// 创建日志文件
	CreateDirectory("Logs", NULL);
	LogFile::GetInstance().CreateLog("Logs\\FSW.log", "Fairy Software Renderer Log file");

	if( bFullscreen && !CheckResolution(nWidth, nHeight) )
	{
		ASSERT(0);
		FSW_LOG("FSWDevice::Init, Invalid resolution for fullscreen mode!");
		return false;
	}

	// 创建一个后台缓冲区
	m_pBackBuffer = new FSWSurface();
	m_pBackBuffer->Create( nWidth,nHeight,FPF_A8R8G8B8 );
	m_pFinalBF = m_pBackBuffer;

	// 创建一个深度缓冲区
	m_pZBuffer = new FSWSurface();
	m_pZBuffer->Create( nWidth,nHeight,FPF_D32 );

	// 初始化变换矩阵
	m_mTMatrix[FTS_WORLD].Identity();
	m_mTMatrix[FTS_VIEW].Identity();
	m_mTMatrix[FTS_PROJECTION].MakePerspProj( FSW_PI/4.0f,(float)nWidth/nHeight,1.0f,1000.0f );
	m_mTMatrix[FTS_TRANSFORM] = m_mTMatrix[FTS_WORLD]*m_mTMatrix[FTS_VIEW]*m_mTMatrix[FTS_PROJECTION];

	// 初始化光照和材质数据
	memset( &m_Material,0,sizeof(SMaterial) );
	memset( &m_Light,0,sizeof(SLight) );
	m_bLightEnable = false;

	// 初始化渲染状态
	m_RenderState.pBackBuffer = m_pBackBuffer;
	m_RenderState.pZBuffer = m_pZBuffer;
	m_RenderState.nWidth = m_pBackBuffer->GetWidth();
	m_RenderState.nHeight = m_pBackBuffer->GetHeight();
	m_RenderState.pTexture = NULL;
	m_RenderState.CullMode = FCM_CCW;
	m_RenderState.FillMode = FFM_SOLID;
	m_RenderState.rcViewport.left = 0;
	m_RenderState.rcViewport.top = 0;
	m_RenderState.rcViewport.right = m_pBackBuffer->GetWidth() - 1;
	m_RenderState.rcViewport.bottom = m_pBackBuffer->GetHeight() - 1;
	m_RenderState.TexFilter = FTF_POINT;
	m_RenderState.bAlphaBlend = false;

	// 初始化着色器列表
	for( int i=0;i<PAINT_NUM;i++ )
	{
		m_Painters[i] = FSWPainter::CreatePainter((EPainterType)i, &m_RenderState);
	}

	m_pActivePainter = m_Painters[PAINT_GOURAUD];

	// 调整全屏模式
	if( m_bFullscreen )
	{
		ToggleFullscreen(m_bFullscreen);
	}

	return true;
}

/** 创建一个渲染目标
*/
FSWSurface* FSWDevice::CreateSurface( UINT nWidth,UINT nHeight )
{
    FSWSurface* surface = new FSWSurface();
    surface->Create( nWidth,nHeight,FPF_A8R8G8B8 );
    return surface;
}

/** 创建一个深度缓冲区
*/
FSWSurface* FSWDevice::CreateZBuffer( UINT nWidth,UINT nHeight )
{
    FSWSurface* surface = new FSWSurface();
    surface->Create( nWidth,nHeight,FPF_D32 );
    return surface;
}

// 获取后台缓冲区
FSWSurface* FSWDevice::GetBackBuffer() const
{
    return m_pBackBuffer;
}

// 获取Z缓冲区
FSWSurface* FSWDevice::GetZBuffer() const
{
    return m_pZBuffer;
}

// 获取当前渲染目标
FSWSurface* FSWDevice::GetRenderTarget() const
{
    return m_RenderState.pBackBuffer;
}

// 设置渲染目标
void FSWDevice::SetRenderTarget( FSWSurface* pSurface )
{
    if( !m_bRendering )
        m_RenderState.pBackBuffer = pSurface;
    else
    {
        ASSERT(0);
		FSW_LOG("FSWDevice::SetRenderTarget, Can't change the render target when device is rendering!");
    }
}

/** 开始渲染场景
*/
void FSWDevice::BeginScene(void)
{
    ASSERT( !m_bRendering );
    m_bRendering = true;
}

/** 清理指定的缓冲区
*/
void FSWDevice::Clear( bool bColor,bool bZBuffer,const Color& clr,float z )
{
    if( bColor )
    {
        DWORD* ptr = (DWORD*)m_pBackBuffer->GetBufferData();
        DWORD* buf_end = ptr + m_pBackBuffer->GetWidth()*m_pBackBuffer->GetHeight();
        while( ptr != buf_end ) *ptr++ = clr.val;
    }

    if( bZBuffer )
    {
        float* ptr = (float*)m_pZBuffer->GetBufferData();
        float* buf_end = ptr + m_pZBuffer->GetWidth()*m_pZBuffer->GetHeight();
        while( ptr != buf_end ) *ptr++ = z;
    }
}

/** 结束渲染场景
*/
void FSWDevice::EndScene(void)
{
    assert( m_bRendering );
    m_bRendering = false;
}

/** 交换前后台缓冲区
*/
void FSWDevice::Present( const RECT* pSrcRect,const RECT* pDestRect )
{
    RECT rc;
    HDC hdc = GetDC( m_hWnd );

    if( hdc )
    {
        if( pDestRect )
            memcpy( &rc,pDestRect,sizeof(RECT) );
        else
            GetClientRect( m_hWnd,&rc );

        BITMAPV4HEADER bi;
        memset( &bi,0,sizeof(BITMAPV4HEADER) );
        bi.bV4Size = sizeof(BITMAPV4HEADER);
        bi.bV4BitCount = 32;
        bi.bV4Planes = 1;
        bi.bV4Width = m_pFinalBF->GetWidth();
        bi.bV4Height = -(LONG)m_pFinalBF->GetHeight();
        bi.bV4V4Compression = BI_BITFIELDS;
        bi.bV4AlphaMask = 0xff000000;
        bi.bV4RedMask = 0x00ff0000;
        bi.bV4GreenMask = 0x0000ff00;
        bi.bV4BlueMask = 0x000000ff;

        if( pSrcRect )
        {
            StretchDIBits( hdc,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,pSrcRect->left,pSrcRect->top,
                pSrcRect->right-pSrcRect->left,pSrcRect->bottom-pSrcRect->top,m_pFinalBF->GetBufferData(),
                (const BITMAPINFO*)&bi,DIB_RGB_COLORS,SRCCOPY );
        }
        else
        {
            StretchDIBits( hdc,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,0,0,
                m_pFinalBF->GetWidth(),m_pFinalBF->GetHeight(),m_pFinalBF->GetBufferData(),
                (const BITMAPINFO*)&bi,DIB_RGB_COLORS,SRCCOPY );
        }
    }

    ReleaseDC( m_hWnd,hdc );
}

/** 截取当前屏幕到文件
*/
void FSWDevice::PrintScreen( const char* filename )
{
    FSWTexture::SaveSurfaceToFile(m_pFinalBF, filename);
}

/** 设置设备的变换矩阵
*/
void FSWDevice::SetTransform( ETransformType ts,const Matrix44& m )
{
    if( ts >= FTS_WORLD && ts < FTS_COUNT )
        m_mTMatrix[ts] = m;
}

/** 获取设备的变换矩阵
*/
void FSWDevice::GetTransform( ETransformType ts,Matrix44& m )
{
    if( ts >= FTS_WORLD && ts < FTS_COUNT )
        m = m_mTMatrix[ts];
}

/** 设置设备的着色模式
*/
void FSWDevice::SetShadeMode( EShadeMode mode )
{
    m_RenderState.ShadeMode = mode;
    UpdatePainter();
}

/** 设置设备的背面剔除模式
*/
void FSWDevice::SetCullMode( ECullMode mode )
{
    m_RenderState.CullMode = mode;
}

/** 设置设备的填充模式
*/
void FSWDevice::SetFillMode( EFillMode mode )
{
    m_RenderState.FillMode = mode;
    UpdatePainter();
}

/** 设置当前的纹理数据
*/
void FSWDevice::SetTexture( FSWTexture* pTexture )
{
    m_RenderState.pTexture = pTexture;
    UpdatePainter();
}

/** 设置纹理的过滤模式
*/
void FSWDevice::SetTextureFilter( ETextureFilter filter )
{
    m_RenderState.TexFilter = filter;
}

/** 设置当前的材质数据
*/
void FSWDevice::SetMaterial( const SMaterial& material )
{
    memcpy( &m_Material,&material,sizeof(SMaterial) );
}

/** 设置当前的光照数据
*/
void FSWDevice::SetLight( const SLight& light )
{
    memcpy( &m_Light,&light,sizeof(SLight) );
}

/** 开启/关闭光照效果
*/
void FSWDevice::SetLightEnable( bool bEnable )
{
    m_bLightEnable = bEnable;
}

/** 开启关闭半透明混合
*/
void FSWDevice::SetAlphaBlend( bool bEnable )
{
    m_RenderState.bAlphaBlend = bEnable;
}

/** 获取设备的宽高比
*/
float FSWDevice::GetAspectRatio() const
{
	return (float)m_RenderState.nWidth / m_RenderState.nHeight;
}

/** 绘制指定的像素
*/
void FSWDevice::DrawPixel( UINT x,UINT y,const Color& clr )
{
    FSWSurface* pBackBuffer = m_RenderState.pBackBuffer;
    if( x < pBackBuffer->GetWidth() && y < pBackBuffer->GetHeight() )
    {
        DWORD* buf = (DWORD*)pBackBuffer->GetBufferData();
        buf[y*pBackBuffer->GetWidth()+x] = clr.val;
    }
}

/** 绘制一个二维线
*/
void FSWDevice::Draw2DLine( const POINT& vStart,const POINT& vEnd,const Color& clr )
{
    FSWSurface* pBackBuffer = m_RenderState.pBackBuffer;

    // 获取像素缓冲区
    DWORD* buf = (DWORD*)pBackBuffer->GetBufferData();
    UINT pitch = pBackBuffer->GetWidth();

    POINT pt1, pt2;
    if( Clip2DLine(m_RenderState.rcViewport,vStart,vEnd,pt1,pt2) )
    {
        float incx,incy,x,y;
        int steps;

        if( abs(pt2.x-pt1.x) > abs(pt2.y-pt1.y) )
            steps = abs(pt2.x - pt1.x);
        else
            steps = abs(pt2.y - pt1.y);

        incx = (float)(pt2.x - pt1.x) / steps;
        incy = (float)(pt2.y - pt1.y) / steps;
        x = (float)pt1.x;
        y = (float)pt1.y;

        for( int i=0;i<steps;++i )
        {
            _WritePixel( buf,pitch,(UINT)x,(UINT)y,clr );
            x += incx;
            y += incy;
        }
    }
}

/** 绘制指定的三维线段
*/
void FSWDevice::Draw3DLine( const SVertex3D* from,const SVertex3D* to )
{
    POINT from2D, to2D;
    float fromZf, toZf;
    float fromWf, toWf;
    int steps;
    float incx, incy, x, y;
    int startR, startG, startB;
    int stepR, stepG, stepB;
    float startZ, stepZ;
    float tmpDiv;
    bool bY = false;

    // 获取后台缓冲区和深度缓冲区指针
    DWORD* lockedSurface = (DWORD*)m_pBackBuffer->GetBufferData();
    float* lockedZBuffer = (float*)m_pBackBuffer->GetBufferData();

    // 将三维顶点变换到屏幕空间
    WorldToScreen(from->pos, m_mTMatrix[FTS_TRANSFORM], m_RenderState.nWidth,
        m_RenderState.nHeight, from2D, fromZf, fromWf);
    WorldToScreen(to->pos, m_mTMatrix[FTS_TRANSFORM], m_RenderState.nWidth,
        m_RenderState.nHeight, to2D, toZf, toWf);

    if( abs(to2D.x-from2D.x) > abs(to2D.y-from2D.y) )
    {
        steps = abs(to2D.x - from2D.x);
        bY = false;
    }
    else
    {
        steps = abs(to2D.y - from2D.y);
        bY = true;
    }

    tmpDiv = 1.0f / (float)steps;
    incx = (float)(to2D.x - from2D.x) * tmpDiv;
    incy = (float)(to2D.y - from2D.y) * tmpDiv;

    startR = _GetRed(from->color);
    startG = _GetGreen(from->color);
    startB = _GetBlue(from->color);
    stepR = (_GetRed(to->color) - startR) * tmpDiv;
    stepG = (_GetGreen(to->color) - startG) * tmpDiv;
    stepB = (_GetBlue(to->color) - startB) * tmpDiv;
    startZ = fromZf;
    stepZ = (toZf - fromZf) * tmpDiv;

    POINT newV1, newV2;
    if( Clip2DLine(m_RenderState.rcViewport, from2D, to2D, newV1, newV2) )
    {
        x = (float)newV1.x;
        y = (float)newV1.y;

        // 检测剪裁后需跳过多少个像素
        int tmp1 = bY ? abs(newV1.y - from2D.y) : abs(newV1.x - from2D.x);
        int tmp2 = bY ? abs(newV2.y - to2D.y) : abs(newV2.x - to2D.x);

        // 计算新的起始数据
        startR += tmp1 * stepR;
        startG += tmp1 * stepG;
        startB += tmp1 * stepB;
        startZ += tmp1 * stepZ;

        steps -= tmp1 + tmp2;
        while( steps-- > 0 )
        {
            UINT nx = (UINT)x;
            UINT ny = (UINT)y;

            if( startZ < lockedZBuffer[ny*m_RenderState.nWidth+nx] )
            {
                lockedZBuffer[ny*m_RenderState.nWidth+nx] = startZ;
                lockedSurface[ny*m_RenderState.nWidth+nx] = _MakeColor(startR, startG, startB, 0xff);
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

/** 绘制指定的图元
@Param 要绘制的图元类型
@Param 顶点缓冲区
@Param 顶点的数目
@Param 索引缓冲区
@Param 要绘制图元的数目
*/
void FSWDevice::DrawPrimitive( EPrimitiveType primType,const void* pVB,UINT nVertCount,
                               const WORD* pIB,UINT nPrimCount )
{
    SVertex3D* pVerts = (SVertex3D*)pVB;

    // 混合三个矩阵
    m_mTMatrix[FTS_TRANSFORM] = m_mTMatrix[FTS_WORLD]*m_mTMatrix[FTS_VIEW]*
        m_mTMatrix[FTS_PROJECTION];

    switch( primType )
    {
    case FPT_LINELIST:
        DrawClippedLineList(pVerts, nVertCount, pIB, nPrimCount);
        break;
    case FPT_TRIANGLELIST:
        DrawClippedTriangleList( pVerts,nVertCount,pIB,nPrimCount );
        break;
    default: ASSERT(0);
    }
}

/** 绘制被视锥剪裁后的线段列表
*/
void FSWDevice::DrawClippedLineList( const SVertex3D* pVerts, UINT nVertCount,
                                     const WORD* pIndis, UINT nPrimCount )
{
    Plane frustum[6];

    // 获取当前视锥的六个平面
    GetFrustumPlane(m_mTMatrix[FTS_VIEW]*m_mTMatrix[FTS_PROJECTION], frustum);

    // 将六个面用世界矩阵进行逆变换，相当于将顶点变换到
    // 世界空间后再进行剪裁
    Matrix44 tpWorld;
    tpWorld.TransposeOf( m_mTMatrix[FTS_WORLD] );
    for( int m=0;m<6;++m )
    {
        Vector4 vec(frustum[m].m_vN, frustum[m].m_fD);
        tpWorld.Transform(vec);
        frustum[m].m_vN.x = vec.x;
        frustum[m].m_vN.y = vec.y;
        frustum[m].m_vN.z = vec.z;
        frustum[m].m_fD = vec.w;
    }

    m_ClippedVerts.clear();
    m_ClippedIndis.clear();

    // 遍历直线列表
    for( UINT i=0;i<nPrimCount;++i )
    {
        bool bCulled = false;
        SVertex3D line[2];
        line[0] = pVerts[*pIndis++];
        line[1] = pVerts[*pIndis++];

        // 依次检测每个面
        for( UINT p=0;p<6;++p )
        {
            int inside = 0;
            int outside = 0;
            bool bIn[2] = { false, false };

            if( frustum[p].Classify(line[0].pos) == PLANE_FRONT )
            {
                bIn[0] = true;
                ++inside;
            }
            else
            {
                bIn[0] = false;
                ++outside;
            }

            if( frustum[p].Classify(line[1].pos) == PLANE_FRONT )
            {
                bIn[1] = true;
                ++inside;
            }
            else
            {
                bIn[1] = false;
                ++outside;
            }

            if( !outside )
                continue;

            // 在剪裁面之外，剔除该线段
            if( !inside )
            {
                bCulled = true;
                break;
            }

            // 被视锥的近平面裁剪，其他平面的裁剪现在不管
            // 交给光栅化阶段来做
            if( p == FFP_NEAR )
            {
                SVertex3D *inV, *ouV;

                if( bIn[0] )
                {
                    inV = &line[0];
                    ouV = &line[1];
                }
                else
                {
                    inV = &line[1];
                    ouV = &line[0];
                }

                // 计算线段与平面的交点
                float f = frustum[p].Intersect(inV->pos, ouV->pos);

                // 计算新的交点（画线不需要光照和纹理信息）
                ouV->pos = inV->pos + (ouV->pos - inV->pos) * f;
                ouV->color = _Lerp(inV->color, ouV->color, f);
                break;
            }
        }

        if( !bCulled )
        {
            m_ClippedIndis.push_back( (WORD)m_ClippedVerts.size() );
            m_ClippedVerts.push_back( line[0] );
            m_ClippedIndis.push_back( (WORD)m_ClippedVerts.size() );
            m_ClippedVerts.push_back( line[1] );
        }
    }

    // 绘制剔除后的线段列表
    UINT lineCount = (UINT)m_ClippedIndis.size() / 2;
    for( UINT l=0;l<lineCount;++l )
        Draw3DLine(&m_ClippedVerts[m_ClippedIndis[l*2]], &m_ClippedVerts[m_ClippedIndis[l*2+1]]);
}

/** 绘制被视锥剪裁后的三角形列表
*/
void FSWDevice::DrawClippedTriangleList( const SVertex3D* pVerts,UINT nVertCount,
                                          const WORD* pIndis,UINT triangleCount )
{
    Plane frustum[6];
    std::vector<SVertex3D> clipVerts;

    // 获取当前视锥的六个平面
    GetFrustumPlane( m_mTMatrix[FTS_VIEW]*m_mTMatrix[FTS_PROJECTION],frustum );

    // 将六个面用世界矩阵进行逆变换，相当于将顶点变换到
    // 世界空间后再进行剪裁
    Matrix44 transWorld;
    transWorld.TransposeOf( m_mTMatrix[FTS_WORLD] );
    for( int p=0;p<6;++p )
    {
        Vector4 vec(frustum[p].m_vN,frustum[p].m_fD);
        vec = transWorld * vec;
        frustum[p].m_vN = Vector3( vec.x,vec.y,vec.z );
        frustum[p].m_fD = vec.w;
    }

    m_ClippedVerts.clear();
    m_ClippedIndis.clear();

    // 遍历三角形，剔除不可见的
    for( UINT i=0;i<triangleCount;++i )
    {
        clipVerts.push_back( pVerts[pIndis[i*3+0]] );
        clipVerts.push_back( pVerts[pIndis[i*3+1]] );
        clipVerts.push_back( pVerts[pIndis[i*3+2]] );

        // 依次检测每个面
        for( UINT p=0;p<6;++p )
        {
            for( UINT v=0;v<clipVerts.size();v+=3 )
            {
                int inside = 0;
                int outside = 0;
                bool bIn[3] = { false,false,false };

                for( UINT t=0;t<3;++t )
                {
                    if( frustum[p].Classify(clipVerts[v+t].pos) == PLANE_FRONT )
                    {
                        bIn[t] = true;
                        ++inside;
                    }
                    else
                    {
                        bIn[t] = false;
                        ++outside;
                    }
                }

                // 如果全在该平面内部，则检测下一平面
                if( !outside )
                    continue;

                // 如果全在该平面外部，则剔除该三角形
                if( !inside )
                {
                    clipVerts.erase( clipVerts.begin()+v,clipVerts.begin()+v+3 );
                    v -= 3;
                    continue;
                }

                // 只有被近平面剪裁时，我们对三角形进行剪裁。
                // 其他面的剪裁交由光栅化阶段来做
                if( p == FFP_NEAR )
                {
                    if( inside == 1 )
                    {
                        SVertex3D *inV,*oV1,*oV2;

                        if( bIn[0] )
                        {
                            inV = &clipVerts[v];
                            oV1 = &clipVerts[v+1];
                            oV2 = &clipVerts[v+2];
                        }
                        else if( bIn[1] )
                        {
                            inV = &clipVerts[v+1];
                            oV1 = &clipVerts[v];
                            oV2 = &clipVerts[v+2];
                        }
                        else
                        {
                            inV = &clipVerts[v+2];
                            oV1 = &clipVerts[v];
                            oV2 = &clipVerts[v+1];
                        }

                        // 计算和平面的交点
                        float f1 = frustum[p].Intersect( inV->pos,oV1->pos );
                        float f2 = frustum[p].Intersect( inV->pos,oV2->pos );

                        // 计算新的O1点
                        oV1->pos = inV->pos + (oV1->pos - inV->pos) * f1;
                        oV1->normal = inV->normal + (oV1->normal - inV->normal) * f1;
                        oV1->tex = inV->tex + (oV1->tex - inV->tex) * f1;
                        oV1->color = _Lerp( inV->color,oV1->color,f1 );

                        // 计算新的O2点
                        oV2->pos = inV->pos + (oV2->pos - inV->pos) * f2;
                        oV2->normal = inV->normal + (oV2->normal - inV->normal) * f2;
                        oV2->tex = inV->tex + (oV2->tex - inV->tex) * f2;
                        oV2->color = _Lerp( inV->color,oV2->color,f2 );
                    }
                    else
                    {
                        SVertex3D v1,v2;
                        SVertex3D *ouV,*iV1,*iV2;
                        int ou,i1,i2;

                        if( !bIn[0] )
                        {
                            ou = 0; i1 = 1; i2 = 2;
                        }
                        else if( !bIn[1] )
                        {
                            ou = 1; i1 = 2; i2 = 0;
                        }
                        else
                        {
                            ou = 2; i1 = 0; i2 = 1;
                        }

                        ouV = &clipVerts[v+ou];
                        iV1 = &clipVerts[v+i1];
                        iV2 = &clipVerts[v+i2];

                        // 计算平面的交点
                        float f1 = frustum[p].Intersect( iV1->pos,ouV->pos );
                        float f2 = frustum[p].Intersect( iV2->pos,ouV->pos );

                        // 计算交点V1
                        v1.pos = iV1->pos + (ouV->pos - iV1->pos) * f1;
                        v1.normal = iV1->normal + (ouV->normal - iV1->normal) * f1;
                        v1.tex = iV1->tex + (ouV->tex - iV1->tex) * f1;
                        v1.color = _Lerp( iV1->color,ouV->color,f1 );

                        // 计算交点V2
                        v2.pos = iV2->pos + (ouV->pos - iV2->pos) * f2;
                        v2.normal = iV2->normal + (ouV->normal - iV2->normal) * f2;
                        v2.tex = iV2->tex + (ouV->tex - iV2->tex) * f2;
                        v2.color = _Lerp( iV2->color,ouV->color,f2 );

                        // 改变原三角形的顶点
                        *ouV = v1;

                        // 添加一个新的三角形
                        clipVerts.insert( clipVerts.end(),3,SVertex3D() );
                        clipVerts[v+3+ou] = v2;
                        clipVerts[v+3+i1] = v1;
                        clipVerts[v+3+i2] = clipVerts[v+i2];
                    }

                    break;
                }
            }
        }

        // 将三角形加入已剪裁列表
        for( UINT cv=0;cv<clipVerts.size();++cv )
        {
            m_ClippedIndis.push_back( (WORD)m_ClippedVerts.size() );
            m_ClippedVerts.push_back( clipVerts[cv] );
        }

        clipVerts.clear();
    }

    POINT pt; float Z,W;
    UINT w = m_pBackBuffer->GetWidth();
    UINT h = m_pBackBuffer->GetHeight();
    m_2DVerts.resize( m_ClippedVerts.size() );

    // 顶点变换、计算光照
    for( UINT i=0;i<m_ClippedVerts.size();++i )
    {
        WorldToScreen( m_ClippedVerts[i].pos,m_mTMatrix[FTS_TRANSFORM],w,h,pt,Z,W );
        m_2DVerts[i].pos = pt;
        m_2DVerts[i].z = Z;
        m_2DVerts[i].w = W;
        m_2DVerts[i].tex = m_ClippedVerts[i].tex;

        // 将纹理坐标变换到纹理像素坐标
        if( m_RenderState.pTexture )
        {
            m_2DVerts[i].tex = m_ClippedVerts[i].tex;
            m_2DVerts[i].tex.x *= m_RenderState.pTexture->GetWidth() - 1;
            m_2DVerts[i].tex.y *= m_RenderState.pTexture->GetHeight() - 1;
        }

        // 计算光照
        if( m_bLightEnable )
        {
            // 顶点的法线变换到世界空间
            Vector4 vNormal( m_ClippedVerts[i].normal,0.0f );
            m_mTMatrix[FTS_WORLD].Transform( vNormal );
            Vector3 norm( vNormal.x,vNormal.y,vNormal.z );
            norm.Normalize();

            // 计算光照颜色
            ColorF diffuse,ambient;
            ColorF temp = m_Light.Diffuse*max(0,-(norm*m_Light.Direction));
            diffuse.Modulate( m_Material.Diffuse,temp );
            ambient.Modulate( m_Material.Ambient,m_Light.Ambient );
            ColorF final = diffuse + ambient;
            final.fA = 1.0f;
            m_2DVerts[i].color = final.ToColor().val;
        }
        else
        {
            // 取顶点颜色
            m_2DVerts[i].color = m_ClippedVerts[i].color;
        }
    }

    // 用当前的活动Painter绘制三角形列表
    if( m_2DVerts.size() && m_ClippedIndis.size() )
    {
        m_pActivePainter->DrawTriangleList( &m_2DVerts[0],(UINT)m_ClippedVerts.size(),
            &m_ClippedIndis[0],(UINT)m_ClippedIndis.size()/3 );
    }
}

/** 改变设备的绘制函数
*/
void FSWDevice::UpdatePainter()
{
    if( !m_RenderState.pTexture )
    {
        if( m_RenderState.FillMode == FFM_SOLID )
            m_pActivePainter = m_Painters[PAINT_GOURAUD];
        else
            m_pActivePainter = m_Painters[PAINT_GOURAUD_WIRE];
    }
    else
    {
        if( m_RenderState.FillMode == FFM_SOLID )
            m_pActivePainter = m_Painters[PAINT_TEXTURE];
        else
            m_pActivePainter = m_Painters[PAINT_TEXTURE_WIRE];
    }
}

/** 重置渲染设备模式
*/
bool FSWDevice::ResetDevice(int width, int height, bool bFullscreen)
{
	if( m_bRendering )
	{
		ASSERT(0);
		FSW_LOG("FSWDevice::ResetDevice, Can't reset the device when is rendering!");
		return false;
	}

	// 检测分辨率是否符合
	if( bFullscreen && !CheckResolution(width, height) )
	{
		ASSERT(0);
		FSW_LOG("FSWDevice::ResetDevice, Invalid resolution for fullscreen mode!");
		return false;
	}

	if( width != m_RenderState.nWidth || height != m_RenderState.nHeight )
	{
		m_RenderState.nWidth = width;
		m_RenderState.nHeight = height;
		SetRect(&m_RenderState.rcViewport, 0, 0, width-1, height-1);

		if( !m_pBackBuffer->Create(width, height, FPF_A8R8G8B8) ||
			!m_pZBuffer->Create(width, height, FPF_D32) )
		{
			ASSERT(0);
			FSW_LOG("FSWDevice::ResetDevice, Create the new buffer for device failed!");
			return false;
		}

		// 重新创建后期特效的SURFACE
		for( size_t i=0;i<m_PostEffects.size();i++ )
			m_PostEffects[i]->OnResetDevice();
	}

	// 处理全屏切换
	if( m_bFullscreen != bFullscreen )
	{
		ToggleFullscreen(bFullscreen);
		m_bFullscreen = bFullscreen;
	}

	return true;
}

// 处理全屏模式
void FSWDevice::ToggleFullscreen(bool bFullscreen)
{
	int screenx = GetSystemMetrics(SM_CXSCREEN);
	int screeny = GetSystemMetrics(SM_CYSCREEN);

	if( bFullscreen )
	{
		DWORD dwStyle = WS_VISIBLE|WS_CLIPCHILDREN|WS_POPUP;
		SetWindowPos( m_hWnd,HWND_TOPMOST,0,0,screenx,screeny,SWP_NOACTIVATE );
		SetWindowLong( m_hWnd,GWL_STYLE,dwStyle );
	}
	else
	{
		// 恢复窗口的风格
		SetWindowLong( m_hWnd,GWL_STYLE,m_dwWndStyle );

		RECT rcWnd;
		SetRect( &rcWnd,0,0,m_RenderState.nWidth,m_RenderState.nHeight );
		AdjustWindowRect( &rcWnd,m_dwWndStyle,FALSE );

		// 使窗口居中
		int w = rcWnd.right - rcWnd.left;
		int h = rcWnd.bottom - rcWnd.top;
		if( w > screenx ) w = screenx;
		if( h > screeny ) h = screeny;
		int x = (screenx - w) / 2;
		int y = (screeny - h) / 2;

		// 设置窗口原先的位置
		SetWindowPos( m_hWnd,HWND_NOTOPMOST,x,y,w,h,SWP_SHOWWINDOW );

		// 刷新桌面
		InvalidateRect(NULL, NULL, TRUE);
	}
}

/** 添加一个后期处理特效
*/
bool FSWDevice::AttachPostEffect(EPostEffectType Type)
{
	for( size_t i=0;i<m_PostEffects.size();i++ )
	{
		if( Type == m_PostEffects[i]->GetEffectType() )
			return false;
	}

	FSWSurface* pSrcSurface;
	if( m_PostEffects.size() )
	{
		FSWPostEffect* pEnd = m_PostEffects[m_PostEffects.size()-1];
		pSrcSurface = pEnd->GetNextSurface();
	}
	else
	{
		pSrcSurface = m_pBackBuffer;
	}

	FSWPostEffect* pNewEffect = FSWPostEffect::CreatePostEffect(Type, pSrcSurface);
	if( !pNewEffect ) return false;
	m_PostEffects.push_back(pNewEffect);
	return true;
}

/** 移除一个后期处理特效
*/
void FSWDevice::DetachPostEffect(EPostEffectType Type)
{
	size_t i;
	FSWPostEffect* pPrev = NULL;
	FSWPostEffect* pNext = NULL;
	FSWPostEffect* pThis = NULL;

	for( i=0;i<m_PostEffects.size();i++ )
	{
		if( Type == m_PostEffects[i]->GetEffectType() )
		{
			pThis = m_PostEffects[i];
			if( i > 0 ) pPrev = m_PostEffects[i-1];
			if( i < m_PostEffects.size()-1 ) pNext = m_PostEffects[i+1];
			break;
		}
	}

	if( pThis )
	{
		if( pPrev )
		{
			if( pNext )
				pNext->SetPrevSurface(pPrev->GetNextSurface());
		}
		else
		{
			if( pNext )
				pNext->SetPrevSurface(m_pBackBuffer);
		}

		SAFE_DELETE(pThis);
		m_PostEffects.erase(m_PostEffects.begin() + i);
	}
}

/** 更新后期处理特效
*/
void FSWDevice::ApplyPostEffect()
{
	if( m_PostEffects.size() )
	{
		for( size_t i=0;i<m_PostEffects.size();i++ )
			m_PostEffects[i]->ApplyEffect();

		m_pFinalBF = m_PostEffects[m_PostEffects.size()-1]->GetNextSurface();
	}
	else
	{
		m_pFinalBF = m_pBackBuffer;
	}
}