//=============================================================================
//
// メイン処理 [keyboard.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "input.h"

//=============================================================================
//キーボードクラス
//=============================================================================
class CInputKeyboard : public CInput
{
public:
	//=========================================================================
	//メンバ関数宣言
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
	//メンバ変数宣言
	//=========================================================================
	LPDIRECTINPUTDEVICE8 m_pKeyDevice;	//入力デバイス
	BYTE m_aKeyState[MAX_KEY_NUM];	//キーボードの入力情報
	BYTE m_aKeyStateTrigger[MAX_KEY_NUM];	//キーボードのトリガー情報
	BYTE m_aKeyStateRelease[MAX_KEY_NUM];	//キーボードのリリース情報
};
#endif

