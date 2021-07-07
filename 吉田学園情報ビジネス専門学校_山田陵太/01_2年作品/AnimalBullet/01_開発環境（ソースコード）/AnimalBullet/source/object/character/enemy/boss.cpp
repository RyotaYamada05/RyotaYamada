//=============================================================================
//
// �{�X���� [boss.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "boss.h"
#include "manager.h"
#include "renderer.h"

#include "bullet.h"
#include "game.h"
#include "player.h"
#include "explosion.h"
#include "score.h"
#include "ui.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define ACCELEU_BULLET_NUM 3						//��x�ɐ�����������e�̐�
#define ACELEU_ATK 10								//�����e�̍U����
#define ACELEU_SPEED 1.0f							//�����e�̑��x
#define DIVISION_SPEED 7.5f							//�����e�̑��x
#define DIVISION_ATK 10								//�����e�̍U����
#define DIVISION_LIFE 40							//�����e�̗̑�
#define STOP_BOS_POSITION 1000.0f					//�{�X�̒�~�ʒu
#define DIED_GAME_CHANGE_FRAEM 300					//�{�X�����S���A�Q�[����Ԃ̕ύX�����s����t���[����
#define CHANGE_ATK_FRAEM 300						//�U���̃p�^�[���̕ύX�t���[����
#define BOSS_MOVE D3DXVECTOR3(-2.0f, 0.0f, 0.0f)	//�ړ���

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CBoss::m_pTexture = NULL;

//�����e�̈ʒu�ύX�p�萔
const float fAcceleValue[ACCELEU_BULLET_NUM] =
{
	BULLET_SIZE_Y ,0 ,-BULLET_SIZE_Y
};

//=============================================================================
//�{�X�N���X�̃R���X�g���N�^
//=============================================================================
CBoss::CBoss(int nPriority):CCharacter(nPriority)
{
	//�e�ϐ��̃N���A
	m_pos = VECTOR3_ZERO_INITIALIZE;
	m_move = VECTOR3_ZERO_INITIALIZE;
	m_nAtkCounter = 0;
	m_nChangeCounter = 0;
	m_nDiedCounter = 0;
	m_buttleState = BUTTLE_STATE_STRAT_UP;
	m_atkPattern = BOSS_ATTACK_PATTERN_NOMRAL;
}

//=============================================================================
//�{�X�N���X�̃f�X�g���N�^
//=============================================================================
CBoss::~CBoss()
{
}

//=============================================================================
//�{�X�N���X�̃N���G�C�g����
//=============================================================================
CBoss * CBoss::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nLife, const int nScore)
{
	//�{�X�N���X�̃|�C���^�ϐ�
	CBoss *pBoss = NULL;

	//�C���X�^���X����
	pBoss = new CBoss;

	if (pBoss)
	{
		//�e���̐ݒ�
		pBoss->SetPos(pos);
		pBoss->SetSize(size);
		pBoss->InitLife(nLife);
		pBoss->m_nScore = nScore;
		//������
		pBoss->Init();
	}
	else
	{
		return NULL;
	}

	return pBoss;
}

//=============================================================================
//�{�X�N���X�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CBoss::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/boss.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//�{�X�N���X�̃e�N�X�`���j������
//=============================================================================
void CBoss::UnLoad(void)
{
	if (m_pTexture)
	{
		//�e�N�X�`���̉��
		m_pTexture->Release();

		//�������̃N���A
		m_pTexture = NULL;
	}
}

//=============================================================================
//�{�X�N���X�̏���������
//=============================================================================
HRESULT CBoss::Init(void)
{
	//CScene2D�̏�����
	CScene2D::Init();

	//�e�N�X�`���̊��蓖��
	CScene2D::BindTexture(m_pTexture);

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_BOSS);

	//�ړ��ʂ̐ݒ�
	m_move = BOSS_MOVE;

	return S_OK;
}

