//=============================================================================
//
// �v���C���[���� [player.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "character.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define CHEETAH_SIZE_X 80.0f	//�`�[�^�[��X�T�C�Y
#define CHEETAH_SIZE_Y 55.0f	//�`�[�^�[��Y�T�C�Y
#define CHEETAH_SIZE D3DXVECTOR3(CHEETAH_SIZE_X, CHEETAH_SIZE_Y, 0.0f)	//�`�[�^�[�̃T�C�Y
#define GORILLA_SIZE_X 80.0f	//�S������X�T�C�Y
#define GORILLA_SIZE_Y 80.0f	//�S������Y�T�C�Y
#define GORILLA_SIZE D3DXVECTOR3(GORILLA_SIZE_X, GORILLA_SIZE_Y, 0.0f)	//�S�����̃T�C�Y
#define TURTLE_SIZE_X 60.0f	//�J����X�T�C�Y
#define TURTLE_SIZE_Y 45.0f	//�J����Y�T�C�Y
#define TURTLE_SIZE D3DXVECTOR3(TURTLE_SIZE_X, TURTLE_SIZE_Y, 0.0f)	//�J���̃T�C�Y
#define MAX_SKILL 100	//�X�L���̍ő�l
//=============================================================================
//�O���錾
//=============================================================================
class CLifeber;
class CSkillber;
class CInputKeyboard;
class CInputJoyStick;

//=============================================================================
//�v���C���[�N���X
//=============================================================================
class CPlayer : public CCharacter
{
public:
	//=========================================================================
	//�v���C���[�̓����^�C�v�̗񋓌^��`
	//=========================================================================
	typedef enum
	{
		ANIMAL_TYPE_NONE = -1,
		ANIMAL_TYPE_CHEETAH,	//�`�[�^�[
		ANIMAL_TYPE_GORILLA,	//�S����
		ANIMAL_TYPE_TURTLE,	//�J��
		ANIMAL_TYPE_MAX
	}ANIMAL_TYPE;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CPlayer(int nPriority = CScene::PRIORITY_PLAYER);
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Damage(const int nAttack);
	void Recovery(const int nRecovery);
	int GetSkillPoint(void)const;
	void AddSkillPoint(const int nAddSkill);
	static ANIMAL_TYPE GetAnimal(void);

private:
	void Moving(void);
	void ChangeAnimal(void);
	void TypebySkil(void);
	void ShotBullet(void);
	void DiedProcess(void);
#ifdef _DEBUG
	void DebugPrint(void);
#endif

	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture[ANIMAL_TYPE_MAX];	//�e�N�X�`���ւ̃|�C���^
	static ANIMAL_TYPE m_Animal;	//�����̃^�C�v
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_move;	//�ړ���
	CInputKeyboard *m_pKeyBoard;	//�L�[�{�[�h�N���X�̃|�C���^�ϐ�
	CInputJoyStick *m_pJoyStick;	//�W���C�X�e�B�b�N�N���X�̃|�C���^�ϐ�
	bool m_bSkill;	//�X�L�����g���Ă��邩�ǂ���
	int m_nSkillCounter;	//�X�L���J�E���^
	int m_nStateCounter;	//�_���[�W�J�E���^�[
	int m_nShotCounter;	//���˃J�E���^�[
	int m_nDefense;	//�h���
	int m_nSkillPoint;	//�X�L���|�C���g
};

#endif