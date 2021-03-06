//=============================================================================
//
// タイトル処理 [title.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define TITLE_SIZE D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f)	//タイトルサイズ

//=============================================================================
//前方宣言
//=============================================================================
class CPolygon;
class CEffect;

//=============================================================================
//タイトルクラス
//=============================================================================
class CTitle : public CScene
{
public:
	//=========================================================================
	//タイトルの種類の列挙型定義
	//=========================================================================
	typedef enum
	{
		TITLE_TYPE_BG = 0,
		TITLE_TYPE_LOGO,
		TITLE_TYPE_ENTER,
		TITLE_TYPE_MAX
	}TITLE_TYPE;

	//=========================================================================
	//タイトル状態の列挙型
	//=========================================================================
	typedef enum
	{
		TITLE_STATE_LOGO_MOVE = 0,
		TITLE_STATE_WAIT,
		TITLE_STATE_PUSH_ENTER,
		TITLE_STATE_MAX
	}TITLE_STATE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CTitle(int nPriority = CScene::PRIORITY_BG);
	~CTitle();

	static CTitle *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool DecisionState(void);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	CPolygon *m_apPolygon[TITLE_TYPE_MAX];	//ポリゴンクラスのポインタ変数
	CEffect *m_apEffect[3];	//エフェクトクラスのポインタ変数
	int m_nCounter;	//カウンター
	int m_nCntEffecter;	//エフェクトカウンター
	D3DXCOLOR m_EnterColor;	//エンターの色
	TITLE_STATE m_state;	//状態
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_move;	//移動量
};
#endif 