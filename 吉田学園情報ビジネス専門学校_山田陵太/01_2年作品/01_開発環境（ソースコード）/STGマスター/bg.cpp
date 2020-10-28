//=============================================================================
//
// メイン処理 [bg.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBg::m_apTexture[MAX_BG_TEX] = {};	//テクスチャのポインタ

//=============================================================================
//背景クラスのコンストラクタ
//=============================================================================
CBg::CBg(int nPriority) : CScene(nPriority)
{
	//0クリア
	memset(m_apScene2D, NULL, sizeof(m_apScene2D));
}

//=============================================================================
//背景クラスのデストラクタ
//=============================================================================
CBg::~CBg()
{
}

//=============================================================================
//背景クラスのクリエイト処理
//=============================================================================
CBg * CBg::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//背景クラスのポインタ変数
	CBg *pBg = NULL;

	//インスタンス生成
	pBg = new CBg;

	//メモリが確保できていたら
	if (pBg != NULL)
	{
		//背景クラスの初期化処理呼び出し
		pBg->Init(pos, size);
	}

	return pBg;
}

//=============================================================================
//背景クラスのテクスチャロード処理
//=============================================================================
HRESULT CBg::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/BG/bg.png", &m_apTexture[0]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/BG/bg01.png", &m_apTexture[1]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/BG/bg02.png", &m_apTexture[2]);

	return S_OK;
}

//=============================================================================
//背景クラスのテクスチャ破棄処理
//=============================================================================
void CBg::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_BG_TEX; nCount++)
	{
		//テクスチャの破棄
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//背景クラスの初期化処理
//=============================================================================
HRESULT CBg::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	for (int nCount = 0; nCount < MAX_2D_NUM; nCount++)
	{
		//2Dポリゴンクラスのインスタンス生成
		m_apScene2D[nCount] = CScene2D::Create(pos, size,CScene::PRIORITY_BG);

		if (m_apScene2D[nCount] != NULL)
		{
			//テクスチャの割り当て
			m_apScene2D[nCount]->BindTexture(m_apTexture[nCount]);
		}
	}
	
	return S_OK;
}

//=============================================================================
//背景クラスの終了処理
//=============================================================================
void CBg::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_2D_NUM; nCount++)
	{
		if (m_apScene2D[nCount] != NULL)
		{
			m_apScene2D[nCount]->Uninit();
		}
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//背景クラスの更新処理
//=============================================================================
void CBg::Update(void)
{
	//移動量
	static float fMove[3] = {};

	//UV座標用の変数
	D3DXVECTOR2 aUVpos[4];
	
	for (int nCount = 0; nCount < MAX_2D_NUM; nCount++)
	{
		if (m_apScene2D[nCount] != NULL)
		{
			//2Dポリゴンクラスの更新処理呼び出し
			m_apScene2D[nCount]->Update();

			//移動量の計算
			fMove[nCount] += 0.0025f + (nCount * 0.0025f);

			//UV座標の定義
			/*aUVpos[0] = D3DXVECTOR2(0.0f , 0.0f - fMove[nCount]);
			aUVpos[1] = D3DXVECTOR2(1.0f + nCount * 0.5f, 0.0f - fMove[nCount]);
			aUVpos[2] = D3DXVECTOR2(0.0f , 1.0f - fMove[nCount]);
			aUVpos[3] = D3DXVECTOR2(1.0f + nCount * 0.5f, 1.0f - fMove[nCount]);*/

			aUVpos[0] = D3DXVECTOR2(0.0f + fMove[nCount], 0.0f);
			aUVpos[1] = D3DXVECTOR2(1.0f + fMove[nCount], 0.0f);
			aUVpos[2] = D3DXVECTOR2(0.0f + fMove[nCount], 1.0f);
			aUVpos[3] = D3DXVECTOR2(1.0f + fMove[nCount], 1.0f);

			//2DポリゴンクラスのUV座標設定処理呼び出し
			m_apScene2D[nCount]->SetUV(&aUVpos[0]);
		}
	}
}

//=============================================================================
//背景クラスの描画処理
//=============================================================================
void CBg::Draw(void)
{
}
