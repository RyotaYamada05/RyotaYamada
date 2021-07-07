//=============================================================================
//
// ボス処理 [boss.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "boss.h"
#include "manager.h"
#include "renderer.h"

#include "bullet.h"
#include "game.h"
#include "player.h"
#include "explosion.h"
#include "score.h"
#include "ui.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define ACCELEU_BULLET_NUM 3						//一度に生成する加速弾の数
#define ACELEU_ATK 10								//加速弾の攻撃力
#define ACELEU_SPEED 1.0f							//加速弾の速度
#define DIVISION_SPEED 7.5f							//分裂弾の速度
#define DIVISION_ATK 10								//分裂弾の攻撃力
#define DIVISION_LIFE 40							//分裂弾の体力
#define STOP_BOS_POSITION 1000.0f					//ボスの停止位置
#define DIED_GAME_CHANGE_FRAEM 300					//ボスが死亡時、ゲーム状態の変更を実行するフレーム数
#define CHANGE_ATK_FRAEM 300						//攻撃のパターンの変更フレーム数
#define BOSS_MOVE D3DXVECTOR3(-2.0f, 0.0f, 0.0f)	//移動量

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBoss::m_pTexture = NULL;

//加速弾の位置変更用定数
const float fAcceleValue[ACCELEU_BULLET_NUM] =
{
	BULLET_SIZE_Y ,0 ,-BULLET_SIZE_Y
};

//=============================================================================
//ボスクラスのコンストラクタ
//=============================================================================
CBoss::CBoss(int nPriority):CCharacter(nPriority)
{
	//各変数のクリア
	m_pos = VECTOR3_ZERO_INITIALIZE;
	m_move = VECTOR3_ZERO_INITIALIZE;
	m_nAtkCounter = 0;
	m_nChangeCounter = 0;
	m_nDiedCounter = 0;
	m_buttleState = BUTTLE_STATE_STRAT_UP;
	m_atkPattern = BOSS_ATTACK_PATTERN_NOMRAL;
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
CBoss * CBoss::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nLife, const int nScore)
{
	//ボスクラスのポインタ変数
	CBoss *pBoss = NULL;

	//インスタンス生成
	pBoss = new CBoss;

	if (pBoss)
	{
		//各情報の設定
		pBoss->SetPos(pos);
		pBoss->SetSize(size);
		pBoss->InitLife(nLife);
		pBoss->m_nScore = nScore;
		//初期化
		pBoss->Init();
	}
	else
	{
		return NULL;
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
	if (m_pTexture)
	{
		//テクスチャの解放
		m_pTexture->Release();

		//メモリのクリア
		m_pTexture = NULL;
	}
}

//=============================================================================
//ボスクラスの初期化処理
//=============================================================================
HRESULT CBoss::Init(void)
{
	//CScene2Dの初期化
	CScene2D::Init();

	//テクスチャの割り当て
	CScene2D::BindTexture(m_pTexture);

	//オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_BOSS);

	//移動量の設定
	m_move = BOSS_MOVE;

	return S_OK;
}

//=============================================================================
//ボスクラスの終了処理
//=============================================================================
void CBoss::Uninit(void)
{
	//CScene2Dの終了
	CScene2D::Uninit();
}

//=============================================================================
//ボスクラスの更新処理
//=============================================================================
void CBoss::Update(void)
{
	//CScene2Dの更新処理
	CScene2D::Update();

	//死亡しているかどうか判定
	if (STATE_DEATH(CheckState()))
	{
		return;
	}

	//位置の取得
	m_pos = GetPos();
	
	//戦闘状態に応じて処理を実行
	switch (m_buttleState)
	{
	//戦闘開始準備
	case BUTTLE_STATE_STRAT_UP:

		if (m_pos.x <= STOP_BOS_POSITION)
		{
			//移動量を初期化
			m_move = VECTOR3_ZERO_INITIALIZE;

			//戦闘状態へ移行
			m_buttleState = BUTTLE_STATE_ON_BUTTLE;
		}

		break;

	//戦闘中
	case BUTTLE_STATE_ON_BUTTLE:

		//攻撃処理
		Attack();

		break;

	default:
		break;
	}

	//移動量を加算
	m_pos += m_move;

	//位置の設定
	SetPos(m_pos);
}

//=============================================================================
//ボスクラスの描画処理
//=============================================================================
void CBoss::Draw(void)
{
	//CScene2Dの描画
	CScene2D::Draw();
}

//=============================================================================
//ボスクラスの死亡時処理
//=============================================================================
void CBoss::DiedProcess(void)
{
	//カウンターがゲーム状態変更フレームに到達したら
	if (m_nDiedCounter >= DIED_GAME_CHANGE_FRAEM)
	{
		//最後の大爆発の生成
		CExplosion::Create(
			m_pos,
			EXPLOSION_SIZE * 5,
			COLOR_GREEN);

		//スコア情報の取得
		CScore *pScore = CGame::GetUi()->GetScore();

		//スコアの加算処理呼び出し
		pScore->AddScore(m_nScore);

		//ゲーム状態をクリアに設定
		CGame::SetGameState(CGame::GAME_STATE_GAME_CLEAR);

		//終了処理
		Uninit();

		return;
	}
	else
	{
		if (m_nDiedCounter % 2 == 0)
		{
			//サイズの取得
			D3DXVECTOR3 size = GetSize();

			//ランダムな位置を計算
			float fXPos = m_pos.x + (-size.x / 2) + rand() % (int)size.x;
			float fYPos = m_pos.y + (-size.y / 2) + rand() % (int)size.y;

			D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, 0.0f);

			//爆発の生成
			CExplosion::Create(
				pos,
				EXPLOSION_SIZE,
				COLOR_GREEN);
		}
	}

	//死亡カウンターのアップ
	m_nDiedCounter++;
}

