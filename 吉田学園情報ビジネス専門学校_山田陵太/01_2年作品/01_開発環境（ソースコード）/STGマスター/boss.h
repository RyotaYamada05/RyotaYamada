//=============================================================================
//
// メイン処理 [boss.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_
#include "scene2d.h"

class CBoss :public CScene2D
{
public:
	typedef enum
	{
		BOSS_STATE_NOEN = 0,
		BOSS_STATE_DAMAGE,
		BOSS_STATE_DIED,
		BOSS_STATE_MAX
	}BOSS_STATE;

	typedef enum
	{
		ACTION_PATTERN_STAERT = 0, 
		ACTION_PATTERN_NOMRAL,	//通常の行動
		ACTION_PATTERN_MOVE_DOWN,	//下へ移動する,
		ACTION_PATTERN_MOVE_UP,	//上へ移動する
		ACTION_PATTERN_STOP,
		ACTION_PATTERN_UP_AND_DOWN,
		ACTION_PATTERN_STALKER,	//追跡する
		ACTION_PATTERN_ACCELEU,
		ACTION_PATTERN_MAX
	}ACTION_PATTERN;

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
	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_size;
	BOSS_STATE m_state;
	ACTION_PATTERN m_action;
	int m_nLife;
	int m_nCount;
	int m_nScore;
	int m_nActionCounter;
	int m_nStateCounter;
};

#endif 