//=============================================================================
//�{�X�N���X�̏I������
//=============================================================================
void CBoss::Uninit(void)
{
	//CScene2D�̏I��
	CScene2D::Uninit();
}

//=============================================================================
//�{�X�N���X�̍X�V����
//=============================================================================
void CBoss::Update(void)
{
	//CScene2D�̍X�V����
	CScene2D::Update();

	//���S���Ă��邩�ǂ�������
	if (STATE_DEATH(CheckState()))
	{
		return;
	}

	//�ʒu�̎擾
	m_pos = GetPos();
	
	//�퓬��Ԃɉ����ď��������s
	switch (m_buttleState)
	{
	//�퓬�J�n����
	case BUTTLE_STATE_STRAT_UP:

		if (m_pos.x <= STOP_BOS_POSITION)
		{
			//�ړ��ʂ�������
			m_move = VECTOR3_ZERO_INITIALIZE;

			//�퓬��Ԃֈڍs
			m_buttleState = BUTTLE_STATE_ON_BUTTLE;
		}

		break;

	//�퓬��
	case BUTTLE_STATE_ON_BUTTLE:

		//�U������
		Attack();

		break;

	default:
		break;
	}

	//�ړ��ʂ����Z
	m_pos += m_move;

	//�ʒu�̐ݒ�
	SetPos(m_pos);
}

//=============================================================================
//�{�X�N���X�̕`�揈��
//=============================================================================
void CBoss::Draw(void)
{
	//CScene2D�̕`��
	CScene2D::Draw();
}

//=============================================================================
//�{�X�N���X�̎��S������
//=============================================================================
void CBoss::DiedProcess(void)
{
	//�J�E���^�[���Q�[����ԕύX�t���[���ɓ��B������
	if (m_nDiedCounter >= DIED_GAME_CHANGE_FRAEM)
	{
		//�Ō�̑唚���̐���
		CExplosion::Create(
			m_pos,
			EXPLOSION_SIZE * 5,
			COLOR_GREEN);

		//�X�R�A���̎擾
		CScore *pScore = CGame::GetUi()->GetScore();

		//�X�R�A�̉��Z�����Ăяo��
		pScore->AddScore(m_nScore);

		//�Q�[����Ԃ��N���A�ɐݒ�
		CGame::SetGameState(CGame::GAME_STATE_GAME_CLEAR);

		//�I������
		Uninit();

		return;
	}
	else
	{
		if (m_nDiedCounter % 2 == 0)
		{
			//�T�C�Y�̎擾
			D3DXVECTOR3 size = GetSize();

			//�����_���Ȉʒu���v�Z
			float fXPos = m_pos.x + (-size.x / 2) + rand() % (int)size.x;
			float fYPos = m_pos.y + (-size.y / 2) + rand() % (int)size.y;

			D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, 0.0f);

			//�����̐���
			CExplosion::Create(
				pos,
				EXPLOSION_SIZE,
				COLOR_GREEN);
		}
	}

	//���S�J�E���^�[�̃A�b�v
	m_nDiedCounter++;
}

