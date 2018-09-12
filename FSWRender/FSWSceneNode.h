/*
 * ------------------------------------------------------------------------
 *  Name:   FSWSceneNode.h
 *  Desc:   本文件定义了一个场景节点对象。
 *  Author: Yish
 *  Date:   2013/4/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSW_SCENENODE_H__
#define __FSW_SCENENODE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "Common.h"
#include "Matrix.h"

///////////////////////////////////////////////////////////////////////////


class FSWSceneObject;

/** 场景节点
*/
class FSWSceneNode
{
public:
	typedef std::map<String,FSWSceneNode*> ChildNodeMap;
	typedef std::vector<FSWSceneObject*> SceneObjects;

public:
	FSWSceneNode( const String& name );
	virtual ~FSWSceneNode();

	// 更新该场景节点
	virtual void Update( float fElapsed );

	// 渲染该场景节点
	virtual void Render();

	// 添加一个场景对象到该节点
	bool AddSceneObject( FSWSceneObject* pObj );

	// 移除一个指定的场景对象
	void RemoveSceneObject( FSWSceneObject* pObj );

	// 移除所有场景对象
	void RemoveAllSceneObjects();

	// 获取指定场景对象
	FSWSceneObject* GetSceneObject( size_t iIndex );

	// 获取场景对象个数
	size_t GetObjectCount() const { return m_Objects.size(); }

	// 创建一个子节点
	FSWSceneNode* CreateChild( const String& name );

	// 移除一个子节点
	void RemoveChildNode( const String& name );
	void RemoveChildNode( FSWSceneNode* pChild );

	// 移除所有子节点
	void RemoveAllChildren();

	// 获取指定子节点
	FSWSceneNode* GetChildNode( const String& name );

	// 获取子节点的个数
	size_t GetChildrenCount() const { return m_Children.size(); }

	// 获取父节点
	FSWSceneNode* GetParentNode() { return m_pParent; }

	// 设置父节点
	void SetParentNode( FSWSceneNode* pNode ) { m_pParent = pNode; }

	// 设置节点的位置
	void SetPosition( float x,float y,float z,bool bLocal=true );

	// 移动节点
	void Move( const Vector3& vec,bool bLocal=true );
	void MoveX( float fX,bool bLocal=true ) { Move(Vector3(fX,0.0f,0.0f), bLocal); }
	void MoveY( float fY,bool bLocal=true ) { Move(Vector3(0.0f,fY,0.0f), bLocal); }
	void MoveZ( float fZ,bool bLocal=true ) { Move(Vector3(0.0f,0.0f,fZ), bLocal); }

	// 绕X轴旋转节点
	void Pitch( float fAngle,bool bLocal=true );

	// 绕Y轴旋转节点
	void Yaw( float fAngle,bool bLocal=true );

	// 绕Z轴旋转模型
	void Roll( float fAngle,bool bLocal=true );

	// 缩放模型
	void Scale( float x,float y,float z,bool bLocal=true );

	// 设置绝对矩阵
	void SetAbsoluteTM( const Matrix44& mat );

	// 设置相对矩阵
	void SetRelativeTM( const Matrix44& mat );

	// 获取相对矩阵
	const Matrix44& GetRelativeTM() const { return m_matRelativeTM; }

	// 获取绝对矩阵
	const Matrix44& GetAbsoluteTM();

	// 获取节点的名字
	const String& GetName() const { return m_sName; }

protected:
	String m_sName;					// 当前节点的名字
	Matrix44 m_matAbsoluteTM;		// 变换到世界空间的矩阵
	Matrix44 m_matRelativeTM;		// 相对于父节点的矩阵
	bool m_bAbsTmDirty;				// 是否需要更新世界的矩阵
	FSWSceneNode* m_pParent;		// 父节点
	ChildNodeMap m_Children;		// 子节点
	SceneObjects m_Objects;			// 场景对象列表

protected:

	// 计算绝对矩阵
	void UpdateAbsluteTM();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_SCENENODE_H__