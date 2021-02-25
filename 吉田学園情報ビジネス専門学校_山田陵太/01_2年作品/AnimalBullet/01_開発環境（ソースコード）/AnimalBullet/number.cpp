//=============================================================================
//
// �i���o�[���� [number.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CNumber::m_pTexture[NUMBER_TYPE_MAX] = {};

//=============================================================================
//�i���o�[�N���X�̃R���X�g���N�^
//=============================================================================
CNumber::CNumber()
{
	//�e�����o�ϐ��̃N���A
	m_nNumber = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVtxBuff = NULL;
	m_type = NUMBER_TYPE_000;
}

//=============================================================================
//�i���o�[�N���X�̃f�X�g���N�^
//=============================================================================
CNumber::~CNumber()
{
}

//=============================================================================
//�i���o�[�N���X�̃N���G�C�g����
//=============================================================================
CNumber * CNumber::Create(const int nNumber, const NUMBER_TYPE Type, const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR color)
{
	CNumber *pNumber = NULL;

	//�������̊m��
	pNumber = new CNumber;

	//���������m�ۂł��Ă�����
	if (pNumber != NULL)
	{
		//�����������Ăяo��
		pNumber->Init(nNumber, Type, pos, size, color);
	}
	//�������m�ۂɎ��s�����Ƃ�
	else
	{
		return NULL;
	}

	return pNumber;
}

//=============================================================================
//�i���o�[�N���X�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CNumber::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Number/number000.png", &m_pTexture[NUMBER_TYPE_000]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Number/number001.png", &m_pTexture[NUMBER_TYPE_001]);

	return S_OK;
}

//=============================================================================
//�i���o�[�N���X�̃e�N�X�`���j������
//=============================================================================
void CNumber::UnLoad(void)
{
	for (int nCount = 0; nCount < NUMBER_TYPE_MAX; nCount++)
	{
		//�e�N�X�`���̔j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//�i���o�[�N���X�̏���������
//=============================================================================
HRESULT CNumber::Init(const int nNumber, const NUMBER_TYPE Type, const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR color)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//�����ʒu�̐ݒ�
	m_pos = pos;

	//�傫���̐ݒ�
	m_size = size;

	//�����̐ݒ�
	m_nNumber = nNumber;

	m_type = Type;

	//���_�o�b�t�@�̍쐬
	pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,	//�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,	//�i�Œ�j
		FVF_VERTEX_2D,	//�t�H�[�}�b�g
		D3DPOOL_MANAGED,	//(�Œ�)
		&m_pVtxBuff,
		NULL
	);

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-m_size.x / 2), m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);

	//rhw�̐ݒ�i�l��1.0�ŌŒ�j
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = color;
	pVtx[1].col = color;
	pVtx[2].col = color;
	pVtx[3].col = color;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_nNumber * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_nNumber * 0.1f + 0.1f, 1.0f);

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//�i���o�[�N���X�̏I������
//=============================================================================
void CNumber::Uninit(void)
{
	//���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();

		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//�i���o�[�N���X�̍X�V����
//=============================================================================
void CNumber::Update(void)
{
}

//=============================================================================
//�i���o�[�N���X�̕`�揈��
//=============================================================================
void CNumber::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�̃|�C���^

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pD3DDevice->SetTexture(0, m_pTexture[m_type]);

	// �|���S���̕`��
	pD3DDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,
		NUM_POLYGON);	//�|���S����

	//�e�N�X�`���̐ݒ�����ɖ߂�
	pD3DDevice->SetTexture(0, NULL);
}

//=============================================================================
//�i���o�[�N���X�̐ݒ菈��
//=============================================================================
void CNumber::SetNumber(const int nNumber)
{
	//�i���o�[�̐ݒ�
	m_nNumber = nNumber;

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_nNumber * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_nNumber * 0.1f + 0.1f, 1.0f);

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�i���o�[�N���X�̐F�ݒ菈��
//=============================================================================
void CNumber::SetColr(const D3DXCOLOR col)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
}
