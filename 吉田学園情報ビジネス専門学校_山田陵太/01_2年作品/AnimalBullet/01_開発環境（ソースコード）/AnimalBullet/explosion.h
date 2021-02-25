//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _EXPLOSOIN_H_
#define _EXPLOSOIN_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define EXPLOSION_SIZE_X 50.0f	//爆発のX軸の大きさ
#define EXPLOSION_SIZE_Y 50.0f	//爆発のY軸の大きさ
#define EXPLOSION_SIZE D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f)

//=============================================================================
//エクスプロージョンクラス
//=============================================================================
class CExplosion : public CScene2D
{
public:
	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CExplosion(int nPriority = PRIORITY_EXPLOSION);
	~CExplosion();

	static CExplosion *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col, int nPriority = PRIORITY_EXPLOSION);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャのポインタ
	int m_nCounterAnim;	//アニメーションカウンター
	int m_nPatternAnim;	//アニメーションパターン
};

#endif