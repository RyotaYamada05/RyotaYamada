//=============================================================================
//
// �V�[���h���� [shield.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "shield.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "bullet.h"
#include "beam.h"
#include "sound.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define COUNTER_ATK 10	//�U����
#define GUARD_LIMIT_TIME 300	//�K�[�h��Ԃ̐�������
#define MAX_COUNTER_LIMIT 600	//�J�E���^�[�̍ő吧������	
#define MIN_COUNTER_LIMIT 300	//�J�E���^�[�̍ŏ���������	

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CShield::m_pTexture = NULL;

//=============================================================================
//�V�[���h�N���X�̃R���X�g���N�^
//=============================================================================
CShield::CShield(int nPriority):CScene2D(nPriority)
{
	//�e�����o�ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nAtk = 0;
	m_nHitCounter = 0;
	m_state = SHIELD_STATE_NONE;
	m_nTimeLimit = 0;
}

//=============================================================================
//�V�[���h�N���X�̃f�X�g���N�^
//=============================================================================
CShield::~CShield()
{
}

//=============================================================================
//�V�[���h�N���X�̃N���G�C�g����
//=============================================================================
CShield * CShield::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�V�[���h�N���X�̃|�C���^�ϐ�
	CShield *pShield = NULL;

	//�C���X�^���X����
	pShield = new CShield;

	//���������m�ۂł��Ă�����
	if (pShield != NULL)
	{
		//�����������Ăяo��
		pShield->Init(pos, size);
	}
	//���s���Ă�����
	else
	{
		return NULL;
	}

	return pShield;
}

//=============================================================================
//�V�[���h�N���X�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CShield::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/shied.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//�V�[���h�N���X�̃e�N�X�`���j������
//=============================================================================
void CShield::UnLoad(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//�V�[���h�N���X�̏���������
//=============================================================================
HRESULT CShield::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�ʒu�̐ݒ�
	m_pos = D3DXVECTOR3(pos.x + SHILED_SIZE_X * 3, pos.y, pos.z);;

	//�T�C�Y�̐ݒ�
	m_size = size;

	//��Ԃ̐ݒ�
	m_state = SHIELD_STATE_GUARD;

	//�U���͂̐ݒ�
	m_nAtk = COUNTER_ATK;

	//�������Ԃ̐ݒ�
	m_nTimeLimit = GUARD_LIMIT_TIME;

	//2D�|���S���N���X�̏�����
	CScene2D::Init(m_pos, m_size);

	//�e�N�X�`���̐ݒ�
	CScene2D::BindTexture(m_pTexture);

	return S_OK;
}

//=============================================================================
//�V�[���h�N���X�̏I������
//=============================================================================
void CShield::Uninit(void)
{
	//�I�������Ăяo��
	CScene2D::Uninit();
}

//=============================================================================
//�V�[���h�N���X�̍X�V����
//=============================================================================
void CShield::Update(void)
{
	//��ԕʂɏ��������s
	switch (m_state)
	{
	//�K�[�h���
	case SHIELD_STATE_GUARD:
		//�K�[�h�������s
		Guard();
		break;

	//�J�E���^�[�U�����
	case SHIELD_STATE_COUNTER_ATK:
		//�r�[���̐���
		CBeam::Create(m_pos, BEAM_SIZE, m_nTimeLimit);
		
		Uninit();
		return;
		
		break;

	default:
		break;
	}

	//�ʒu�̎擾
	m_pos = GetPos();

	//�v���C���[�̈ʒu�̎擾
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

	//�ʒu�̍X�V
	m_pos = D3DXVECTOR3(playerPos.x + SHILED_SIZE_X * 3, playerPos.y, playerPos.z);

	//�ʒu�̐ݒ�
	SetPos(m_pos);

	//�������Ԃ̌���
	m_nTimeLimit--;
}

//=============================================================================
//�V�[���h�N���X�̕`�揈��
//=============================================================================
void CShield::Draw(void)
{
	//�`�揈���Ăяo��
	CScene2D::Draw();
}

//=============================================================================
//�V�[���h�N���X�̃K�[�h����
//=============================================================================
void CShield::Guard(void)
{
	//�����^�C�}�[���K�[�h�����̐������ԓ���������
	if (m_nTimeLimit > 0)
	{
		//�o���b�g�Ɠ����������ǂ������擾
		CScene2D *pScene2D = GetCollision(OBJTYPE_BULLET, m_pos, m_size);

		//�����蔻��
		if (pScene2D != NULL)
		{
			//�o���b�g�N���X�փL���X�g
			CBullet *pBullet = (CBullet*)pScene2D;

			//�o���b�g�̎g�p�҂��G�������ꍇ
			if (pBullet->GetUser() == CBullet::BULLET_USER_ENEMY)
			{
				//�q�b�g�J�E���g�����Z
				m_nHitCounter++;

				//�o���b�g���폜
				pBullet->Uninit();
				pBullet = NULL;

				//�K�[�h���̍Đ�
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GUARD);
			}
		}
	}
	//�������ԃI�[�o�[��������
	else
	{
		//�J�E���^�[�U����Ԃ֕ύX
		m_state = SHIELD_STATE_COUNTER_ATK;

		//�������Ԃ��J�E���^�[�U����ԗp�֐ݒ肷��(�Œ᎞��+�q�b�g��*5)
		m_nTimeLimit = MIN_COUNTER_LIMIT + m_nHitCounter * 5;

		//�������Ԃ��ő�萔��蒴�����ꍇ�͏C��
		if (m_nTimeLimit > MAX_COUNTER_LIMIT)
		{
			m_nTimeLimit = MAX_COUNTER_LIMIT;
		}
	}
}