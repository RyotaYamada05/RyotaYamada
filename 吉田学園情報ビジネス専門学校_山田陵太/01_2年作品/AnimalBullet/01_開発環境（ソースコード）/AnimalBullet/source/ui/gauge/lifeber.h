//=============================================================================
//
// ���C������ [lifeber.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _LIFEBER_H_
#define _LIFEBER_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "gauge.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define LIFE_GAGUE_SIZE_X 450.0f	//���C�t�Q�[�W��X���̃T�C�Y
#define LIFE_GAGUE_SIZE_Y 18	//���C�t�Q�[�W��Y���̃T�C�Y
#define LIFE_GAGUE_SIZE D3DXVECTOR3(LIFE_GAGUE_SIZE_X,LIFE_GAGUE_SIZE_Y,0.0f)	//���C�t�Q�[�W�̃T�C�Y

//=============================================================================
//���C�t�o�[�N���X
//=============================================================================
class CLifeber :public CGauge
{
public:
	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CLifeber(int nPriority = CScene::PRIORITY_UI);
	~CLifeber();

	static CLifeber *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col,
		const int nGaugeNum, const int nMaxGaugeNum);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
private:
};
#endif