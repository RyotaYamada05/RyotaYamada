//=============================================================================
//
// UI���� [ui.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "ui.h"
#include "lifeber.h"
#include "score.h"
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "animalui.h"
#include "skillber.h"
#include "game.h"
#include "player.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
CLifeber *CUi::m_pLifeber = NULL;
CSkillber *CUi::m_pSkillber = NULL;
CScore * CUi::m_pScore = NULL;
CAnimalUi * CUi::m_pAnimalUI = NULL;
LPDIRECT3DTEXTURE9 CUi::m_pTexture[UI_TEX_MAX] = {};

//=============================================================================
//UI�N���X�̃R���X�g���N�^
//=============================================================================
CUi::CUi()
{
}

//=============================================================================
//UI�N���X�̃f�X�g���N�^
//=============================================================================
CUi::~CUi()
{
}

//=============================================================================
//UI�N���X�̃N���G�C�g����
//=============================================================================
CUi * CUi::Create(void)
{
	CUi *pUi = NULL;

	//�C���X�^���X�̐���
	pUi = new CUi;

	if (pUi)
	{
		//����������
		pUi->Init();
	}
	else
	{
		return NULL;
	}
	return pUi;
}

//=============================================================================
//UI�N���X�̃e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CUi::Load(void)
{
	//�f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/UnderUI.png", &m_pTexture[UI_TEX_1]);
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/UI/gaugeUI.png", &m_pTexture[UI_TEX_2]);

	return S_OK;
}

//=============================================================================
//UI�N���X�̃e�N�X�`���̔j������
//=============================================================================
void CUi::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < UI_TEX_MAX; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (m_pTexture[nCntTex])
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//UI�N���X�̏���������
//=============================================================================
HRESULT CUi::Init(void)
{
	CScene2D *pScene2D;	

	//�㑤UI�p�|���S������
	pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f + UNDER_UI_SIZE_Y / 2, 0.0f), 
		UNDER_UI_SIZE, CScene::PRIORITY_UI);
	pScene2D->BindTexture(m_pTexture[UI_TEX_1]);

	//�X�R�A�N���X�̐���
	m_pScore = CScore::Create();

	////���C�t�N���X�̐���
	m_pLifeber = CLifeber::Create(
		D3DXVECTOR3(100.0f, 15.0f, 0.0f), 
		LIFE_GAGUE_SIZE, 
		COLOR_GREEN,
		CGame::GetPlayer()->GetLife(), 
		CGame::GetPlayer()->GetMaxLife());

	//�X�L���N���X�̐���
	m_pSkillber = CSkillber::Create(
		D3DXVECTOR3(100.0f, 41, 0.0f),
		SKILL_GAGUE_SIZE,
		COLOR_CYAN,
		MAX_SKILL,
		MAX_SKILL);

	//�A�j�}��UI�N���X�̐���
	m_pAnimalUI = CAnimalUi::Create();

	//�|���S���N���X�𐶐�
	pScene2D = CScene2D::Create(D3DXVECTOR3((GAUGE_UI_SIZE_X / 2), (GAUGE_UI_SIZE_Y / 2), 0.0f),
		GAUGE_UI_SIZE, CScene::PRIORITY_UI);
	pScene2D->BindTexture(m_pTexture[UI_TEX_2]);
	//pScene2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
	return S_OK;
}

//=============================================================================
//UI�N���X�̏I������
//=============================================================================
void CUi::Uninit(void)
{
}

//=============================================================================
//UI�N���X�̍X�V����
//=============================================================================
void CUi::Update(void)
{
}

//=============================================================================
//UI�N���X�̕`�揈��
//=============================================================================
void CUi::Draw(void)
{
}

//=============================================================================
//�X�R�A���擾
//=============================================================================
CScore * CUi::GetScore(void)
{
	return m_pScore;
}

//=============================================================================
//���C�t���擾
//=============================================================================
CLifeber * CUi::GetLifeber(void)
{
	return m_pLifeber;
}

//=============================================================================
//�X�L�����擾
//=============================================================================
CSkillber * CUi::GetSkillber(void)
{
	return m_pSkillber;
}

//=============================================================================
//�A�j�}��UI���擾
//=============================================================================
CAnimalUi * CUi::GetAnimalUI(void) 
{
	return m_pAnimalUI;
}
