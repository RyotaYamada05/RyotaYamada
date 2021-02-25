//=============================================================================
//
// �����L���O���� [ranking.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "ranking.h"
#include "score.h"
#include "number.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define RANKING_NUM_SIZE_X 70.0f	//�����L���O��X���̃T�C�Y
#define RANKING_NUM_SIZE_Y 70.0f	//�����L���O��Y���̃T�C�Y
#define RANKING_NUM_SIZE D3DXVECTOR3(RANKING_NUM_SIZE_X,RANKING_NUM_SIZE_Y,0.0f)//�����L���O�̃T�C�Y
#define RANKING_DATA_FILE "data/Resource/ranking.txt"	//�����L���O�t�@�C���̃p�X

//=============================================================================
//�����L���O�N���X�̃R���X�g���N�^
//=============================================================================
CRanking::CRanking()
{
	//�e�����o�ϐ��̃N���A
	m_nRank = 0;
	m_nScore = 0;
	memset(m_apNumber, 0, sizeof(m_apNumber));
	memset(m_nRanking, 0, sizeof(m_nRanking));
	memset(m_apRank, 0, sizeof(m_apRank));
	memset(m_apScoreNum, 0, sizeof(m_apScoreNum));
}

//=============================================================================
//�����L���O�N���X�̃f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{
}

//=============================================================================
//�����L���O�N���X�̃N���G�C�g����
//=============================================================================
CRanking * CRanking::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CRanking *pRanking = NULL;

	//�C���X�^���X����
	pRanking = new CRanking;

	//���������m�ۂł��Ă�����
	if (pRanking != NULL)
	{
		//�����������Ăяo��
		pRanking->Init(pos,size);
	}
	//���s������
	else
	{
		return NULL;
	}

	return pRanking;
}

//=============================================================================
//�����L���O�N���X�̏���������
//=============================================================================
HRESULT CRanking::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�����L���O�̐ݒ�
	SetRanking(CScore::GetScore());

	//�i���o�[�̐ݒ�
	SetNumber();
	return E_NOTIMPL;
}

//=============================================================================
//�����L���O�N���X�̏I������
//=============================================================================
void CRanking::Uninit(void)
{
	for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
	{
		//���ʐ����̏I��
		if (m_apRank[nCntRanking] != NULL)
		{
			//�I������
			m_apRank[nCntRanking]->Uninit();
			
			//�������̍폜
			delete m_apRank[nCntRanking];

			//�������̃N���A
			m_apRank[nCntRanking] = NULL;

		}
		for (int nCntNum = 0; nCntNum < NUM_LENGTH; nCntNum++)
		{
			//�i���o�[�̏I��
			if (m_apNumber[nCntRanking][nCntNum] != NULL)
			{
				//�I������
				m_apNumber[nCntRanking][nCntNum]->Uninit();

				//�������̍폜
				delete m_apNumber[nCntRanking][nCntNum];

				//�������̃N���A
				m_apNumber[nCntRanking][nCntNum] = NULL;
			}

			//�X�R�A�i���o�[�̏I��
			if (m_apScoreNum[nCntNum] != NULL)
			{
				//�I������
				m_apScoreNum[nCntNum]->Uninit();

				//�������̍폜
				delete m_apScoreNum[nCntNum];

				//�������̃N���A
				m_apScoreNum[nCntNum] = NULL;
			}
		}
	}
}

//=============================================================================
//�����L���O�N���X�̍X�V����
//=============================================================================
void CRanking::Update(void)
{
}

//=============================================================================
//�����L���O�N���X�̕`�揈��
//=============================================================================
void CRanking::Draw(void)
{
	for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
	{
		//���ʐ����̕`��
		if (m_apRank[nCntRanking] != NULL)
		{
			m_apRank[nCntRanking]->Draw();
		}
		for (int nCntNum = 0; nCntNum < NUM_LENGTH; nCntNum++)
		{
			//�i���o�[�̕`��
			if (m_apNumber[nCntRanking][nCntNum] != NULL)
			{
				m_apNumber[nCntRanking][nCntNum]->Draw();
			}

			//�X�R�A�i���o�[�̕`��
			if (m_apScoreNum[nCntNum] != NULL)
			{
				m_apScoreNum[nCntNum]->Draw();
			}
		}
	}
}

