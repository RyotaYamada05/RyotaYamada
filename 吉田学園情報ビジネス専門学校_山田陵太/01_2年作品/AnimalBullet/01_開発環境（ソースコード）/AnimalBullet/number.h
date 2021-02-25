//=============================================================================
//
// ナンバー処理 [number.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define NUMBER_SIZE_X 25.0f	//ナンバーのX軸の大きさ
#define NUMBER_SIZE_Y 50.0f	//ナンバーのY軸の大きさ
#define NUMBER_SIZE D3DXVECTOR3(NUMBER_SIZE_X,NUMBER_SIZE_Y,0.0f)	//ナンバーの大きさ

//=============================================================================
//ナンバークラス
//=============================================================================
class CNumber
{
public:
	//=========================================================================
	//列挙型定義
	//=========================================================================
	typedef enum
	{
		NUMBER_TYPE_000 = 0,
		NUMBER_TYPE_001,
		NUMBER_TYPE_MAX
	}NUMBER_TYPE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CNumber();
	~CNumber();

	static CNumber *Create(const int nNumber, const NUMBER_TYPE Type, const D3DXVECTOR3 pos, const D3DXVECTOR3 size,const D3DXCOLOR color);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const int nNumber, const NUMBER_TYPE Type, const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR color);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(const int nNumber);
	void SetColr(const D3DXCOLOR col);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture[NUMBER_TYPE_MAX];	//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	int m_nNumber;	//ナンバー
	D3DXVECTOR3 m_size;	//ポリゴンのサイズ
	D3DXVECTOR3 m_pos;	//ナンバーの位置
	NUMBER_TYPE m_type;	//種類
};
#endif 