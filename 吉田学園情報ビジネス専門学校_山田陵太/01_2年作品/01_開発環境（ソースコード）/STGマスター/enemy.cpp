//=============================================================================
//
// ���C������ [enemy.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "enemy.h"
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
#define ATK_NORMAL 1

#define LIFE_NO_BULLET 2	//�e�������Ȃ��^�C�v�̗̑�
#define LIFE_NORMAL 10	//���ʂ̓G�̗̑�
#define LIFE_SNIPING 20	//�_�������^�C�v�̗�
#define LIFE_HOMING 20	//�z�[�~���O�^�C�v�̗̑�
#define LIFE_HADE 50	//�n�[�h�^�C�v�̗̑�
#define LIFE_DIVISION 30	//�����e�^�C�v�̗̑�

#define SCORE_NO_BULLET 100	//�e�������Ȃ��^�C�v�̃X�R�A
#define SCORE_NORMAL 200	//���ʂ̓G�̃X�R�A
#define SCORE_SNIPING 300	//�_�������^�C�v�X�R�A
#define SCORE_HOMING 300	//�z�[�~���O�^�C�v�̃X�R�A
#define SCORE_HADE 300	//�n�[�h�^�C�v�̃X�R�A
#define SCORE_DIVISION 300	//�����e�^�C�v�̃X�R�A

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[CEnemy::ENEMY_TYPE_MAX] = {};

//=============================================================================
//�G�l�~�[�N���X�̃R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(int nPriority):CScene2D(nPriority)
{
	//�e�����o�ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCounter = 0;
	m_nLife = 0;
	m_nScore = 0;
	m_State = ENEMY_STATE_NORMAL;
	m_nStateCounter = 0;
	m_nActionCounter = 0;
	m_DeadProcess = Dead_Process_NOEN;
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
CEnemy * CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const ENEMY_TYPE type,
	const ACTION_PATTERN action, Dead_Process DeadProcess)
{
	//�G�l�~�N���X�̃|�C���^�ϐ�
	CEnemy *pEnemy = NULL;

	//�C���X�^���X����
	pEnemy = new CEnemy;

	if (pEnemy != NULL)
	{
		//�����������Ăяo��
		pEnemy->Init(pos, size, type, action, DeadProcess);
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
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy.png", &m_pTexture[ENEMY_TYPE_NO_BULLET]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy.png", &m_pTexture[ENEMY_TYPE_NORMAL]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy.png", &m_pTexture[ENEMY_TYPE_SNIPING]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy001.png", &m_pTexture[ENEMY_TYPE_HADE]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy001.png", &m_pTexture[ENEMY_TYPE_DIVISION]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy001.png", &m_pTexture[ENEMY_TYPE_STOKER]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/enemy/enemy003.png", &m_pTexture[ENEMY_TYPE_HOMING]);
	
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
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//�G�l�~�[�N���X�̏���������
//=============================================================================
HRESULT CEnemy::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const ENEMY_TYPE type,
	const ACTION_PATTERN action, const Dead_Process DeadProcess)
{
	//2D�|���S���N���X�̏����������Ăяo��
	CScene2D::Init(pos, size);

	//�T�C�Y�̐ݒ�
	m_size = size;

	m_move.x = -ENEMY_MOVE_X;

	//��ނ̐ݒ�
	m_type = type;

	//�e�N�X�`���̊��蓖��
	CScene2D::BindTexture(m_pTexture[m_type]);

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_ENEMY);

	m_move.y = 3.0f;

	//�s���p�^�[���̐ݒ�
	m_action = action;

	//���S���̏�����ݒ�
	m_DeadProcess = DeadProcess;

	//���C�t�̐ݒ�
	SetData();
	
	//�|���S���ʒu�̎擾
	m_pos = CScene2D::GetPos();
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
	//��Ԕ��菈��
	if (DecisionState() == true)
	{
		return;
	}

	//�|���S���ʒu�̎擾
	m_pos = CScene2D::GetPos();

	//�s���p�^�[���ʂ̍X�V����
	ActionUpdate();

	//��p�̍X�V�����̌Ăяo��
	switch (m_type)
	{
	case ENEMY_TYPE_NO_BULLET:
		break;
	case ENEMY_TYPE_NORMAL:
		NormalUpdate();
		break;

	case ENEMY_TYPE_SNIPING:
		SnipingUpdate();
		break;

	case ENEMY_TYPE_HADE:
		HadeUpdate();
		break;
	}

	//�v���C���[�Ɠ����������ǂ����̏����擾
	CPlayer *pPlayer = (CPlayer*)CScene2D::Collision(CScene2D::OBJTYPE_PLAYER, m_pos, m_size);
	if (pPlayer != NULL)
	{
		pPlayer->Damage(10);
	}

	//�ʒu��ݒ肷��
	CScene2D::SetPos(m_pos);

	//��ʊO�֏o���Ƃ��폜����
	if (m_pos.x <= -100 || m_pos.y <= 0 || m_pos.y >= SCREEN_HEIGHT + 100)
	{
		if (CGame::GetGameState() != CGame::GAME_STATE_GAME_OVER &&  CGame::GetGameState() != CGame::GAME_STATE_GAME_CLEAR)
		{
			if (m_DeadProcess == Dead_Process_BOSS)
			{
				CGame::SetGameState(CGame::GAME_STATE_BOSS_START);
			}
		}
		//�I�������Ăяo��
		Uninit();

		return;
	}
}

