//=============================================================================
//
// ���C������ [bome.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "bome.h"
#include "manager.h"
#include "renderer.h"
#include "enemy.h"
#include "effect.h"
#include "boss.h"

#define BOME_ADDSIZE 3.0f
#define BOME_DAMAGE 1

#define BOME_EFFECT_LIFE 20
//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CBome::m_pTexture = NULL;

//=============================================================================
//�{���N���X�̃R���X�g���N�^
//=============================================================================
CBome::CBome(int nPriority):CScene2D(nPriority)
{
	m_pos = D3DXVECTOR3();
	m_move = D3DXVECTOR3();
	m_size - D3DXVECTOR3();
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nCounter = 0;
	m_state = BOME_STATE_NORMAL;
}

//=============================================================================
//�{���N���X�̃f�X�g���N�^
//=============================================================================
CBome::~CBome()
{
}

//=============================================================================
//�{���N���X�̃N���G�C�g����
//=============================================================================
CBome * CBome::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�{���N���X�̃|�C���^�ϐ�
	CBome *pBome = NULL;

	//�������̊m��
	pBome = new CBome;

	//���������m�ۂł��Ă�����
	if (pBome != NULL)
	{
		pBome->Init(pos, size);
	}
	//�������m�ۂɎ��s�����Ƃ�
	else
	{
		return NULL;
	}

	return pBome;
}

//=============================================================================
//�{���N���X�̃e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CBome::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/ShockWave.png", &m_pTexture);
	return S_OK;
}

//=============================================================================
//�{���N���X�̃e�N�X�`���j������
//=============================================================================
void CBome::UnLoad(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//�{���N���X�̏���������
//=============================================================================
HRESULT CBome::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�����������Ăяo��
	CScene2D::Init(pos, size);

	//�e�N�X�`���̊��蓖��
	CScene2D::BindTexture(m_pTexture);

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_BOME);

	m_size = size;

	m_move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
	return S_OK;
}

//=============================================================================
//�{���N���X�̏I������
//=============================================================================
void CBome::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
//�{���N���X�̍X�V����
//=============================================================================
void CBome::Update(void)
{
	switch (m_state)
	{
		//�ʏ���
	case BOME_STATE_NORMAL:
		//�ʒu�̎擾
		m_pos = CScene2D::GetPos();
		if (m_nCounter % 5 == 0)
		{
			//�G�t�F�N�g�̐���
			CEffect::Create(m_pos, BOME_SIZE, COLOR_WHITE, BOME_EFFECT_LIFE, CEffect::EFFECT_TYPE_BOME);
		}
		
		//�ʒu�̉��Z
		m_pos += m_move;

		//�ʒu�̐ݒ�
		CScene2D::SetPos(m_pos);
		if (m_nCounter >= 120)
		{
			m_state = BOME_STATE_EXPLOSION;
			m_nCounter = 0;
		}
		break;

		//�������
	case BOME_STATE_EXPLOSION:
		//�T�C�Y�̊g��
		m_size += D3DXVECTOR3(BOME_ADDSIZE, BOME_ADDSIZE, 0.0f);

		//�T�C�Y�̐ݒ�
		CScene2D::SetSize(m_size);

		//�F�̐ݒ�
		m_col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a - 0.00444446f);

		CScene2D::SetColor(m_col);

		

		//�F�̐ݒ�
		D3DXCOLOR col;
		switch (rand() % 4 + 1)
		{
		case 1:
			col = COLOR_RED;
			break;
		case 2:
			col = COLOR_BLUE;
			break;
		case 3:
			col = COLOR_GREEN;
			break;
		case 4:
			col = COLOR_YELLOW;
			break;
		case 5:
			col = COLOR_PURPLE;
			break;
		}

		for (int nCount = 0; nCount < 5; nCount++)
		{
			float rX = (m_pos.x + m_size.x / 3) - (float)(rand() % (int)m_size.x / 1.5f);
			float rY = (m_pos.y + m_size.y / 3) - (float)(rand() % (int)m_size.y / 1.5f);

			//�G�t�F�N�g�̐���
			CEffect::Create(D3DXVECTOR3(rX, rY, 0.0f), BOME_SIZE, col, BOME_EFFECT_LIFE, CEffect::EFFECT_TYPE_BOME);
		}

		//�G�l�~�[�Ƃ̓����蔻��
		CEnemy*pEnemy = (CEnemy*)CScene2D::Collision(CScene2D::OBJTYPE_ENEMY, m_pos, m_size);

		//�����蔻��
		if (pEnemy != NULL)
		{
			//�G�l�~�[�փ_���[�W��^����
			pEnemy->Damage(BOME_DAMAGE);
		}

		//�{�X�Ƃ̓����蔻��
		CBoss*pBoss = (CBoss*)CScene2D::Collision(CScene2D::OBJTYPE_BOSS, m_pos, m_size);

		//�����蔻��
		if (pBoss != NULL)
		{
			//�{�X�փ_���[�W��^����
			pBoss->Damage(BOME_DAMAGE);
		}

		if (m_nCounter >= 180)
		{
			Uninit();
			return;
		}
		break;
	}

	//�J�E���^�[�A�b�v
	m_nCounter++;
}

//=============================================================================
//�{���N���X�̕`�揈��
//=============================================================================
void CBome::Draw(void)
{
	CScene2D::Draw();
}
