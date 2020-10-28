//=============================================================================
//
// メイン処理 [enemy.cpp]
// Author : 山田陵太
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "ui.h"
#include "player.h"
#include "game.h"
#include "item.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define ATK_NORMAL 1

#define LIFE_NO_BULLET 2	//弾を撃たないタイプの体力
#define LIFE_NORMAL 10	//普通の敵の体力
#define LIFE_SNIPING 20	//狙い撃ちタイプ体力
#define LIFE_HOMING 20	//ホーミングタイプの体力
#define LIFE_HADE 50	//ハードタイプの体力
#define LIFE_DIVISION 30	//分裂弾タイプの体力

#define SCORE_NO_BULLET 100	//弾を撃たないタイプのスコア
#define SCORE_NORMAL 200	//普通の敵のスコア
#define SCORE_SNIPING 300	//狙い撃ちタイプスコア
#define SCORE_HOMING 300	//ホーミングタイプのスコア
#define SCORE_HADE 300	//ハードタイプのスコア
#define SCORE_DIVISION 300	//分裂弾タイプのスコア

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[CEnemy::ENEMY_TYPE_MAX] = {};

//=============================================================================
//エネミークラスのコンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority):CScene2D(nPriority)
{
	//各メンバ変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCounter = 0;
	m_nLife = 0;
	m_nScore = 0;
	m_State = ENEMY_STATE_NORMAL;
	m_nStateCounter = 0;
	m_nActionCounter = 0;
	m_DeadProcess = Dead_Process_NOEN;
}

//=============================================================================
//エネミークラスのデストラクタ
//=============================================================================
CEnemy::~CEnemy()
{
}

//=============================================================================
//エネミークラスのクリエイト処理
//=============================================================================
CEnemy * CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const ENEMY_TYPE type,
	const ACTION_PATTERN action, Dead_Process DeadProcess)
{
	//エネミクラスのポインタ変数
	CEnemy *pEnemy = NULL;

	//インスタンス生成
	pEnemy = new CEnemy;

	if (pEnemy != NULL)
	{
		//初期化処理呼び出し
		pEnemy->Init(pos, size, type, action, DeadProcess);
	}
	return pEnemy;
}

//=============================================================================
//エネミークラスのテクスチャロード処理
//=============================================================================
HRESULT CEnemy::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy.png", &m_pTexture[ENEMY_TYPE_NO_BULLET]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy.png", &m_pTexture[ENEMY_TYPE_NORMAL]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy.png", &m_pTexture[ENEMY_TYPE_SNIPING]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy001.png", &m_pTexture[ENEMY_TYPE_HADE]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy001.png", &m_pTexture[ENEMY_TYPE_DIVISION]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy001.png", &m_pTexture[ENEMY_TYPE_STOKER]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy003.png", &m_pTexture[ENEMY_TYPE_HOMING]);
	
	return S_OK;
}

//=============================================================================
//エネミークラスのテクスチャ破棄処理
//=============================================================================
void CEnemy::UnLoad(void)
{
	//テクスチャの破棄
	for (int nCount = 0; nCount < ENEMY_TYPE_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//エネミークラスの初期化処理
//=============================================================================
HRESULT CEnemy::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const ENEMY_TYPE type,
	const ACTION_PATTERN action, const Dead_Process DeadProcess)
{
	//2Dポリゴンクラスの初期化処理呼び出し
	CScene2D::Init(pos, size);

	//サイズの設定
	m_size = size;

	m_move.x = -ENEMY_MOVE_X;

	//種類の設定
	m_type = type;

	//テクスチャの割り当て
	CScene2D::BindTexture(m_pTexture[m_type]);

	//オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_ENEMY);

	m_move.y = 3.0f;

	//行動パターンの設定
	m_action = action;

	//死亡時の処理を設定
	m_DeadProcess = DeadProcess;

	//ライフの設定
	SetData();
	
	//ポリゴン位置の取得
	m_pos = CScene2D::GetPos();
	return S_OK;
}

//=============================================================================
//エネミークラスの終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	//2Dポリゴンクラスの終了処理呼び出し
	CScene2D::Uninit();
}

//=============================================================================
//エネミークラスの更新処理
//=============================================================================
void CEnemy::Update(void)
{
	//状態判定処理
	if (DecisionState() == true)
	{
		return;
	}

	//ポリゴン位置の取得
	m_pos = CScene2D::GetPos();

	//行動パターン別の更新処理
	ActionUpdate();

	//専用の更新処理の呼び出し
	switch (m_type)
	{
	case ENEMY_TYPE_NO_BULLET:
		break;
	case ENEMY_TYPE_NORMAL:
		NormalUpdate();
		break;

	case ENEMY_TYPE_SNIPING:
		SnipingUpdate();
		break;

	case ENEMY_TYPE_HADE:
		HadeUpdate();
		break;
	}

	//プレイヤーと当たったかどうかの情報を取得
	CPlayer *pPlayer = (CPlayer*)CScene2D::Collision(CScene2D::OBJTYPE_PLAYER, m_pos, m_size);
	if (pPlayer != NULL)
	{
		pPlayer->Damage(10);
	}

	//位置を設定する
	CScene2D::SetPos(m_pos);

	//画面外へ出たとき削除する
	if (m_pos.x <= -100 || m_pos.y <= 0 || m_pos.y >= SCREEN_HEIGHT + 100)
	{
		if (CGame::GetGameState() != CGame::GAME_STATE_GAME_OVER &&  CGame::GetGameState() != CGame::GAME_STATE_GAME_CLEAR)
		{
			if (m_DeadProcess == Dead_Process_BOSS)
			{
				CGame::SetGameState(CGame::GAME_STATE_BOSS_START);
			}
		}
		//終了処理呼び出し
		Uninit();

		return;
	}
}

