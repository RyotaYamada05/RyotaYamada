//=============================================================================
//
// �A�C�e������ [item.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "ui.h"
#include "skillber.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define ITEM_MOVE 2.5f	//�A�C�e���̈ړ���
#define LIFE_RECOVERY_VALUE 20 //�̗͂̉񕜗�
#define SKILL_RECOVERY_VALUE 50	//�X�L���̉񕜗�

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CItem::m_apTexture[CItem::ITEM_TYPE_MAX] = {};

//=============================================================================
//�A�C�e���N���X�̃R���X�g���N�^
//=============================================================================
CItem::CItem(int nPriority) : CScene2D(nPriority)
{
	//�e�����o�ϐ��̃N���A
	m_pos = VECTOR3_ZERO_INITIALIZE;
	m_size = VECTOR3_ZERO_INITIALIZE;
	m_type = ITEM_TYPE_LIFE;
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
	//�A�C�e���N���X�̃|�C���^
	CItem *pItem = NULL;

	//�C���X�^���X����
	pItem = new CItem;

	//�������m�ۂł��Ă�����
	if (pItem)
	{
		pItem->SetPos(pos);
		pItem->SetSize(size);
		pItem->m_type = type;
		//�������������s
		pItem->Init();
	}
	//���s���Ă�����
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
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/Skill2.png", &m_apTexture[ITEM_TYPE_SKILL]);	//�X�L��
	
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
		if (m_apTexture[nCount])
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//�A�C�e���N���X�̏���������
//=============================================================================
HRESULT CItem::Init()
{
	//2D�|���S���N���X�̏����������Ăяo��
	CScene2D::Init();

	//�e�N�X�`���̐ݒ�
	CScene2D::BindTexture(m_apTexture[m_type]);

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(OBJTYPE_ITEM);

	//�X�L���A�C�e���������ꍇ
	if (m_type == ITEM_TYPE_SKILL)
	{
		//�F�̐ݒ�
		CScene2D::SetColor(COLOR_BLUE);
	}
	
	return S_OK;
}

//=============================================================================
//�A�C�e���N���X�̏I������
//=============================================================================
void CItem::Uninit(void)
{
	//2D�|���S���N���X�̏I�������Ăяo��
	CScene2D::Uninit();
}

//=============================================================================
//�A�C�e���N���X�̍X�V����
//=============================================================================
void CItem::Update(void)
{
	//CScene2D�̍X�V����
	CScene2D::Update();

	//�ʒu�̎擾
	m_pos = CScene2D::GetPos();

	//�v���C���[�Ƃ��������ǂ����̏����擾
	CPlayer *pPlayer = (CPlayer*)CScene2D::JudgeCollision(CScene2D::OBJTYPE_PLAYER, m_pos, GetSize());

	if (pPlayer)
	{
		switch (m_type)
		{
		//���C�t�A�C�e���������ꍇ
		case ITEM_TYPE_LIFE:
			//�v���C���[�̗̑͂���
			pPlayer->Recovery(LIFE_RECOVERY_VALUE);

			//�I�������Ăяo��
			Uninit();

			return;
			break;

		//�X�L���A�C�e���������ꍇ
		case ITEM_TYPE_SKILL:
			//�X�L���|�C���g�����Z
			pPlayer->AddSkillPoint(SKILL_RECOVERY_VALUE);
			
			//�I�������Ăяo��
			Uninit();
			return;

			break;
		}
	}

	//�ړ��ʂ����Z
	m_pos.x -= ITEM_MOVE;

	//�ʒu�̐ݒ�
	CScene2D::SetPos(m_pos);

	//�A�C�e������ʊO�ɍs������
	if (m_pos.x <= -100)
	{
		//�I�������Ăяo��
		Uninit();
		return;
	}
}

//=============================================================================
//�A�C�e���N���X�̕`�揈��
//=============================================================================
void CItem::Draw(void)
{
	//2D�|���S���N���X�̕`�揈���Ăяo��
	CScene2D::Draw();
}
