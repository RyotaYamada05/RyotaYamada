//=============================================================================
//
// �L�����N�^�[���� [character.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene2d.h"

//=============================================================================
//�L�����N�^�[�̏��
//=============================================================================
typedef enum
{
	CHARACTER_STATE_NORMAL = 0,	//�ʏ���
	CHARACTER_STATE_DAMAGE,		//��e���
	CHARACTER_STATE_DIED,		//���S���
	CHARACTER_STATE_MAX,
}CHARACTER_STATE;

//=========================================================================
//�}�N����`
//=========================================================================
#define STATE_NORMAL(state)(((CHARACTER_STATE)(state)) == CHARACTER_STATE_NORMAL)	//��Ԃ�CHARACTER_STATE_NORMAL���m�F����}�N��
#define STATE_DAMAGE(state)(((CHARACTER_STATE)(state)) == CHARACTER_STATE_DAMAGE)	//��Ԃ�CHARACTER_STATE_DAMAGE���m�F����}�N��
#define STATE_DEATH(state)(((CHARACTER_STATE)(state)) == CHARACTER_STATE_DIED)		//��Ԃ�CHARACTER_STATE_DIED���m�F����}�N��

//=============================================================================
//�L�����N�^�[�N���X
//=============================================================================
class CCharacter :public CScene2D
{
public:
	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CCharacter(int nPriority);
	~CCharacter();

protected:
	CHARACTER_STATE CheckState(void);
	void SetState(CHARACTER_STATE state);
	CHARACTER_STATE GetState(void);
	virtual void DiedProcess(void) = 0;
	void InitLife(int nDefaultLife);
	int AddLife(const int nAddValue);
	int SubLife(const int nSubValue);
	void SetLife(const int nLife);
	void SetMaxLife(const int nMaxLife);

public:
	int GetLife(void);
	int GetMaxLife(void);

	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
private:
	CHARACTER_STATE m_state;	//���
	int m_nLife;				//�̗�
	int m_nMaxLife;				//�ő�̗�
	int m_nStateCounter;		//��ԃJ�E���^�[
};
#endif