void CEnemy::NormalUpdate(void)
{
	//プレイヤーの位置情報を取得
	D3DXVECTOR3 Playerpos = CGame::GetPlayer()->GetPos();

	m_nCounter++;

	if (m_nCounter % 120 == 0 && m_pos.x >= Playerpos.x)
	{
		D3DXVECTOR3 move = D3DXVECTOR3(-10.0f,0.0f,0.0f);

		//バレットの生成
		CBullet::Create(m_pos, move, BULLET_SIZE, COLOR_RED,
			CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_NORMAL, ATK_NORMAL);
	}
}

void CEnemy::SnipingUpdate(void)
{
	//プレイヤーの位置情報を取得
	D3DXVECTOR3 Playerpos = CGame::GetPlayer()->GetPos();

	m_nCounter++;

	if (m_nCounter % 80 == 0 && m_pos.x >= Playerpos.x)
	{
		//角度θの計算（ラジアン）
		float fAngle = atan2f((Playerpos.y - m_pos.y), (Playerpos.x - m_pos.x));

		//移動量を求める
		D3DXVECTOR3 move = D3DXVECTOR3(cosf(fAngle) * 10, sinf(fAngle) * 10, 0.0f);

		//バレットの生成
		CBullet::Create(m_pos, move, BULLET_SIZE, COLOR_RED,
			CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_NORMAL, ATK_NORMAL);
	}
}

void CEnemy::HadeUpdate(void)
{
	if(m_pos.x <= 1000)
	{
		int nNum = 10;	//弾の数
		float fAngle = 15.0f;	//弾と弾の間の角度
		float fRad_Step = D3DXToRadian(fAngle);	//ラジアンに変換

		//端の弾と中心の弾との間の角度を計算(偶数・奇数によって結果変動)
		float fRad = nNum % 2 ? -nNum / 2 * fRad_Step : (-nNum / 2 + 0.5f) * fRad_Step;

		m_nCounter++;

		if (m_nCounter % 120 == 0)
		{
			for (int nCount = 0; nCount < nNum; nCount++, fRad += fRad_Step)
			{
				CBullet::Create(m_pos, D3DXVECTOR3(-cosf(fRad) * 8,
					sinf(fRad) * 8, 0.0f), BULLET_SIZE,
					COLOR_BLUE, CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_NORMAL, 5);
			}
		}

		nNum = 9;

		//端の弾と中心の弾との間の角度を計算(偶数・奇数によって結果変動)
		fRad = nNum % 2 ? -nNum / 2 * fRad_Step : (-nNum / 2 + 0.5f) * fRad_Step;
		if (m_nCounter % 140 == 0)
		{
			for (int nCount = 0; nCount < nNum; nCount++, fRad += fRad_Step)
			{
				CBullet::Create(m_pos, D3DXVECTOR3(-cosf(fRad) * 5,
					sinf(fRad) * 5, 0.0f), BULLET_SIZE,
					COLOR_GREEN, CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_NORMAL, 5);
			}
		}
	}
}

void CEnemy::DivisionUpdate(void)
{
}


