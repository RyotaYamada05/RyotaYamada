//=============================================================================
//
// �Q�[���������� [game.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "game.h"
#include "ui.h"
#include "player.h"
#include "bg.h"
#include "polygon.h"
#include "scene2d.h"
#include "boss.h"
#include "fade.h"
#include "manager.h"
#include "sound.h"
#include "enemy_control.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
CUi *CGame::m_pUi = NULL;
CPlayer *CGame::m_pPlayer = NULL;
CGame::GAME_STATE CGame::m_state = CGame::GAME_STATE_NOMRAL;
CPolygon *CGame::m_pPolygon = NULL;

//=============================================================================
//�Q�[���N���X�̃R���X�g���N�^
//=============================================================================
CGame::CGame(int nPriority) :CScene(nPriority)
{
	m_nEnemyCounter = 0;
	m_nStaeCounter = 0;
	m_pEnemyControl = NULL;
}

//=============================================================================
//�Q�[���N���X�̃f�X�g���N�^
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
//�Q�[���N���X�̃N���G�C�g����
//=============================================================================
CGame * CGame::Create(void)
{
	//�Q�[���N���X�̃|�C���^�ϐ�
	CGame *pGame = NULL;

	//�������̊m��
	pGame = new CGame;

	//���������m�ۂł��Ă�����
	if (pGame)
	{
		//�����������Ăяo��
		pGame->Init();
	}
	//�������m�ۂɎ��s�����Ƃ�
	else
	{
		return NULL;
	}

	return pGame;
}

//=============================================================================
//�Q�[���N���X�̏���������
//=============================================================================
HRESULT CGame::Init(void)
{
	m_state = CGame::GAME_STATE_NOMRAL;
	//SetGameState(CGame::GAME_STATE_BOSS_START);

	// �w�i�N���X�̐���
	CBg::Create();

	//�v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(CHEETAH_SIZE_X / 2, SCREEN_HEIGHT / 2, 0.0f), CHEETAH_SIZE);

	//UI�N���X�̐���
	m_pUi = CUi::Create();

	//BGM�̍Đ�
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME);
	
	//�G�l�~�[�R���g���[���N���X�̃C���X�^���X����
	m_pEnemyControl = new CEnemyControl;

	if (m_pEnemyControl)
	{
		//�G�l�~�[�R���g���[���N���X�̏�����
		m_pEnemyControl->Init();
	}

	return S_OK;
}

//=============================================================================
//�Q�[���N���X�̏I������
//=============================================================================
void CGame::Uninit(void)
{
	if (m_pEnemyControl)
	{
		//�G�l�~�[�R���g���[���N���X�̏I������
		m_pEnemyControl->Uninit();

		//�������̍폜
		delete m_pEnemyControl;

		//�������̃N���A
		m_pEnemyControl = NULL;
	}
	if (m_pPolygon)
	{
		//�|���S���N���X�̏I������
		m_pPolygon->Uninit();

		//�������̔j��
		delete m_pPolygon;

		//�������̃N���A
		m_pPolygon = NULL;
	}

	if (m_pUi)
	{
		//UI�N���X�̏I������
		m_pUi->Uninit();

		//�������̔j��
		delete m_pUi;

		//�������̃N���A
		m_pUi = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	SetDeathFlag();
}

//=============================================================================
//�Q�[���N���X�̍X�V����
//=============================================================================
void CGame::Update(void)
{
	if (m_pUi)
	{
		//UI�N���X�̍X�V
		m_pUi->Update();
	}
	if (m_pEnemyControl)
	{
		//�G�l�~�[�R���g���[���N���X�̍X�V
		m_pEnemyControl->Update();
	}
	switch (m_state)
	{
	case GAME_STATE_NOMRAL:
		
		break;

	//�{�X��J�n����
	case GAME_STATE_BOSS_START:
		
		if (m_pPolygon)
		{
			//�_�ŏ����̌Ăяo��
			m_pPolygon->Flashing();

			if (m_nStaeCounter >= 240)
			{
				m_pPolygon->Uninit();

				delete m_pPolygon;

				m_pPolygon = NULL;

				m_nStaeCounter = 0;
				SetGameState(GAME_STATE_BOSS_BATTLE);
			}

			//�X�e�[�g�J�E���g�A�b�v
			m_nStaeCounter++;
		}
		break;

	case GAME_STATE_GAME_CLEAR:
	case GAME_STATE_GAME_OVER:
		if (m_nStaeCounter == 360)
		{
			CManager::GetFade()->SetFade(CManager::MODE_TYPE_RESULT);
			CManager::GetSound()->Stop();
		}

		//�X�e�[�g�J�E���g�A�b�v
		m_nStaeCounter++;

		break;
	}
}

//=============================================================================
//�Q�[���N���X�̕`�揈��
//=============================================================================
void CGame::Draw(void)
{
	if (m_pPolygon)
	{
		m_pPolygon->Draw();
	}
	if (m_pUi)
	{
		m_pUi->Draw();
	}
}

//=============================================================================
//�Q�[���N���X�̏�Ԑݒ菈��
//=============================================================================
void CGame::SetGameState(const GAME_STATE state)
{
	m_state = state;

	switch (m_state)
	{
	//�{�X��J�n�O����
	case GAME_STATE_BOSS_START:
		if (!m_pPolygon)
		{
			//BGM���~���x�������Đ�
			CManager::GetSound()->Stop();
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_WARNING);

			//�x�����b�Z�[�W�̕\��
			m_pPolygon = CPolygon::Create(SCREEN_CNTER_POS,
				D3DXVECTOR3(700.0f, 175.0f, 0.0f), CPolygon::TEX_TYPE_WARNING);
		}
		break;

	//�{�X��
	case GAME_STATE_BOSS_BATTLE:
		//�{�X�𐶐�
		CBoss::Create(
			D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE_X, SCREEN_HEIGHT / 2, 0.0f),
			BOSS_SIZE,
			BOSS_LIFE,
			BOSS_SCORE);

		//�{�X��p��BGM���Đ�
		CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_BOSS);
		break;

	//�Q�[���N���A
	case GAME_STATE_GAME_CLEAR:
		if (!m_pPolygon)
		{
			//BGM��~
			CManager::GetSound()->Stop();

			//�������̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_CLEAR);

			//�����\��
			m_pPolygon = CPolygon::Create(SCREEN_CNTER_POS,
				D3DXVECTOR3(675.0f, 50.0f, 0.0f), CPolygon::TEX_TYPE_GAME_CLEAR);
		}
		break;

	//�Q�[���I�[�o�[
	case GAME_STATE_GAME_OVER:
		if (!m_pPolygon)
		{
			//BGM�̍Đ�
			CManager::GetSound()->Stop();
			
			//�s�k���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_OVER);
		
			//�s�k�\��
			m_pPolygon = CPolygon::Create(SCREEN_CNTER_POS,
				D3DXVECTOR3(675.0f, 50.0f, 0.0f), CPolygon::TEX_TYPE_GAME_OVER);
		}
		break;
	default:
		break;
	}
}

//=============================================================================
//�Q�[���N���X�̏�Ԏ擾����
//=============================================================================
CGame::GAME_STATE CGame::GetGameState(void)
{
	return m_state;
}

//=============================================================================
//UI���̎擾����
//=============================================================================
CUi * CGame::GetUi(void)
{
	return m_pUi;
}

//=============================================================================
//�v���C���[���̎擾
//=============================================================================
CPlayer * CGame::GetPlayer(void)
{
	return m_pPlayer;
}
