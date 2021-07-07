//=============================================================================
//
// キャラクター処理 [character.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene2d.h"

//=============================================================================
//キャラクターの状態
//=============================================================================
typedef enum
{
	CHARACTER_STATE_NORMAL = 0,	//通常状態
	CHARACTER_STATE_DAMAGE,		//被弾状態
	CHARACTER_STATE_DIED,		//死亡状態
	CHARACTER_STATE_MAX,
}CHARACTER_STATE;

//=========================================================================
//マクロ定義
//=========================================================================
#define STATE_NORMAL(state)(((CHARACTER_STATE)(state)) == CHARACTER_STATE_NORMAL)	//状態がCHARACTER_STATE_NORMALか確認するマクロ
#define STATE_DAMAGE(state)(((CHARACTER_STATE)(state)) == CHARACTER_STATE_DAMAGE)	//状態がCHARACTER_STATE_DAMAGEか確認するマクロ
#define STATE_DEATH(state)(((CHARACTER_STATE)(state)) == CHARACTER_STATE_DIED)		//状態がCHARACTER_STATE_DIEDか確認するマクロ

//=============================================================================
//キャラクタークラス
//=============================================================================
class CCharacter :public CScene2D
{
public:
	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CCharacter(int nPriority);
	~CCharacter();

protected:
	CHARACTER_STATE CheckState(void);
	void SetState(CHARACTER_STATE state);
	CHARACTER_STATE GetState(void);
	virtual void DiedProcess(void) = 0;
	void InitLife(int nDefaultLife);
	int AddLife(const int nAddValue);
	int SubLife(const int nSubValue);
	void SetLife(const int nLife);
	void SetMaxLife(const int nMaxLife);

public:
	int GetLife(void);
	int GetMaxLife(void);

	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
private:
	CHARACTER_STATE m_state;	//状態
	int m_nLife;				//体力
	int m_nMaxLife;				//最大体力
	int m_nStateCounter;		//状態カウンター
};
#endif