//=============================================================================
//�{�X�N���X�̃_���[�W����
//=============================================================================
void CBoss::Damage(const int nDamage)
{
	//�{�X���퓬������ԈȊO�̎�
	if (m_buttleState != BUTTLE_STATE_STRAT_UP)
	{
		//�̗͂��_���[�W�����炷
		int nNowLife = SubLife(nDamage);

		//�̗͂�0��菬�������{�X�͎��S
		if (nNowLife <= 0)
		{
			//���S��Ԃɂ���
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

//=============================================================================
//�{�X�N���X�̍U������
//=============================================================================
void CBoss::Attack(void)
{
	D3DXVECTOR3 bulletMove = VECTOR3_ZERO_INITIALIZE;	//�e�̈ړ���
	
	if (m_nAtkCounter % 60 == 0)
	{
		switch (m_atkPattern)
		{
		//�ʏ�U��
		case BOSS_ATTACK_PATTERN_NOMRAL:

			//�{�X����v���C���[�����ւ̃x�N�g���̌v�Z
			bulletMove = TargetWayVec3Calculation(CGame::GetPlayer()->GetPos(), DIVISION_SPEED);

			//�o���b�g�̐���
			CBullet::Create(
				m_pos, 
				bulletMove, 
				BULLET_SIZE, 
				COLOR_RED,
				CBullet::BULLET_USER_ENEMY, 
				CBullet::BULLET_TYPE_DIVISION, 
				DIVISION_ATK,
				DIVISION_LIFE);

			break;

		//�����e�U��
		case BOSS_ATTACK_PATTERN_ACCELEU_BULLET:

			//�{�X����v���C���[�����ւ̃x�N�g���̌v�Z
			bulletMove = TargetWayVec3Calculation(CGame::GetPlayer()->GetPos(), ACELEU_SPEED);

			//�����e�̐���
			for (int nCntAccele = 0; nCntAccele < ACCELEU_BULLET_NUM; nCntAccele++)
			{
				CBullet::Create(
					D3DXVECTOR3(m_pos.x, m_pos.y + fAcceleValue[nCntAccele], m_pos.z),
					bulletMove,
					BULLET_SIZE,
					COLOR_BLUE,
					CBullet::BULLET_USER_ENEMY,
					CBullet::BULLET_TYPE_ACCELE,
					ACELEU_ATK);
			}
			break;

		default:
			break;
		}
	}

	//�U���J�E���^�[�A�b�v
	m_nAtkCounter++;

	//�U���p�^�[���̕ύX����
	ChangeAttackPattern();
}

//=============================================================================
//�{�X�N���X�̍U���p�^�[���ύX����
//=============================================================================
void CBoss::ChangeAttackPattern(void)
{
	switch (m_atkPattern)
	{
	//�ʏ�U��
	case BOSS_ATTACK_PATTERN_NOMRAL:
		if (m_nChangeCounter == CHANGE_ATK_FRAEM)
		{
			//�����e�U���ɐ؂�ւ�
			m_atkPattern = BOSS_ATTACK_PATTERN_ACCELEU_BULLET;
			
			//�J�E���^�[�N���A
			m_nChangeCounter = 0;
			return;
		}
		break;
		
	//�����e�U��
	case BOSS_ATTACK_PATTERN_ACCELEU_BULLET:
		if (m_nChangeCounter == CHANGE_ATK_FRAEM)
		{
			//�ʏ�U���ɐ؂�ւ�
			m_atkPattern = BOSS_ATTACK_PATTERN_NOMRAL;
			
			//�J�E���^�[�N���A
			m_nChangeCounter = 0;
			return;
		}
		break;

	default:
		break;
	}

	//�J�E���g�A�b�v
	m_nChangeCounter++;
}

//=============================================================================
//�{�X�N���X�̃^�[�Q�b�g�����ւ�3�����x�N�g���v�Z����
//=============================================================================
D3DXVECTOR3 CBoss::TargetWayVec3Calculation(D3DXVECTOR3 TargetPos, float fSpeed)
{
	//2�_�Ԃ̃x�N�g�������߂�i�I�_[�ڕW�n�_] - �n�_[���g�̈ʒu]�j
	D3DXVECTOR3 vector = TargetPos - GetPos();

	//�P�ʃx�N�g���p�ϐ�
	D3DXVECTOR3 unitVector = VECTOR3_ZERO_INITIALIZE;

	//�P�ʃx�N�g�������߂�(���̃x�N�g�� / �x�N�g���̑傫��)
	D3DXVec3Normalize(&unitVector, &vector);

	//�P�ʃx�N�g���𑬓x�{���ĕԂ�(VectorNor*fSpeed)
	return unitVector * fSpeed;
}