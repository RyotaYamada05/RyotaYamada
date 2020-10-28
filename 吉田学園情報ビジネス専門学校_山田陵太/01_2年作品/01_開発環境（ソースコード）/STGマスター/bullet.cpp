//=============================================================================
//
// ���C������ [bullet.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "sound.h"
#include "effect.h"
#include "game.h"
#include "boss.h"

#define HOMING_SPEED 5
//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//=============================================================================
//�o���b�g�N���X�̃R���X�g���N�^
//=============================================================================
CBullet::CBullet(int nPriority) :CScene2D(nPriority)
{
	//�����o�ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ���
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�T�C�Y
	m_nLife = 0;	//���C�t
	m_nAtk = 0;	//�U����
	m_nEffectCounter = 0;
	m_bulletUser = BULLET_USER_NONE;	//�g�p��
	m_bulletType = BULLET_TYPE_NORMAL;	//���
}

//=============================================================================
//�o���b�g�N���X�̃f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
//�o���b�g�N���X�̃N���G�C�g����
//=============================================================================
CBullet * CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const D3DXCOLOR col,
	const BULLET_USER bulletUser, const BULLET_TYPE bulletType, int nAtk, int nLife)
{
	//�o���b�g�N���X�̃|�C���^�ϐ�
	CBullet *pBullet = NULL;

	//�C���X�^���X����
	pBullet = new CBullet;

	//���������m�ۂł��Ă�����
	if (pBullet != NULL)
	{
		//�����������Ăяo��
		pBullet->Init(pos, move, size, col, bulletUser, bulletType, nAtk, nLife);
	}

	return pBullet;
}

//=============================================================================
//�o���b�g�N���X�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CBullet::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Bullet.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//�o���b�g�N���X�̃e�N�X�`���̔j������
//=============================================================================
void CBullet::UnLoad(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//�o���b�g�N���X�̏���������
//=============================================================================
HRESULT CBullet::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const D3DXCOLOR col,
	const BULLET_USER bulletUser, const BULLET_TYPE bulletType, int nAtk, int nLife)
{
	//�����������Ăяo��
	CScene2D::Init(pos, size);

	//�e�N�X�`���̊��蓖��
	CScene2D::BindTexture(m_pTexture);

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_BULLET);

	//�ړ��ʂ̐ݒ�
	m_move = move;

	//�T�C�Y�̐ݒ�
	m_size = size;

	//�����̐ݒ�
	m_nLife = nLife;

	//�F�̐ݒ�
	m_col = col;
	CScene2D::SetColor(m_col);

	//�g�p�҂̐ݒ�
	m_bulletUser = bulletUser;

	//�e�̎�ނ̐ݒ�
	m_bulletType = bulletType;

	//�U���͂̐ݒ�
	m_nAtk = nAtk;

	//�T�E���h�N���X�̏��擾
	CSound *pSound = CManager::GetSound();

	//���ˉ��̍Đ�
	//pSound->Play(CSound::SOUND_LABEL_SHOUT);

	return S_OK;
}

//=============================================================================
//�o���b�g�N���X�̏I������
//=============================================================================
void CBullet::Uninit(void)
{
	//2D�|���S���N���X�̏I�������Ăяo��
	CScene2D::Uninit();
}

//=============================================================================
//�o���b�g�N���X�̍X�V����
//=============================================================================
void CBullet::Update(void)
{
	//�ʒu�̎擾
	m_pos = CScene2D::GetPos();

	switch (m_bulletType)
	{
	case BULLET_TYPE_NORMAL:
		break;
	case BULLET_TYPE_HOMING:
		HomingUpdate();
		break;

	case BULLET_TYPE_DIVISION:
		DivisionUpdate();
		break;

	case BULLET_TYPE_ACCELE:
		ACCELEUpdetae();
		break;
	default:
		break;
	}

	if (m_nEffectCounter % 2 == 0)
	{
		EffectsByType();
	}
	//�ʒu�Ɉړ��ʂ����Z
	m_pos += m_move;

	//�X�V�ʒu�̐ݒ�
	CScene2D::SetPos(m_pos);

	//�g�p�҂��G�l�~�[�������ꍇ
	if (m_bulletUser == BULLET_USER_ENEMY)
	{
		CScene2D *pScene2D = CScene2D::Collision(CScene2D::OBJTYPE_PLAYER, m_pos,m_size);
		//�����蔻��
		if (pScene2D != NULL)
		{
			//��������
			CExplosion::Create(m_pos, D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f), COLOR_CYAN);

			CPlayer *pPlayer = (CPlayer*)pScene2D;

			//�v���C���[�փ_���[�W��^����
			pPlayer->Damage(m_nAtk);
			//�I�������Ăяo��
			Uninit();

			return;
		}
	}
	//�g�p�҂��v���C���[�������ꍇ
	else if (m_bulletUser == BULLET_USER_PLAYER)
	{
		CScene2D *pScene2D = CScene2D::Collision(CScene2D::OBJTYPE_ENEMY, m_pos, m_size);

		//�����蔻��
		if (pScene2D != NULL)
		{
			CEnemy*pEnemy = (CEnemy*)pScene2D;

			//��������
			CExplosion::Create(m_pos, D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f), COLOR_CYAN);

			//�G�l�~�[�փ_���[�W��^����
			pEnemy->Damage(m_nAtk);
			//�I�������Ăяo��
			Uninit();

			return;
		}

		pScene2D = CScene2D::Collision(CScene2D::OBJTYPE_BOSS, m_pos, m_size);

		//�����蔻��
		if (pScene2D != NULL)
		{
			CBoss*pBoss = (CBoss*)pScene2D;
			//��������
			CExplosion::Create(m_pos, D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f), COLOR_CYAN);

			//�{�X�փ_���[�W��^����
			pBoss->Damage(m_nAtk);

			//�I�������Ăяo��
			Uninit();

			return;
		}
	}

	//�o���b�g�̎������s�����Ƃ�
	if (m_nLife <= 0)
	{
		//��������
		CExplosion::Create(m_pos, D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f), m_col);

		//�I�������Ăяo��
		Uninit();

		return;
	}

	//�o���b�g����ʊO�ɏo���Ƃ�
	if (m_pos.x <= 0 || m_pos.x >= SCREEN_WIDTH || m_pos.y <= 0 || m_pos.y >= SCREEN_HEIGHT)
	{
		//�I�������Ăяo��
		Uninit();

		return;
	}

	//�G�t�F�N�g�J�E���^�[�A�b�v
	m_nEffectCounter++;

	//���C�t�����炷
	m_nLife--;
}

