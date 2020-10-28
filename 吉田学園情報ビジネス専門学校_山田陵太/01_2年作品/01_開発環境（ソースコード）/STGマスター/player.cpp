//=============================================================================
//
// メイン処理 [player.cpp]
// Author : 山田陵太
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include <time.h>
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "bullet.h"
#include "sound.h"
#include "life.h"
#include "ui.h"
#include "animalui.h"
#include "polygon.h"
#include "effect.h"
#include "skill.h"
#include "game.h"
#include "bome.h"
#include "joystick.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define BULLET_MOVE D3DXVECTOR3(10.0f, 0.0f, 0.0f)		//弾の移動速度
#define STATE_TIME_COUNTER 60	//状態遷移用のカウンタ定数
#define PLAYER_MOVE_SPEED 2.0f	//移動量定数
#define RATE_MOVE 0.2f	//慣性係数
#define CHEETAH_ATK 1	//チータの時の攻撃力
#define GORILLA_ATK 10	//ゴリラの時の攻撃力
#define TURTLE_ATK 5	//カメの時の攻撃力
#define DEFENSE_VALUE 5	//防御力定数
#define CHEETAH_SKILL_ATK 2

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;	//テクスチャへのポインタ変数
CPlayer::ANIMAL_TYPE CPlayer::m_Animal = ANIMAL_TYPE_CHEETAH;	//動物の種類用の変数

//=============================================================================
//プレイヤークラスのコンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority):CScene2D(nPriority)
{
	//各メンバ情報をクリア
	m_nShotCounter = 0;	//発射カウンタ
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量
	m_State = PLAYER_STATE_NORMAL;	//通常状態
	m_bSkill = false;	//スキル発動中かどうか
	m_nLife = PLAYER_MAX_LIFE;	//プレイヤーライフ
	m_nSkillCounter = 0;	//スキルカウンター
	m_nStateCounter = 0;	//ダメージカウンター変数
	m_nDefense = 0;	//防御力
	m_pKeyBoard = NULL;
	m_pJoyStick = NULL;
}


//=============================================================================
//プレイヤークラスのデストラクタ
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
//プレイヤークラスのクリエイト処理
//=============================================================================
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//プレイヤークラスのポインタ変数
	CPlayer *pPlayer = NULL;

	//インスタンス生成
	pPlayer = new CPlayer;

	//メモリが確保できていたら
	if (pPlayer != NULL)
	{
		//プレイヤークラスの初期化処理呼び出し
		pPlayer->Init(pos, size);
	}
	else
	{
		return NULL;
	}

	return pPlayer;
}

//=============================================================================
//プレイヤークラスのテクスチャロード処理
//=============================================================================
HRESULT CPlayer::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/getemono.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//プレイヤークラスのテクスチャ破棄処理
//=============================================================================
void CPlayer::UnLoad(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//プレイヤークラスの初期化処理
//=============================================================================
HRESULT CPlayer::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//2Dポリゴンクラスの初期化処理呼び出し
	CScene2D::Init(pos, size);

	//テクスチャの割り当て
	CScene2D::BindTexture(m_pTexture);

	//オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);

	//ライフクラスの情報取得
	CLife *m_pLife = CGame::GetUi()->GetLife();
	
	//アニマルタイプの初期化
	m_Animal = ANIMAL_TYPE_CHEETAH;

	//ライフの設定
	m_pLife->SetLife(m_nLife);

	//乱数の変更
	srand((unsigned int)time(NULL));

	//入力デバイス情報の取得
	m_pKeyBoard = CManager::GetKeyborad();

	//ジョイスティック情報の取得
	m_pJoyStick = CManager::GetJoyStick();
	return S_OK;
}

//=============================================================================
//プレイヤークラスの終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	//2Dポリゴンクラスの更新処理呼び出し
	CScene2D::Uninit();
}

//=============================================================================
//プレイヤークラスの更新処理
//=============================================================================
void CPlayer::Update(void)
{
	//ポリゴン位置の取得
	m_pos = CScene2D::GetPos();

	//状態判定処理呼び出し
	if (DecisionState() == true)
	{
		return;
	}

	if (m_pKeyBoard->GetKeyBoardTrigger(DIK_X) || m_pJoyStick->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_Y)
		|| m_pJoyStick->GetPushRelease(CROSS_KEY_UP))
	{
		//動物タイプの変更処理呼び出し
		ChangeAnimal();
	}

	if (m_pKeyBoard->GetKeyBoardPress(DIK_SPACE) || m_pJoyStick->GetJoyStickPress(CInputJoyStick::JOY_BUTTON_R_TRIGGER2))
	{
		//バレット発射処理呼び出し
		ShotBullet();
	}

	//スキルポイントが100より大きく、かつMキーが押されたら
	if ((m_pKeyBoard->GetKeyBoardTrigger(DIK_M) || m_pJoyStick->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_X)) &&
		CGame::GetUi()->GetSkill()->GetSkill() >= 100)
	{
		//スキル発動
		m_bSkill = true;
		//スキルポイントを0にする
		CGame::GetUi()->GetSkill()->SetSkill(0);
	}

	if (m_bSkill == true)
	{
		//動物別のスキル発動
		TypebySkil();
	}

	//移動処理の呼び出し
	Moving();

	//位置情報の更新
	CScene2D::SetPos(m_pos);
}

