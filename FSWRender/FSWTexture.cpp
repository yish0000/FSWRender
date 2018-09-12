/*
 * -----------------------------------------------------------------------
 *  Name:   FSWTexture.cpp
 *  Desc:   ���ļ�ʵ����һ���������
 *  Author: Yish
 *  Date:   2011/7/28
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FSWTexture.h"


/** ���캯��
*/
FSWTexture::FSWTexture(void) : m_pSurface(NULL),m_bAlpha(false)
{
}

/** ��������
*/
FSWTexture::~FSWTexture(void)
{
    SAFE_DELETE( m_pSurface );
}

// ����һ���յ�����
bool FSWTexture::Create( UINT nWidth, UINT nHeight )
{
    SAFE_DELETE( m_pSurface );

    // �����µ�Surface
    m_pSurface = new FSWSurface();
    if( !m_pSurface->Create(nWidth, nHeight, FPF_A8R8G8B8) )
    {
        SAFE_DELETE( m_pSurface );
        return false;
    }

    m_bAlpha = true;
    return true;
}

/** ��λͼ�м�������
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

    // ��ȡλͼ�ļ�ͷ
    fread( &header,sizeof(BITMAPFILEHEADER),1,pFile );

    // �Ƿ���Ч��λͼ�ļ�
    if( header.bfType != 0x4d42 )
    {
        fclose( pFile );
        return false;
    }

    // ��ȡλͼ��Ϣ
    fread( &infoHead,sizeof(BITMAPINFOHEADER),1,pFile );

    // ֻ�ܼ���24λ��32λλͼ
    if( infoHead.biBitCount != 24 && infoHead.biBitCount != 32 )
    {
        fclose( pFile );
        return false;
    }

    w = infoHead.biWidth;
    h = infoHead.biHeight;
    bpp = infoHead.biBitCount / 8;

    // �����������洢����
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

    // �Ƶ�����������
    fseek( pFile,header.bfOffBits,SEEK_SET );

    // ��ȡ����
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

        // ������������
        memcpy( &pBits[w*bpp*i],pScanLine,w*bpp );
    }

    free( pScanLine );
    fclose( pFile );

    // �����������
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

/** ���汾�����ļ�
*/
void FSWTexture::SaveToBitmap( const char* filename )
{
    SaveSurfaceToFile(m_pSurface, filename);
}

/** ����ָ������Ⱦ����
*/
void FSWTexture::SaveSurfaceToFile( FSWSurface* pSurface, const char* filename )
{
    LONG bmofs;
    BITMAPFILEHEADER fileHead;
    BITMAPINFOHEADER infoHead;

    FILE* pFile = fopen(filename, "wb");
    if( !pFile ) return;

    // ��д���ļ�ͷռλ����д��������������
    fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, pFile);

    // λͼ��Ϣ
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

    // ������������ƫ��
    bmofs = ftell(pFile);

    // д����������
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