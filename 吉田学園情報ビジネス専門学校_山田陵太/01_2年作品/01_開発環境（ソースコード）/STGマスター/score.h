//=============================================================================
//
// メイン処理 [score.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define SCORE_MAX_NUM 8	//スコアの最大桁数

//=============================================================================
//前方宣言
//=============================================================================
class CNumber;

//=============================================================================
//スコアクラス
//=============================================================================
class CScore : public CScene
{
public:
	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CScore(int nPriority = CScene::PRIORITY_UI);
	~CScore();

	static CScore *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddScore(const int nScore); 
	static int GetScore(void);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static int m_nScore;	//スコア
	CNumber *m_apNumber[SCORE_MAX_NUM];	//ナンバークラスポインタ
};
#endif 