//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//サウンドクラス
//=============================================================================
class CSound
{
public:
	//=========================================================================
	//列挙型定義
	//=========================================================================
	typedef enum
	{
		SOUND_LABEL_SE_SHOUT = 0,	//発射音
		SOUND_LABEL_SE_EXEPLOSION,	//爆発音
		SOUND_LABEL_SE_ENTER,	//エンターキー
		SOUND_LABEL_SE_BEAM,	//ビーム音
		SOUND_LABEL_SE_GUARD,	//ガード音
		SOUND_LABEL_SE_WARNING,	//警告音
		SOUND_LABEL_SE_GAME_CLEAR,	//ゲームクリアSE
		SOUND_LABEL_SE_GAME_OVER,	//ゲームオーバーSE
		SOUND_LABEL_BGM_TITLE,	//タイトルBGM
		SOUND_LABEL_BGM_GAME,	//ゲームBGM
		SOUND_LABEL_BGM_BOSS,	//ボス戦用BGM
		SOUND_LABEL_MAX	//ラベル最大数
	}SOUND_LABEL;

	//=========================================================================
	//サウンドのループするかどうかの列挙型
	//=========================================================================
	typedef enum
	{
		SOUND_LOOP_ON = -1,	//ループする
		SOUND_LOOP_OFF,		//ループしない
	}SOUND_LOOP;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);

private:
	//=========================================================================
	//構造体定義
	//=========================================================================
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	}PARAM;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];

	static PARAM m_aParam[SOUND_LABEL_MAX];
};

#endif 