//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : 山田陵太
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "ranking.h"
#include "score.h"
#include "number.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define RANKING_NUM_SIZE_X 70.0f	//ランキングのX軸のサイズ
#define RANKING_NUM_SIZE_Y 70.0f	//ランキングのY軸のサイズ
#define RANKING_NUM_SIZE D3DXVECTOR3(RANKING_NUM_SIZE_X,RANKING_NUM_SIZE_Y,0.0f)//ランキングのサイズ
#define RANKING_DATA_FILE "data/Resource/ranking.txt"	//ランキングファイルのパス

//=============================================================================
//ランキングクラスのコンストラクタ
//=============================================================================
CRanking::CRanking()
{
	//各メンバ変数のクリア
	m_nRank = 0;
	m_nScore = 0;
	memset(m_apNumber, 0, sizeof(m_apNumber));
	memset(m_nRanking, 0, sizeof(m_nRanking));
	memset(m_apRank, 0, sizeof(m_apRank));
	memset(m_apScoreNum, 0, sizeof(m_apScoreNum));
}

//=============================================================================
//ランキングクラスのデストラクタ
//=============================================================================
CRanking::~CRanking()
{
}

//=============================================================================
//ランキングクラスのクリエイト処理
//=============================================================================
CRanking * CRanking::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CRanking *pRanking = NULL;

	//インスタンス生成
	pRanking = new CRanking;

	//メモリが確保できていたら
	if (pRanking != NULL)
	{
		//初期化処理呼び出し
		pRanking->Init(pos,size);
	}
	//失敗したら
	else
	{
		return NULL;
	}

	return pRanking;
}

//=============================================================================
//ランキングクラスの初期化処理
//=============================================================================
HRESULT CRanking::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//ランキングの設定
	SetRanking(CScore::GetScore());

	//ナンバーの設定
	SetNumber();
	return E_NOTIMPL;
}

//=============================================================================
//ランキングクラスの終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
	{
		//順位数字の終了
		if (m_apRank[nCntRanking] != NULL)
		{
			//終了処理
			m_apRank[nCntRanking]->Uninit();
			
			//メモリの削除
			delete m_apRank[nCntRanking];

			//メモリのクリア
			m_apRank[nCntRanking] = NULL;

		}
		for (int nCntNum = 0; nCntNum < NUM_LENGTH; nCntNum++)
		{
			//ナンバーの終了
			if (m_apNumber[nCntRanking][nCntNum] != NULL)
			{
				//終了処理
				m_apNumber[nCntRanking][nCntNum]->Uninit();

				//メモリの削除
				delete m_apNumber[nCntRanking][nCntNum];

				//メモリのクリア
				m_apNumber[nCntRanking][nCntNum] = NULL;
			}

			//スコアナンバーの終了
			if (m_apScoreNum[nCntNum] != NULL)
			{
				//終了処理
				m_apScoreNum[nCntNum]->Uninit();

				//メモリの削除
				delete m_apScoreNum[nCntNum];

				//メモリのクリア
				m_apScoreNum[nCntNum] = NULL;
			}
		}
	}
}

//=============================================================================
//ランキングクラスの更新処理
//=============================================================================
void CRanking::Update(void)
{
}

//=============================================================================
//ランキングクラスの描画処理
//=============================================================================
void CRanking::Draw(void)
{
	for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
	{
		//順位数字の描画
		if (m_apRank[nCntRanking] != NULL)
		{
			m_apRank[nCntRanking]->Draw();
		}
		for (int nCntNum = 0; nCntNum < NUM_LENGTH; nCntNum++)
		{
			//ナンバーの描画
			if (m_apNumber[nCntRanking][nCntNum] != NULL)
			{
				m_apNumber[nCntRanking][nCntNum]->Draw();
			}

			//スコアナンバーの描画
			if (m_apScoreNum[nCntNum] != NULL)
			{
				m_apScoreNum[nCntNum]->Draw();
			}
		}
	}
}

//=============================================================================
//ランキングクラスのファイル読み込み
//=============================================================================
void CRanking::ReadFile(void)
{
	//ファイルポインタ
	FILE *pFile;

	//読み込みモードでファイルを開く
	pFile = fopen(RANKING_DATA_FILE, "r");

	if (pFile != NULL)
	{
		//10人分のデータを読み込む
		for (int nCount = 0; nCount < RANKING_NUM; nCount++)
		{
			fscanf(pFile, "%d", &m_nRanking[nCount]);	//スコア
		}

		//ファイルクローズ
		fclose(pFile);
	}
	else
	{
		return;
	}
}

//=============================================================================
//ランキングクラスのファイル書き込み
//=============================================================================
void CRanking::WriteFile(void)
{
	//ファイルポインタ
	FILE *pFile;

	//書き込みモードで開く
	pFile = fopen(RANKING_DATA_FILE, "w");

	if (pFile != NULL)
	{
		//10人分のデータを書き込む
		for (int nCount = 0; nCount < RANKING_NUM; nCount++)
		{
			fprintf(pFile, "%d\n", m_nRanking[nCount]);	//スコア
		}

		//ファイルクローズ
		fclose(pFile);
	}
	else
	{
		return;
	}
}

