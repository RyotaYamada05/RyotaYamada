//=============================================================================
//
// ���C������ [boss.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "boss.h"
#include "bullet.h"
#include "game.h"
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "enemy.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define STOP_POS D3DXVECTOR3(1100.0f,SCREEN_HEIGHT / 2 , 0.0f)
#define BOSS_SPEED 5.0f
#define BOSS_LIFE 600
#define BOSS_SCORE 1000

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CBoss::m_pTexture = NULL;

//=============================================================================
//�{�X�N���X�̃R���X�g���N�^
//=============================================================================
CBoss::CBoss(int nPriority) :CScene2D(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = BOSS_STATE_NOEN;
	m_action = ACTION_PATTERN_STAERT;
	m_nLife = 0;
	m_nCount = 0;
	m_nScore = 0;
	m_nActionCounter = 0;
	m_nStateCounter = 0;
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
CBoss * CBoss::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CBoss *pBoss = NULL;

	pBoss = new CBoss;

	if (pBoss != NULL)
	{
		pBoss->Init(pos, size);
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
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//�{�X�N���X�̏���������
//=============================================================================
HRESULT CBoss::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	CScene2D::Init(pos, size);
	CScene2D::SetObjType(CScene2D::OBJTYPE_BOSS);
	CScene2D::BindTexture(m_pTexture);

	m_nLife = BOSS_LIFE;
	m_nScore = BOSS_SCORE;

	m_move = VectorMath(STOP_POS, BOSS_SPEED);
	return S_OK;
}

//=============================================================================
//�{�X�N���X�̏I������
//=============================================================================
void CBoss::Uninit(void)
{
	CExplosion::Create(m_pos, EXPLOSION_SIZE * 8.0f, COLOR_GREEN);

	CScene2D::Uninit();
}

//=============================================================================
//�{�X�N���X�̍X�V����
//=============================================================================
void CBoss::Update(void)
{
	if (DecisionState() == true)
	{
		Uninit();
		return;
	}

	//���S���Ă��Ȃ��Ƃ�
	if (m_state != BOSS_STATE_DIED)
	{
		//�ʒu�̎擾
		m_pos = CScene2D::GetPos();

		//�s�������Ăяo��
		Action();

		//�ʒu�̐ݒ�
		CScene2D::SetPos(m_pos);
	}
}

//=============================================================================
//�{�X�N���X�̕`�揈��
//=============================================================================
void CBoss::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
//�{�X�N���X�̍s������
//=============================================================================
void CBoss::Action(void)
{
	D3DXVECTOR3 move;

	switch (m_action)
	{
	case ACTION_PATTERN_STAERT:
		if (m_pos.x <= STOP_POS.x)
		{
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_action = ACTION_PATTERN_NOMRAL;
		}
		break;

	case ACTION_PATTERN_NOMRAL:
		
		if (m_nCount % 60 == 0)
		{
			move = VectorMath(CGame::GetPlayer()->GetPos(), BOSS_SPEED * 1.5f);

			//�o���b�g�̐���
			CBullet::Create(m_pos, move, BULLET_SIZE, COLOR_RED,
				CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_DIVISION, 10,40);
		}
		break;

	case ACTION_PATTERN_ACCELEU:
		if (m_nCount % 60 == 0)
		{
			move = VectorMath(CGame::GetPlayer()->GetPos(), 2.0f);

			//�o���b�g�̐���
			CBullet::Create(m_pos, move, BULLET_SIZE, COLOR_BLUE,
				CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_ACCELE, 10);

			CBullet::Create(D3DXVECTOR3(m_pos.x, m_pos.y + BULLET_SIZE_Y, m_pos.z), move, BULLET_SIZE, COLOR_BLUE,
				CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_ACCELE, 10);

			CBullet::Create(D3DXVECTOR3(m_pos.x, m_pos.y - BULLET_SIZE_Y, m_pos.z), move, BULLET_SIZE, COLOR_BLUE,
				CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_ACCELE, 10);
		}
		break;
	default:
		break;
	}

	if (m_nActionCounter == 300)
	{
		switch (m_action)
		{
		case ACTION_PATTERN_NOMRAL:
			m_action = ACTION_PATTERN_ACCELEU;

			break;

		case ACTION_PATTERN_ACCELEU:
			m_action = ACTION_PATTERN_NOMRAL;

			break;
		default:
			break;
		}
		m_nActionCounter = 0;
	}

	if (m_action != ACTION_PATTERN_STAERT)
	{
		if (m_nCount % 6000 == 0)
		{
			CEnemy::Create(D3DXVECTOR3(1300.0f, 200.0f, 0.0f),
				ENEMY_SIZE, CEnemy::ENEMY_TYPE_STOKER, CEnemy::ACTION_PATTERN_STALKER, CEnemy::Dead_Process_SKILL);

			CEnemy::Create(D3DXVECTOR3(1300.0f, 600.0f, 0.0f),
				ENEMY_SIZE, CEnemy::ENEMY_TYPE_STOKER, CEnemy::ACTION_PATTERN_STALKER, CEnemy::Dead_Process_LIFE);

		}
		else if (m_nCount % 600 == 0)
		{
			CEnemy::Create(D3DXVECTOR3(1300.0f, 200.0f, 0.0f),
				ENEMY_SIZE, CEnemy::ENEMY_TYPE_HOMING, CEnemy::ACTION_PATTERN_STOP_RETURN);

			CEnemy::Create(D3DXVECTOR3(1300.0f, 600.0f, 0.0f),
				ENEMY_SIZE, CEnemy::ENEMY_TYPE_HOMING, CEnemy::ACTION_PATTERN_STOP_RETURN);
		}

		m_nCount++;
	}
	m_nActionCounter++;

	//�ʒu�Ɉړ��ʂ�^����
	m_pos += m_move;
}

//=============================================================================
//�{�X�N���X�̃_���[�W����
//=============================================================================
void CBoss::Damage(const int nDamage)
{
	//���C�t�����炷
	m_nLife -= nDamage;

	//���C�t��0�ȉ��ɂȂ�����
	if (m_nLife <= 0)
	{
		SetColor(COLOR_WHITE);
		m_state = BOSS_STATE_DIED;
	}
	else
	{
		m_state = BOSS_STATE_DAMAGE;
	}
}

//=============================================================================
//�{�X�N���X�̏�ԏ���
//=============================================================================
bool CBoss::DecisionState(void)
{
	switch (m_state)
	{
	case BOSS_STATE_NOEN:
		break;

	//�_���[�W���
	case BOSS_STATE_DAMAGE:
		
		if (m_nStateCounter >= 30)
		{
			SetColor(COLOR_WHITE);
			m_nStateCounter = 0;

			m_state = BOSS_STATE_NOEN;
		}
		else if (m_nStateCounter % 10 <= 5)
		{
			SetColor(COLOR_ALPHA);
		}
		else
		{
			SetColor(COLOR_WHITE);
		}

		//�X�e�[�g�J�E���^�[�A�b�v
		m_nStateCounter++;
		break;

	//���S���
	case BOSS_STATE_DIED:
		if (m_nStateCounter >= 300)
		{
			CGame::SetGameState(CGame::GAME_STATE_GAME_CLEAR);
			return true;
		}
		else
		{
			//����
			if (m_nStateCounter % 20 == 0)
			{
				float X = (m_pos.x + m_size.x / 3) - (float)(rand() % (int)m_size.x / 1.5f);
				float Y = (m_pos.y + m_size.y / 3) - (float)(rand() % (int)m_size.y / 1.5f);

				D3DXVECTOR3 pos = D3DXVECTOR3(X, Y, 0.0f);
				CExplosion::Create(pos, EXPLOSION_SIZE * 2, COLOR_GREEN);
			}
		}
		
		//�X�e�[�g�J�E���^�[�A�b�v
		m_nStateCounter++;
		
		break;
	default:
		break;
	}
	
	return false;
}

//=============================================================================
//�{�X�N���X�̃x�N�g���v�Z����
//=============================================================================
D3DXVECTOR3 CBoss::VectorMath(D3DXVECTOR3 TargetPos, float fSpeed)
{
	//2�_�Ԃ̃x�N�g�������߂�i�I�_[�ڕW�n�_] - �n�_[���g�̈ʒu]�j
	D3DXVECTOR3 Vector = TargetPos - m_pos;

	//�x�N�g���̑傫�������߂�(c^2 = a^2 * b^2)
	float fVectorSize = D3DXVec3Length(&Vector);
	
	//�P�ʃx�N�g���p�ϐ�
	D3DXVECTOR3 UnitVector;

	//�P�ʃx�N�g�������߂�(���̃x�N�g�� / �x�N�g���̑傫��)
	D3DXVec3Normalize(&UnitVector,&Vector);

	//�P�ʃx�N�g���𑬓x�{���ĕԂ�(VectorNor*fSpeed)
	return UnitVector * fSpeed;
}
