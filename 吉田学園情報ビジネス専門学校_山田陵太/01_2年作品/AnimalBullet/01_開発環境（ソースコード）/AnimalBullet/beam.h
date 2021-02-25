//=============================================================================
//
// ビーム処理 [beam.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _BEAM_H_
#define _BEAM_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define BEAM_SIZE_X 800.0f	//ビームのX軸の大きさ
#define BEAM_SIZE_Y 40.0f	//ビームのY軸の大きさ
#define BEAM_SIZE D3DXVECTOR3(BEAM_SIZE_X,BEAM_SIZE_Y,0.0f)	//ビームの大きさ

//=============================================================================
//前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
//ビームクラス
//=============================================================================
class CBeam : public CScene
{
public:
	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CBeam(int nPriority = CScene::PRIORITY_BULEET);
	~CBeam();

	static CBeam *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,
		 const int nLimit);

	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Collision(void);
	void SetLimit(const int nLimit);
	CScene2D *GetCollision(OBJTYPE objtype);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_move;	//移動量
	D3DXVECTOR3 m_size;	//サイズ
	int m_nCntLimit;	//制限時間のカウンター
	float m_fTexMove;	//テクスチャアニメ用
};
#endif 