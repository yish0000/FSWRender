/*
 * ------------------------------------------------------------------------
 *  Name:   FSWSceneNode.cpp
 *  Desc:   本文件实现了一个场景节点对象。
 *  Author: Yish
 *  Date:   2013/4/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FSWSceneNode.h"
#include "FSWSceneObject.h"


/** 构造函数
*/
FSWSceneNode::FSWSceneNode( const String& name ) : m_sName(name),m_pParent(NULL)
{
	m_bAbsTmDirty = true;
	m_matAbsoluteTM.Identity();
	m_matRelativeTM.Identity();
}

/** 析构函数
*/
FSWSceneNode::~FSWSceneNode()
{
	RemoveAllSceneObjects();
	RemoveAllChildren();
}

/** 更新该场景节点
*/
void FSWSceneNode::Update( float fElapsed )
{
	UpdateAbsluteTM();

	// 更新所有场景对象
	SceneObjects::iterator it;
	for( it=m_Objects.begin();it!=m_Objects.end();++it )
		(*it)->Update(fElapsed);

	// 更新子节点
	ChildNodeMap::iterator child;
	for( child=m_Children.begin();child!=m_Children.end();++child )
		child->second->Update(fElapsed);
}

/** 渲染该场景节点
*/
void FSWSceneNode::Render()
{
	SceneObjects::iterator it;
	for( it=m_Objects.begin();it!=m_Objects.end();++it )
		(*it)->Render();

	// 渲染子节点
	ChildNodeMap::iterator child;
	for( child=m_Children.begin();child!=m_Children.end();++child )
		child->second->Render();
}

/** 添加一个场景对象到该节点
*/
bool FSWSceneNode::AddSceneObject( FSWSceneObject* pObj )
{
	if( !pObj || pObj->GetParentNode() ||
		std::find(m_Objects.begin(), m_Objects.end(), pObj) != m_Objects.end() )
		return false;

	pObj->SetParentNode(this);
	m_Objects.push_back(pObj);
	return true;
}

/** 移除指定的场景对象
*/
void FSWSceneNode::RemoveSceneObject( FSWSceneObject* pObj )
{
	SceneObjects::iterator it;
	if( pObj && ((it = std::find(m_Objects.begin(),m_Objects.end(), pObj))
		!= m_Objects.end()) )
	{
		pObj->SetParentNode(NULL);
		m_Objects.erase(it);
	}
}

/** 移除所有场景对象
*/
void FSWSceneNode::RemoveAllSceneObjects()
{
	SceneObjects::iterator it;
	for( it=m_Objects.begin();it!=m_Objects.end();++it )
	{
		(*it)->SetParentNode(NULL);
	}

	m_Objects.clear();
}

/** 获取指定场景对象
*/
FSWSceneObject* FSWSceneNode::GetSceneObject( size_t iIndex )
{
	return iIndex < m_Objects.size() ? m_Objects[iIndex] : NULL;
}

/** 创建一个子节点
*/
FSWSceneNode* FSWSceneNode::CreateChild( const String& name )
{
	if( m_Children.find(name) != m_Children.end() )
		return false;

	FSWSceneNode* pNode = new FSWSceneNode(name);
	pNode->SetParentNode(this);
	m_Children[name] = pNode;
	return pNode;
}

/** 移除指定的子节点
*/
void FSWSceneNode::RemoveChildNode( const String& name )
{
	ChildNodeMap::iterator it;
	if( (it = m_Children.find(name)) != m_Children.end() )
	{
		SAFE_DELETE( it->second );
		m_Children.erase(it);
	}
}

/** 移除指定的子节点
*/
void FSWSceneNode::RemoveChildNode( FSWSceneNode* pChild )
{
	if( pChild )
		RemoveChildNode(pChild->GetName());
}

/** 移除所有节点
*/
void FSWSceneNode::RemoveAllChildren()
{
	ChildNodeMap::iterator it;
	for( it=m_Children.begin();it!=m_Children.end();++it )
	{
		SAFE_DELETE( it->second );
	}

	m_Children.clear();
}

/** 获取指定的子节点
*/
FSWSceneNode* FSWSceneNode::GetChildNode( const String& name )
{
	ChildNodeMap::iterator it;
	if( (it = m_Children.find(name)) != m_Children.end() )
		return it->second;
	else
		return NULL;
}

