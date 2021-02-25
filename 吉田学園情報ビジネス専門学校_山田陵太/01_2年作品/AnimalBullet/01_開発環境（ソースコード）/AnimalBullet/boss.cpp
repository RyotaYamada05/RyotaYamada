//=============================================================================
//
// ボス処理 [boss.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "boss.h"
#include "bullet.h"
#include "game.h"
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "enemy.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define STOP_POS D3DXVECTOR3(1100.0f,SCREEN_HEIGHT / 2 , 0.0f)	//停止位置
#define BOSS_SPEED 5.0f	//ボスの移動量
#define BOSS_LIFE 600	//ボスの体力
#define BOSS_SCORE 1000	//ボス撃破時に得られるスコア

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBoss::m_pTexture = NULL;

//=============================================================================
//ボスクラスのコンストラクタ
//=============================================================================
CBoss::CBoss(int nPriority) :CScene2D(nPriority)
{
	//各メンバ変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = BOSS_STATE_NORMAL;
	m_action = ACTION_PATTERN_STAERT;
	m_nLife = 0;
	m_nCount = 0;
	m_nScore = 0;
	m_nActionCounter = 0;
	m_nStateCounter = 0;
}

//=============================================================================
//ボスクラスのデストラクタ
//=============================================================================
CBoss::~CBoss()
{
}

//=============================================================================
//ボスクラスのクリエイト処理
//=============================================================================
CBoss * CBoss::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CBoss *pBoss = NULL;

	//インスタンス生成
	pBoss = new CBoss;

	if (pBoss != NULL)
	{
		//初期化処理
		pBoss->Init(pos, size);
	}
	return pBoss;
}

//=============================================================================
//ボスクラスのテクスチャロード処理
//=============================================================================
HRESULT CBoss::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/boss.png", &m_pTexture);
	return S_OK;
}

//=============================================================================
//ボスクラスのテクスチャ破棄処理
//=============================================================================
void CBoss::UnLoad(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//ボスクラスの初期化処理
//=============================================================================
HRESULT CBoss::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//位置の設定
	m_pos = pos;

	//サイズの設定
	m_size = size;

	//2Dポリゴンクラスの初期化
	CScene2D::Init(pos, size);

	//オブジェタイプの設定
	CScene2D::SetObjType(CScene2D::OBJTYPE_BOSS);

	//テクスチャの設定
	CScene2D::BindTexture(m_pTexture);

	//ライフの設定
	m_nLife = BOSS_LIFE;

	//スコアの設定
	m_nScore = BOSS_SCORE;

	//移動量の計算
	m_move = VectorMath(STOP_POS, BOSS_SPEED);

	return S_OK;
}

//=============================================================================
//ボスクラスの終了処理
//=============================================================================
void CBoss::Uninit(void)
{
	//爆発の清セリ
	CExplosion::Create(m_pos, EXPLOSION_SIZE * 8.0f, COLOR_GREEN);

	//2Dポリゴンクラスの終了処理呼び出し
	CScene2D::Uninit();
}

//=============================================================================
//ボスクラスの更新処理
//=============================================================================
void CBoss::Update(void)
{
	//死亡状態だった場合
	if (DecisionState() == true)
	{
		Uninit();
		return;
	}

	//死亡していないとき
	if (m_state != BOSS_STATE_DIED)
	{
		//位置の取得
		m_pos = CScene2D::GetPos();

		//行動処理呼び出し
		Action();

		//位置の設定
		CScene2D::SetPos(m_pos);
	}
}

//=============================================================================
//ボスクラスの描画処理
//=============================================================================
void CBoss::Draw(void)
{
	//2Dポリゴンクラスの描画処理呼び出し
	CScene2D::Draw();
}