//=============================================================================
//プレイヤークラスの描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	if (m_State != PLAYER_STATE_DIED)
	{
		//2Dポリゴンクラスの描画処理呼び出し
		CScene2D::Draw();
	}
}

//=============================================================================
//プレイヤークラスの移動処理
//=============================================================================
void CPlayer::Moving(void)
{
	DIJOYSTATE joy = CManager::GetJoyStick()->GetJoyStick();

	//半径の計算
	float fRadius = sqrtf((PLAYER_MOVE_SPEED*PLAYER_MOVE_SPEED) + (PLAYER_MOVE_SPEED*PLAYER_MOVE_SPEED));

	//========================================================================
	//上への移動
	//========================================================================
	if (m_pKeyBoard->GetKeyBoardPress(DIK_W))
	{
		m_move.y -= m_pKeyBoard->GetKeyBoardPress(DIK_A) || m_pKeyBoard->GetKeyBoardPress(DIK_D) ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
	//左スティックを上に傾けたとき
	else if (joy.lY < 0)
	{
		m_move.y -= joy.lX < 0 || joy.lX > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#ifdef _RSTICK_
	//右スティックを上に傾けたとき
	else if (joy.lRz < 0)
	{
		m_move.y -= joy.lZ < 0 || joy.lZ > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#endif

	//========================================================================
	//下への移動
	//========================================================================

	if (m_pKeyBoard->GetKeyBoardPress(DIK_S))
	{
		m_move.y += m_pKeyBoard->GetKeyBoardPress(DIK_A) || m_pKeyBoard->GetKeyBoardPress(DIK_D) ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
	//左スティックを下に傾けたとき
	else if (joy.lY > 0)
	{
		m_move.y += joy.lX < 0 || joy.lX > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#ifdef _RSTICK_
	//右スティックを下に傾けたとき
	else if (joy.lRz > 0)
	{
		m_move.y += joy.lZ < 0 || joy.lZ > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#endif

	//========================================================================
	//左への移動
	//========================================================================
	if (m_pKeyBoard->GetKeyBoardPress(DIK_A))
	{
		m_move.x -= m_pKeyBoard->GetKeyBoardPress(DIK_W) || m_pKeyBoard->GetKeyBoardPress(DIK_S) ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
	//左スティックを左に傾けたとき
	else if (joy.lX < 0)
	{
		m_move.x -= joy.lY < 0 || joy.lY > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#ifdef _RSTICK_
	//右スティックを左に傾けたとき
	else if (joy.lZ < 0)
	{
		m_move.x -= joy.lRz < 0 || joy.lRz > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#endif

	//========================================================================
	//右への移動
	//========================================================================
	if (m_pKeyBoard->GetKeyBoardPress(DIK_D))
	{
		m_move.x += m_pKeyBoard->GetKeyBoardPress(DIK_W) || m_pKeyBoard->GetKeyBoardPress(DIK_S) ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
	//左スティックを右に傾けたとき
	else if (joy.lX > 0)
	{
		m_move.x += joy.lY < 0 || joy.lY > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#ifdef _RSTICK_
	//右スティックを右に傾けたとき
	else if (joy.lZ > 0)
	{
		m_move.x += joy.lRz < 0 || joy.lRz > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#endif

	//プレイヤーが死んでいないとき
	if (m_State != PLAYER_STATE_DIED)
	{
		//慣性の処理
		m_move.x += (0.0f - m_move.x) * RATE_MOVE;
		m_move.y += (0.0f - m_move.y) * RATE_MOVE;

		//ポリゴンの位置更新
		m_pos.x += m_move.x;
		m_pos.y += m_move.y;
	}

	//プレイヤーが画面外へ行かないようにする処理
	if (m_pos.x + (PLAYER_SIZE_X / 2) >= SCREEN_WIDTH)
	{
		m_pos.x = SCREEN_WIDTH - (PLAYER_SIZE_X / 2);
	}
	if (m_pos.x - (PLAYER_SIZE_X / 2) <= 0)
	{
		m_pos.x = 0 + PLAYER_SIZE_X / 2;
	}
	if (m_pos.y + (PLAYER_SIZE_Y / 2) >= SCREEN_HEIGHT)
	{
		m_pos.y = SCREEN_HEIGHT - (PLAYER_SIZE_Y / 2);
	}
	if (m_pos.y - (PLAYER_SIZE_Y / 2) <= UNDER_UI_SIZE_Y)
	{
		m_pos.y = UNDER_UI_SIZE_Y + PLAYER_SIZE_Y / 2;
	}
}

//=============================================================================
//動物タイプの変更処理
//=============================================================================
void CPlayer::ChangeAnimal(void)
{
	//アニマルUI上のポリゴン情報を取得
	CPolygon *pPolygon = CGame::GetUi()->GetAnimalUI()->GetPolygon();
	
	switch (m_Animal)
	{
	//チーターの時
	case ANIMAL_TYPE_CHEETAH:
		//ゴリラに変更する
		m_Animal = ANIMAL_TYPE_GORILLA;

		//エフェクトの生成
		CEffect::Create(m_pos, PLAYER_SIZE, COLOR_PURPLE, 80, CEffect::EFFECT_TYPE_GORILLA,CScene::PRIORITY_PLAYER);

		//UIテクスチャをゴリラに変更する
		pPolygon->SetType(CPolygon::TEX_TYPE_GORILLA);
		break;

	//ゴリラの時
	case ANIMAL_TYPE_GORILLA:
		//カメにする
		m_Animal = ANIMAL_TYPE_TURTLE;

		//エフェクトの生成
		CEffect::Create(m_pos, PLAYER_SIZE, COLOR_GREEN, 80, CEffect::EFFECT_TYPE_TURTLE, CScene::PRIORITY_PLAYER);

		//UIテクスチャをカメに変更する
		pPolygon->SetType(CPolygon::TEX_TYPE_TURTLE);

		//防御力の設定
		m_nDefense = DEFENSE_VALUE;
		break;

	//カメの時
	case ANIMAL_TYPE_TURTLE:
		//チーターにする
		m_Animal = ANIMAL_TYPE_CHEETAH;

		//エフェクトの生成
		CEffect::Create(m_pos, PLAYER_SIZE, COLOR_YELLOW, 80, CEffect::EFFECT_TYPE_CHEETAH, CScene::PRIORITY_PLAYER);

		//UIテクスチャをチーターに変更する
		pPolygon->SetType(CPolygon::TEX_TYPE_CHEETAH);

		//防御力を元に戻す
		m_nDefense = 0;

		break;
	}

	//発射カウンターをクリアする
	m_nShotCounter = 0;
}

//=============================================================================
//動物タイプ別のスキル発動処理
//=============================================================================
void CPlayer::TypebySkil(void)
{
	//速度の変更範囲
	float fSpeed_Step = 10.0f;

	switch (m_Animal)
	{
	//チーターの時
	case ANIMAL_TYPE_CHEETAH:
		if (m_nSkillCounter == 60)
		{
			//スキル使用をオフにする
			m_bSkill = false;

			//スキルカウンターをクリアする
			m_nSkillCounter = 0;
		}
		else
		{
			for (int nCount = 0; nCount < 2; nCount++)
			{
				float fAngle = 0.15f;	//弾と弾の間の角度

				//発射角度
				float fAngle0 = 0.0f + fAngle * ((float)rand() / RAND_MAX - 0.5f);
				float fRad = D3DXToRadian(fAngle0);

				//発射速度
				float fSpeed = 15.0f + fSpeed_Step * ((float)rand() / RAND_MAX - 0.5f);

				//バレットの生成
				CBullet::Create(m_pos, D3DXVECTOR3(cosf(fAngle0) * fSpeed,
					sinf(fAngle0) * fSpeed, 0.0f), BULLET_SIZE,
					COLOR_YELLOW, CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_NORMAL, CHEETAH_SKILL_ATK);
			}
		}
		
		break;

	//ゴリラの時
	case ANIMAL_TYPE_GORILLA:
		//ボムの生成
		CBome::Create(m_pos, BOME_SIZE);

		//スキル使用をオフにする
		m_bSkill = false;

		//スキルカウンターをクリアする
		m_nSkillCounter = 0;
		break;

	//カメの時
	case ANIMAL_TYPE_TURTLE:

		if (m_nSkillCounter == 180)
		{
			//スキル使用をオフにする
			m_bSkill = false;

			//スキルカウンターをクリアする
			m_nSkillCounter = 0;
		}
		break;
	}

	//スキルカウンターアップ
	m_nSkillCounter++;
}

//=============================================================================
//プレイヤークラスのバレット発射処理
//=============================================================================
void CPlayer::ShotBullet(void)
{
	switch (m_Animal)
	{
	//チーターの時
	case ANIMAL_TYPE_CHEETAH:
		if (/*m_nShotCounter == 0 ||*/ m_nShotCounter % 5 == 0)
		{
			//バレットの生成
			CBullet::Create(m_pos, BULLET_MOVE * 2, BULLET_SIZE / 1.2f, COLOR_YELLOW,
				CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_CHEETAH, CHEETAH_ATK ,100);
		}
		break;

	//ゴリラの時
	case ANIMAL_TYPE_GORILLA:
		if (/*m_nShotCounter == 0 ||*/ m_nShotCounter % 30 == 0)
		{
			//バレットの生成
			CBullet::Create(m_pos, BULLET_MOVE / 1.5f, BULLET_SIZE*1.5f, COLOR_PURPLE,
				CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_GORILLA, GORILLA_ATK,100);
		}
		break;

	//カメの時
	case ANIMAL_TYPE_TURTLE:
		if (/*m_nShotCounter == 0 || */m_nShotCounter % 20 == 0)
		{
			//バレットの生成
			CBullet::Create(m_pos, BULLET_MOVE, BULLET_SIZE, COLOR_GREEN,
				CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_TURTLE, TURTLE_ATK,100);
		}
		break;
	}

	//チャージカウントアップ
	m_nShotCounter++;
}

//=============================================================================
//プレイヤークラスのダメージ処理
//=============================================================================
void CPlayer::Damage(const int nAttack)
{
	int nDamage;

	//ダメージを受けていないとき
	if (m_State != PLAYER_STATE_DAMAGE)
	{
		//ダメージと防御力の計算をし、実際のダメージ量を求める
		nDamage = nAttack - m_nDefense;

		//ダメージ量が0以下だった場合
		if (nDamage <= 0)
		{
			//処理終了
			return;
		}
		else
		{
			//ライフクラスにダメージを与える
			CGame::GetUi()->GetLife()->SubLife(nDamage);

			//体力をダメージ分減らす
			m_nLife -= nDamage;

			//スキルの加算
			CGame::GetUi()->GetSkill()->AddSkill(10);
			
			//体力が0より小さい時プレイヤーは死亡
			if (m_nLife <= 0)
			{
				//死亡状態へ移行する
				m_State = PLAYER_STATE_DIED;

				return;
			}
			//体力が0より大きいとき
			else
			{
				//ダメージ状態へ移行する
				m_State = PLAYER_STATE_DAMAGE;
			}
		}
	}
}

//=============================================================================
//プレイヤークラスの体力回復処理
//=============================================================================
void CPlayer::Recovery(const int nRecovery)
{
	//死亡してないとき
	if (m_State != PLAYER_STATE_DIED)
	{
		//ライフクラスに現在の体力を与える
		CGame::GetUi()->GetLife()->AddLife(nRecovery);

		//体力の回復
		m_nLife += nRecovery;

		//体力が最大数より大きくなった時、最大値に修正する
		if (m_nLife >= PLAYER_MAX_LIFE)
		{
			m_nLife = PLAYER_MAX_LIFE;
		}
	}
}

//=============================================================================
//プレイヤークラスの状態判定処理
//=============================================================================
bool CPlayer::DecisionState(void)
{
	switch (m_State)
	{
	//通常状態
	case PLAYER_STATE_NORMAL:
		break;

	//ダメージを受けた状態
	case PLAYER_STATE_DAMAGE:

		//色の設定
		SetColor(COLOR_ALPHA);

		//ダメージカウンターアップ
		m_nStateCounter++;

		if (m_nStateCounter >= STATE_TIME_COUNTER)
		{
			//色の設定
			SetColor(COLOR_WHITE);
			m_nStateCounter = 0;

			//通常状態へ移行
			m_State = PLAYER_STATE_NORMAL;
		}
		else if (m_nStateCounter % 20 <= 5)
		{
			//色の設定
			SetColor(COLOR_ALPHA);
		}
		else
		{
			//色の設定
			SetColor(COLOR_WHITE);
		}
		break;

	//死んでいる状態
	case PLAYER_STATE_DIED:

		CGame::SetGameState(CGame::GAME_STATE_GAME_OVER);

		//終了処理呼び出し
		Uninit();
		return true;

		break;
	}

	return false;
}

//=============================================================================
//プレイヤークラスの位置情報の取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void) const
{
	return m_pos;
}

//=============================================================================
//プレイヤークラスの動物情報の取得
//=============================================================================
CPlayer::ANIMAL_TYPE CPlayer::GetAnimal(void)
{
	return m_Animal;
}