/*
 * ------------------------------------------------------------------------
 *  Name:   FSWModel.h
 *  Desc:   ���ļ�������һ��ģ�Ͷ���
 *  Author: Yish
 *  Date:   2013/3/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSW_MODEL_H__
#define __FSW_MODEL_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWTypes.h"
#include "FSWSceneObject.h"

///////////////////////////////////////////////////////////////////////////


class FSWDevice;
class FSWTexture;

/** ģ�Ͷ���
*/
class FSWModel : public FSWSceneObject
{
	// ����
    struct MATERIAL
    {
        SMaterial material;
        FSWTexture* texture;
        bool bAlpha;
    };

	// ������
    struct SUBMESH
    {
        UINT nNumVerts;
        UINT nNumIndis;
        SVertex3D* pVB;
        WORD* pIB;
        UINT nMaterialID;

        SVertex3D* pNormalVB;
        WORD* pNormalIB;
    };

    typedef std::vector<MATERIAL> MaterialList;
    typedef std::vector<SUBMESH> SubMeshList;

public:
    FSWModel( FSWDevice* pDevice );
    virtual ~FSWModel();

	// ���³�������
	void Update( float fElapsed );

	// ��Ⱦģ��
	void Render();

	// ��ָ�����ļ��м���ģ������
	bool LoadModel( const char* filename );

    // ���һ������
    bool AddMaterial( const SMaterial& material, const char* szTexture );

    // �Ƴ�ָ���Ĳ���
    void RemoveMaterial( UINT nIndex );

    // �Ƴ����еĲ���
    void RemoveAllMaterials();

    // ���һ��������
    bool AddSubMesh( UINT nNumVerts, const SVertex3D* pVerts, UINT nNumIndis,
        const WORD* pIndis, UINT nMaterialID );

    // �Ƴ�ָ����������
    void RemoveSubMesh( UINT nIndex );

    // �Ƴ����е�������
    void RemoveAllSubMeshes();

    // �ֶ���������ķ�����
    void BuildNormals();

    // ��ʾ����ķ�����
    void ShowNormals() { m_bShowNormal = !m_bShowNormal; }

protected:
    FSWDevice* m_pDevice;           // ͼ���豸��ָ��
    MaterialList m_Materials;       // ����Ĳ����б�
    SubMeshList m_SubMeshes;        // �������б�
    bool m_bShowNormal;             // �Ƿ���ʾ����

    // �������ߵ��߶�����
    void _BuildNormalLines();
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSW_MODEL_H__