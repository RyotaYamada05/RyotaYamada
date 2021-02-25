//=============================================================================
//
// ナンバー処理 [number.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CNumber::m_pTexture[NUMBER_TYPE_MAX] = {};

//=============================================================================
//ナンバークラスのコンストラクタ
//=============================================================================
CNumber::CNumber()
{
	//各メンバ変数のクリア
	m_nNumber = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVtxBuff = NULL;
	m_type = NUMBER_TYPE_000;
}

//=============================================================================
//ナンバークラスのデストラクタ
//=============================================================================
CNumber::~CNumber()
{
}

//=============================================================================
//ナンバークラスのクリエイト処理
//=============================================================================
CNumber * CNumber::Create(const int nNumber, const NUMBER_TYPE Type, const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR color)
{
	CNumber *pNumber = NULL;

	//メモリの確保
	pNumber = new CNumber;

	//メモリが確保できていたら
	if (pNumber != NULL)
	{
		//初期化処理呼び出し
		pNumber->Init(nNumber, Type, pos, size, color);
	}
	//メモリ確保に失敗したとき
	else
	{
		return NULL;
	}

	return pNumber;
}

//=============================================================================
//ナンバークラスのテクスチャロード処理
//=============================================================================
HRESULT CNumber::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Number/number000.png", &m_pTexture[NUMBER_TYPE_000]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Number/number001.png", &m_pTexture[NUMBER_TYPE_001]);

	return S_OK;
}

//=============================================================================
//ナンバークラスのテクスチャ破棄処理
//=============================================================================
void CNumber::UnLoad(void)
{
	for (int nCount = 0; nCount < NUMBER_TYPE_MAX; nCount++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//ナンバークラスの初期化処理
//=============================================================================
HRESULT CNumber::Init(const int nNumber, const NUMBER_TYPE Type, const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR color)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//初期位置の設定
	m_pos = pos;

	//大きさの設定
	m_size = size;

	//数字の設定
	m_nNumber = nNumber;

	m_type = Type;

	//頂点バッファの作成
	pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,	//バッファサイズ
		D3DUSAGE_WRITEONLY,	//（固定）
		FVF_VERTEX_2D,	//フォーマット
		D3DPOOL_MANAGED,	//(固定)
		&m_pVtxBuff,
		NULL
	);

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-m_size.x / 2), m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);

	//rhwの設定（値は1.0で固定）
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = color;
	pVtx[1].col = color;
	pVtx[2].col = color;
	pVtx[3].col = color;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(m_nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_nNumber * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_nNumber * 0.1f + 0.1f, 1.0f);

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//ナンバークラスの終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();

		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//ナンバークラスの更新処理
//=============================================================================
void CNumber::Update(void)
{
}

//=============================================================================
//ナンバークラスの描画処理
//=============================================================================
void CNumber::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();	//デバイスのポインタ

	//頂点バッファをデバイスのデータストリームにバインド
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pD3DDevice->SetTexture(0, m_pTexture[m_type]);

	// ポリゴンの描画
	pD3DDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,
		NUM_POLYGON);	//ポリゴン数

	//テクスチャの設定を元に戻す
	pD3DDevice->SetTexture(0, NULL);
}

//=============================================================================
//ナンバークラスの設定処理
//=============================================================================
void CNumber::SetNumber(const int nNumber)
{
	//ナンバーの設定
	m_nNumber = nNumber;

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(m_nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_nNumber * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_nNumber * 0.1f + 0.1f, 1.0f);

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//ナンバークラスの色設定処理
//=============================================================================
void CNumber::SetColr(const D3DXCOLOR col)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}
