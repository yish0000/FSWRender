/*
 * ------------------------------------------------------------------------
 *  Name:   FSWScene.cpp
 *  Desc:    本文件实现了一个场景对象。
 *  Author: Yish
 *  Date:   2013/4/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FSWScene.h"
#include "FSWSceneNode.h"
#include "FSWSceneObject.h"
#include "FSWDevice.h"
#include "FSWCamera.h"
#include "FSWModel.h"


/** 构造函数
*/
FSWScene::FSWScene( FSWDevice* pDevice ) : m_pDevice(pDevice),m_bFogEnable(false),
	m_fFogNear(15.0f),m_fFogFar(45.0f)
{
	// 创建场景的根节点
	m_pRoot = new FSWSceneNode("Scene Root");

	// 创建一个相机
	m_pCamera = new FSWCamera(pDevice);
 	m_pCamera->SetPosition(0, 5.0f, -30.0f);
// 	m_pCamera->LookAt(0.0f, 0.0f, 0.0f);
	m_pCamera->SetYawFixed(true);
	m_pCamera->SetViewPlanes(0.1f, 1000.0f);
}

/** 析构函数
*/
FSWScene::~FSWScene()
{
	RemoveAllObjects();

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pRoot);
}

/** 重置当前场景内容
*/
void FSWScene::Reset()
{
	m_pRoot->RemoveAllChildren();
	m_pRoot->RemoveAllSceneObjects();

	RemoveAllObjects();
}

/** 创建一个场景对象
*/
FSWSceneObject* FSWScene::CreateObject( const String& name,ObjectType type )
{
	FSWSceneObject* pObj = NULL;

	if( m_Objects.find(name) != m_Objects.end() )
		return NULL;

	switch( type )
	{
	case OBJECT_MODEL: pObj = new FSWModel(m_pDevice); break;
	default: ASSERT(!"Unknown scene object type!"); return NULL;
	}

	m_Objects[name] = pObj;
	return pObj;
}

/** 移除指定的场景对象
*/
void FSWScene::RemoveObject( const String& name )
{
	SceneObjects::iterator it;
	if( (it = m_Objects.find(name)) != m_Objects.end() )
	{
		SAFE_DELETE( it->second );
		m_Objects.erase(it);
	}
}

/** 移除所有的场景对象
*/
void FSWScene::RemoveAllObjects()
{
	SceneObjects::iterator it;
	for( it=m_Objects.begin();it!=m_Objects.end();++it )
	{
		SAFE_DELETE( it->second );
	}

	m_Objects.clear();
}

/** 获取指定的场景对象
*/
FSWSceneObject* FSWScene::GetObjectByName( const String& name )
{
	SceneObjects::iterator it;
	return (it = m_Objects.find(name)) != m_Objects.end() ? it->second : NULL;
}

/** 更新场景树
*/
void FSWScene::Update( float fElapsed )
{
	m_pDevice->SetTransform(FTS_VIEW, m_pCamera->GetViewMatrix());
	m_pDevice->SetTransform(FTS_PROJECTION, m_pCamera->GetProjectionMatrix());

	if( m_pRoot )
		m_pRoot->Update(fElapsed);
}

/** 渲染场景树
*/
void FSWScene::Render()
{
	if( m_pRoot )
		m_pRoot->Render();
}

/** 设置场景的雾化模式
*/
void FSWScene::SetFogMode( bool bEnable,float fNear,float fFar )
{
}