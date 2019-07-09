#include "stdafx.h"
#include "..\Headers\Scene_Loading.h"
#include "Player.h"
#include "Scene_Stage.h"
#include "Scene_Dragon.h"
#include "Management.h"
#include "Back_Logo.h"
#include "UI_Loading.h"
#include "Loading.h"
#include "QuestNPC.h"
#include "SoundManager.h"
#include "Time.h"

_USING(Client)

CScene_Loading::CScene_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Loading::Ready_Scene(SCENEID eID)
{
	SoundSetting();

	m_eCurrentScene = eID;

	// 로고씬에서 사용할 원형컴포넌트들의 생성.
	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	// 로고씬에서 사용할 객체들의 원형을 준비해놓는다.
	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	m_pLoading = CLoading::Create(m_pGraphic_Device, m_eCurrentScene);
	if (nullptr == m_pLoading)
		return E_FAIL;
	
	m_pCUI_Loading = dynamic_cast<CUI_Loading*>(CObject_Manager::GetInstance()->Get_Object(SCENE_LOADING, L"Layer_BackGround"));
	Safe_AddRef(m_pCUI_Loading);

	return NOERROR;
}

_int CScene_Loading::Update_Scene(const _float & fTimeDelta)
{
	// 로딩 정도를 보내주기 위한 구문
	m_pCUI_Loading->Set_LoadingProgressValue(m_pLoading->Get_Complete());

	return _int(CScene::Update_Scene(fTimeDelta));
}

_int CScene_Loading::LateUpdate_Scene(const _float & fTimeDelta)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	pManagement->AddRef();
	
	if (100 == m_pLoading->Get_Complete())
	{
		if (m_eCurrentScene == SCENE_LOGO)
		{
			// 로고 씬에서 넘어올때만, 키 입력을 받아 넘긴다.
			if (GetKeyState(VK_SPACE) & 0x8000)
			{			
				if (FAILED(pManagement->SetUp_CurrentScene(CScene_Stage::Create(m_pGraphic_Device), SCENE_STAGE)))
				{
					Safe_Release(pManagement);
					return -1;
				}

				dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Set_Navigation_Component(SCENE_STAGE);

				CQuestNPC* pQuestNPC = dynamic_cast<CQuestNPC*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_NPC", 0));
				pQuestNPC->SetCurrentQuestStateInfo(CQuestNPC::QUEST_STATE::QUEST_START);
				pQuestNPC->Set_ScriptNumber(-1);
				pQuestNPC->Set_EndScriptNum(3);
				pQuestNPC->Set_LoopScriptNum(4);

				Safe_Release(pManagement);
				return 0;
			}
		}
		// 보통 씬 전환은 별도의 키 입력 없이 자동으로 이루어진다.
		//else if (m_eCurrentScene == SCENE_STAGE)
		//{
		//	// 이 부분에 추가.
		//	if (FAILED(pManagement->SetUp_CurrentScene(CScene_Stage::Create(m_pGraphic_Device), SCENE_DRAGON)))
		//	{
		//		Safe_Release(pManagement);
		//		return -1;
		//	}
		//	dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Set_Navigation_Component(SCENE_DRAGON);
		//	Safe_Release(pManagement);
		//	return 0;
		//}
		else if (m_eCurrentScene == SCENE_DRAGON)
		{
			//// [ 이 부분에 추가 씬 전환 시 필요한 값들을 셋팅해준다 ] ////
			CPlayer*	pPlayer = dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"));

			// 씬 전환 완료 후, 플레이어의 애니메이션을 지정해준는 구문.
			pPlayer->Set_AniIndex(CPlayer::PLAYER_ANI::Idle);
			// 씬 전환 완료 후, 플레이어의 포지션을 지정해주는 구문.
			pPlayer->Get_TransformMove()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(250.f, 0.f, 50.f));
			// 현재 플레이어가 밟고있는 땅의 Index를 설정해주는 구문.
			pPlayer->Get_NaviMesh()->SetUp_CurrentIndex(0);
			// 씬 전환 완료 후, 무기의 위치를 지정해주는 구문.
			dynamic_cast<CWeapon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Weapon"))->Set_BoneMatrix(1);

			if (FAILED(pManagement->SetUp_CurrentScene(CScene_Dragon::Create(m_pGraphic_Device), SCENE_DRAGON)))
			{
				Safe_Release(pManagement);
				return -1;
			}

			dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Set_Navigation_Component(SCENE_DRAGON);

			Safe_Release(pManagement);
			return 0;
		}
		else if (m_eCurrentScene == SCENE_STAGE)
		{
			CPlayer*	pPlayer = dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"));

			// 씬 전환 완료 후, 플레이어의 애니메이션을 지정해준는 구문.
			pPlayer->Set_AniIndex(CPlayer::PLAYER_ANI::Idle);
			// 씬 전환 완료 후, 플레이어의 포지션을 지정해주는 구문.
			pPlayer->Get_TransformMove()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(12.f, 0.f, 233.f));

			// 씬 전환 완료 후, 무기의 위치를 지정해주는 구문.
			dynamic_cast<CWeapon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Weapon"))->Set_BoneMatrix(1);

			if (FAILED(pManagement->SetUp_CurrentScene(CScene_Stage::Create(m_pGraphic_Device), SCENE_STAGE)))
			{
				Safe_Release(pManagement);
				return -1;
			}

			dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Set_Navigation_Component(SCENE_STAGE);
			
			// 현재 플레이어가 밟고있는 땅의 Index를 설정해주는 구문.
			pPlayer->Get_NaviMesh()->SetUp_CurrentIndex(pPlayer->Get_NaviMesh()->Get_CellIndex(_vec3(250.f, 0.f, 50.f)));
			
			CQuestNPC* pQuestNPC = dynamic_cast<CQuestNPC*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_NPC", 0));
			pQuestNPC->SetCurrentQuestStateInfo(CQuestNPC::QUEST_STATE::QUEST_REWARD);
			pQuestNPC->Set_EndScriptNum(3);
			pQuestNPC->Set_LoopScriptNum(4);

			Safe_Release(pManagement);
			return 0;
		}
	}

	//if (CManagement::GetInstance()->Get_PreScene() == (_uint)(SCENE_LOADING))
	//{
	//	if (GetKeyState(VK_SPACE) & 0x8000)
	//	{
	//		if (100 == m_pLoading->Get_Complete())
	//		{
	//			if (FAILED(pManagement->SetUp_CurrentScene(CScene_Stage::Create(m_pGraphic_Device), SCENE_STAGE)))
	//			{
	//				Safe_Release(pManagement);
	//				return -1;
	//			}
	//			dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Set_Navigation_Component(SCENE_STAGE);
	//			Safe_Release(pManagement);
	//			return 0;
	//		}
	//	}
	//}


	Safe_Release(pManagement);

	return _int(CScene::LateUpdate_Scene(fTimeDelta));
}

