//=============================================================================
//
// �o���b�g���� [bullet.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene2d.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define BULLET_SIZE_X 20.0f	//�o���b�g��X�T�C�Y
#define BULLET_SIZE_Y 20.0f	//�o���b�g��Y�T�C�Y
#define BULLET_SIZE D3DXVECTOR3(BULLET_SIZE_X,BULLET_SIZE_Y,0.0f)
#define BULLET_LIFE 600	//�o���b�g�̎���

//=============================================================================
//�o���b�g�N���X
//=============================================================================
class CBullet : public CScene2D
{
public:
	//=========================================================================
	//�o���b�g�g�p�҂̗񋓌^��`
	//=========================================================================
	typedef enum
	{
		BULLET_USER_NONE = 0,
		BULLET_USER_PLAYER,	//�v���C���[�̃o���b�g
		BULLET_USER_ENEMY,	//�G�l�~�[�̃o���b�g
		BULLET_USER_MAX,	//�o���b�g�^�C�v�̍ő吔
	}BULLET_USER;

	//=========================================================================
	//�o���b�g�̎�ނ̗񋓌^��`
	//=========================================================================
	typedef enum
	{
		BULLET_TYPE_NORMAL = 0,
		BULLET_TYPE_CHEETAH,	//�`�[�^�[
		BULLET_TYPE_GORILLA,	//�S����
		BULLET_TYPE_TURTLE,	//�J��
		BULLET_TYPE_HOMING,	//�z�[�~���O�e
		BULLET_TYPE_DIVISION,	//�����e
		BULLET_TYPE_ACCELE,	//�����e
		BULLET_TYPE_BOMB,	//�{��
		BULLET_TYPE_MAX,
	}BULLET_TYPE;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CBullet(int nPriority = CScene::PRIORITY_BULEET);
	~CBullet();

	static CBullet*Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const D3DXCOLOR col,
		const BULLET_USER user, const BULLET_TYPE type, int nAtk, int nLife = BULLET_LIFE);
	static HRESULT Load(void);
	static void UnLoad(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	bool BulletCollision(void);
	void HomingUpdate(void);
	void DivisionUpdate(void);
	void ACCELEUpdetae(void);
	BULLET_USER GetUser(void)const;

private:
	//=========================================================================
	//�����o�ϐ���
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_pos;	//���݈ʒu
	D3DXVECTOR3 m_move;	//�ړ���
	D3DXCOLOR m_col;	//�F
	D3DXVECTOR3 m_size;	//�T�C�Y
	int m_nLife;	//�ʂ̎���
	int m_nAtk;	//�U����
	int m_nEffectCounter;
	BULLET_USER m_user;	//�o���b�g�̎g�p��
	BULLET_TYPE m_type;	//�e�̎��
};

#endif