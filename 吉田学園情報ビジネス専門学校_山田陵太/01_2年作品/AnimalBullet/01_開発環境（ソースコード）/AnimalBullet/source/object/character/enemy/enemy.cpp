//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "enemy.h"
#include "enemynomral.h"
#include "enemysniping.h"
#include "enemyhoming.h"
#include "middleboss.h"

#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "ui.h"
#include "player.h"
#include "game.h"
#include "item.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define CONTACT_DAMAGE 5	//�v���C���[�Ƃ̐ڐG���ɗ^����_���[�W

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[ENEMY_TYPE_MAX] = {};

//=============================================================================
//�G�l�~�[�N���X�̃R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(int nPriority):CCharacter(nPriority)
{
	//�e�����o�ϐ��̃N���A
	m_pos = VECTOR3_ZERO_INITIALIZE;
	m_move = VECTOR3_ZERO_INITIALIZE;
	m_nCounter = 0;
	m_nScore = 0;
	m_nActionCounter = 0;
	m_DeathAction = DEATH_ACTION_NOEN;
}

//=============================================================================
//�G�l�~�[�N���X�̃f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{
}

//=============================================================================
//�G�l�~�[�N���X�̃N���G�C�g����
//=============================================================================
CEnemy * CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const ENEMY_TYPE type, const int nLife, const int nScore,
	const ACTION_PATTERN action, DEATH_ACTION DeathAction)
{
	//�G�l�~�N���X�̃|�C���^�ϐ�
	CEnemy *pEnemy = NULL;

	//��ޕʂɃC���X�^���X����
	switch (type)
	{
	case ENEMY_TYPE_NO_BULLET:
		pEnemy = new CEnemy;
		break;

	case ENEMY_TYPE_NORMAL:
		pEnemy = new CEnemyNomral;
		break;

	case ENEMY_TYPE_SNIPING:
		pEnemy = new CEnemySniping;
		break;

	case ENEMY_TYPE_HOMING:
		pEnemy = new CEnemyHoming;
		break;

	case ENEMY_TYPE_MIDDLEBOSS:
		pEnemy = new CMiddleBoss;
		break;

	default:
		break;
	}
	
	if (pEnemy)
	{
		pEnemy->SetPos(pos);
		pEnemy->SetSize(size);
		pEnemy->InitLife(nLife);
		pEnemy->m_nScore = nScore;
		pEnemy->m_type = type;
		pEnemy->m_action = action;
		pEnemy->m_DeathAction = DeathAction;
		//�����������Ăяo��
		pEnemy->Init();
	}
	else
	{
		return NULL;
	}

	return pEnemy;
}

//=============================================================================
//�G�l�~�[�N���X�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CEnemy::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/Enemy_Cheetah.png", &m_pTexture[ENEMY_TYPE_NO_BULLET]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/Enemy_Gorilla.png", &m_pTexture[ENEMY_TYPE_NORMAL]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/Enemy_Cheetah.png", &m_pTexture[ENEMY_TYPE_SNIPING]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/Enemy_Gorilla.png", &m_pTexture[ENEMY_TYPE_MIDDLEBOSS]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/Enemy_Turtle.png", &m_pTexture[ENEMY_TYPE_HOMING]);

	return S_OK;
}

//=============================================================================
//�G�l�~�[�N���X�̃e�N�X�`���j������
//=============================================================================
void CEnemy::UnLoad(void)
{
	//�e�N�X�`���̔j��
	for (int nCount = 0; nCount < ENEMY_TYPE_MAX; nCount++)
	{
		if (m_pTexture[nCount])
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//�G�l�~�[�N���X�̏���������
//=============================================================================
HRESULT CEnemy::Init(void)
{
	//2D�|���S���N���X�̏����������Ăяo��
	CScene2D::Init();

	//�e�N�X�`���̊��蓖��
	CScene2D::BindTexture(m_pTexture[m_type]);

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_ENEMY);

	//�|���S���ʒu�̎擾
	m_pos = GetPos();

	//�ړ��ʂ̐ݒ�
	m_move = ENEMY_MOVE;

	return S_OK;
}

//=============================================================================
//�G�l�~�[�N���X�̏I������
//=============================================================================
void CEnemy::Uninit(void)
{
	//2D�|���S���N���X�̏I�������Ăяo��
	CScene2D::Uninit();
}

//=============================================================================
//�G�l�~�[�N���X�̍X�V����
//=============================================================================
void CEnemy::Update(void)
{
	//CScene2D�̍X�V����
	CScene2D::Update();

	//��Ԕ��菈���Ăяo��
	if (STATE_DEATH(CheckState()))
	{
		return;
	}

	//�|���S���ʒu�̎擾
	m_pos = CScene2D::GetPos();

	//�s���p�^�[���ʂ̍X�V����
	ActionUpdate();

	//�U������
	Attack();

	//�T�C�Y�̎擾
	D3DXVECTOR3 size = GetSize();

	//�v���C���[�Ɠ����������ǂ����̏����擾
	CPlayer *pPlayer = (CPlayer*)CScene2D::JudgeCollision(CScene::OBJTYPE_PLAYER, m_pos, size);
	
	if (pPlayer)
	{
		//�ڐG�_���[�W��^����
		pPlayer->Damage(CONTACT_DAMAGE);
	}

	//�ʒu��ݒ肷��
	CScene2D::SetPos(m_pos);

	//��ʊO�֏o���Ƃ��폜����
	if (m_pos.x <= -100 || m_pos.y <= 0 || m_pos.y >= SCREEN_HEIGHT + 100)
	{
		if (CGame::GetGameState() != CGame::GAME_STATE_GAME_OVER && 
			CGame::GetGameState() != CGame::GAME_STATE_GAME_CLEAR)
		{
			if (m_DeathAction == DEATH_ACTION_BOSS)
			{
				CGame::SetGameState(CGame::GAME_STATE_BOSS_START);
			}
		}

		//�I�������Ăяo��
		Uninit();

		return;
	}
}

//=============================================================================
//�G�l�~�[�N���X�̕`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	//2D�|���S���N���X�̕`�揈���Ăяo��
	CScene2D::Draw();
}

