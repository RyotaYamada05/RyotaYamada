//=============================================================================
//
// UI���� [ui.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define UNDER_UI_SIZE_X SCREEN_WIDTH	//UI��X���̃T�C�Y
#define UNDER_UI_SIZE_Y 90.0f			//UI��Y���̃T�C�Y
#define UNDER_UI_SIZE D3DXVECTOR3(SCREEN_WIDTH,UNDER_UI_SIZE_Y,0.0f)	//UI�̃T�C�Y

//=============================================================================
//�O���錾
//=============================================================================
class CLife;
class CScore;
class CScene2D;
class CAnimalUi;
class CSkill;

//=============================================================================
//UI�N���X
//=============================================================================
class CUi
{
public:
	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CUi();
	~CUi();
	static CUi *Create(void);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore *GetScore(void);
	static CLife *GetLife(void);
	static CSkill *GetSkill(void);
	static CAnimalUi *GetAnimalUI(void);
private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static CScore *m_pScore;	//�X�R�A�N���X�̃|�C���^
	static CLife *m_pLife;	//���C�t�N���X�̃|�C���^�ϐ�
	static CSkill *m_pSkill;	//�X�L���N���X�̃|�C���^�ϐ�
	static CAnimalUi *m_pAnimalUI;	//�A�j�}��UI�N���X�̃|�C���^�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���ւ̃|�C���^
	CScene2D *m_pScene2D;	//2D�|���S���N���X�ւ̃|�C���^
};
#endif 