//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _ENEMY_H
#define _ENEMY_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "character.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define ENEMY_SIZE_X 70.0f	//エネミーのX軸の大きさ
#define ENEMY_SIZE_Y 70.0f	//エネミーのY軸の大きさ
#define ENEMY_SIZE D3DXVECTOR3(ENEMY_SIZE_X, ENEMY_SIZE_Y, 0.0f)	//エネミーの大きさ
#define ENEMY_MOVE_X -2.0f	//エネミーのX軸の移動量
#define ENEMY_MOVE_Y 2.0f	//エネミーのY軸の移動量
#define ENEMY_MOVE D3DXVECTOR3(ENEMY_MOVE_X, ENEMY_MOVE_Y, 0.0f)	//エネミーの移動量

//=============================================================================
//エネミークラス
//=============================================================================
class CEnemy : public CCharacter
{
public:
	//=========================================================================
	//エネミーの種類
	//=========================================================================
	typedef enum
	{
		ENEMY_TYPE_NO_BULLET = 0,	//弾を撃たない敵
		ENEMY_TYPE_NORMAL,			//普通の敵
		ENEMY_TYPE_SNIPING,			//狙い撃ちの敵
		ENEMY_TYPE_HOMING,			//ホーミング弾の敵
		ENEMY_TYPE_MIDDLEBOSS,		//中ボス
		ENEMY_TYPE_MAX
	}ENEMY_TYPE;

	//=========================================================================
	//エネミーの死亡時の処理情報
	//=========================================================================
	typedef enum
	{
		DEATH_ACTION_NOEN = 0,	//死亡時に何もしない
		DEATH_ACTION_LIFE,		//死亡時に回復アイテムを生成する
		DEATH_ACTION_SKILL,		//死亡時にスキル回復アイテムを生成する
		DEATH_ACTION_BOSS,		//死亡時にボス戦開始の合図を出す
	}DEATH_ACTION;

	//=========================================================================
	//行動パターン
	//=========================================================================
	typedef enum
	{
		ACTION_PATTERN_NOMRAL = 0,	//通常の行動
		ACTION_PATTERN_MOVE_DOWN,	//下へ移動する,
		ACTION_PATTERN_MOVE_UP,		//上へ移動する
		ACTION_PATTERN_STOP,		//停止する
		ACTION_PATTERN_STOP_RETURN,	//停止後、一定時間経過で帰還する
		ACTION_PATTERN_UP_AND_DOWN,	//上下に移動する
		ACTION_PATTERN_STALKER,		//プレイヤーを追跡する
		ACTION_PATTERN_MAX
	}ACTION_PATTERN;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CEnemy(int nPriority = CScene::PRIORITY_ENEMY);
	virtual ~CEnemy();

	static CEnemy *Create(
		const D3DXVECTOR3 pos, 
		const D3DXVECTOR3 size, 
		const ENEMY_TYPE type, 
		const int nLife, 
		const int nScore,
		const ACTION_PATTERN action = ACTION_PATTERN_NOMRAL, 
		DEATH_ACTION DeathAction = DEATH_ACTION_NOEN);

	static HRESULT Load(void);
	static void UnLoad(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Attack(void);
	void ActionUpdate(void);
	void Damage(const int nDamage);
	virtual void DiedProcess(void);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture[ENEMY_TYPE_MAX];	//テクスチャへのポインタ
	int m_nScore;	//スコア
	DEATH_ACTION m_DeathAction;	//死亡時の処理用変数
	int m_nActionCounter;
	ENEMY_TYPE m_type;	//エネミーの種類
	
protected:
	D3DXVECTOR3 m_move;	//移動量
	ACTION_PATTERN m_action;	//行動パターン
	int m_nCounter;	//カウンター
	D3DXVECTOR3 m_pos;	//位置
};
#endif