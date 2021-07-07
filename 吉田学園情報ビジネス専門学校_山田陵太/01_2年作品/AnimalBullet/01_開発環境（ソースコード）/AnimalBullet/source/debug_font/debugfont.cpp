//=============================================================================
//
// デバッグフォント処理 [debugfont.h]
// Author : 山田陵太
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//警告抑制
#include "debugfont.h"
//#include <stdarg.h>
#include <stdio.h>
#include "manager.h"
#include "renderer.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPD3DXFONT CDebugFont::m_pFont = NULL;
char CDebugFont::m_aString[DEBUG_LAYER_MAX][STR_LENGTH] = {};

//=============================================================================
//デバッグ情報表示クラスのコンストラクタ
//=============================================================================
CDebugFont::CDebugFont()
{
}

//=============================================================================
//デバッグ情報表示クラスのデストラクタ
//=============================================================================
CDebugFont::~CDebugFont()
{
}

//=============================================================================
//デバッグ情報表示クラスの初期化処理
//=============================================================================
void CDebugFont::Init(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//デバッグ情報表示用フォントの生成
	D3DXCreateFont(pD3DDevice,	//デバイス
		18,						//文字の高さ
		0,						//文字幅
		0,						//フォントの太さ
		0,						//下線
		FALSE,					//斜体かどうか
		SHIFTJIS_CHARSET,		//文字セット
		OUT_DEFAULT_PRECIS,		//出力精度
		DEFAULT_QUALITY,		//出力品質
		DEFAULT_PITCH,			//フォントピッチ
		"Terminal",				//フォントの種類
		&m_pFont);				//フォントポインタへのアドレス
}

//=============================================================================
//デバッグ情報表示クラスの終了処理
//=============================================================================
void CDebugFont::Uninit(void)
{
	if (m_pFont)
	{
		//フォントのポインタの解放
		m_pFont->Release();

		//ポインタのクリア
		m_pFont = NULL;
	}
}

//=============================================================================
//デバッグ情報表示クラスの情報設定処理
//=============================================================================
void CDebugFont::Print(DEBUG_LAYER nLayer, char * pFmt, ...)
{
	char pMoji[256];

	ZeroMemory(pMoji, sizeof(pMoji));

	//リスト変数
	va_list list;

	//可変引数リストの初期化
	va_start(list, pFmt);

	vsprintf(pMoji, pFmt, list);

	//文字列を結合する
	strcat(m_aString[nLayer], pMoji);

	//リストの終了
	va_end(list);
}


//=============================================================================
//デバッグ情報表示クラスの描画処理
//=============================================================================
void CDebugFont::Draw(void)
{
	RECT rect;

	if (m_pFont)
	{
		for (int nCntLayer = 0; nCntLayer < DEBUG_LAYER_MAX; nCntLayer++)
		{
			if (nCntLayer == 0)
			{
				SetRect(&rect, 0, 18, SCREEN_WIDTH, SCREEN_HEIGHT);

				//描画範囲の取得
				m_pFont->DrawText(NULL, m_aString[nCntLayer], -1, &rect,
					DT_LEFT | DT_CALCRECT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

				OffsetRect(&rect, 0, 0);

				// テキスト描画
				m_pFont->DrawText(NULL, m_aString[nCntLayer], -1, &rect,
					DT_LEFT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
			}
			else
			{
				SetRect(&rect, 0, 18, SCREEN_WIDTH, SCREEN_HEIGHT);

				//描画範囲の取得
				m_pFont->DrawText(NULL, m_aString[nCntLayer], -1, &rect,
					DT_RIGHT | DT_CALCRECT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

				OffsetRect(&rect, 0, 0);

				// テキスト描画
				m_pFont->DrawText(NULL, m_aString[nCntLayer], -1, &rect,
					DT_RIGHT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
			}
		}
		//すべての情報を表示し終わったらメモリをクリアする
		ZeroMemory(m_aString, sizeof(m_aString));
	}
}