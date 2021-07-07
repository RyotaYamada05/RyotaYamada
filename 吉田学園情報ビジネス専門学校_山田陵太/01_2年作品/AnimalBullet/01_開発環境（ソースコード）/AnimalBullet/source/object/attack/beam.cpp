//=============================================================================
//
// ビーム処理 [beam.h]
// Author : 山田陵太
//
//=============================================================================
#include "beam.h"
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "boss.h"
#include "explosion.h"
#include "sound.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define TEX_MOVE_RATE 0.05f

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBeam::m_pTexture = NULL;

//=============================================================================
//ビームクラスのコンストラクタ
//=============================================================================
CBeam::CBeam(int nPriority):CScene(nPriority)
{
	//各メンバ変数のクリア
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntLimit = 0;
	m_fTexMove = 0.0f;
}

//=============================================================================
//ビームクラスのデストラクタ
//=============================================================================
CBeam::~CBeam()
{
}

//=============================================================================
//ビームクラスのクリエイト処理
//=============================================================================
CBeam * CBeam::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,
	 const int nLimit)
{
	//ビームクラスのポインタ変数
	CBeam *pBeam = NULL;

	//インスタンス生成
	pBeam = new CBeam;

	//メモリが確保できていたら
	if (pBeam)
	{
		pBeam->SetLimit(nLimit);
		pBeam->m_pos = pos;
		pBeam->m_size = size;
		//初期化処理呼び出し
		pBeam->Init();
	}
	else
	{
		return NULL;
	}

	return pBeam;
}

//=============================================================================
//ビームクラスのテクスチャロード処理
//=============================================================================
HRESULT CBeam::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/Beam.png", &m_pTexture);

	return S_OK;
}
//=============================================================================
//ビームクラスのテクスチャの破棄処理
//=============================================================================
void CBeam::UnLoad(void)
{
	//テクスチャの破棄
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//ビームクラスの初期化処理
//=============================================================================
HRESULT CBeam::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

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
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + (m_size.y / 2), 0.0f);

	//rhwの設定（値は1.0で固定）
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	//ビーム音の再生
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BEAM);

	return S_OK;
}

//=============================================================================
//ビームクラスの終了処理
//=============================================================================
void CBeam::Uninit(void)
{
	//ビーム音の停止
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_SE_BEAM);

	//頂点バッファの開放
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクトの破棄
	SetDeathFlag();
}

//=============================================================================
//ビームクラスの更新処理
//=============================================================================
void CBeam::Update(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//プレイヤーの位置取得
	m_pos = CGame::GetPlayer()->GetPos();

	//制限時間カウンターが0以下になったら
	if (m_nCntLimit <= 0)
	{
		Uninit();

		return;
	}
	else if (m_nCntLimit <= 20)
	{
		//サイズを小さくする
		m_size.y -= 1.0f;
	}

	m_fTexMove += TEX_MOVE_RATE;

	//当たり判定
	Collision();

	//制限時間カウンターの減少
	m_nCntLimit--;

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を更新
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + (m_size.y / 2), 0.0f);

	//テクスチャ座標を更新
	pVtx[0].tex = D3DXVECTOR2(0.0f - m_fTexMove, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f - m_fTexMove, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f - m_fTexMove, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f - m_fTexMove, 1.0f);

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//ビームクラスの描画処理
//=============================================================================
void CBeam::Draw(void)
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
//ビームクラスの当たり判定処理
//=============================================================================
void CBeam::Collision(void)
{
	//エネミーと当たったかどうか情報を取得
	CEnemy *pEnemy = (CEnemy*)GetCollision(CScene2D::OBJTYPE_ENEMY);

	if (pEnemy)
	{
		//エネミーの位置取得
		D3DXVECTOR3 enemyPos = pEnemy->GetPos();

		//ダメージを与える
		pEnemy->Damage(1);

		pEnemy = NULL;

		//爆発の生成
		CExplosion::Create(enemyPos, EXPLOSION_SIZE*2.5f, COLOR_GREEN);
	}

	//ボスと当たったかどうか情報を取得
	CBoss *pBoss = (CBoss*)GetCollision(CScene2D::OBJTYPE_BOSS);

	if (pBoss)
	{
		//ボスの位置取得
		D3DXVECTOR3 bossPos = pBoss->GetPos();

		//ダメージを与える
		pBoss->Damage(1);

		pBoss = NULL;

		//爆発の生成
		CExplosion::Create(bossPos, EXPLOSION_SIZE, COLOR_GREEN);
	}

	//バレットと当たったかどうか情報を取得
	CBullet *pBullet = (CBullet*)GetCollision(CScene2D::OBJTYPE_BULLET);

	if (pBullet)
	{
		//バレットの使用者が敵のもののみ判定を実行する
		if (pBullet->GetUser() == CBullet::BULLET_USER_ENEMY)
		{
			//バレットの位置取得
			D3DXVECTOR3 bulletPos = pBullet->GetPos();

			//バレットを削除
			pBullet->Uninit();
			pBullet = NULL;

			//爆発の生成
			CExplosion::Create(bulletPos, EXPLOSION_SIZE, COLOR_GREEN);
		}
	}
}

//=============================================================================
//ビームクラスの制限時間の設定の処理
//=============================================================================
void CBeam::SetLimit(const int nLimit)
{
	m_nCntLimit = nLimit;
}

//=============================================================================
//ビームクラスの当たり情報の取得の処理
//=============================================================================
CScene2D *CBeam::GetCollision(OBJTYPE objtype)
{
	for (int nCntPrity = 0; nCntPrity < CScene::PRIORITY_MAX; nCntPrity++)
	{
		//リストの先頭情報の取得
		CScene* pScene = CScene::GetTop(nCntPrity);

		//NULLになるまで繰り返す
		while (pScene)
		{
			OBJTYPE type = pScene->GetObjType();

			//オブジェクトタイプが目的のものだった場合
			if (type == objtype)
			{
				//2Dポリゴンクラスポインタにキャストし、情報を取得
				CScene2D *pScene2D = (CScene2D*)pScene;

				//ターゲットの位置とサイズ情報を取得
				D3DXVECTOR3 targetPos = pScene2D->GetPos();
				D3DXVECTOR3 targetSize = pScene2D->GetSize();

				//当たり判定
				if (targetPos.x + (targetSize.x / 2) >= m_pos.x &&
					targetPos.x - (targetSize.x / 2) <= m_pos.x + m_size.x &&
					targetPos.y + (targetSize.y / 2) >= m_pos.y - (m_size.y / 2) &&
					targetPos.y - (targetSize.y / 2) <= m_pos.y + (m_size.y / 2))
				{
					return pScene2D;
				}
			}
			//次の情報を取得
			pScene = pScene->GetNext();
		}
	}
	return NULL;
}
