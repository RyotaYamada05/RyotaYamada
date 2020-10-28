//=============================================================================
//
// メイン処理 [bome.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _BOME_H_
#define _BOME_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene2d.h"
#define BOME_SIZE_X 50.0f
#define BOME_SIZE_Y 50.0f
#define BOME_SIZE D3DXVECTOR3(BOME_SIZE_X,BOME_SIZE_Y,0.0f)
//=============================================================================
//ボムクラス
//=============================================================================
class CBome : public CScene2D
{
public:
	//=========================================================================
	//列挙型定義
	//=========================================================================
	typedef enum
	{
		BOME_STATE_NORMAL = 0, 
		BOME_STATE_EXPLOSION,
		BOME_STATE_MAX,
	}BOME_STATE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CBome(int nPriority = CScene::PRIORITY_BULEET);
	~CBome();

	static CBome *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_move;	//移動量
	D3DXVECTOR3 m_size;	//サイズ
	D3DXCOLOR m_col;	//色
	BOME_STATE m_state;	//状態
	int m_nCounter;	//カウンター
};

#endif