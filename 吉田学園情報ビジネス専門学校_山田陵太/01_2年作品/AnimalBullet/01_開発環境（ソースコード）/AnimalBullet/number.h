//=============================================================================
//
// �i���o�[���� [number.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define NUMBER_SIZE_X 25.0f	//�i���o�[��X���̑傫��
#define NUMBER_SIZE_Y 50.0f	//�i���o�[��Y���̑傫��
#define NUMBER_SIZE D3DXVECTOR3(NUMBER_SIZE_X,NUMBER_SIZE_Y,0.0f)	//�i���o�[�̑傫��

//=============================================================================
//�i���o�[�N���X
//=============================================================================
class CNumber
{
public:
	//=========================================================================
	//�񋓌^��`
	//=========================================================================
	typedef enum
	{
		NUMBER_TYPE_000 = 0,
		NUMBER_TYPE_001,
		NUMBER_TYPE_MAX
	}NUMBER_TYPE;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CNumber();
	~CNumber();

	static CNumber *Create(const int nNumber, const NUMBER_TYPE Type, const D3DXVECTOR3 pos, const D3DXVECTOR3 size,const D3DXCOLOR color);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const int nNumber, const NUMBER_TYPE Type, const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR color);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(const int nNumber);
	void SetColr(const D3DXCOLOR col);

private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture[NUMBER_TYPE_MAX];	//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	int m_nNumber;	//�i���o�[
	D3DXVECTOR3 m_size;	//�|���S���̃T�C�Y
	D3DXVECTOR3 m_pos;	//�i���o�[�̈ʒu
	NUMBER_TYPE m_type;	//���
};
#endif 