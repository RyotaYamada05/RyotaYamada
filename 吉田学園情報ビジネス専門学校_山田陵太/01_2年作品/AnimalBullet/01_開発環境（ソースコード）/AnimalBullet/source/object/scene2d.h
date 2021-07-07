//=============================================================================
//
// 2D�|���S������ [scene2d.h]
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

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	static CScene2D *JudgeCollision(CScene::OBJTYPE objtype, D3DXVECTOR3 pos, D3DXVECTOR3 size);
	
	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void)const;
	void SetSize(const D3DXVECTOR3 size);
	D3DXVECTOR3 GetSize(void)const;
	void SetUV(const D3DXVECTOR2 *UVpos);
	void SetColor(const D3DXCOLOR col);
	D3DXCOLOR GetColor(void)const;
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	
private:
	void VertexUpdate(void);
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;	//�|���S���̈ʒu
	D3DXVECTOR3 m_size;	//�|���S���̃T�C�Y
	D3DXCOLOR m_col;	//�|���S���̐F
};

#endif