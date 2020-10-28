//=============================================================================
//
// ���C������ [gauge.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//�Q�[�W�N���X
//=============================================================================
class CGauge
{
public:
	//=========================================================================
	//�Q�[�W�^�C�v�̗񋓌^
	//=========================================================================
	typedef enum
	{
		GAUGE_TYPE_LIFE_GREEN = 0,	//���C�t�p�΃Q�[�W
		GAUGE_TYPE_LIFE_ORANGE,	//���C�t�p�I�����W�Q�[�W
		GAUGE_TYPE_LIFE_RED,	//���C�t�p�ԃQ�[�W
		GAUGE_TYPE_SKILL,
		GAUGE_TYPE_MAX	//�Q�[�W�ő吔
	}GAUGE_TYPE;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CGauge();
	~CGauge();

	static CGauge*Create(const D3DXVECTOR3 pos,const D3DXVECTOR3 size,int nGaugeNum, const GAUGE_TYPE type);
	static HRESULT Load(void);
	static void UnLoad(void);
	
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, int nGaugeNum, const GAUGE_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetGauge(const int nGaugeNum);
	void SetType(const GAUGE_TYPE type);
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
private:
	static LPDIRECT3DTEXTURE9 m_apTextuer[GAUGE_TYPE_MAX];	//�e�N�X�`���ւ̃|�C���^�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	int m_nGaugeNum;	//�Q�[�W
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_size;	//�T�C�Y
	GAUGE_TYPE m_type;	//�Q�[�W�^�C�v
};


#endif