//=============================================================================
//�G�l�~�[�N���X�̍U������
//=============================================================================
void CEnemy::Attack(void)
{
	//ENEMY_TYPE_NO_BULLET�̏ꍇ�͍U�����s��Ȃ�
	return;
}

//=============================================================================
//�G�l�~�[�N���X�̍s���p�^�[���ʂ̍X�V����
//=============================================================================
void CEnemy::ActionUpdate(void)
{
	switch (m_action)
	{
	case ACTION_PATTERN_NOMRAL:
		break;

	//���ֈړ�����
	case ACTION_PATTERN_MOVE_DOWN:
		if (m_pos.x <= 800.0f)
		{
			//�ړ��ʂ��ʒu�ɐݒ�
			m_move.y = 2.0f;

			m_pos.y += m_move.y;
		}
		break;

	//��ֈړ�����
	case ACTION_PATTERN_MOVE_UP:
		if (m_pos.x <= 800.0f)
		{
			//�ړ��ʂ��ʒu�ɉ��Z
			m_move.y = -2.0f;

			m_pos.y += m_move.y;
		}
		break;

	//����̈ʒu�Ŏ~�܂�
	case ACTION_PATTERN_STOP:
		if (m_pos.x <= 1000)
		{
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		break;

	//����̈ʒu�Ŏ~�܂�A��莞�Ԍ�ދp
	case ACTION_PATTERN_STOP_RETURN:
		if (m_pos.x <= 1000)
		{
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		if (m_nActionCounter == 600)
		{
			m_move = D3DXVECTOR3(ENEMY_MOVE_X, 0.0f, 0.0f);
		}
		m_nActionCounter++;
		break;


	//�㉺�Ɉړ�����
	case ACTION_PATTERN_UP_AND_DOWN:
		if (m_pos.x <= 1000)
		{
			m_move.x = 0.0f;
			m_pos.y += m_move.y;
			if (m_pos.y >= 700 || m_pos.y <= 100)
			{
				m_move.y *= -1;
			}
		}
		break;

	//�ǐՃ^�C�v
	case ACTION_PATTERN_STALKER:
		//�v���C���[�̈ʒu�����擾
		D3DXVECTOR3 Playerpos = CGame::GetPlayer()->GetPos();
		//�p�x�Ƃ̌v�Z�i���W�A���j
		float fAngle = atan2f((Playerpos.y - m_pos.y), (Playerpos.x - m_pos.x));

		//�p�x�����ƂɈړ��ʂ����߂�
		D3DXVECTOR3 TargettoMove = D3DXVECTOR3(cosf(fAngle), sinf(fAngle), 0.0f);

		//���߂��ړ��ʂ𐳋K������
		D3DXVec3Normalize(&TargettoMove, &TargettoMove);

		//�ړ��ʂɐݒ�
		m_move = TargettoMove * 5;
		m_pos.y += m_move.y;
		break;
	}

	//�ړ��ʂ��ʒu�ɉ��Z
	m_pos.x += m_move.x;
}


//=============================================================================
//�G�l�~�[�N���X�̃_���[�W����
//=============================================================================
void CEnemy::Damage(const int nDamage)
{
	//�̗͂��_���[�W�����炷
	int nNowLife = SubLife(nDamage);

	//�̗͂�0��菬�������G�l�~�[�͎��S
	if (nNowLife <= 0)
	{
		SetState(CHARACTER_STATE_DIED);

	}
	//�̗͂�0���傫���Ƃ�
	else
	{
		//�_���[�W��Ԃֈڍs����
		SetState(CHARACTER_STATE_DAMAGE);
	}
}

//=============================================================================
//�G�l�~�[�N���X�̎��S����
//=============================================================================
void CEnemy::DiedProcess(void)
{
	//�X�R�A���̎擾
	CScore *pScore = CGame::GetUi()->GetScore();

	//�X�R�A�̉��Z�����Ăяo��
	pScore->AddScore(m_nScore);

	//��������
	CExplosion::Create(m_pos, D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f), COLOR_RED);

	if (CGame::GetGameState() != CGame::GAME_STATE_GAME_OVER && 
		CGame::GetGameState() != CGame::GAME_STATE_GAME_CLEAR)
	{
		//���S���̏��������s
		switch (m_DeathAction)
		{
		case DEATH_ACTION_NOEN:
			break;

		case DEATH_ACTION_LIFE:
			//�̗͉񕜃A�C�e���̐���
			CItem::Create(m_pos, ITEM_SIZE, CItem::ITEM_TYPE_LIFE);
			break;

		case DEATH_ACTION_SKILL:
			//�X�L���񕜃A�C�e���̐���
			CItem::Create(m_pos, ITEM_SIZE, CItem::ITEM_TYPE_SKILL);
			break;

		case DEATH_ACTION_BOSS:
			CGame::SetGameState(CGame::GAME_STATE_BOSS_START);
			break;
		}
	}
	//�I�������Ăяo��
	Uninit();
}