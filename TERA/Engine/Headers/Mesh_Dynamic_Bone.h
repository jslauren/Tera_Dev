#pragma once
#include "Component.h"

_BEGIN(Engine)

class CAnimationCtrl;
class CHierarchyLoader_Player;
class _DLL_EXPORTS CMesh_Dynamic_Bone final : public CComponent
{
private:
	explicit CMesh_Dynamic_Bone(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh_Dynamic_Bone(const CMesh_Dynamic_Bone& rhs);
	virtual ~CMesh_Dynamic_Bone() = default;
public:	// Setter
	void	Set_HeadFrame(D3DXFRAME* pFrame) { m_pHeadFrame = pFrame; }
	void	Set_BodyFrame(D3DXFRAME* pFrame) { m_pBodyFrame = pFrame; }
	void	Set_HandFrame(D3DXFRAME* pFrame) { m_pArmsFrame = pFrame; }
	void	Set_LegFrame(D3DXFRAME* pFrame) { m_pLegsFrame = pFrame; }
	void	Set_TailFrame(D3DXFRAME* pFrame) { m_pTailFrame = pFrame; }

public:
	const D3DXFRAME_DERIVED*	Get_FrameDesc(const char* pFrameName) { return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName); }
	const _uint&				Get_NowPlayAniIndex();
	const _double&				Get_CurrentTrackPos();
	const _double&				Get_Period();
public:
	HRESULT Ready_Mesh_Dynamic(const _tchar* pFilePath, const _tchar* pFileName);
	HRESULT SetUp_AnimationSet(_uint iIndex, _float fAniSpeed = 1.f);
	HRESULT SetUp_AnimationSet(const char* pName);
	HRESULT Play_Animation(const _float& fTimeDelta, const _float fAniSpeed = 1.f);
public:
	_bool	IsAnimationEnded(_double fCtrlEndTime = 0.f);
	HRESULT ChangePivot(_vec3 vState, _float fDegree);
private:
	CHierarchyLoader_Player*	m_pLoader = nullptr;
	D3DXFRAME*					m_pRootFrame = nullptr;
	D3DXFRAME*					m_pHeadFrame = nullptr;
	D3DXFRAME*					m_pBodyFrame = nullptr;
	D3DXFRAME*					m_pArmsFrame = nullptr;
	D3DXFRAME*					m_pLegsFrame = nullptr;
	D3DXFRAME*					m_pTailFrame = nullptr;

	CAnimationCtrl*				m_pAniCtrl = nullptr;
	_matrix						m_matPivot;

private:
	HRESULT Update_CombinedTransformationMatrix(D3DXFRAME* pFrame, const D3DXMATRIX* pParentMatrix, D3DXFRAME* pSub1, D3DXFRAME* pSub2, D3DXFRAME* pSub3, D3DXFRAME* pSub4, D3DXFRAME* pSub5);

public:
	static CMesh_Dynamic_Bone*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*			Clone(void* pArg = nullptr);
	virtual void				Free();

};

_END