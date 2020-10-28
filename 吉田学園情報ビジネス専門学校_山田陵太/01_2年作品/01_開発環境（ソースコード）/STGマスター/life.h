//=============================================================================
//
// メイン処理 [life.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define GAUGE_SIZE_X 600.0f	//ライフのX軸の大きさ
#define GAUGE_SIZE_Y 80.0f	//ライフのY軸の大きさ
#define GAUGE_SIZE D3DXVECTOR3(GAUGE_SIZE_X,GAUGE_SIZE_Y,0.0f)	//ライフのサイズ
#define PLAYER_MAX_LIFE	 100 //プレイヤーの最大体力
#define LIFE_GAGUE_SIZE_X 4.5f
#define LIFE_GAGUE_SIZE_Y 30
#define LIFE_GAGUE_SIZE D3DXVECTOR3(LIFE_GAGUE_SIZE_X,LIFE_GAGUE_SIZE_Y,0.0f)

//=============================================================================
//前方宣言
//=============================================================================
class CGauge;
class CPolygon;

//=============================================================================
//ライフクラス
//=============================================================================
class CLife :public CScene
{
public:
	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CLife(int nPriority = CScene::PRIORITY_UI);
	~CLife();
	static CLife *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetLife(int nLife);
	void AddLife(int nLife);
	void SubLife(int nLife);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	CGauge *m_pGauge;
	CPolygon *m_Polygon;
	int m_nLife;
};
#endif