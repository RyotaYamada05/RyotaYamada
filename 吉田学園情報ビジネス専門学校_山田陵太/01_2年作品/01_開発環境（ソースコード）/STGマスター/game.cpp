//=============================================================================
//
// メイン処理 [game.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "game.h"
#include "ui.h"
#include "player.h"
#include "enemy.h"
#include "bg.h"
#include "polygon.h"
#include "scene2d.h"
#include "boss.h"
#include "fade.h"
#include "manager.h"
#include "sound.h"
//=============================================================================
//静的メンバ変数宣言
//=============================================================================
CUi *CGame::m_pUi = NULL;
CPlayer *CGame::m_pPlayer = NULL;
CGame::GAME_STATE CGame::m_state = CGame::GAME_STATE_NOMRAL;
CPolygon *CGame::m_pPolygon = NULL;

//=============================================================================
//ゲームクラスのコンストラクタ
//=============================================================================
CGame::CGame(int nPriority) :CScene(nPriority)
{
	m_nEnemyCounter = 0;
	m_nStaeCounter = 0;
	m_pPolygon = NULL;
}

//=============================================================================
//ゲームクラスのデストラクタ
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
//ゲームクラスのクリエイト処理
//=============================================================================
CGame * CGame::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//ゲームクラスのポインタ変数
	CGame *pGame = NULL;

	//メモリの確保
	pGame = new CGame;

	//メモリが確保できていたら
	if (pGame != NULL)
	{
		//初期化処理呼び出し
		pGame->Init(pos, size);
	}
	//メモリ確保に失敗したとき
	else
	{
		return NULL;
	}

	return pGame;
}

//=============================================================================
//ゲームクラスの初期化処理
//=============================================================================
HRESULT CGame::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_state = CGame::GAME_STATE_NOMRAL;

	// 背景クラスの生成
	CBg::Create();

	//UIクラスの生成
	CUi::Create();

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f + PLAYER_SIZE_X / 2, SCREEN_HEIGHT / 2, 0.0f), PLAYER_SIZE);

	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME);
	
	return S_OK;
}

