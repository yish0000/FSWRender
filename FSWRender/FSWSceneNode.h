/*
 * ------------------------------------------------------------------------
 *  Name:   FSWSceneNode.h
 *  Desc:   ���ļ�������һ�������ڵ����
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

/** �����ڵ�
*/
class FSWSceneNode
{
public:
	typedef std::map<String,FSWSceneNode*> ChildNodeMap;
	typedef std::vector<FSWSceneObject*> SceneObjects;

public:
	FSWSceneNode( const String& name );
	virtual ~FSWSceneNode();

	// ���¸ó����ڵ�
	virtual void Update( float fElapsed );

	// ��Ⱦ�ó����ڵ�
	virtual void Render();

	// ���һ���������󵽸ýڵ�
	bool AddSceneObject( FSWSceneObject* pObj );

	// �Ƴ�һ��ָ���ĳ�������
	void RemoveSceneObject( FSWSceneObject* pObj );

	// �Ƴ����г�������
	void RemoveAllSceneObjects();

	// ��ȡָ����������
	FSWSceneObject* GetSceneObject( size_t iIndex );

	// ��ȡ�����������
	size_t GetObjectCount() const { return m_Objects.size(); }

	// ����һ���ӽڵ�
	FSWSceneNode* CreateChild( const String& name );

	// �Ƴ�һ���ӽڵ�
	void RemoveChildNode( const String& name );
	void RemoveChildNode( FSWSceneNode* pChild );

	// �Ƴ������ӽڵ�
	void RemoveAllChildren();

	// ��ȡָ���ӽڵ�
	FSWSceneNode* GetChildNode( const String& name );

	// ��ȡ�ӽڵ�ĸ���
	size_t GetChildrenCount() const { return m_Children.size(); }

	// ��ȡ���ڵ�
	FSWSceneNode* GetParentNode() { return m_pParent; }

	// ���ø��ڵ�
	void SetParentNode( FSWSceneNode* pNode ) { m_pParent = pNode; }

	// ���ýڵ��λ��
	void SetPosition( float x,float y,float z,bool bLocal=true );

	// �ƶ��ڵ�
	void Move( const Vector3& vec,bool bLocal=true );
	void MoveX( float fX,bool bLocal=true ) { Move(Vector3(fX,0.0f,0.0f), bLocal); }
	void MoveY( float fY,bool bLocal=true ) { Move(Vector3(0.0f,fY,0.0f), bLocal); }
	void MoveZ( float fZ,bool bLocal=true ) { Move(Vector3(0.0f,0.0f,fZ), bLocal); }

	// ��X����ת�ڵ�
	void Pitch( float fAngle,bool bLocal=true );

	// ��Y����ת�ڵ�
	void Yaw( float fAngle,bool bLocal=true );

	// ��Z����תģ��
	void Roll( float fAngle,bool bLocal=true );

	// ����ģ��
	void Scale( float x,float y,float z,bool bLocal=true );

	// ���þ��Ծ���
	void SetAbsoluteTM( const Matrix44& mat );

	// ������Ծ���
	void SetRelativeTM( const Matrix44& mat );

	// ��ȡ��Ծ���
	const Matrix44& GetRelativeTM() const { return m_matRelativeTM; }

	// ��ȡ���Ծ���
	const Matrix44& GetAbsoluteTM();

	// ��ȡ�ڵ������
	const String& GetName() const { return m_sName; }

protected:
	String m_sName;					// ��ǰ�ڵ������
	Matrix44 m_matAbsoluteTM;		// �任������ռ�ľ���
	Matrix44 m_matRelativeTM;		// ����ڸ��ڵ�ľ���
	bool m_bAbsTmDirty;				// �Ƿ���Ҫ��������ľ���
	FSWSceneNode* m_pParent;		// ���ڵ�
	ChildNodeMap m_Children;		// �ӽڵ�
	SceneObjects m_Objects;			// ���������б�

protected:

	// ������Ծ���
	void UpdateAbsluteTM();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_SCENENODE_H__