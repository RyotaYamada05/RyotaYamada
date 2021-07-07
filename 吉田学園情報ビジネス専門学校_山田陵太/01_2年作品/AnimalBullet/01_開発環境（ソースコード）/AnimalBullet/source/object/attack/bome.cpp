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
#define BOME_ADD_SIZE 3.0f	//�T�C�Y���Z�p�萔
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
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	if (pBome)
	{
		pBome->SetPos(pos);
		pBome->SetSize(size);
		pBome->Init();
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
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//�{���N���X�̏���������
//=============================================================================
HRESULT CBome::Init(void)
{
	//�����������Ăяo��
	CScene2D::Init();

	//�e�N�X�`���̊��蓖��
	CScene2D::BindTexture(m_pTexture);

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_BOME);

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
	//CScene2D�̍X�V����
	CScene2D::Update();

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
		pEnemy = (CEnemy *)CScene2D::JudgeCollision(CScene2D::OBJTYPE_ENEMY, m_pos, GetSize());

		//�{�X�Ƃ̓����蔻��
		pBoss = (CBoss *)CScene2D::JudgeCollision(CScene2D::OBJTYPE_BOSS, m_pos, GetSize());

		//�J�E���^�[��120�t���[���o�߂܂��́A�G�l�~�[�E�{�X�Ɠ������Ă����甚����Ԃֈڍs
		if (m_nCounter >= 120 || pEnemy || pBoss)
		{
			//������Ԃֈڍs
			m_state = BOME_STATE_EXPLOSION;

			//�J�E���^�[���N���A
			m_nCounter = 0;
		}
		break;

		//�������
	case BOME_STATE_EXPLOSION:

		m_size = GetSize();
		m_col = GetColor();

		//�T�C�Y�̊g��
		m_size += D3DXVECTOR3(BOME_ADD_SIZE, BOME_ADD_SIZE, 0.0f);

		//�T�C�Y�̐ݒ�
		CScene2D::SetSize(m_size);

		//���l������������
		m_col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a - 0.005f);

		//���N���X�֏���ݒ肷��
		CScene2D::SetColor(m_col);

		//���j�G�t�F�N�g�p�̕ϐ�
		D3DXCOLOR randomCol;

		//���j�G�t�F�N�g�p�̐F�𗐐��Őݒ�
		switch (rand() % 4 + 1)
		{
		case 1:
			randomCol = COLOR_RED;
			break;
		case 2:
			randomCol = COLOR_BLUE;
			break;
		case 3:
			randomCol = COLOR_GREEN;
			break;
		case 4:
			randomCol = COLOR_YELLOW;
			break;
		case 5:
			randomCol = COLOR_PURPLE;
			break;
		}

		//��x��5����
		for (int nCount = 0; nCount < 5; nCount++)
		{
			float fRandomX = m_pos.x + (-m_size.x / 3) + rand() % (int)m_size.x / 1.5f;
			float fRandomY = m_pos.y + (-m_size.y / 3) + rand() % (int)m_size.y / 1.5f;

			//�G�t�F�N�g�̐���
			CEffect::Create(D3DXVECTOR3(fRandomX, fRandomY, 0.0f), BOME_SIZE, randomCol, BOME_EFFECT_LIFE, CEffect::EFFECT_TYPE_BOME);
		}

		//�G�l�~�[�Ƃ̓����蔻��
		JudgeCollisionEnemy();
	
		//�{�X�Ƃ̓����蔻��
		pBoss = (CBoss *)CScene2D::JudgeCollision(CScene2D::OBJTYPE_BOSS, m_pos, m_size);

		//�����蔻��
		if (pBoss)
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

//=============================================================================
//�{���N���X�̓G�Ƃ̓����蔻�菈��
//=============================================================================
void CBome::JudgeCollisionEnemy(void)
{
	for (int nCntPrity = 0; nCntPrity < CScene::PRIORITY_MAX; nCntPrity++)
	{
		//���X�g�̐擪���̎擾
		CScene *pScene = CScene::GetTop(nCntPrity);

		//NULL�ɂȂ�܂ŌJ��Ԃ�
		while (pScene)
		{
			//�I�u�W�F�N�g�̎�ނ��擾
			OBJTYPE type = pScene->GetObjType();

			//�I�u�W�F�N�g�^�C�v���ړI�̂��̂������ꍇ
			if (type == CScene2D::OBJTYPE_ENEMY)
			{
				//2D�|���S���N���X�|�C���^�ɃL���X�g���A�����擾
				CScene2D *pScene2D = (CScene2D *)pScene;

				//�^�[�Q�b�g�̈ʒu�ƃT�C�Y�����擾
				D3DXVECTOR3 targetPos = pScene2D->GetPos();
				D3DXVECTOR3 targetSize = pScene2D->GetSize();

				//�����蔻��
				if (targetPos.x + (targetSize.x / 2) >= m_pos.x - (m_size.x / 2) &&
					targetPos.x - (targetSize.x / 2) <= m_pos.x + (m_size.x / 2) &&
					targetPos.y + (targetSize.y / 2) >= m_pos.y - (m_size.y / 2) &&
					targetPos.y - (targetSize.y / 2) <= m_pos.y + (m_size.y / 2))
				{
					//CEnemy�̃|�C���^�^�փL���X�g
					CEnemy *pEnemy = (CEnemy *)pScene2D;

					//�_���[�W��^����
					pEnemy->Damage(BOME_DAMAGE);
				}
			}

			//���̏����擾
			pScene = pScene->GetNext();
		}
	}
}
