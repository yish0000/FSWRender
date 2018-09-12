/*
 * -----------------------------------------------------------------------
 *  Name:   FSWTexture.cpp
 *  Desc:   本文件实现了一个纹理对象。
 *  Author: Yish
 *  Date:   2011/7/28
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FSWTexture.h"


/** 构造函数
*/
FSWTexture::FSWTexture(void) : m_pSurface(NULL),m_bAlpha(false)
{
}

/** 析构函数
*/
FSWTexture::~FSWTexture(void)
{
    SAFE_DELETE( m_pSurface );
}

// 创建一个空的纹理
bool FSWTexture::Create( UINT nWidth, UINT nHeight )
{
    SAFE_DELETE( m_pSurface );

    // 创建新的Surface
    m_pSurface = new FSWSurface();
    if( !m_pSurface->Create(nWidth, nHeight, FPF_A8R8G8B8) )
    {
        SAFE_DELETE( m_pSurface );
        return false;
    }

    m_bAlpha = true;
    return true;
}

/** 从位图中加载数据
*/
bool FSWTexture::LoadFromBitmap( const char* filename )
{
    UINT bpp,w,h;
    BYTE* pBits;
    BITMAPFILEHEADER header;
    BITMAPINFOHEADER infoHead;
    BYTE* pScanLine;
    int nRowBytes;

    FILE* pFile = fopen( filename,"rb" );
    if( !pFile ) return false;

    // 读取位图文件头
    fread( &header,sizeof(BITMAPFILEHEADER),1,pFile );

    // 是否有效的位图文件
    if( header.bfType != 0x4d42 )
    {
        fclose( pFile );
        return false;
    }

    // 读取位图信息
    fread( &infoHead,sizeof(BITMAPINFOHEADER),1,pFile );

    // 只能加载24位和32位位图
    if( infoHead.biBitCount != 24 && infoHead.biBitCount != 32 )
    {
        fclose( pFile );
        return false;
    }

    w = infoHead.biWidth;
    h = infoHead.biHeight;
    bpp = infoHead.biBitCount / 8;

    // 创建缓冲区存储像素
    pBits = new BYTE[w*h*bpp];
    if( bpp == 3 )
    {
        nRowBytes = ((w * 3) + 3) / 4;
        nRowBytes *= 4;
        pScanLine = reinterpret_cast<UCHAR*>(malloc(nRowBytes));
    }
    else
    {
        nRowBytes = w * 4;
        pScanLine = reinterpret_cast<UCHAR*>(malloc(nRowBytes));
    }

    // 移到像素数据区
    fseek( pFile,header.bfOffBits,SEEK_SET );

    // 读取像素
    for( int i=h-1;i>=0;i-- )
    {
        int nBytes;

        if( bpp == 3 )
        {
            nBytes = (w*3+3) / 4;
            nBytes *= 4;

            fread( pScanLine,w,3,pFile );
            nBytes -= w * 3;

            while( nBytes-- > 0 ) fgetc( pFile );
        }
        else
        {
            nBytes = w * 4;
            fread( pScanLine,w,4,pFile );
        }

        // 保存这行数据
        memcpy( &pBits[w*bpp*i],pScanLine,w*bpp );
    }

    free( pScanLine );
    fclose( pFile );

    // 创建纹理对象
    m_pSurface = new FSWSurface();
    m_pSurface->Create( w,h,FPF_A8R8G8B8 );

    DWORD* dest = (DWORD*)m_pSurface->GetBufferData();
    if( bpp == 3 )
    {
        m_bAlpha = false;

        for( UINT i=0;i<w*h*bpp;i+=3 )
        {
            *dest++ = (DWORD)(pBits[i+0] | pBits[i+1] << 8 | pBits[i+2] << 16 | 0xff000000);
        }
    }
    else
    {
        m_bAlpha = true;
        memcpy( dest,pBits,w*h*bpp );
    }

    delete[] pBits;
    return true;
}

/** 保存本纹理到文件
*/
void FSWTexture::SaveToBitmap( const char* filename )
{
    SaveSurfaceToFile(m_pSurface, filename);
}

/** 保存指定的渲染表面
*/
void FSWTexture::SaveSurfaceToFile( FSWSurface* pSurface, const char* filename )
{
    LONG bmofs;
    BITMAPFILEHEADER fileHead;
    BITMAPINFOHEADER infoHead;

    FILE* pFile = fopen(filename, "wb");
    if( !pFile ) return;

    // 先写入文件头占位，等写完像素再来覆盖
    fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, pFile);

    // 位图信息
    infoHead.biSize = sizeof(BITMAPINFOHEADER);
    infoHead.biWidth = pSurface->GetWidth();
    infoHead.biHeight = pSurface->GetHeight();
    infoHead.biPlanes = 1;
    infoHead.biBitCount = 32;
    infoHead.biCompression = BI_RGB;
    infoHead.biSizeImage = pSurface->GetWidth() * pSurface->GetHeight();
    infoHead.biXPelsPerMeter = 0;
    infoHead.biYPelsPerMeter = 0;
    infoHead.biClrUsed = 0;
    infoHead.biClrImportant = 0;
    fwrite(&infoHead, sizeof(BITMAPINFOHEADER), 1, pFile);

    // 保存像素区的偏移
    bmofs = ftell(pFile);

    // 写入像素数据
    DWORD* buf = (DWORD*)pSurface->GetBufferData();
    for( int i=(int)pSurface->GetHeight()-1;i>=0;--i )
    {
        DWORD* pLine = &buf[pSurface->GetWidth()*i];
        fwrite(pLine, sizeof(DWORD), pSurface->GetWidth(), pFile);
    }

    fileHead.bfType = 0x4d42;
    fileHead.bfSize = ftell(pFile);
    fileHead.bfOffBits = bmofs;
    fseek(pFile, 0L, SEEK_SET);
    fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, pFile);
    fclose(pFile);
}