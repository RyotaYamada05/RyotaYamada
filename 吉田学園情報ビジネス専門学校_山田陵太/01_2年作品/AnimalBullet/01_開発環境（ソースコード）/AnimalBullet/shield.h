//=============================================================================
//
// シールド処理 [shield.cpp]
// Author : 山田陵太
//
//=============================================================================
#ifndef _SHIELD_H_
#define _SHIELD_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define SHILED_SIZE_X 20.0f	//シールドのX軸の大きさ
#define SHILED_SIZE_Y 60.0f	//シールドのY軸の大きさ
#define SHILED_SIZE D3DXVECTOR3(SHILED_SIZE_X,SHILED_SIZE_Y,0.0f)	//シールドの大きさ

//=============================================================================
//シールドクラス
//=============================================================================
class CShield : public CScene2D
{
public:
	//=========================================================================
	//シールドの状態の列挙型定義
	//=========================================================================
	typedef enum
	{
		SHIELD_STATE_NONE = -1,
		SHIELD_STATE_GUARD,	//ガード状態
		SHIELD_STATE_COUNTER_ATK,	//カウンター攻撃状態
	}SHIELD_STATE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CShield(int nPriority = CScene::PRIORITY_EFFECT);
	~CShield();

	static CShield *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Guard(void);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_size;	//サイズ
	SHIELD_STATE m_state;	//状態
	int m_nAtk;	//攻撃力
	int m_nHitCounter;	//攻撃が当たった回数のカウンター
	int m_nTimeLimit;	//処理の制限時間
};
#endif 