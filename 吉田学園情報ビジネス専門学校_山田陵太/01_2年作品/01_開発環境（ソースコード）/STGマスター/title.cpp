//=============================================================================
//
// ���C������ [title.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "title.h"
#include "polygon.h"
#include "manager.h"
#include "keyboard.h"
#include "fade.h"
#include "sound.h"
#include "effect.h"
#include "joystick.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define LOGO_POS D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f)
#define LOGO_MOVE_SPEED 2.0f
#define EFFECT_SIZE_X 100.0f	//�v���C���[��X�T�C�Y
#define EFFECT_SIZE_Y 100.0f	//�v���C���[��Y�T�C�Y
#define EFFECT_SIZE D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f)	//�v���C���[�̃T�C�Y
//=============================================================================
//�^�C�g���N���X�̃R���X�g���N�^
//=============================================================================
CTitle::CTitle(int nPriority) :CScene(nPriority)
{
	memset(m_apPolygon, 0, sizeof(m_apPolygon));
	memset(m_apEffect, 0, sizeof(m_apEffect));
	m_EnterColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nCounter = 0;
	m_nCntEffecter = 0;
	m_state = TITLE_STATE_LOGO_MOVE;
	m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, -50.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
//�^�C�g���N���X�̃f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
//�^�C�g���N���X�̃N���G�C�g����
//=============================================================================
CTitle * CTitle::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�^�C�g���N���X�̃|�C���^�ϐ�
	CTitle *pTitle = NULL;

	//�������̊m��
	pTitle = new CTitle;

	//���������m�ۂł��Ă�����
	if (pTitle != NULL)
	{
		pTitle->Init(pos, size);
	}
	//�������m�ۂɎ��s�����Ƃ�
	else
	{
		return NULL;
	}

	return pTitle;
}

//=============================================================================
//�^�C�g���N���X�̏���������
//=============================================================================
HRESULT CTitle::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�^�C�g���w�i�p�|���S���̐���
	m_apPolygon[TITLE_TYPE_BG] = CPolygon::Create(pos, size, CPolygon::TEX_TYPE_TITLE);

	//�^�C�g�����S�p�|���S���̐���
	m_apPolygon[TITLE_TYPE_LOGO] = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH/2,-50.0f,0.0f), D3DXVECTOR3(800.0f,200.0f,0.0f), CPolygon::TEX_TYPE_TITLE_LOGO);

	//�x�N�g�������߂�
	D3DXVECTOR3 Vector = D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f) - m_pos;

	//�x�N�g���̑傫�������߂�
	float fVectorSize = sqrtf((Vector.x*Vector.x) + (Vector.y*Vector.y));

	//�P�ʃx�N�g�������߂�
	D3DXVECTOR3 VectorNor = D3DXVECTOR3(Vector.x / fVectorSize, Vector.y / fVectorSize, 0.0f);

	m_move = D3DXVECTOR3(VectorNor.x * LOGO_MOVE_SPEED, VectorNor.y * LOGO_MOVE_SPEED, 0.0f);

	return S_OK;
}