//=============================================================================
//�����L���O�N���X�̃t�@�C���ǂݍ���
//=============================================================================
void CRanking::ReadFile(void)
{
	//�t�@�C���|�C���^
	FILE *pFile;

	//�ǂݍ��݃��[�h�Ńt�@�C�����J��
	pFile = fopen(RANKING_DATA_FILE, "r");

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

//=============================================================================
//�����L���O�N���X�̃t�@�C����������
//=============================================================================
void CRanking::WriteFile(void)
{
	//�t�@�C���|�C���^
	FILE *pFile;

	//�������݃��[�h�ŊJ��
	pFile = fopen(RANKING_DATA_FILE, "w");

	if (pFile != NULL)
	{
		//10�l���̃f�[�^����������
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

//=============================================================================
//�����L���O�N���X�̃����L���O�̐ݒ菈��
//=============================================================================
void CRanking::SetRanking(int nScore)
{
	int nCntRanking;

	//�Q�[���I�����_�̃X�R�A�̐ݒ�
	m_nScore = nScore;

	//�����L���O�f�[�^��ǂݍ���
	ReadFile();

	for (nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
	{
		if (m_nScore > m_nRanking[nCntRanking])
		{
			//�����L���O�����ɂ��炷
			for (int nCntRankMove = RANKING_NUM - 1; nCntRankMove > nCntRanking; nCntRankMove--)
			{
				m_nRanking[nCntRankMove] = m_nRanking[nCntRankMove - 1];
			}
			m_nRanking[nCntRanking] = m_nScore;

			//���ʂ���������
			WriteFile();

			//�X�V���ꂽ���ʂ�ݒ肷��
			m_nRank = nCntRanking;
			return;
		}
	}
	//�X�V����Ȃ������ꍇ�A-1�ʂƂ��Đݒ肷��
	m_nRank = -1;

	return;
}

//=============================================================================
//�����L���O�N���X�̃i���o�[�ݒ菈��
//=============================================================================
void CRanking::SetNumber(void)
{
	float fIndex;  //�w���@(�X�R�A�\���p)
	float fRadix = 10.0f; //��@(�X�R�A�\���p)

	//�����L���O��\������
	for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++)
	{
		fIndex = 0;

		//���ʗp�i���o�[�̐���
		m_apRank[nCntRanking] = CNumber::Create(nCntRanking+1, CNumber::NUMBER_TYPE_001,//�\�����鐔���Ǝ��
			D3DXVECTOR3((SCREEN_WIDTH / 2) - (RANKING_NUM_SIZE_X*RANKING_NUM),	//X���̈ʒu
				(SCREEN_HEIGHT / 2) - (RANKING_NUM_SIZE_Y * RANKING_NUM) / 2//Y���̊�ʒu
				+ ((nCntRanking * RANKING_NUM_SIZE_Y) + (RANKING_NUM_SIZE_X / 2)), //�e�����L���O��y���̈ʒu
				0.0f),	//z�̈ʒu
			RANKING_NUM_SIZE, D3DXCOLOR(0, 0, 0, 255));	//�T�C�Y�ƐF

		//8�����N���G�C�g���邽�߂�for��
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

	//�Q�[���I�����̃X�R�A�p�̐����pfor��
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

	//�����N�C�����Ă�����
	if (m_nRank != -1)
	{
		//�e���̃X�R�A�̐F��ς���
		for (int nCntNum = 0; nCntNum < NUM_LENGTH; nCntNum++)
		{
			if (m_apNumber[m_nRank][nCntNum] != NULL)
			{
				m_apNumber[m_nRank][nCntNum]->SetColr(D3DXCOLOR(255, 0, 0, 255));
			}
		}
	}
}