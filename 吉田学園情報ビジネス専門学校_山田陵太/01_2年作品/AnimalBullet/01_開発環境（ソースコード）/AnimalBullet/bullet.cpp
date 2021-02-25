//=============================================================================
//
// バレット処理 [bullet.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "sound.h"
#include "effect.h"
#include "game.h"
#include "boss.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define HOMING_SPEED 5	//ホーミングの移送速度

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//=============================================================================
//バレットクラスのコンストラクタ
//=============================================================================
CBullet::CBullet(int nPriority) :CScene2D(nPriority)
{
	//メンバ変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//サイズ
	m_nLife = 0;	//ライフ
	m_nAtk = 0;	//攻撃力
	m_nEffectCounter = 0;
	m_user = BULLET_USER_NONE;	//使用者
	m_type = BULLET_TYPE_NORMAL;	//種類
}

//=============================================================================
//バレットクラスのデストラクタ
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
//バレットクラスのクリエイト処理
//=============================================================================
CBullet * CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const D3DXCOLOR col,
	const BULLET_USER user, const BULLET_TYPE type, int nAtk, int nLife)
{
	//バレットクラスのポインタ変数
	CBullet *pBullet = NULL;

	//インスタンス生成
	pBullet = new CBullet;

	//メモリが確保できていたら
	if (pBullet != NULL)
	{
		//初期化処理呼び出し
		pBullet->Init(pos, move, size, col, user, type, nAtk, nLife);
	}

	return pBullet;
}

//=============================================================================
//バレットクラスのテクスチャロード処理
//=============================================================================
HRESULT CBullet::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Bullet.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//バレットクラスのテクスチャの破棄処理
//=============================================================================
void CBullet::UnLoad(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//バレットクラスの初期化処理
//=============================================================================
HRESULT CBullet::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const D3DXCOLOR col,
	const BULLET_USER user, const BULLET_TYPE type, int nAtk, int nLife)
{
	//初期化処理呼び出し
	CScene2D::Init(pos, size);

	//テクスチャの割り当て
	CScene2D::BindTexture(m_pTexture);

	//オブジェクトタイプの設定
	CScene::SetObjType(OBJTYPE_BULLET);

	//移動量の設定
	m_move = move;

	//サイズの設定
	m_size = size;

	//寿命の設定
	m_nLife = nLife;

	//色の設定
	m_col = col;
	CScene2D::SetColor(m_col);

	//使用者の設定
	m_user = user;

	//弾の種類の設定
	m_type = type;

	//攻撃力の設定
	m_nAtk = nAtk;

	//サウンドクラスの情報取得
	CSound *pSound = CManager::GetSound();

	return S_OK;
}

//=============================================================================
//バレットクラスの終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	//2Dポリゴンクラスの終了処理呼び出し
	CScene2D::Uninit();
}

//=============================================================================
//バレットクラスの更新処理
//=============================================================================
void CBullet::Update(void)
{
	//位置の取得
	m_pos = CScene2D::GetPos();

	switch (m_type)
	{
	//通常弾の更新
	case BULLET_TYPE_NORMAL:
		break;

	//ホーミング弾の更新
	case BULLET_TYPE_HOMING:
		HomingUpdate();
		break;

	//分裂弾の更新
	case BULLET_TYPE_DIVISION:
		DivisionUpdate();
		break;

	//加速弾の更新
	case BULLET_TYPE_ACCELE:
		ACCELEUpdetae();
		break;

	default:
		break;
	}

	if (m_nEffectCounter % 2 == 0)
	{
		//エフェクト生成
		CEffect::Create(m_pos, D3DXVECTOR3(m_size.x, m_size.y, m_size.z), m_col, 10, CEffect::EFFECT_TYPE_BULLET);
	}
	//位置に移動量を加算
	m_pos += m_move;

	//更新位置の設定
	CScene2D::SetPos(m_pos);

	//バレットの当たり判定処理
	if (BulletCollision() == true)
	{
		//終了処理
		Uninit();

		return;
	}
	//バレットの寿命が尽きたとき
	if (m_nLife <= 0)
	{
		//爆発生成
		CExplosion::Create(m_pos, D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f), m_col);

		//終了処理呼び出し
		Uninit();

		return;
	}

	//バレットが画面外に出たとき
	if (m_pos.x <= 0 || m_pos.x >= SCREEN_WIDTH || 
		m_pos.y <= 0 || m_pos.y >= SCREEN_HEIGHT)
	{
		//終了処理呼び出し
		Uninit();

		return;
	}

	//エフェクトカウンターアップ
	m_nEffectCounter++;

	//ライフを減らす
	m_nLife--;
}

