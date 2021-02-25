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
#include "scene2d.h"

//=============================================================================
//�{�X�N���X
//=============================================================================
class CBoss :public CScene2D
{
public:
	//=========================================================================
	//�{�X��Ԃ̗񋓌^
	//=========================================================================
	typedef enum
	{
		BOSS_STATE_NORMAL = 0,	//�ʏ���
		BOSS_STATE_DAMAGE,	//�_���[�W���
		BOSS_STATE_DIED,	//���S���
		BOSS_STATE_MAX
	}BOSS_STATE;

	typedef enum
	{
		ACTION_PATTERN_STAERT = 0,	//START��Ԃ̍s��
		ACTION_PATTERN_NOMRAL,	//�ʏ�̍s��
		ACTION_PATTERN_MOVE_DOWN,	//���ֈړ�����,
		ACTION_PATTERN_MOVE_UP,	//��ֈړ�����
		ACTION_PATTERN_STOP,	//��~����
		ACTION_PATTERN_UP_AND_DOWN,	//�㉺�Ɉړ�����
		ACTION_PATTERN_STALKER,	//�ǐՂ���
		ACTION_PATTERN_ACCELEU,	//����
		ACTION_PATTERN_MAX
	}ACTION_PATTERN;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CBoss(int nPriority = CScene::PRIORITY_ENEMY);
	~CBoss();

	static CBoss *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Action(void);
	void Damage(const int nDamage);
	bool DecisionState(void);
	D3DXVECTOR3 VectorMath(D3DXVECTOR3 TargetPos, float fSpeed);
	
private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_move;	//�ړ���
	D3DXVECTOR3 m_size;	//�T�C�Y
	BOSS_STATE m_state;	//���
	ACTION_PATTERN m_action;	//�s���p�^�[��
	int m_nLife;	//���C�t
	int m_nCount;	//�J�E���^�[
	int m_nScore;	//�X�R�A
	int m_nActionCounter;	//�A�N�V�����J�E���^�[
	int m_nStateCounter;	//��ԃJ�E���^�[
};

#endif 