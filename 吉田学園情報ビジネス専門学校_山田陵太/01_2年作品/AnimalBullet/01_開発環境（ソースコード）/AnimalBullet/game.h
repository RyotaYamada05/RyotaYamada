//=============================================================================
//
// ゲーム処理 [game.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
//前方宣言
//=============================================================================
class CUi;
class CPlayer;
class CPolygon;

//=============================================================================
//ゲームクラス
//=============================================================================
class CGame : public CScene
{
public:
	//=========================================================================
	//ゲーム状態判定用列挙型定義
	//========================================================================
	typedef enum
	{
		GAME_STATE_NOMRAL = 0,	//通常ゲーム
		GAME_STATE_BOSS_START,	//ボス戦開始前処理
		GAME_STATE_BOSS_BATTLE,	//ボス戦
		GAME_STATE_GAME_CLEAR,	//ゲームクリア処理
		GAME_STATE_GAME_OVER,	//ゲームオーバー処理
		GAME_STATE_MAX,	//ゲーム状態の最大数
	}GAME_STATE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CGame(int nPriority = CScene::PRIORITY_EXPLOSION);
	~CGame();

	static CGame *Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetGameState(const GAME_STATE state);
	static GAME_STATE GetGameState(void);
	static CUi *GetUi(void);
	static CPlayer *GetPlayer(void);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static CUi *m_pUi;	//UIクラスのポインタ
	static CPlayer *m_pPlayer;	//プレイヤークラスのポインタ変数
	static CPolygon *m_pPolygon;	//ポリゴンクラスのポインタ変数
	static GAME_STATE m_state;	//ゲーム状態の変数
	int m_nEnemyCounter;	//エネミー生成用カウンター
	int m_nStaeCounter;	//状態用カウンター
};
#endif