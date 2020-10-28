//=============================================================================
//
// メイン処理 [exeplosion.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "explosion.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"
#include "sound.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

//=============================================================================
//エクスプロージョンクラスのコンストラクタ
//=============================================================================
CExplosion::CExplosion(int nPriority) :CScene2D(nPriority)
{
	//メンバ変数のクリア
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//=============================================================================
//エクスプロージョンクラスのデストラクタ
//=============================================================================
CExplosion::~CExplosion()
{
}

//=============================================================================
//エクスプロージョンクラスのクリエイト処理
//=============================================================================
CExplosion * CExplosion::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col)
{
	//エクスプロージョンクラスのポインタ
	CExplosion *pExeplosion = NULL;

	//インスタンス生成
	pExeplosion = new CExplosion;

	//メモリが確保できていたら
	if (pExeplosion != NULL)
	{
		//初期化処理呼び出し
		pExeplosion->Init(pos, size, col);
	}

	return pExeplosion;
}

//=============================================================================
//エクスプロージョンクラスのテクスチャロード処理
//=============================================================================
HRESULT CExplosion::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/explosion.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//エクスプロージョンクラスのテクスチャ破棄処理
//=============================================================================
void CExplosion::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		//メモリの削除
		m_pTexture->Release();

		//メモリのクリア
		m_pTexture = NULL;
	}
}

//=============================================================================
//エクスプロージョンクラスの初期化処理
//=============================================================================
HRESULT CExplosion::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col)
{
	//メンバ変数の初期化
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;

	//2Dポリゴンクラスの初期化処理呼び出し
	CScene2D::Init(pos, size);

	//2Dポリゴンクラスの色設定処理呼び出し
	CScene2D::SetColor(col);

	//テクスチャ割り当て処理呼び出し
	CScene2D::BindTexture(m_pTexture);

	//オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_EXPLOSION);

	//UV座標用の変数
	D3DXVECTOR2 aUVpos[4];

	//UV座標の定義
	aUVpos[0] = D3DXVECTOR2(0, 0.0f);
	aUVpos[1] = D3DXVECTOR2(0.125f, 0.0f);
	aUVpos[2] = D3DXVECTOR2(0, 1.0f);
	aUVpos[3] = D3DXVECTOR2(0.125f, 1.0f);

	//2DポリゴンクラスのUV座標設定処理呼び出し
	CScene2D::SetUV(&aUVpos[0]);

	//サウンドクラスの情報取得
	CSound *pSound = CManager::GetSound();

	//爆発音の再生
	pSound->Play(CSound::SOUND_LABEL_SE_EXEPLOSION);

	return S_OK;
}

//=============================================================================
//エクスプロージョンクラスの終了処理
//=============================================================================
void CExplosion::Uninit(void)
{
	//2Dポリゴンクラスの終了処理呼び出し
	CScene2D::Uninit();
}

//=============================================================================
//エクスプロージョンクラスの更新処理
//=============================================================================
void CExplosion::Update(void)
{
	//UV座標用の変数
	D3DXVECTOR2 aUVpos[4];

	//アニメーションカウンタアップ
	m_nCounterAnim++;

	//m_nCounterAnimの余りが4のとき
	if (m_nCounterAnim % 4 == 0)
	{
		//パターンカウンタアップ
		m_nPatternAnim++;

		//UV座標の定義
		aUVpos[0] = D3DXVECTOR2(m_nPatternAnim * 0.125f, 0.0f);
		aUVpos[1] = D3DXVECTOR2(m_nPatternAnim * 0.125f + 0.125f, 0.0f);
		aUVpos[2] = D3DXVECTOR2(m_nPatternAnim * 0.125f, 1.0f);
		aUVpos[3] = D3DXVECTOR2(m_nPatternAnim * 0.125f + 0.125f, 1.0f);

		//2DポリゴンクラスのUV座標設定処理呼び出し
		CScene2D::SetUV(&aUVpos[0]);

		if (m_nPatternAnim >= 8)
		{
			//終了処理呼び出し
			Uninit();
		}
	}
}

//=============================================================================
//エクスプロージョンクラスの描画処理
//=============================================================================
void CExplosion::Draw(void)
{
	//2Dポリゴンクラスの描画処理呼び出し
	CScene2D::Draw();
}
