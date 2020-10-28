//=============================================================================
//
// メイン処理 [effect.h]
// Author : 
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define ALPHA_SUBTRACT_VALUE 0.111f	//アルファ減算値
#define SIZE_SUBTRACT_VALUE 0.5f	//サイズ減算値

//=============================================================================
//エフェクトクラス
//=============================================================================
class CEffect :public CScene2D
{
public:
	//=========================================================================
	//エフェクトの種類用の列挙型
	//=========================================================================
	typedef enum
	{
		EFFECT_TYPE_BULLET = 0,	//バレット
		EFFECT_TYPE_CHEETAH,	//チーター
		EFFECT_TYPE_GORILLA,	//ゴリラ
		EFFECT_TYPE_TURTLE,
		EFFECT_TYPE_BOME,	//ボム
		EFFECT_TYPE_TITLE,
		EFFECT_TYPE_MAX
	}EFFECT_TYPE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CEffect(int nPriority = CScene::PRIORITY_EFFECT);
	~CEffect();

	static CEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col, int nLife, const EFFECT_TYPE type, const int nPriority = CScene::PRIORITY_EFFECT);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col, int nLife, const EFFECT_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void UpdateBullet(void);
	void EffectByType(void);
	void BomeUpdate(void);
	void TitleUpdate(void);
	void SetType(const EFFECT_TYPE type);
private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture[EFFECT_TYPE_MAX];	//テクスチャポインタ
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_size;	//サイズ
	D3DXCOLOR m_col;	//色
	EFFECT_TYPE m_type;	//種類
	int m_nLife;	//ライフ
};
#endif