//=============================================================================
//�o���b�g�N���X�̕`�揈��
//=============================================================================
void CBullet::Draw(void)
{
	//2D�|���S���N���X�̕`�揈���Ăяo��
	CScene2D::Draw();
}

//=============================================================================
//�o���b�g�N���X�̃z�[�~���O�X�V����
//=============================================================================
void CBullet::HomingUpdate(void)
{
	//�v���C���[�̈ʒu�����擾
	D3DXVECTOR3 Playerpos = CGame::GetPlayer()->GetPos();

	if (m_pos.x >= Playerpos.x)
	{
		if (m_nEffectCounter <= 90)
		{
			//�p�x�Ƃ̌v�Z�i���W�A���j
			float fAngle = atan2f((Playerpos.y - m_pos.y), (Playerpos.x - m_pos.x));
		
			//�ړ��ʂ����߂�
			m_move = D3DXVECTOR3(cosf(fAngle) * HOMING_SPEED, sinf(fAngle) * HOMING_SPEED, 0.0f);
		}
	}
}

//=============================================================================
//�o���b�g�N���X�̕����e�̍X�V����
//=============================================================================
void CBullet::DivisionUpdate(void)
{
	if (m_nLife <= 0)
	{
		int nNum = 10 + rand() % 2;	//�e�̐�
		float fAngle = 18.0f;	//�e�ƒe�̊Ԃ̊p�x
		float fRad_Step = D3DXToRadian(fAngle);	//���W�A���ɕϊ�

		//�[�̒e�ƒ��S�̒e�Ƃ̊Ԃ̊p�x���v�Z(�����E��ɂ���Č��ʕϓ�)
		float fRad = nNum % 2 ? -nNum / 2 * fRad_Step : (-nNum / 2 + 0.5f) * fRad_Step;

		for (int nCount = 0; nCount < nNum; nCount++, fRad += fRad_Step)
		{
			CBullet::Create(m_pos, D3DXVECTOR3(-cosf(fRad) * 5,
				sinf(fRad) * 5, 0.0f), BULLET_SIZE,
				m_col, CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_NORMAL, 5);
		}
	}
}

void CBullet::ACCELEUpdetae(void)
{
	D3DXVECTOR3 no;
	D3DXVec3Normalize(&no, &m_move);

	m_move.x += 0.5f*no.x;
	m_move.y += 0.5f*no.y;

}

void CBullet::EffectsByType(void)
{
	switch (m_bulletType)
	{
		//�`�[�^�[�̎�
	case BULLET_TYPE_CHEETAH:
		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos, D3DXVECTOR3(m_size.x , m_size.y, m_size.z), m_col, 10, CEffect::EFFECT_TYPE_BULLET);
		break;
		//�S�����̎�
	case BULLET_TYPE_GORILLA:
		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos, D3DXVECTOR3(m_size.x , m_size.y, m_size.z), m_col, 10, CEffect::EFFECT_TYPE_BULLET);
		break;

		//�J���̎�
	case BULLET_TYPE_TURTLE:
	case BULLET_TYPE_NORMAL:
	case BULLET_TYPE_ACCELE:
	case BULLET_TYPE_HOMING:
	case BULLET_TYPE_DIVISION:
		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos, D3DXVECTOR3(m_size.x , m_size.y, m_size.z), m_col, 10, CEffect::EFFECT_TYPE_BULLET);
		break;
	default:
		break;
	}
}

