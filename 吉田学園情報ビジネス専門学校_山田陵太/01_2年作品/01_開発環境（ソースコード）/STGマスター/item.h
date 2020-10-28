//=============================================================================
//
// メイン処理 [item.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene2d.h"

#define ITEM_SIZE_X 40.0f
#define ITEM_SIZE_Y 40.0f
#define ITEM_SIZE D3DXVECTOR3(ITEM_SIZE_X,ITEM_SIZE_Y,0.0f)
//=============================================================================
//アイテムクラス
//=============================================================================
class CItem : public CScene2D
{
public:
	//=========================================================================
	//列挙型定義
	//=========================================================================
	typedef enum
	{
		ITEM_TYPE_LIFE = 0, 
		ITEM_TYPE_SKILL,
		ITEM_TYPE_MAX,
	}ITEM_TYPE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CItem(int nPriority = CScene::PRIORITY_DEFAULT);
	~CItem();

	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, ITEM_TYPE type);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, ITEM_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_apTexture[ITEM_TYPE_MAX];	//テクスチャのポインタ変数
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_size;	//サイズ
	ITEM_TYPE m_type;	//種類
	int m_nLife;	//ライフ
};
#endif