void CEnemy::NormalUpdate(void)
{
	//�v���C���[�̈ʒu�����擾
	D3DXVECTOR3 Playerpos = CGame::GetPlayer()->GetPos();

	m_nCounter++;

	if (m_nCounter % 120 == 0 && m_pos.x >= Playerpos.x)
	{
		D3DXVECTOR3 move = D3DXVECTOR3(-10.0f,0.0f,0.0f);

		//�o���b�g�̐���
		CBullet::Create(m_pos, move, BULLET_SIZE, COLOR_RED,
			CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_NORMAL, ATK_NORMAL);
	}
}

void CEnemy::SnipingUpdate(void)
{
	//�v���C���[�̈ʒu�����擾
	D3DXVECTOR3 Playerpos = CGame::GetPlayer()->GetPos();

	m_nCounter++;

	if (m_nCounter % 80 == 0 && m_pos.x >= Playerpos.x)
	{
		//�p�x�Ƃ̌v�Z�i���W�A���j
		float fAngle = atan2f((Playerpos.y - m_pos.y), (Playerpos.x - m_pos.x));

		//�ړ��ʂ����߂�
		D3DXVECTOR3 move = D3DXVECTOR3(cosf(fAngle) * 10, sinf(fAngle) * 10, 0.0f);

		//�o���b�g�̐���
		CBullet::Create(m_pos, move, BULLET_SIZE, COLOR_RED,
			CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_NORMAL, ATK_NORMAL);
	}
}

void CEnemy::HadeUpdate(void)
{
	if(m_pos.x <= 1000)
	{
		int nNum = 10;	//�e�̐�
		float fAngle = 15.0f;	//�e�ƒe�̊Ԃ̊p�x
		float fRad_Step = D3DXToRadian(fAngle);	//���W�A���ɕϊ�

		//�[�̒e�ƒ��S�̒e�Ƃ̊Ԃ̊p�x���v�Z(�����E��ɂ���Č��ʕϓ�)
		float fRad = nNum % 2 ? -nNum / 2 * fRad_Step : (-nNum / 2 + 0.5f) * fRad_Step;

		m_nCounter++;

		if (m_nCounter % 120 == 0)
		{
			for (int nCount = 0; nCount < nNum; nCount++, fRad += fRad_Step)
			{
				CBullet::Create(m_pos, D3DXVECTOR3(-cosf(fRad) * 8,
					sinf(fRad) * 8, 0.0f), BULLET_SIZE,
					COLOR_BLUE, CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_NORMAL, 5);
			}
		}

		nNum = 9;

		//�[�̒e�ƒ��S�̒e�Ƃ̊Ԃ̊p�x���v�Z(�����E��ɂ���Č��ʕϓ�)
		fRad = nNum % 2 ? -nNum / 2 * fRad_Step : (-nNum / 2 + 0.5f) * fRad_Step;
		if (m_nCounter % 140 == 0)
		{
			for (int nCount = 0; nCount < nNum; nCount++, fRad += fRad_Step)
			{
				CBullet::Create(m_pos, D3DXVECTOR3(-cosf(fRad) * 5,
					sinf(fRad) * 5, 0.0f), BULLET_SIZE,
					COLOR_GREEN, CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_NORMAL, 5);
			}
		}
	}
}

