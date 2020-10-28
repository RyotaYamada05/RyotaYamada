//=============================================================================
//
// ���C������ [keyboard.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "input.h"

//=============================================================================
//�L�[�{�[�h�N���X
//=============================================================================
class CInputKeyboard : public CInput
{
public:
	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT InIt(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	BOOL GetKeyBoardPress(int nKey);
	BOOL GetKeyBoardTrigger(int nKey);
	BOOL GetKeyBoardRelease(int nKey);

private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	LPDIRECTINPUTDEVICE8 m_pKeyDevice;	//���̓f�o�C�X
	BYTE m_aKeyState[MAX_KEY_NUM];	//�L�[�{�[�h�̓��͏��
	BYTE m_aKeyStateTrigger[MAX_KEY_NUM];	//�L�[�{�[�h�̃g���K�[���
	BYTE m_aKeyStateRelease[MAX_KEY_NUM];	//�L�[�{�[�h�̃����[�X���
};
#endif

