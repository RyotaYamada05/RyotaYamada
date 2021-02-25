//=============================================================================
//
// �|���S������ [life.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "polygon.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define FLASHING_RATE 0.05f	//�_�ő��x�̒萔
#define MAX_ALPHA 1.0f	//���l�̍ő�l
#define MIN_ALPHA 0.0f	//���l�̍Œ�l

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CPolygon::m_pTexture[CPolygon::TEX_TYPE_MAX] = {};

//=============================================================================
//�|���S���N���X�̃R���X�g���N�^
//=============================================================================
CPolygon::CPolygon()
{
	//�e�����o�ϐ��̃N���A
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_type = TEX_TYPE_NORE;
	m_Flashing = FLASHING_NONE;
}

//=============================================================================
//�|���S���N���X�̃f�X�g���N�^
//=============================================================================
CPolygon::~CPolygon()
{
}

//=============================================================================
//�|���S���N���X�̃N���G�C�g����
//=============================================================================
CPolygon * CPolygon::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const TEX_TYPE type )
{
	//�|���S���N���X�̃|�C���^�ϐ�
	CPolygon *pPolygon = NULL;

	//�������̊m��
	pPolygon = new CPolygon;

	//���������m�ۂł��Ă�����
	if (pPolygon != NULL)
	{
		//�����������Ăяo��
		pPolygon->Init(pos, size, type);
	}
	else
	{
		return NULL;
	}
	return pPolygon;
}

//=============================================================================
//�|���S���N���X�̃e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CPolygon::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	m_pTexture[TEX_TYPE_NORE] = NULL;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/BG/Title.png", &m_pTexture[TEX_TYPE_TITLE]);	//�^�C�g��
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/TItleLogo.png", &m_pTexture[TEX_TYPE_TITLE_LOGO]);	//�^�C�g��
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/BG/Result.png", &m_pTexture[TEX_TYPE_RESULET]);	//���U���g
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/gaugeUI.png", &m_pTexture[TEX_TYPE_GAUGE_UI]);	//�Q�[�WUI
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/UnderUI.png", &m_pTexture[TEX_TYPE_UNDERUI]);	//UnderUI
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/Cheetah.png", &m_pTexture[TEX_TYPE_CHEETAH]);	//�`�[�^�[
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/Gorilla.png", &m_pTexture[TEX_TYPE_GORILLA]);	//�S����
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/Turtle.png", &m_pTexture[TEX_TYPE_TURTLE]);	//�J��
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/PRESS_ENTER.png", &m_pTexture[TEX_TYPE_PRESSENTER]);	//PRESS ENTER�e�N�X�`��
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/WARNING.png", &m_pTexture[TEX_TYPE_WARNING]);	//WARNING�e�N�X�`��
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/GAME CLEAR.png", &m_pTexture[TEX_TYPE_GAME_CLEAR]);	//WARNING�e�N�X�`��
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/GAME OVER.png", &m_pTexture[TEX_TYPE_GAME_OVER]);	//WARNING�e�N�X�`��

	return S_OK;
}

//=============================================================================
//�|���S���N���X�̃e�N�X�`���j������
//=============================================================================
void CPolygon::UnLoad(void)
{
	for (int nCount = 0; nCount < TEX_TYPE_MAX; nCount++)
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
//�|���S���N���X�̏���������
//=============================================================================
HRESULT CPolygon::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const TEX_TYPE type)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//�����ʒu�̐ݒ�
	m_pos = pos;

	//�傫���̐ݒ�
	m_size = size;

	//�e�N�X�`���̐ݒ�
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
//�|���S���N���X�̏I������
//=============================================================================
void CPolygon::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//�|���S���N���X�̍X�V����
//=============================================================================
void CPolygon::Update(void)
{
}

//=============================================================================
//�|���S���N���X�̕`�揈��
//=============================================================================
void CPolygon::Draw(void)
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
//�|���S���N���X�̎�ނ̐ݒ菈��
//=============================================================================
void CPolygon::SetType(const TEX_TYPE type)
{
	m_type = type;
}

//=============================================================================
//�|���S���N���X�̐F�̐ݒ菈��
//=============================================================================
void CPolygon::SetColor(const D3DXCOLOR color)
{
	m_col = color;

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�|���S���N���X�̈ʒu�̐ݒ菈��
//=============================================================================
void CPolygon::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;

	VERTEX_2D *pVtx;

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-m_size.x / 2), m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);


	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�|���S���N���X�̈ʒu�̎擾����
//=============================================================================
D3DXVECTOR3 CPolygon::GetPos(void)
{
	return m_pos;
}

//=============================================================================
//�|���S���N���X�̓_�ŏ���
//=============================================================================
void CPolygon::Flashing(void)
{
	if (m_Flashing == FLASHING_NONE)
	{
		//���Z��Ԃֈڍs
		m_Flashing = FLASHING_SUB;
		m_col.a = MAX_ALPHA;
	}

	if (m_Flashing == FLASHING_ADD)
	{
		//���l�̉��Z
		m_col.a += FLASHING_RATE;

		if (m_col.a >= MAX_ALPHA)
		{
			m_col.a = MAX_ALPHA;

			//�ʏ��Ԃֈڍs
			m_Flashing = FLASHING_NONE;
		}
	}
	else if (m_Flashing == FLASHING_SUB)
	{
		//���l�̌��Z
		m_col.a -= FLASHING_RATE;

		if (m_col.a <= MIN_ALPHA)
		{
			m_col.a = MIN_ALPHA;

			//���Z��Ԃֈڍs
			m_Flashing = FLASHING_ADD;
		}
	}

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
}
