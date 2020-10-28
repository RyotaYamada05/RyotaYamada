//=============================================================================
//
// ���C������ [player.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene2d.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define PLAYER_SIZE_X 45.0f	//�v���C���[��X�T�C�Y
#define PLAYER_SIZE_Y 45.0f	//�v���C���[��Y�T�C�Y
#define PLAYER_SIZE D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, 0.0f)	//�v���C���[�̃T�C�Y


//=============================================================================
//�O���錾
//=============================================================================
class CLife;
class CInputKeyboard;
class CInputJoyStick;
//=============================================================================
//�v���C���[�N���X
//=============================================================================
class CPlayer : public CScene2D
{
public:
	//=========================================================================
	//�v���C���[�̏�Ԕ���p�̗񋓌^��`
	//=========================================================================
	typedef enum
	{
		PLAYER_STATE_NORMAL = 0,	//�ʏ���
		PLAYER_STATE_DAMAGE,	//��e���
		PLAYER_STATE_DIED,	//���S���
	}PLAYER_STATE;

	//=========================================================================
	//�v���C���[�̓����^�C�v�̗񋓌^��`
	//=========================================================================
	typedef enum
	{
		ANIMAL_TYPE_NONE = 0,
		ANIMAL_TYPE_CHEETAH,	//�`�[�^�[
		ANIMAL_TYPE_GORILLA,	//�S����
		ANIMAL_TYPE_TURTLE,	//�J��
	}ANIMAL_TYPE;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CPlayer(int nPriority = CScene::PRIORITY_PLAYER);
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Moving(void);
	void ChangeAnimal(void);
	void TypebySkil(void);
	void ShotBullet(void);
	void Damage(const int nAttack);
	void Recovery(const int nRecovery);
	bool DecisionState(void);
	D3DXVECTOR3 GetPos(void)const;
	static ANIMAL_TYPE GetAnimal(void);
	
private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_move;	//�ړ���
	PLAYER_STATE m_State;	//�v���C���[�̏��
	CInputKeyboard *m_pKeyBoard;
	CInputJoyStick *m_pJoyStick;
	static ANIMAL_TYPE m_Animal;	//�����̃^�C�v
	bool m_bSkill;
	int m_nLife;	//�v���C���[�̗̑�
	int m_nSkillCounter;	//�X�L���J�E���^
	int m_nStateCounter;	//�_���[�W�J�E���^�[
	int m_nShotCounter;	//���˃J�E���^�[
	int m_nDefense;	//�h���
};

#endif