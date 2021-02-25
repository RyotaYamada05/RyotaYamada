//=============================================================================
//
// �������� [explosion.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _EXPLOSOIN_H_
#define _EXPLOSOIN_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene2d.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define EXPLOSION_SIZE_X 50.0f	//������X���̑傫��
#define EXPLOSION_SIZE_Y 50.0f	//������Y���̑傫��
#define EXPLOSION_SIZE D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f)

//=============================================================================
//�G�N�X�v���[�W�����N���X
//=============================================================================
class CExplosion : public CScene2D
{
public:
	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CExplosion(int nPriority = PRIORITY_EXPLOSION);
	~CExplosion();

	static CExplosion *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col, int nPriority = PRIORITY_EXPLOSION);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���̃|�C���^
	int m_nCounterAnim;	//�A�j���[�V�����J�E���^�[
	int m_nPatternAnim;	//�A�j���[�V�����p�^�[��
};

#endif