//=============================================================================
//ランキングクラスのランキングの設定処理
//=============================================================================
void CRanking::SetRanking(int nScore)
{
	int nCntRanking;

	//ゲーム終了時点のスコアの設定
	m_nScore = nScore;

	//ランキングデータを読み込む
	ReadFile();

	for (nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
	{
		if (m_nScore > m_nRanking[nCntRanking])
		{
			//ランキングを後ろにずらす
			for (int nCntRankMove = RANKING_NUM - 1; nCntRankMove > nCntRanking; nCntRankMove--)
			{
				m_nRanking[nCntRankMove] = m_nRanking[nCntRankMove - 1];
			}
			m_nRanking[nCntRanking] = m_nScore;

			//結果を書き込む
			WriteFile();

			//更新された順位を設定する
			m_nRank = nCntRanking;
			return;
		}
	}
	//更新されなかった場合、-1位として設定する
	m_nRank = -1;

	return;
}

//=============================================================================
//ランキングクラスのナンバー設定処理
//=============================================================================
void CRanking::SetNumber(void)
{
	float fIndex;  //指数　(スコア表示用)
	float fRadix = 10.0f; //基数　(スコア表示用)

	//ランキングを表示する
	for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
	{
		fIndex = 0;

		//順位用ナンバーの生成
		m_apRank[nCntRanking] = CNumber::Create(nCntRanking+1, CNumber::NUMBER_TYPE_001,//表示する数字と種類
			D3DXVECTOR3((SCREEN_WIDTH / 2) - (RANKING_NUM_SIZE_X*RANKING_NUM),	//X軸の位置
				(SCREEN_HEIGHT / 2) - (RANKING_NUM_SIZE_Y * RANKING_NUM) / 2//Y軸の基準位置
				+ ((nCntRanking * RANKING_NUM_SIZE_Y) + (RANKING_NUM_SIZE_X / 2)), //各ランキングのy軸の位置
				0.0f),	//zの位置
			RANKING_NUM_SIZE, D3DXCOLOR(0, 0, 0, 255));	//サイズと色

		//8桁分クリエイトするためのfor文
		for (int nCntNum = 0; nCntNum < NUM_LENGTH; nCntNum++, fIndex++)
		{
			//各桁の値を求める
			int nScore = (int)powf(fRadix, NUM_LENGTH - fIndex);
			int nScore2 = (int)powf(fRadix, NUM_LENGTH - fIndex - 1);
			int nAnswer = (m_nRanking[nCntRanking] % nScore) / nScore2;

			//ナンバークラスの生成
			m_apNumber[nCntRanking][nCntNum] = CNumber::Create(nAnswer, CNumber::NUMBER_TYPE_001,	//表示する数字と種類
				D3DXVECTOR3((SCREEN_WIDTH / 2) - (RANKING_NUM_SIZE_X * NUM_LENGTH) / 2 //X軸の基準位置
					+ ((nCntNum * RANKING_NUM_SIZE_X) + (RANKING_NUM_SIZE_X / 2)),		//各桁のx軸の位置
					(SCREEN_HEIGHT / 2) - (RANKING_NUM_SIZE_Y * RANKING_NUM)/2//Y軸の基準位置
					+ ((nCntRanking * RANKING_NUM_SIZE_Y) + (RANKING_NUM_SIZE_X / 2)), //各ランキングのy軸の位置
					0.0f),	//zの位置
				RANKING_NUM_SIZE, D3DXCOLOR(0, 0, 0, 255));	//サイズと色
		}
	}

	//ゲーム終了時のスコア用の生成用for文
	for (int nCount = 0, fIndex = 0; nCount < SCORE_MAX_NUM; nCount++, fIndex++)
	{
			//各桁の値を求める
			int nScore = (int)powf(fRadix, SCORE_MAX_NUM - fIndex);
			int nScore2 = (int)powf(fRadix, SCORE_MAX_NUM - fIndex - 1);
			int nAnswer = (m_nScore % nScore) / nScore2;

			m_apScoreNum[nCount] = CNumber::Create(nAnswer, CNumber::NUMBER_TYPE_001,	//表示する数字と種類
				D3DXVECTOR3(960.0f + ((nCount * RANKING_NUM_SIZE_X/2) + (RANKING_NUM_SIZE_X / 2)),		//各桁のx軸の位置
					670.0f, //各y軸の位置
					0.0f),	//zの位置
				RANKING_NUM_SIZE/1.5f, D3DXCOLOR(0, 0, 0, 255));	//サイズと色
			
	}

	//ランクインしていたら
	if (m_nRank != -1)
	{
		//各桁のスコアの色を変える
		for (int nCntNum = 0; nCntNum < NUM_LENGTH; nCntNum++)
		{
			if (m_apNumber[m_nRank][nCntNum] != NULL)
			{
				m_apNumber[m_nRank][nCntNum]->SetColr(D3DXCOLOR(255, 0, 0, 255));
			}
		}
	}
}