//=============================================================================
//
// ���C������ [enemy.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _ENEMY_H
#define _ENEMY_H

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene2d.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define ENEMY_SIZE_X 70.0f	//�G�l�~�[��X���̑傫��
#define ENEMY_SIZE_Y 70.0f	//�G�l�~�[��Y���̑傫��
#define ENEMY_SIZE D3DXVECTOR3(ENEMY_SIZE_X, ENEMY_SIZE_Y, 0.0f)	//�G�l�~�[�̑傫��
#define ENEMY_MOVE_X 3.0f	//�G�l�~�[��X���̈ړ���
#define ENEMY_MOVE_Y 3.0f	//�G�l�~�[��Y���̈ړ���

//=============================================================================
//�G�l�~�[�N���X
//=============================================================================
class CEnemy : public CScene2D
{
public:
	//=========================================================================
	//�񋓌^��`
	//=========================================================================
	typedef enum
	{
		ENEMY_STATE_NORMAL = 0,	//�ʏ���
		ENEMY_STATE_DAMAGE,	//��e���
		ENEMY_STATE_DIED,	//���S���
		ENEMY_STATE_MAX,
	}ENEMY_STATE;

	typedef enum
	{
		ENEMY_TYPE_NO_BULLET = 0,
		ENEMY_TYPE_NORMAL,	//���ʂ̓G
		ENEMY_TYPE_SNIPING,	//�_�������̓G
		ENEMY_TYPE_HOMING,	//�z�[�~���O�e�̓G
		ENEMY_TYPE_HADE,
		ENEMY_TYPE_DIVISION,	//�����e�̓G
		ENEMY_TYPE_STOKER,
		ENEMY_TYPE_MAX
	}ENEMY_TYPE;

	typedef enum
	{
		Dead_Process_NOEN = 0,
		Dead_Process_LIFE,
		Dead_Process_SKILL,
		Dead_Process_BOSS,
	}Dead_Process;

	//=========================================================================
	//�s���p�^�[��
	//=========================================================================
	typedef enum
	{
		ACTION_PATTERN_NOMRAL = 0,	//�ʏ�̍s��
		ACTION_PATTERN_MOVE_DOWN,	//���ֈړ�����,
		ACTION_PATTERN_MOVE_UP,	//��ֈړ�����
		ACTION_PATTERN_STOP,
		ACTION_PATTERN_STOP_RETURN,
		ACTION_PATTERN_UP_AND_DOWN,
		ACTION_PATTERN_STALKER,	//�ǐՂ���
		ACTION_PATTERN_MAX
	}ACTION_PATTERN;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CEnemy(int nPriority = CScene::PRIORITY_ENEMY);
	~CEnemy();

	static CEnemy*Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const ENEMY_TYPE type,
		const ACTION_PATTERN action = ACTION_PATTERN_NOMRAL, Dead_Process DeadProcess = Dead_Process_NOEN);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const ENEMY_TYPE type,
		const ACTION_PATTERN action, const Dead_Process DeadProcess);
	void Uninit(void);
	void Update(void);
	void NormalUpdate(void);
	void SnipingUpdate(void);
	void HadeUpdate(void);
	void DivisionUpdate(void);


	void ActionUpdate(void);
	void Draw(void);
	void SetData(void);
	void Damage(const int nDamage);
	bool DecisionState(void);
	D3DXVECTOR3 GetPos(void)const;

private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture[ENEMY_TYPE_MAX];	//�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_move;	//�ړ���
	D3DXVECTOR3 m_size;	//�T�C�Y
	int m_nLife;	//���C�t
	int m_nCounter;
	int m_nScore;	//�X�R�A
	ENEMY_STATE m_State;	//�G�l�~�[�̏��
	Dead_Process m_DeadProcess;	//���S���̏����p�ϐ�
	int m_nStateCounter;
	int m_nActionCounter;
	ENEMY_TYPE m_type;	//�G�l�~�[�̎��
	ACTION_PATTERN m_action;	//�s���p�^�[��
	float m_fSpeed;	//���x
	float m_fDistance;	//����
};
#endif