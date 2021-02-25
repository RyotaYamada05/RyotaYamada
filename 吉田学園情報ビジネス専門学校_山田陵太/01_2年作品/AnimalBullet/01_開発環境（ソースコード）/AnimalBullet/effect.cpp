//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : 
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "player.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define ALPHA_SUBTRACT_VALUE 0.1f	//アルファ減算値
#define SIZE_SUBTRACT_VALUE 0.5f	//サイズ減算値

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture[CEffect::EFFECT_TYPE_MAX] = {};

//=============================================================================
//エフェクトクラスのコンストラクタ
//=============================================================================
CEffect::CEffect(int nPriority) :CScene2D(nPriority)
{
	//各メンバ変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = COLOR_WHITE;
	m_nLife = 0;
}

//=============================================================================
//エフェクトクラスのデストラクタ
//=============================================================================
CEffect::~CEffect()
{
}

//=============================================================================
//エフェクトクラスのクリエイト処理
//=============================================================================
CEffect * CEffect::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col, int nLife, const EFFECT_TYPE type, const int nPriority)
{
	//エフェクトクラスのポインタ変数
	CEffect *pEffect = NULL;

	//メモリの確保
	pEffect = new CEffect(nPriority);

	//メモリが確保できていたら
	if (pEffect != NULL)
	{
		//初期化処理呼び出し
		pEffect->Init(pos, size, col, nLife, type);
	}
	//メモリ確保に失敗したとき
	else
	{
		return NULL;
	}

	return pEffect;
}

//=============================================================================
//エフェクトクラスのテクスチャロード処理
//=============================================================================
HRESULT CEffect::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/effect002.png", &m_pTexture[EFFECT_TYPE_BULLET]);	//バレット用
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/Cheetah.png", &m_pTexture[EFFECT_TYPE_CHEETAH]);	//チーター用
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/Gorilla.png", &m_pTexture[EFFECT_TYPE_GORILLA]);	//ゴリラ用
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/Turtle.png", &m_pTexture[EFFECT_TYPE_TURTLE]);	//カメ用
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/ShockWave.png", &m_pTexture[EFFECT_TYPE_BOME]);	//ボム用

	return S_OK;
}

//=============================================================================
//エフェクトクラスのテクスチャ破棄処理
//=============================================================================
void CEffect::UnLoad(void)
{
	for (int nCount = 0; nCount < EFFECT_TYPE_MAX; nCount++)
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
//エフェクトクラスの初期化処理
//=============================================================================
HRESULT CEffect::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col, int nLife, const EFFECT_TYPE type)
{
	//2Dポリゴンクラスの初期化処理呼び出し
	CScene2D::Init(pos, size);

	//色の設定
	m_col = col;
	CScene2D::SetColor(m_col);

	//種類の設定
	m_type = type;

	//テクスチャの設定
	CScene2D::BindTexture(m_pTexture[m_type]);

	//位置設定
	m_pos = pos;

	//サイズ設定
	m_size = size;

	//ライフの設定
	m_nLife = nLife;

	return S_OK;
}

//=============================================================================
//エフェクトクラスの終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	//2Dポリゴンクラスの終了処理呼び出し
	CScene2D::Uninit();
}

//=============================================================================
//エフェクトクラスの更新処理
//=============================================================================
void CEffect::Update(void)
{
	switch (m_type)
	{
	//バレットタイプ
	case EFFECT_TYPE_BULLET:
		UpdateBullet();
		break;

	//チーター・ゴリラ・カメタイプ
	case EFFECT_TYPE_CHEETAH:
	case EFFECT_TYPE_GORILLA:
	case EFFECT_TYPE_TURTLE:
		EffectByType();
		break;

	//ボムタイプ
	case EFFECT_TYPE_BOME:
		BomeUpdate();
		break;

	//タイトル用
	case EFFECT_TYPE_TITLE:
		TitleUpdate();
		break;
	default:
		break;
	}

	//ライフの減算
	m_nLife--;

	//ライフが0以下になったら
	if (m_nLife <= 0 || (m_size.x <= 0.0f && m_size.y <= 0.0f))
	{
		//終了処理呼び出し
		Uninit();

		return;
	}
}

//=============================================================================
//エフェクトクラスの描画処理
//=============================================================================
void CEffect::Draw(void)
{
	//2Dポリゴンクラスの描画処理
	CScene2D::Draw();
}

//=============================================================================
//エフェクトクラスのバレット用の更新処理
//=============================================================================
void CEffect::UpdateBullet(void)
{
	//サイズの減算
	if (m_size.x <= 0.0f && m_size.y <= 0.0f)
	{
		m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		m_size += D3DXVECTOR3(- SIZE_SUBTRACT_VALUE , - SIZE_SUBTRACT_VALUE, 0.0f);
	}

	//アルファ値の減算
	m_col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a - ALPHA_SUBTRACT_VALUE);

	//サイズの設定
	CScene2D::SetSize(m_size);

	//色の設定
	CScene2D::SetColor(m_col);
}

//=============================================================================
//エフェクトクラスの動物用の更新処理
//=============================================================================
void CEffect::EffectByType(void)
{
	//プレイヤーの位置を取得
	D3DXVECTOR3 Playerpos = CGame::GetPlayer()->GetPos();

	//現在位置をプレイヤーの位置に更新
	m_pos = Playerpos;

	//サイズの拡大
	m_size += D3DXVECTOR3(SIZE_SUBTRACT_VALUE * 2, SIZE_SUBTRACT_VALUE * 2, 0.0f);

	//アルファ値の減算
	m_col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a - 0.0125f);

	//位置の設定
	CScene2D::SetPos(m_pos);

	//サイズの設定
	CScene2D::SetSize(m_size);

	//色の設定
	CScene2D::SetColor(m_col);
}

//=============================================================================
//エフェクトクラスのボム用の更新処理
//=============================================================================
void CEffect::BomeUpdate(void)
{
	//サイズの拡大
	m_size += D3DXVECTOR3(SIZE_SUBTRACT_VALUE, SIZE_SUBTRACT_VALUE, 0.0f);

	//アルファ値の減算
	m_col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a - 0.05f);

	//サイズの設定
	CScene2D::SetSize(m_size);

	//色の設定
	CScene2D::SetColor(m_col);
}

//=============================================================================
//エフェクトクラスのタイトル用の更新処理
//=============================================================================
void CEffect::TitleUpdate(void)
{
	//サイズの拡大
	m_size += D3DXVECTOR3(0.5f, 0.5f, 0.0f);

	//アルファ値の減算
	m_col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a - 0.005f);

	//サイズの設定
	CScene2D::SetSize(m_size);

	//色の設定
	CScene2D::SetColor(m_col);
}

//=============================================================================
//エフェクトクラスの種類設定用処理
//=============================================================================
void CEffect::SetTexType(const EFFECT_TYPE type)
{
	//テクスチャの設定
	CScene2D::BindTexture(m_pTexture[type]);
}