//=============================================================================
//�^�C�g���N���X�̏I������
//=============================================================================
void CTitle::Uninit(void)
{
	for (int nCount = 0; nCount < TITLE_TYPE_MAX; nCount++)
	{
		if (m_apPolygon[nCount] != NULL)
		{
			//�|���S���N���X�̏I�������Ăяo��
			m_apPolygon[nCount]->Uninit();

			//�������̍폜
			delete m_apPolygon[nCount];

			//�������̃N���A
			m_apPolygon[nCount] = NULL;
		}
	}
	
	//�I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
//�^�C�g���N���X�̍X�V
//=============================================================================
void CTitle::Update(void)
{
	//�L�[�{�[�h�N���X�̏��擾
	CInputKeyboard *pKeyBoard = CManager::GetKeyborad();

	//��ԕʂ̍X�V
	if (DecisionState() == true)
	{
		//�t�F�[�h�����擾
		CFade *pFade = CManager::GetFade();

		if (pFade != NULL)
		{
			//�t�F�[�h���s��
			pFade->SetFade(CManager::MODE_TYPE_GAME);
			
			return;
		}
	}

	if (m_nCntEffecter % 120 == 0)
	{
		//�`�[�^�[
		m_apEffect[0] = CEffect::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 400, 0.0f),
			EFFECT_SIZE, COLOR_YELLOW, 120, CEffect::EFFECT_TYPE_TITLE);
		m_apEffect[0]->SetType(CEffect::EFFECT_TYPE_CHEETAH);

		//�S����
		m_apEffect[1] = CEffect::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 200.0f, 400, 0.0f),
			EFFECT_SIZE, COLOR_PURPLE, 120, CEffect::EFFECT_TYPE_TITLE);
		m_apEffect[1]->SetType(CEffect::EFFECT_TYPE_GORILLA);

		//�J��
		m_apEffect[2] = CEffect::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) + 200.0f, 400, 0.0f),
			EFFECT_SIZE, COLOR_GREEN, 120, CEffect::EFFECT_TYPE_TITLE);
		m_apEffect[2]->SetType(CEffect::EFFECT_TYPE_TURTLE);
	}
	m_nCntEffecter++;
	//�������ł�����
	if (m_apPolygon[TITLE_TYPE_ENTER] != NULL)
	{
		//ENTER�L�[�������ꂽ��
		if (pKeyBoard->GetKeyBoardTrigger(DIK_RETURN) && m_state != TITLE_STATE_PUSH_ENTER || 
			CManager::GetJoyStick()->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_B))
		{
			//��Ԃ�TITLE_STATE_PUSH_ENTER�֕ύX
			m_state = TITLE_STATE_PUSH_ENTER;
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ENTER);
			//�J�E���^�[�N���A
			m_nCounter = 0;
		}
		
		//�J���[�̐ݒ�
		m_apPolygon[TITLE_TYPE_ENTER]->SetColor(m_EnterColor);

		//�J�E���g�A�b�v
		m_nCounter++;
	}
}

//=============================================================================
//�^�C�g���N���X�̕`�揈��
//=============================================================================
void CTitle::Draw(void)
{
	for (int nCount = 0; nCount < TITLE_TYPE_MAX; nCount++)
	{
		if (m_apPolygon[nCount] != NULL)
		{
			//�|���S���N���X�̕`�揈���Ăяo��
			m_apPolygon[nCount]->Draw();
		}
	}
}

//=============================================================================
//�^�C�g���N���X�̏�ԕʍX�V
//=============================================================================
bool CTitle::DecisionState(void)
{
	switch (m_state)
	{
	case TITLE_STATE_LOGO_MOVE:
		//�L�[�{�[�h�N���X�̏��擾
		if (CManager::GetKeyborad()->GetKeyBoardTrigger(DIK_RETURN) || CManager::GetJoyStick()->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_B))
		{
			m_pos = LOGO_POS;

			//�ʒu�̐ݒ�
			m_apPolygon[TITLE_TYPE_LOGO]->SetPos(m_pos);
		}
		
		if (m_pos.y <= 200.0f)
		{
			//�ʒu�̎擾
			m_pos = m_apPolygon[TITLE_TYPE_LOGO]->GetPos();

			//�ʒu�Ɉړ��ʂ�^����
			m_pos += m_move;

			//�ʒu�̐ݒ�
			m_apPolygon[TITLE_TYPE_LOGO]->SetPos(m_pos);
		}
		else
		{
			if (m_apPolygon[TITLE_TYPE_ENTER] == NULL)
			{
				//PRESS ENTER�p�|���S���̐���
				m_apPolygon[TITLE_TYPE_ENTER] = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200.0f, 0.0f),
					D3DXVECTOR3(600.0f, 100.0f, 0.0f), CPolygon::TEX_TYPE_PRESSENTER);

				//�ҋ@��Ԃֈڍs
				m_state = TITLE_STATE_WAIT;
			}
		}
		break;

	case TITLE_STATE_WAIT:
		if (m_nCounter % 60 == 0)
		{
			if (m_EnterColor.a == 1.0f)
			{
				m_EnterColor.a = 0.0f;
			}
			else
			{
				m_EnterColor.a = 1.0f;
			}
		}
		break;

	case TITLE_STATE_PUSH_ENTER:
		if (m_nCounter % 5 == 0)
		{
			if (m_EnterColor.a == 1.0f)
			{
				m_EnterColor.a = 0.0f;
			}
			else
			{
				m_EnterColor.a = 1.0f;
			}
		}

		if (m_nCounter % 60 == 0)
		{
			m_nCounter += 1;
			return true;
		}
		break;
	}

	return false;
}
