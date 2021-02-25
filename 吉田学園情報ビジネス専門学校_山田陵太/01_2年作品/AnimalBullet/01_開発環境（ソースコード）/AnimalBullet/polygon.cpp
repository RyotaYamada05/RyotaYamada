//=============================================================================
//
// ポリゴン処理 [life.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "polygon.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define FLASHING_RATE 0.05f	//点滅速度の定数
#define MAX_ALPHA 1.0f	//α値の最大値
#define MIN_ALPHA 0.0f	//α値の最低値

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CPolygon::m_pTexture[CPolygon::TEX_TYPE_MAX] = {};

//=============================================================================
//ポリゴンクラスのコンストラクタ
//=============================================================================
CPolygon::CPolygon()
{
	//各メンバ変数のクリア
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_type = TEX_TYPE_NORE;
	m_Flashing = FLASHING_NONE;
}

//=============================================================================
//ポリゴンクラスのデストラクタ
//=============================================================================
CPolygon::~CPolygon()
{
}

//=============================================================================
//ポリゴンクラスのクリエイト処理
//=============================================================================
CPolygon * CPolygon::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const TEX_TYPE type )
{
	//ポリゴンクラスのポインタ変数
	CPolygon *pPolygon = NULL;

	//メモリの確保
	pPolygon = new CPolygon;

	//メモリを確保できていたら
	if (pPolygon != NULL)
	{
		//初期化処理呼び出し
		pPolygon->Init(pos, size, type);
	}
	else
	{
		return NULL;
	}
	return pPolygon;
}

//=============================================================================
//ポリゴンクラスのテクスチャ読み込み処理
//=============================================================================
HRESULT CPolygon::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	m_pTexture[TEX_TYPE_NORE] = NULL;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/BG/Title.png", &m_pTexture[TEX_TYPE_TITLE]);	//タイトル
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/TItleLogo.png", &m_pTexture[TEX_TYPE_TITLE_LOGO]);	//タイトル
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/BG/Result.png", &m_pTexture[TEX_TYPE_RESULET]);	//リザルト
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/gaugeUI.png", &m_pTexture[TEX_TYPE_GAUGE_UI]);	//ゲージUI
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/UnderUI.png", &m_pTexture[TEX_TYPE_UNDERUI]);	//UnderUI
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/Cheetah.png", &m_pTexture[TEX_TYPE_CHEETAH]);	//チーター
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/Gorilla.png", &m_pTexture[TEX_TYPE_GORILLA]);	//ゴリラ
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/Turtle.png", &m_pTexture[TEX_TYPE_TURTLE]);	//カメ
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/PRESS_ENTER.png", &m_pTexture[TEX_TYPE_PRESSENTER]);	//PRESS ENTERテクスチャ
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/WARNING.png", &m_pTexture[TEX_TYPE_WARNING]);	//WARNINGテクスチャ
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/GAME CLEAR.png", &m_pTexture[TEX_TYPE_GAME_CLEAR]);	//WARNINGテクスチャ
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/GAME OVER.png", &m_pTexture[TEX_TYPE_GAME_OVER]);	//WARNINGテクスチャ

	return S_OK;
}

//=============================================================================
//ポリゴンクラスのテクスチャ破棄処理
//=============================================================================
void CPolygon::UnLoad(void)
{
	for (int nCount = 0; nCount < TEX_TYPE_MAX; nCount++)
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
//ポリゴンクラスの初期化処理
//=============================================================================
HRESULT CPolygon::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const TEX_TYPE type)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//初期位置の設定
	m_pos = pos;

	//大きさの設定
	m_size = size;

	//テクスチャの設定
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
//ポリゴンクラスの終了処理
//=============================================================================
void CPolygon::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//ポリゴンクラスの更新処理
//=============================================================================
void CPolygon::Update(void)
{
}

//=============================================================================
//ポリゴンクラスの描画処理
//=============================================================================
void CPolygon::Draw(void)
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
//ポリゴンクラスの種類の設定処理
//=============================================================================
void CPolygon::SetType(const TEX_TYPE type)
{
	m_type = type;
}

//=============================================================================
//ポリゴンクラスの色の設定処理
//=============================================================================
void CPolygon::SetColor(const D3DXCOLOR color)
{
	m_col = color;

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//ポリゴンクラスの位置の設定処理
//=============================================================================
void CPolygon::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;

	VERTEX_2D *pVtx;

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-m_size.x / 2), m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);


	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//ポリゴンクラスの位置の取得処理
//=============================================================================
D3DXVECTOR3 CPolygon::GetPos(void)
{
	return m_pos;
}

//=============================================================================
//ポリゴンクラスの点滅処理
//=============================================================================
void CPolygon::Flashing(void)
{
	if (m_Flashing == FLASHING_NONE)
	{
		//減算状態へ移行
		m_Flashing = FLASHING_SUB;
		m_col.a = MAX_ALPHA;
	}

	if (m_Flashing == FLASHING_ADD)
	{
		//α値の加算
		m_col.a += FLASHING_RATE;

		if (m_col.a >= MAX_ALPHA)
		{
			m_col.a = MAX_ALPHA;

			//通常状態へ移行
			m_Flashing = FLASHING_NONE;
		}
	}
	else if (m_Flashing == FLASHING_SUB)
	{
		//α値の減算
		m_col.a -= FLASHING_RATE;

		if (m_col.a <= MIN_ALPHA)
		{
			m_col.a = MIN_ALPHA;

			//加算状態へ移行
			m_Flashing = FLASHING_ADD;
		}
	}

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}
