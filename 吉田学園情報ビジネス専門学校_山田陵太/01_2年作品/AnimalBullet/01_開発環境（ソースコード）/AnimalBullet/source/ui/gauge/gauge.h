//=============================================================================
//
// �Q�[�W���� [gauge.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene.h"

//=============================================================================
//�Q�[�W�N���X
//=============================================================================
class CGauge : public CScene
{
public:
	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CGauge(int nPriority = PRIORITY_UI);
	virtual ~CGauge();

	static CGauge*Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col,
		const int nGaugeNum, const int nMaxGaugeNum);
	static HRESULT Load(void);
	static void UnLoad(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetPos(const D3DXVECTOR3 pos);
	void SetSize(const D3DXVECTOR3 size);
	void SetColor(const D3DXCOLOR col);
	void SetGauge(const int nGaugeNum);
	int GetGauge(void)const;
	void SetMaxGauge(const int nMaxGaugeNum);
	int GetMaxGague(void)const;
	void AddGauge(int nAddGauge);
	void SubGauge(int nSubGauge);
	
private:
	void VertexUpdate(void);

	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTextuer;	//�e�N�X�`���ւ̃|�C���^�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	int m_nGaugeNum;	//�Q�[�W
	int m_nMaxGaugeNum;	//�Q�[�W�̍ő�l
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_size;	//�T�C�Y
	D3DXCOLOR m_col;	//�F
};


#endif