//=============================================================================
//
// メイン処理 [manager.h]
// Author : 
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//前方宣言
//=============================================================================
class CRenderer;
class CInputKeyboard;
class CSound;
class CTitle;
class CGame;
class CResult;
class CFade;
class CInputJoyStick;

//=============================================================================
//マネージャークラス
//=============================================================================
class CManager
{
public:
	//=========================================================================
	//列挙型定義
	//=========================================================================
	typedef enum
	{ 
		MODE_TYPE_TITLE = 0,	//タイトルモード
		MODE_TYPE_GAME,	//ゲームモード
		MODE_TYPE_RESULT,	//リザルトモード
		MODE_TYPE_MAX,
	}MODE_TYPE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void LoadAll(void);
	void UnLoadAll(void);

	static void SetMode(MODE_TYPE mode);
	static MODE_TYPE GetMode(void);

	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetKeyborad(void);
	static CSound *GetSound(void);
	static CFade *GetFade(void);
	static CInputJoyStick *GetJoyStick(void);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static MODE_TYPE m_mode;	//ゲームモード判定用変数
	static CRenderer *m_pRenderer;	//レンダラークラスのポインタ
	static CInputKeyboard *m_pKeyBoard;	//キーボードクラスのポインタ
	static CSound *m_pSound;	//サウンドクラスのポインタ
	static CTitle *m_pTitle;
	static CGame *m_pGame;
	static CResult *m_pResult;	
	static CFade *m_pFade;
	static CInputJoyStick *m_pJoyStick;
};

#endif