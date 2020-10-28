//=============================================================================
//
// ���C������ [effect.cpp]
// Author : 
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "player.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture[CEffect::EFFECT_TYPE_MAX] = {};

//=============================================================================
//�G�t�F�N�g�N���X�̃R���X�g���N�^
//=============================================================================
CEffect::CEffect(int nPriority) :CScene2D(nPriority)
{
	//�e�����o�ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = COLOR_WHITE;
	m_nLife = 0;
}

//=============================================================================
//�G�t�F�N�g�N���X�̃f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{
}

//=============================================================================
//�G�t�F�N�g�N���X�̃N���G�C�g����
//=============================================================================
CEffect * CEffect::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col, int nLife, const EFFECT_TYPE type, const int nPriority)
{
	//�G�t�F�N�g�N���X�̃|�C���^�ϐ�
	CEffect *pEffect = NULL;

	//�������̊m��
	pEffect = new CEffect(nPriority);

	//���������m�ۂł��Ă�����
	if (pEffect != NULL)
	{
		//�����������Ăяo��
		pEffect->Init(pos, size, col, nLife, type);
	}
	//�������m�ۂɎ��s�����Ƃ�
	else
	{
		return NULL;
	}

	return pEffect;
}

//=============================================================================
//�G�t�F�N�g�N���X�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CEffect::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/effect000.jpg", &m_pTexture[EFFECT_TYPE_BULLET]);	//�o���b�g�p

	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/Cheetah.png", &m_pTexture[EFFECT_TYPE_CHEETAH]);	//�`�[�^�[�p

	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/Gorilla.png", &m_pTexture[EFFECT_TYPE_GORILLA]);	//�S�����p

	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/Turtle.png", &m_pTexture[EFFECT_TYPE_TURTLE]);	//�J���p

	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/ShockWave.png", &m_pTexture[EFFECT_TYPE_BOME]);	//�{���p

	return S_OK;
}

//=============================================================================
//�G�t�F�N�g�N���X�̃e�N�X�`���j������
//=============================================================================
void CEffect::UnLoad(void)
{
	for (int nCount = 0; nCount < EFFECT_TYPE_MAX; nCount++)
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
//�G�t�F�N�g�N���X�̏���������
//=============================================================================
HRESULT CEffect::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col, int nLife, const EFFECT_TYPE type)
{
	//2D�|���S���N���X�̏����������Ăяo��
	CScene2D::Init(pos, size);

	//�F�̐ݒ�
	m_col = col;
	CScene2D::SetColor(m_col);

	//��ނ̐ݒ�
	m_type = type;

	//�e�N�X�`���̐ݒ�
	CScene2D::BindTexture(m_pTexture[m_type]);

	//�ʒu�ݒ�
	m_pos = pos;

	//�T�C�Y�ݒ�
	m_size = size;

	//���C�t�̐ݒ�
	m_nLife = nLife;

	return S_OK;
}

//=============================================================================
//�G�t�F�N�g�N���X�̏I������
//=============================================================================
void CEffect::Uninit(void)
{
	//2D�|���S���N���X�̏I�������Ăяo��
	CScene2D::Uninit();
}

//=============================================================================
//�G�t�F�N�g�N���X�̍X�V����
//=============================================================================
void CEffect::Update(void)
{
	switch (m_type)
	{
	//�o���b�g�^�C�v
	case EFFECT_TYPE_BULLET:
		UpdateBullet();
		break;

	//�`�[�^�[�E�S�����E�J���^�C�v
	case EFFECT_TYPE_CHEETAH:
	case EFFECT_TYPE_GORILLA:
	case EFFECT_TYPE_TURTLE:
		EffectByType();
		break;

	case EFFECT_TYPE_BOME:
		BomeUpdate();
		break;

	case EFFECT_TYPE_TITLE:
		TitleUpdate();
		break;
	default:
		break;
	}

	//���C�t�̌��Z
	m_nLife--;

	//���C�t��0�ȉ��ɂȂ�����
	if (m_nLife <= 0 || (m_size.x <= 0.0f && m_size.y <= 0.0f))
	{
		//�I�������Ăяo��
		Uninit();

		return;
	}
}

//=============================================================================
//�G�t�F�N�g�N���X�̕`�揈��
//=============================================================================
void CEffect::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//���Z����������悤�ɐݒ�
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);;	//���f�X�e�B�l�[�V�����J���[�̐ݒ�

	//2D�|���S���N���X�̕`�揈��
	CScene2D::Draw();
	
	//�ݒ�����ɖ߂�
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//���f�X�e�B�l�[�V�����J���[�̐ݒ�
}

//=============================================================================
//�G�t�F�N�g�N���X�̃o���b�g�p�̍X�V����
//=============================================================================
void CEffect::UpdateBullet(void)
{
	//�T�C�Y�̌��Z
	if (m_size.x <= 0.0f && m_size.y <= 0.0f)
	{
		m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		m_size = D3DXVECTOR3(m_size.x - SIZE_SUBTRACT_VALUE , m_size.y - SIZE_SUBTRACT_VALUE, 0.0f);
	}

	//�A���t�@�l�̌��Z
	m_col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a - ALPHA_SUBTRACT_VALUE);

	//�T�C�Y�̐ݒ�
	CScene2D::SetSize(m_size);

	//�F�̐ݒ�
	CScene2D::SetColor(m_col);
}

//=============================================================================
//�G�t�F�N�g�N���X�̓����p�̍X�V����
//=============================================================================
void CEffect::EffectByType(void)
{
	//�v���C���[�̈ʒu���擾
	D3DXVECTOR3 Playerpos = CGame::GetPlayer()->GetPos();

	//���݈ʒu���v���C���[�̈ʒu�ɍX�V
	m_pos = Playerpos;

	//�T�C�Y�̊g��
	m_size = D3DXVECTOR3(m_size.x + SIZE_SUBTRACT_VALUE * 2, m_size.y + SIZE_SUBTRACT_VALUE * 2, 0.0f);

	//�A���t�@�l�̌��Z
	m_col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a - 0.0125f);

	//�ʒu�̐ݒ�
	CScene2D::SetPos(m_pos);

	//�T�C�Y�̐ݒ�
	CScene2D::SetSize(m_size);

	//�F�̐ݒ�
	CScene2D::SetColor(m_col);
}

void CEffect::BomeUpdate(void)
{
	//�T�C�Y�̊g��
	m_size = D3DXVECTOR3(m_size.x + SIZE_SUBTRACT_VALUE, m_size.y + SIZE_SUBTRACT_VALUE, 0.0f);

	//�A���t�@�l�̌��Z
	m_col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a - 0.05f);

	//�T�C�Y�̐ݒ�
	CScene2D::SetSize(m_size);

	//�F�̐ݒ�
	CScene2D::SetColor(m_col);
}

void CEffect::TitleUpdate(void)
{
	//�T�C�Y�̊g��
	m_size = D3DXVECTOR3(m_size.x + 0.5f, m_size.y + 0.5f, 0.0f);

	//�A���t�@�l�̌��Z
	m_col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a - 0.005f);

	//�T�C�Y�̐ݒ�
	CScene2D::SetSize(m_size);

	//�F�̐ݒ�
	CScene2D::SetColor(m_col);
}

void CEffect::SetType(const EFFECT_TYPE type)
{
	//�e�N�X�`���̐ݒ�
	CScene2D::BindTexture(m_pTexture[type]);
}
