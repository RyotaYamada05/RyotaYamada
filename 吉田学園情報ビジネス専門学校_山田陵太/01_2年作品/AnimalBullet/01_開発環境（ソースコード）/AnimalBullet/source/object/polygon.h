//=============================================================================
//
// ポリゴン処理 [polygon.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#define MAX_ALPHA 1.0f	//α値の最大値
#define MIN_ALPHA 0.0f	//α値の最低値
//=============================================================================
//ライフクラス
//=============================================================================
class CPolygon
{
public:
	//=========================================================================
	//列挙型定義
	//=========================================================================
	typedef enum
	{
		TEX_TYPE_NORE = 0,
		TEX_TYPE_TITLE,	//タイトルテクスチャ
		TEX_TYPE_TITLE_LOGO,
		TEX_TYPE_RESULET,	//リザルトテクスチャ
		TEX_TYPE_UNDERUI,	//アンダーUIテクスチャ
		TEX_TYPE_CHEETAH,	//チーター
		TEX_TYPE_GORILLA,	//ゴリラ
		TEX_TYPE_TURTLE,	//カメ
		TEX_TYPE_PRESSENTER,	//PRESS ENTERテクスチャ
		TEX_TYPE_WARNING,	//WARNINGテクスチャ
		TEX_TYPE_GAME_CLEAR,//GAME_CLEARテクスチャ
		TEX_TYPE_GAME_OVER,	//GAME_OVERテクスチャ
		TEX_TYPE_MAX,	//最大テクスチャ
	}TEX_TYPE;

	//=========================================================================
	//点滅処理用の列挙型定義
	//=========================================================================
	typedef enum
	{
		FLASHING_NONE = 0,
		FLASHING_ADD,	//加算
		FLASHING_SUB,	//減算
		FLASHING_MAX,	//最大
	}FLASHING;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CPolygon();
	~CPolygon();

	static CPolygon *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const TEX_TYPE type);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const TEX_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(const TEX_TYPE type);
	void SetColor(const D3DXCOLOR color);
	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void);
	void Flashing(void);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture[TEX_TYPE_MAX];	//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3 m_size;	//ポリゴンのサイズ
	D3DXVECTOR3 m_pos;	//ポリゴンの位置
	D3DXCOLOR m_col;	//色
	TEX_TYPE m_type;	//種類
	FLASHING m_Flashing;	//点滅情報
};
#endif 