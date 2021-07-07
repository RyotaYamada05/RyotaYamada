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
#define UNDER_UI_SIZE_Y 90.0f			//UNDER_UI��Y���̃T�C�Y
#define UNDER_UI_SIZE D3DXVECTOR3(SCREEN_WIDTH,UNDER_UI_SIZE_Y,0.0f)	//UNDER_UI�̃T�C�Y
#define GAUGE_UI_SIZE_X 600.0f	//�Q�[�WUI��X���̑傫��
#define GAUGE_UI_SIZE_Y 80.0f	//�Q�[�WUI��Y���̑傫��
#define GAUGE_UI_SIZE D3DXVECTOR3(GAUGE_UI_SIZE_X,GAUGE_UI_SIZE_Y,0.0f)	//�Q�[�WUI�̃T�C�Y

//=============================================================================
//�O���錾
//=============================================================================
class CLifeber;
class CScore;
class CScene2D;
class CAnimalUi;
class CSkillber;

//=============================================================================
//UI�N���X
//=============================================================================
class CUi
{
public:
	//=========================================================================
	//�\���̒�`
	//=========================================================================
	typedef enum
	{
		UI_TEX_1 = 0, 
		UI_TEX_2,
		UI_TEX_MAX
	}UI_TEX;

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
	static CLifeber *GetLifeber(void);
	static CSkillber *GetSkillber(void);
	static CAnimalUi *GetAnimalUI(void);
private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static CScore *m_pScore;	//�X�R�A�N���X�̃|�C���^
	static CLifeber *m_pLifeber;	//���C�t�N���X�̃|�C���^�ϐ�
	static CSkillber *m_pSkillber;	//�X�L���N���X�̃|�C���^�ϐ�
	static CAnimalUi *m_pAnimalUI;	//�A�j�}��UI�N���X�̃|�C���^�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture[UI_TEX_MAX];	//�e�N�X�`���ւ̃|�C���^
};
#endif 