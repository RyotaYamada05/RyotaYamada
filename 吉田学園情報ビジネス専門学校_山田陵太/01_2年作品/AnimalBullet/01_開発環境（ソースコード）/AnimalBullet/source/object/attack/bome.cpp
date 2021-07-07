//=============================================================================
//
// ボム処理 [bome.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "bome.h"
#include "manager.h"
#include "renderer.h"
#include "enemy.h"
#include "effect.h"
#include "boss.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define BOME_ADD_SIZE 3.0f	//サイズ加算用定数
#define BOME_DAMAGE 1		//ボムの攻撃力
#define BOME_EFFECT_LIFE 20	//ボムに付随するエフェクトのライフ

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBome::m_pTexture = NULL;

//=============================================================================
//ボムクラスのコンストラクタ
//=============================================================================
CBome::CBome(int nPriority):CScene2D(nPriority)
{
	//各メンバ変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nCounter = 0;
	m_state = BOME_STATE_NORMAL;
}

//=============================================================================
//ボムクラスのデストラクタ
//=============================================================================
CBome::~CBome()
{
}

//=============================================================================
//ボムクラスのクリエイト処理
//=============================================================================
CBome * CBome::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//ボムクラスのポインタ変数
	CBome *pBome = NULL;

	//メモリの確保
	pBome = new CBome;

	//メモリが確保できていたら
	if (pBome)
	{
		pBome->SetPos(pos);
		pBome->SetSize(size);
		pBome->Init();
	}
	//メモリ確保に失敗したとき
	else
	{
		return NULL;
	}

	return pBome;
}

//=============================================================================
//ボムクラスのテクスチャ読み込み処理
//=============================================================================
HRESULT CBome::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/ShockWave.png", &m_pTexture);
	return S_OK;
}

//=============================================================================
//ボムクラスのテクスチャ破棄処理
//=============================================================================
void CBome::UnLoad(void)
{
	//テクスチャの破棄
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//ボムクラスの初期化処理
//=============================================================================
HRESULT CBome::Init(void)
{
	//初期化処理呼び出し
	CScene2D::Init();

	//テクスチャの割り当て
	CScene2D::BindTexture(m_pTexture);

	//オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_BOME);

	//移動量の設定
	m_move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);

	//色の設定
	m_col = COLOR_PURPLE;
	SetColor(m_col);

	return S_OK;
}

//=============================================================================
//ボムクラスの終了処理
//=============================================================================
void CBome::Uninit(void)
{
	//2Dポリゴンクラスの終了処理呼び出し
	CScene2D::Uninit();
}

