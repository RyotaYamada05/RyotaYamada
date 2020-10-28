//=============================================================================
//
// メイン処理 [gauge.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "gauge.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CGauge::m_apTextuer[CGauge::GAUGE_TYPE_MAX] = {};

//=============================================================================
//ゲージクラスのコンストラクタ
//=============================================================================
CGauge::CGauge()
{
	//各メンバ変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = GAUGE_TYPE_LIFE_GREEN;
	m_pVtxBuff = NULL;
	m_nGaugeNum = 0;
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
CGauge * CGauge::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, int nGaugeNum, const GAUGE_TYPE type)
{
	//ゲージクラスのポインタ変数
	CGauge *pGauge = NULL;

	//メモリの確保
	pGauge = new CGauge;

	//メモリが確保できていたら
	if (pGauge != NULL)
	{
		//初期化処理呼び出し
		pGauge->Init(pos, size, nGaugeNum, type);
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
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/gaugeberHP.png", &m_apTextuer[GAUGE_TYPE_LIFE_GREEN]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/gaugeberHP_ORANGE.png", &m_apTextuer[GAUGE_TYPE_LIFE_ORANGE]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/gaugeberHP_RED.png", &m_apTextuer[GAUGE_TYPE_LIFE_RED]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/skillber.png", &m_apTextuer[GAUGE_TYPE_SKILL]);
	
	return S_OK;
}

//=============================================================================
//ゲージクラスのテクスチャ破棄処理
//=============================================================================
void CGauge::UnLoad(void)
{
	for (int nCntTexture = 0; nCntTexture < GAUGE_TYPE_MAX; nCntTexture++)
	{
		//テクスチャ破棄
		if (m_apTextuer[nCntTexture] != NULL)
		{
			m_apTextuer[nCntTexture]->Release();
			m_apTextuer[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
//ゲージクラスの初期化処理
//=============================================================================
HRESULT CGauge::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, int nGaugeNum, const GAUGE_TYPE type)
{
	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//位置の設定
	m_pos = pos;

	//サイズの設定
	m_size = size;

	//ゲージの設定
	m_nGaugeNum = nGaugeNum;

	//ゲージタイプの設定
	m_type = type;

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
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_nGaugeNum * m_size.x), m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_nGaugeNum * m_size.x), m_pos.y + m_size.y, 0.0f);

	//rhwの設定（値は1.0で固定）
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

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
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//ゲージクラスの更新処理
//=============================================================================
void CGauge::Update(void)
{
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
	pD3DDevice->SetTexture(0, m_apTextuer[m_type]);

	// ポリゴンの描画
	pD3DDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,
		NUM_POLYGON);	//ポリゴン数

	//テクスチャの設定を元に戻す
	pD3DDevice->SetTexture(0, NULL);
}

//=============================================================================
//ゲージクラスの設定処理
//=============================================================================
void CGauge::SetGauge(const int nGaugeNum)
{
	m_nGaugeNum = nGaugeNum;

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_nGaugeNum * m_size.x), m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_nGaugeNum * m_size.x), m_pos.y + m_size.y, 0.0f);

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//ゲージクラスのタイプ設定処理
//=============================================================================
void CGauge::SetType(const GAUGE_TYPE type)
{
   	m_type = type;
}
