//=============================================================================
//
// UI処理 [ui.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "ui.h"
#include "lifeber.h"
#include "score.h"
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "animalui.h"
#include "skillber.h"
#include "game.h"
#include "player.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
CLifeber *CUi::m_pLifeber = NULL;
CSkillber *CUi::m_pSkillber = NULL;
CScore * CUi::m_pScore = NULL;
CAnimalUi * CUi::m_pAnimalUI = NULL;
LPDIRECT3DTEXTURE9 CUi::m_pTexture[UI_TEX_MAX] = {};

//=============================================================================
//UIクラスのコンストラクタ
//=============================================================================
CUi::CUi()
{
}

//=============================================================================
//UIクラスのデストラクタ
//=============================================================================
CUi::~CUi()
{
}

//=============================================================================
//UIクラスのクリエイト処理
//=============================================================================
CUi * CUi::Create(void)
{
	CUi *pUi = NULL;

	//インスタンスの生成
	pUi = new CUi;

	if (pUi)
	{
		//初期化処理
		pUi->Init();
	}
	else
	{
		return NULL;
	}
	return pUi;
}

//=============================================================================
//UIクラスのテクスチャの読み込み処理
//=============================================================================
HRESULT CUi::Load(void)
{
	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/UnderUI.png", &m_pTexture[UI_TEX_1]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/gaugeUI.png", &m_pTexture[UI_TEX_2]);

	return S_OK;
}

//=============================================================================
//UIクラスのテクスチャの破棄処理
//=============================================================================
void CUi::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < UI_TEX_MAX; nCntTex++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCntTex])
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//UIクラスの初期化処理
//=============================================================================
HRESULT CUi::Init(void)
{
	CScene2D *pScene2D;	

	//上側UI用ポリゴン生成
	pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f + UNDER_UI_SIZE_Y / 2, 0.0f), 
		UNDER_UI_SIZE, CScene::PRIORITY_UI);
	pScene2D->BindTexture(m_pTexture[UI_TEX_1]);

	//スコアクラスの生成
	m_pScore = CScore::Create();

	////ライフクラスの生成
	m_pLifeber = CLifeber::Create(
		D3DXVECTOR3(100.0f, 15.0f, 0.0f), 
		LIFE_GAGUE_SIZE, 
		COLOR_GREEN,
		CGame::GetPlayer()->GetLife(), 
		CGame::GetPlayer()->GetMaxLife());

	//スキルクラスの生成
	m_pSkillber = CSkillber::Create(
		D3DXVECTOR3(100.0f, 41, 0.0f),
		SKILL_GAGUE_SIZE,
		COLOR_CYAN,
		MAX_SKILL,
		MAX_SKILL);

	//アニマルUIクラスの生成
	m_pAnimalUI = CAnimalUi::Create();

	//ポリゴンクラスを生成
	pScene2D = CScene2D::Create(D3DXVECTOR3((GAUGE_UI_SIZE_X / 2), (GAUGE_UI_SIZE_Y / 2), 0.0f),
		GAUGE_UI_SIZE, CScene::PRIORITY_UI);
	pScene2D->BindTexture(m_pTexture[UI_TEX_2]);
	//pScene2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
	return S_OK;
}

//=============================================================================
//UIクラスの終了処理
//=============================================================================
void CUi::Uninit(void)
{
}

//=============================================================================
//UIクラスの更新処理
//=============================================================================
void CUi::Update(void)
{
}

//=============================================================================
//UIクラスの描画処理
//=============================================================================
void CUi::Draw(void)
{
}

//=============================================================================
//スコア情報取得
//=============================================================================
CScore * CUi::GetScore(void)
{
	return m_pScore;
}

//=============================================================================
//ライフ情報取得
//=============================================================================
CLifeber * CUi::GetLifeber(void)
{
	return m_pLifeber;
}

//=============================================================================
//スキル情報取得
//=============================================================================
CSkillber * CUi::GetSkillber(void)
{
	return m_pSkillber;
}

//=============================================================================
//アニマルUI情報取得
//=============================================================================
CAnimalUi * CUi::GetAnimalUI(void) 
{
	return m_pAnimalUI;
}
