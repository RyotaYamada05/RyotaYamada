//=============================================================================
//
// 動物UI処理 [animal.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _ANIMALUI_H_
#define _ANIMALUI_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
//前方宣言
//=============================================================================
class CPolygon;

//=============================================================================
//アニマルUIクラス
//=============================================================================
class CAnimalUi : public CScene
{
public:
	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CAnimalUi(int nPriority = CScene::PRIORITY_UI);
	~CAnimalUi();

	static CAnimalUi *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CPolygon *GetPolygon(void)const;

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	CPolygon *m_Polygon;	//ポリゴンクラスのポインタ
};












#endif