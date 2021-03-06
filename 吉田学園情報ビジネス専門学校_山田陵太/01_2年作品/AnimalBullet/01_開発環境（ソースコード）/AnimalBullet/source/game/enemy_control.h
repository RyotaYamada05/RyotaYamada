//=============================================================================
//
// エネミーコントロール処理 [enemy_control.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _ENEMY_CONTROL_H_
#define _ENEMY_CONTROL_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "enemy.h"

//=============================================================================
//エネミーコントロールクラス
//=============================================================================
class CEnemyControl
{
public:
	//=========================================================================
	//エネミーデータの構造体定義
	//=========================================================================
	typedef struct
	{
		int nSpoonTime;						//生成タイム
		D3DXVECTOR3 pos;					//位置
		D3DXVECTOR3 size;					//サイズ
		CEnemy::ENEMY_TYPE enemyType;		//エネミーの種類
		int nLife;							//ライフ
		int nScore;							//スコア
		CEnemy::ACTION_PATTERN action;		//行動パターン
		CEnemy::DEATH_ACTION deathAction;	//死亡時の行動
	}ENEMY_DATA;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CEnemyControl();
	~CEnemyControl();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	HRESULT ReadFile(void);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	ENEMY_DATA *m_pEnemyData;	//エネミーデータ
	int m_nSpoonTimer;			//生成タイマー
	int m_nIndex;				//敵のインデックス
	int m_nMaxEnemyNum;			//敵の総数
};
#endif 