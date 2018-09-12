/*
 * ------------------------------------------------------------------------
 *  Name:   FSWModel.h
 *  Desc:   本文件定义了一个模型对象。
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

/** 模型对象
*/
class FSWModel : public FSWSceneObject
{
	// 材质
    struct MATERIAL
    {
        SMaterial material;
        FSWTexture* texture;
        bool bAlpha;
    };

	// 子网格
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

	// 更新场景对象
	void Update( float fElapsed );

	// 渲染模型
	void Render();

	// 从指定的文件中加载模型数据
	bool LoadModel( const char* filename );

    // 添加一个材质
    bool AddMaterial( const SMaterial& material, const char* szTexture );

    // 移除指定的材质
    void RemoveMaterial( UINT nIndex );

    // 移除所有的材质
    void RemoveAllMaterials();

    // 添加一个子网格
    bool AddSubMesh( UINT nNumVerts, const SVertex3D* pVerts, UINT nNumIndis,
        const WORD* pIndis, UINT nMaterialID );

    // 移除指定的子网格
    void RemoveSubMesh( UINT nIndex );

    // 移除所有的子网格
    void RemoveAllSubMeshes();

    // 手动计算网格的法向量
    void BuildNormals();

    // 显示顶点的法向量
    void ShowNormals() { m_bShowNormal = !m_bShowNormal; }

protected:
    FSWDevice* m_pDevice;           // 图形设备的指针
    MaterialList m_Materials;       // 网格的材质列表
    SubMeshList m_SubMeshes;        // 子网格列表
    bool m_bShowNormal;             // 是否显示法线

    // 创建法线的线段数据
    void _BuildNormalLines();
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSW_MODEL_H__