//=============================================================================
//ゲームクラスの終了処理
//=============================================================================
void CGame::Uninit(void)
{
	if (m_pPolygon != NULL)
	{
		m_pPolygon->Uninit();
		delete m_pPolygon;
		m_pPolygon = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//ゲームクラスの更新処理
//=============================================================================
void CGame::Update(void)
{
	switch (m_state)
	{
	case GAME_STATE_NOMRAL:
		m_nEnemyCounter++;

		if (m_nEnemyCounter == 60)
		{
			for (int nCount = 0; nCount < 6; nCount++)
			{
				CEnemy::Create(D3DXVECTOR3(1300.0f + ENEMY_SIZE_X * nCount, 200.0f, 0.0f),
					ENEMY_SIZE, CEnemy::ENEMY_TYPE_NO_BULLET, CEnemy::ACTION_PATTERN_MOVE_DOWN);

				CEnemy::Create(D3DXVECTOR3(1300.0f + ENEMY_SIZE_X * nCount, 600.0f, 0.0f),
					ENEMY_SIZE, CEnemy::ENEMY_TYPE_NO_BULLET, CEnemy::ACTION_PATTERN_MOVE_UP);
			}
		}
		if (m_nEnemyCounter == 300)
		{
			for (int nCount = 0; nCount < 6; nCount++)
			{
				CEnemy::Create(D3DXVECTOR3(1300.0f + ENEMY_SIZE_X * nCount, SCREEN_HEIGHT/2, 0.0f),
					ENEMY_SIZE, CEnemy::ENEMY_TYPE_NO_BULLET, CEnemy::ACTION_PATTERN_NOMRAL);

			}
		}
		if (m_nEnemyCounter == 500)
		{
			for (int nCount = 0; nCount < 6; nCount++)
			{
				for (int nCount2 = 0; nCount2 < 10; nCount2++)
				{
					CEnemy::Create(D3DXVECTOR3(1300.0f + ENEMY_SIZE_X * nCount2, 
						(SCREEN_HEIGHT / 2) - (ENEMY_SIZE_Y * 6) / 2+
						(ENEMY_SIZE_Y * nCount)+(ENEMY_SIZE_Y/2), 0.0f),
						ENEMY_SIZE, CEnemy::ENEMY_TYPE_NO_BULLET, CEnemy::ACTION_PATTERN_NOMRAL);
				}
			}
		}
		if (m_nEnemyCounter == 900)
		{

			CEnemy::Create(D3DXVECTOR3(1300.0f, 150.0f + (ENEMY_SIZE_Y / 1.5f)*0 , 0.0f),
				ENEMY_SIZE / 1.5f, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN);
			CEnemy::Create(D3DXVECTOR3(1300.0f, 150.0f + (ENEMY_SIZE_Y / 1.5f)*1, 0.0f),
				ENEMY_SIZE / 1.5f, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN,CEnemy::Dead_Process_LIFE);
			CEnemy::Create(D3DXVECTOR3(1300.0f, 150.0f + (ENEMY_SIZE_Y / 1.5f)*2, 0.0f),
				ENEMY_SIZE / 1.5f, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN);

		}
		if (m_nEnemyCounter == 1000)
		{	
			CEnemy::Create(D3DXVECTOR3(1300.0f, 600.0f + (ENEMY_SIZE_Y / 1.5f) * 0, 0.0f),
					ENEMY_SIZE / 1.5f, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN);
			CEnemy::Create(D3DXVECTOR3(1300.0f, 600.0f + (ENEMY_SIZE_Y / 1.5f) * 1, 0.0f),
				ENEMY_SIZE / 1.5f, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN, CEnemy::Dead_Process_SKILL);
			CEnemy::Create(D3DXVECTOR3(1300.0f, 600.0f + (ENEMY_SIZE_Y / 1.5f) * 2, 0.0f),
				ENEMY_SIZE / 1.5f, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN);
			
		}
	

		if (m_nEnemyCounter == 1500)
		{
			CEnemy::Create(D3DXVECTOR3(1300.0f, SCREEN_HEIGHT/2, 0.0f),
				ENEMY_SIZE, CEnemy::ENEMY_TYPE_HADE, CEnemy::ACTION_PATTERN_STOP, CEnemy::Dead_Process_BOSS);
		}
		if (m_nEnemyCounter == 1200)
		{
			CEnemy::Create(D3DXVECTOR3(1300.0f, SCREEN_HEIGHT/2, 0.0f),
				ENEMY_SIZE, CEnemy::ENEMY_TYPE_STOKER, CEnemy::ACTION_PATTERN_STALKER);
		}
		break;

	//ボス戦開始処理
	case GAME_STATE_BOSS_START:
		
		if (m_pPolygon != NULL)
		{
			//点滅処理の呼び出し
			m_pPolygon->Flashing();

			if (m_nStaeCounter >= 240)
			{
				m_pPolygon->Uninit();
				delete m_pPolygon;
				m_pPolygon = NULL;

				m_nStaeCounter = 0;
				SetGameState(GAME_STATE_BOSS_BATTLE);
			}

			//ステートカウントアップ
			m_nStaeCounter++;
		}
		break;

	case GAME_STATE_GAME_CLEAR:
	case GAME_STATE_GAME_OVER:
		if (m_nStaeCounter == 360)
		{
			CManager::GetFade()->SetFade(CManager::MODE_TYPE_RESULT);
			CManager::GetSound()->Stop();
		}
		//ステートカウントアップ
		m_nStaeCounter++;

		break;
	}
}

//=============================================================================
//ゲームクラスの描画処理
//=============================================================================
void CGame::Draw(void)
{
	if (m_pPolygon != NULL)
	{
		m_pPolygon->Draw();
	}
}

void CGame::SetGameState(const GAME_STATE state)
{
	m_state = state;

	switch (m_state)
	{
	case GAME_STATE_BOSS_START:
		if (m_pPolygon == NULL)
		{
			CManager::GetSound()->Stop();
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_WARNING);
			m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 
				D3DXVECTOR3(700.0f, 175.0f, 0.0f), CPolygon::TEX_TYPE_WARNING);
		}
		break;

	case GAME_STATE_BOSS_BATTLE:
		//ボスを生成
		CBoss::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE_X, SCREEN_HEIGHT / 2, 0.0f), ENEMY_SIZE * 3);
		//ボス戦用のBGMを再生
		CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_BOSS);
		break;

	case GAME_STATE_GAME_CLEAR:
		if (m_pPolygon == NULL)
		{
			CManager::GetSound()->Stop();
			//再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_CLEAR);
			m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
				D3DXVECTOR3(675.0f, 50.0f, 0.0f), CPolygon::TEX_TYPE_GAME_CLEAR);
		}
		break;

	case GAME_STATE_GAME_OVER:
		if (m_pPolygon == NULL)
		{
			CManager::GetSound()->Stop();
			//再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_OVER);
			m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
				D3DXVECTOR3(675.0f, 50.0f, 0.0f), CPolygon::TEX_TYPE_GAME_OVER);
		}
		break;
	default:
		break;
	}
}

CGame::GAME_STATE CGame::GetGameState(void)
{
	return m_state;
}

//=============================================================================
//UI情報の取得処理
//=============================================================================
CUi * CGame::GetUi(void)
{
	return m_pUi;
}

//=============================================================================
//プレイヤー情報の取得
//=============================================================================
CPlayer * CGame::GetPlayer(void)
{
	return m_pPlayer;
}
