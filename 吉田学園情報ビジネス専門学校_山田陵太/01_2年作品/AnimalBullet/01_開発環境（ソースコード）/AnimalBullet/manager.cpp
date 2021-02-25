//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "keyboard.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "sound.h"
#include "bg.h"
#include "number.h"
#include "polygon.h"
#include "life.h"
#include "title.h"
#include "result.h"
#include "effect.h"
#include "gauge.h"
#include "ui.h"
#include "game.h"
#include "fade.h"
#include "bome.h"
#include "item.h"
#include "boss.h"
#include "joystick.h"
#include "beam.h"
#include "shield.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
CManager::MODE_TYPE CManager::m_mode = CManager::MODE_TYPE_TITLE;	//ゲームモード判定用変数
CRenderer *CManager::m_pRenderer = NULL;	//レンダラークラスのポインタ変数
CInputKeyboard *CManager::m_pKeyBoard = NULL	;//入力処理クラスのポインタ変数
CSound *CManager::m_pSound = NULL;	//サウンドクラスのポインタ変数
CTitle *CManager::m_pTitle = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;
CFade *CManager::m_pFade = NULL;
CInputJoyStick *CManager::m_pJoyStick = NULL;

//=============================================================================
//コンストラクタ
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//レンダラークラスのインスタンス生成
	m_pRenderer = new CRenderer;

	//メモリが確保できたら
	if (m_pRenderer != NULL)
	{
		// 初期化処理
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{
			return -1;
		}
	}

	//入力処理クラスのインスタンス生成
	m_pKeyBoard = new CInputKeyboard;

	//メモリが確保できたら
	if (m_pKeyBoard != NULL)
	{
		if (FAILED(m_pKeyBoard->InIt(hInstance, hWnd)))
		{
			return -1;
		}
	}

	//ジョイスティックの作成
	m_pJoyStick = new CInputJoyStick;

	m_pJoyStick->InIt(hInstance, hWnd);


	//サウンドクラスのインスタンス生成
	m_pSound = new CSound;

	//メモリが確保できたら
	if (m_pSound != NULL)
	{
		if (FAILED(m_pSound->Init(hWnd)))
		{
			return -1;
		}
	}
	//全テクスチャの読み込み
	LoadAll();

	//タイトルクラスの生成
	m_pTitle = CTitle::Create();

	//フェードクラスの生成
	m_pFade = CFade::Create();

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//現在のモード情報を破棄
	switch (m_mode)
	{
		//タイトルモード
	case MODE_TYPE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();

			m_pTitle = NULL;
		}
		break;

		//ゲームモード
	case MODE_TYPE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();

			m_pGame = NULL;
		}
		break;

		//リザルトモード
	case MODE_TYPE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();

			m_pResult = NULL;
		}
		break;
	}
	//オブジェクトクラスのすべての終了処理呼び出し
	CScene::ReleaseAll();

	//全テクスチャの破棄
	UnLoadAll();

	if (m_pSound != NULL)
	{
		//サウンドクラスの終了処理呼び出し
		m_pKeyBoard->Uninit();

		//メモリの削除
		delete m_pSound;

		//メモリのクリア
		m_pSound = NULL;
	}

	if (m_pJoyStick != NULL)
	{
		//ジョイスティッククラスの終了処理呼び出し
		m_pJoyStick->Uninit();

		//メモリの削除
		delete m_pJoyStick;

		//メモリのクリア
		m_pJoyStick = NULL;
	}

	if (m_pKeyBoard != NULL)
	{
		//入力処理クラスの終了処理呼び出し
		m_pKeyBoard->Uninit();

		//メモリの削除
		delete m_pKeyBoard;

		//メモリのクリア
		m_pKeyBoard = NULL;
	}

	if (m_pFade != NULL)
	{
		//フェードクラスの終了処理呼び出し
		m_pFade->Uninit();

		//メモリの削除
		delete m_pFade;

		//メモリのクリア
		m_pFade = NULL;
	}

	if (m_pRenderer != NULL)
	{
		//レンダラークラスの終了処理呼び出し
		m_pRenderer->Uninit();

		//メモリの削除
		delete m_pRenderer;

		//メモリのクリア
		m_pRenderer = NULL;
	}
}

//=============================================================================
//更新処理
//=============================================================================
void CManager::Update(void)
{
	if (m_pKeyBoard != NULL)
	{
		//入力処理クラスの更新処理呼び出し
		m_pKeyBoard->Update();
	}

	if (m_pJoyStick != NULL)
	{
		//jジョイスティッククラスの更新処理
		m_pJoyStick->Update();
	}
	if (m_pRenderer != NULL)
	{
		//レンダラークラスの更新処理呼び出し
		m_pRenderer->Update();
	}
	
	if (m_pFade != NULL)
	{
		//フェードクラスの更新処理
		m_pFade->Update();
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		//レンダラークラスの描画処理呼び出し
		m_pRenderer->Draw();
	}
}

