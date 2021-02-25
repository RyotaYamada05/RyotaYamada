//=============================================================================
//
// ジョイスティック処理 [joystick.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "joystick.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAN_RANGE 1000	//スティックの最大範囲
#define MIN_RANGE -1000	//スティックの最小範囲

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECTINPUTDEVICE8 CInputJoyStick::m_pJoyDevice = NULL;

//=============================================================================
//ジョイスティッククラスのコンストラクタ
//=============================================================================
CInputJoyStick::CInputJoyStick()
{
	memset(&m_JoyState, 0, sizeof(m_JoyState));
}

//=============================================================================
//ジョイスティッククラスのデストラクタ
//=============================================================================
CInputJoyStick::~CInputJoyStick()
{
}

//=============================================================================
//ジョイスティッククラスの初期化処理
//=============================================================================
HRESULT CInputJoyStick::InIt(HINSTANCE hInstance, HWND hWnd)
{
	//入力処理の初期化
	CInput::InIt(hInstance, hWnd);

	//ジョイスティックの列挙
	if (FAILED(m_pInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,	//列挙したいデバイスの種類
		DeviceFindCallBack,		//デバイスが列挙した際に実行される関数ポインタ(コールバック関数)
		NULL,					//第二引数実行時に引数として渡されるデータのポインタ
		DIEDFL_ATTACHEDONLY)))	//取得するデバイスを限定するかどうかのフラグ
	{
		MessageBox(hWnd, "ジョイスティックデバイスの列挙に失敗しました", "警告",MB_OK);

		return E_FAIL;
	}

	//ジョイスティックが見つからなかった場合
	if (!m_pJoyDevice)
	{
#ifdef _DEBUG
		MessageBox(hWnd, "ジョイスティックが見つかりませんでした", "警告", MB_OK);
#endif
		return E_FAIL;
	}

	if (m_pJoyDevice != NULL)
	{
		//データフォーマットの設定
		if (FAILED(m_pJoyDevice->SetDataFormat(&c_dfDIJoystick)))
		{
			MessageBox(NULL, "ジョイスティックデバイスのデータフォーマットの設定が失敗しました", "警告", MB_OK);
			m_pJoyDevice->Release();
			return DIENUM_STOP;
		}

		//協調モードの設定
		if (FAILED(m_pJoyDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		{
			MessageBox(NULL, "ジョイスティックデバイスの協調モードの設定が失敗しました", "警告", MB_OK);
			m_pJoyDevice->Release();
			return DIENUM_STOP;
		}

		//入力範囲の設定
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(diprg);	//ヘッダが保持している構造体のサイズ
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);	//DIPROPHEADER構造体サイズ
		diprg.diph.dwHow = DIPH_BYOFFSET;	//オブジェクトの解釈方法
		diprg.lMax = MAN_RANGE;	//軸の値の最大範囲
		diprg.lMin = MIN_RANGE;	//軸の値の最小範囲

		//X軸の設定
		diprg.diph.dwObj = DIJOFS_X;
		m_pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		//Y軸の設定
		diprg.diph.dwObj = DIJOFS_Y;
		m_pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		//Z軸の設定
		diprg.diph.dwObj = DIJOFS_Z;
		m_pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		//RX軸の設定
		diprg.diph.dwObj = DIJOFS_RX;
		m_pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		//RY軸の設定
		diprg.diph.dwObj = DIJOFS_RY;
		m_pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		//RZ軸の設定
		diprg.diph.dwObj = DIJOFS_RZ;
		m_pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		//起動準備完了
		m_pJoyDevice->Poll();

#ifdef _DEBUG
		MessageBox(NULL, "ジョイスティックデバイス作成に成功", "報告", MB_OK);
#endif 
		//制御開始
		if (m_pJoyDevice == NULL)
		{
			Uninit();
		}

		m_pJoyDevice->Acquire();
	}

	return S_OK;
}

//=============================================================================
//ジョイスティッククラスの終了処理
//=============================================================================
void CInputJoyStick::Uninit(void)
{
	//デバイス制御の停止
	if (m_pJoyDevice != NULL)
	{
		m_pJoyDevice->Unacquire();
		m_pJoyDevice->Release();
		m_pJoyDevice = NULL;
	}

	CInput::Uninit();
}

