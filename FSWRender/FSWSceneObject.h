/*
 * ------------------------------------------------------------------------
 *  Name:   FSWSceneObject.h
 *  Desc:   ���ļ�������һ������������ࡣ
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

/** ��������
*/
class FSWSceneObject
{
public:
	FSWSceneObject();
	virtual ~FSWSceneObject();

	// ���³�������
	virtual void Update( float fElapsed ) = 0;

	// ��Ⱦ��������
	virtual void Render() = 0;

	// ��ȡ��������ı任����
	const Matrix44& GetWorldMatrix() const;

	// ��ȡ��ǰ�����λ��
	Vector3 GetPosition() const
	{
		const Matrix44& mWorld = GetWorldMatrix();
		return Vector3(mWorld._41, mWorld._42, mWorld._43);
	}

	// ��ȡ�����ڵ�
	FSWSceneNode* GetParentNode() { return m_pNode; }

	// ���������ڵ�
	void SetParentNode( FSWSceneNode* pNode ) { m_pNode = pNode; }

protected:
	FSWSceneNode* m_pNode;		// �����ڵ�
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_SCENEOBJECT_H__