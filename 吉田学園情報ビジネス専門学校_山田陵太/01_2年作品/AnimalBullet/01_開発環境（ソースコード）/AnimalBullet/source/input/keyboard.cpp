//=============================================================================
//
// �L�[�{�[�h���� [input.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "keyboard.h"

//=============================================================================
//�L�[�{�[�h�N���X�̃R���X�g���N�^
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	m_pKeyDevice = NULL;	//���̓f�o�C�X

	//�e�����o�ϐ���0�N���A
	memset(m_aKeyState, 0, sizeof(m_aKeyState));
	memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
	memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
}

//=============================================================================
//�L�[�{�[�h�N���X�̃f�X�g���N�^
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
//�L�[�{�[�h�N���X�̏���������
//=============================================================================
HRESULT CInputKeyboard::InIt(HINSTANCE hInstance, HWND hWnd)
{
	//���͏����N���X�̏����������Ăяo��
	CInput::InIt(hInstance, hWnd);

	//���̓f�o�C�X�i�L�[�{�[�h�j�̍쐬
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyDevice, NULL)))
	{
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g�ݒ�
	if (FAILED(m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�i�t�@�I�O���E���h����r�����[�h�j
	if (FAILED(m_pKeyDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//�L�[�{�[�h�ւ̃A�N�Z�X�����l���i���͐���J�n�j
	m_pKeyDevice->Acquire();

	return S_OK;
}

//=============================================================================
//�L�[�{�[�h�N���X�̏I������
//=============================================================================
void CInputKeyboard::Uninit(void)
{ 
	//���̓f�o�C�X�̊J��
	if (m_pKeyDevice)
	{
		//���̓f�o�C�X�ւ̃A�N�Z�X���J���i���͐���I���j
		m_pKeyDevice->Unacquire();	//�f�o�C�X����̒�~

		//�������̍폜
		m_pKeyDevice->Release();

		//�������̃N���A
		m_pKeyDevice = NULL;
	}
	//���͏����N���X�̏I�������Ăяo��
	CInput::Uninit();
}

//=============================================================================
//�L�[�{�[�h�N���X�̍X�V����
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[MAX_KEY_NUM];	//�L�[�{�[�h�̓��͏��

	//�f�o�C�X����f�[�^�擾
	if (SUCCEEDED(m_pKeyDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (int nCntKey = 0; nCntKey < MAX_KEY_NUM; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];	//�L�[�g���K�[����ۑ�
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&~aKeyState[nCntKey];	//�L�[�����[�X����ۑ�
			m_aKeyState[nCntKey] = aKeyState[nCntKey];	//�L�[�v���X����ۑ�
		}
	}
	else
	{
		//�L�[�{�[�h�ւ̃A�N�Z�X�����擾
		m_pKeyDevice->Acquire();
	}
}


//=============================================================================
//�L�[�{�[�h�̃v���X���̎擾
//=============================================================================
BOOL CInputKeyboard::GetKeyBoardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
//�L�[�{�[�h�̃g���K�[���̎擾
//=============================================================================
BOOL CInputKeyboard::GetKeyBoardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
//�L�[�{�[�h�̃����[�X���̎擾
//=============================================================================
BOOL CInputKeyboard::GetKeyBoardRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}