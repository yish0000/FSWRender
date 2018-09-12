/*
 * ------------------------------------------------------------------------
 *  Name:   FSWSceneObject.cpp
 *  Desc:   本文件实现了一个场景对象基类。
 *  Author: Yish
 *  Date:   2013/4/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */



#include "FSWSceneObject.h"
#include "FSWSceneNode.h"


// 构造函数
FSWSceneObject::FSWSceneObject() : m_pNode(NULL)
{
}

// 析构函数
FSWSceneObject::~FSWSceneObject()
{
	// 如果隶属于某个节点，则从该节点移除
	if( m_pNode )
		m_pNode->RemoveSceneObject(this);
}

// 获取世界矩阵
const Matrix44& FSWSceneObject::GetWorldMatrix() const
{
	return m_pNode->GetAbsoluteTM();
}