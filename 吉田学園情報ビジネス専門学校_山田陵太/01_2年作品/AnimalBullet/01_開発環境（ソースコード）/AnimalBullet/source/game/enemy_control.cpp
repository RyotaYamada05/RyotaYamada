//=============================================================================
//
// �G�l�~�[�R���g���[������ [enemy_control.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//�x���}���}�N��
#include "enemy_control.h"
#include <stdio.h>

//=============================================================================
//�}�N����`
//=============================================================================
#define ENEMY_NUM 100	//�G�̍ő吔
#define ENEMY_FILE_PATH "data/Text/enemydata.txt"	//�G���̃t�@�C���p�X
#define ENEMY_TEST_FILE_PATH "data/Text/enemy_Test.txt"	//�G���̃t�@�C���p�X
//=============================================================================
//�G�l�~�[�R���g���[���N���X�̃R���X�g���N�^
//=============================================================================
CEnemyControl::CEnemyControl()
{
	m_pEnemyData = NULL;
	m_nSpoonTimer = 0;
	m_nIndex = 0;
	m_nMaxEnemyNum = 0;
}

//=============================================================================
//�G�l�~�[�R���g���[���N���X�̃f�X�g���N�^
//=============================================================================
CEnemyControl::~CEnemyControl()
{
}

//=============================================================================
//�G�l�~�[�R���g���[���N���X�̏���������
//=============================================================================
HRESULT CEnemyControl::Init(void)
{
	//�G���̓ǂݍ���
	if (FAILED(ReadFile())) 
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//�G�l�~�[�R���g���[���N���X�̏I������
//=============================================================================
void CEnemyControl::Uninit(void)
{
	if (m_pEnemyData)
	{
		//�������̍폜
		delete[] m_pEnemyData;

		//�������̃N���A
		m_pEnemyData = NULL;
	}
}

//=============================================================================
//�G�l�~�[�R���g���[���N���X�̍X�V����
//=============================================================================
void CEnemyControl::Update(void)
{
	if (m_pEnemyData)
	{
		//���ݐ����G�̑�����菭�Ȃ���
		if (m_nIndex < m_nMaxEnemyNum)
		{
			if (m_nSpoonTimer == m_pEnemyData[m_nIndex].nSpoonTime)
			{
				//�G�l�~�[�𐶐�
				CEnemy::Create(
					m_pEnemyData[m_nIndex].pos,
					m_pEnemyData[m_nIndex].size,
					m_pEnemyData[m_nIndex].enemyType,
					m_pEnemyData[m_nIndex].nLife,
					m_pEnemyData[m_nIndex].nScore,
					m_pEnemyData[m_nIndex].action,
					m_pEnemyData[m_nIndex].deathAction);

				//�^�C�}�[���N���A
				m_nSpoonTimer = 0;

				//�C���f�b�N�X��1�i�߂�
				m_nIndex++;
				return;
			}
			m_nSpoonTimer++;
		}
	}
}

//=============================================================================
//�G�l�~�[�R���g���[���N���X�̃t�@�C���ǂݍ��ݏ���
//=============================================================================
HRESULT CEnemyControl::ReadFile(void)
{
	FILE *pFile;	//�t�@�C���|�C���^
	char aHeadData[1024];
	char aModeName[1024];	//

	m_pEnemyData = new ENEMY_DATA[ENEMY_NUM];

	if (m_pEnemyData)
	{
		//�ǂݍ��݃��[�h�Ńt�@�C�����J��
		pFile = fopen(ENEMY_FILE_PATH, "r");

		if (pFile)
		{
			do
			{
				//���ǂݍ���Ń��[�h���𒊏o
				fgets(aHeadData, sizeof(aHeadData), pFile);
				sscanf(aHeadData, "%s", aModeName);

				if (strcmp(aModeName, "SET_ENEMY") == 0)
				{
					while (strcmp(aModeName, "END_ENEMY") != 0)
					{
						//���ǂݍ���Ń��[�h���𒊏o
						fgets(aHeadData, sizeof(aHeadData), pFile);
						sscanf(aHeadData, "%s", aModeName);

						if (strcmp(aModeName, "SPOON_TIME") == 0)
						{
							//�����^�C���̏����擾
							sscanf(aHeadData, "%*s %*s %d", 
								&m_pEnemyData[m_nMaxEnemyNum].nSpoonTime);
						}
						if (strcmp(aModeName, "POS") == 0)
						{
							//�ʒu�����擾
							sscanf(aHeadData, "%*s %*s %f %f %f",
								&m_pEnemyData[m_nMaxEnemyNum].pos.x,
								&m_pEnemyData[m_nMaxEnemyNum].pos.y,
								&m_pEnemyData[m_nMaxEnemyNum].pos.z);
						}
						if (strcmp(aModeName, "SIZE") == 0)
						{
							//�ʒu�����擾
							sscanf(aHeadData, "%*s %*s %f %f %f",
								&m_pEnemyData[m_nMaxEnemyNum].size.x,
								&m_pEnemyData[m_nMaxEnemyNum].size.y,
								&m_pEnemyData[m_nMaxEnemyNum].size.z);
						}
						if (strcmp(aModeName, "ENEMY_TYPE") == 0)
						{
							//�ʒu�����擾
							sscanf(aHeadData, "%*s %*s %d",
								&m_pEnemyData[m_nMaxEnemyNum].enemyType);
						}
						if (strcmp(aModeName, "LIFE") == 0)
						{
							//�ʒu�����擾
							sscanf(aHeadData, "%*s %*s %d",
								&m_pEnemyData[m_nMaxEnemyNum].nLife);
						}
						if (strcmp(aModeName, "SCORE") == 0)
						{
							//�ʒu�����擾
							sscanf(aHeadData, "%*s %*s %d",
								&m_pEnemyData[m_nMaxEnemyNum].nScore);
						}
						if (strcmp(aModeName, "ACTION") == 0)
						{
							//�ʒu�����擾
							sscanf(aHeadData, "%*s %*s %d",
								&m_pEnemyData[m_nMaxEnemyNum].action);
						}
						if (strcmp(aModeName, "DEATH_ACTION") == 0)
						{
							//�ʒu�����擾
							sscanf(aHeadData, "%*s %*s %d",
								&m_pEnemyData[m_nMaxEnemyNum].deathAction);
						}
					}
					//�G�l�~�[������1���₷
					m_nMaxEnemyNum++;
				}

			} while (strcmp(aModeName, "END_SCRIPT") != 0);
		}
		else
		{
			return E_FAIL;
		}

		//�t�@�C�������
		fclose(pFile);
	}
	return S_OK;
}