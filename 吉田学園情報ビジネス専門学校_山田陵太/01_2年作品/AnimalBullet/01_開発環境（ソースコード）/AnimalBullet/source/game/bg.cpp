//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define RATE_BG 0.0008f

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CBg::m_apTexture[MAX_BG_TEX] = {};	//�e�N�X�`���̃|�C���^

//=============================================================================
//�w�i�N���X�̃R���X�g���N�^
//=============================================================================
CBg::CBg(int nPriority) : CScene(nPriority)
{
	//0�N���A
	memset(m_apScene2D, NULL, sizeof(m_apScene2D));
}

//=============================================================================
//�w�i�N���X�̃f�X�g���N�^
//=============================================================================
CBg::~CBg()
{
}

//=============================================================================
//�w�i�N���X�̃N���G�C�g����
//=============================================================================
CBg * CBg::Create(void)
{
	//�w�i�N���X�̃|�C���^�ϐ�
	CBg *pBg = NULL;

	//�C���X�^���X����
	pBg = new CBg;

	//���������m�ۂł��Ă�����
	if (pBg)
	{
		//�w�i�N���X�̏����������Ăяo��
		pBg->Init();
	}

	return pBg;
}

//=============================================================================
//�w�i�N���X�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CBg::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/BG/sky.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/BG/mountain.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/BG/wood.png", &m_apTexture[2]);

	return S_OK;
}

//=============================================================================
//�w�i�N���X�̃e�N�X�`���j������
//=============================================================================
void CBg::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_BG_TEX; nCount++)
	{
		//�e�N�X�`���̔j��
		if (m_apTexture[nCount])
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//�w�i�N���X�̏���������
//=============================================================================
HRESULT CBg::Init(void)
{
	for (int nCount = 0; nCount < MAX_2D_NUM; nCount++)
	{
		//2D�|���S���N���X�̃C���X�^���X����
		m_apScene2D[nCount] = CScene2D::Create(SCREEN_CNTER_POS, SCREEN_SIZE, CScene::PRIORITY_BG);

		if (m_apScene2D[nCount])
		{
			//�e�N�X�`���̊��蓖��
			m_apScene2D[nCount]->BindTexture(m_apTexture[nCount]);
		}
	}
	
	return S_OK;
}

//=============================================================================
//�w�i�N���X�̏I������
//=============================================================================
void CBg::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_2D_NUM; nCount++)
	{
		if (m_apScene2D[nCount])
		{
			m_apScene2D[nCount]->Uninit();
		}
	}

	//�I�u�W�F�N�g�̔j��
	SetDeathFlag();
}

//=============================================================================
//�w�i�N���X�̍X�V����
//=============================================================================
void CBg::Update(void)
{
	//�ړ���
	static float fMove[3] = {};

	//UV���W�p�̕ϐ�
	D3DXVECTOR2 aUVpos[4];
	
	for (int nCount = 0; nCount < MAX_2D_NUM; nCount++)
	{
		if (m_apScene2D[nCount])
		{
			//�ړ��ʂ̌v�Z
			fMove[nCount] += RATE_BG + (nCount * RATE_BG);

			//UV���W�̒�`
			aUVpos[0] = D3DXVECTOR2(0.0f + fMove[nCount], 0.0f);
			aUVpos[1] = D3DXVECTOR2(1.0f + fMove[nCount], 0.0f);
			aUVpos[2] = D3DXVECTOR2(0.0f + fMove[nCount], 1.0f);
			aUVpos[3] = D3DXVECTOR2(1.0f + fMove[nCount], 1.0f);

			//2D�|���S���N���X��UV���W�ݒ菈���Ăяo��
			m_apScene2D[nCount]->SetUV(&aUVpos[0]);
		}
	}
}

//=============================================================================
//�w�i�N���X�̕`�揈��
//=============================================================================
void CBg::Draw(void)
{
}
