//=============================================================================
//
// ���C������ [life.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define GAUGE_SIZE_X 600.0f	//�Q�[�W��X���̑傫��
#define GAUGE_SIZE_Y 80.0f	//�Q�[�W��Y���̑傫��
#define GAUGE_SIZE D3DXVECTOR3(GAUGE_SIZE_X,GAUGE_SIZE_Y,0.0f)	//�Q�[�W�̃T�C�Y
#define PLAYER_MAX_LIFE	 100 //�v���C���[�̍ő�̗�
#define LIFE_GAGUE_SIZE_X 4.5f	//���C�t�Q�[�W��X���̃T�C�Y
#define LIFE_GAGUE_SIZE_Y 30	//���C�t�Q�[�W��Y���̃T�C�Y
#define LIFE_GAGUE_SIZE D3DXVECTOR3(LIFE_GAGUE_SIZE_X,LIFE_GAGUE_SIZE_Y,0.0f)	//���C�t�Q�[�W�̃T�C�Y

//=============================================================================
//�O���錾
//=============================================================================
class CGauge;
class CPolygon;

//=============================================================================
//���C�t�N���X
//=============================================================================
class CLife :public CScene
{
public:
	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CLife(int nPriority = CScene::PRIORITY_UI);
	~CLife();
	static CLife *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetLife(int nLife);
	void AddLife(int nLife);
	void SubLife(int nLife);

private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	CGauge *m_pGauge;	//�Q�[�W�N���X�̃|�C���^�ϐ�
	CPolygon *m_Polygon;	//�|���S���N���X�̃|�C���^�ϐ�
	int m_nLife;	//�̗͂̒l
};
#endif