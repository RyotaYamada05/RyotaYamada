//=============================================================================
//
// メイン処理 [main.h]
// Author : 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//=============================================================================
// ヘッダファイルのインクルード
//=============================================================================
#include <windows.h>
#define DIRECTINPUT_VERSION (0x0800)	//警告対処用（"dinput.h"の直前に定義）
#include <dinput.h>
#include "d3dx9.h"
#include "XAudio2.h"

//=============================================================================
// ライブラリファイルのリンク
//=============================================================================
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")	//入力処理に必要

//=============================================================================
// マクロ定義
//=============================================================================
#define	WINDOW_POS_X	(0)
#define	WINDOW_POS_Y	(0)
#define SCREEN_WIDTH	(1280)	//ウィンドウの横幅
#define SCREEN_HEIGHT	(720)	//ウィンドウの縦幅
#define SCREEN_CNTER_POS D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f)	//ウィンドウの中心位置
#define SCREEN_SIZE D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f)	//ウィンドウのサイズ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//2Dポリゴンの頂点フォーマット
#define NUM_VERTEX (4)	//頂点数
#define NUM_POLYGON (2)	//ポリゴン数
#define VECTOR3_ZERO_INITIALIZE D3DXVECTOR3(0.0f, 0.0f, 0.0f)	//3Dベクトルのゼロ初期化マクロ
//#undef _DEBUG	//_DEBUGフラグを無効化する
//#define _RSTICK_	//コメントアウト：右スティック移動の無効化

//=============================================================================
//色のマクロ定義
//=============================================================================
#define COLOR_RED D3DCOLOR_RGBA(255,50,50,255)		//赤色
#define COLOR_BLUE D3DCOLOR_RGBA(50,50,255,255)		//青色
#define COLOR_CYAN D3DCOLOR_RGBA(50,255,255,255)	//シアン
#define COLOR_GREEN D3DCOLOR_RGBA(50,255,50,255)	//緑色
#define COLOR_YELLOW D3DCOLOR_RGBA(255,255,50,255)	//黄色
#define COLOR_PURPLE D3DCOLOR_RGBA(128,50,128,255)	//紫
#define COLOR_ORANGE D3DCOLOR_RGBA(255,165,0,255)	//オレンジ
#define COLOR_BLACK D3DCOLOR_RGBA(0,0,0,255)		//黒色
#define COLOR_WHITE D3DCOLOR_RGBA(255,255,255,255)	//白色
#define COLOR_ALPHA D3DCOLOR_RGBA(255,255,255,50)	//半透明の白色

//=============================================================================
// 2Dポリゴンの頂点情報の構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//座標変換用係数（1.0fで固定）
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

#ifdef _DEBUG
//=============================================================================
//プロトタイプ関数宣言
//=============================================================================
int GetFPS(void);
#endif

#endif