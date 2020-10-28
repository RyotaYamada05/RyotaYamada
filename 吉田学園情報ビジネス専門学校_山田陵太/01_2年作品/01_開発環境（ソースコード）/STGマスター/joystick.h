//=============================================================================
//
// メイン処理 [joystick.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "input.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_JoyStick_NUM 2	//コントローラーの接続最大数
#define CROSS_KEY_UP	0				//十字キー上
#define CROSS_KEY_TOP_RIGHT 4500		//十字キー右上
#define CROSS_KEY_RIGHT	9000			//十字キー右
#define CROSS_KEY_BOTTOM_RIGHT 13500	//十字キー右下
#define CROSS_KEY_DOWN	18000			//十字キー下
#define CROSS_KEY_BOTTOM_LEFT 22500		//十字キー左下
#define CROSS_KEY_LEFT	27000			//十字キー左
#define CROSS_KEY_TOP_LEFT	31500		//十字キー左上

//=============================================================================
//ジョイスティッククラス
//=============================================================================
class CInputJoyStick :public CInput
{
public:
	//=============================================================================
	//ジョイスティックのボタン用の列挙型定義
	//=============================================================================
	typedef enum
	{
		JOY_BUTTON_X = 0,		//Xボタン
		JOY_BUTTON_Y,			//Yボタン
		JOY_BUTTON_A,			//Aボタン
		JOY_BUTTON_B,			//Bボタン
		JOY_BUTTON_L_TRIGGER,	//Lトリガーボタン
		JOY_BUTTON_R_TRIGGER,	//Rトリガーボタン
		JOY_BUTTON_L_TRIGGER2,	//Lトリガーボタン2
		JOY_BUTTON_R_TRIGGER2,	//Rトリガーボタン2
		JOY_BUTTON_L3,			//Lボタン
		JOY_BUTTON_R3,			//Rボタン
		JOY_BUTTON_SELECT,		//SELECTボタン
		JOY_BUTTON_START,		//STARTボタン
		JOY_BUTTON_HOME,		//HOMEボタン
		JOY_BUTTON_MAX,			//最大ボタン数
	}JOY_BUTTON;

	//=============================================================================
	//ジョイスティック状態判別用の構造体定義
	//=============================================================================
	typedef struct
	{
		DIJOYSTATE	Old;		//パッドボタンの前回情報
		DIJOYSTATE	Press;		//パッドボタンのプレス情報
		DIJOYSTATE	Trigger;	//パッドボタンのトリガー情報
		DIJOYSTATE	Release;	//パッドボタンのリリース情報
	}JoyStickState;

	//=============================================================================
	//メンバ関数宣言
	//=============================================================================
	CInputJoyStick();
	~CInputJoyStick();

	HRESULT InIt(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetJoyStickPress(int nButton);
	bool GetJoyStickTrigger(int nButton);
	bool GetJoyStickReleas(int nButton);

	bool GetPushCross(int nButtond);
	bool GetPushRelease(int nButtond);

	DIJOYSTATE GetJoyStick(void);
	static BOOL CALLBACK DeviceFindCallBack(const DIDEVICEINSTANCE * pdidInstance, VOID * pContext);
private:
	//=============================================================================
	//メンバ変数宣言
	//=============================================================================
	static LPDIRECTINPUTDEVICE8 m_pJoyDevice;	//ジョイスティックのデバイス情報
	JoyStickState m_JoyState;	//ジョイスティックの状態
};


#endif
