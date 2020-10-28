//=============================================================================
//
// ���C������ [ranking.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _RANKING_H
#define _RANKING_H

#include "scene.h"
#define RANKING_NUM 5	//�����L���O��
#define NUM_LENGTH 8	//����
class CNumber;

class CRanking: public CScene
{
public:
	CRanking();
	~CRanking();

	static CRanking *Create(D3DXVECTOR3 pos = D3DXVECTOR3(), D3DXVECTOR3 size = D3DXVECTOR3());

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void ReadFile(void);
	void WriteFile(void);
	void SetRanking(int nScore);
	void SetNumber(void);

private:
	int m_nRank;
	int m_nScore;
	int m_nRanking[RANKING_NUM];
	CNumber *m_apNumber[RANKING_NUM][NUM_LENGTH];
	CNumber *m_apScoreNum[NUM_LENGTH];
	
	CNumber *m_apRank[RANKING_NUM];
};

#endif 