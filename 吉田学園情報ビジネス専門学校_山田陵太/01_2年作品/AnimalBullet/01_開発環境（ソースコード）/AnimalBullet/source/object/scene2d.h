//=============================================================================
//
// 2Dポリゴン処理 [scene2d.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _SCENE2D_H
#define _SCENE2D_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define POLIGON_SIZE_X 100.0f	//ポリゴンのX軸の大きさ
#define POLIGON_SIZE_Y 100.0f	//ポリゴンのY軸の大きさ
#define POLYGON_SIZE D3DXVECTOR3(POLIGON_SIZE_X, POLIGON_SIZE_Y, 0.0f)	//ポリゴンの大きさ

//============================================================================
//2Dポリゴンクラス
//=============================================================================
class CScene2D : public CScene
{
public:
	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CScene2D(int nPriority = CScene::PRIORITY_DEFAULT);
	virtual ~CScene2D();

	static CScene2D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nPriority = CScene::PRIORITY_DEFAULT);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	static CScene2D *JudgeCollision(CScene::OBJTYPE objtype, D3DXVECTOR3 pos, D3DXVECTOR3 size);
	
	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void)const;
	void SetSize(const D3DXVECTOR3 size);
	D3DXVECTOR3 GetSize(void)const;
	void SetUV(const D3DXVECTOR2 *UVpos);
	void SetColor(const D3DXCOLOR col);
	D3DXCOLOR GetColor(void)const;
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	
private:
	void VertexUpdate(void);
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;	//ポリゴンの位置
	D3DXVECTOR3 m_size;	//ポリゴンのサイズ
	D3DXCOLOR m_col;	//ポリゴンの色
};

#endif