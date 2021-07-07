//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _ENEMY_H
#define _ENEMY_H

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "character.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define ENEMY_SIZE_X 70.0f	//�G�l�~�[��X���̑傫��
#define ENEMY_SIZE_Y 70.0f	//�G�l�~�[��Y���̑傫��
#define ENEMY_SIZE D3DXVECTOR3(ENEMY_SIZE_X, ENEMY_SIZE_Y, 0.0f)	//�G�l�~�[�̑傫��
#define ENEMY_MOVE_X -2.0f	//�G�l�~�[��X���̈ړ���
#define ENEMY_MOVE_Y 2.0f	//�G�l�~�[��Y���̈ړ���
#define ENEMY_MOVE D3DXVECTOR3(ENEMY_MOVE_X, ENEMY_MOVE_Y, 0.0f)	//�G�l�~�[�̈ړ���

//=============================================================================
//�G�l�~�[�N���X
//=============================================================================
class CEnemy : public CCharacter
{
public:
	//=========================================================================
	//�G�l�~�[�̎��
	//=========================================================================
	typedef enum
	{
		ENEMY_TYPE_NO_BULLET = 0,	//�e�������Ȃ��G
		ENEMY_TYPE_NORMAL,			//���ʂ̓G
		ENEMY_TYPE_SNIPING,			//�_�������̓G
		ENEMY_TYPE_HOMING,			//�z�[�~���O�e�̓G
		ENEMY_TYPE_MIDDLEBOSS,		//���{�X
		ENEMY_TYPE_MAX
	}ENEMY_TYPE;

	//=========================================================================
	//�G�l�~�[�̎��S���̏������
	//=========================================================================
	typedef enum
	{
		DEATH_ACTION_NOEN = 0,	//���S���ɉ������Ȃ�
		DEATH_ACTION_LIFE,		//���S���ɉ񕜃A�C�e���𐶐�����
		DEATH_ACTION_SKILL,		//���S���ɃX�L���񕜃A�C�e���𐶐�����
		DEATH_ACTION_BOSS,		//���S���Ƀ{�X��J�n�̍��}���o��
	}DEATH_ACTION;

	//=========================================================================
	//�s���p�^�[��
	//=========================================================================
	typedef enum
	{
		ACTION_PATTERN_NOMRAL = 0,	//�ʏ�̍s��
		ACTION_PATTERN_MOVE_DOWN,	//���ֈړ�����,
		ACTION_PATTERN_MOVE_UP,		//��ֈړ�����
		ACTION_PATTERN_STOP,		//��~����
		ACTION_PATTERN_STOP_RETURN,	//��~��A��莞�Ԍo�߂ŋA�҂���
		ACTION_PATTERN_UP_AND_DOWN,	//�㉺�Ɉړ�����
		ACTION_PATTERN_STALKER,		//�v���C���[��ǐՂ���
		ACTION_PATTERN_MAX
	}ACTION_PATTERN;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CEnemy(int nPriority = CScene::PRIORITY_ENEMY);
	virtual ~CEnemy();

	static CEnemy *Create(
		const D3DXVECTOR3 pos, 
		const D3DXVECTOR3 size, 
		const ENEMY_TYPE type, 
		const int nLife, 
		const int nScore,
		const ACTION_PATTERN action = ACTION_PATTERN_NOMRAL, 
		DEATH_ACTION DeathAction = DEATH_ACTION_NOEN);

	static HRESULT Load(void);
	static void UnLoad(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Attack(void);
	void ActionUpdate(void);
	void Damage(const int nDamage);
	virtual void DiedProcess(void);

private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture[ENEMY_TYPE_MAX];	//�e�N�X�`���ւ̃|�C���^
	int m_nScore;	//�X�R�A
	DEATH_ACTION m_DeathAction;	//���S���̏����p�ϐ�
	int m_nActionCounter;
	ENEMY_TYPE m_type;	//�G�l�~�[�̎��
	
protected:
	D3DXVECTOR3 m_move;	//�ړ���
	ACTION_PATTERN m_action;	//�s���p�^�[��
	int m_nCounter;	//�J�E���^�[
	D3DXVECTOR3 m_pos;	//�ʒu
};
#endif