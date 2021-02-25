//=============================================================================
//
// �r�[������ [beam.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _BEAM_H_
#define _BEAM_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define BEAM_SIZE_X 800.0f	//�r�[����X���̑傫��
#define BEAM_SIZE_Y 40.0f	//�r�[����Y���̑傫��
#define BEAM_SIZE D3DXVECTOR3(BEAM_SIZE_X,BEAM_SIZE_Y,0.0f)	//�r�[���̑傫��

//=============================================================================
//�O���錾
//=============================================================================
class CScene2D;

//=============================================================================
//�r�[���N���X
//=============================================================================
class CBeam : public CScene
{
public:
	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CBeam(int nPriority = CScene::PRIORITY_BULEET);
	~CBeam();

	static CBeam *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,
		 const int nLimit);

	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Collision(void);
	void SetLimit(const int nLimit);
	CScene2D *GetCollision(OBJTYPE objtype);

private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_move;	//�ړ���
	D3DXVECTOR3 m_size;	//�T�C�Y
	int m_nCntLimit;	//�������Ԃ̃J�E���^�[
	float m_fTexMove;	//�e�N�X�`���A�j���p
};
#endif 