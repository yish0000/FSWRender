/*
 * ------------------------------------------------------------------------
 *  Name:   SimpleSceneListener.cpp
 *  Desc:   本文件实现了一个监听器，测试简单场景。
 *  Author: Yish
 *  Date:   2013/4/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FSWTexture.h"
#include "FSWDevice.h"
#include "SimpleSceneListener.h"


/** 构造函数
*/
SimpleSceneListener::SimpleSceneListener(FSWDevice* pDevice)
	: FrameListener(pDevice)
{
	m_pTexture = new FSWTexture();
	if( !m_pTexture->LoadFromBitmap("texture.bmp") )
	{
		delete m_pTexture;
		m_pTexture = NULL;
	}
}

/** 析构函数
*/
SimpleSceneListener::~SimpleSceneListener()
{
	SAFE_DELETE( m_pTexture );
}

/** 更新此帧
*/
void SimpleSceneListener::Tick(float fElapsed)
{
}

/** 渲染此帧
*/
void SimpleSceneListener::Render()
{
	// 绘制场景
	float fTime = (float)GetTickCount() / 1000.0f;

	// 旋转立方体
	Matrix44 rX,rY;
	rX.RotateX( cosf(fTime) );
	rY.RotateY( sinf(fTime) );
	rX = rX * rY;
	m_pDevice->SetTransform( FTS_WORLD,rX );

	SVertex3D verts[8] =
	{
		{ Vector3(-2.0f, 2.0f,-2.0f),Vector3(-1, 1,-1),0xffffffff,Vector2(0,0) },
		{ Vector3( 2.0f, 2.0f,-2.0f),Vector3( 1, 1,-1),0xff00ffff,Vector2(2,0) },
		{ Vector3( 2.0f, 2.0f, 2.0f),Vector3( 1, 1, 1),0xffff00ff,Vector2(0,0) },
		{ Vector3(-2.0f, 2.0f, 2.0f),Vector3(-1, 1, 1),0xffffff00,Vector2(2,0) },
		{ Vector3(-2.0f,-2.0f,-2.0f),Vector3(-1,-1,-1),0xff0000ff,Vector2(0,2) },
		{ Vector3( 2.0f,-2.0f,-2.0f),Vector3( 1,-1,-1),0xffff0000,Vector2(2,2) },
		{ Vector3( 2.0f,-2.0f, 2.0f),Vector3( 1,-1, 1),0xff00ff00,Vector2(0,2) },
		{ Vector3(-2.0f,-2.0f, 2.0f),Vector3(-1,-1, 1),0xff888888,Vector2(2,2) },
	};

	WORD indis[36] =
	{
		0,1,4,4,1,5,7,2,3,7,6,2,
		0,3,2,0,2,1,4,5,6,4,6,7,
		1,2,6,1,6,5,0,4,7,0,7,3,
	};

	m_pDevice->SetTexture( m_pTexture );
	m_pDevice->DrawPrimitive( FPT_TRIANGLELIST,verts,8,indis,12 );

	/*
	// 绘制一个立方体
	SVertex3D verts[8] =
	{
	{ Vector3(-2.0f, 2.0f,-2.0f),Vector3(0,0,0),0xffffffff,Vector2(0,0) },
	{ Vector3( 2.0f, 2.0f,-2.0f),Vector3(0,0,0),0xff00ffff,Vector2(0,0) },
	{ Vector3( 2.0f, 2.0f, 2.0f),Vector3(0,0,0),0xffff00ff,Vector2(0,0) },
	{ Vector3(-2.0f, 2.0f, 2.0f),Vector3(0,0,0),0xffffff00,Vector2(0,0) },
	{ Vector3(-2.0f,-2.0f,-2.0f),Vector3(0,0,0),0xff0000ff,Vector2(0,0) },
	{ Vector3( 2.0f,-2.0f,-2.0f),Vector3(0,0,0),0xffff0000,Vector2(0,0) },
	{ Vector3( 2.0f,-2.0f, 2.0f),Vector3(0,0,0),0xff00ff00,Vector2(0,0) },
	{ Vector3(-2.0f,-2.0f, 2.0f),Vector3(0,0,0),0xffff0000,Vector2(0,0) },
	};
	WORD indis[] = { 0,1,0,3,1,2,2,3,4,5,4,7,5,6,6,7,0,4,1,5,2,6,3,7 };
	m_pDevice->DrawPrimitive( FPT_LINELIST,verts,8,indis,12 );
	*/
}