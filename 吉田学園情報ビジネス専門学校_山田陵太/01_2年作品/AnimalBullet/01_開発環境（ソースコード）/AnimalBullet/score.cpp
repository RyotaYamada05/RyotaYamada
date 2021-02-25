//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "score.h"
#include "number.h"
#include "scene2d.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
int CScore::m_nScore = 0;

//=============================================================================
//�X�R�A�N���X�̃R���X�g���N�^
//=============================================================================
CScore::CScore(int nPriority) :CScene(nPriority)
{
	//�e�����o�ϐ��̃N���A
	memset(m_apNumber, 0, sizeof(m_apNumber));
}

//=============================================================================
//�X�R�A�N���X�̃f�X�g���N�^
//=============================================================================
CScore::~CScore()
{
}

//=============================================================================
//�X�R�A�N���X�̃N���G�C�g����
//=============================================================================
CScore * CScore::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�X�R�A�N���X�̃|�C���^�ϐ�
	CScore *pScore = NULL;

	//�������̊m��
	pScore = new CScore;

	//���������m�ۂł��Ă�����
	if (pScore != NULL)
	{
		//�����������Ăяo��
		pScore->Init(pos, size);
	}
	//�������m�ۂɎ��s������
	else
	{
		return NULL;
	}

	return pScore;
}

//=============================================================================
//�X�R�A�N���X�̏���������
//=============================================================================
HRESULT CScore::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_nScore = 0;

	for (int nCount = 0; nCount < SCORE_MAX_NUM; nCount++)
	{
		//�i���o�[�N���X��SCORE_MAX_NUM������
		m_apNumber[nCount] = CNumber::Create(0, CNumber::NUMBER_TYPE_000,	//�\�����鐔���Ǝ��
			D3DXVECTOR3((SCREEN_WIDTH - NUMBER_SIZE_X * SCORE_MAX_NUM) + ((nCount * NUMBER_SIZE_X)+ (NUMBER_SIZE_X / 2)),		//x���̈ʒu
				(NUMBER_SIZE_Y / 2), //y�̈ʒu
				0.0f),	//z�̈ʒu
			NUMBER_SIZE, COLOR_CYAN);	//�T�C�Y�ƐF
	}
	return S_OK;
}

//=============================================================================
//�X�R�A�N���X�̏I������
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCount = 0; nCount < SCORE_MAX_NUM; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			//�i���o�[�N���X�̏I�������Ăяo��
			m_apNumber[nCount]->Uninit();

			//�������̍폜
			delete m_apNumber[nCount];

			//�������̃N���A
			m_apNumber[nCount] = NULL;
		}
	}

	//�I�u�W�F�N�g�̔j��
	SetDeathFlag();
}

//=============================================================================
//�X�R�A�N���X�̍X�V����
//=============================================================================
void CScore::Update(void)
{
}

//=============================================================================
//�X�R�A�N���X�̕`�揈��
//=============================================================================
void CScore::Draw(void)
{
	for (int nCount = 0; nCount < SCORE_MAX_NUM; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			//�i���o�[�N���X�̕`�揈���Ăяo��
			m_apNumber[nCount]->Draw();
		}
	}
}

//=============================================================================
//�X�R�A�N���X�̃X�R�A���Z����
//=============================================================================
void CScore::AddScore(const int nScore)
{
	//�X�R�A�̉��Z
	m_nScore += nScore;
	float fIndex;  //�w���@(�X�R�A�\���p)
	float fRadix = 10.0f; //��@(�X�R�A�\���p)

	for (fIndex = 0; fIndex < SCORE_MAX_NUM; fIndex++)
	{
		if (m_apNumber[(int)fIndex] != NULL)
		{
			//�e���̒l�����߂�
			int nScore = (int)powf(fRadix, SCORE_MAX_NUM - fIndex);
			int nScore2 = (int)powf(fRadix, SCORE_MAX_NUM - fIndex - 1);
			int nAnswer = (m_nScore % nScore) / nScore2;

			//�i���o�[�N���X�̍X�V�����Ăяo��
			m_apNumber[(int)fIndex]->Update();

			//�����̐ݒ�
			m_apNumber[(int)fIndex]->SetNumber(nAnswer);
		}
	}
}

//=============================================================================
//�X�R�A�N���X�̃X�R�A�擾����
//=============================================================================
int CScore::GetScore(void) 
{
	return m_nScore;
}
