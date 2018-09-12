/*
 * -----------------------------------------------------------------------
 *  Name:   Color.h
 *  Desc:   ���ļ�������һЩ�����������ɫ�ࡣ
 *  Author: Yish
 *  Date:   11/12/2010
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __COLOR_H__
#define __COLOR_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Common.h"

//////////////////////////////////////////////////////////////////////////


// ���ڲ���32λ��ɫ�ĺ�
#define FSWCOLORRGBA(r,g,b,a) (DWORD)(((a)<<24) | ((r)<<16) | ((g)<<8) | (b))
#define FSWCOLORRGB(r,g,b) FSWCOLORRGBA(r,g,b,255)

struct Color;
struct ColorF;

/** 32λ����ɫ
@remarks
    ��ɫ����A8R8G8B8��ʽ
*/
struct Color
{
    union
    {
        struct 
        {
            BYTE b;
            BYTE g;
            BYTE r;
            BYTE a;
        };

        DWORD val;
    };

    Color(void) : val(0) {}
    Color( BYTE R,BYTE G,BYTE B,BYTE A=255 ) : r(R),g(G),b(B),a(A) {}
    Color( DWORD nColor ) : val(nColor) {}
    Color( const ColorF& clr );

    /** ����һЩ�����
    */
    Color operator + ( const Color& clr ) const
    {
        return Color(
            r+clr.r, g+clr.g, b+clr.b, a+clr.a
            );
    }

    Color& operator += ( const Color& clr )
    {
        r += clr.r;
        g += clr.g;
        b += clr.b;
        a += clr.a;

        return *this;
    }

    Color operator - ( const Color& clr ) const
    {
        return Color(
            r-clr.r, g-clr.g, b-clr.b, a-clr.a
            );
    }

    Color& operator -= ( const Color& clr )
    {
        r -= clr.r;
        g -= clr.g;
        b -= clr.b;
        a -= clr.a;

        return *this;
    }

    Color operator * ( BYTE v ) const
    {
        return Color(
            r*v, g*v, b*v, a*v
            );
    }

    Color& operator *= ( BYTE v )
    {
        r *= v;
        g *= v;
        b *= v;
        a *= v;

        return *this;
    }

    Color operator / ( BYTE v ) const
    {
        return Color(
            r/v, g/v, b/v, a/v
            );
    }

    Color& operator /= ( BYTE v )
    {
        r /= v;
        g /= v;
        b /= v;
        a /= v;

        return *this;
    }

    bool operator == ( const Color& clr ) const
    {
        return val == clr.val;
    }

    bool operator != ( const Color& clr ) const
    {
        return val != clr.val;
    }

    ColorF ToColorF() const;
};

/** 128λ������ɫ
*/
struct ColorF
{
    float fR;           // ��ɫͨ��
    float fG;           // ��ɫͨ��
    float fB;           // ��ɫͨ��
    float fA;           // Alphaͨ��

    // �������캯��
    ColorF(void) : fR(0.0f),fG(0.0f),fB(0.0f),fA(1.0f) {}
    ColorF( float r,float g,float b,float a=1.0f ) : fR(r),fG(g),fB(b),fA(a) {}
    ColorF( const Color& clr );

    //////////////////////////////////////////////////////////////////////
    // ���ؼ��������

    ColorF operator + ( const ColorF& color ) const
    {
        ColorF clr;
        clr.fR = fR + color.fR;
        clr.fG = fG + color.fG;
        clr.fB = fB + color.fB;
        clr.fA = fA + color.fA;
        return clr;
    }

    ColorF& operator += ( const ColorF& color )
    {
        fR += color.fR;
        fG += color.fG;
        fB += color.fB;
        fA += color.fA;

        return *this;
    }

    ColorF operator - ( const ColorF& color ) const
    {
        ColorF clr;
        clr.fR = fR - color.fR;
        clr.fG = fG - color.fG;
        clr.fB = fB - color.fB;
        clr.fA = fA - color.fA;
        return clr;
    }

    ColorF& operator -= ( const ColorF& color )
    {
        fR -= color.fR;
        fG -= color.fG;
        fB -= color.fB;
        fA -= color.fA;

        return *this;
    }

    ColorF operator * ( float fValue ) const
    {
        ColorF clr;
        clr.fR = fR * fValue;
        clr.fG = fG * fValue;
        clr.fB = fB * fValue;
        clr.fA = fA * fValue;
        return clr;
    }

    ColorF& operator *= ( float fValue )
    {
        fR *= fValue;
        fG *= fValue;
        fB *= fValue;
        fA *= fValue;

        return *this;
    }

    ColorF operator / ( float fValue ) const
    {
        ColorF clr;
        clr.fR = fR / fValue;
        clr.fG = fG / fValue;
        clr.fB = fB / fValue;
        clr.fA = fA / fValue;
        return clr;
    }

    ColorF& operator /= ( float fValue )
    {
        fR /= fValue;
        fG /= fValue;
        fB /= fValue;
        fA /= fValue;

        return *this;
    }

    Color ToColor() const;

    // ȡ�෴����ɫ
    void Negative()
    {
        fR = 1.0f - fR;
        fG = 1.0f - fG;
        fB = 1.0f - fB;
        fA = 1.0f - fA;
    }

    // ���������ɫ
    void Modulate( const ColorF& c1,const ColorF& c2 )
    {
        fR = c1.fR * c2.fR;
        fG = c1.fG * c2.fG;
        fB = c1.fB * c2.fB;
        fA = c1.fA * c2.fA;
    }

    // ��������ɫ��ֵ
    void Lerp( const ColorF& c1,const ColorF& c2,float f )
    {
        fR = c1.fR + (c2.fR - c1.fR) * f;
        fG = c1.fG + (c2.fG - c1.fG) * f;
        fB = c1.fB + (c2.fB - c1.fB) * f;
        fA = c1.fA + (c2.fA - c1.fA) * f;
    }

    //////////////////////////////////////////////////////////////////////
    // һЩ���õ���ɫ����
    static const ColorF Red;
    static const ColorF Green;
    static const ColorF Blue;
    static const ColorF White;
    static const ColorF Black;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __COLOR_H__