//=============================================================================
//ボムクラスの更新処理
//=============================================================================
void CBome::Update(void)
{
	//CScene2Dの更新処理
	CScene2D::Update();

	//エネミーとボスのポインタ変数
	CEnemy *pEnemy = NULL;
	CBoss *pBoss = NULL;

	switch (m_state)
	{
		//通常状態
	case BOME_STATE_NORMAL:

		//位置の取得
		m_pos = CScene2D::GetPos();

		if (m_nCounter % 5 == 0)
		{
			//エフェクトの生成
			CEffect::Create(m_pos, BOME_SIZE, m_col, BOME_EFFECT_LIFE, CEffect::EFFECT_TYPE_BOME);
		}
		
		//位置の加算
		m_pos += m_move;

		//位置の設定
		CScene2D::SetPos(m_pos);

		//エネミーとの当たり判定
		pEnemy = (CEnemy *)CScene2D::JudgeCollision(CScene2D::OBJTYPE_ENEMY, m_pos, GetSize());

		//ボスとの当たり判定
		pBoss = (CBoss *)CScene2D::JudgeCollision(CScene2D::OBJTYPE_BOSS, m_pos, GetSize());

		//カウンターが120フレーム経過または、エネミー・ボスと当たっていたら爆発状態へ移行
		if (m_nCounter >= 120 || pEnemy || pBoss)
		{
			//爆発状態へ移行
			m_state = BOME_STATE_EXPLOSION;

			//カウンターをクリア
			m_nCounter = 0;
		}
		break;

		//爆発状態
	case BOME_STATE_EXPLOSION:

		m_size = GetSize();
		m_col = GetColor();

		//サイズの拡大
		m_size += D3DXVECTOR3(BOME_ADD_SIZE, BOME_ADD_SIZE, 0.0f);

		//サイズの設定
		CScene2D::SetSize(m_size);

		//α値を減少させる
		m_col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a - 0.005f);

		//基底クラスへ情報を設定する
		CScene2D::SetColor(m_col);

		//爆破エフェクト用の変数
		D3DXCOLOR randomCol;

		//爆破エフェクト用の色を乱数で設定
		switch (rand() % 4 + 1)
		{
		case 1:
			randomCol = COLOR_RED;
			break;
		case 2:
			randomCol = COLOR_BLUE;
			break;
		case 3:
			randomCol = COLOR_GREEN;
			break;
		case 4:
			randomCol = COLOR_YELLOW;
			break;
		case 5:
			randomCol = COLOR_PURPLE;
			break;
		}

		//一度に5つ生成
		for (int nCount = 0; nCount < 5; nCount++)
		{
			float fRandomX = m_pos.x + (-m_size.x / 3) + rand() % (int)m_size.x / 1.5f;
			float fRandomY = m_pos.y + (-m_size.y / 3) + rand() % (int)m_size.y / 1.5f;

			//エフェクトの生成
			CEffect::Create(D3DXVECTOR3(fRandomX, fRandomY, 0.0f), BOME_SIZE, randomCol, BOME_EFFECT_LIFE, CEffect::EFFECT_TYPE_BOME);
		}

		//エネミーとの当たり判定
		JudgeCollisionEnemy();
	
		//ボスとの当たり判定
		pBoss = (CBoss *)CScene2D::JudgeCollision(CScene2D::OBJTYPE_BOSS, m_pos, m_size);

		//当たり判定
		if (pBoss)
		{
			//ボスへダメージを与える
			pBoss->Damage(BOME_DAMAGE);
		}

		//カウンターが180フレーム経過したら
		if (m_nCounter >= 180)
		{
			Uninit();
			return;
		}
		break;
	}

	//カウンターアップ
	m_nCounter++;
}

//=============================================================================
//ボムクラスの描画処理
//=============================================================================
void CBome::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
//ボムクラスの敵との当たり判定処理
//=============================================================================
void CBome::JudgeCollisionEnemy(void)
{
	for (int nCntPrity = 0; nCntPrity < CScene::PRIORITY_MAX; nCntPrity++)
	{
		//リストの先頭情報の取得
		CScene *pScene = CScene::GetTop(nCntPrity);

		//NULLになるまで繰り返す
		while (pScene)
		{
			//オブジェクトの種類を取得
			OBJTYPE type = pScene->GetObjType();

			//オブジェクトタイプが目的のものだった場合
			if (type == CScene2D::OBJTYPE_ENEMY)
			{
				//2Dポリゴンクラスポインタにキャストし、情報を取得
				CScene2D *pScene2D = (CScene2D *)pScene;

				//ターゲットの位置とサイズ情報を取得
				D3DXVECTOR3 targetPos = pScene2D->GetPos();
				D3DXVECTOR3 targetSize = pScene2D->GetSize();

				//当たり判定
				if (targetPos.x + (targetSize.x / 2) >= m_pos.x - (m_size.x / 2) &&
					targetPos.x - (targetSize.x / 2) <= m_pos.x + (m_size.x / 2) &&
					targetPos.y + (targetSize.y / 2) >= m_pos.y - (m_size.y / 2) &&
					targetPos.y - (targetSize.y / 2) <= m_pos.y + (m_size.y / 2))
				{
					//CEnemyのポインタ型へキャスト
					CEnemy *pEnemy = (CEnemy *)pScene2D;

					//ダメージを与える
					pEnemy->Damage(BOME_DAMAGE);
				}
			}

			//次の情報を取得
			pScene = pScene->GetNext();
		}
	}
}
