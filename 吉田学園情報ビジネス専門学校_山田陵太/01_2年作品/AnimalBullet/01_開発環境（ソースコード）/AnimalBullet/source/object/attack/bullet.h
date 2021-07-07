//=============================================================================
//
// バレット処理 [bullet.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define BULLET_SIZE_X 20.0f	//バレットのXサイズ
#define BULLET_SIZE_Y 20.0f	//バレットのYサイズ
#define BULLET_SIZE D3DXVECTOR3(BULLET_SIZE_X,BULLET_SIZE_Y,0.0f)
#define BULLET_LIFE 600	//バレットの寿命

//=============================================================================
//バレットクラス
//=============================================================================
class CBullet : public CScene2D
{
public:
	//=========================================================================
	//バレット使用者の列挙型定義
	//=========================================================================
	typedef enum
	{
		BULLET_USER_NONE = 0,
		BULLET_USER_PLAYER,	//プレイヤーのバレット
		BULLET_USER_ENEMY,	//エネミーのバレット
		BULLET_USER_MAX,	//バレットタイプの最大数
	}BULLET_USER;

	//=========================================================================
	//バレットの種類の列挙型定義
	//=========================================================================
	typedef enum
	{
		BULLET_TYPE_NORMAL = 0,
		BULLET_TYPE_CHEETAH,	//チーター
		BULLET_TYPE_GORILLA,	//ゴリラ
		BULLET_TYPE_TURTLE,	//カメ
		BULLET_TYPE_HOMING,	//ホーミング弾
		BULLET_TYPE_DIVISION,	//分裂弾
		BULLET_TYPE_ACCELE,	//加速弾
		BULLET_TYPE_BOMB,	//ボム
		BULLET_TYPE_MAX,
	}BULLET_TYPE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CBullet(int nPriority = CScene::PRIORITY_BULEET);
	~CBullet();

	static CBullet*Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const D3DXCOLOR col,
		const BULLET_USER user, const BULLET_TYPE type, int nAtk, int nLife = BULLET_LIFE);
	static HRESULT Load(void);
	static void UnLoad(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	bool BulletCollision(void);
	void HomingUpdate(void);
	void DivisionUpdate(void);
	void ACCELEUpdetae(void);
	BULLET_USER GetUser(void)const;

private:
	//=========================================================================
	//メンバ変数宣
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	D3DXVECTOR3 m_pos;	//現在位置
	D3DXVECTOR3 m_move;	//移動量
	D3DXCOLOR m_col;	//色
	D3DXVECTOR3 m_size;	//サイズ
	int m_nLife;	//玉の寿命
	int m_nAtk;	//攻撃力
	int m_nEffectCounter;
	BULLET_USER m_user;	//バレットの使用者
	BULLET_TYPE m_type;	//弾の種類
};

#endif