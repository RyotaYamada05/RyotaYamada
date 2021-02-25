//=============================================================================
//
// �V�[���h���� [shield.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _SHIELD_H_
#define _SHIELD_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene2d.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define SHILED_SIZE_X 20.0f	//�V�[���h��X���̑傫��
#define SHILED_SIZE_Y 60.0f	//�V�[���h��Y���̑傫��
#define SHILED_SIZE D3DXVECTOR3(SHILED_SIZE_X,SHILED_SIZE_Y,0.0f)	//�V�[���h�̑傫��

//=============================================================================
//�V�[���h�N���X
//=============================================================================
class CShield : public CScene2D
{
public:
	//=========================================================================
	//�V�[���h�̏�Ԃ̗񋓌^��`
	//=========================================================================
	typedef enum
	{
		SHIELD_STATE_NONE = -1,
		SHIELD_STATE_GUARD,	//�K�[�h���
		SHIELD_STATE_COUNTER_ATK,	//�J�E���^�[�U�����
	}SHIELD_STATE;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CShield(int nPriority = CScene::PRIORITY_EFFECT);
	~CShield();

	static CShield *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Guard(void);

private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_size;	//�T�C�Y
	SHIELD_STATE m_state;	//���
	int m_nAtk;	//�U����
	int m_nHitCounter;	//�U�������������񐔂̃J�E���^�[
	int m_nTimeLimit;	//�����̐�������
};
#endif 