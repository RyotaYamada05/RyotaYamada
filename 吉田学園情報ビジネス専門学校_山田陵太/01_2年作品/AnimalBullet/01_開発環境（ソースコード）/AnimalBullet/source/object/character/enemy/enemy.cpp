//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : 山田陵太
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "enemy.h"
#include "enemynomral.h"
#include "enemysniping.h"
#include "enemyhoming.h"
#include "middleboss.h"

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
#define CONTACT_DAMAGE 5	//プレイヤーとの接触時に与えるダメージ

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[ENEMY_TYPE_MAX] = {};

//=============================================================================
//エネミークラスのコンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority):CCharacter(nPriority)
{
	//各メンバ変数のクリア
	m_pos = VECTOR3_ZERO_INITIALIZE;
	m_move = VECTOR3_ZERO_INITIALIZE;
	m_nCounter = 0;
	m_nScore = 0;
	m_nActionCounter = 0;
	m_DeathAction = DEATH_ACTION_NOEN;
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
CEnemy * CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const ENEMY_TYPE type, const int nLife, const int nScore,
	const ACTION_PATTERN action, DEATH_ACTION DeathAction)
{
	//エネミクラスのポインタ変数
	CEnemy *pEnemy = NULL;

	//種類別にインスタンス生成
	switch (type)
	{
	case ENEMY_TYPE_NO_BULLET:
		pEnemy = new CEnemy;
		break;

	case ENEMY_TYPE_NORMAL:
		pEnemy = new CEnemyNomral;
		break;

	case ENEMY_TYPE_SNIPING:
		pEnemy = new CEnemySniping;
		break;

	case ENEMY_TYPE_HOMING:
		pEnemy = new CEnemyHoming;
		break;

	case ENEMY_TYPE_MIDDLEBOSS:
		pEnemy = new CMiddleBoss;
		break;

	default:
		break;
	}
	
	if (pEnemy)
	{
		pEnemy->SetPos(pos);
		pEnemy->SetSize(size);
		pEnemy->InitLife(nLife);
		pEnemy->m_nScore = nScore;
		pEnemy->m_type = type;
		pEnemy->m_action = action;
		pEnemy->m_DeathAction = DeathAction;
		//初期化処理呼び出し
		pEnemy->Init();
	}
	else
	{
		return NULL;
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
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/Enemy_Cheetah.png", &m_pTexture[ENEMY_TYPE_NO_BULLET]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/Enemy_Gorilla.png", &m_pTexture[ENEMY_TYPE_NORMAL]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/Enemy_Cheetah.png", &m_pTexture[ENEMY_TYPE_SNIPING]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/Enemy_Gorilla.png", &m_pTexture[ENEMY_TYPE_MIDDLEBOSS]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/Enemy_Turtle.png", &m_pTexture[ENEMY_TYPE_HOMING]);

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
		if (m_pTexture[nCount])
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//エネミークラスの初期化処理
//=============================================================================
HRESULT CEnemy::Init(void)
{
	//2Dポリゴンクラスの初期化処理呼び出し
	CScene2D::Init();

	//テクスチャの割り当て
	CScene2D::BindTexture(m_pTexture[m_type]);

	//オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_ENEMY);

	//ポリゴン位置の取得
	m_pos = GetPos();

	//移動量の設定
	m_move = ENEMY_MOVE;

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
	//CScene2Dの更新処理
	CScene2D::Update();

	//状態判定処理呼び出し
	if (STATE_DEATH(CheckState()))
	{
		return;
	}

	//ポリゴン位置の取得
	m_pos = CScene2D::GetPos();

	//行動パターン別の更新処理
	ActionUpdate();

	//攻撃処理
	Attack();

	//サイズの取得
	D3DXVECTOR3 size = GetSize();

	//プレイヤーと当たったかどうかの情報を取得
	CPlayer *pPlayer = (CPlayer*)CScene2D::JudgeCollision(CScene::OBJTYPE_PLAYER, m_pos, size);
	
	if (pPlayer)
	{
		//接触ダメージを与える
		pPlayer->Damage(CONTACT_DAMAGE);
	}

	//位置を設定する
	CScene2D::SetPos(m_pos);

	//画面外へ出たとき削除する
	if (m_pos.x <= -100 || m_pos.y <= 0 || m_pos.y >= SCREEN_HEIGHT + 100)
	{
		if (CGame::GetGameState() != CGame::GAME_STATE_GAME_OVER && 
			CGame::GetGameState() != CGame::GAME_STATE_GAME_CLEAR)
		{
			if (m_DeathAction == DEATH_ACTION_BOSS)
			{
				CGame::SetGameState(CGame::GAME_STATE_BOSS_START);
			}
		}

		//終了処理呼び出し
		Uninit();

		return;
	}
}

//=============================================================================
//エネミークラスの描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	//2Dポリゴンクラスの描画処理呼び出し
	CScene2D::Draw();
}

//=============================================================================
//エネミークラスの攻撃処理
//=============================================================================
void CEnemy::Attack(void)
{
	//ENEMY_TYPE_NO_BULLETの場合は攻撃を行わない
	return;
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
		if (m_nActionCounter == 600)
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
//エネミークラスのダメージ処理
//=============================================================================
void CEnemy::Damage(const int nDamage)
{
	//体力をダメージ分減らす
	int nNowLife = SubLife(nDamage);

	//体力が0より小さい時エネミーは死亡
	if (nNowLife <= 0)
	{
		SetState(CHARACTER_STATE_DIED);

	}
	//体力が0より大きいとき
	else
	{
		//ダメージ状態へ移行する
		SetState(CHARACTER_STATE_DAMAGE);
	}
}

//=============================================================================
//エネミークラスの死亡処理
//=============================================================================
void CEnemy::DiedProcess(void)
{
	//スコア情報の取得
	CScore *pScore = CGame::GetUi()->GetScore();

	//スコアの加算処理呼び出し
	pScore->AddScore(m_nScore);

	//爆発生成
	CExplosion::Create(m_pos, D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f), COLOR_RED);

	if (CGame::GetGameState() != CGame::GAME_STATE_GAME_OVER && 
		CGame::GetGameState() != CGame::GAME_STATE_GAME_CLEAR)
	{
		//死亡時の処理を実行
		switch (m_DeathAction)
		{
		case DEATH_ACTION_NOEN:
			break;

		case DEATH_ACTION_LIFE:
			//体力回復アイテムの生成
			CItem::Create(m_pos, ITEM_SIZE, CItem::ITEM_TYPE_LIFE);
			break;

		case DEATH_ACTION_SKILL:
			//スキル回復アイテムの生成
			CItem::Create(m_pos, ITEM_SIZE, CItem::ITEM_TYPE_SKILL);
			break;

		case DEATH_ACTION_BOSS:
			CGame::SetGameState(CGame::GAME_STATE_BOSS_START);
			break;
		}
	}
	//終了処理呼び出し
	Uninit();
}