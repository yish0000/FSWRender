/*
 * ------------------------------------------------------------------------
 *  Name:   FSWSceneNode.cpp
 *  Desc:   ���ļ�ʵ����һ�������ڵ����
 *  Author: Yish
 *  Date:   2013/4/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FSWSceneNode.h"
#include "FSWSceneObject.h"


/** ���캯��
*/
FSWSceneNode::FSWSceneNode( const String& name ) : m_sName(name),m_pParent(NULL)
{
	m_bAbsTmDirty = true;
	m_matAbsoluteTM.Identity();
	m_matRelativeTM.Identity();
}

/** ��������
*/
FSWSceneNode::~FSWSceneNode()
{
	RemoveAllSceneObjects();
	RemoveAllChildren();
}

/** ���¸ó����ڵ�
*/
void FSWSceneNode::Update( float fElapsed )
{
	UpdateAbsluteTM();

	// �������г�������
	SceneObjects::iterator it;
	for( it=m_Objects.begin();it!=m_Objects.end();++it )
		(*it)->Update(fElapsed);

	// �����ӽڵ�
	ChildNodeMap::iterator child;
	for( child=m_Children.begin();child!=m_Children.end();++child )
		child->second->Update(fElapsed);
}

/** ��Ⱦ�ó����ڵ�
*/
void FSWSceneNode::Render()
{
	SceneObjects::iterator it;
	for( it=m_Objects.begin();it!=m_Objects.end();++it )
		(*it)->Render();

	// ��Ⱦ�ӽڵ�
	ChildNodeMap::iterator child;
	for( child=m_Children.begin();child!=m_Children.end();++child )
		child->second->Render();
}

/** ���һ���������󵽸ýڵ�
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

/** �Ƴ�ָ���ĳ�������
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

/** �Ƴ����г�������
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

/** ��ȡָ����������
*/
FSWSceneObject* FSWSceneNode::GetSceneObject( size_t iIndex )
{
	return iIndex < m_Objects.size() ? m_Objects[iIndex] : NULL;
}

/** ����һ���ӽڵ�
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

/** �Ƴ�ָ�����ӽڵ�
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

/** �Ƴ�ָ�����ӽڵ�
*/
void FSWSceneNode::RemoveChildNode( FSWSceneNode* pChild )
{
	if( pChild )
		RemoveChildNode(pChild->GetName());
}

/** �Ƴ����нڵ�
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

/** ��ȡָ�����ӽڵ�
*/
FSWSceneNode* FSWSceneNode::GetChildNode( const String& name )
{
	ChildNodeMap::iterator it;
	if( (it = m_Children.find(name)) != m_Children.end() )
		return it->second;
	else
		return NULL;
}

/** ���ýڵ��λ��
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

/** �ƶ��ڵ�
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

/** ��X����ת�ڵ�
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

/** ��Y����ת�ڵ�
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

/** ��Z����ת�ڵ�
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

/** ���Žڵ�
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

/** ���½ڵ�ľ��Ծ���
*/
void FSWSceneNode::UpdateAbsluteTM()
{
	if( m_bAbsTmDirty )
	{
		// ���㱾�ڵ�ľ��Ծ���
		if( m_pParent )
			m_matAbsoluteTM = m_pParent->GetAbsoluteTM() * m_matRelativeTM;
		else
			m_matAbsoluteTM = m_matRelativeTM;

		m_bAbsTmDirty = false;
	}
}

/** ���þ��Ծ���
*/
void FSWSceneNode::SetAbsoluteTM( const Matrix44& mat )
{
	m_matAbsoluteTM = mat;
	m_bAbsTmDirty = false;
}

/** ������Ծ���
*/
void FSWSceneNode::SetRelativeTM( const Matrix44& mat )
{
	m_matRelativeTM = mat;
	m_bAbsTmDirty = true;
}

/** ��ȡ���Ծ���
*/
const Matrix44& FSWSceneNode::GetAbsoluteTM()
{
	if( m_bAbsTmDirty )
		UpdateAbsluteTM();
	return m_matAbsoluteTM;
}