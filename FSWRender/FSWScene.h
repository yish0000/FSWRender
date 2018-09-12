/*
 * ------------------------------------------------------------------------
 *  Name:   FSWScene.h
 *  Desc:   ���ļ�������һ����������
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

/** ������
*/
class FSWScene
{
public:

	enum ObjectType
	{
		OBJECT_MODEL,		// ��̬ģ��
		OBJECT_SKELETON,	// ��������ģ��
		OBJECT_PARTICLE,	// ����ϵͳ
	};

	typedef std::map<String,FSWSceneObject*> SceneObjects;

public:
	FSWScene( FSWDevice* pDevice );
	virtual ~FSWScene();

	// ���ٵ�ǰ��������
	void Reset();

	// ����һ����������
	FSWSceneObject* CreateObject( const String& name,ObjectType type );

	// �Ƴ�ָ���ĳ�������
	void RemoveObject( const String& name );

	// �Ƴ����еĳ�������
	void RemoveAllObjects();

	// ��ȡ��������ĸ���
	size_t GetObjectCount() const { return m_Objects.size(); }

	// ��ȡָ���ĳ�������
	FSWSceneObject* GetObjectByName( const String& name );

	// ���³�����
	virtual void Update( float fElapsed );

	// ��Ⱦ������
	virtual void Render();

	// ���ó�������ģʽ
	void SetFogMode(bool bEnable, float fNear = 15.0f, float fFar = 45.0f);

	// ��ȡ�����ĸ��ڵ�
	FSWSceneNode* GetSceneRoot() const { return m_pRoot; }

	// ��ȡ�ó������������
	FSWCamera* GetCamera() const { return m_pCamera; }

protected:
	FSWDevice* m_pDevice;			// 3D�����豸ָ��
	FSWSceneNode* m_pRoot;			// �����ĸ��ڵ�
	FSWCamera* m_pCamera;			// �ó������������
	SceneObjects m_Objects;			// ���������б�

	bool m_bFogEnable;				// �Ƿ����ģʽ
	float m_fFogNear;				// ��������
	float m_fFogFar;				// ��Զ����
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_SCENE_H__