void CEnemy::DivisionUpdate(void)
{
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
		if (m_nActionCounter == 300)
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
//�G�l�~�[�N���X�̕`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	//2D�|���S���N���X�̕`�揈���Ăяo��
	CScene2D::Draw();
}

void CEnemy::SetData(void)
{
	switch (m_type)
	{
	case ENEMY_TYPE_NO_BULLET:
		m_nLife = LIFE_NO_BULLET;
		m_nScore = SCORE_NO_BULLET;
		break;
	case ENEMY_TYPE_NORMAL:
		m_nLife = LIFE_NORMAL;
		m_nScore = SCORE_NORMAL;
		break;
	case ENEMY_TYPE_SNIPING:
		m_nLife = LIFE_SNIPING;
		m_nScore = SCORE_SNIPING;
		break;
	case ENEMY_TYPE_HOMING:
		m_nLife = LIFE_HOMING;
		m_nScore = SCORE_HOMING;
		break;
	case ENEMY_TYPE_HADE:
		m_nLife = LIFE_HADE;
		m_nScore = SCORE_HADE;
		break;
	case ENEMY_TYPE_DIVISION:
		m_nLife = LIFE_DIVISION;
		m_nScore = SCORE_DIVISION;
		break;

	case ENEMY_TYPE_STOKER:
		m_nLife = 10;
		m_nScore = 200;

	default:
		break;
	}
}

//=============================================================================
//�G�l�~�[�N���X�̃_���[�W����
//=============================================================================
void CEnemy::Damage(const int nDamage)
{
	//�̗͂��_���[�W�����炷
	m_nLife -= nDamage;

	//�̗͂�0��菬�������G�l�~�[�͎��S
	if (m_nLife <= 0)
	{
		m_State = ENEMY_STATE_DIED;
		
	}
	//�̗͂�0���傫���Ƃ�
	else
	{
		//�_���[�W��Ԃֈڍs����
		m_State = ENEMY_STATE_DAMAGE;
	}
}

//=============================================================================
//�G�l�~�[�N���X�̏�Ԕ��菈��
//=============================================================================
bool CEnemy::DecisionState(void)
{
	switch (m_State)
	{
		//�ʏ���
	case ENEMY_STATE_NORMAL:
		break;

		//�_���[�W���󂯂����
	case ENEMY_STATE_DAMAGE:
		SetColor(COLOR_ALPHA);

		m_nStateCounter++;

		if (m_nStateCounter >= 30)
		{
			SetColor(COLOR_WHITE);
			m_nStateCounter = 0;

			m_State = ENEMY_STATE_NORMAL;
		}
		else if (m_nStateCounter % 10 <= 5)
		{
			SetColor(COLOR_ALPHA);
		}
		else
		{
			SetColor(COLOR_WHITE);
		}
		break;

		//����ł�����
	case ENEMY_STATE_DIED:
		
		CScore *pScore = CGame::GetUi()->GetScore();

		//�X�R�A�̉��Z�����Ăяo��
		pScore->AddScore(m_nScore);

		//��������
		CExplosion::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), D3DXVECTOR3(EXPLOSION_SIZE_X * 2, EXPLOSION_SIZE_Y * 2, 0.0f), COLOR_RED);

		if (CGame::GetGameState() != CGame::GAME_STATE_GAME_OVER &&  CGame::GetGameState() != CGame::GAME_STATE_GAME_CLEAR)
		{
			//���S���̏��������s
			switch (m_DeadProcess)
			{
			case Dead_Process_NOEN:
				break;

			case Dead_Process_LIFE:
				//�̗͉񕜃A�C�e���̐���
				CItem::Create(m_pos, ITEM_SIZE, CItem::ITEM_TYPE_LIFE);
				break;

			case Dead_Process_SKILL:
				//�X�L���񕜃A�C�e���̐���
				CItem::Create(m_pos, ITEM_SIZE, CItem::ITEM_TYPE_SKILL);
				break;

			case Dead_Process_BOSS:
				CGame::SetGameState(CGame::GAME_STATE_BOSS_START);
				break;
			}
		}
		//�I�������Ăяo��
		Uninit();

		return true;
		break;
	}

	return false;
}

//=============================================================================
//�ʒu���̎擾
//=============================================================================
D3DXVECTOR3 CEnemy::GetPos(void) const
{
	return m_pos;
}
