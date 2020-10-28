//=============================================================================
//
// ���C������ [scene2d.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _SCENE2D_H
#define _SCENE2D_H

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define POLIGON_SIZE_X 100.0f	//�|���S����X���̑傫��
#define POLIGON_SIZE_Y 100.0f	//�|���S����Y���̑傫��
#define POLYGON_SIZE D3DXVECTOR3(POLIGON_SIZE_X, POLIGON_SIZE_Y, 0.0f)	//�|���S���̑傫��

//=============================================================================
//�F�̃}�N����`
//=============================================================================
#define COLOR_RED D3DCOLOR_RGBA(255,50,50,255)	//�ԐF
#define COLOR_BLUE D3DCOLOR_RGBA(50,50,255,255)	//�F
#define COLOR_CYAN D3DCOLOR_RGBA(50,255,255,255)	//�V�A��
#define COLOR_GREEN D3DCOLOR_RGBA(50,255,50,255)	//�ΐF
#define COLOR_YELLOW D3DCOLOR_RGBA(255,255,50,255)	//���F
#define COLOR_PURPLE D3DCOLOR_RGBA(128,50,128,255)	//��
#define COLOR_BLACK D3DCOLOR_RGBA(0,0,0,255)	//���F
#define COLOR_WHITE D3DCOLOR_RGBA(255,255,255,255)	//���F
#define COLOR_ALPHA D3DCOLOR_RGBA(255,255,255,50)


//============================================================================
//2D�|���S���N���X
//=============================================================================
class CScene2D : public CScene
{
public:
	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CScene2D(int nPriority = CScene::PRIORITY_DEFAULT);
	virtual ~CScene2D();
	static CScene2D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nPriority = CScene::PRIORITY_DEFAULT);

	virtual HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	static CScene2D *Collision(CScene::OBJTYPE objtype, D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void)const;
	D3DXVECTOR3 GetSize(void)const;
	void SetSize(const D3DXVECTOR3 size);
	void SetUV(const D3DXVECTOR2 *UVpos);
	void SetColor(const D3DXCOLOR col);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	
private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	D3DXVECTOR3 m_pos;	//�|���S���̈ʒu
	LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_Size;	//�|���S���̃T�C�Y

	float m_fLength;	// �Ίp���̒���
	float m_fAngle;	// �Ίp���̊p�x
	D3DXVECTOR3 m_rot;	// ����
	float m_fScale;	// �g��
	float m_fAddScale;	// �g����Z�l
};

#endif