//=============================================================================
//ボスクラスの行動処理
//=============================================================================
void CBoss::Action(void)
{
	D3DXVECTOR3 move;	//移動量

	switch (m_action)
	{
	///START状態だった場合
	case ACTION_PATTERN_STAERT:
		if (m_pos.x <= STOP_POS.x)
		{
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_action = ACTION_PATTERN_NOMRAL;
		}
		break;

	//通常パターン場合
	case ACTION_PATTERN_NOMRAL:
		
		if (m_nCount % 60 == 0)
		{
			move = VectorMath(CGame::GetPlayer()->GetPos(), BOSS_SPEED * 1.5f);

			//バレットの生成
			CBullet::Create(m_pos, move, BULLET_SIZE, COLOR_RED,
				CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_DIVISION, 10,40);
		}
		break;

	//加速弾発射の状態
	case ACTION_PATTERN_ACCELEU:
		if (m_nCount % 60 == 0)
		{
			move = VectorMath(CGame::GetPlayer()->GetPos(), 2.0f);

			//バレットの生成
			CBullet::Create(m_pos, move, BULLET_SIZE, COLOR_BLUE,
				CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_ACCELE, 10);

			CBullet::Create(D3DXVECTOR3(m_pos.x, m_pos.y + BULLET_SIZE_Y, m_pos.z), move, BULLET_SIZE, COLOR_BLUE,
				CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_ACCELE, 10);

			CBullet::Create(D3DXVECTOR3(m_pos.x, m_pos.y - BULLET_SIZE_Y, m_pos.z), move, BULLET_SIZE, COLOR_BLUE,
				CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_ACCELE, 10);
		}
		break;
	default:
		break;
	}

	//300フレーム経過すると状態を遷移
	if (m_nActionCounter == 300)
	{
		switch (m_action)
		{
		//通常状態の場合
		case ACTION_PATTERN_NOMRAL:

			//加速弾状態へ移行
			m_action = ACTION_PATTERN_ACCELEU;

			break;

		//加速弾状態の場合
		case ACTION_PATTERN_ACCELEU:

			////通常状態へ移行
			m_action = ACTION_PATTERN_NOMRAL;

			break;
		default:
			break;
		}

		//カウンターをクリア
		m_nActionCounter = 0;
	}

	if (m_action != ACTION_PATTERN_STAERT)
	{
		if (m_nCount % 6000 == 0)
		{
			CEnemy::Create(D3DXVECTOR3(1300.0f, 200.0f, 0.0f),
				ENEMY_SIZE, CEnemy::ENEMY_TYPE_STOKER, CEnemy::ACTION_PATTERN_STALKER, CEnemy::Dead_Process_SKILL);

			CEnemy::Create(D3DXVECTOR3(1300.0f, 600.0f, 0.0f),
				ENEMY_SIZE, CEnemy::ENEMY_TYPE_STOKER, CEnemy::ACTION_PATTERN_STALKER, CEnemy::Dead_Process_LIFE);

		}
		else if (m_nCount % 600 == 0)
		{
			CEnemy::Create(D3DXVECTOR3(1300.0f, 200.0f, 0.0f),
				ENEMY_SIZE, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN);

			CEnemy::Create(D3DXVECTOR3(1300.0f, 600.0f, 0.0f),
				ENEMY_SIZE, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN);
		}

		m_nCount++;
	}
	m_nActionCounter++;

	//位置に移動量を与える
	m_pos += m_move;
}

//=============================================================================
//ボスクラスのダメージ処理
//=============================================================================
void CBoss::Damage(const int nDamage)
{
	//ライフを減らす
	m_nLife -= nDamage;

	//ライフが0以下になったら
	if (m_nLife <= 0)
	{
		SetColor(COLOR_WHITE);
		m_state = BOSS_STATE_DIED;
	}
	else
	{
		m_state = BOSS_STATE_DAMAGE;
	}
}

//=============================================================================
//ボスクラスの状態処理
//=============================================================================
bool CBoss::DecisionState(void)
{
	switch (m_state)
	{
	case BOSS_STATE_NORMAL:
		break;

	//ダメージ状態
	case BOSS_STATE_DAMAGE:
		
		if (m_nStateCounter >= 30)
		{
			SetColor(COLOR_WHITE);

			//カウンターをクリア
			m_nStateCounter = 0;

			//通常状態へ戻す
			m_state = BOSS_STATE_NORMAL;
		}
		else if (m_nStateCounter % 10 <= 5)
		{
			SetColor(COLOR_ALPHA);
		}
		else
		{
			SetColor(COLOR_WHITE);
		}

		//ステートカウンターアップ
		m_nStateCounter++;
		break;

	//死亡状態
	case BOSS_STATE_DIED:
		if (m_nStateCounter >= 300)
		{
			CGame::SetGameState(CGame::GAME_STATE_GAME_CLEAR);
			return true;
		}
		else
		{
			//爆発
			if (m_nStateCounter % 20 == 0)
			{
				float X = (m_pos.x + m_size.x / 3) - (float)(rand() % (int)m_size.x / 1.5f);
				float Y = (m_pos.y + m_size.y / 3) - (float)(rand() % (int)m_size.y / 1.5f);

				D3DXVECTOR3 pos = D3DXVECTOR3(X, Y, 0.0f);
				CExplosion::Create(pos, EXPLOSION_SIZE * 2, COLOR_GREEN);
			}
		}
		
		//ステートカウンターアップ
		m_nStateCounter++;
		
		break;
	default:
		break;
	}
	
	return false;
}

//=============================================================================
//ボスクラスのベクトル計算処理
//=============================================================================
D3DXVECTOR3 CBoss::VectorMath(D3DXVECTOR3 TargetPos, float fSpeed)
{
	//2点間のベクトルを求める（終点[目標地点] - 始点[自身の位置]）
	D3DXVECTOR3 Vector = TargetPos - m_pos;

	//ベクトルの大きさを求める(c^2 = a^2 * b^2)
	float fVectorSize = D3DXVec3Length(&Vector);
	
	//単位ベクトル用変数
	D3DXVECTOR3 UnitVector;

	//単位ベクトルを求める(元のベクトル / ベクトルの大きさ)
	D3DXVec3Normalize(&UnitVector,&Vector);

	//単位ベクトルを速度倍して返す(VectorNor*fSpeed)
	return UnitVector * fSpeed;
}
