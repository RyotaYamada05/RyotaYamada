//=============================================================================
//
// ���C������ [scene.h]
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
//�}�N����`
//=============================================================================
#define MAX_OBJ_NUM 2000	//�ő吔

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
	static CScene *GetScene(const int nPriority, const int nCntObjNum);

protected:
	void Release(void);

private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static CScene *m_apScene[PRIORITY_MAX][MAX_OBJ_NUM];	//�I�u�W�F�N�g�N���X�̃|�C���^�z��
	static int m_nNumAll;	//����
	int m_nID;	//�i���o�[�̕ۑ�
	int m_nPriority;	//�D�揇��
	OBJTYPE m_ObjType;	//�I�u�W�F�N�g�̎��
};

#endif