//=============================================================================
//バレットクラスの描画処理
//=============================================================================
void CBullet::Draw(void)
{
	//2Dポリゴンクラスの描画処理呼び出し
	CScene2D::Draw();
}

//=============================================================================
//バレットクラスの当たり判定の処理
//=============================================================================
bool CBullet::BulletCollision(void)
{
	CPlayer *pPlayer = NULL;
	CEnemy *pEnemy = NULL;
	CBoss *pBoss = NULL;

	switch (m_user)
	{
	//使用者がエネミーだった場合
	case BULLET_USER_ENEMY:

		//プレイヤーと当たったか情報を取得
		pPlayer = (CPlayer*)CScene2D::GetCollision(OBJTYPE_PLAYER, m_pos, m_size);

		//当たり判定
		if (pPlayer != NULL)
		{
			//爆発生成
			CExplosion::Create(m_pos, D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f), m_col);

			//プレイヤーへダメージを与える
			pPlayer->Damage(m_nAtk);

			return true;
		}
		break;

	//使用者がプレイヤーだった場合
	case BULLET_USER_PLAYER:

		//敵と当たったか情報を取得
		pEnemy = (CEnemy*)CScene2D::GetCollision(OBJTYPE_ENEMY, m_pos, m_size);

		//当たり判定
		if (pEnemy != NULL)
		{
			//爆発生成
			CExplosion::Create(m_pos, D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f), m_col);

			//エネミーへダメージを与える
			pEnemy->Damage(m_nAtk);

			return true;
		}

		//ボスと当たったか情報を取得
		pBoss = (CBoss*)CScene2D::GetCollision(OBJTYPE_BOSS, m_pos, m_size);

		//当たり判定
		if (pBoss != NULL)
		{
			//爆発生成
			CExplosion::Create(m_pos, D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f), m_col);

			//ボスへダメージを与える
			pBoss->Damage(m_nAtk);

			return true;
		}
		break;
	}

	return false;
}

//=============================================================================
//バレットクラスのホーミング更新処理
//=============================================================================
void CBullet::HomingUpdate(void)
{
	//プレイヤーの位置情報を取得
	D3DXVECTOR3 Playerpos = CGame::GetPlayer()->GetPos();

	if (m_pos.x >= Playerpos.x)
	{
		if (m_nEffectCounter <= 90)
		{
			//角度θの計算（ラジアン）
			float fAngle = atan2f((Playerpos.y - m_pos.y), (Playerpos.x - m_pos.x));
		
			//移動量を求める
			m_move = D3DXVECTOR3(cosf(fAngle) * HOMING_SPEED, sinf(fAngle) * HOMING_SPEED, 0.0f);
		}
	}
}

//=============================================================================
//バレットクラスの分裂弾の更新処理
//=============================================================================
void CBullet::DivisionUpdate(void)
{
	if (m_nLife <= 0)
	{
		int nNum = 10 + rand() % 2;	//弾の数
		float fAngle = 18.0f;	//弾と弾の間の角度
		float fRad_Step = D3DXToRadian(fAngle);	//ラジアンに変換

		//端の弾と中心の弾との間の角度を計算(偶数・奇数によって結果変動)
		float fRad = nNum % 2 ? -nNum / 2 * fRad_Step : (-nNum / 2 + 0.5f) * fRad_Step;

		for (int nCount = 0; nCount < nNum; nCount++, fRad += fRad_Step)
		{
			CBullet::Create(m_pos, D3DXVECTOR3(-cosf(fRad) * 5,
				sinf(fRad) * 5, 0.0f), BULLET_SIZE,
				m_col, BULLET_USER_ENEMY, BULLET_TYPE_NORMAL, 5);
		}
	}
}

//=============================================================================
//バレットクラスの加速弾の更新処理
//=============================================================================
void CBullet::ACCELEUpdetae(void)
{
	D3DXVECTOR3 no;
	D3DXVec3Normalize(&no, &m_move);

	m_move.x += 0.5f*no.x;
	m_move.y += 0.5f*no.y;
}

//=============================================================================
//バレットクラスの使用者情報の取得処理
//=============================================================================
CBullet::BULLET_USER CBullet::GetUser(void) const
{
	return m_user;
}

