/*
 * ------------------------------------------------------------------------
 *  Name:   FSWSceneObject.cpp
 *  Desc:   ���ļ�ʵ����һ������������ࡣ
 *  Author: Yish
 *  Date:   2013/4/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */



#include "FSWSceneObject.h"
#include "FSWSceneNode.h"


// ���캯��
FSWSceneObject::FSWSceneObject() : m_pNode(NULL)
{
}

// ��������
FSWSceneObject::~FSWSceneObject()
{
	// ���������ĳ���ڵ㣬��Ӹýڵ��Ƴ�
	if( m_pNode )
		m_pNode->RemoveSceneObject(this);
}

// ��ȡ�������
const Matrix44& FSWSceneObject::GetWorldMatrix() const
{
	return m_pNode->GetAbsoluteTM();
}