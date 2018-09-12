/*
 * ------------------------------------------------------------------------
 *  Name:   FSWSceneObject.h
 *  Desc:   本文件定义了一个场景对象基类。
 *  Author: Yish
 *  Date:   2013/4/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSW_SCENEOBJECT_H__
#define __FSW_SCENEOBJECT_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "Common.h"
#include "Matrix.h"

///////////////////////////////////////////////////////////////////////////


class FSWSceneNode;

/** 场景对象
*/
class FSWSceneObject
{
public:
	FSWSceneObject();
	virtual ~FSWSceneObject();

	// 更新场景对象
	virtual void Update( float fElapsed ) = 0;

	// 渲染场景对象
	virtual void Render() = 0;

	// 获取场景对象的变换矩阵
	const Matrix44& GetWorldMatrix() const;

	// 获取当前物体的位置
	Vector3 GetPosition() const
	{
		const Matrix44& mWorld = GetWorldMatrix();
		return Vector3(mWorld._41, mWorld._42, mWorld._43);
	}

	// 获取所属节点
	FSWSceneNode* GetParentNode() { return m_pNode; }

	// 设置所属节点
	void SetParentNode( FSWSceneNode* pNode ) { m_pNode = pNode; }

protected:
	FSWSceneNode* m_pNode;		// 所属节点
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_SCENEOBJECT_H__