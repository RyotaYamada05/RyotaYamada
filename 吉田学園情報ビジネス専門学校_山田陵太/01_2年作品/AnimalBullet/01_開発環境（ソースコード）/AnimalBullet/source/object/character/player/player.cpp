//=============================================================================
//
// �v���C���[���� [player.cpp]
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
//�}�N����`
//=============================================================================
#define BULLET_MOVE D3DXVECTOR3(10.0f, 0.0f, 0.0f)		//�e�̈ړ����x
#define STATE_TIME_COUNTER 60	//��ԑJ�ڗp�̃J�E���^�萔
#define PLAYER_MOVE_SPEED 2.0f	//�ړ��ʒ萔
#define RATE_MOVE 0.2f	//�����W��
#define CHEETAH_ATK 1	//�`�[�^�̎��̍U����
#define GORILLA_ATK 10	//�S�����̎��̍U����
#define TURTLE_ATK 3	//�J���̎��̍U����
#define DEFENSE_VALUE 3	//�h��͒萔
#define CHEETAH_SKILL_ATK 2
#define PLAYER_MAX_LIFE 100	//�v���C���[�̍ő�̗�

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture[ANIMAL_TYPE_MAX] = {};	//�e�N�X�`���ւ̃|�C���^�ϐ�
CPlayer::ANIMAL_TYPE CPlayer::m_Animal = ANIMAL_TYPE_CHEETAH;	//�����̎�ޗp�̕ϐ�

//=============================================================================
//�v���C���[�N���X�̃R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority):CCharacter(nPriority)
{
	//�e�����o�����N���A
	m_nShotCounter = 0;	//���˃J�E���^
	m_move = VECTOR3_ZERO_INITIALIZE;	//�ړ���
	m_bSkill = false;	//�X�L�����������ǂ���
	m_nSkillCounter = 0;	//�X�L���J�E���^�[
	m_nStateCounter = 0;	//�_���[�W�J�E���^�[�ϐ�
	m_nDefense = 0;	//�h���
	m_pKeyBoard = NULL;
	m_pJoyStick = NULL;
	m_nSkillPoint = 0;
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
	if (pPlayer)
	{
		pPlayer->SetPos(pos);
		pPlayer->SetSize(size);
		//�v���C���[�N���X�̏����������Ăяo��
		pPlayer->Init();
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
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Player/Player_Cheetah.png", &m_pTexture[ANIMAL_TYPE_CHEETAH]);	//�`�[�^�[
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Player/Player_Gorilla.png", &m_pTexture[ANIMAL_TYPE_GORILLA]);	//�S����
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Player/Player_Turtle.png", &m_pTexture[ANIMAL_TYPE_TURTLE]);	//�J��

	return S_OK;
}

//=============================================================================
//�v���C���[�N���X�̃e�N�X�`���j������
//=============================================================================
void CPlayer::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < ANIMAL_TYPE_MAX; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (m_pTexture[nCntTex])
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//�v���C���[�N���X�̏���������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	//�̗͂̏�����
	InitLife(PLAYER_MAX_LIFE);

	//�X�L���|�C���g�̏�����
	m_nSkillPoint = MAX_SKILL;

	//2D�|���S���N���X�̏����������Ăяo��
	CScene2D::Init();

	//�A�j�}���^�C�v�̏�����
	m_Animal = ANIMAL_TYPE_CHEETAH;

	//�e�N�X�`���̊��蓖��
	CScene2D::BindTexture(m_pTexture[m_Animal]);

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);

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
	//CScene2D�̍X�V����
	CScene2D::Update();

	//�|���S���ʒu�̎擾
	m_pos = CScene2D::GetPos();

	//��Ԕ��菈���Ăяo��
	if (STATE_DEATH(CheckState()))
	{
		return;
	}

	//X�L�[�܂��̓W���C�X�e�B�b�NY�{�^���܂��͏\����L�[���������Ƃ�
	if (m_pKeyBoard->GetKeyBoardTrigger(DIK_X) || 
		m_pJoyStick->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_Y) || 
		m_pJoyStick->GetPushRelease(CROSS_KEY_UP))
	{
		//�����^�C�v�̕ύX�����Ăяo��
		ChangeAnimal();
	}

	//�X�y�[�X�L�[�܂��͉E�g���K�[2���������Ƃ�
	if (m_pKeyBoard->GetKeyBoardPress(DIK_SPACE) || 
		m_pJoyStick->GetJoyStickPress(CInputJoyStick::JOY_BUTTON_R_TRIGGER2))
	{
		//�o���b�g���ˏ����Ăяo��
		ShotBullet();
	}

	//�X�L���|�C���g��MAX_SKILL���傫���A����M�L�[�܂��̓W���C�X�e�B�b�NX�{�^���������ꂽ��
	if ((m_pKeyBoard->GetKeyBoardTrigger(DIK_M) || 
		m_pJoyStick->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_X)) &&
		m_nSkillPoint >= MAX_SKILL)
	{
		//�X�L������
		m_bSkill = true;

		//�X�L���|�C���g��0�ɂ���
		m_nSkillPoint = 0;
		CUi::GetSkillber()->SetGauge(m_nSkillPoint);
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

#ifdef _DEBUG
	//�f�o�b�O���̕\��
	DebugPrint();
#endif
}

