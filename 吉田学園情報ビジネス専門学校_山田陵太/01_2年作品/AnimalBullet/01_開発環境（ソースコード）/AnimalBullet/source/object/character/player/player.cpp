//=============================================================================
//
// プレイヤー処理 [player.cpp]
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
#include "lifeber.h"
#include "ui.h"
#include "animalui.h"
#include "polygon.h"
#include "effect.h"
#include "skillber.h"
#include "game.h"
#include "bome.h"
#include "shield.h"
#include "debugfont.h"
#include "skillber.h"
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
#define TURTLE_ATK 3	//カメの時の攻撃力
#define DEFENSE_VALUE 3	//防御力定数
#define CHEETAH_SKILL_ATK 2
#define PLAYER_MAX_LIFE 100	//プレイヤーの最大体力

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture[ANIMAL_TYPE_MAX] = {};	//テクスチャへのポインタ変数
CPlayer::ANIMAL_TYPE CPlayer::m_Animal = ANIMAL_TYPE_CHEETAH;	//動物の種類用の変数

//=============================================================================
//プレイヤークラスのコンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority):CCharacter(nPriority)
{
	//各メンバ情報をクリア
	m_nShotCounter = 0;	//発射カウンタ
	m_move = VECTOR3_ZERO_INITIALIZE;	//移動量
	m_bSkill = false;	//スキル発動中かどうか
	m_nSkillCounter = 0;	//スキルカウンター
	m_nStateCounter = 0;	//ダメージカウンター変数
	m_nDefense = 0;	//防御力
	m_pKeyBoard = NULL;
	m_pJoyStick = NULL;
	m_nSkillPoint = 0;
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
	if (pPlayer)
	{
		pPlayer->SetPos(pos);
		pPlayer->SetSize(size);
		//プレイヤークラスの初期化処理呼び出し
		pPlayer->Init();
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
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Player/Player_Cheetah.png", &m_pTexture[ANIMAL_TYPE_CHEETAH]);	//チーター
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Player/Player_Gorilla.png", &m_pTexture[ANIMAL_TYPE_GORILLA]);	//ゴリラ
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Player/Player_Turtle.png", &m_pTexture[ANIMAL_TYPE_TURTLE]);	//カメ

	return S_OK;
}

//=============================================================================
//プレイヤークラスのテクスチャ破棄処理
//=============================================================================
void CPlayer::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < ANIMAL_TYPE_MAX; nCntTex++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCntTex])
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//プレイヤークラスの初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	//体力の初期化
	InitLife(PLAYER_MAX_LIFE);

	//スキルポイントの初期化
	m_nSkillPoint = MAX_SKILL;

	//2Dポリゴンクラスの初期化処理呼び出し
	CScene2D::Init();

	//アニマルタイプの初期化
	m_Animal = ANIMAL_TYPE_CHEETAH;

	//テクスチャの割り当て
	CScene2D::BindTexture(m_pTexture[m_Animal]);

	//オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);

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
	//CScene2Dの更新処理
	CScene2D::Update();

	//ポリゴン位置の取得
	m_pos = CScene2D::GetPos();

	//状態判定処理呼び出し
	if (STATE_DEATH(CheckState()))
	{
		return;
	}

	//XキーまたはジョイスティックYボタンまたは十字上キーを押したとき
	if (m_pKeyBoard->GetKeyBoardTrigger(DIK_X) || 
		m_pJoyStick->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_Y) || 
		m_pJoyStick->GetPushRelease(CROSS_KEY_UP))
	{
		//動物タイプの変更処理呼び出し
		ChangeAnimal();
	}

	//スペースキーまたは右トリガー2を押したとき
	if (m_pKeyBoard->GetKeyBoardPress(DIK_SPACE) || 
		m_pJoyStick->GetJoyStickPress(CInputJoyStick::JOY_BUTTON_R_TRIGGER2))
	{
		//バレット発射処理呼び出し
		ShotBullet();
	}

	//スキルポイントがMAX_SKILLより大きく、かつMキーまたはジョイスティックXボタンが押されたら
	if ((m_pKeyBoard->GetKeyBoardTrigger(DIK_M) || 
		m_pJoyStick->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_X)) &&
		m_nSkillPoint >= MAX_SKILL)
	{
		//スキル発動
		m_bSkill = true;

		//スキルポイントを0にする
		m_nSkillPoint = 0;
		CUi::GetSkillber()->SetGauge(m_nSkillPoint);
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

#ifdef _DEBUG
	//デバッグ情報の表示
	DebugPrint();
#endif
}

//=============================================================================
//プレイヤークラスの描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	//2Dポリゴンクラスの描画処理呼び出し
	CScene2D::Draw();
}

