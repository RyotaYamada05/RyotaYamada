//=============================================================================
//
// ���C������ [player.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C��
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
//�}�N����`
//=============================================================================
#define BULLET_MOVE D3DXVECTOR3(10.0f, 0.0f, 0.0f)		//�e�̈ړ����x
#define STATE_TIME_COUNTER 60	//��ԑJ�ڗp�̃J�E���^�萔
#define PLAYER_MOVE_SPEED 2.0f	//�ړ��ʒ萔
#define RATE_MOVE 0.2f	//�����W��
#define CHEETAH_ATK 1	//�`�[�^�̎��̍U����
#define GORILLA_ATK 10	//�S�����̎��̍U����
#define TURTLE_ATK 5	//�J���̎��̍U����
#define DEFENSE_VALUE 5	//�h��͒萔
#define CHEETAH_SKILL_ATK 2

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;	//�e�N�X�`���ւ̃|�C���^�ϐ�
CPlayer::ANIMAL_TYPE CPlayer::m_Animal = ANIMAL_TYPE_CHEETAH;	//�����̎�ޗp�̕ϐ�

//=============================================================================
//�v���C���[�N���X�̃R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority):CScene2D(nPriority)
{
	//�e�����o�����N���A
	m_nShotCounter = 0;	//���˃J�E���^
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ���
	m_State = PLAYER_STATE_NORMAL;	//�ʏ���
	m_bSkill = false;	//�X�L�����������ǂ���
	m_nLife = PLAYER_MAX_LIFE;	//�v���C���[���C�t
	m_nSkillCounter = 0;	//�X�L���J�E���^�[
	m_nStateCounter = 0;	//�_���[�W�J�E���^�[�ϐ�
	m_nDefense = 0;	//�h���
	m_pKeyBoard = NULL;
	m_pJoyStick = NULL;
}


//=============================================================================
//�v���C���[�N���X�̃f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
//�v���C���[�N���X�̃N���G�C�g����
//=============================================================================
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�v���C���[�N���X�̃|�C���^�ϐ�
	CPlayer *pPlayer = NULL;

	//�C���X�^���X����
	pPlayer = new CPlayer;

	//���������m�ۂł��Ă�����
	if (pPlayer != NULL)
	{
		//�v���C���[�N���X�̏����������Ăяo��
		pPlayer->Init(pos, size);
	}
	else
	{
		return NULL;
	}

	return pPlayer;
}

//=============================================================================
//�v���C���[�N���X�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CPlayer::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/getemono.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//�v���C���[�N���X�̃e�N�X�`���j������
//=============================================================================
void CPlayer::UnLoad(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//�v���C���[�N���X�̏���������
//=============================================================================
HRESULT CPlayer::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//2D�|���S���N���X�̏����������Ăяo��
	CScene2D::Init(pos, size);

	//�e�N�X�`���̊��蓖��
	CScene2D::BindTexture(m_pTexture);

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);

	//���C�t�N���X�̏��擾
	CLife *m_pLife = CGame::GetUi()->GetLife();
	
	//�A�j�}���^�C�v�̏�����
	m_Animal = ANIMAL_TYPE_CHEETAH;

	//���C�t�̐ݒ�
	m_pLife->SetLife(m_nLife);

	//�����̕ύX
	srand((unsigned int)time(NULL));

	//���̓f�o�C�X���̎擾
	m_pKeyBoard = CManager::GetKeyborad();

	//�W���C�X�e�B�b�N���̎擾
	m_pJoyStick = CManager::GetJoyStick();
	return S_OK;
}

//=============================================================================
//�v���C���[�N���X�̏I������
//=============================================================================
void CPlayer::Uninit(void)
{
	//2D�|���S���N���X�̍X�V�����Ăяo��
	CScene2D::Uninit();
}

//=============================================================================
//�v���C���[�N���X�̍X�V����
//=============================================================================
void CPlayer::Update(void)
{
	//�|���S���ʒu�̎擾
	m_pos = CScene2D::GetPos();

	//��Ԕ��菈���Ăяo��
	if (DecisionState() == true)
	{
		return;
	}

	if (m_pKeyBoard->GetKeyBoardTrigger(DIK_X) || m_pJoyStick->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_Y)
		|| m_pJoyStick->GetPushRelease(CROSS_KEY_UP))
	{
		//�����^�C�v�̕ύX�����Ăяo��
		ChangeAnimal();
	}

	if (m_pKeyBoard->GetKeyBoardPress(DIK_SPACE) || m_pJoyStick->GetJoyStickPress(CInputJoyStick::JOY_BUTTON_R_TRIGGER2))
	{
		//�o���b�g���ˏ����Ăяo��
		ShotBullet();
	}

	//�X�L���|�C���g��100���傫���A����M�L�[�������ꂽ��
	if ((m_pKeyBoard->GetKeyBoardTrigger(DIK_M) || m_pJoyStick->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_X)) &&
		CGame::GetUi()->GetSkill()->GetSkill() >= 100)
	{
		//�X�L������
		m_bSkill = true;
		//�X�L���|�C���g��0�ɂ���
		CGame::GetUi()->GetSkill()->SetSkill(0);
	}

	if (m_bSkill == true)
	{
		//�����ʂ̃X�L������
		TypebySkil();
	}

	//�ړ������̌Ăяo��
	Moving();

	//�ʒu���̍X�V
	CScene2D::SetPos(m_pos);
}

