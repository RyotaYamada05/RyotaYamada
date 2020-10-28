//=============================================================================
//
// メイン処理 [scene2d.h]
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

//=============================================================================
//色のマクロ定義
//=============================================================================
#define COLOR_RED D3DCOLOR_RGBA(255,50,50,255)	//赤色
#define COLOR_BLUE D3DCOLOR_RGBA(50,50,255,255)	//青色
#define COLOR_CYAN D3DCOLOR_RGBA(50,255,255,255)	//シアン
#define COLOR_GREEN D3DCOLOR_RGBA(50,255,50,255)	//緑色
#define COLOR_YELLOW D3DCOLOR_RGBA(255,255,50,255)	//黄色
#define COLOR_PURPLE D3DCOLOR_RGBA(128,50,128,255)	//紫
#define COLOR_BLACK D3DCOLOR_RGBA(0,0,0,255)	//黒色
#define COLOR_WHITE D3DCOLOR_RGBA(255,255,255,255)	//白色
#define COLOR_ALPHA D3DCOLOR_RGBA(255,255,255,50)


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

	virtual HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	static CScene2D *Collision(CScene::OBJTYPE objtype, D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void)const;
	D3DXVECTOR3 GetSize(void)const;
	void SetSize(const D3DXVECTOR3 size);
	void SetUV(const D3DXVECTOR2 *UVpos);
	void SetColor(const D3DXCOLOR col);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	
private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	D3DXVECTOR3 m_pos;	//ポリゴンの位置
	LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3 m_Size;	//ポリゴンのサイズ

	float m_fLength;	// 対角線の長さ
	float m_fAngle;	// 対角線の角度
	D3DXVECTOR3 m_rot;	// 向き
	float m_fScale;	// 拡大
	float m_fAddScale;	// 拡大加算値
};

#endif