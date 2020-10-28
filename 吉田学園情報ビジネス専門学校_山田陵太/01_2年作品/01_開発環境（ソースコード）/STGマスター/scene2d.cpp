//=============================================================================
//
// メイン処理 [scene2d.cpp]
// Author : 山田陵太
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//2Dポリゴンクラスのコンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority) :CScene(nPriority)
{
	//各メンバ変数のクリア
	m_pTexture = NULL;	//テクスチャへのポインタ
	m_pVtxBuff = NULL;	//頂点バッファへのポインタ
	m_Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ポリゴンのサイズ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ポリゴンの位置
	m_fAngle = 0.0f;	//中心の角度
	m_fAddScale = 0.05f;	//拡大加算率
	m_fScale = 1.5f;	//拡大
	m_fLength = 0.0f;	//対角線の長さ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き

}

//=============================================================================
//2Dポリゴンクラスのデストラクタ
//=============================================================================
CScene2D::~CScene2D()
{
}

//=============================================================================
//2Dポリゴンクラスの生成処理
//=============================================================================
CScene2D *CScene2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nPriority)
{
	CScene2D *pScene2D;	//2Dポリゴンクラスのポインタ変数
	
	//2Dポリゴンクラスのインスタンス生成
	pScene2D = new CScene2D(nPriority);

	//メモリが確保できていたら
	if (pScene2D != NULL)
	{
		//初期化処理呼び出し
		pScene2D->Init(pos,size);
	}
	return pScene2D;
}


//=============================================================================
//2Dポリゴンクラスの初期化処理
//=============================================================================
HRESULT CScene2D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//初期位置の設定
	m_pos = pos;

	//サイズの設定
	m_Size = size;

	//頂点バッファの作成
	pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX,	//バッファサイズ
		D3DUSAGE_WRITEONLY,	//（固定）
		FVF_VERTEX_2D,	//フォーマット
		D3DPOOL_MANAGED,	//(固定)
		&m_pVtxBuff,
		NULL
	);

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-m_Size.x / 2), m_pos.y + (-m_Size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (-m_Size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);

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
//2Dポリゴンクラスの終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	
	//オブジェクトの破棄
	Release();
}

//=============================================================================
//2Dポリゴンクラスの更新処理
//=============================================================================
void CScene2D::Update(void)
{
	//対角線の計算
	m_fLength = sqrtf((m_Size.x / 2)*(m_Size.x / 2) + (m_Size.y / 2)*(m_Size.y / 2));

	//対角線の角度の計算
	m_fAngle = atan2f(m_Size.y / 2, m_Size.x / 2);

	// 回転
	m_rot.z += D3DX_PI * 0.01f;
	if (m_rot.z >= D3DX_PI)
	{
		m_rot.z -= D3DX_PI * 2.0f;
	}

	// 拡大縮小
	m_fScale += m_fAddScale;
	if (m_fScale >= 3.0f || m_fScale <= 1.0f)
	{
		m_fAddScale *= -1;
	}

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
#if 0
	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-POLIGON_SIZE_X / 2), m_pos.y + (-POLIGON_SIZE_Y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (POLIGON_SIZE_X / 2), m_pos.y + (-POLIGON_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-POLIGON_SIZE_X / 2), m_pos.y + (POLIGON_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (POLIGON_SIZE_X / 2), m_pos.y + (POLIGON_SIZE_Y / 2), 0.0f);

#elif 0
	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x - cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.y = m_pos.y - sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.y = m_pos.y - sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x - cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.y = m_pos.y + sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.y = m_pos.y + sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.z = 0.0f;

#elif 0
	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x - cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	pVtx[0].pos.y = m_pos.y - sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	pVtx[1].pos.y = m_pos.y - sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x - cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	pVtx[2].pos.y = m_pos.y + sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	pVtx[3].pos.y = m_pos.y + sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	pVtx[3].pos.z = 0.0f;

#endif
	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//2Dポリゴンクラスの描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pD3DDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pD3DDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,
		NUM_POLYGON);	//ポリゴン数

	//テクスチャの設定を元に戻す
	pD3DDevice->SetTexture(0, NULL);
}

//=============================================================================
//2Dポリゴンクラスの矩形の当たり判定処理
//=============================================================================
CScene2D *CScene2D::Collision(CScene::OBJTYPE objtype, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	for (int nCntObjNum = 0; nCntObjNum < MAX_OBJ_NUM; nCntObjNum++)
	{
		for (int nCntPrity = 0; nCntPrity < CScene::PRIORITY_MAX; nCntPrity++)
		{
			//オブジェクト情報を取得
			CScene *pScene = GetScene(nCntPrity, nCntObjNum);

			//メモリが確保できていたら
			if (pScene != NULL)
			{
				//オブジェクトタイプを取得
				OBJTYPE targetobjType = pScene->GetObjType();

				//オブジェクトタイプが目的のものだった場合
				if (targetobjType == objtype)
				{
					//2Dポリゴンクラスポインタにキャストし、情報を取得
					CScene2D *pScene2D = (CScene2D*)pScene;

					//ターゲットの位置とサイズ情報を取得
					D3DXVECTOR3 targetPos = pScene2D->GetPos();
					D3DXVECTOR3 targetSize = pScene2D->GetSize();

					//当たり判定
					if (targetPos.x + (targetSize.x / 2) >= pos.x - (size.x / 2) &&
						targetPos.x - (targetSize.x / 2) <= pos.x + (size.x / 2) &&
						targetPos.y + (targetSize.y / 2) >= pos.y - (size.y / 2) &&
						targetPos.y - (targetSize.y / 2) <= pos.y + (size.y / 2))
					{
						return pScene2D;
					}
				}
			}
		}
	}
	return NULL;
}

//=============================================================================
//2Dポリゴンクラスの位置設定処理
//=============================================================================
void CScene2D::SetPos(const D3DXVECTOR3 pos)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//位置の設定
	m_pos = pos;
	
	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を更新
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-m_Size.x / 2), m_pos.y + (-m_Size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (-m_Size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//2Dポリゴンクラスの位置取得処理
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void) const
{
	return m_pos;
}

//=============================================================================
//2Dポリゴンクラスのサイズ取得処理
//=============================================================================
D3DXVECTOR3 CScene2D::GetSize(void) const
{
	return m_Size;
}

//=============================================================================
//2Dポリゴンクラスのサイズ設定処理
//=============================================================================
void CScene2D::SetSize(const D3DXVECTOR3 size)
{
	m_Size = size;

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を更新
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-m_Size.x / 2), m_pos.y + (-m_Size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (-m_Size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//2DポリゴンクラスのUV座標設定処理
//=============================================================================
void CScene2D::SetUV(const D3DXVECTOR2 *UVpos)
{
	//頂点情報へのポインタ
	VERTEX_2D*pVtx;

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = UVpos[0];
	pVtx[1].tex = UVpos[1];
	pVtx[2].tex = UVpos[2];
	pVtx[3].tex = UVpos[3];

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//2Dポリゴンクラスの色の設定処理
//=============================================================================
void CScene2D::SetColor(const D3DXCOLOR col)
{
	//頂点情報へのポインタ
	VERTEX_2D*pVtx;

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//色の設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//2Dポリゴンクラスのテクスチャ割り当て処理
//=============================================================================
void CScene2D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