//=============================================================================
//ボスクラスのダメージ処理
//=============================================================================
void CBoss::Damage(const int nDamage)
{
	//ボスが戦闘準備状態以外の時
	if (m_buttleState != BUTTLE_STATE_STRAT_UP)
	{
		//体力をダメージ分減らす
		int nNowLife = SubLife(nDamage);

		//体力が0より小さい時ボスは死亡
		if (nNowLife <= 0)
		{
			//死亡状態にする
			SetState(CHARACTER_STATE_DIED);
		}
		//体力が0より大きいとき
		else
		{
			//ダメージ状態へ移行する
			SetState(CHARACTER_STATE_DAMAGE);
		}
	}
}

//=============================================================================
//ボスクラスの攻撃処理
//=============================================================================
void CBoss::Attack(void)
{
	D3DXVECTOR3 bulletMove = VECTOR3_ZERO_INITIALIZE;	//弾の移動量
	
	if (m_nAtkCounter % 60 == 0)
	{
		switch (m_atkPattern)
		{
		//通常攻撃
		case BOSS_ATTACK_PATTERN_NOMRAL:

			//ボスからプレイヤー方向へのベクトルの計算
			bulletMove = TargetWayVec3Calculation(CGame::GetPlayer()->GetPos(), DIVISION_SPEED);

			//バレットの生成
			CBullet::Create(
				m_pos, 
				bulletMove, 
				BULLET_SIZE, 
				COLOR_RED,
				CBullet::BULLET_USER_ENEMY, 
				CBullet::BULLET_TYPE_DIVISION, 
				DIVISION_ATK,
				DIVISION_LIFE);

			break;

		//加速弾攻撃
		case BOSS_ATTACK_PATTERN_ACCELEU_BULLET:

			//ボスからプレイヤー方向へのベクトルの計算
			bulletMove = TargetWayVec3Calculation(CGame::GetPlayer()->GetPos(), ACELEU_SPEED);

			//加速弾の生成
			for (int nCntAccele = 0; nCntAccele < ACCELEU_BULLET_NUM; nCntAccele++)
			{
				CBullet::Create(
					D3DXVECTOR3(m_pos.x, m_pos.y + fAcceleValue[nCntAccele], m_pos.z),
					bulletMove,
					BULLET_SIZE,
					COLOR_BLUE,
					CBullet::BULLET_USER_ENEMY,
					CBullet::BULLET_TYPE_ACCELE,
					ACELEU_ATK);
			}
			break;

		default:
			break;
		}
	}

	//攻撃カウンターアップ
	m_nAtkCounter++;

	//攻撃パターンの変更処理
	ChangeAttackPattern();
}

//=============================================================================
//ボスクラスの攻撃パターン変更処理
//=============================================================================
void CBoss::ChangeAttackPattern(void)
{
	switch (m_atkPattern)
	{
	//通常攻撃
	case BOSS_ATTACK_PATTERN_NOMRAL:
		if (m_nChangeCounter == CHANGE_ATK_FRAEM)
		{
			//加速弾攻撃に切り替え
			m_atkPattern = BOSS_ATTACK_PATTERN_ACCELEU_BULLET;
			
			//カウンタークリア
			m_nChangeCounter = 0;
			return;
		}
		break;
		
	//加速弾攻撃
	case BOSS_ATTACK_PATTERN_ACCELEU_BULLET:
		if (m_nChangeCounter == CHANGE_ATK_FRAEM)
		{
			//通常攻撃に切り替え
			m_atkPattern = BOSS_ATTACK_PATTERN_NOMRAL;
			
			//カウンタークリア
			m_nChangeCounter = 0;
			return;
		}
		break;

	default:
		break;
	}

	//カウントアップ
	m_nChangeCounter++;
}

//=============================================================================
//ボスクラスのターゲット方向への3次元ベクトル計算処理
//=============================================================================
D3DXVECTOR3 CBoss::TargetWayVec3Calculation(D3DXVECTOR3 TargetPos, float fSpeed)
{
	//2点間のベクトルを求める（終点[目標地点] - 始点[自身の位置]）
	D3DXVECTOR3 vector = TargetPos - GetPos();

	//単位ベクトル用変数
	D3DXVECTOR3 unitVector = VECTOR3_ZERO_INITIALIZE;

	//単位ベクトルを求める(元のベクトル / ベクトルの大きさ)
	D3DXVec3Normalize(&unitVector, &vector);

	//単位ベクトルを速度倍して返す(VectorNor*fSpeed)
	return unitVector * fSpeed;
}