//=============================================================================
//ジョイスティッククラスの更新処理
//=============================================================================
void CInputJoyStick::Update(void)
{
	DIJOYSTATE js;
	SecureZeroMemory(&js, sizeof(js));

	if (m_pJoyDevice != NULL)
	{
		//1フレーム前の情報を保存
		m_JoyState.Old.rgdwPOV[0] = m_JoyState.Press.rgdwPOV[0];

		m_pJoyDevice->Poll();

		if (SUCCEEDED(m_pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE), &js)))
		{
			//十字キーの情報を保存
			m_JoyState.Press.rgdwPOV[0] = js.rgdwPOV[0];

			//ボタン情報
			for (int nCntButton = 0; nCntButton < JOY_BUTTON_MAX; nCntButton++)
			{
				//トリガー情報を保存
				m_JoyState.Trigger.rgbButtons[nCntButton] = (m_JoyState.Press.rgbButtons[nCntButton] ^ js.rgbButtons[nCntButton])&js.rgbButtons[nCntButton];

				//リリース情報を保存
				m_JoyState.Release.rgbButtons[nCntButton] = (m_JoyState.Press.rgbButtons[nCntButton] ^ js.rgbButtons[nCntButton])&~js.rgbButtons[nCntButton];

				//プレス保存
				m_JoyState.Press.rgbButtons[nCntButton] = js.rgbButtons[nCntButton];
			}
		}
		else
		{
			//失敗した場合再度接続を試みる
			m_pJoyDevice->Acquire();
		}
	}
}

//=============================================================================
//ジョイスティッククラスのプレス情報の取得
//=============================================================================
bool CInputJoyStick::GetJoyStickPress(int nButton)
{
	return (m_JoyState.Press.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
//ジョイスティッククラスのトリガー情報の取得
//=============================================================================
bool CInputJoyStick::GetJoyStickTrigger(int nButton)
{
	return (m_JoyState.Trigger.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
//ジョイスティッククラスのリリース情報の取得
//=============================================================================
bool CInputJoyStick::GetJoyStickReleas(int nButton)
{
	return (m_JoyState.Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
//ジョイスティッククラスの十字キーが押された瞬間の判定処理
//=============================================================================
bool CInputJoyStick::GetPushCross(int nButton)
{
	//前回が何も押されていないかつ、現在が押されていたら
	if (m_JoyState.Old.rgdwPOV[0] == 0xFFFFFFFF && 
		m_JoyState.Press.rgdwPOV[0] == nButton)
	{
		return true;
	}
	return false;
}

//=============================================================================
//ジョイスティッククラスの十字キーが離した瞬間の判定処理
//=============================================================================
bool CInputJoyStick::GetPushRelease(int nButtond)
{
	//前回が押されているかつ、現在が押されていない
	if (m_JoyState.Old.rgdwPOV[0] == nButtond && 
		m_JoyState.Press.rgdwPOV[0] == 0xFFFFFFFF)
	{
		return true;
	}
	return false;
}

//=============================================================================
//ジョイスティッククラスのスティック情報の取得
//=============================================================================
DIJOYSTATE CInputJoyStick::GetJoyStick(void)
{
	DIJOYSTATE js;
	SecureZeroMemory(&js, sizeof(js));

	if (m_pJoyDevice != NULL)
	{
		m_pJoyDevice->Poll();
		m_pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE), &js);
	}

	return js;
}


//=============================================================================
//ジョイスティッククラスのデバイス列挙コールバック関数
//=============================================================================
BOOL CALLBACK CInputJoyStick::DeviceFindCallBack(const DIDEVICEINSTANCE * pdidInstance, VOID * pContext)
{
	//ジョイスティックの作成
	if (FAILED(m_pInput->CreateDevice(
		pdidInstance->guidInstance,
		&m_pJoyDevice,
		NULL)))
	{
		MessageBox(NULL, "ジョイスティックデバイスの作成に失敗しました", "警告", MB_OK);
		return DIENUM_STOP;
	}

	//検索を終了する
	return DIENUM_STOP;
}



