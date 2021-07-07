//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : 
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene2d.h"

//=============================================================================
//�G�t�F�N�g�N���X
//=============================================================================
class CEffect :public CScene2D
{
public:
	//=========================================================================
	//�G�t�F�N�g�̎�ޗp�̗񋓌^
	//=========================================================================
	typedef enum
	{
		EFFECT_TYPE_BULLET = 0,	//�o���b�g
		EFFECT_TYPE_CHEETAH,	//�`�[�^�[
		EFFECT_TYPE_GORILLA,	//�S����
		EFFECT_TYPE_TURTLE,		//�J��
		EFFECT_TYPE_BOME,		//�{��
		EFFECT_TYPE_TITLE,		//�^�C�g��
		EFFECT_TYPE_MAX
	}EFFECT_TYPE;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CEffect(int nPriority = CScene::PRIORITY_EFFECT);
	~CEffect();

	static CEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col, int nLife, const EFFECT_TYPE type, const int nPriority = CScene::PRIORITY_EFFECT);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void UpdateBullet(void);
	void EffectByType(void);
	void BomeUpdate(void);
	void TitleUpdate(void);
	void SetTexType(const EFFECT_TYPE type);
private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture[EFFECT_TYPE_MAX];	//�e�N�X�`���|�C���^
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_size;	//�T�C�Y
	D3DXCOLOR m_col;	//�F
	EFFECT_TYPE m_type;	//���
	int m_nLife;	//���C�t
};
#endif