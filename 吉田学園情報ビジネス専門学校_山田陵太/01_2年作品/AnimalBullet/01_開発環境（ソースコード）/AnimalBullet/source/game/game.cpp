//=============================================================================
//
// ゲーム処理処理 [game.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "game.h"
#include "ui.h"
#include "player.h"
#include "bg.h"
#include "polygon.h"
#include "scene2d.h"
#include "boss.h"
#include "fade.h"
#include "manager.h"
#include "sound.h"
#include "enemy_control.h"

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
	m_pEnemyControl = NULL;
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
CGame * CGame::Create(void)
{
	//ゲームクラスのポインタ変数
	CGame *pGame = NULL;

	//メモリの確保
	pGame = new CGame;

	//メモリが確保できていたら
	if (pGame)
	{
		//初期化処理呼び出し
		pGame->Init();
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
HRESULT CGame::Init(void)
{
	m_state = CGame::GAME_STATE_NOMRAL;
	//SetGameState(CGame::GAME_STATE_BOSS_START);

	// 背景クラスの生成
	CBg::Create();

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(CHEETAH_SIZE_X / 2, SCREEN_HEIGHT / 2, 0.0f), CHEETAH_SIZE);

	//UIクラスの生成
	m_pUi = CUi::Create();

	//BGMの再生
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME);
	
	//エネミーコントロールクラスのインスタンス生成
	m_pEnemyControl = new CEnemyControl;

	if (m_pEnemyControl)
	{
		//エネミーコントロールクラスの初期化
		m_pEnemyControl->Init();
	}

	return S_OK;
}

//=============================================================================
//ゲームクラスの終了処理
//=============================================================================
void CGame::Uninit(void)
{
	if (m_pEnemyControl)
	{
		//エネミーコントロールクラスの終了処理
		m_pEnemyControl->Uninit();

		//メモリの削除
		delete m_pEnemyControl;

		//メモリのクリア
		m_pEnemyControl = NULL;
	}
	if (m_pPolygon)
	{
		//ポリゴンクラスの終了処理
		m_pPolygon->Uninit();

		//メモリの破棄
		delete m_pPolygon;

		//メモリのクリア
		m_pPolygon = NULL;
	}

	if (m_pUi)
	{
		//UIクラスの終了処理
		m_pUi->Uninit();

		//メモリの破棄
		delete m_pUi;

		//メモリのクリア
		m_pUi = NULL;
	}

	//オブジェクトの破棄
	SetDeathFlag();
}

//=============================================================================
//ゲームクラスの更新処理
//=============================================================================
void CGame::Update(void)
{
	if (m_pUi)
	{
		//UIクラスの更新
		m_pUi->Update();
	}
	if (m_pEnemyControl)
	{
		//エネミーコントロールクラスの更新
		m_pEnemyControl->Update();
	}
	switch (m_state)
	{
	case GAME_STATE_NOMRAL:
		
		break;

	//ボス戦開始処理
	case GAME_STATE_BOSS_START:
		
		if (m_pPolygon)
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
	if (m_pPolygon)
	{
		m_pPolygon->Draw();
	}
	if (m_pUi)
	{
		m_pUi->Draw();
	}
}

//=============================================================================
//ゲームクラスの状態設定処理
//=============================================================================
void CGame::SetGameState(const GAME_STATE state)
{
	m_state = state;

	switch (m_state)
	{
	//ボス戦開始前処理
	case GAME_STATE_BOSS_START:
		if (!m_pPolygon)
		{
			//BGMを停止し警告音を再生
			CManager::GetSound()->Stop();
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_WARNING);

			//警告メッセージの表示
			m_pPolygon = CPolygon::Create(SCREEN_CNTER_POS,
				D3DXVECTOR3(700.0f, 175.0f, 0.0f), CPolygon::TEX_TYPE_WARNING);
		}
		break;

	//ボス戦
	case GAME_STATE_BOSS_BATTLE:
		//ボスを生成
		CBoss::Create(
			D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE_X, SCREEN_HEIGHT / 2, 0.0f),
			BOSS_SIZE,
			BOSS_LIFE,
			BOSS_SCORE);

		//ボス戦用のBGMを再生
		CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_BOSS);
		break;

	//ゲームクリア
	case GAME_STATE_GAME_CLEAR:
		if (!m_pPolygon)
		{
			//BGM停止
			CManager::GetSound()->Stop();

			//勝利音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_CLEAR);

			//勝利表示
			m_pPolygon = CPolygon::Create(SCREEN_CNTER_POS,
				D3DXVECTOR3(675.0f, 50.0f, 0.0f), CPolygon::TEX_TYPE_GAME_CLEAR);
		}
		break;

	//ゲームオーバー
	case GAME_STATE_GAME_OVER:
		if (!m_pPolygon)
		{
			//BGMの再生
			CManager::GetSound()->Stop();
			
			//敗北音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_OVER);
		
			//敗北表示
			m_pPolygon = CPolygon::Create(SCREEN_CNTER_POS,
				D3DXVECTOR3(675.0f, 50.0f, 0.0f), CPolygon::TEX_TYPE_GAME_OVER);
		}
		break;
	default:
		break;
	}
}

//=============================================================================
//ゲームクラスの状態取得処理
//=============================================================================
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
