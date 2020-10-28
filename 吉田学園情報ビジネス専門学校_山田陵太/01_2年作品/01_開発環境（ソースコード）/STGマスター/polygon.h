//=============================================================================
//
// ���C������ [polygon.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//���C�t�N���X
//=============================================================================
class CPolygon
{
public:
	//=========================================================================
	//�񋓌^��`
	//=========================================================================
	typedef enum
	{
		TEX_TYPE_NORE = 0,
		TEX_TYPE_TITLE,	//�^�C�g���e�N�X�`��
		TEX_TYPE_TITLE_LOGO,
		TEX_TYPE_RESULET,	//���U���g�e�N�X�`��
		TEX_TYPE_GAUGE_UI,	//�Q�[�WUI
		TEX_TYPE_UNDERUI,	//�A���_�[UI�e�N�X�`��
		TEX_TYPE_CHEETAH,	//�`�[�^�[
		TEX_TYPE_GORILLA,	//�S����
		TEX_TYPE_TURTLE,	//�J��
		TEX_TYPE_PRESSENTER,	//PRESS ENTER�e�N�X�`��
		TEX_TYPE_WARNING,
		TEX_TYPE_GAME_CLEAR,
		TEX_TYPE_GAME_OVER,
		TEX_TYPE_MAX,	//�ő�e�N�X�`��
	}TEX_TYPE;

	typedef enum
	{
		FLASHING_NONE = 0,
		FLASHING_ADD,
		FLASHING_SUB,
		FLASHING_MAX,
	}FLASHING;
	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CPolygon();
	~CPolygon();

	static CPolygon *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const TEX_TYPE type);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const TEX_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(const TEX_TYPE type);
	void SetColor(const D3DXCOLOR color);
	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void);
	void Flashing(void);

private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture[TEX_TYPE_MAX];	//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_size;	//�|���S���̃T�C�Y
	D3DXVECTOR3 m_pos;	//�|���S���̈ʒu
	D3DXCOLOR m_col;
	TEX_TYPE m_Type;
	FLASHING m_Flashing;
};
#endif 