//=============================================================================
//
// ゲージ処理 [gauge.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
//ゲージクラス
//=============================================================================
class CGauge : public CScene
{
public:
	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CGauge(int nPriority = PRIORITY_UI);
	virtual ~CGauge();

	static CGauge*Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col,
		const int nGaugeNum, const int nMaxGaugeNum);
	static HRESULT Load(void);
	static void UnLoad(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetPos(const D3DXVECTOR3 pos);
	void SetSize(const D3DXVECTOR3 size);
	void SetColor(const D3DXCOLOR col);
	void SetGauge(const int nGaugeNum);
	int GetGauge(void)const;
	void SetMaxGauge(const int nMaxGaugeNum);
	int GetMaxGague(void)const;
	void AddGauge(int nAddGauge);
	void SubGauge(int nSubGauge);
	
private:
	void VertexUpdate(void);

	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTextuer;	//テクスチャへのポインタ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	int m_nGaugeNum;	//ゲージ
	int m_nMaxGaugeNum;	//ゲージの最大値
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_size;	//サイズ
	D3DXCOLOR m_col;	//色
};


#endif