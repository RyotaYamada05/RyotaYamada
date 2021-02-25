//=============================================================================
//
// シールド処理 [shield.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "shield.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "bullet.h"
#include "beam.h"
#include "sound.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define COUNTER_ATK 10	//攻撃力
#define GUARD_LIMIT_TIME 300	//ガード状態の制限時間
#define MAX_COUNTER_LIMIT 600	//カウンターの最大制限時間	
#define MIN_COUNTER_LIMIT 300	//カウンターの最小制限時間	

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CShield::m_pTexture = NULL;

//=============================================================================
//シールドクラスのコンストラクタ
//=============================================================================
CShield::CShield(int nPriority):CScene2D(nPriority)
{
	//各メンバ変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nAtk = 0;
	m_nHitCounter = 0;
	m_state = SHIELD_STATE_NONE;
	m_nTimeLimit = 0;
}

//=============================================================================
//シールドクラスのデストラクタ
//=============================================================================
CShield::~CShield()
{
}

//=============================================================================
//シールドクラスのクリエイト処理
//=============================================================================
CShield * CShield::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//シールドクラスのポインタ変数
	CShield *pShield = NULL;

	//インスタンス生成
	pShield = new CShield;

	//メモリが確保できていたら
	if (pShield != NULL)
	{
		//初期化処理呼び出し
		pShield->Init(pos, size);
	}
	//失敗していたら
	else
	{
		return NULL;
	}

	return pShield;
}

//=============================================================================
//シールドクラスのテクスチャロード処理
//=============================================================================
HRESULT CShield::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/shied.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//シールドクラスのテクスチャ破棄処理
//=============================================================================
void CShield::UnLoad(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//シールドクラスの初期化処理
//=============================================================================
HRESULT CShield::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//位置の設定
	m_pos = D3DXVECTOR3(pos.x + SHILED_SIZE_X * 3, pos.y, pos.z);;

	//サイズの設定
	m_size = size;

	//状態の設定
	m_state = SHIELD_STATE_GUARD;

	//攻撃力の設定
	m_nAtk = COUNTER_ATK;

	//制限時間の設定
	m_nTimeLimit = GUARD_LIMIT_TIME;

	//2Dポリゴンクラスの初期化
	CScene2D::Init(m_pos, m_size);

	//テクスチャの設定
	CScene2D::BindTexture(m_pTexture);

	return S_OK;
}

//=============================================================================
//シールドクラスの終了処理
//=============================================================================
void CShield::Uninit(void)
{
	//終了処理呼び出し
	CScene2D::Uninit();
}

//=============================================================================
//シールドクラスの更新処理
//=============================================================================
void CShield::Update(void)
{
	//状態別に処理を実行
	switch (m_state)
	{
	//ガード状態
	case SHIELD_STATE_GUARD:
		//ガード処理実行
		Guard();
		break;

	//カウンター攻撃状態
	case SHIELD_STATE_COUNTER_ATK:
		//ビームの生成
		CBeam::Create(m_pos, BEAM_SIZE, m_nTimeLimit);
		
		Uninit();
		return;
		
		break;

	default:
		break;
	}

	//位置の取得
	m_pos = GetPos();

	//プレイヤーの位置の取得
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

	//位置の更新
	m_pos = D3DXVECTOR3(playerPos.x + SHILED_SIZE_X * 3, playerPos.y, playerPos.z);

	//位置の設定
	SetPos(m_pos);

	//制限時間の減少
	m_nTimeLimit--;
}

//=============================================================================
//シールドクラスの描画処理
//=============================================================================
void CShield::Draw(void)
{
	//描画処理呼び出し
	CScene2D::Draw();
}

//=============================================================================
//シールドクラスのガード処理
//=============================================================================
void CShield::Guard(void)
{
	//処理タイマーがガード処理の制限時間内だったら
	if (m_nTimeLimit > 0)
	{
		//バレットと当たったかどうか情報取得
		CScene2D *pScene2D = GetCollision(OBJTYPE_BULLET, m_pos, m_size);

		//当たり判定
		if (pScene2D != NULL)
		{
			//バレットクラスへキャスト
			CBullet *pBullet = (CBullet*)pScene2D;

			//バレットの使用者が敵だった場合
			if (pBullet->GetUser() == CBullet::BULLET_USER_ENEMY)
			{
				//ヒットカウントを加算
				m_nHitCounter++;

				//バレットを削除
				pBullet->Uninit();
				pBullet = NULL;

				//ガード音の再生
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GUARD);
			}
		}
	}
	//制限時間オーバーだったら
	else
	{
		//カウンター攻撃状態へ変更
		m_state = SHIELD_STATE_COUNTER_ATK;

		//制限時間をカウンター攻撃状態用へ設定する(最低時間+ヒット数*5)
		m_nTimeLimit = MIN_COUNTER_LIMIT + m_nHitCounter * 5;

		//制限時間が最大定数より超えた場合は修正
		if (m_nTimeLimit > MAX_COUNTER_LIMIT)
		{
			m_nTimeLimit = MAX_COUNTER_LIMIT;
		}
	}
}