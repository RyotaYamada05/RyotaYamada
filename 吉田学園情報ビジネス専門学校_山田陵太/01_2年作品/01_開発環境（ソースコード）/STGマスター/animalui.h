//=============================================================================
//
// メイン処理 [animal.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _ANIMALUI_H_
#define _ANIMALUI_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

class CPolygon;

//=============================================================================
//アニマルUIクラス
//=============================================================================
class CAnimalUi : public CScene
{
public:
	CAnimalUi(int nPriority = CScene::PRIORITY_UI);
	~CAnimalUi();

	static CAnimalUi *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CPolygon *GetPolygon(void)const;

private:
	CPolygon *m_Polygon;
};












#endif