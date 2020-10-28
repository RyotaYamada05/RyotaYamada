//=============================================================================
//
// ���C������ [gauge.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "gauge.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CGauge::m_apTextuer[CGauge::GAUGE_TYPE_MAX] = {};

//=============================================================================
//�Q�[�W�N���X�̃R���X�g���N�^
//=============================================================================
CGauge::CGauge()
{
	//�e�����o�ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = GAUGE_TYPE_LIFE_GREEN;
	m_pVtxBuff = NULL;
	m_nGaugeNum = 0;
}

//=============================================================================
//�Q�[�W�N���X�̃f�X�g���N�^
//=============================================================================
CGauge::~CGauge()
{
}

//=============================================================================
//�Q�[�W�N���X�̃N���G�C�g����
//=============================================================================
CGauge * CGauge::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, int nGaugeNum, const GAUGE_TYPE type)
{
	//�Q�[�W�N���X�̃|�C���^�ϐ�
	CGauge *pGauge = NULL;

	//�������̊m��
	pGauge = new CGauge;

	//���������m�ۂł��Ă�����
	if (pGauge != NULL)
	{
		//�����������Ăяo��
		pGauge->Init(pos, size, nGaugeNum, type);
	}
	//�������m�ۂɎ��s������
	else
	{
		return NULL;
	}

	return pGauge;
}

//=============================================================================
//�Q�[�W�N���X�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CGauge::Load(void)
{
	//�f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/gaugeberHP.png", &m_apTextuer[GAUGE_TYPE_LIFE_GREEN]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/gaugeberHP_ORANGE.png", &m_apTextuer[GAUGE_TYPE_LIFE_ORANGE]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/gaugeberHP_RED.png", &m_apTextuer[GAUGE_TYPE_LIFE_RED]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/skillber.png", &m_apTextuer[GAUGE_TYPE_SKILL]);
	
	return S_OK;
}

//=============================================================================
//�Q�[�W�N���X�̃e�N�X�`���j������
//=============================================================================
void CGauge::UnLoad(void)
{
	for (int nCntTexture = 0; nCntTexture < GAUGE_TYPE_MAX; nCntTexture++)
	{
		//�e�N�X�`���j��
		if (m_apTextuer[nCntTexture] != NULL)
		{
			m_apTextuer[nCntTexture]->Release();
			m_apTextuer[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
//�Q�[�W�N���X�̏���������
//=============================================================================
HRESULT CGauge::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, int nGaugeNum, const GAUGE_TYPE type)
{
	//�f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�ʒu�̐ݒ�
	m_pos = pos;

	//�T�C�Y�̐ݒ�
	m_size = size;

	//�Q�[�W�̐ݒ�
	m_nGaugeNum = nGaugeNum;

	//�Q�[�W�^�C�v�̐ݒ�
	m_type = type;

	//���_�o�b�t�@�̍쐬
	pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,	//�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,	//�i�Œ�j
		FVF_VERTEX_2D,	//�t�H�[�}�b�g
		D3DPOOL_MANAGED,	//(�Œ�)
		&m_pVtxBuff,
		NULL
	);
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_nGaugeNum * m_size.x), m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_nGaugeNum * m_size.x), m_pos.y + m_size.y, 0.0f);

	//rhw�̐ݒ�i�l��1.0�ŌŒ�j
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//�Q�[�W�N���X�̏I������
//=============================================================================
void CGauge::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//�Q�[�W�N���X�̍X�V����
//=============================================================================
void CGauge::Update(void)
{
}

//=============================================================================
//�Q�[�W�N���X�̕`�揈��
//=============================================================================
void CGauge::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pD3DDevice->SetTexture(0, m_apTextuer[m_type]);

	// �|���S���̕`��
	pD3DDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,
		NUM_POLYGON);	//�|���S����

	//�e�N�X�`���̐ݒ�����ɖ߂�
	pD3DDevice->SetTexture(0, NULL);
}

//=============================================================================
//�Q�[�W�N���X�̐ݒ菈��
//=============================================================================
void CGauge::SetGauge(const int nGaugeNum)
{
	m_nGaugeNum = nGaugeNum;

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_nGaugeNum * m_size.x), m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_nGaugeNum * m_size.x), m_pos.y + m_size.y, 0.0f);

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�Q�[�W�N���X�̃^�C�v�ݒ菈��
//=============================================================================
void CGauge::SetType(const GAUGE_TYPE type)
{
   	m_type = type;
}