/** 设置节点的位置
*/
void FSWSceneNode::SetPosition( float x,float y,float z,bool bLocal/* =true */ )
{
	if( bLocal )
	{
		m_matRelativeTM._41 = x;
		m_matRelativeTM._42 = y;
		m_matRelativeTM._43 = z;
		m_bAbsTmDirty = true;
	}
	else
	{
		UpdateAbsluteTM();

		m_matAbsoluteTM._41 = x;
		m_matAbsoluteTM._42 = y;
		m_matAbsoluteTM._43 = z;
	}
}

/** 移动节点
*/
void FSWSceneNode::Move( const Vector3& vec,bool bLocal/* =true */ )
{
	if( bLocal )
	{
		m_matRelativeTM._41 += vec.x;
		m_matRelativeTM._42 += vec.y;
		m_matRelativeTM._43 += vec.z;
		m_bAbsTmDirty = true;
	}
	else
	{
		UpdateAbsluteTM();

		m_matAbsoluteTM._41 += vec.x;
		m_matAbsoluteTM._42 += vec.y;
		m_matAbsoluteTM._43 += vec.z;
	}
}

/** 绕X轴旋转节点
*/
void FSWSceneNode::Pitch( float fAngle,bool bLocal/* =true */ )
{
	Matrix44 m,mat;
	m.RotateX( fAngle );

	if( bLocal )
	{
		mat = m_matRelativeTM * m;
		m_matRelativeTM = mat;
		m_bAbsTmDirty = true;
	}
	else
	{
		UpdateAbsluteTM();

		mat = m_matAbsoluteTM * m;
		m_matAbsoluteTM = mat;
	}
}

/** 绕Y轴旋转节点
*/
void FSWSceneNode::Yaw( float fAngle,bool bLocal/* =true */ )
{
	Matrix44 m,mat;
	m.RotateY( fAngle );

	if( bLocal )
	{
		mat = m_matRelativeTM * m;
		m_matRelativeTM = mat;
		m_bAbsTmDirty = true;
	}
	else
	{
		UpdateAbsluteTM();

		mat = m_matAbsoluteTM * m;
		m_matAbsoluteTM = mat;
	}
}

/** 绕Z轴旋转节点
*/
void FSWSceneNode::Roll( float fAngle,bool bLocal/* =true */ )
{
	Matrix44 m,mat;
	m.RotateZ( fAngle );

	if( bLocal )
	{
		mat = m_matRelativeTM * m;
		m_matRelativeTM = mat;
		m_bAbsTmDirty = true;
	}
	else
	{
		UpdateAbsluteTM();

		mat = m_matAbsoluteTM * m;
		m_matAbsoluteTM = mat;
	}
}

/** 缩放节点
*/
void FSWSceneNode::Scale( float x,float y,float z,bool bLocal/* =true */ )
{
	Matrix44 m,mat;
	m.Scale(x, y, z);

	if( bLocal )
	{
		mat = m_matRelativeTM * m;
		m_matRelativeTM = mat;
		m_bAbsTmDirty = true;
	}
	else
	{
		UpdateAbsluteTM();

		mat = m_matAbsoluteTM * m;
		m_matAbsoluteTM = mat;
	}
}

/** 更新节点的绝对矩阵
*/
void FSWSceneNode::UpdateAbsluteTM()
{
	if( m_bAbsTmDirty )
	{
		// 计算本节点的绝对矩阵
		if( m_pParent )
			m_matAbsoluteTM = m_pParent->GetAbsoluteTM() * m_matRelativeTM;
		else
			m_matAbsoluteTM = m_matRelativeTM;

		m_bAbsTmDirty = false;
	}
}

/** 设置绝对矩阵
*/
void FSWSceneNode::SetAbsoluteTM( const Matrix44& mat )
{
	m_matAbsoluteTM = mat;
	m_bAbsTmDirty = false;
}

/** 设置相对矩阵
*/
void FSWSceneNode::SetRelativeTM( const Matrix44& mat )
{
	m_matRelativeTM = mat;
	m_bAbsTmDirty = true;
}

/** 获取绝对矩阵
*/
const Matrix44& FSWSceneNode::GetAbsoluteTM()
{
	if( m_bAbsTmDirty )
		UpdateAbsluteTM();
	return m_matAbsoluteTM;
}