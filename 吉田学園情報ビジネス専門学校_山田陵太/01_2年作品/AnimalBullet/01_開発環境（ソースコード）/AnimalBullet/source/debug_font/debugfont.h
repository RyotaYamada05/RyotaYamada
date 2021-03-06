//=============================================================================
//
// デバッグフォント処理 [debugfont.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _DEBUG_H_
#define _DEBUG_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define STR_LENGTH 256	//文字列の長さ

//=============================================================================
//デバッグ情報表示クラス
//=============================================================================
class CDebugFont
{
public:
	//=========================================================================
	//フォントの幅寄せ情報の構造体
	//=========================================================================
	typedef enum
	{
		DEBUG_LAYER_LEFT = 0,	//左寄せ
		DEBUG_LAYER_RIGHT,		//右寄せ
		DEBUG_LAYER_MAX
	}DEBUG_LAYER;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CDebugFont();
	~CDebugFont();

	static void Init(void);
	static void Uninit(void);
	static void Print(DEBUG_LAYER nLayer, char *pFmt, ...);
	static void Draw(void);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPD3DXFONT m_pFont;	//フォントのポインタ変数
	static char m_aString[DEBUG_LAYER_MAX][STR_LENGTH];	//描画内容文字列
};
#endif 