//=============================================================================
//�v���C���[�N���X�̕`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	//2D�|���S���N���X�̕`�揈���Ăяo��
	CScene2D::Draw();
}

//=============================================================================
//�v���C���[�N���X�̃_���[�W����
//=============================================================================
void CPlayer::Damage(const int nAttack)
{
	int nDamage;

	//�_���[�W���󂯂Ă��Ȃ��Ƃ�
	if (GetState() != CHARACTER_STATE_DAMAGE)
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
			//�̗͂����炷
			int nNowLife = SubLife(nDamage);

			//���C�t�Q�[�W�ɂ��_���[�W�Ɠ��l��^����
			CGame::GetUi()->GetLifeber()->SubGauge(nDamage);

			//�X�L���̉��Z
			AddSkillPoint(10);
			
			//�̗͂�0��菬�������v���C���[�͎��S
			if (nNowLife <= 0)
			{
				//���S��Ԃֈڍs����
				SetState(CHARACTER_STATE_DIED);
			}
			//�̗͂�0���傫���Ƃ�
			else
			{
				//�_���[�W��Ԃֈڍs����
				SetState(CHARACTER_STATE_DAMAGE);
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
	if (GetState() != CHARACTER_STATE_DIED)
	{
		//�̗͂̉�
		AddLife(nRecovery);

		//���C�t�N���X�Ɍ��݂̗̑͂�^����
		CGame::GetUi()->GetLifeber()->AddGauge(nRecovery);
	}
}

//=============================================================================
//�v���C���[�N���X�̎��S����
//=============================================================================
void CPlayer::DiedProcess(void)
{
	CGame::SetGameState(CGame::GAME_STATE_GAME_OVER);

	//�I�������Ăяo��
	Uninit();
}

//=============================================================================
//�v���C���[�N���X�̃X�L���|�C���g�̎擾
//=============================================================================
int CPlayer::GetSkillPoint(void) const
{
	return m_nSkillPoint;
}

//=============================================================================
//�v���C���[�N���X�̃X�L���|�C���g�̉��Z
//=============================================================================
void CPlayer::AddSkillPoint(const int nAddSkill)
{
	m_nSkillPoint += nAddSkill;

	CUi::GetSkillber()->SetGauge(m_nSkillPoint);
	//�ő�l�𒴂�����C������
	if (m_nSkillPoint > MAX_SKILL)
	{
		m_nSkillPoint = MAX_SKILL;
	}
}

//=============================================================================
//�v���C���[�N���X�̓������̎擾
//=============================================================================
CPlayer::ANIMAL_TYPE CPlayer::GetAnimal(void)
{
	return m_Animal;
}

//=============================================================================
//�v���C���[�N���X�̈ړ�����
//=============================================================================
void CPlayer::Moving(void)
{
	//�X�e�B�b�N�����擾
	DIJOYSTATE joy;
	SecureZeroMemory(&joy, sizeof(joy));
	joy = m_pJoyStick->GetJoyStickInfo();

	//���a�̌v�Z
	float fRadius = sqrtf((PLAYER_MOVE_SPEED * PLAYER_MOVE_SPEED) + (PLAYER_MOVE_SPEED * PLAYER_MOVE_SPEED));

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

	//�v���C���[������ł��Ȃ��Ƃ�
	if (GetState() != CHARACTER_STATE_DIED)
	{
		//�����̏���
		m_move.x += (0.0f - m_move.x) * RATE_MOVE;
		m_move.y += (0.0f - m_move.y) * RATE_MOVE;

		//�|���S���̈ʒu�X�V
		m_pos.x += m_move.x;
		m_pos.y += m_move.y;
	}

	//�T�C�Y�̎擾
	D3DXVECTOR3 size = GetSize();

	//�v���C���[����ʊO�֍s���Ȃ��悤�ɂ��鏈��
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
//�����^�C�v�̕ύX����
//=============================================================================
void CPlayer::ChangeAnimal(void)
{
	//�A�j�}��UI��̃|���S�������擾
	CPolygon *pPolygon = CUi::GetAnimalUI()->GetPolygon();
	D3DXVECTOR3 size;

	switch (m_Animal)
	{
		//�`�[�^�[�̎�
	case ANIMAL_TYPE_CHEETAH:
		//�S�����ɕύX����
		m_Animal = ANIMAL_TYPE_GORILLA;

		//�ύX����T�C�Y���w��
		size = GORILLA_SIZE;

		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos, CHEETAH_SIZE, COLOR_PURPLE, 80, CEffect::EFFECT_TYPE_GORILLA, CScene::PRIORITY_PLAYER);

		//UI�e�N�X�`�����S�����ɕύX����
		pPolygon->SetType(CPolygon::TEX_TYPE_GORILLA);
		break;

		//�S�����̎�
	case ANIMAL_TYPE_GORILLA:
		//�J���ɂ���
		m_Animal = ANIMAL_TYPE_TURTLE;

		//�ύX����T�C�Y���w��
		size = TURTLE_SIZE;

		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos, CHEETAH_SIZE, COLOR_GREEN, 80, CEffect::EFFECT_TYPE_TURTLE, CScene::PRIORITY_PLAYER);

		//UI�e�N�X�`�����J���ɕύX����
		pPolygon->SetType(CPolygon::TEX_TYPE_TURTLE);

		//�h��͂̐ݒ�
		m_nDefense = DEFENSE_VALUE;
		break;

		//�J���̎�
	case ANIMAL_TYPE_TURTLE:
		//�`�[�^�[�ɂ���
		m_Animal = ANIMAL_TYPE_CHEETAH;

		//�ύX����T�C�Y���w��
		size = CHEETAH_SIZE;

		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos, CHEETAH_SIZE, COLOR_YELLOW, 80, CEffect::EFFECT_TYPE_CHEETAH, CScene::PRIORITY_PLAYER);

		//UI�e�N�X�`�����`�[�^�[�ɕύX����
		pPolygon->SetType(CPolygon::TEX_TYPE_CHEETAH);

		//�h��͂����ɖ߂�
		m_nDefense = 0;

		break;
	}

	//�e�N�X�`���̕ύX
	CScene2D::BindTexture(m_pTexture[m_Animal]);

	//�T�C�Y�̕ύX
	SetSize(size);

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
				float fShotAngle = 0.0f + fAngle * ((float)rand() / RAND_MAX - 0.5f);
				float fRad = D3DXToRadian(fShotAngle);

				//���ˑ��x
				float fSpeed = 15.0f + fSpeed_Step * ((float)rand() / RAND_MAX - 0.5f);

				//�e�̈ړ��ʂ��v�Z
				D3DXVECTOR3 bulletMove = D3DXVECTOR3(cosf(fShotAngle) * fSpeed, sinf(fShotAngle) * fSpeed, 0.0f);

				//�o���b�g�̐���
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
		CShield::Create(m_pos, SHILED_SIZE);

		//�X�L���g�p���I�t�ɂ���
		m_bSkill = false;

		//�X�L���J�E���^�[���N���A����
		m_nSkillCounter = 0;

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
		if (m_nShotCounter % 5 == 0)
		{
			//�o���b�g�̐���
			CBullet::Create(m_pos, BULLET_MOVE * 2, BULLET_SIZE / 1.2f, COLOR_YELLOW,
				CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_CHEETAH, CHEETAH_ATK, 100);
		}
		break;

		//�S�����̎�
	case ANIMAL_TYPE_GORILLA:
		if (m_nShotCounter % 30 == 0)
		{
			//�o���b�g�̐���
			CBullet::Create(m_pos, BULLET_MOVE / 1.5f, BULLET_SIZE*1.5f, COLOR_PURPLE,
				CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_GORILLA, GORILLA_ATK, 100);
		}
		break;

		//�J���̎�
	case ANIMAL_TYPE_TURTLE:
		if (m_nShotCounter % 20 == 0)
		{
			//�o���b�g�̐���
			CBullet::Create(m_pos, BULLET_MOVE, BULLET_SIZE, COLOR_GREEN,
				CBullet::BULLET_USER_PLAYER, CBullet::BULLET_TYPE_TURTLE, TURTLE_ATK, 100);
		}
		break;
	}

	//���˃J�E���g�A�b�v
	m_nShotCounter++;
}

#ifdef _DEBUG
//=============================================================================
//�v���C���[�N���X�̃f�o�b�O���̕\���Ƃ�܂Ƃ�
//=============================================================================
void CPlayer::DebugPrint(void)
{
	CDebugFont::Print(CDebugFont::DEBUG_LAYER_LEFT, "--------------------�v���C���[���--------------------\n");
	CDebugFont::Print(CDebugFont::DEBUG_LAYER_LEFT, "�y�ʒu�zX:%0.2f Y:%0.2f\n", m_pos.x, m_pos.y);
	CDebugFont::Print(CDebugFont::DEBUG_LAYER_LEFT, "�y�ړ��ʁzX:%0.2f Y:%0.2f\n", m_move.x, m_move.y);
	CDebugFont::Print(CDebugFont::DEBUG_LAYER_LEFT, "�y�ő�̗́z%d\n", GetMaxLife());
	CDebugFont::Print(CDebugFont::DEBUG_LAYER_LEFT, "�y���ݑ̗́z%d\n", GetLife());
	CDebugFont::Print(CDebugFont::DEBUG_LAYER_LEFT, "�y�X�L���|�C���g�z%d\n", m_nSkillPoint);
}
#endif