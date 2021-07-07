//=============================================================================
//
// ゲージ処理 [gauge.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "gauge.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define GAUGE_RATE_SPPED 10.0f	//ゲージの速度

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CGauge::m_pTextuer = NULL;

//=============================================================================
//ゲージクラスのコンストラクタ
//=============================================================================
CGauge::CGauge(int nPriority) : CScene(nPriority)
{
	//各メンバ変数のクリア
	m_pos = VECTOR3_ZERO_INITIALIZE;
	m_size = VECTOR3_ZERO_INITIALIZE;
	m_pVtxBuff = NULL;
	m_nGaugeNum = 0;
	m_nMaxGaugeNum = 0;
}

//=============================================================================
//ゲージクラスのデストラクタ
//=============================================================================
CGauge::~CGauge()
{
}

//=============================================================================
//ゲージクラスのクリエイト処理
//=============================================================================
CGauge * CGauge::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col,
	const int nGaugeNum, const int nMaxGaugeNum)
{
	//ゲージクラスのポインタ変数
	CGauge *pGauge = NULL;

	//メモリの確保
	pGauge = new CGauge;

	//メモリが確保できていたら
	if (pGauge)
	{
		pGauge->SetPos(pos);
		pGauge->SetSize(size);
		pGauge->SetColor(col);
		pGauge->SetMaxGauge(nMaxGaugeNum);
		pGauge->SetGauge(nGaugeNum);
		//初期化処理呼び出し
		pGauge->Init();
	}
	//メモリ確保に失敗したら
	else
	{
		return NULL;
	}

	return pGauge;
}

//=============================================================================
//ゲージクラスのテクスチャロード処理
//=============================================================================
HRESULT CGauge::Load(void)
{
	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/gaugeber.png", &m_pTextuer);
	
	return S_OK;
}

//=============================================================================
//ゲージクラスのテクスチャ破棄処理
//=============================================================================
void CGauge::UnLoad(void)
{
	//テクスチャ破棄
	if (m_pTextuer)
	{
		m_pTextuer->Release();
		m_pTextuer = NULL;
	}

}

//=============================================================================
//ゲージクラスの初期化処理
//=============================================================================
HRESULT CGauge::Init()
{
	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの作成
	pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,	//バッファサイズ
		D3DUSAGE_WRITEONLY,	//（固定）
		FVF_VERTEX_2D,	//フォーマット
		D3DPOOL_MANAGED,	//(固定)
		&m_pVtxBuff,
		NULL
	);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + ((float)m_nGaugeNum / (float)m_nMaxGaugeNum) * m_size.x, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + ((float)m_nGaugeNum / (float)m_nMaxGaugeNum) * m_size.x, m_pos.y + m_size.y, 0.0f);;

	//rhwの設定（値は1.0で固定）
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//ゲージクラスの終了処理
//=============================================================================
void CGauge::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクトの破棄
	SetDeathFlag();
}

//=============================================================================
//ゲージクラスの更新処理
//=============================================================================
void CGauge::Update(void)
{
	//頂点の更新処理
	VertexUpdate();
}

//=============================================================================
//ゲージクラスの描画処理
//=============================================================================
void CGauge::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pD3DDevice->SetTexture(0, m_pTextuer);

	// ポリゴンの描画
	pD3DDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,
		NUM_POLYGON);	//ポリゴン数

	//テクスチャの設定を元に戻す
	pD3DDevice->SetTexture(0, NULL);
}

//=============================================================================
//ゲージクラスの位置の設定処理
//=============================================================================
void CGauge::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
//ゲージクラスのサイズ設定処理
//=============================================================================
void CGauge::SetSize(const D3DXVECTOR3 size)
{
	m_size = size;
}

//=============================================================================
//ゲージクラスの色の設定処理
//=============================================================================
void CGauge::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
//ゲージクラスの設定処理
//=============================================================================
void CGauge::SetGauge(const int nGaugeNum)
{
	m_nGaugeNum = nGaugeNum;

	//最大数より大きくなったら最大数へ修正
	if (m_nGaugeNum > m_nMaxGaugeNum)
	{
		m_nGaugeNum = m_nMaxGaugeNum;
	}
}

//=============================================================================
//ゲージクラスの現在値の取得処理
//=============================================================================
int CGauge::GetGauge(void) const
{
	return m_nGaugeNum;
}

//=============================================================================
//ゲージクラスの最大値設定処理
//=============================================================================
void CGauge::SetMaxGauge(const int nMaxGaugeNum)
{
	m_nMaxGaugeNum = nMaxGaugeNum;
}

//=============================================================================
//ゲージクラスの最大値の取得処理
//=============================================================================
int CGauge::GetMaxGague(void) const
{
	return m_nMaxGaugeNum;
}

//=============================================================================
//ゲージクラスの加算処理
//=============================================================================
void CGauge::AddGauge(int nAddGauge)
{
	m_nGaugeNum += nAddGauge;

	//最大数より大きくなったら最大数へ修正
	if (m_nGaugeNum > m_nMaxGaugeNum)
	{
		m_nGaugeNum = m_nMaxGaugeNum;
	}
}

//=============================================================================
//ゲージクラスの減算処理
//=============================================================================
void CGauge::SubGauge(int nSubGauge)
{
	m_nGaugeNum -= nSubGauge;
}

//=============================================================================
//ゲージクラスの頂点の更新処理
//=============================================================================
void CGauge::VertexUpdate(void)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + ((float)m_nGaugeNum / (float)m_nMaxGaugeNum) * m_size.x, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + ((float)m_nGaugeNum / (float)m_nMaxGaugeNum) * m_size.x, m_pos.y + m_size.y, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
} 