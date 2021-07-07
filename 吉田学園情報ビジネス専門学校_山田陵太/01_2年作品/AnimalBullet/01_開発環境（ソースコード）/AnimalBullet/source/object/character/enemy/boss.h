//=============================================================================
//
// ボス処理 [boss.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "character.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define BOSS_LIFE 10	//ボスの体力
#define BOSS_SCORE 1000	//ボスのスコア
#define BOSS_SIZE_X 210.0f	//ボスのX軸の大きさ
#define BOSS_SIZE_Y 210.0f	//ボスのY軸の大きさ
#define BOSS_SIZE D3DXVECTOR3(BOSS_SIZE_X, BOSS_SIZE_Y, 0.0f)	//ボスの大きさ

//=============================================================================
//ボスクラス
//=============================================================================
class CBoss : public CCharacter
{
public:
	//=========================================================================
	//ボスの攻撃パターンの列挙型
	//=========================================================================
	typedef enum
	{
		BOSS_ATTACK_PATTERN_NOMRAL = 0,		//通常攻撃
		BOSS_ATTACK_PATTERN_ACCELEU_BULLET,	//加速弾での攻撃
	}BOSS_ATTACK_PATTERN;

	//=========================================================================
	//戦闘状態の列挙型
	//=========================================================================
	typedef enum
	{
		BUTTLE_STATE_STRAT_UP = 0,	//戦闘開始準備状態
		BUTTLE_STATE_ON_BUTTLE,		//戦闘状態
	}BUTTLE_STATE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CBoss(int nPriority = CScene::PRIORITY_ENEMY);
	~CBoss();
	
	static CBoss *Create(
		const D3DXVECTOR3 pos,
		const D3DXVECTOR3 size,
		const int nLife,
		const int nScore);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DiedProcess(void);

	void Damage(const int nDamage);
	
	void Attack(void);
	void ChangeAttackPattern(void);
	D3DXVECTOR3 TargetWayVec3Calculation(D3DXVECTOR3 TargetPos, float fSpeed);
	
private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_move;	//移動量
	BUTTLE_STATE m_buttleState;	//戦闘状態
	BOSS_ATTACK_PATTERN m_atkPattern;	//攻撃パターン
	int m_nScore;		//スコア
	int m_nAtkCounter;	//攻撃カウンター
	int m_nChangeCounter;	//攻撃パターンの変更カウンター
	int m_nDiedCounter;		//死亡カウンター
};

#endif 