//=============================================================================
//
// キーボード処理 [input.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "keyboard.h"

//=============================================================================
//キーボードクラスのコンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	m_pKeyDevice = NULL;	//入力デバイス

	//各メンバ変数を0クリア
	memset(m_aKeyState, 0, sizeof(m_aKeyState));
	memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
	memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
}

//=============================================================================
//キーボードクラスのデストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
//キーボードクラスの初期化処理
//=============================================================================
HRESULT CInputKeyboard::InIt(HINSTANCE hInstance, HWND hWnd)
{
	//入力処理クラスの初期化処理呼び出し
	CInput::InIt(hInstance, hWnd);

	//入力デバイス（キーボード）の作成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyDevice, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマット設定
	if (FAILED(m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//協調モードを設定（ファオグラウンド＆非排他モード）
	if (FAILED(m_pKeyDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//キーボードへのアクセス権を獲得（入力制御開始）
	m_pKeyDevice->Acquire();

	return S_OK;
}

//=============================================================================
//キーボードクラスの終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{ 
	//入力デバイスの開放
	if (m_pKeyDevice)
	{
		//入力デバイスへのアクセス権開放（入力制御終了）
		m_pKeyDevice->Unacquire();	//デバイス制御の停止

		//メモリの削除
		m_pKeyDevice->Release();

		//メモリのクリア
		m_pKeyDevice = NULL;
	}
	//入力処理クラスの終了処理呼び出し
	CInput::Uninit();
}

//=============================================================================
//キーボードクラスの更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[MAX_KEY_NUM];	//キーボードの入力情報

	//デバイスからデータ取得
	if (SUCCEEDED(m_pKeyDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (int nCntKey = 0; nCntKey < MAX_KEY_NUM; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];	//キートリガー情報を保存
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&~aKeyState[nCntKey];	//キーリリース情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];	//キープレス情報を保存
		}
	}
	else
	{
		//キーボードへのアクセス権を取得
		m_pKeyDevice->Acquire();
	}
}


//=============================================================================
//キーボードのプレス情報の取得
//=============================================================================
BOOL CInputKeyboard::GetKeyBoardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
//キーボードのトリガー情報の取得
//=============================================================================
BOOL CInputKeyboard::GetKeyBoardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
//キーボードのリリース情報の取得
//=============================================================================
BOOL CInputKeyboard::GetKeyBoardRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}