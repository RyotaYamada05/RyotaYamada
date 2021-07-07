//=============================================================================
//
// �Q�[�W���� [gauge.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "gauge.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define GAUGE_RATE_SPPED 10.0f	//�Q�[�W�̑��x

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CGauge::m_pTextuer = NULL;

//=============================================================================
//�Q�[�W�N���X�̃R���X�g���N�^
//=============================================================================
CGauge::CGauge(int nPriority) : CScene(nPriority)
{
	//�e�����o�ϐ��̃N���A
	m_pos = VECTOR3_ZERO_INITIALIZE;
	m_size = VECTOR3_ZERO_INITIALIZE;
	m_pVtxBuff = NULL;
	m_nGaugeNum = 0;
	m_nMaxGaugeNum = 0;
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
CGauge * CGauge::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col,
	const int nGaugeNum, const int nMaxGaugeNum)
{
	//�Q�[�W�N���X�̃|�C���^�ϐ�
	CGauge *pGauge = NULL;

	//�������̊m��
	pGauge = new CGauge;

	//���������m�ۂł��Ă�����
	if (pGauge)
	{
		pGauge->SetPos(pos);
		pGauge->SetSize(size);
		pGauge->SetColor(col);
		pGauge->SetMaxGauge(nMaxGaugeNum);
		pGauge->SetGauge(nGaugeNum);
		//�����������Ăяo��
		pGauge->Init();
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
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/gaugeber.png", &m_pTextuer);
	
	return S_OK;
}

//=============================================================================
//�Q�[�W�N���X�̃e�N�X�`���j������
//=============================================================================
void CGauge::UnLoad(void)
{
	//�e�N�X�`���j��
	if (m_pTextuer)
	{
		m_pTextuer->Release();
		m_pTextuer = NULL;
	}

}

//=============================================================================
//�Q�[�W�N���X�̏���������
//=============================================================================
HRESULT CGauge::Init()
{
	//�f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

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
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + ((float)m_nGaugeNum / (float)m_nMaxGaugeNum) * m_size.x, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + ((float)m_nGaugeNum / (float)m_nMaxGaugeNum) * m_size.x, m_pos.y + m_size.y, 0.0f);;

	//rhw�̐ݒ�i�l��1.0�ŌŒ�j
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

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
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	SetDeathFlag();
}

//=============================================================================
//�Q�[�W�N���X�̍X�V����
//=============================================================================
void CGauge::Update(void)
{
	//���_�̍X�V����
	VertexUpdate();
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
	pD3DDevice->SetTexture(0, m_pTextuer);

	// �|���S���̕`��
	pD3DDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,
		NUM_POLYGON);	//�|���S����

	//�e�N�X�`���̐ݒ�����ɖ߂�
	pD3DDevice->SetTexture(0, NULL);
}

//=============================================================================
//�Q�[�W�N���X�̈ʒu�̐ݒ菈��
//=============================================================================
void CGauge::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
//�Q�[�W�N���X�̃T�C�Y�ݒ菈��
//=============================================================================
void CGauge::SetSize(const D3DXVECTOR3 size)
{
	m_size = size;
}

//=============================================================================
//�Q�[�W�N���X�̐F�̐ݒ菈��
//=============================================================================
void CGauge::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
//�Q�[�W�N���X�̐ݒ菈��
//=============================================================================
void CGauge::SetGauge(const int nGaugeNum)
{
	m_nGaugeNum = nGaugeNum;

	//�ő吔���傫���Ȃ�����ő吔�֏C��
	if (m_nGaugeNum > m_nMaxGaugeNum)
	{
		m_nGaugeNum = m_nMaxGaugeNum;
	}
}

//=============================================================================
//�Q�[�W�N���X�̌��ݒl�̎擾����
//=============================================================================
int CGauge::GetGauge(void) const
{
	return m_nGaugeNum;
}

//=============================================================================
//�Q�[�W�N���X�̍ő�l�ݒ菈��
//=============================================================================
void CGauge::SetMaxGauge(const int nMaxGaugeNum)
{
	m_nMaxGaugeNum = nMaxGaugeNum;
}

//=============================================================================
//�Q�[�W�N���X�̍ő�l�̎擾����
//=============================================================================
int CGauge::GetMaxGague(void) const
{
	return m_nMaxGaugeNum;
}

//=============================================================================
//�Q�[�W�N���X�̉��Z����
//=============================================================================
void CGauge::AddGauge(int nAddGauge)
{
	m_nGaugeNum += nAddGauge;

	//�ő吔���傫���Ȃ�����ő吔�֏C��
	if (m_nGaugeNum > m_nMaxGaugeNum)
	{
		m_nGaugeNum = m_nMaxGaugeNum;
	}
}

//=============================================================================
//�Q�[�W�N���X�̌��Z����
//=============================================================================
void CGauge::SubGauge(int nSubGauge)
{
	m_nGaugeNum -= nSubGauge;
}

//=============================================================================
//�Q�[�W�N���X�̒��_�̍X�V����
//=============================================================================
void CGauge::VertexUpdate(void)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + ((float)m_nGaugeNum / (float)m_nMaxGaugeNum) * m_size.x, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + ((float)m_nGaugeNum / (float)m_nMaxGaugeNum) * m_size.x, m_pos.y + m_size.y, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
} 