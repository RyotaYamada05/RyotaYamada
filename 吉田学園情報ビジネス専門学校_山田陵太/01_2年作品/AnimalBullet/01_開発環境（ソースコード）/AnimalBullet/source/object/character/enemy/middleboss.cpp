//=============================================================================
//
// 中ボス処理 [middleboss.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "middleboss.h"
#include "game.h"
#include "player.h"
#include "bullet.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define Middle_ATK 10 //中ボスの攻撃力
#define Middle_BULLET_SPEED 8.0f //中ボスの弾の速度

//=============================================================================
//中ボスクラスのコンストラクタ
//=============================================================================
CMiddleBoss::CMiddleBoss()
{
}

//=============================================================================
//中ボスクラスのデストラクタ
//=============================================================================
CMiddleBoss::~CMiddleBoss()
{
}

//=============================================================================
//中ボスクラスの初期化処理
//=============================================================================
HRESULT CMiddleBoss::Init(void)
{
	CEnemy::Init();
	return S_OK;
}

//=============================================================================
//中ボスクラスの終了処理
//=============================================================================
void CMiddleBoss::Uninit(void)
{
	CEnemy::Uninit();
}

//=============================================================================
//中ボスクラスの更新処理
//=============================================================================
void CMiddleBoss::Update(void)
{
	CEnemy::Update();
}

//=============================================================================
//中ボスクラスの描画処理
//=============================================================================
void CMiddleBoss::Draw(void)
{
	CEnemy::Draw();
}

//=============================================================================
//中ボスクラスの攻撃処理
//=============================================================================
void CMiddleBoss::Attack(void)
{
	if (m_pos.x <= 1000)
	{
		int nNum = 10;	//弾の数
		float fAngle = 15.0f;	//弾と弾の間の角度
		float fRad_Step = D3DXToRadian(fAngle);	//ラジアンに変換

		//端の弾と中心の弾との間の角度を計算(偶数・奇数によって結果変動)
		float fRad = nNum % 2 ? -nNum / 2 * fRad_Step : (-nNum / 2 + 0.5f) * fRad_Step;

		m_nCounter++;

		if (m_nCounter % 120 == 0)
		{
			for (int nCount = 0; nCount < nNum; nCount++, fRad += fRad_Step)
			{
				CBullet::Create(m_pos, D3DXVECTOR3(-cosf(fRad) * Middle_BULLET_SPEED,
					sinf(fRad) * Middle_BULLET_SPEED, 0.0f), BULLET_SIZE,
					COLOR_BLUE, CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_NORMAL, Middle_ATK);
			}
		}

		nNum = 9;

		//端の弾と中心の弾との間の角度を計算(偶数・奇数によって結果変動)
		fRad = nNum % 2 ? -nNum / 2 * fRad_Step : (-nNum / 2 + 0.5f) * fRad_Step;
		if (m_nCounter % 140 == 0)
		{
			for (int nCount = 0; nCount < nNum; nCount++, fRad += fRad_Step)
			{
				CBullet::Create(m_pos, D3DXVECTOR3(-cosf(fRad) * 5,
					sinf(fRad) * 5, 0.0f), BULLET_SIZE,
					COLOR_GREEN, CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_NORMAL, Middle_ATK);
			}
		}
	}
}