HRESULT CScene_Loading::Render_Scene()
{
	return CScene::Render_Scene();
}

HRESULT CScene_Loading::Ready_Component_Prototype()
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Loading::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CScene_Loading::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	// Add_Object에서 처음에 넣는 씬 ID는 프로토타입 ID은 원본객체,
	// 두 번째 씬 ID는 실사용 객체이다.
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_Loading", SCENE_LOADING, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

void CScene_Loading::SoundSetting()
{
	CSoundManager::GetInstance()->Stop_AllSound();

	srand((unsigned)time(NULL));

	int iRandMusic = rand() % 2;

	if (iRandMusic == 0)
		CSoundManager::GetInstance()->Play_BGM("Flower_Garden.ogg");
	else if(iRandMusic == 1)
		CSoundManager::GetInstance()->Play_BGM("WarriorAcademy_Vellica.ogg");

}

CScene_Loading * CScene_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, SCENEID eID)
{
	CScene_Loading* pInstance = new CScene_Loading(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene(eID)))
	{
		_MSGBOX("CScene_Loading Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CScene_Loading::Free()
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return;

	if (FAILED(m_pObject_Manager->Clear_Object(SCENE_LOADING)))
		return;

	if (FAILED(m_pObject_Manager->Clear_Prototype(SCENE_LOADING)))
		return;

	if (FAILED(m_pComponent_Manager->Clear_Component_Prototype(SCENE_LOADING)))
		return;

	Safe_Release(m_pLoading);
	Safe_Release(m_pCUI_Loading);

	CScene::Free();
}
