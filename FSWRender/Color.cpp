/*
 * -----------------------------------------------------------------------
 *  Name:   Color.cpp
 *  Desc:   ���ļ������˼������õ���ɫ������
 *  Author: Yish
 *  Date:   11/8/2010
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#include "Color.h"


// һЩ���õ���ɫ����
const ColorF ColorF::Red( 1.0f,0.0f,0.0f );
const ColorF ColorF::Green( 0.0f,1.0f,0.0f );
const ColorF ColorF::Blue( 0.0f,0.0f,1.0f );
const ColorF ColorF::Black( 0.0f,0.0f,0.0f );
const ColorF ColorF::White( 1.0f,1.0f,1.0f );

/** ���캯��
*/
Color::Color( const ColorF& clr )
{
    r = (BYTE)(255 * F_Clamp(clr.fR, 0.0f, 1.0f));
    g = (BYTE)(255 * F_Clamp(clr.fG, 0.0f, 1.0f));
    b = (BYTE)(255 * F_Clamp(clr.fB, 0.0f, 1.0f));
    a = (BYTE)(255 * F_Clamp(clr.fA, 0.0f, 1.0f));
}

/** ��������ɫת��Ϊ������ɫ
*/
ColorF Color::ToColorF() const
{
    float fInv = 1.0f / 255.0f;

    return ColorF(
        (float)r * fInv,
        (float)g * fInv,
        (float)b * fInv,
        (float)a * fInv
        );
}

/** ���캯��
*/
ColorF::ColorF( const Color& clr )
{
    float fInv = 1.0f / 255.0f;

    fA = (float)clr.a * fInv;
    fR = (float)clr.r * fInv;
    fG = (float)clr.g * fInv;
    fB = (float)clr.b * fInv;
}

/** ��������ɫת��Ϊ������ɫ
*/
Color ColorF::ToColor() const
{
    return Color(
        (BYTE)(F_Clamp(fR,0.0f,1.0f) * 255.0f),
        (BYTE)(F_Clamp(fG,0.0f,1.0f) * 255.0f),
        (BYTE)(F_Clamp(fB,0.0f,1.0f) * 255.0f),
        (BYTE)(F_Clamp(fA,0.0f,1.0f) * 255.0f)
        );
}