//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "game.h"
#include "ui.h"
#include "skill.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define ITEM_MOVE 2.5f	//アイテムの移動量

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CItem::m_apTexture[CItem::ITEM_TYPE_MAX] = {};

//=============================================================================
//アイテムクラスのコンストラクタ
//=============================================================================
CItem::CItem(int nPriority) : CScene2D(nPriority)
{
	//各メンバ変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = ITEM_TYPE_LIFE;
}

//=============================================================================
//アイテムクラスのデストラクタ
//=============================================================================
CItem::~CItem()
{
}

//=============================================================================
//アイテムクラスのクリエイト処理
//=============================================================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, ITEM_TYPE type)
{
	//アイテムクラスのポインタ
	CItem *pItem = NULL;

	//インスタンス生成
	pItem = new CItem;

	//メモリ確保できていたら
	if (pItem != NULL)
	{
		//初期化処理実行
		pItem->Init(pos, size, type);
	}
	//失敗していたら
	else
	{
		return NULL;
	}

	return pItem;
}

//=============================================================================
//アイテムクラスのテクスチャロード処理
//=============================================================================
HRESULT CItem::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Life2.png", &m_apTexture[ITEM_TYPE_LIFE]);	//ライフ
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/Skill.png", &m_apTexture[ITEM_TYPE_SKILL]);	//スキル
	
	return S_OK;
}

//=============================================================================
//アイテムクラスのテクスチャ破棄処理
//=============================================================================
void CItem::UnLoad(void)
{
	for (int nCount = 0; nCount < ITEM_TYPE_MAX; nCount++)
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
//アイテムクラスの初期化処理
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, ITEM_TYPE type)
{
	//位置の設定
	m_pos = pos;

	//サイズの設定
	m_size = size;

	//種類の設定
	m_type = type;

	//2Dポリゴンクラスの初期化処理呼び出し
	CScene2D::Init(m_pos, m_size);

	//テクスチャの設定
	CScene2D::BindTexture(m_apTexture[m_type]);

	//オブジェクトタイプの設定
	CScene::SetObjType(OBJTYPE_ITEM);

	return E_NOTIMPL;
}

//=============================================================================
//アイテムクラスの終了処理
//=============================================================================
void CItem::Uninit(void)
{
	//2Dポリゴンクラスの終了処理呼び出し
	CScene2D::Uninit();
}

//=============================================================================
//アイテムクラスの更新処理
//=============================================================================
void CItem::Update(void)
{
	//位置の取得
	m_pos = CScene2D::GetPos();

	//プレイヤーとあったかどうかの情報を取得
	CPlayer *pPlayer = (CPlayer*)CScene2D::GetCollision(CScene2D::OBJTYPE_PLAYER, m_pos, m_size);

	if (pPlayer != NULL)
	{
		switch (m_type)
		{
		//ライフアイテムだった場合
		case ITEM_TYPE_LIFE:
			//プレイヤーの体力を回復
			CGame::GetPlayer()->Recovery(25);

			//終了処理呼び出し
			Uninit();

			return;
			break;

		//スキルアイテムだった場合
		case ITEM_TYPE_SKILL:
			//スキルポイントを加算
			CGame::GetUi()->GetSkill()->AddSkill(50);

			//終了処理呼び出し
			Uninit();
			return;

			break;
		}
	}

	//移動量を加算
	m_pos.x -= ITEM_MOVE;

	//位置の設定
	CScene2D::SetPos(m_pos);

	//アイテムが画面外に行ったら
	if (m_pos.x <= -100)
	{
		//終了処理呼び出し
		Uninit();
		return;
	}
}

//=============================================================================
//アイテムクラスの描画処理
//=============================================================================
void CItem::Draw(void)
{
	//2Dポリゴンクラスの描画処理呼び出し
	CScene2D::Draw();
}
