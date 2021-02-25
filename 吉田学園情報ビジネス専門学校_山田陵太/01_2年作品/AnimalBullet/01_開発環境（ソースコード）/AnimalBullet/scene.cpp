//=============================================================================
//
// �I�u�W�F�N�g���� [scene.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "scene.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
CScene * CScene::m_pTop[PRIORITY_MAX] = {};
CScene * CScene::m_pCur[PRIORITY_MAX] = {};
bool CScene::m_bPause = false;

//=============================================================================
//�I�u�W�F�N�g�N���X�̃R���X�g���N�^
//=============================================================================
CScene::CScene(int nPriority)
{
	//�v���C�I���e�B�̐ݒ�
	m_nPriority = nPriority;

	//���S�t���O��false�ɐݒ肷��
	m_bDeath = false;

	//�擪�I�u�W�F�N�g���m�ۂ���Ă��Ȃ��Ƃ�
	if (!m_pTop[m_nPriority])
	{
		//�擪�I�u�W�F�N�g�Ɏ��g�̃|�C���^��ݒ肷��
		m_pTop[m_nPriority] = this;

		//�擪�̂��߁A�O����NULL�ŃN���A����
		m_pPrev = NULL;
	}

	//���݃I�u�W�F�N�g���m�ۂ���Ă��Ȃ������ꍇ
	if (!m_pCur[m_nPriority])
	{
		//���݃I�u�W�F�N�g�֎��g�̃|�C���^��ݒ�
		m_pCur[m_nPriority] = this;
	}

	//���݃I�u�W�F�N�g�̎��̃I�u�W�F�N�g���Ɏ����̏�������
	m_pCur[m_nPriority]->m_pNext = this;

	//���݃I�u�W�F�N�g�������������ꍇ
	if (m_pCur[m_nPriority] == this)
	{
		//�O����NULL������
		m_pPrev = NULL;
	}
	else
	{
		//�O���Ɍ��݃I�u�W�F�N�g������
		m_pPrev = m_pCur[m_nPriority];
	}

	//���݃I�u�W�F�N�g�Ɏ��g�̃|�C���^���ɏ㏑������
	m_pCur[m_nPriority] = this;

	//���g�̎������N���A����
	m_pNext = NULL;

	m_ObjType = OBJTYPE_NONE;
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
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�擪�I�u�W�F�N�g�̏����擾
		CScene *pScene = m_pTop[nCntPriority];

		while (pScene)
		{
			//�����̈ꎞ�ۑ�
			CScene *pNext = pScene->m_pNext;

			//�I�������Ăяo��
			pScene->Uninit();

			//���X�g�̍Đڑ�
			pScene->ReConnectList();

			//�I�u�W�F�N�g�������̂��̂ֈڍs����
			pScene = pNext;
		}
	}
}

//=============================================================================
//�S�Ă̍X�V����
//=============================================================================
void CScene::AllUpdate(void)
{
	//�X�V����
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�擪�I�u�W�F�N�g�̏����擾
		CScene *pScene = m_pTop[nCntPriority];

		while (pScene)
		{
			//�����̈ꎞ�ۑ�
			CScene *pNext = pScene->m_pNext;

			//m_bPause��false�̎�
			if (!m_bPause)
			{
				if (!pScene->m_bDeath)
				{
					//�X�V�����Ăяo��
					pScene->Update();
				}
			}
			//�I�u�W�F�N�g�������̂��̂ֈڍs
			pScene = pNext;
		}
	}

	//���S�t���O�������Ă�����̂��܂Ƃ߂č폜����
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�擪�I�u�W�F�N�g�̏����擾
		CScene *pScene = m_pTop[nCntPriority];

		while (pScene)
		{
			//�����̈ꎞ�ۑ�
			CScene *pNext = pScene->m_pNext;

			if (pScene->m_bDeath)
			{
				//���X�g�̍Đڑ�
				pScene->ReConnectList();
			}

			//�I�u�W�F�N�g�������̂��̂ֈڍs
			pScene = pNext;
		}
	}
}

//=============================================================================
//�S�Ă̕`�揈��
//=============================================================================
void CScene::AllDraw(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�擪�I�u�W�F�N�g�̏����擾
		CScene *pScene = m_pTop[nCntPriority];

		while (pScene)
		{
			if (!pScene->m_bDeath)
			{
				//�`�揈���̌Ăяo��
				pScene->Draw();
			}
			//�I�u�W�F�N�g�������̂��̂ֈڍs����
			pScene = pScene->m_pNext;
		}
	}
}

//=============================================================================
//�폜����
//=============================================================================
void CScene::SetDeathFlag(void)
{
	//���S�t���O��true�ɂ���
	m_bDeath = true;
}

//=============================================================================
//���X�g�̍Đڑ�����
//=============================================================================
void CScene::ReConnectList(void)
{
	//���g���擪�I�u�W�F�N�g�������ꍇ
	if (this == m_pTop[m_nPriority])
	{
		//���g�������̃I�u�W�F�N�g������擪�Ɏw�肷��
		m_pTop[m_nPriority] = m_pNext;
	}

	//���g�����݃I�u�W�F�N�g�������ꍇ
	if (this == m_pCur[m_nPriority])
	{
		//���g�����O�̃I�u�W�F�N�g�������݂Ɏw�肷��
		m_pCur[m_nPriority] = m_pPrev;
	}

	//�O���������Ă���Ȃ�
	if (m_pPrev)
	{
		//�O�̃I�u�W�F�N�g�̎����Ɏ��g�̎�����n��
		m_pPrev->m_pNext = m_pNext;
	}

	//�����������Ă���Ȃ�
	if (m_pNext)
	{
		//���̃I�u�W�F�N�g�̑O���Ɏ��g�̑O����n��
		m_pNext->m_pPrev = m_pPrev;
	}

	//�������̍폜
	delete this;
}

//=============================================================================
//�I�u�W�F�N�g�^�C�v�ݒ菈��
//=============================================================================
void CScene::SetObjType(const OBJTYPE objtype)
{
	m_ObjType = objtype;
}

//=============================================================================
//�I�u�W�F�N�g�^�C�v���擾����
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)const
{
	return m_ObjType;
}

//=============================================================================
//�����̎擾����
//=============================================================================
CScene * CScene::GetNext(void)
{
	return m_pNext;
}

//=============================================================================
//�擪���̎擾����
//=============================================================================
CScene * CScene::GetTop(int nNum)
{
	return m_pTop[nNum];
}

//=============================================================================
//�|�[�Y��Ԃ̐ݒ菈��
//=============================================================================
void CScene::SetPause(bool bPause)
{
	m_bPause = bPause;
}
