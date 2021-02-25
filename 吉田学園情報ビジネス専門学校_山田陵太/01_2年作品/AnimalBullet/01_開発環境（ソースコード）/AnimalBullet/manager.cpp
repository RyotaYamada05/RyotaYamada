//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : 
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "keyboard.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "sound.h"
#include "bg.h"
#include "number.h"
#include "polygon.h"
#include "life.h"
#include "title.h"
#include "result.h"
#include "effect.h"
#include "gauge.h"
#include "ui.h"
#include "game.h"
#include "fade.h"
#include "bome.h"
#include "item.h"
#include "boss.h"
#include "joystick.h"
#include "beam.h"
#include "shield.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
CManager::MODE_TYPE CManager::m_mode = CManager::MODE_TYPE_TITLE;	//�Q�[�����[�h����p�ϐ�
CRenderer *CManager::m_pRenderer = NULL;	//�����_���[�N���X�̃|�C���^�ϐ�
CInputKeyboard *CManager::m_pKeyBoard = NULL	;//���͏����N���X�̃|�C���^�ϐ�
CSound *CManager::m_pSound = NULL;	//�T�E���h�N���X�̃|�C���^�ϐ�
CTitle *CManager::m_pTitle = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;
CFade *CManager::m_pFade = NULL;
CInputJoyStick *CManager::m_pJoyStick = NULL;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
//����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//�����_���[�N���X�̃C���X�^���X����
	m_pRenderer = new CRenderer;

	//���������m�ۂł�����
	if (m_pRenderer != NULL)
	{
		// ����������
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{
			return -1;
		}
	}

	//���͏����N���X�̃C���X�^���X����
	m_pKeyBoard = new CInputKeyboard;

	//���������m�ۂł�����
	if (m_pKeyBoard != NULL)
	{
		if (FAILED(m_pKeyBoard->InIt(hInstance, hWnd)))
		{
			return -1;
		}
	}

	//�W���C�X�e�B�b�N�̍쐬
	m_pJoyStick = new CInputJoyStick;

	m_pJoyStick->InIt(hInstance, hWnd);


	//�T�E���h�N���X�̃C���X�^���X����
	m_pSound = new CSound;

	//���������m�ۂł�����
	if (m_pSound != NULL)
	{
		if (FAILED(m_pSound->Init(hWnd)))
		{
			return -1;
		}
	}
	//�S�e�N�X�`���̓ǂݍ���
	LoadAll();

	//�^�C�g���N���X�̐���
	m_pTitle = CTitle::Create();

	//�t�F�[�h�N���X�̐���
	m_pFade = CFade::Create();

	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CManager::Uninit(void)
{
	//���݂̃��[�h����j��
	switch (m_mode)
	{
		//�^�C�g�����[�h
	case MODE_TYPE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();

			m_pTitle = NULL;
		}
		break;

		//�Q�[�����[�h
	case MODE_TYPE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();

			m_pGame = NULL;
		}
		break;

		//���U���g���[�h
	case MODE_TYPE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();

			m_pResult = NULL;
		}
		break;
	}
	//�I�u�W�F�N�g�N���X�̂��ׂĂ̏I�������Ăяo��
	CScene::ReleaseAll();

	//�S�e�N�X�`���̔j��
	UnLoadAll();

	if (m_pSound != NULL)
	{
		//�T�E���h�N���X�̏I�������Ăяo��
		m_pKeyBoard->Uninit();

		//�������̍폜
		delete m_pSound;

		//�������̃N���A
		m_pSound = NULL;
	}

	if (m_pJoyStick != NULL)
	{
		//�W���C�X�e�B�b�N�N���X�̏I�������Ăяo��
		m_pJoyStick->Uninit();

		//�������̍폜
		delete m_pJoyStick;

		//�������̃N���A
		m_pJoyStick = NULL;
	}

	if (m_pKeyBoard != NULL)
	{
		//���͏����N���X�̏I�������Ăяo��
		m_pKeyBoard->Uninit();

		//�������̍폜
		delete m_pKeyBoard;

		//�������̃N���A
		m_pKeyBoard = NULL;
	}

	if (m_pFade != NULL)
	{
		//�t�F�[�h�N���X�̏I�������Ăяo��
		m_pFade->Uninit();

		//�������̍폜
		delete m_pFade;

		//�������̃N���A
		m_pFade = NULL;
	}

	if (m_pRenderer != NULL)
	{
		//�����_���[�N���X�̏I�������Ăяo��
		m_pRenderer->Uninit();

		//�������̍폜
		delete m_pRenderer;

		//�������̃N���A
		m_pRenderer = NULL;
	}
}

//=============================================================================
//�X�V����
//=============================================================================
void CManager::Update(void)
{
	if (m_pKeyBoard != NULL)
	{
		//���͏����N���X�̍X�V�����Ăяo��
		m_pKeyBoard->Update();
	}

	if (m_pJoyStick != NULL)
	{
		//j�W���C�X�e�B�b�N�N���X�̍X�V����
		m_pJoyStick->Update();
	}
	if (m_pRenderer != NULL)
	{
		//�����_���[�N���X�̍X�V�����Ăяo��
		m_pRenderer->Update();
	}
	
	if (m_pFade != NULL)
	{
		//�t�F�[�h�N���X�̍X�V����
		m_pFade->Update();
	}
}

//=============================================================================
//�`�揈��
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		//�����_���[�N���X�̕`�揈���Ăяo��
		m_pRenderer->Draw();
	}
}

