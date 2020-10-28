//=============================================================================
//
// ���C������ [item.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "game.h"
#include "ui.h"
#include "skill.h"

#define ITEM_LIFE 120
#define ITEM_MOVE 2.5f
//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CItem::m_apTexture[CItem::ITEM_TYPE_MAX] = {};

//=============================================================================
//�A�C�e���N���X�̃R���X�g���N�^
//=============================================================================
CItem::CItem(int nPriority) : CScene2D(nPriority)
{
	m_pos = D3DXVECTOR3();
	m_size = D3DXVECTOR3();
	m_type = ITEM_TYPE_LIFE;
	m_nLife = 0;
}

//=============================================================================
//�A�C�e���N���X�̃f�X�g���N�^
//=============================================================================
CItem::~CItem()
{
}

//=============================================================================
//�A�C�e���N���X�̃N���G�C�g����
//=============================================================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, ITEM_TYPE type)
{
	CItem *pItem = NULL;

	pItem = new CItem;

	if (pItem != NULL)
	{
		pItem->Init(pos, size, type);
	}
	else
	{
		return NULL;
	}

	return pItem;
}

//=============================================================================
//�A�C�e���N���X�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CItem::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Life2.png", &m_apTexture[ITEM_TYPE_LIFE]);	//���C�t
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/Skill.png", &m_apTexture[ITEM_TYPE_SKILL]);	//�X�L��
	
	return S_OK;
}

//=============================================================================
//�A�C�e���N���X�̃e�N�X�`���j������
//=============================================================================
void CItem::UnLoad(void)
{
	for (int nCount = 0; nCount < ITEM_TYPE_MAX; nCount++)
	{
		//�e�N�X�`���̔j��
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//�A�C�e���N���X�̏���������
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, ITEM_TYPE type)
{
	m_pos = pos;
	m_size = size;
	m_type = type;
	m_nLife = ITEM_LIFE;

	CScene2D::Init(m_pos, m_size);
	CScene2D::BindTexture(m_apTexture[m_type]);
	CScene::SetObjType(OBJTYPE_ITEM);

	return E_NOTIMPL;
}

//=============================================================================
//�A�C�e���N���X�̏I������
//=============================================================================
void CItem::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
//�A�C�e���N���X�̍X�V����
//=============================================================================
void CItem::Update(void)
{
	m_pos = CScene2D::GetPos();
	//�v���C���[�Ƃ��������ǂ����̏����擾
	CPlayer *pPlayer = (CPlayer*)CScene2D::Collision(CScene2D::OBJTYPE_PLAYER, m_pos, m_size);

	if (pPlayer != NULL)
	{
		switch (m_type)
		{
		case ITEM_TYPE_LIFE:
			CGame::GetPlayer()->Recovery(25);
			Uninit();
			return;
			break;

		case ITEM_TYPE_SKILL:
			CGame::GetUi()->GetSkill()->AddSkill(50);
			Uninit();
			return;

			break;
		}
	}
	m_pos.x -= ITEM_MOVE;

	CScene2D::SetPos(m_pos);

	if (m_pos.x <= -100)
	{
		Uninit();
		return;
	}
	/*m_nLife--;
	if (m_nLife <= 0)
	{
		Uninit();
		return;
	}*/
}

//=============================================================================
//�A�C�e���N���X�̕`�揈��
//=============================================================================
void CItem::Draw(void)
{
	CScene2D::Draw();
}
