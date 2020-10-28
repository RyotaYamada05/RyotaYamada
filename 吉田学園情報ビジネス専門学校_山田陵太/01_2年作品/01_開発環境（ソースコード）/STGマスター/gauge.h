//=============================================================================
//
// メイン処理 [gauge.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//ゲージクラス
//=============================================================================
class CGauge
{
public:
	//=========================================================================
	//ゲージタイプの列挙型
	//=========================================================================
	typedef enum
	{
		GAUGE_TYPE_LIFE_GREEN = 0,	//ライフ用緑ゲージ
		GAUGE_TYPE_LIFE_ORANGE,	//ライフ用オレンジゲージ
		GAUGE_TYPE_LIFE_RED,	//ライフ用赤ゲージ
		GAUGE_TYPE_SKILL,
		GAUGE_TYPE_MAX	//ゲージ最大数
	}GAUGE_TYPE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CGauge();
	~CGauge();

	static CGauge*Create(const D3DXVECTOR3 pos,const D3DXVECTOR3 size,int nGaugeNum, const GAUGE_TYPE type);
	static HRESULT Load(void);
	static void UnLoad(void);
	
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, int nGaugeNum, const GAUGE_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetGauge(const int nGaugeNum);
	void SetType(const GAUGE_TYPE type);
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
private:
	static LPDIRECT3DTEXTURE9 m_apTextuer[GAUGE_TYPE_MAX];	//テクスチャへのポインタ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	int m_nGaugeNum;	//ゲージ
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_size;	//サイズ
	GAUGE_TYPE m_type;	//ゲージタイプ
};


#endif