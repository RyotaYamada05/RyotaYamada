//=============================================================================
//
// メイン処理 [enemy.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _ENEMY_H
#define _ENEMY_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define ENEMY_SIZE_X 70.0f	//エネミーのX軸の大きさ
#define ENEMY_SIZE_Y 70.0f	//エネミーのY軸の大きさ
#define ENEMY_SIZE D3DXVECTOR3(ENEMY_SIZE_X, ENEMY_SIZE_Y, 0.0f)	//エネミーの大きさ
#define ENEMY_MOVE_X 3.0f	//エネミーのX軸の移動量
#define ENEMY_MOVE_Y 3.0f	//エネミーのY軸の移動量

//=============================================================================
//エネミークラス
//=============================================================================
class CEnemy : public CScene2D
{
public:
	//=========================================================================
	//列挙型定義
	//=========================================================================
	typedef enum
	{
		ENEMY_STATE_NORMAL = 0,	//通常状態
		ENEMY_STATE_DAMAGE,	//被弾状態
		ENEMY_STATE_DIED,	//死亡状態
		ENEMY_STATE_MAX,
	}ENEMY_STATE;

	typedef enum
	{
		ENEMY_TYPE_NO_BULLET = 0,
		ENEMY_TYPE_NORMAL,	//普通の敵
		ENEMY_TYPE_SNIPING,	//狙い撃ちの敵
		ENEMY_TYPE_HOMING,	//ホーミング弾の敵
		ENEMY_TYPE_HADE,
		ENEMY_TYPE_DIVISION,	//分裂弾の敵
		ENEMY_TYPE_STOKER,
		ENEMY_TYPE_MAX
	}ENEMY_TYPE;

	typedef enum
	{
		Dead_Process_NOEN = 0,
		Dead_Process_LIFE,
		Dead_Process_SKILL,
		Dead_Process_BOSS,
	}Dead_Process;

	//=========================================================================
	//行動パターン
	//=========================================================================
	typedef enum
	{
		ACTION_PATTERN_NOMRAL = 0,	//通常の行動
		ACTION_PATTERN_MOVE_DOWN,	//下へ移動する,
		ACTION_PATTERN_MOVE_UP,	//上へ移動する
		ACTION_PATTERN_STOP,
		ACTION_PATTERN_STOP_RETURN,
		ACTION_PATTERN_UP_AND_DOWN,
		ACTION_PATTERN_STALKER,	//追跡する
		ACTION_PATTERN_MAX
	}ACTION_PATTERN;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CEnemy(int nPriority = CScene::PRIORITY_ENEMY);
	~CEnemy();

	static CEnemy*Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const ENEMY_TYPE type,
		const ACTION_PATTERN action = ACTION_PATTERN_NOMRAL, Dead_Process DeadProcess = Dead_Process_NOEN);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const ENEMY_TYPE type,
		const ACTION_PATTERN action, const Dead_Process DeadProcess);
	void Uninit(void);
	void Update(void);
	void NormalUpdate(void);
	void SnipingUpdate(void);
	void HadeUpdate(void);
	void DivisionUpdate(void);


	void ActionUpdate(void);
	void Draw(void);
	void SetData(void);
	void Damage(const int nDamage);
	bool DecisionState(void);
	D3DXVECTOR3 GetPos(void)const;

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture[ENEMY_TYPE_MAX];	//テクスチャへのポインタ
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_move;	//移動量
	D3DXVECTOR3 m_size;	//サイズ
	int m_nLife;	//ライフ
	int m_nCounter;
	int m_nScore;	//スコア
	ENEMY_STATE m_State;	//エネミーの状態
	Dead_Process m_DeadProcess;	//死亡時の処理用変数
	int m_nStateCounter;
	int m_nActionCounter;
	ENEMY_TYPE m_type;	//エネミーの種類
	ACTION_PATTERN m_action;	//行動パターン
	float m_fSpeed;	//速度
	float m_fDistance;	//距離
};
#endif