//=============================================================================
//�e�N�X�`���̑S���[�h����
//=============================================================================
void CManager::LoadAll(void)
{
	//�V�[���h�N���X�̃e�N�X�`���ǂݍ���
	CShield::Load();

	//�r�[���N���X�̃e�N�X�`���ǂݍ���
	CBeam::Load();

	//�{�X�N���X�̃e�N�X�`���ǂݍ���
	CBoss::Load();

	//�A�C�e���N���X�̃e�N�X�`���ǂݍ���
	CItem::Load();

	//�{���N���X�̃e�N�X�`���ǂݍ���
	CBome::Load();

	//UI�N���X�̃e�N�X�`���ǂݍ���
	CUi::Load();

	//�w�i�N���X�̃e�N�X�`���ǂݍ���
	CBg::Load();

	//�v���C���[�N���X�̃e�N�X�`���ǂݍ���
	CPlayer::Load();

	//�G�l�~�[�N���X�̃e�N�X�`���ǂݍ���
	CEnemy::Load();

	//�o���b�g�N���X�̃e�N�X�`���ǂݍ���
	CBullet::Load();

	//�G�t�F�N�g�N���X�̃e�N�X�`���ǂݍ���
	CEffect::Load();

	//�G�N�X�v���[�W�����N���X�̃e�N�X�`���ǂݍ���
	CExplosion::Load();

	//�i���o�[�N���X�̃e�N�X�`���ǂݍ���
	CNumber::Load();

	//�|���S���N���X�̃e�N�X�`���ǂݍ���
	CPolygon::Load();

	//�Q�[�W�N���X�̃e�N�X�`���ǂݍ���
	CGauge::Load();
}

//=============================================================================
//�e�N�X�`���̑S�A�����[�h����
//=============================================================================
void CManager::UnLoadAll(void)
{
	//�Q�[�W�N���X�̃e�N�X�`���j��
	CGauge::UnLoad();

	//�|���S���N���X�̃e�N�X�`���j��
	CPolygon::UnLoad();

	//�i���o�[�N���X�̃e�N�X�`���j��
	CNumber::UnLoad();
	
	//�G�N�X�v���[�W�����N���X�̃e�N�X�`���j��
	CExplosion::UnLoad();

	//�G�t�F�N�g�N���X�̃e�N�X�`���j��
	CEffect::UnLoad();

	//�o���b�g�N���X�̃e�N�X�`���j��
	CBullet::UnLoad();

	//�G�l�~�[�N���X�̃e�N�X�`���j��
	CEnemy::UnLoad();

	//�v���C���[�N���X�̃e�N�X�`���j��
	CPlayer::UnLoad();

	//�w�i�N���X�̃e�N�X�`���j��
	CBg::UnLoad();

	//UI�N���X�̃e�N�X�`���j��
	CUi::UnLoad();

	//�{���N���X�̃e�N�X�`���j��
	CBome::UnLoad();

	//�A�C�e���N���X�̃e�N�X�`���j��
	CItem::UnLoad();

	//�{�X�N���X�̃e�N�X�`���j��
	CBoss::UnLoad();

	//�r�[���N���X�̃e�N�X�`���j��
	CBeam::UnLoad();

	//�Q�[�W�N���X�̃e�N�X�`���j��
	CShield::UnLoad();
}
//=============================================================================
//���[�h�̐ݒ菈��
//=============================================================================
void CManager::SetMode(MODE_TYPE mode)
{
	//���݂̃��[�h����j��
	switch (m_mode)
	{
	//�^�C�g�����[�h
	case MODE_TYPE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();

			m_pTitle = NULL;

			CScene::ReleaseAll();
		}
		break;

	//�Q�[�����[�h
	case MODE_TYPE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();

			m_pGame = NULL;

			CScene::ReleaseAll();
		}
		break;

	//���U���g���[�h
	case MODE_TYPE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();

			m_pResult = NULL;
			CScene::ReleaseAll();
		}
		break;

	}

	//���[�h���Z�b�g
	m_mode = mode;
	
	//�Z�b�g���ꂽ���[�h�ɑΉ��������𐶐�����
	switch (m_mode)
	{
	//�^�C�g�����[�h
	case MODE_TYPE_TITLE:
		//�^�C�g���N���X�̐���
		m_pTitle = CTitle::Create();
		break;

	//�Q�[�����[�h
	case MODE_TYPE_GAME:
		m_pGame = CGame::Create();
		break;

	//���U���g���[�h
	case MODE_TYPE_RESULT:
		//���U���g�N���X�̐���
		m_pResult = CResult::Create();
		break;
	}
}

//=============================================================================
//�Q�[�����[�h���̎擾
//=============================================================================
CManager::MODE_TYPE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
//�����_���[���擾
//=============================================================================
CRenderer * CManager::GetRenderer(void)
{
	return m_pRenderer;
}


//=============================================================================
//�L�[�{�[�h���擾
//=============================================================================
CInputKeyboard * CManager::GetKeyborad(void)
{
	return m_pKeyBoard;
}

//=============================================================================
//�T�E���h���擾
//=============================================================================
CSound * CManager::GetSound(void)
{
	return m_pSound;
}

//=============================================================================
//�t�F�[�h���擾
//=============================================================================
CFade * CManager::GetFade(void)
{
	return m_pFade;
}

//=============================================================================
//�W���C�X�e�B�b�N���擾
//=============================================================================
CInputJoyStick * CManager::GetJoyStick(void)
{
	return m_pJoyStick;
}
