//=============================================================================
//
// メイン処理 [player.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define PLAYER_SIZE_X 45.0f	//プレイヤーのXサイズ
#define PLAYER_SIZE_Y 45.0f	//プレイヤーのYサイズ
#define PLAYER_SIZE D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, 0.0f)	//プレイヤーのサイズ


//=============================================================================
//前方宣言
//=============================================================================
class CLife;
class CInputKeyboard;
class CInputJoyStick;
//=============================================================================
//プレイヤークラス
//=============================================================================
class CPlayer : public CScene2D
{
public:
	//=========================================================================
	//プレイヤーの状態判定用の列挙型定義
	//=========================================================================
	typedef enum
	{
		PLAYER_STATE_NORMAL = 0,	//通常状態
		PLAYER_STATE_DAMAGE,	//被弾状態
		PLAYER_STATE_DIED,	//死亡状態
	}PLAYER_STATE;

	//=========================================================================
	//プレイヤーの動物タイプの列挙型定義
	//=========================================================================
	typedef enum
	{
		ANIMAL_TYPE_NONE = 0,
		ANIMAL_TYPE_CHEETAH,	//チーター
		ANIMAL_TYPE_GORILLA,	//ゴリラ
		ANIMAL_TYPE_TURTLE,	//カメ
	}ANIMAL_TYPE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CPlayer(int nPriority = CScene::PRIORITY_PLAYER);
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Moving(void);
	void ChangeAnimal(void);
	void TypebySkil(void);
	void ShotBullet(void);
	void Damage(const int nAttack);
	void Recovery(const int nRecovery);
	bool DecisionState(void);
	D3DXVECTOR3 GetPos(void)const;
	static ANIMAL_TYPE GetAnimal(void);
	
private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_move;	//移動量
	PLAYER_STATE m_State;	//プレイヤーの状態
	CInputKeyboard *m_pKeyBoard;
	CInputJoyStick *m_pJoyStick;
	static ANIMAL_TYPE m_Animal;	//動物のタイプ
	bool m_bSkill;
	int m_nLife;	//プレイヤーの体力
	int m_nSkillCounter;	//スキルカウンタ
	int m_nStateCounter;	//ダメージカウンター
	int m_nShotCounter;	//発射カウンター
	int m_nDefense;	//防御力
};

#endif