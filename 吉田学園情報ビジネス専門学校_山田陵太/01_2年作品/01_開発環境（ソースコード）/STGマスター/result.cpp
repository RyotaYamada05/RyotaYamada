//=============================================================================
//
// ���C������ [resulet.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "result.h"
#include "polygon.h"
#include "manager.h"
#include "keyboard.h"
#include "fade.h"
#include "ranking.h"
#include "sound.h"
#include "joystick.h"

//=============================================================================
//���U���g�N���X�̃R���X�g���N�^
//=============================================================================
CResult::CResult(int nPriority) : CScene(nPriority)
{
	//�����o�ϐ��̃N���A
	m_pPolygon = NULL;
	m_bEnter = false;
}

//=============================================================================
//���U���g�N���X�̃f�X�g���N�^
//=============================================================================
CResult::~CResult()
{
}

//=============================================================================
//���U���g�N���X�̃N���G�C�g����
//=============================================================================
CResult * CResult::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//���U���g�N���X�̃|�C���^�ϐ�
	CResult *pResult = NULL;

	//�������̊m��
	pResult = new CResult;

	//���������m�ۂł��Ă�����
	if (pResult != NULL)
	{
		//�����������Ăяo��
		pResult->Init(pos, size);
	}
	//�������m�ۂɎ��s�����Ƃ�
	else
	{
		return NULL;
	}

	return pResult;
}

//=============================================================================
//���U���g�N���X�̏���������
//=============================================================================
HRESULT CResult::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�|���S���𐶐�
	m_pPolygon = CPolygon::Create(pos, size, CPolygon::TEX_TYPE_RESULET);

	CRanking::Create();
	return S_OK;
}

//=============================================================================
//���U���g�N���X�̏I������
//=============================================================================
void CResult::Uninit(void)
{
	if (m_pPolygon != NULL)
	{
		//�|���S���N���X�̏I�������Ăяo��
		m_pPolygon->Uninit();

		//�������̍폜
		delete m_pPolygon;

		//�������̃N���A
		m_pPolygon = NULL;
	}

	//�I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
//���U���g�N���X�̍X�V
//=============================================================================
void CResult::Update(void)
{
	if (m_pPolygon != NULL)
	{
		//�|���S���N���X�̍X�V�����Ăяo��
		m_pPolygon->Update();
	}

	//�L�[�{�[�h�N���X�̏��擾
	CInputKeyboard *pKeyBoard = CManager::GetKeyborad();
	CInputJoyStick *pJoyStick = CManager::GetJoyStick();
	//ENTER�L�[�������ꂽ��
	if ((pKeyBoard->GetKeyBoardRelease(DIK_RETURN) || pJoyStick->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_B))&& m_bEnter == false)
	{
		m_bEnter = true;

		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ENTER);
		//�t�F�[�h�����擾
		CFade *pFade = CManager::GetFade();

		if (pFade != NULL)
		{
			//�t�F�[�h���s��
			pFade->SetFade(CManager::MODE_TYPE_TITLE);
		}

		return;
	}
}

//=============================================================================
//���U���g�N���X�̕`�揈��
//=============================================================================
void CResult::Draw(void)
{
	if (m_pPolygon != NULL)
	{
		//�|���S���N���X�̕`�揈���Ăяo��
		m_pPolygon->Draw();
	}
}
