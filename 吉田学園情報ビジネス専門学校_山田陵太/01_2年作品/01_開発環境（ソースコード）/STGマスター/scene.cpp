//=============================================================================
//
// ���C������ [scene.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "scene.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
CScene *CScene::m_apScene[PRIORITY_MAX][MAX_OBJ_NUM] = {};	//�I�u�W�F�N�g�N���X�̃|�C���^�ϐ�
int CScene::m_nNumAll = 0;	//�I�u�W�F�N�g�̑����J�E���g�ϐ�
//=============================================================================
//�I�u�W�F�N�g�N���X�̃f�t�H���g�R���X�g���N�^
//=============================================================================
CScene::CScene(int nPriority)
{
	//�ő�|���S�������J��Ԃ�
	for (int nCntScene = 0; nCntScene < MAX_OBJ_NUM; nCntScene++)
	{
		//���������m�ۂ���Ă��Ȃ��Ƃ�
		if (m_apScene[nPriority][nCntScene] == NULL)
		{
			//�|�C���^���̎擾
			m_apScene[nPriority][nCntScene] = this;

			//�I�u�W�F�N�g�^�C�v�̏����ݒ�
			m_apScene[nPriority][nCntScene]->m_ObjType = CScene::OBJTYPE_NONE;

			//�쐬�����C���X�^���X�̔ԍ���ۑ�
			m_nID = nCntScene;

			//�D�揇�ʂ̕ۑ�
			m_nPriority = nPriority;

			//�����̃J�E���g�A�b�v
			m_nNumAll++;
			break;
		}
	}
}

//=============================================================================
//�I�u�W�F�N�g�N���X�̃f�t�H���g�f�X�g���N�^
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
//�S�Ă̏I������
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nPriority = 0; nPriority < PRIORITY_MAX; nPriority++)
	{
		//�ő�|���S�������J��Ԃ�
		for (int nCntScene = 0; nCntScene < MAX_OBJ_NUM; nCntScene++)
		{
			//���������m�ۂł��Ă�����
			if (m_apScene[nPriority][nCntScene] != NULL)
			{
				//�I�������Ăяo��
				m_apScene[nPriority][nCntScene]->Uninit();
			}
		}
	}
	
}

//=============================================================================
//�S�Ă̍X�V����
//=============================================================================
void CScene::AllUpdate(void)
{
	for (int nPriority = 0; nPriority < PRIORITY_MAX; nPriority++)
	{
		//�ő�|���S�������J��Ԃ�
		for (int nCntScene = 0; nCntScene < MAX_OBJ_NUM; nCntScene++)
		{
			//���������m�ۂł��Ă�����
			if (m_apScene[nPriority][nCntScene] != NULL)
			{
				//�X�V�����Ăяo��
				m_apScene[nPriority][nCntScene]->Update();
			}
		}
	}
}

//=============================================================================
//�S�Ă̕`�揈��
//=============================================================================
void CScene::AllDraw(void)
{
	for (int nPriority = 0; nPriority < PRIORITY_MAX; nPriority++)
	{
		//�ő�|���S�������J��Ԃ�
		for (int nCntScene = 0; nCntScene < MAX_OBJ_NUM; nCntScene++)
		{
			//���������m�ۂł��Ă�����
			if (m_apScene[nPriority][nCntScene] != NULL)
			{
				//�`�揈���Ăяo��
				m_apScene[nPriority][nCntScene]->Draw();
			}
		}
	}
}

//=============================================================================
//�폜����
//=============================================================================
void CScene::Release(void)
{
	//���������m�ۂł��Ă�����
	if (m_apScene[m_nID] != NULL)
	{
 		int nID = m_nID;
		int nPriority = m_nPriority;

		//���������폜
		delete m_apScene[nPriority][nID];

		//�������̃N���A
		m_apScene[nPriority][nID] = NULL;

		//���������炷
		m_nNumAll--;
	}
}

//=============================================================================
//�I�u�W�F�N�g�^�C�v�ݒ菈��
//=============================================================================
void CScene::SetObjType(const OBJTYPE objtype)
{
	//���������m�ۂł��Ă�����
	if (m_apScene[m_nPriority][m_nID] != NULL)
	{
		m_apScene[m_nPriority][m_nID]->m_ObjType = objtype;
	}
}

//=============================================================================
//�I�u�W�F�N�g�^�C�v���擾����
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)const
{
	return m_apScene[m_nPriority][m_nID]->m_ObjType;
}

//=============================================================================
//�I�u�W�F�N�g���擾����
//=============================================================================
CScene * CScene::GetScene(const int nPriority, const int nNum)
{
	return m_apScene[nPriority][nNum];
}
