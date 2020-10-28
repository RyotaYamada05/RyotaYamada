//=============================================================================
//
// メイン処理 [title.h]
// Author : 山田陵太
//
//=============================================================================
#include "title.h"
#include "polygon.h"
#include "manager.h"
#include "keyboard.h"
#include "fade.h"
#include "sound.h"
#include "effect.h"
#include "joystick.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define LOGO_POS D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f)
#define LOGO_MOVE_SPEED 2.0f
#define EFFECT_SIZE_X 100.0f	//プレイヤーのXサイズ
#define EFFECT_SIZE_Y 100.0f	//プレイヤーのYサイズ
#define EFFECT_SIZE D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f)	//プレイヤーのサイズ
//=============================================================================
//タイトルクラスのコンストラクタ
//=============================================================================
CTitle::CTitle(int nPriority) :CScene(nPriority)
{
	memset(m_apPolygon, 0, sizeof(m_apPolygon));
	memset(m_apEffect, 0, sizeof(m_apEffect));
	m_EnterColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nCounter = 0;
	m_nCntEffecter = 0;
	m_state = TITLE_STATE_LOGO_MOVE;
	m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, -50.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
//タイトルクラスのデストラクタ
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
//タイトルクラスのクリエイト処理
//=============================================================================
CTitle * CTitle::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//タイトルクラスのポインタ変数
	CTitle *pTitle = NULL;

	//メモリの確保
	pTitle = new CTitle;

	//メモリが確保できていたら
	if (pTitle != NULL)
	{
		pTitle->Init(pos, size);
	}
	//メモリ確保に失敗したとき
	else
	{
		return NULL;
	}

	return pTitle;
}

//=============================================================================
//タイトルクラスの初期化処理
//=============================================================================
HRESULT CTitle::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//タイトル背景用ポリゴンの生成
	m_apPolygon[TITLE_TYPE_BG] = CPolygon::Create(pos, size, CPolygon::TEX_TYPE_TITLE);

	//タイトルロゴ用ポリゴンの生成
	m_apPolygon[TITLE_TYPE_LOGO] = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH/2,-50.0f,0.0f), D3DXVECTOR3(800.0f,200.0f,0.0f), CPolygon::TEX_TYPE_TITLE_LOGO);

	//ベクトルを求める
	D3DXVECTOR3 Vector = D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f) - m_pos;

	//ベクトルの大きさを求める
	float fVectorSize = sqrtf((Vector.x*Vector.x) + (Vector.y*Vector.y));

	//単位ベクトルを求める
	D3DXVECTOR3 VectorNor = D3DXVECTOR3(Vector.x / fVectorSize, Vector.y / fVectorSize, 0.0f);

	m_move = D3DXVECTOR3(VectorNor.x * LOGO_MOVE_SPEED, VectorNor.y * LOGO_MOVE_SPEED, 0.0f);

	return S_OK;
}

//=============================================================================
//タイトルクラスの終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	for (int nCount = 0; nCount < TITLE_TYPE_MAX; nCount++)
	{
		if (m_apPolygon[nCount] != NULL)
		{
			//ポリゴンクラスの終了処理呼び出し
			m_apPolygon[nCount]->Uninit();

			//メモリの削除
			delete m_apPolygon[nCount];

			//メモリのクリア
			m_apPolygon[nCount] = NULL;
		}
	}
	
	//オブジェクト破棄
	Release();
}

