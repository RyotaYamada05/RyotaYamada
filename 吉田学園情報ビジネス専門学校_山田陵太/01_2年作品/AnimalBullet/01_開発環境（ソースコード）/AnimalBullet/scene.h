//=============================================================================
//
// �I�u�W�F�N�g���� [scene.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//�I�u�W�F�N�g�N���X
//=============================================================================
class CScene
{
public:
	//=========================================================================
	//�I�u�W�F�N�g�^�C�v�̗񋓌^
	//=========================================================================
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,	//�v���C���[�^�C�v
		OBJTYPE_BULLET,	//�o���b�g�^�C�v
		OBJTYPE_ENEMY,	//�G�l�~�[�^�C�v
		OBJTYPE_BOSS,
		OBJTYPE_EXPLOSION,	//�����^�C�v
		OBJTYPE_BOME,	//�{���^�C�v
		OBJTYPE_ITEM,	//�A�C�e���^�C�v
		OBJTYPE_SHIELD,	//�V�[���h�^�C�v
		OBJTYPE_MAX	//�I�u�W�F�N�g�^�C�v�̍ő吔
	}OBJTYPE;	//�I�u�W�F�N�g�^�C�v

	//=========================================================================
	//�`��D�揇�ʂ̗񋓌^
	//=========================================================================
	typedef enum
	{
		PRIORITY_BG = 0, 
		PRIORITY_EFFECT,	//�G�t�F�N�g
		PRIORITY_BULEET,	//�o���b�g
		PRIORITY_DEFAULT,	//������
		PRIORITY_ENEMY,	//�G�l�~�[
		PRIORITY_PLAYER,	//�v���C���[
		PRIORITY_EXPLOSION,	//����
		PRIORITY_UI,	//UI
		PRIORITY_TOPUI,	//TOPUI
		PRIORITY_FADE,
		PRIORITY_MAX	//�D�揇�ʂ̍ő吔
	}PRIORITY;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CScene(int nPriority = PRIORITY_DEFAULT);
	virtual ~CScene();
	static void ReleaseAll(void);
	static void AllUpdate(void);
	static void AllDraw(void);

	virtual HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	void SetObjType(const OBJTYPE objtype);
	OBJTYPE GetObjType(void)const;
	CScene *GetNext(void);
	static CScene *GetTop(int nNum);
	static void SetPause(bool bPause);

protected:
	void SetDeathFlag(void);

private:
	void ReConnectList(void);
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static CScene *m_pTop[PRIORITY_MAX];	//�擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CScene *m_pCur[PRIORITY_MAX];	//���݂̃I�u�W�F�N�g�ւ̃|�C���^
	static bool m_bPause;	//�|�[�Y���Ă��邩�ǂ���
	CScene *m_pPrev;	//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene *m_pNext;	//���̃I�u�W�F�N�g�ւ̃|�C���^
	int m_nPriority;	//�`��̗D�揇��
	bool m_bDeath;	//���S�t���O
	OBJTYPE m_ObjType;	//�I�u�W�F�N�g�̎��
};

#endif