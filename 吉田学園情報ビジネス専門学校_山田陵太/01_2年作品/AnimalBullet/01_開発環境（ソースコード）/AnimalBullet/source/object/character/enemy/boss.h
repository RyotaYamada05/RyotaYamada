//=============================================================================
//
// �{�X���� [boss.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "character.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define BOSS_LIFE 10	//�{�X�̗̑�
#define BOSS_SCORE 1000	//�{�X�̃X�R�A
#define BOSS_SIZE_X 210.0f	//�{�X��X���̑傫��
#define BOSS_SIZE_Y 210.0f	//�{�X��Y���̑傫��
#define BOSS_SIZE D3DXVECTOR3(BOSS_SIZE_X, BOSS_SIZE_Y, 0.0f)	//�{�X�̑傫��

//=============================================================================
//�{�X�N���X
//=============================================================================
class CBoss : public CCharacter
{
public:
	//=========================================================================
	//�{�X�̍U���p�^�[���̗񋓌^
	//=========================================================================
	typedef enum
	{
		BOSS_ATTACK_PATTERN_NOMRAL = 0,		//�ʏ�U��
		BOSS_ATTACK_PATTERN_ACCELEU_BULLET,	//�����e�ł̍U��
	}BOSS_ATTACK_PATTERN;

	//=========================================================================
	//�퓬��Ԃ̗񋓌^
	//=========================================================================
	typedef enum
	{
		BUTTLE_STATE_STRAT_UP = 0,	//�퓬�J�n�������
		BUTTLE_STATE_ON_BUTTLE,		//�퓬���
	}BUTTLE_STATE;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CBoss(int nPriority = CScene::PRIORITY_ENEMY);
	~CBoss();
	
	static CBoss *Create(
		const D3DXVECTOR3 pos,
		const D3DXVECTOR3 size,
		const int nLife,
		const int nScore);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DiedProcess(void);

	void Damage(const int nDamage);
	
	void Attack(void);
	void ChangeAttackPattern(void);
	D3DXVECTOR3 TargetWayVec3Calculation(D3DXVECTOR3 TargetPos, float fSpeed);
	
private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_move;	//�ړ���
	BUTTLE_STATE m_buttleState;	//�퓬���
	BOSS_ATTACK_PATTERN m_atkPattern;	//�U���p�^�[��
	int m_nScore;		//�X�R�A
	int m_nAtkCounter;	//�U���J�E���^�[
	int m_nChangeCounter;	//�U���p�^�[���̕ύX�J�E���^�[
	int m_nDiedCounter;		//���S�J�E���^�[
};

#endif 