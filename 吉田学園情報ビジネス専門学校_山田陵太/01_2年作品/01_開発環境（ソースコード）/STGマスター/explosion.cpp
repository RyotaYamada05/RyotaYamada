//=============================================================================
//
// ���C������ [exeplosion.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "explosion.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"
#include "sound.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

//=============================================================================
//�G�N�X�v���[�W�����N���X�̃R���X�g���N�^
//=============================================================================
CExplosion::CExplosion(int nPriority) :CScene2D(nPriority)
{
	//�����o�ϐ��̃N���A
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//=============================================================================
//�G�N�X�v���[�W�����N���X�̃f�X�g���N�^
//=============================================================================
CExplosion::~CExplosion()
{
}

//=============================================================================
//�G�N�X�v���[�W�����N���X�̃N���G�C�g����
//=============================================================================
CExplosion * CExplosion::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col)
{
	//�G�N�X�v���[�W�����N���X�̃|�C���^
	CExplosion *pExeplosion = NULL;

	//�C���X�^���X����
	pExeplosion = new CExplosion;

	//���������m�ۂł��Ă�����
	if (pExeplosion != NULL)
	{
		//�����������Ăяo��
		pExeplosion->Init(pos, size, col);
	}

	return pExeplosion;
}

//=============================================================================
//�G�N�X�v���[�W�����N���X�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CExplosion::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/explosion.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//�G�N�X�v���[�W�����N���X�̃e�N�X�`���j������
//=============================================================================
void CExplosion::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		//�������̍폜
		m_pTexture->Release();

		//�������̃N���A
		m_pTexture = NULL;
	}
}

//=============================================================================
//�G�N�X�v���[�W�����N���X�̏���������
//=============================================================================
HRESULT CExplosion::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col)
{
	//�����o�ϐ��̏�����
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;

	//2D�|���S���N���X�̏����������Ăяo��
	CScene2D::Init(pos, size);

	//2D�|���S���N���X�̐F�ݒ菈���Ăяo��
	CScene2D::SetColor(col);

	//�e�N�X�`�����蓖�ď����Ăяo��
	CScene2D::BindTexture(m_pTexture);

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_EXPLOSION);

	//UV���W�p�̕ϐ�
	D3DXVECTOR2 aUVpos[4];

	//UV���W�̒�`
	aUVpos[0] = D3DXVECTOR2(0, 0.0f);
	aUVpos[1] = D3DXVECTOR2(0.125f, 0.0f);
	aUVpos[2] = D3DXVECTOR2(0, 1.0f);
	aUVpos[3] = D3DXVECTOR2(0.125f, 1.0f);

	//2D�|���S���N���X��UV���W�ݒ菈���Ăяo��
	CScene2D::SetUV(&aUVpos[0]);

	//�T�E���h�N���X�̏��擾
	CSound *pSound = CManager::GetSound();

	//�������̍Đ�
	pSound->Play(CSound::SOUND_LABEL_SE_EXEPLOSION);

	return S_OK;
}

//=============================================================================
//�G�N�X�v���[�W�����N���X�̏I������
//=============================================================================
void CExplosion::Uninit(void)
{
	//2D�|���S���N���X�̏I�������Ăяo��
	CScene2D::Uninit();
}

//=============================================================================
//�G�N�X�v���[�W�����N���X�̍X�V����
//=============================================================================
void CExplosion::Update(void)
{
	//UV���W�p�̕ϐ�
	D3DXVECTOR2 aUVpos[4];

	//�A�j���[�V�����J�E���^�A�b�v
	m_nCounterAnim++;

	//m_nCounterAnim�̗]�肪4�̂Ƃ�
	if (m_nCounterAnim % 4 == 0)
	{
		//�p�^�[���J�E���^�A�b�v
		m_nPatternAnim++;

		//UV���W�̒�`
		aUVpos[0] = D3DXVECTOR2(m_nPatternAnim * 0.125f, 0.0f);
		aUVpos[1] = D3DXVECTOR2(m_nPatternAnim * 0.125f + 0.125f, 0.0f);
		aUVpos[2] = D3DXVECTOR2(m_nPatternAnim * 0.125f, 1.0f);
		aUVpos[3] = D3DXVECTOR2(m_nPatternAnim * 0.125f + 0.125f, 1.0f);

		//2D�|���S���N���X��UV���W�ݒ菈���Ăяo��
		CScene2D::SetUV(&aUVpos[0]);

		if (m_nPatternAnim >= 8)
		{
			//�I�������Ăяo��
			Uninit();
		}
	}
}

//=============================================================================
//�G�N�X�v���[�W�����N���X�̕`�揈��
//=============================================================================
void CExplosion::Draw(void)
{
	//2D�|���S���N���X�̕`�揈���Ăяo��
	CScene2D::Draw();
}
