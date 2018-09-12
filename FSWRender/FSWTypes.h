/*
 * -----------------------------------------------------------------------
 *  Name:   FSWTypes.h
 *  Desc:   本文件定义了图形设备所需的一些类型。
 *  Author: Yish
 *  Date:   2011/7/24
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FSWTYPES_H__
#define __FSWTYPES_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Common.h"
#include "Vector.h"
#include "Matrix.h"
#include "Color.h"

//////////////////////////////////////////////////////////////////////////


// 定义一些常量
const float FSW_PI = 3.14159265358979f;

//////////////////////////////////////////////////////////////////////////
// 程序的枚举类型

enum EPixelFormat
{
    FPF_A8R8G8B8,       // 32位颜色缓冲区
    FPF_D32,            // 32位深度缓冲区
};

enum EPrimitiveType
{
    FPT_LINELIST,       // 线表
    FPT_TRIANGLELIST,   // 三角形表
};

enum ETransformType
{
    FTS_WORLD,          // 世界变换矩阵
    FTS_VIEW,           // 观察变换矩阵
    FTS_PROJECTION,     // 投影变换矩阵
    FTS_TRANSFORM,      // 混合后的矩阵
    FTS_COUNT,
};

enum EShadeMode
{
    FSM_FLAT,           // 平面着色
    FSM_GOURAUD,        // 高罗德着色
};

enum ECullMode
{
    FCM_NONE,           // 不进行背面剔除
    FCM_CW,             // 顺时针背面剔除
    FCM_CCW,            // 逆时针背面剔除
};

enum EFillMode
{
    FFM_SOLID,          // 填充模式
    FFM_WIREFRAME,      // 线框模式
};

enum EDepthBufferType
{
    FDB_NONE,           // 关闭Z缓冲区
    FDB_READONLY,       // Z缓冲区只读
    FDB_READWRITE,      // Z缓冲区可读可写
};

enum ETextureFilter
{
    FTF_POINT,          // 最近点采样
    FTF_LINEAR,         // 双线性插值采样
};

enum EFrustumPlane
{
    FFP_LEFT,           // 左平面
    FFP_RIGHT,          // 右平面
    FFP_TOP,            // 顶平面
    FFP_BOTTOM,         // 底平面
    FFP_NEAR,           // 近平面
    FFP_FAR,            // 远平面
};

//////////////////////////////////////////////////////////////////////////

/** 顶点格式(三维)
*/
struct SVertex3D
{
    Vector3 pos;        // 位置
    Vector3 normal;     // 法线
    DWORD color;        // 顶点颜色
    Vector2 tex;        // 纹理坐标
};

/** 顶点格式(变换到屏幕的二维顶点)
*/
struct SVertex2D
{
    POINT pos;          // 屏幕上的坐标
    DWORD color;        // 顶点颜色
    float z;            // 顶点的深度值
    float w;            // 用于透视校正
    Vector2 tex;        // 纹理上的坐标
};

/** 材质信息
*/
struct SMaterial
{
    ColorF Ambient;     // 环境光颜色
    ColorF Diffuse;     // 漫射光颜色
};

/** 光照信息
*/
struct SLight
{
    ColorF Ambient;     // 环境光颜色
    ColorF Diffuse;     // 漫射光颜色
    Vector3 Direction;  // 光照的方向
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWTYPES_H__