//=============================================================================
//
// �X�L������ [skillber.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _SKILL_H_
#define _SKILL_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "gauge.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define SKILL_GAGUE_SIZE_X 433.0f	//�X�L���Q�[�W��X���̃T�C�Y
#define SKILL_GAGUE_SIZE_Y 19	//�X�L���Q�[�W��Y���̃T�C�Y
#define SKILL_GAGUE_SIZE D3DXVECTOR3(SKILL_GAGUE_SIZE_X,SKILL_GAGUE_SIZE_Y,0.0f)	//�X�L���Q�[�W�̃T�C�Y

//=============================================================================
//�X�L���N���X
//=============================================================================
class CSkillber : public CGauge
{
public:
	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CSkillber(int nPriority = CScene::PRIORITY_UI);
	~CSkillber();

	static CSkillber *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col,
		const int nGaugeNum, const int nMaxGaugeNum);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};

#endif 