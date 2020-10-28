//=============================================================================
//
// ���C������ [ranking.cpp]
// Author : �R�c�ˑ�
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
	//�����L���O�̐ݒ�
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
	//�t�@�C���|�C���^
	FILE *pFile;

	//�ǂݍ��݃��[�h�Ńt�@�C�����J��
	pFile = fopen("data/Resource/ranking.txt", "r");

	if (pFile != NULL)
	{
		//10�l���̃f�[�^��ǂݍ���
		for (int nCount = 0; nCount < RANKING_NUM; nCount++)
		{
			fscanf(pFile, "%d", &m_nRanking[nCount]);	//�X�R�A
		}

		//�t�@�C���N���[�Y
		fclose(pFile);
	}
	else
	{
		return;
	}
}

void CRanking::WriteFile(void)
{
	//�t�@�C���|�C���^
	FILE *pFile;

	//�������݃��[�h�ŊJ��
	pFile = fopen("data/Resource/ranking.txt", "w");

	if (pFile != NULL)
	{
		//10�l���̃f�[�^��ǂݍ���
		for (int nCount = 0; nCount < RANKING_NUM; nCount++)
		{
			fprintf(pFile, "%d\n", m_nRanking[nCount]);	//�X�R�A
		}

		//�t�@�C���N���[�Y
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
	//�����L���O�f�[�^��ǂݍ���
	ReadFile();

	for (nCount = 0; nCount < RANKING_NUM; nCount++)
	{
		if (m_nScore > m_nRanking[nCount])
		{
			//�����L���O�����ɂ��炷
			for (int nCntMove = RANKING_NUM - 1; nCntMove > nCount; nCntMove--)
			{
				m_nRanking[nCntMove] = m_nRanking[nCntMove - 1];
			}
			m_nRanking[nCount] = m_nScore;

			//���ʂ���������
			WriteFile();

			//�X�V���ꂽ���ʂ�ݒ肷��
			m_nRank = nCount;
			return;
		}
	}
	//�X�V����Ȃ������ꍇ�A-1�ʂƂ��Đݒ肷��
	m_nRank = -1;

	return;
}

void CRanking::SetNumber(void)
{
	float fIndex;  //�w���@(�X�R�A�\���p)
	float fRadix = 10.0f; //��@(�X�R�A�\���p)

	//�X�R�A��\������
	for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
	{
		fIndex = 0;
		//���ʗp�i���o�[�̐���
		m_apRank[nCntRanking] = CNumber::Create(nCntRanking+1, CNumber::NUMBER_TYPE_001,//�\�����鐔���Ǝ��
			D3DXVECTOR3((SCREEN_WIDTH / 2)
				- (RANKING_NUM_SIZE_X*RANKING_NUM),
				(SCREEN_HEIGHT / 2) - (RANKING_NUM_SIZE_Y * RANKING_NUM) / 2//Y���̊�ʒu
				+ ((nCntRanking * RANKING_NUM_SIZE_Y) + (RANKING_NUM_SIZE_X / 2)), //�e�����L���O��y���̈ʒu
				0.0f),	//z�̈ʒu
			RANKING_NUM_SIZE, D3DXCOLOR(0, 0, 0, 255));	//�T�C�Y�ƐF

		for (int nCntNum = 0; nCntNum < NUM_LENGTH; nCntNum++, fIndex++)
		{
			//�e���̒l�����߂�
			int nScore = (int)powf(fRadix, NUM_LENGTH - fIndex);
			int nScore2 = (int)powf(fRadix, NUM_LENGTH - fIndex - 1);
			int nAnswer = (m_nRanking[nCntRanking] % nScore) / nScore2;

			//�i���o�[�N���X�̐���
			m_apNumber[nCntRanking][nCntNum] = CNumber::Create(nAnswer, CNumber::NUMBER_TYPE_001,	//�\�����鐔���Ǝ��
				D3DXVECTOR3((SCREEN_WIDTH / 2) - (RANKING_NUM_SIZE_X * NUM_LENGTH) / 2 //X���̊�ʒu
					+ ((nCntNum * RANKING_NUM_SIZE_X) + (RANKING_NUM_SIZE_X / 2)),		//�e����x���̈ʒu
					(SCREEN_HEIGHT / 2) - (RANKING_NUM_SIZE_Y * RANKING_NUM)/2//Y���̊�ʒu
					+ ((nCntRanking * RANKING_NUM_SIZE_Y) + (RANKING_NUM_SIZE_X / 2)), //�e�����L���O��y���̈ʒu
					0.0f),	//z�̈ʒu
				RANKING_NUM_SIZE, D3DXCOLOR(0, 0, 0, 255));	//�T�C�Y�ƐF
		}
	}

	for (int nCount = 0, fIndex = 0; nCount < SCORE_MAX_NUM; nCount++, fIndex++)
	{
			//�e���̒l�����߂�
			int nScore = (int)powf(fRadix, SCORE_MAX_NUM - fIndex);
			int nScore2 = (int)powf(fRadix, SCORE_MAX_NUM - fIndex - 1);
			int nAnswer = (m_nScore % nScore) / nScore2;

			m_apScoreNum[nCount] = CNumber::Create(nAnswer, CNumber::NUMBER_TYPE_001,	//�\�����鐔���Ǝ��
				D3DXVECTOR3(960.0f + ((nCount * RANKING_NUM_SIZE_X/2) + (RANKING_NUM_SIZE_X / 2)),		//�e����x���̈ʒu
					670.0f, //�ey���̈ʒu
					0.0f),	//z�̈ʒu
				RANKING_NUM_SIZE/1.5f, D3DXCOLOR(0, 0, 0, 255));	//�T�C�Y�ƐF
			
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
