//=============================================================================
//
// UI処理 [ui.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define UNDER_UI_SIZE_Y 90.0f			//UNDER_UIのY軸のサイズ
#define UNDER_UI_SIZE D3DXVECTOR3(SCREEN_WIDTH,UNDER_UI_SIZE_Y,0.0f)	//UNDER_UIのサイズ
#define GAUGE_UI_SIZE_X 600.0f	//ゲージUIのX軸の大きさ
#define GAUGE_UI_SIZE_Y 80.0f	//ゲージUIのY軸の大きさ
#define GAUGE_UI_SIZE D3DXVECTOR3(GAUGE_UI_SIZE_X,GAUGE_UI_SIZE_Y,0.0f)	//ゲージUIのサイズ

//=============================================================================
//前方宣言
//=============================================================================
class CLifeber;
class CScore;
class CScene2D;
class CAnimalUi;
class CSkillber;

//=============================================================================
//UIクラス
//=============================================================================
class CUi
{
public:
	//=========================================================================
	//構造体定義
	//=========================================================================
	typedef enum
	{
		UI_TEX_1 = 0, 
		UI_TEX_2,
		UI_TEX_MAX
	}UI_TEX;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CUi();
	~CUi();
	static CUi *Create(void);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore *GetScore(void);
	static CLifeber *GetLifeber(void);
	static CSkillber *GetSkillber(void);
	static CAnimalUi *GetAnimalUI(void);
private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static CScore *m_pScore;	//スコアクラスのポインタ
	static CLifeber *m_pLifeber;	//ライフクラスのポインタ変数
	static CSkillber *m_pSkillber;	//スキルクラスのポインタ変数
	static CAnimalUi *m_pAnimalUI;	//アニマルUIクラスのポインタ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[UI_TEX_MAX];	//テクスチャへのポインタ
};
#endif 