//=============================================================================
//エネミークラスの行動パターン別の更新処理
//=============================================================================
void CEnemy::ActionUpdate(void)
{
	switch (m_action)
	{
	case ACTION_PATTERN_NOMRAL:
		break;

	//下へ移動する
	case ACTION_PATTERN_MOVE_DOWN:
		if (m_pos.x <= 800.0f)
		{
			//移動量を位置に設定
			m_move.y = 2.0f;

			m_pos.y += m_move.y;
		}
		break;

	//上へ移動する
	case ACTION_PATTERN_MOVE_UP:
		if (m_pos.x <= 800.0f)
		{
			//移動量を位置に加算
			m_move.y = -2.0f;

			m_pos.y += m_move.y;
		}
		break;

	//特定の位置で止まる
	case ACTION_PATTERN_STOP:
		if (m_pos.x <= 1000)
		{
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		break;

	//特定の位置で止まり、一定時間後退却
	case ACTION_PATTERN_STOP_RETURN:
		if (m_pos.x <= 1000)
		{
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		if (m_nActionCounter == 300)
		{
			m_move = D3DXVECTOR3(ENEMY_MOVE_X, 0.0f, 0.0f);
		}
		m_nActionCounter++;
		break;


	//上下に移動する
	case ACTION_PATTERN_UP_AND_DOWN:
		if (m_pos.x <= 1000)
		{
			m_move.x = 0.0f;
			m_pos.y += m_move.y;
			if (m_pos.y >= 700 || m_pos.y <= 100)
			{
				m_move.y *= -1;
			}
		}
		break;

	//追跡タイプ
	case ACTION_PATTERN_STALKER:
		//プレイヤーの位置情報を取得
		D3DXVECTOR3 Playerpos = CGame::GetPlayer()->GetPos();
		//角度θの計算（ラジアン）
		float fAngle = atan2f((Playerpos.y - m_pos.y), (Playerpos.x - m_pos.x));

		//角度をもとに移動量を求める
		D3DXVECTOR3 TargettoMove = D3DXVECTOR3(cosf(fAngle), sinf(fAngle), 0.0f);

		//求めた移動量を正規化する
		D3DXVec3Normalize(&TargettoMove, &TargettoMove);

		//移動量に設定
		m_move = TargettoMove * 5;
		m_pos.y += m_move.y;
		break;
	}

	//移動量を位置に加算
	m_pos.x += m_move.x;
}

//=============================================================================
//エネミークラスの描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	//2Dポリゴンクラスの描画処理呼び出し
	CScene2D::Draw();
}

void CEnemy::SetData(void)
{
	switch (m_type)
	{
	case ENEMY_TYPE_NO_BULLET:
		m_nLife = LIFE_NO_BULLET;
		m_nScore = SCORE_NO_BULLET;
		break;
	case ENEMY_TYPE_NORMAL:
		m_nLife = LIFE_NORMAL;
		m_nScore = SCORE_NORMAL;
		break;
	case ENEMY_TYPE_SNIPING:
		m_nLife = LIFE_SNIPING;
		m_nScore = SCORE_SNIPING;
		break;
	case ENEMY_TYPE_HOMING:
		m_nLife = LIFE_HOMING;
		m_nScore = SCORE_HOMING;
		break;
	case ENEMY_TYPE_HADE:
		m_nLife = LIFE_HADE;
		m_nScore = SCORE_HADE;
		break;
	case ENEMY_TYPE_DIVISION:
		m_nLife = LIFE_DIVISION;
		m_nScore = SCORE_DIVISION;
		break;

	case ENEMY_TYPE_STOKER:
		m_nLife = 10;
		m_nScore = 200;

	default:
		break;
	}
}

//=============================================================================
//エネミークラスのダメージ処理
//=============================================================================
void CEnemy::Damage(const int nDamage)
{
	//体力をダメージ分減らす
	m_nLife -= nDamage;

	//体力が0より小さい時エネミーは死亡
	if (m_nLife <= 0)
	{
		m_State = ENEMY_STATE_DIED;
		
	}
	//体力が0より大きいとき
	else
	{
		//ダメージ状態へ移行する
		m_State = ENEMY_STATE_DAMAGE;
	}
}

//=============================================================================
//エネミークラスの状態判定処理
//=============================================================================
bool CEnemy::DecisionState(void)
{
	switch (m_State)
	{
		//通常状態
	case ENEMY_STATE_NORMAL:
		break;

		//ダメージを受けた状態
	case ENEMY_STATE_DAMAGE:
		SetColor(COLOR_ALPHA);

		m_nStateCounter++;

		if (m_nStateCounter >= 30)
		{
			SetColor(COLOR_WHITE);
			m_nStateCounter = 0;

			m_State = ENEMY_STATE_NORMAL;
		}
		else if (m_nStateCounter % 10 <= 5)
		{
			SetColor(COLOR_ALPHA);
		}
		else
		{
			SetColor(COLOR_WHITE);
		}
		break;

		//死んでいる状態
	case ENEMY_STATE_DIED:
		
		CScore *pScore = CGame::GetUi()->GetScore();

		//スコアの加算処理呼び出し
		pScore->AddScore(m_nScore);

		//爆発生成
		CExplosion::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), D3DXVECTOR3(EXPLOSION_SIZE_X * 2, EXPLOSION_SIZE_Y * 2, 0.0f), COLOR_RED);

		if (CGame::GetGameState() != CGame::GAME_STATE_GAME_OVER &&  CGame::GetGameState() != CGame::GAME_STATE_GAME_CLEAR)
		{
			//死亡時の処理を実行
			switch (m_DeadProcess)
			{
			case Dead_Process_NOEN:
				break;

			case Dead_Process_LIFE:
				//体力回復アイテムの生成
				CItem::Create(m_pos, ITEM_SIZE, CItem::ITEM_TYPE_LIFE);
				break;

			case Dead_Process_SKILL:
				//スキル回復アイテムの生成
				CItem::Create(m_pos, ITEM_SIZE, CItem::ITEM_TYPE_SKILL);
				break;

			case Dead_Process_BOSS:
				CGame::SetGameState(CGame::GAME_STATE_BOSS_START);
				break;
			}
		}
		//終了処理呼び出し
		Uninit();

		return true;
		break;
	}

	return false;
}

//=============================================================================
//位置情報の取得
//=============================================================================
D3DXVECTOR3 CEnemy::GetPos(void) const
{
	return m_pos;
}
