#include "stdafx.h"
#include "..\Headers\DataManager.h"

_IMPLEMENT_SINGLETON(CDataManager)

CDataManager::CDataManager()
{
}

void CDataManager::Set_Terrain_Data()
{
	TERRAINDATA* pTerrainData = new TERRAINDATA;

	pTerrainData->iNumVtxX = m_iNumVtxX;
	pTerrainData->iNumVtxZ = m_iNumVtxZ;
	pTerrainData->fInterval = m_fInterval;
	pTerrainData->fDetail = m_fDetail;
	pTerrainData->pImgPath = m_pImgPath;

	m_MapTerrainData.emplace(1, pTerrainData);
}

void CDataManager::Take_Out_Terrain_Data_In_Map()
{
	for (auto& Pair : m_MapTerrainData)
	{
		Pair.second->iNumVtxX = m_iNumVtxX;
		Pair.second->iNumVtxZ = m_iNumVtxX;
		Pair.second->fInterval = m_fInterval;
		Pair.second->fDetail = m_fDetail;
		Pair.second->pImgPath = m_pImgPath;
	}
}

void CDataManager::Free()
{
	for (auto& pair : m_MapTerrainData)
	{
		//Safe_Delete_Array(pair.second->pImgPath);
		Safe_Delete(pair.second);
	}

	//for (auto& pair : m_MapMeshData)
	//{
	//	for(auto& iter : pair.second)
	//		Safe_Delete(pair.second);
	//}

	m_MapTerrainData.clear();

}
