//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "character.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define CHEETAH_SIZE_X 80.0f	//チーターのXサイズ
#define CHEETAH_SIZE_Y 55.0f	//チーターのYサイズ
#define CHEETAH_SIZE D3DXVECTOR3(CHEETAH_SIZE_X, CHEETAH_SIZE_Y, 0.0f)	//チーターのサイズ
#define GORILLA_SIZE_X 80.0f	//ゴリラのXサイズ
#define GORILLA_SIZE_Y 80.0f	//ゴリラのYサイズ
#define GORILLA_SIZE D3DXVECTOR3(GORILLA_SIZE_X, GORILLA_SIZE_Y, 0.0f)	//ゴリラのサイズ
#define TURTLE_SIZE_X 60.0f	//カメのXサイズ
#define TURTLE_SIZE_Y 45.0f	//カメのYサイズ
#define TURTLE_SIZE D3DXVECTOR3(TURTLE_SIZE_X, TURTLE_SIZE_Y, 0.0f)	//カメのサイズ
#define MAX_SKILL 100	//スキルの最大値
//=============================================================================
//前方宣言
//=============================================================================
class CLifeber;
class CSkillber;
class CInputKeyboard;
class CInputJoyStick;

//=============================================================================
//プレイヤークラス
//=============================================================================
class CPlayer : public CCharacter
{
public:
	//=========================================================================
	//プレイヤーの動物タイプの列挙型定義
	//=========================================================================
	typedef enum
	{
		ANIMAL_TYPE_NONE = -1,
		ANIMAL_TYPE_CHEETAH,	//チーター
		ANIMAL_TYPE_GORILLA,	//ゴリラ
		ANIMAL_TYPE_TURTLE,	//カメ
		ANIMAL_TYPE_MAX
	}ANIMAL_TYPE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CPlayer(int nPriority = CScene::PRIORITY_PLAYER);
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Damage(const int nAttack);
	void Recovery(const int nRecovery);
	int GetSkillPoint(void)const;
	void AddSkillPoint(const int nAddSkill);
	static ANIMAL_TYPE GetAnimal(void);

private:
	void Moving(void);
	void ChangeAnimal(void);
	void TypebySkil(void);
	void ShotBullet(void);
	void DiedProcess(void);
#ifdef _DEBUG
	void DebugPrint(void);
#endif

	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture[ANIMAL_TYPE_MAX];	//テクスチャへのポインタ
	static ANIMAL_TYPE m_Animal;	//動物のタイプ
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_move;	//移動量
	CInputKeyboard *m_pKeyBoard;	//キーボードクラスのポインタ変数
	CInputJoyStick *m_pJoyStick;	//ジョイスティッククラスのポインタ変数
	bool m_bSkill;	//スキルを使っているかどうか
	int m_nSkillCounter;	//スキルカウンタ
	int m_nStateCounter;	//ダメージカウンター
	int m_nShotCounter;	//発射カウンター
	int m_nDefense;	//防御力
	int m_nSkillPoint;	//スキルポイント
};

#endif