//=============================================================================
//�v���C���[�N���X�̕`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	if (m_State != PLAYER_STATE_DIED)
	{
		//2D�|���S���N���X�̕`�揈���Ăяo��
		CScene2D::Draw();
	}
}

//=============================================================================
//�v���C���[�N���X�̈ړ�����
//=============================================================================
void CPlayer::Moving(void)
{
	DIJOYSTATE joy = CManager::GetJoyStick()->GetJoyStick();

	//���a�̌v�Z
	float fRadius = sqrtf((PLAYER_MOVE_SPEED*PLAYER_MOVE_SPEED) + (PLAYER_MOVE_SPEED*PLAYER_MOVE_SPEED));

	//========================================================================
	//��ւ̈ړ�
	//========================================================================
	if (m_pKeyBoard->GetKeyBoardPress(DIK_W))
	{
		m_move.y -= m_pKeyBoard->GetKeyBoardPress(DIK_A) || m_pKeyBoard->GetKeyBoardPress(DIK_D) ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
	//���X�e�B�b�N����ɌX�����Ƃ�
	else if (joy.lY < 0)
	{
		m_move.y -= joy.lX < 0 || joy.lX > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#ifdef _RSTICK_
	//�E�X�e�B�b�N����ɌX�����Ƃ�
	else if (joy.lRz < 0)
	{
		m_move.y -= joy.lZ < 0 || joy.lZ > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#endif

	//========================================================================
	//���ւ̈ړ�
	//========================================================================

	if (m_pKeyBoard->GetKeyBoardPress(DIK_S))
	{
		m_move.y += m_pKeyBoard->GetKeyBoardPress(DIK_A) || m_pKeyBoard->GetKeyBoardPress(DIK_D) ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
	//���X�e�B�b�N�����ɌX�����Ƃ�
	else if (joy.lY > 0)
	{
		m_move.y += joy.lX < 0 || joy.lX > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#ifdef _RSTICK_
	//�E�X�e�B�b�N�����ɌX�����Ƃ�
	else if (joy.lRz > 0)
	{
		m_move.y += joy.lZ < 0 || joy.lZ > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#endif

	//========================================================================
	//���ւ̈ړ�
	//========================================================================
	if (m_pKeyBoard->GetKeyBoardPress(DIK_A))
	{
		m_move.x -= m_pKeyBoard->GetKeyBoardPress(DIK_W) || m_pKeyBoard->GetKeyBoardPress(DIK_S) ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
	//���X�e�B�b�N�����ɌX�����Ƃ�
	else if (joy.lX < 0)
	{
		m_move.x -= joy.lY < 0 || joy.lY > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#ifdef _RSTICK_
	//�E�X�e�B�b�N�����ɌX�����Ƃ�
	else if (joy.lZ < 0)
	{
		m_move.x -= joy.lRz < 0 || joy.lRz > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#endif

	//========================================================================
	//�E�ւ̈ړ�
	//========================================================================
	if (m_pKeyBoard->GetKeyBoardPress(DIK_D))
	{
		m_move.x += m_pKeyBoard->GetKeyBoardPress(DIK_W) || m_pKeyBoard->GetKeyBoardPress(DIK_S) ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
	//���X�e�B�b�N���E�ɌX�����Ƃ�
	else if (joy.lX > 0)
	{
		m_move.x += joy.lY < 0 || joy.lY > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#ifdef _RSTICK_
	//�E�X�e�B�b�N���E�ɌX�����Ƃ�
	else if (joy.lZ > 0)
	{
		m_move.x += joy.lRz < 0 || joy.lRz > 0 ? (fRadius / 2) : PLAYER_MOVE_SPEED;
	}
#endif

	//�v���C���[������ł��Ȃ��Ƃ�
	if (m_State != PLAYER_STATE_DIED)
	{
		//�����̏���
		m_move.x += (0.0f - m_move.x) * RATE_MOVE;
		m_move.y += (0.0f - m_move.y) * RATE_MOVE;

		//�|���S���̈ʒu�X�V
		m_pos.x += m_move.x;
		m_pos.y += m_move.y;
	}

	//�v���C���[����ʊO�֍s���Ȃ��悤�ɂ��鏈��
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
//�����^�C�v�̕ύX����
//=============================================================================
void CPlayer::ChangeAnimal(void)
{
	//�A�j�}��UI��̃|���S�������擾
	CPolygon *pPolygon = CGame::GetUi()->GetAnimalUI()->GetPolygon();
	
	switch (m_Animal)
	{
	//�`�[�^�[�̎�
	case ANIMAL_TYPE_CHEETAH:
		//�S�����ɕύX����
		m_Animal = ANIMAL_TYPE_GORILLA;

		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos, PLAYER_SIZE, COLOR_PURPLE, 80, CEffect::EFFECT_TYPE_GORILLA,CScene::PRIORITY_PLAYER);

		//UI�e�N�X�`�����S�����ɕύX����
		pPolygon->SetType(CPolygon::TEX_TYPE_GORILLA);
		break;

	//�S�����̎�
	case ANIMAL_TYPE_GORILLA:
		//�J���ɂ���
		m_Animal = ANIMAL_TYPE_TURTLE;

		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos, PLAYER_SIZE, COLOR_GREEN, 80, CEffect::EFFECT_TYPE_TURTLE, CScene::PRIORITY_PLAYER);

		//UI�e�N�X�`�����J���ɕύX����
		pPolygon->SetType(CPolygon::TEX_TYPE_TURTLE);

		//�h��͂̐ݒ�
		m_nDefense = DEFENSE_VALUE;
		break;

	//�J���̎�
	case ANIMAL_TYPE_TURTLE:
		//�`�[�^�[�ɂ���
		m_Animal = ANIMAL_TYPE_CHEETAH;

		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos, PLAYER_SIZE, COLOR_YELLOW, 80, CEffect::EFFECT_TYPE_CHEETAH, CScene::PRIORITY_PLAYER);

		//UI�e�N�X�`�����`�[�^�[�ɕύX����
		pPolygon->SetType(CPolygon::TEX_TYPE_CHEETAH);

		//�h��͂����ɖ߂�
		m_nDefense = 0;

		break;
	}

	//���˃J�E���^�[���N���A����
	m_nShotCounter = 0;
}

//=============================================================================
//�����^�C�v�ʂ̃X�L����������
//=============================================================================
void CPlayer::TypebySkil(void)
{
	//���x�̕ύX�͈�
	float fSpeed_Step = 10.0f;

	switch (m_Animal)
	{
	//�`�[�^�[�̎�
	case ANIMAL_TYPE_CHEETAH:
		if (m_nSkillCounter == 60)
		{
			//�X�L���g�p���I�t�ɂ���
			m_bSkill = false;

			//�X�L���J�E���^�[���N���A����
			m_nSkillCounter = 0;
		}
		else
		{
			for (int nCount = 0; nCount < 2; nCount++)
			{
				float fAngle = 0.15f;	//�e�ƒe�̊Ԃ̊p�x

				//���ˊp�x
				float fAngle0 = 0.0f + fAngle * ((float)rand() / RAND_MAX - 0.5f);
				float fRad = D3DXToRadian(fAngle0);

				//���ˑ��x
				float fSpeed = 15.0f + fSpeed_Step * ((float)rand() / RAND_MAX - 0.5f);

				//�o���b�g�̐���
				CBullet::Create(m_pos, D3DXVECTOR3(cosf(fAngle0) * fSpeed,
					sinf(fAngle0) * fSpeed, 0.0f), BULLET_SIZE,
					COLOR_YELLOW, CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_NORMAL, CHEETAH_SKILL_ATK);
			}
		}
		
		break;

	//�S�����̎�
	case ANIMAL_TYPE_GORILLA:
		//�{���̐���
		CBome::Create(m_pos, BOME_SIZE);

		//�X�L���g�p���I�t�ɂ���
		m_bSkill = false;

		//�X�L���J�E���^�[���N���A����
		m_nSkillCounter = 0;
		break;

	//�J���̎�
	case ANIMAL_TYPE_TURTLE:

		if (m_nSkillCounter == 180)
		{
			//�X�L���g�p���I�t�ɂ���
			m_bSkill = false;

			//�X�L���J�E���^�[���N���A����
			m_nSkillCounter = 0;
		}
		break;
	}

	//�X�L���J�E���^�[�A�b�v
	m_nSkillCounter++;
}

//=============================================================================
//�v���C���[�N���X�̃o���b�g���ˏ���
//=============================================================================
void CPlayer::ShotBullet(void)
{
	switch (m_Animal)
	{
	//�`�[�^�[�̎�
	case ANIMAL_TYPE_CHEETAH:
		if (/*m_nShotCounter == 0 ||*/ m_nShotCounter % 5 == 0)
		{
			//�o���b�g�̐���
			CBullet::Create(m_pos, BULLET_MOVE * 2, BULLET_SIZE / 1.2f, COLOR_YELLOW,
				CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_CHEETAH, CHEETAH_ATK ,100);
		}
		break;

	//�S�����̎�
	case ANIMAL_TYPE_GORILLA:
		if (/*m_nShotCounter == 0 ||*/ m_nShotCounter % 30 == 0)
		{
			//�o���b�g�̐���
			CBullet::Create(m_pos, BULLET_MOVE / 1.5f, BULLET_SIZE*1.5f, COLOR_PURPLE,
				CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_GORILLA, GORILLA_ATK,100);
		}
		break;

	//�J���̎�
	case ANIMAL_TYPE_TURTLE:
		if (/*m_nShotCounter == 0 || */m_nShotCounter % 20 == 0)
		{
			//�o���b�g�̐���
			CBullet::Create(m_pos, BULLET_MOVE, BULLET_SIZE, COLOR_GREEN,
				CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_TURTLE, TURTLE_ATK,100);
		}
		break;
	}

	//�`���[�W�J�E���g�A�b�v
	m_nShotCounter++;
}

//=============================================================================
//�v���C���[�N���X�̃_���[�W����
//=============================================================================
void CPlayer::Damage(const int nAttack)
{
	int nDamage;

	//�_���[�W���󂯂Ă��Ȃ��Ƃ�
	if (m_State != PLAYER_STATE_DAMAGE)
	{
		//�_���[�W�Ɩh��͂̌v�Z�����A���ۂ̃_���[�W�ʂ����߂�
		nDamage = nAttack - m_nDefense;

		//�_���[�W�ʂ�0�ȉ��������ꍇ
		if (nDamage <= 0)
		{
			//�����I��
			return;
		}
		else
		{
			//���C�t�N���X�Ƀ_���[�W��^����
			CGame::GetUi()->GetLife()->SubLife(nDamage);

			//�̗͂��_���[�W�����炷
			m_nLife -= nDamage;

			//�X�L���̉��Z
			CGame::GetUi()->GetSkill()->AddSkill(10);
			
			//�̗͂�0��菬�������v���C���[�͎��S
			if (m_nLife <= 0)
			{
				//���S��Ԃֈڍs����
				m_State = PLAYER_STATE_DIED;

				return;
			}
			//�̗͂�0���傫���Ƃ�
			else
			{
				//�_���[�W��Ԃֈڍs����
				m_State = PLAYER_STATE_DAMAGE;
			}
		}
	}
}

//=============================================================================
//�v���C���[�N���X�̗͉̑񕜏���
//=============================================================================
void CPlayer::Recovery(const int nRecovery)
{
	//���S���ĂȂ��Ƃ�
	if (m_State != PLAYER_STATE_DIED)
	{
		//���C�t�N���X�Ɍ��݂̗̑͂�^����
		CGame::GetUi()->GetLife()->AddLife(nRecovery);

		//�̗͂̉�
		m_nLife += nRecovery;

		//�̗͂��ő吔���傫���Ȃ������A�ő�l�ɏC������
		if (m_nLife >= PLAYER_MAX_LIFE)
		{
			m_nLife = PLAYER_MAX_LIFE;
		}
	}
}

//=============================================================================
//�v���C���[�N���X�̏�Ԕ��菈��
//=============================================================================
bool CPlayer::DecisionState(void)
{
	switch (m_State)
	{
	//�ʏ���
	case PLAYER_STATE_NORMAL:
		break;

	//�_���[�W���󂯂����
	case PLAYER_STATE_DAMAGE:

		//�F�̐ݒ�
		SetColor(COLOR_ALPHA);

		//�_���[�W�J�E���^�[�A�b�v
		m_nStateCounter++;

		if (m_nStateCounter >= STATE_TIME_COUNTER)
		{
			//�F�̐ݒ�
			SetColor(COLOR_WHITE);
			m_nStateCounter = 0;

			//�ʏ��Ԃֈڍs
			m_State = PLAYER_STATE_NORMAL;
		}
		else if (m_nStateCounter % 20 <= 5)
		{
			//�F�̐ݒ�
			SetColor(COLOR_ALPHA);
		}
		else
		{
			//�F�̐ݒ�
			SetColor(COLOR_WHITE);
		}
		break;

	//����ł�����
	case PLAYER_STATE_DIED:

		CGame::SetGameState(CGame::GAME_STATE_GAME_OVER);

		//�I�������Ăяo��
		Uninit();
		return true;

		break;
	}

	return false;
}

//=============================================================================
//�v���C���[�N���X�̈ʒu���̎擾
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void) const
{
	return m_pos;
}

//=============================================================================
//�v���C���[�N���X�̓������̎擾
//=============================================================================
CPlayer::ANIMAL_TYPE CPlayer::GetAnimal(void)
{
	return m_Animal;
}