//=============================================================================
//プレイヤークラスのダメージ処理
//=============================================================================
void CPlayer::Damage(const int nAttack)
{
	int nDamage;

	//ダメージを受けていないとき
	if (GetState() != CHARACTER_STATE_DAMAGE)
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
			//体力を減らす
			int nNowLife = SubLife(nDamage);

			//ライフゲージにもダメージと同値を与える
			CGame::GetUi()->GetLifeber()->SubGauge(nDamage);

			//スキルの加算
			AddSkillPoint(10);
			
			//体力が0より小さい時プレイヤーは死亡
			if (nNowLife <= 0)
			{
				//死亡状態へ移行する
				SetState(CHARACTER_STATE_DIED);
			}
			//体力が0より大きいとき
			else
			{
				//ダメージ状態へ移行する
				SetState(CHARACTER_STATE_DAMAGE);
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
	if (GetState() != CHARACTER_STATE_DIED)
	{
		//体力の回復
		AddLife(nRecovery);

		//ライフクラスに現在の体力を与える
		CGame::GetUi()->GetLifeber()->AddGauge(nRecovery);
	}
}

//=============================================================================
//プレイヤークラスの死亡処理
//=============================================================================
void CPlayer::DiedProcess(void)
{
	CGame::SetGameState(CGame::GAME_STATE_GAME_OVER);

	//終了処理呼び出し
	Uninit();
}

//=============================================================================
//プレイヤークラスのスキルポイントの取得
//=============================================================================
int CPlayer::GetSkillPoint(void) const
{
	return m_nSkillPoint;
}

//=============================================================================
//プレイヤークラスのスキルポイントの加算
//=============================================================================
void CPlayer::AddSkillPoint(const int nAddSkill)
{
	m_nSkillPoint += nAddSkill;

	CUi::GetSkillber()->SetGauge(m_nSkillPoint);
	//最大値を超えたら修正する
	if (m_nSkillPoint > MAX_SKILL)
	{
		m_nSkillPoint = MAX_SKILL;
	}
}

//=============================================================================
//プレイヤークラスの動物情報の取得
//=============================================================================
CPlayer::ANIMAL_TYPE CPlayer::GetAnimal(void)
{
	return m_Animal;
}

//=============================================================================
//プレイヤークラスの移動処理
//=============================================================================
void CPlayer::Moving(void)
{
	//スティック情報を取得
	DIJOYSTATE joy;
	SecureZeroMemory(&joy, sizeof(joy));
	joy = m_pJoyStick->GetJoyStickInfo();

	//半径の計算
	float fRadius = sqrtf((PLAYER_MOVE_SPEED * PLAYER_MOVE_SPEED) + (PLAYER_MOVE_SPEED * PLAYER_MOVE_SPEED));

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

	//プレイヤーが死んでいないとき
	if (GetState() != CHARACTER_STATE_DIED)
	{
		//慣性の処理
		m_move.x += (0.0f - m_move.x) * RATE_MOVE;
		m_move.y += (0.0f - m_move.y) * RATE_MOVE;

		//ポリゴンの位置更新
		m_pos.x += m_move.x;
		m_pos.y += m_move.y;
	}

	//サイズの取得
	D3DXVECTOR3 size = GetSize();

	//プレイヤーが画面外へ行かないようにする処理
	if (m_pos.x + (size.x / 2) >= SCREEN_WIDTH)
	{
		m_pos.x = SCREEN_WIDTH - (size.x / 2);
	}
	if (m_pos.x - (size.x / 2) <= 0)
	{
		m_pos.x = 0 + size.x / 2;
	}
	if (m_pos.y + (size.y / 2) >= SCREEN_HEIGHT)
	{
		m_pos.y = SCREEN_HEIGHT - (size.y / 2);
	}
	if (m_pos.y - (size.y / 2) <= UNDER_UI_SIZE_Y)
	{
		m_pos.y = UNDER_UI_SIZE_Y + size.y / 2;
	}
}

//=============================================================================
//動物タイプの変更処理
//=============================================================================
void CPlayer::ChangeAnimal(void)
{
	//アニマルUI上のポリゴン情報を取得
	CPolygon *pPolygon = CUi::GetAnimalUI()->GetPolygon();
	D3DXVECTOR3 size;

	switch (m_Animal)
	{
		//チーターの時
	case ANIMAL_TYPE_CHEETAH:
		//ゴリラに変更する
		m_Animal = ANIMAL_TYPE_GORILLA;

		//変更するサイズを指定
		size = GORILLA_SIZE;

		//エフェクトの生成
		CEffect::Create(m_pos, CHEETAH_SIZE, COLOR_PURPLE, 80, CEffect::EFFECT_TYPE_GORILLA, CScene::PRIORITY_PLAYER);

		//UIテクスチャをゴリラに変更する
		pPolygon->SetType(CPolygon::TEX_TYPE_GORILLA);
		break;

		//ゴリラの時
	case ANIMAL_TYPE_GORILLA:
		//カメにする
		m_Animal = ANIMAL_TYPE_TURTLE;

		//変更するサイズを指定
		size = TURTLE_SIZE;

		//エフェクトの生成
		CEffect::Create(m_pos, CHEETAH_SIZE, COLOR_GREEN, 80, CEffect::EFFECT_TYPE_TURTLE, CScene::PRIORITY_PLAYER);

		//UIテクスチャをカメに変更する
		pPolygon->SetType(CPolygon::TEX_TYPE_TURTLE);

		//防御力の設定
		m_nDefense = DEFENSE_VALUE;
		break;

		//カメの時
	case ANIMAL_TYPE_TURTLE:
		//チーターにする
		m_Animal = ANIMAL_TYPE_CHEETAH;

		//変更するサイズを指定
		size = CHEETAH_SIZE;

		//エフェクトの生成
		CEffect::Create(m_pos, CHEETAH_SIZE, COLOR_YELLOW, 80, CEffect::EFFECT_TYPE_CHEETAH, CScene::PRIORITY_PLAYER);

		//UIテクスチャをチーターに変更する
		pPolygon->SetType(CPolygon::TEX_TYPE_CHEETAH);

		//防御力を元に戻す
		m_nDefense = 0;

		break;
	}

	//テクスチャの変更
	CScene2D::BindTexture(m_pTexture[m_Animal]);

	//サイズの変更
	SetSize(size);

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
				float fShotAngle = 0.0f + fAngle * ((float)rand() / RAND_MAX - 0.5f);
				float fRad = D3DXToRadian(fShotAngle);

				//発射速度
				float fSpeed = 15.0f + fSpeed_Step * ((float)rand() / RAND_MAX - 0.5f);

				//弾の移動量を計算
				D3DXVECTOR3 bulletMove = D3DXVECTOR3(cosf(fShotAngle) * fSpeed, sinf(fShotAngle) * fSpeed, 0.0f);

				//バレットの生成
				CBullet::Create(
					m_pos,
					bulletMove,
					BULLET_SIZE,
					COLOR_YELLOW,
					CBullet::BULLET_USER_PLAYER,
					CBullet::BULLET_TYPE_NORMAL,
					CHEETAH_SKILL_ATK);
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
		CShield::Create(m_pos, SHILED_SIZE);

		//スキル使用をオフにする
		m_bSkill = false;

		//スキルカウンターをクリアする
		m_nSkillCounter = 0;

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
		if (m_nShotCounter % 5 == 0)
		{
			//バレットの生成
			CBullet::Create(m_pos, BULLET_MOVE * 2, BULLET_SIZE / 1.2f, COLOR_YELLOW,
				CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_CHEETAH, CHEETAH_ATK, 100);
		}
		break;

		//ゴリラの時
	case ANIMAL_TYPE_GORILLA:
		if (m_nShotCounter % 30 == 0)
		{
			//バレットの生成
			CBullet::Create(m_pos, BULLET_MOVE / 1.5f, BULLET_SIZE*1.5f, COLOR_PURPLE,
				CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_GORILLA, GORILLA_ATK, 100);
		}
		break;

		//カメの時
	case ANIMAL_TYPE_TURTLE:
		if (m_nShotCounter % 20 == 0)
		{
			//バレットの生成
			CBullet::Create(m_pos, BULLET_MOVE, BULLET_SIZE, COLOR_GREEN,
				CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_TURTLE, TURTLE_ATK, 100);
		}
		break;
	}

	//発射カウントアップ
	m_nShotCounter++;
}

#ifdef _DEBUG
//=============================================================================
//プレイヤークラスのデバッグ情報の表示とりまとめ
//=============================================================================
void CPlayer::DebugPrint(void)
{
	CDebugFont::Print(CDebugFont::DEBUG_LAYER_LEFT, "--------------------プレイヤー情報--------------------\n");
	CDebugFont::Print(CDebugFont::DEBUG_LAYER_LEFT, "【位置】X:%0.2f Y:%0.2f\n", m_pos.x, m_pos.y);
	CDebugFont::Print(CDebugFont::DEBUG_LAYER_LEFT, "【移動量】X:%0.2f Y:%0.2f\n", m_move.x, m_move.y);
	CDebugFont::Print(CDebugFont::DEBUG_LAYER_LEFT, "【最大体力】%d\n", GetMaxLife());
	CDebugFont::Print(CDebugFont::DEBUG_LAYER_LEFT, "【現在体力】%d\n", GetLife());
	CDebugFont::Print(CDebugFont::DEBUG_LAYER_LEFT, "【スキルポイント】%d\n", m_nSkillPoint);
}
#endif