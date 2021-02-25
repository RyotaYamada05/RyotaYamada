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
#include "scene2d.h"

//=============================================================================
//ボスクラス
//=============================================================================
class CBoss :public CScene2D
{
public:
	//=========================================================================
	//ボス状態の列挙型
	//=========================================================================
	typedef enum
	{
		BOSS_STATE_NORMAL = 0,	//通常状態
		BOSS_STATE_DAMAGE,	//ダメージ状態
		BOSS_STATE_DIED,	//死亡状態
		BOSS_STATE_MAX
	}BOSS_STATE;

	typedef enum
	{
		ACTION_PATTERN_STAERT = 0,	//START状態の行動
		ACTION_PATTERN_NOMRAL,	//通常の行動
		ACTION_PATTERN_MOVE_DOWN,	//下へ移動する,
		ACTION_PATTERN_MOVE_UP,	//上へ移動する
		ACTION_PATTERN_STOP,	//停止する
		ACTION_PATTERN_UP_AND_DOWN,	//上下に移動する
		ACTION_PATTERN_STALKER,	//追跡する
		ACTION_PATTERN_ACCELEU,	//加速
		ACTION_PATTERN_MAX
	}ACTION_PATTERN;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CBoss(int nPriority = CScene::PRIORITY_ENEMY);
	~CBoss();

	static CBoss *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Action(void);
	void Damage(const int nDamage);
	bool DecisionState(void);
	D3DXVECTOR3 VectorMath(D3DXVECTOR3 TargetPos, float fSpeed);
	
private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_move;	//移動量
	D3DXVECTOR3 m_size;	//サイズ
	BOSS_STATE m_state;	//状態
	ACTION_PATTERN m_action;	//行動パターン
	int m_nLife;	//ライフ
	int m_nCount;	//カウンター
	int m_nScore;	//スコア
	int m_nActionCounter;	//アクションカウンター
	int m_nStateCounter;	//状態カウンター
};

#endif 