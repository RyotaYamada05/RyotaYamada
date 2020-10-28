//=============================================================================
//
// ���C������ [life.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "life.h"
#include "gauge.h"
#include "polygon.h"

//=============================================================================
//���C�t�N���X�̃R���X�g���N�^
//=============================================================================
CLife::CLife(int nPriority) :CScene(nPriority)
{
	//�����o�ϐ��̃N���A
	m_pGauge = NULL;
	m_Polygon = NULL;
	m_nLife = 0;
}

//=============================================================================
//���C�t�N���X�̃f�X�g���N�^
//=============================================================================
CLife::~CLife()
{
}

//=============================================================================
//���C�t�N���X�̃N���G�C�g����
//=============================================================================
CLife * CLife::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//���C�t�N���X�̃|�C���^�ϐ�
	CLife *m_pLife = NULL;

	//�������̊m��
	m_pLife = new CLife;

	//���������m�ۂł��Ă�����
	if (m_pLife != NULL)
	{
		//�����������Ăяo��
		m_pLife->Init(pos, size);
	}
	//�������m�ۂɎ��s�����Ƃ�
	else
	{
		return NULL;
	}
	return m_pLife;
}

//=============================================================================
//���C�t�N���X�̏���������
//=============================================================================
HRESULT CLife::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�|���S���N���X�𐶐�
	m_Polygon = CPolygon::Create(D3DXVECTOR3(0.0f + (+GAUGE_SIZE_X / 2), 0.0f + (GAUGE_SIZE_Y / 2), 0.0f),
		GAUGE_SIZE, CPolygon::TEX_TYPE_GAUGE_UI);

	//�Q�[�W�N���X�̐���
	m_pGauge = CGauge::Create(D3DXVECTOR3(100.0f, 11.0f, 0.0f), LIFE_GAGUE_SIZE, m_nLife, CGauge::GAUGE_TYPE_LIFE_GREEN);

	return S_OK;
}

//=============================================================================
//���C�t�N���X�̏I������
//=============================================================================
void CLife::Uninit(void)
{
	if (m_Polygon != NULL)
	{
		//�|���S���N���X�̏I�������Ăяo��
		m_Polygon->Uninit();

		//�������̍폜
		delete m_Polygon;

		//�������̃N���A
		m_Polygon = NULL;
	}

	if (m_pGauge != NULL)
	{
		//�Q�[�W�N���X�̏I�������Ăяo��
		m_pGauge->Uninit();

		//�������̍폜
		delete m_pGauge;

		//�������̃N���A
		m_pGauge = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//���C�t�N���X�̍X�V����
//=============================================================================
void CLife::Update(void)
{
}

//=============================================================================
//���C�t�N���X�̕`�揈��
//=============================================================================
void CLife::Draw(void)
{
	if (m_pGauge != NULL)
	{
		//�Q�[�W�N���X�̕`�揈���Ăяo��
		m_pGauge->Draw();
	}
	if (m_Polygon != NULL)
	{
		//�|���S���N���X�̕`�揈���Ăяo��
		m_Polygon->Draw();
	}
}

//=============================================================================
//���C�t�N���X�̐ݒ菈��
//=============================================================================
void CLife::SetLife(int nLife)
{
	m_nLife = nLife;

	//�̗͂�50�ȉ��̎�
	if (m_nLife >= 50)
	{
		//�e�N�X�`����ύX����
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_GREEN);
	}
	//�̗͂�15�ȉ��̎�
	else if (m_nLife >= 15)
	{
		//�e�N�X�`����ύX����
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_ORANGE);
	}
	else
	{
		//�e�N�X�`����ύX����
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_RED);
	}

	//�̗͂�100���傫���Ȃ����Ƃ�
	if (m_nLife > 100)
	{
		m_nLife = 100;
	}
	//�̗͂�0��菬�����Ȃ����Ƃ�
	else if (m_nLife < 0)
	{
		m_nLife = 0;
	}
	//�Q�[�W�N���X�֌��ݑ̗͂�ݒ肷��
	m_pGauge->SetGauge(m_nLife);
}

//=============================================================================
//���C�t�N���X�̗͉̑��Z����
//=============================================================================
void CLife::AddLife(int nLife)
{
	//�̗͂̉��Z
	m_nLife += nLife;

	//�̗͂�50�ȉ��̎�
	if (m_nLife >= 50)
	{
		//�e�N�X�`����ύX����
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_GREEN);
	}
	//�̗͂�15�ȉ��̎�
	else if (m_nLife >= 15)
	{
		//�e�N�X�`����ύX����
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_ORANGE);
	}

	//�̗͂�100���傫���Ȃ����Ƃ�
	if (m_nLife > 100)
	{
		m_nLife = 100;
	}

	//�Q�[�W�N���X�֌��ݑ̗͂�ݒ肷��
	m_pGauge->SetGauge(m_nLife);
}

//=============================================================================
//���C�t�N���X�̗̑͌��Z����
//=============================================================================
void CLife::SubLife(int nLife)
{
	//�̗͂̌��Z
	m_nLife -= nLife;

	//�̗͂�15��菬������
	if (m_nLife < 15)
	{
		//�e�N�X�`����ύX����
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_RED);
	}
	//�̗͂�50��菬������
	else if (m_nLife < 50)
	{
		//�e�N�X�`����ύX����
		m_pGauge->SetType(CGauge::GAUGE_TYPE_LIFE_ORANGE);
	}

	//�̗͂�0��菬�����Ȃ����Ƃ�
	if (m_nLife < 0)
	{
		m_nLife = 0;
	}

	//�Q�[�W�N���X�֌��ݑ̗͂�ݒ肷��
	m_pGauge->SetGauge(m_nLife);
}


