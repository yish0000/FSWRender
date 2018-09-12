/*
 * ------------------------------------------------------------------------
 *  Name:   FSWScene.h
 *  Desc:   本文件定义了一个场景对象。
 *  Author: Yish
 *  Date:   2013/4/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSW_SCENE_H__
#define __FSW_SCENE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "Common.h"

///////////////////////////////////////////////////////////////////////////


class FSWDevice;
class FSWCamera;
class FSWSceneObject;
class FSWSceneNode;

/** 场景树
*/
class FSWScene
{
public:

	enum ObjectType
	{
		OBJECT_MODEL,		// 静态模型
		OBJECT_SKELETON,	// 带骨骼的模型
		OBJECT_PARTICLE,	// 粒子系统
	};

	typedef std::map<String,FSWSceneObject*> SceneObjects;

public:
	FSWScene( FSWDevice* pDevice );
	virtual ~FSWScene();

	// 销毁当前场景内容
	void Reset();

	// 创建一个场景对象
	FSWSceneObject* CreateObject( const String& name,ObjectType type );

	// 移除指定的场景对象
	void RemoveObject( const String& name );

	// 移除所有的场景对象
	void RemoveAllObjects();

	// 获取场景对象的个数
	size_t GetObjectCount() const { return m_Objects.size(); }

	// 获取指定的场景对象
	FSWSceneObject* GetObjectByName( const String& name );

	// 更新场景树
	virtual void Update( float fElapsed );

	// 渲染场景树
	virtual void Render();

	// 设置场景的雾化模式
	void SetFogMode(bool bEnable, float fNear = 15.0f, float fFar = 45.0f);

	// 获取场景的根节点
	FSWSceneNode* GetSceneRoot() const { return m_pRoot; }

	// 获取该场景的相机对象
	FSWCamera* GetCamera() const { return m_pCamera; }

protected:
	FSWDevice* m_pDevice;			// 3D引擎设备指针
	FSWSceneNode* m_pRoot;			// 场景的根节点
	FSWCamera* m_pCamera;			// 该场景的相机对象
	SceneObjects m_Objects;			// 场景对象列表

	bool m_bFogEnable;				// 是否打开雾化模式
	float m_fFogNear;				// 雾化近距离
	float m_fFogFar;				// 雾化远距离
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_SCENE_H__