//=============================================================================
//タイトルクラスの更新
//=============================================================================
void CTitle::Update(void)
{
	//キーボードクラスの情報取得
	CInputKeyboard *pKeyBoard = CManager::GetKeyborad();

	//状態別の更新
	if (DecisionState() == true)
	{
		//フェード情報を取得
		CFade *pFade = CManager::GetFade();

		if (pFade != NULL)
		{
			//フェードを行う
			pFade->SetFade(CManager::MODE_TYPE_GAME);
			
			return;
		}
	}

	if (m_nCntEffecter % 120 == 0)
	{
		//チーター
		m_apEffect[0] = CEffect::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 400, 0.0f),
			EFFECT_SIZE, COLOR_YELLOW, 120, CEffect::EFFECT_TYPE_TITLE);
		m_apEffect[0]->SetType(CEffect::EFFECT_TYPE_CHEETAH);

		//ゴリラ
		m_apEffect[1] = CEffect::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 200.0f, 400, 0.0f),
			EFFECT_SIZE, COLOR_PURPLE, 120, CEffect::EFFECT_TYPE_TITLE);
		m_apEffect[1]->SetType(CEffect::EFFECT_TYPE_GORILLA);

		//カメ
		m_apEffect[2] = CEffect::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) + 200.0f, 400, 0.0f),
			EFFECT_SIZE, COLOR_GREEN, 120, CEffect::EFFECT_TYPE_TITLE);
		m_apEffect[2]->SetType(CEffect::EFFECT_TYPE_TURTLE);
	}
	m_nCntEffecter++;
	//生成ができたら
	if (m_apPolygon[TITLE_TYPE_ENTER] != NULL)
	{
		//ENTERキーが押されたら
		if (pKeyBoard->GetKeyBoardTrigger(DIK_RETURN) && m_state != TITLE_STATE_PUSH_ENTER || 
			CManager::GetJoyStick()->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_B))
		{
			//状態をTITLE_STATE_PUSH_ENTERへ変更
			m_state = TITLE_STATE_PUSH_ENTER;
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ENTER);
			//カウンタークリア
			m_nCounter = 0;
		}
		
		//カラーの設定
		m_apPolygon[TITLE_TYPE_ENTER]->SetColor(m_EnterColor);

		//カウントアップ
		m_nCounter++;
	}
}

//=============================================================================
//タイトルクラスの描画処理
//=============================================================================
void CTitle::Draw(void)
{
	for (int nCount = 0; nCount < TITLE_TYPE_MAX; nCount++)
	{
		if (m_apPolygon[nCount] != NULL)
		{
			//ポリゴンクラスの描画処理呼び出し
			m_apPolygon[nCount]->Draw();
		}
	}
}

//=============================================================================
//タイトルクラスの状態別更新
//=============================================================================
bool CTitle::DecisionState(void)
{
	switch (m_state)
	{
	case TITLE_STATE_LOGO_MOVE:
		//キーボードクラスの情報取得
		if (CManager::GetKeyborad()->GetKeyBoardTrigger(DIK_RETURN) || CManager::GetJoyStick()->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_B))
		{
			m_pos = LOGO_POS;

			//位置の設定
			m_apPolygon[TITLE_TYPE_LOGO]->SetPos(m_pos);
		}
		
		if (m_pos.y <= 200.0f)
		{
			//位置の取得
			m_pos = m_apPolygon[TITLE_TYPE_LOGO]->GetPos();

			//位置に移動量を与える
			m_pos += m_move;

			//位置の設定
			m_apPolygon[TITLE_TYPE_LOGO]->SetPos(m_pos);
		}
		else
		{
			if (m_apPolygon[TITLE_TYPE_ENTER] == NULL)
			{
				//PRESS ENTER用ポリゴンの生成
				m_apPolygon[TITLE_TYPE_ENTER] = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200.0f, 0.0f),
					D3DXVECTOR3(600.0f, 100.0f, 0.0f), CPolygon::TEX_TYPE_PRESSENTER);

				//待機状態へ移行
				m_state = TITLE_STATE_WAIT;
			}
		}
		break;

	case TITLE_STATE_WAIT:
		if (m_nCounter % 60 == 0)
		{
			if (m_EnterColor.a == 1.0f)
			{
				m_EnterColor.a = 0.0f;
			}
			else
			{
				m_EnterColor.a = 1.0f;
			}
		}
		break;

	case TITLE_STATE_PUSH_ENTER:
		if (m_nCounter % 5 == 0)
		{
			if (m_EnterColor.a == 1.0f)
			{
				m_EnterColor.a = 0.0f;
			}
			else
			{
				m_EnterColor.a = 1.0f;
			}
		}

		if (m_nCounter % 60 == 0)
		{
			m_nCounter += 1;
			return true;
		}
		break;
	}

	return false;
}