//=============================================================================
//テクスチャの全ロード処理
//=============================================================================
void CManager::LoadAll(void)
{
	//シールドクラスのテクスチャ読み込み
	CShield::Load();

	//ビームクラスのテクスチャ読み込み
	CBeam::Load();

	//ボスクラスのテクスチャ読み込み
	CBoss::Load();

	//アイテムクラスのテクスチャ読み込み
	CItem::Load();

	//ボムクラスのテクスチャ読み込み
	CBome::Load();

	//UIクラスのテクスチャ読み込み
	CUi::Load();

	//背景クラスのテクスチャ読み込み
	CBg::Load();

	//プレイヤークラスのテクスチャ読み込み
	CPlayer::Load();

	//エネミークラスのテクスチャ読み込み
	CEnemy::Load();

	//バレットクラスのテクスチャ読み込み
	CBullet::Load();

	//エフェクトクラスのテクスチャ読み込み
	CEffect::Load();

	//エクスプロージョンクラスのテクスチャ読み込み
	CExplosion::Load();

	//ナンバークラスのテクスチャ読み込み
	CNumber::Load();

	//ポリゴンクラスのテクスチャ読み込み
	CPolygon::Load();

	//ゲージクラスのテクスチャ読み込み
	CGauge::Load();
}

//=============================================================================
//テクスチャの全アンロード処理
//=============================================================================
void CManager::UnLoadAll(void)
{
	//ゲージクラスのテクスチャ破棄
	CGauge::UnLoad();

	//ポリゴンクラスのテクスチャ破棄
	CPolygon::UnLoad();

	//ナンバークラスのテクスチャ破棄
	CNumber::UnLoad();
	
	//エクスプロージョンクラスのテクスチャ破棄
	CExplosion::UnLoad();

	//エフェクトクラスのテクスチャ破棄
	CEffect::UnLoad();

	//バレットクラスのテクスチャ破棄
	CBullet::UnLoad();

	//エネミークラスのテクスチャ破棄
	CEnemy::UnLoad();

	//プレイヤークラスのテクスチャ破棄
	CPlayer::UnLoad();

	//背景クラスのテクスチャ破棄
	CBg::UnLoad();

	//UIクラスのテクスチャ破棄
	CUi::UnLoad();

	//ボムクラスのテクスチャ破棄
	CBome::UnLoad();

	//アイテムクラスのテクスチャ破棄
	CItem::UnLoad();

	//ボスクラスのテクスチャ破棄
	CBoss::UnLoad();

	//ビームクラスのテクスチャ破棄
	CBeam::UnLoad();

	//ゲージクラスのテクスチャ破棄
	CShield::UnLoad();
}
//=============================================================================
//モードの設定処理
//=============================================================================
void CManager::SetMode(MODE_TYPE mode)
{
	//現在のモード情報を破棄
	switch (m_mode)
	{
	//タイトルモード
	case MODE_TYPE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();

			m_pTitle = NULL;

			CScene::ReleaseAll();
		}
		break;

	//ゲームモード
	case MODE_TYPE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();

			m_pGame = NULL;

			CScene::ReleaseAll();
		}
		break;

	//リザルトモード
	case MODE_TYPE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();

			m_pResult = NULL;
			CScene::ReleaseAll();
		}
		break;

	}

	//モードをセット
	m_mode = mode;
	
	//セットされたモードに対応した情報を生成する
	switch (m_mode)
	{
	//タイトルモード
	case MODE_TYPE_TITLE:
		//タイトルクラスの生成
		m_pTitle = CTitle::Create();
		break;

	//ゲームモード
	case MODE_TYPE_GAME:
		m_pGame = CGame::Create();
		break;

	//リザルトモード
	case MODE_TYPE_RESULT:
		//リザルトクラスの生成
		m_pResult = CResult::Create();
		break;
	}
}

//=============================================================================
//ゲームモード情報の取得
//=============================================================================
CManager::MODE_TYPE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
//レンダラー情報取得
//=============================================================================
CRenderer * CManager::GetRenderer(void)
{
	return m_pRenderer;
}


//=============================================================================
//キーボード情報取得
//=============================================================================
CInputKeyboard * CManager::GetKeyborad(void)
{
	return m_pKeyBoard;
}

//=============================================================================
//サウンド情報取得
//=============================================================================
CSound * CManager::GetSound(void)
{
	return m_pSound;
}

//=============================================================================
//フェード情報取得
//=============================================================================
CFade * CManager::GetFade(void)
{
	return m_pFade;
}

//=============================================================================
//ジョイスティック情報取得
//=============================================================================
CInputJoyStick * CManager::GetJoyStick(void)
{
	return m_pJoyStick;
}
