//=============================================================================
//
// �Q�[���������� [game.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "game.h"
#include "ui.h"
#include "player.h"
#include "enemy.h"
#include "bg.h"
#include "polygon.h"
#include "scene2d.h"
#include "boss.h"
#include "fade.h"
#include "manager.h"
#include "sound.h"

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
	m_pPolygon = NULL;
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
CGame * CGame::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//�Q�[���N���X�̃|�C���^�ϐ�
	CGame *pGame = NULL;

	//�������̊m��
	pGame = new CGame;

	//���������m�ۂł��Ă�����
	if (pGame != NULL)
	{
		//�����������Ăяo��
		pGame->Init(pos, size);
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
HRESULT CGame::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_state = CGame::GAME_STATE_NOMRAL;

	// �w�i�N���X�̐���
	CBg::Create();

	//UI�N���X�̐���
	CUi::Create();

	//�v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f + CHEETAH_SIZE_X / 2, SCREEN_HEIGHT / 2, 0.0f), CHEETAH_SIZE);

	//BGM�̍Đ�
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME);
	
	return S_OK;
}

//=============================================================================
//�Q�[���N���X�̏I������
//=============================================================================
void CGame::Uninit(void)
{
	if (m_pPolygon != NULL)
	{
		//�|���S���N���X�̏I������
		m_pPolygon->Uninit();

		//�������̔j��
		delete m_pPolygon;

		//�������̃N���A
		m_pPolygon = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	SetDeathFlag();
}

//=============================================================================
//�Q�[���N���X�̍X�V����
//=============================================================================
void CGame::Update(void)
{
	switch (m_state)
	{
	case GAME_STATE_NOMRAL:
		m_nEnemyCounter++;

		if (m_nEnemyCounter == 60)
		{
			for (int nCount = 0; nCount < 6; nCount++)
			{
				CEnemy::Create(D3DXVECTOR3(1300.0f + ENEMY_SIZE_X * nCount, 200.0f, 0.0f),
					ENEMY_SIZE, CEnemy::ENEMY_TYPE_NO_BULLET, CEnemy::ACTION_PATTERN_MOVE_DOWN);

				CEnemy::Create(D3DXVECTOR3(1300.0f + ENEMY_SIZE_X * nCount, 600.0f, 0.0f),
					ENEMY_SIZE, CEnemy::ENEMY_TYPE_NO_BULLET, CEnemy::ACTION_PATTERN_MOVE_UP);
			}
		}
		if (m_nEnemyCounter == 300)
		{
			for (int nCount = 0; nCount < 6; nCount++)
			{
				CEnemy::Create(D3DXVECTOR3(1300.0f + ENEMY_SIZE_X * nCount, SCREEN_HEIGHT/2, 0.0f),
					ENEMY_SIZE, CEnemy::ENEMY_TYPE_NO_BULLET, CEnemy::ACTION_PATTERN_NOMRAL);

			}
		}
		if (m_nEnemyCounter == 500)
		{
			for (int nCount = 0; nCount < 6; nCount++)
			{
				for (int nCount2 = 0; nCount2 < 10; nCount2++)
				{
					CEnemy::Create(D3DXVECTOR3(1300.0f + ENEMY_SIZE_X * nCount2, 
						(SCREEN_HEIGHT / 2) - (ENEMY_SIZE_Y * 6) / 2+
						(ENEMY_SIZE_Y * nCount)+(ENEMY_SIZE_Y/2), 0.0f),
						ENEMY_SIZE, CEnemy::ENEMY_TYPE_NO_BULLET, CEnemy::ACTION_PATTERN_NOMRAL);
				}
			}
		}
		if (m_nEnemyCounter == 900)
		{

			CEnemy::Create(D3DXVECTOR3(1300.0f, 150.0f + (ENEMY_SIZE_Y / 1.5f)*0 , 0.0f),
				ENEMY_SIZE / 1.5f, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN);
			CEnemy::Create(D3DXVECTOR3(1300.0f, 150.0f + (ENEMY_SIZE_Y / 1.5f)*1, 0.0f),
				ENEMY_SIZE / 1.5f, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN,CEnemy::Dead_Process_LIFE);
			CEnemy::Create(D3DXVECTOR3(1300.0f, 150.0f + (ENEMY_SIZE_Y / 1.5f)*2, 0.0f),
				ENEMY_SIZE / 1.5f, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN);

		}
		if (m_nEnemyCounter == 1000)
		{	
			CEnemy::Create(D3DXVECTOR3(1300.0f, 600.0f + (ENEMY_SIZE_Y / 1.5f) * 0, 0.0f),
					ENEMY_SIZE / 1.5f, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN);
			CEnemy::Create(D3DXVECTOR3(1300.0f, 600.0f + (ENEMY_SIZE_Y / 1.5f) * 1, 0.0f),
				ENEMY_SIZE / 1.5f, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN, CEnemy::Dead_Process_SKILL);
			CEnemy::Create(D3DXVECTOR3(1300.0f, 600.0f + (ENEMY_SIZE_Y / 1.5f) * 2, 0.0f),
				ENEMY_SIZE / 1.5f, CEnemy::ENEMY_TYPE_SNIPING, CEnemy::ACTION_PATTERN_STOP_RETURN);
			
		}
	

		if (m_nEnemyCounter == 1500)
		{
			CEnemy::Create(D3DXVECTOR3(1300.0f, SCREEN_HEIGHT/2, 0.0f),
				ENEMY_SIZE, CEnemy::ENEMY_TYPE_HADE, CEnemy::ACTION_PATTERN_STOP, CEnemy::Dead_Process_BOSS);
		}
		if (m_nEnemyCounter == 1200)
		{
			CEnemy::Create(D3DXVECTOR3(1300.0f, SCREEN_HEIGHT/2, 0.0f),
				ENEMY_SIZE, CEnemy::ENEMY_TYPE_STOKER, CEnemy::ACTION_PATTERN_STALKER);
		}
		break;

	//�{�X��J�n����
	case GAME_STATE_BOSS_START:
		
		if (m_pPolygon != NULL)
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
	if (m_pPolygon != NULL)
	{
		m_pPolygon->Draw();
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
		if (m_pPolygon == NULL)
		{
			//BGM���~���x�������Đ�
			CManager::GetSound()->Stop();
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_WARNING);

			//�x�����b�Z�[�W�̕\��
			m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 
				D3DXVECTOR3(700.0f, 175.0f, 0.0f), CPolygon::TEX_TYPE_WARNING);
		}
		break;

	//�{�X��
	case GAME_STATE_BOSS_BATTLE:
		//�{�X�𐶐�
		CBoss::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE_X, SCREEN_HEIGHT / 2, 0.0f), ENEMY_SIZE * 3);
		//�{�X��p��BGM���Đ�
		CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_BOSS);
		break;

	//�Q�[���N���A
	case GAME_STATE_GAME_CLEAR:
		if (m_pPolygon == NULL)
		{
			//BGM��~
			CManager::GetSound()->Stop();

			//�������̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_CLEAR);

			//�����\��
			m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
				D3DXVECTOR3(675.0f, 50.0f, 0.0f), CPolygon::TEX_TYPE_GAME_CLEAR);
		}
		break;

	//�Q�[���I�[�o�[
	case GAME_STATE_GAME_OVER:
		if (m_pPolygon == NULL)
		{
			//BGM�̍Đ�
			CManager::GetSound()->Stop();
			
			//�s�k���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_OVER);
		
			//�s�k�\��
			m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
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
