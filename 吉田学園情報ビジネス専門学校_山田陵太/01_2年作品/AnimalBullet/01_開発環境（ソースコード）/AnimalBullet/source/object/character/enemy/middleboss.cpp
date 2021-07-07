//=============================================================================
//
// ���{�X���� [middleboss.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "middleboss.h"
#include "game.h"
#include "player.h"
#include "bullet.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define Middle_ATK 10 //���{�X�̍U����
#define Middle_BULLET_SPEED 8.0f //���{�X�̒e�̑��x

//=============================================================================
//���{�X�N���X�̃R���X�g���N�^
//=============================================================================
CMiddleBoss::CMiddleBoss()
{
}

//=============================================================================
//���{�X�N���X�̃f�X�g���N�^
//=============================================================================
CMiddleBoss::~CMiddleBoss()
{
}

//=============================================================================
//���{�X�N���X�̏���������
//=============================================================================
HRESULT CMiddleBoss::Init(void)
{
	CEnemy::Init();
	return S_OK;
}

//=============================================================================
//���{�X�N���X�̏I������
//=============================================================================
void CMiddleBoss::Uninit(void)
{
	CEnemy::Uninit();
}

//=============================================================================
//���{�X�N���X�̍X�V����
//=============================================================================
void CMiddleBoss::Update(void)
{
	CEnemy::Update();
}

//=============================================================================
//���{�X�N���X�̕`�揈��
//=============================================================================
void CMiddleBoss::Draw(void)
{
	CEnemy::Draw();
}

//=============================================================================
//���{�X�N���X�̍U������
//=============================================================================
void CMiddleBoss::Attack(void)
{
	if (m_pos.x <= 1000)
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
				CBullet::Create(m_pos, D3DXVECTOR3(-cosf(fRad) * Middle_BULLET_SPEED,
					sinf(fRad) * Middle_BULLET_SPEED, 0.0f), BULLET_SIZE,
					COLOR_BLUE, CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_NORMAL, Middle_ATK);
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
					COLOR_GREEN, CBullet::BULLET_USER_ENEMY, CBullet::BULLET_TYPE_NORMAL, Middle_ATK);
			}
		}
	}
}
