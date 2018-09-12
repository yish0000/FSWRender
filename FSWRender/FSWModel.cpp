/*
 * ------------------------------------------------------------------------
 *  Name:   FSWModel.cpp
 *  Desc:   ���ļ�ʵ����һ��ģ���ࡣ
 *  Author: Yish
 *  Date:   2013/3/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FSWModel.h"
#include "FSWDevice.h"
#include "FSWTexture.h"


///////////////////////////////////////////////////////////////////////////
// SMD��̬ģ���ļ���ʽ

struct SMD_HEADER
{
    DWORD dwVersion;
    LONG nNumMaterials;
    LONG nNumMeshes;
};

struct SMD_MATERIAL
{
    float fAmbient[4];
    float fDiffuse[4];
    float fEmissive[4];
    float fSpecular[4];
    float fPower;
    std::string sTexture;
};

struct SMD_VERTEX
{
    float x,y,z;
    float normal[3];
    float tu,tv;
};

struct SMD_MESH
{
    UINT nMaterialID;
    UINT nNumVerts;
    UINT nNumFaces;
    SMD_VERTEX* pVerts;
    WORD* pIndis;
};

///////////////////////////////////////////////////////////////////////////

// ��ȡָ���ļ�·����Ŀ¼
static std::string _ExtractFolder( const std::string& path )
{
    std::string folder;
    std::string temp = path;
    std::replace( temp.begin(), temp.end(), '\\', '/' );
    size_t i = temp.find_last_of('/');
    if( i == std::string::npos )
        folder = "";
    else
        folder = temp.substr(0, i+1);
    return folder;
}

///////////////////////////////////////////////////////////////////////////

/** ���캯��
*/
FSWModel::FSWModel( FSWDevice *pDevice ) : m_pDevice(pDevice),m_bShowNormal(false)
{
}

/** ��������
*/
FSWModel::~FSWModel()
{
    RemoveAllSubMeshes();
    RemoveAllMaterials();
}

/** ���һ������
*/
bool FSWModel::AddMaterial( const SMaterial& material, const char* szTexture )
{
    MATERIAL mat;

    // ��������
    memcpy( &mat.material, &material, sizeof(SMaterial) );

    if( szTexture )
    {
        mat.texture = new FSWTexture();
        if( !mat.texture->LoadFromBitmap(szTexture) )
        {
            delete mat.texture;
            return false;
        }

        mat.bAlpha = mat.texture->HaveAlpha();
    }
    else
    {
        mat.texture = NULL;
        mat.bAlpha = false;
    }

    // ��ӵ������б�
    m_Materials.push_back( mat );
    return true;
}

/** �Ƴ�һ��ָ���Ĳ��ʣ�ͬʱ�Ƴ�ʹ�øò��ʵ�������
*/
void FSWModel::RemoveMaterial( UINT nIndex )
{
    if( nIndex < m_Materials.size() )
    {
        for( UINT i=0;i<m_SubMeshes.size();i++ )
        {
            if( m_SubMeshes[i].nMaterialID == nIndex )
            {
                RemoveSubMesh(i);
                --i;
            }
        }

        // �Ƴ�����
        SAFE_DELETE( m_Materials[nIndex].texture );
        m_Materials.erase( m_Materials.begin() + nIndex );
    }
}

// �Ƴ����еĲ���
void FSWModel::RemoveAllMaterials()
{
    RemoveAllSubMeshes();

    MaterialList::iterator it;
    for( it=m_Materials.begin();it!=m_Materials.end();++it )
        SAFE_DELETE( it->texture );
    m_Materials.clear();
}

// ���һ��������
bool FSWModel::AddSubMesh( UINT nNumVerts, const SVertex3D* pVerts,
                           UINT nNumIndis, const WORD* pIndis, UINT nMaterialID )
{
    SUBMESH sm;
    sm.nMaterialID = nMaterialID;
    sm.nNumVerts = nNumVerts;
    sm.nNumIndis = nNumIndis;
    sm.pNormalVB = NULL;
    sm.pNormalIB = NULL;

    // �������㻺����
    sm.pVB = new SVertex3D[nNumVerts];
    memcpy( sm.pVB, pVerts, sizeof(SVertex3D)*nNumVerts );

    // ��������������
    sm.pIB = new WORD[nNumIndis];
    memcpy( sm.pIB, pIndis, sizeof(WORD)*nNumIndis );

    // ��ӵ��������б�
    m_SubMeshes.push_back(sm);
    return true;
}

