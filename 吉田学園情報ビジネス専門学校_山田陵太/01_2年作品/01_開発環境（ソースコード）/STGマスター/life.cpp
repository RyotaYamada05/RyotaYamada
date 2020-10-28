//=============================================================================
//
// メイン処理 [life.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "life.h"
#include "gauge.h"
#include "polygon.h"

//=============================================================================
//ライフクラスのコンストラクタ
//=============================================================================
CLife::CLife(int nPriority) :CScene(nPriority)
{
	//メンバ変数のクリア
	m_pGauge = NULL;
	m_Polygon = NULL;
	m_nLife = 0;
}

//=============================================================================
//ライフクラスのデストラクタ
//=============================================================================
CLife::~CLife()
{
}

//=============================================================================
//ライフクラスのクリエイト処理
//=============================================================================
CLife * CLife::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//ライフクラスのポインタ変数
	CLife *m_pLife = NULL;

	//メモリの確保
	m_pLife = new CLife;

	//メモリが確保できていたら
	if (m_pLife != NULL)
	{
		//初期化処理呼び出し
		m_pLife->Init(pos, size);
	}
	//メモリ確保に失敗したとき
	else
	{
		return NULL;
	}
	return m_pLife;
}

//=============================================================================
//ライフクラスの初期化処理
//=============================================================================
HRESULT CLife::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//ポリゴンクラスを生成
	m_Polygon = CPolygon::Create(D3DXVECTOR3(0.0f + (+GAUGE_SIZE_X / 2), 0.0f + (GAUGE_SIZE_Y / 2), 0.0f),
		GAUGE_SIZE, CPolygon::TEX_TYPE_GAUGE_UI);

	//ゲージクラスの生成
	m_pGauge = CGauge::Create(D3DXVECTOR3(100.0f, 11.0f, 0.0f), LIFE_GAGUE_SIZE, m_nLife, CGauge::GAUGE_TYPE_LIFE_GREEN);

	return S_OK;
}

//=============================================================================
//ライフクラスの終了処理
//=============================================================================
void CLife::Uninit(void)
{
	if (m_Polygon != NULL)
	{
		//ポリゴンクラスの終了処理呼び出し
		m_Polygon->Uninit();

		//メモリの削除
		delete m_Polygon;

		//メモリのクリア
		m_Polygon = NULL;
	}

	if (m_pGauge != NULL)
	{
		//ゲージクラスの終了処理呼び出し
		m_pGauge->Uninit();

		//メモリの削除
		delete m_pGauge;

		//メモリのクリア
		m_pGauge = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//ライフクラスの更新処理
//=============================================================================
void CLife::Update(void)
{
}

//=============================================================================
//ライフクラスの描画処理
//=============================================================================
void CLife::Draw(void)
{
	if (m_pGauge != NULL)
	{
		//ゲージクラスの描画処理呼び出し
		m_pGauge->Draw();
	}
	if (m_Polygon != NULL)
	{
		//ポリゴンクラスの描画処理呼び出し
		m_Polygon->Draw();
	}
}

//=============================================================================
//ライフクラスの設定処理
//=============================================================================
void CLife::SetLife(int nLife)
{
	m_nLife = nLife;

	//体力が50以下の時
	if (m_nLife >= 50)
	{
		//テクスチャを変更する
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_GREEN);
	}
	//体力が15以下の時
	else if (m_nLife >= 15)
	{
		//テクスチャを変更する
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_ORANGE);
	}
	else
	{
		//テクスチャを変更する
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_RED);
	}

	//体力が100より大きくなったとき
	if (m_nLife > 100)
	{
		m_nLife = 100;
	}
	//体力が0より小さくなったとき
	else if (m_nLife < 0)
	{
		m_nLife = 0;
	}
	//ゲージクラスへ現在体力を設定する
	m_pGauge->SetGauge(m_nLife);
}

//=============================================================================
//ライフクラスの体力加算処理
//=============================================================================
void CLife::AddLife(int nLife)
{
	//体力の加算
	m_nLife += nLife;

	//体力が50以下の時
	if (m_nLife >= 50)
	{
		//テクスチャを変更する
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_GREEN);
	}
	//体力が15以下の時
	else if (m_nLife >= 15)
	{
		//テクスチャを変更する
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_ORANGE);
	}

	//体力が100より大きくなったとき
	if (m_nLife > 100)
	{
		m_nLife = 100;
	}

	//ゲージクラスへ現在体力を設定する
	m_pGauge->SetGauge(m_nLife);
}

//=============================================================================
//ライフクラスの体力減算処理
//=============================================================================
void CLife::SubLife(int nLife)
{
	//体力の減算
	m_nLife -= nLife;

	//体力が15より小さい時
	if (m_nLife < 15)
	{
		//テクスチャを変更する
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_RED);
	}
	//体力が50より小さい時
	else if (m_nLife < 50)
	{
		//テクスチャを変更する
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_ORANGE);
	}

	//体力が0より小さくなったとき
	if (m_nLife < 0)
	{
		m_nLife = 0;
	}

	//ゲージクラスへ現在体力を設定する
	m_pGauge->SetGauge(m_nLife);
}


