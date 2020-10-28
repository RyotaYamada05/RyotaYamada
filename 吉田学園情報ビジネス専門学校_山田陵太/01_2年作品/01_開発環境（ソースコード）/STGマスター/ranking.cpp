//=============================================================================
//
// メイン処理 [ranking.cpp]
// Author : 山田陵太
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "ranking.h"
#include "score.h"
#include "number.h"

#define RANKING_NUM_SIZE_X 70.0f
#define RANKING_NUM_SIZE_Y 70.0f
#define RANKING_NUM_SIZE D3DXVECTOR3(RANKING_NUM_SIZE_X,RANKING_NUM_SIZE_Y,0.0f)
CRanking::CRanking()
{
	m_nRank = 0;
	m_nScore = 0;
	memset(m_apNumber, 0, sizeof(m_apNumber));
	memset(m_nRanking, 0, sizeof(m_nRanking));
	memset(m_apRank, 0, sizeof(m_apRank));
	memset(m_apScoreNum, 0, sizeof(m_apScoreNum));
}

CRanking::~CRanking()
{
}

CRanking * CRanking::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CRanking *pRanking = NULL;

	pRanking = new CRanking;

	if (pRanking != NULL)
	{
		pRanking->Init(pos,size);
	}
	return nullptr;
}

HRESULT CRanking::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//ランキングの設定
	SetRanking(CScore::GetScore());

	SetNumber();
	return E_NOTIMPL;
}

void CRanking::Uninit(void)
{
	for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
	{
		if (m_apRank[nCntRanking] != NULL)
		{
			m_apRank[nCntRanking]->Uninit();
			delete m_apRank[nCntRanking];
			m_apRank[nCntRanking] = NULL;

		}
		for (int nCntNum = 0; nCntNum < NUM_LENGTH; nCntNum++)
		{
			if (m_apNumber[nCntRanking][nCntNum] != NULL)
			{
				m_apNumber[nCntRanking][nCntNum]->Uninit();

				delete m_apNumber[nCntRanking][nCntNum];

				m_apNumber[nCntRanking][nCntNum] = NULL;
			}
			if (m_apScoreNum[nCntNum] != NULL)
			{
				m_apScoreNum[nCntNum]->Uninit();
				delete m_apScoreNum[nCntNum];
				m_apScoreNum[nCntNum] = NULL;
			}
		}
	}

}

void CRanking::Update(void)
{
}

void CRanking::Draw(void)
{
	for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
	{
		if (m_apRank[nCntRanking] != NULL)
		{
			m_apRank[nCntRanking]->Draw();
		}
		for (int nCntNum = 0; nCntNum < NUM_LENGTH; nCntNum++)
		{
			if (m_apNumber[nCntRanking][nCntNum] != NULL)
			{
				
				m_apNumber[nCntRanking][nCntNum]->Draw();
			}

			if (m_apScoreNum[nCntNum] != NULL)
			{
				m_apScoreNum[nCntNum]->Draw();
			}
		}
	}
}

void CRanking::ReadFile(void)
{
	//ファイルポインタ
	FILE *pFile;

	//読み込みモードでファイルを開く
	pFile = fopen("data/Resource/ranking.txt", "r");

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

void CRanking::WriteFile(void)
{
	//ファイルポインタ
	FILE *pFile;

	//書き込みモードで開く
	pFile = fopen("data/Resource/ranking.txt", "w");

	if (pFile != NULL)
	{
		//10人分のデータを読み込む
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

void CRanking::SetRanking(int nScore)
{
	int nCount;
	m_nScore = nScore;
	//ランキングデータを読み込む
	ReadFile();

	for (nCount = 0; nCount < RANKING_NUM; nCount++)
	{
		if (m_nScore > m_nRanking[nCount])
		{
			//ランキングを後ろにずらす
			for (int nCntMove = RANKING_NUM - 1; nCntMove > nCount; nCntMove--)
			{
				m_nRanking[nCntMove] = m_nRanking[nCntMove - 1];
			}
			m_nRanking[nCount] = m_nScore;

			//結果を書き込む
			WriteFile();

			//更新された順位を設定する
			m_nRank = nCount;
			return;
		}
	}
	//更新されなかった場合、-1位として設定する
	m_nRank = -1;

	return;
}

void CRanking::SetNumber(void)
{
	float fIndex;  //指数　(スコア表示用)
	float fRadix = 10.0f; //基数　(スコア表示用)

	//スコアを表示する
	for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
	{
		fIndex = 0;
		//順位用ナンバーの生成
		m_apRank[nCntRanking] = CNumber::Create(nCntRanking+1, CNumber::NUMBER_TYPE_001,//表示する数字と種類
			D3DXVECTOR3((SCREEN_WIDTH / 2)
				- (RANKING_NUM_SIZE_X*RANKING_NUM),
				(SCREEN_HEIGHT / 2) - (RANKING_NUM_SIZE_Y * RANKING_NUM) / 2//Y軸の基準位置
				+ ((nCntRanking * RANKING_NUM_SIZE_Y) + (RANKING_NUM_SIZE_X / 2)), //各ランキングのy軸の位置
				0.0f),	//zの位置
			RANKING_NUM_SIZE, D3DXCOLOR(0, 0, 0, 255));	//サイズと色

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

	if (m_nRank != -1)
	{
		for (int nCntNum = 0; nCntNum < NUM_LENGTH; nCntNum++)
		{
			if (m_apNumber[m_nRank][nCntNum] != NULL)
			{
				m_apNumber[m_nRank][nCntNum]->SetColr(D3DXCOLOR(255, 0, 0, 255));
			}
		}
	}
}