/** �Ƴ�ָ����������
*/
void FSWModel::RemoveSubMesh( UINT nIndex )
{
    if( nIndex < m_SubMeshes.size() )
    {
        SAFE_DELETE_ARRAY( m_SubMeshes[nIndex].pVB );
        SAFE_DELETE_ARRAY( m_SubMeshes[nIndex].pIB );
        SAFE_DELETE_ARRAY( m_SubMeshes[nIndex].pNormalVB );
        SAFE_DELETE_ARRAY( m_SubMeshes[nIndex].pNormalIB );
        m_SubMeshes.erase( m_SubMeshes.begin() + nIndex );
    }
}

/** �Ƴ����е�������
*/
void FSWModel::RemoveAllSubMeshes()
{
    SubMeshList::iterator it;
    for( it=m_SubMeshes.begin();it!=m_SubMeshes.end();++it )
    {
        SAFE_DELETE_ARRAY( it->pVB );
        SAFE_DELETE_ARRAY( it->pIB );
        SAFE_DELETE_ARRAY( it->pNormalVB );
        SAFE_DELETE_ARRAY( it->pNormalIB );
    }

    m_SubMeshes.clear();
}

/** ���ļ��м�����������
*/
bool FSWModel::LoadModel( const char* filename )
{
    FILE* pFile;
    char identifier[4];
    SMD_HEADER header;

    pFile = fopen(filename, "rb");
    if( !pFile ) return false;

    fread( identifier,sizeof(char),4,pFile );
    if( identifier[0] != 'S' || identifier[1] != 'M' || identifier[2] != 'D' ||
        identifier[3] != '\0' )
    {
        fclose( pFile );
        return false;
    }

    // ����ɵ�ģ������
    RemoveAllSubMeshes();
    RemoveAllMaterials();

    // ��ȡ�ļ�ͷ
    fread( &header, sizeof(SMD_HEADER), 1, pFile );

    // ��ȡ������Ϣ
    for( LONG i=0;i<header.nNumMaterials;i++ )
    {
        size_t len;
        std::string sTexture, sPath;

        SMaterial mat;
        fread(&mat, sizeof(SMaterial), 1, pFile);
        fseek(pFile, sizeof(float)*9, SEEK_CUR);

        // ��ȡ������Ϣ
        fread(&len, sizeof(size_t), 1, pFile);
        sTexture.resize(len, '\0');
        fread(&sTexture[0], len, 1, pFile);
        sPath = _ExtractFolder(filename) + sTexture;

        // ��Ӳ���
        AddMaterial(mat, sPath.c_str());
    }

    // ��ȡ����������
    for( LONG m=0;m<header.nNumMeshes;m++ )
    {
        UINT nNumVerts, nNumFaces;
        SVertex3D* pVerts;
        WORD* pIndis;
        UINT nMaterialID;

        fread(&nMaterialID, sizeof(UINT), 1, pFile);
        fread(&nNumVerts, sizeof(UINT), 1, pFile);
        fread(&nNumFaces, sizeof(UINT), 1, pFile);

        // ��ȡ��������
        pVerts = new SVertex3D[nNumVerts];
        for( UINT v=0;v<nNumVerts;v++ )
        {
            fread(&pVerts[v], sizeof(Vector3), 2, pFile);
            fread(&pVerts[v].tex, sizeof(Vector2), 1, pFile);
            pVerts[v].color = 0xffffffff;
        }

        // ��ȡ��������
        pIndis = new WORD[nNumFaces*3];
        fread(pIndis, sizeof(WORD), nNumFaces*3, pFile);

        // ���������
        AddSubMesh(nNumVerts, pVerts, nNumFaces*3, pIndis, nMaterialID);

        delete[] pVerts;
        delete[] pIndis;
    }

    fclose( pFile );

    // ���������߶�
    _BuildNormalLines();
    return true;
}

/** ����ģ��
*/
void FSWModel::Update( float fElapsed )
{
}

