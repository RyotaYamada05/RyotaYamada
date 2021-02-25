//=============================================================================
//
// �{������ [bome.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "bome.h"
#include "manager.h"
#include "renderer.h"
#include "enemy.h"
#include "effect.h"
#include "boss.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define BOME_ADDSIZE 3.0f	//�T�C�Y���Z�p�萔
#define BOME_DAMAGE 1		//�{���̍U����
#define BOME_EFFECT_LIFE 20	//�{���ɕt������G�t�F�N�g�̃��C�t

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CBome::m_pTexture = NULL;

//=============================================================================
//�{���N���X�̃R���X�g���N�^
//=============================================================================
CBome::CBome(int nPriority):CScene2D(nPriority)
{
	//�e�����o�ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size - D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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

	//�T�C�Y�̐ݒ�
	m_size = size;

	//�ړ��ʂ̐ݒ�
	m_move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);

	//�F�̐ݒ�
	m_col = COLOR_PURPLE;
	SetColor(m_col);

	return S_OK;
}

//=============================================================================
//�{���N���X�̏I������
//=============================================================================
void CBome::Uninit(void)
{
	//2D�|���S���N���X�̏I�������Ăяo��
	CScene2D::Uninit();
}

//=============================================================================
//�{���N���X�̍X�V����
//=============================================================================
void CBome::Update(void)
{
	//�G�l�~�[�ƃ{�X�̃|�C���^�ϐ�
	CEnemy *pEnemy = NULL;
	CBoss *pBoss = NULL;

	switch (m_state)
	{
		//�ʏ���
	case BOME_STATE_NORMAL:

		//�ʒu�̎擾
		m_pos = CScene2D::GetPos();

		if (m_nCounter % 5 == 0)
		{
			//�G�t�F�N�g�̐���
			CEffect::Create(m_pos, BOME_SIZE, m_col, BOME_EFFECT_LIFE, CEffect::EFFECT_TYPE_BOME);
		}
		
		//�ʒu�̉��Z
		m_pos += m_move;

		//�ʒu�̐ݒ�
		CScene2D::SetPos(m_pos);

		//�G�l�~�[�Ƃ̓����蔻��
		pEnemy = (CEnemy*)CScene2D::GetCollision(CScene2D::OBJTYPE_ENEMY, m_pos, m_size);

		//�{�X�Ƃ̓����蔻��
		pBoss = (CBoss*)CScene2D::GetCollision(CScene2D::OBJTYPE_BOSS, m_pos, m_size);

		//�J�E���^�[��120�t���[���o�߂܂��́A�G�l�~�[�E�{�X�Ɠ������Ă����甚����Ԃֈڍs
		if (m_nCounter >= 120 || pEnemy != NULL || pBoss != NULL)
		{
			//������Ԃֈڍs
			m_state = BOME_STATE_EXPLOSION;

			//�J�E���^�[���N���A
			m_nCounter = 0;
		}
		break;

		//�������
	case BOME_STATE_EXPLOSION:

		//�T�C�Y�̊g��
		m_size += D3DXVECTOR3(BOME_ADDSIZE, BOME_ADDSIZE, 0.0f);

		//�T�C�Y�̐ݒ�
		CScene2D::SetSize(m_size);

		//���l������������
		m_col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a - 0.00444446f);

		//���N���X�֏���ݒ肷��
		CScene2D::SetColor(m_col);

		//���j�G�t�F�N�g�p
		D3DXCOLOR col;

		//���j�G�t�F�N�g�p�̐F�𗐐��Őݒ�
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

		//��x��5����
		for (int nCount = 0; nCount < 5; nCount++)
		{
			float fRandomX = (m_pos.x + m_size.x / 3) - (float)(rand() % (int)m_size.x / 1.5f);
			float fRandomY = (m_pos.y + m_size.y / 3) - (float)(rand() % (int)m_size.y / 1.5f);

			//�G�t�F�N�g�̐���
			CEffect::Create(D3DXVECTOR3(fRandomX, fRandomY, 0.0f), BOME_SIZE, col, BOME_EFFECT_LIFE, CEffect::EFFECT_TYPE_BOME);
		}

		//�G�l�~�[�Ƃ̓����蔻��
		pEnemy = (CEnemy*)CScene2D::GetCollision(CScene2D::OBJTYPE_ENEMY, m_pos, m_size);

		//�����蔻��
		if (pEnemy != NULL)
		{
			//�G�l�~�[�փ_���[�W��^����
			pEnemy->Damage(BOME_DAMAGE);
		}

		//�{�X�Ƃ̓����蔻��
		pBoss = (CBoss*)CScene2D::GetCollision(CScene2D::OBJTYPE_BOSS, m_pos, m_size);

		//�����蔻��
		if (pBoss != NULL)
		{
			//�{�X�փ_���[�W��^����
			pBoss->Damage(BOME_DAMAGE);
		}

		//�J�E���^�[��180�t���[���o�߂�����
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
