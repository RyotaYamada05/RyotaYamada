//=============================================================================
//
// メイン処理 [input.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "input.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;	//DirectInputオブジェクトのポインタ

//=============================================================================
//入力処理クラスのコンストラクタ
//=============================================================================
CInput::CInput()
{
}

//=============================================================================
//入力処理クラスのデストラクタ
//=============================================================================
CInput::~CInput()
{
}
//
//=============================================================================
//入力処理クラスの初期化処理
//=============================================================================
HRESULT CInput::InIt(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		//DirectInputオブジェクトの作成
		if (FAILED(DirectInput8Create(
			hInstance,				//アプリケーションのインスタンスハンドル
			DIRECTINPUT_VERSION,	//使用するDirectInputのバージョン
			IID_IDirectInput8,		//目的とするインターフェースの識別子
			(void**)&m_pInput,		//IDirectInput8インターフェースのポインタを受け取る(成否の情報が保存される)
			NULL)))
		{
			MessageBox(hWnd, "DirectInputオブジェクトの作成に失敗しました", "警告", MB_OK);
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
//入力処理クラスの終了処理
//=============================================================================
void CInput::Uninit(void)
{
	//DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		//メモリの削除
		m_pInput->Release();

		//メモリのクリア
		m_pInput = NULL;
	}
}