/** ��Ⱦ��ģ��
*/
void FSWModel::Render()
{
    // ����ģ�͵��������
    m_pDevice->SetTransform(FTS_WORLD, GetWorldMatrix());

    // ���λ���������
    SubMeshList::iterator it;
    for( it=m_SubMeshes.begin();it!=m_SubMeshes.end();++it )
    {
        MATERIAL& mat = m_Materials[it->nMaterialID];

        m_pDevice->SetMaterial( mat.material );
        m_pDevice->SetTexture( mat.texture );
        m_pDevice->SetAlphaBlend( mat.bAlpha );

        m_pDevice->DrawPrimitive(FPT_TRIANGLELIST, it->pVB, it->nNumVerts,
            it->pIB, it->nNumIndis/3);

        // �Ƿ���Ʒ���
        if( m_bShowNormal )
        {
            m_pDevice->DrawPrimitive(FPT_LINELIST, it->pNormalVB, it->nNumVerts*2,
                it->pNormalIB, it->nNumVerts);
        }
    }
}

/** �ֶ���������ķ���
*/
void FSWModel::BuildNormals()
{
    UINT v, f;
    UINT nNumFaces;

    SubMeshList::iterator sm;
    for( sm=m_SubMeshes.begin();sm!=m_SubMeshes.end();++sm )
    {
        nNumFaces = sm->nNumIndis / 3;

        // ����ÿ������������淨��Ȩ�غ�
        float* pWeights = new float[sm->nNumVerts];

        // ����������ķ�����Ϊ0
        for( v=0;v<sm->nNumVerts;++v )
        {
            sm->pVB[v].normal = Vector3::ZERO;
            pWeights[v] = 0.0f;
        }

        // �������е�������
        for( f=0;f<nNumFaces;++f )
        {
            float fWeight;
            Vector3 vNormal, v1, v2;

            WORD w1 = sm->pIB[f*3+0];
            WORD w2 = sm->pIB[f*3+1];
            WORD w3 = sm->pIB[f*3+2];

            // ���������εķ���
            v1 = sm->pVB[w2].pos - sm->pVB[w1].pos;
            v2 = sm->pVB[w3].pos - sm->pVB[w1].pos;
            v1.Normalize();
            v2.Normalize();
            vNormal.Cross(v1, v2);
            vNormal.Normalize();

            // ���㱾���Ȩ��(�Ƕ�Խ�󣬻�Ϻ�ķ���Խƫ����)
            fWeight = acosf(v1.DotProduct(v2));

            // ������������ϱ���ķ���
            sm->pVB[w1].normal += vNormal * fWeight;
            sm->pVB[w2].normal += vNormal * fWeight;
            sm->pVB[w3].normal += vNormal * fWeight;
            pWeights[w1] += fWeight;
            pWeights[w2] += fWeight;
            pWeights[w3] += fWeight;
        }

        // ����ÿ������Ļ�Ϻ�ķ���
        for( v=0;v<sm->nNumVerts;v++ )
        {
            if( pWeights[v] != 0.0f )
                sm->pVB[v].normal /= pWeights[v];
            else
                sm->pVB[v].normal = Vector3::ZERO;
        }

        delete[] pWeights;
    }

    // �������ߵ��߶�����
    _BuildNormalLines();
}

/** ���������߶�
*/
void FSWModel::_BuildNormalLines()
{
    SubMeshList::iterator sm;
    for( sm=m_SubMeshes.begin();sm!=m_SubMeshes.end();++sm )
    {
        SAFE_DELETE_ARRAY( sm->pNormalVB );
        SAFE_DELETE_ARRAY( sm->pNormalIB );

        // �������ߵ�����
        sm->pNormalVB = new SVertex3D[sm->nNumVerts*2];
        memset(sm->pNormalVB, 0, sizeof(SVertex3D)*sm->nNumVerts*2);
        sm->pNormalIB = new WORD[sm->nNumVerts*2];
        memset(sm->pNormalIB, 0, sizeof(WORD)*sm->nNumVerts*2);
        for( UINT v=0;v<sm->nNumVerts;++v )
        {
            SVertex3D* start = &sm->pNormalVB[v*2+0];
            SVertex3D* end = &sm->pNormalVB[v*2+1];
            start->pos = sm->pVB[v].pos;
            start->color = 0xffffffff;
            Vector3 vNorm = sm->pVB[v].normal;
            vNorm.Normalize();
            end->pos = start->pos + vNorm;
            end->color = 0xffffffff;
            sm->pNormalIB[v*2+0] = v*2+0;
            sm->pNormalIB[v*2+1] = v*2+1;
        }
    }
}