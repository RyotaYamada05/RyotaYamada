//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define FADE_RATE (0.02f)	//���l�ϓ��W��

//=============================================================================
//�t�F�[�h�N���X�̃R���X�g���N�^
//=============================================================================
CFade::CFade()
{
	m_pPolygon = NULL;
	m_FadeMode = FADE_MODE_NONE;
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_modeNext = CManager::MODE_TYPE_TITLE;
}

//=============================================================================
//�t�F�[�h�N���X�̃f�X�g���N�^
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
//�t�F�[�h�N���X�̃N���G�C�g����
//=============================================================================
CFade * CFade::Create(void)
{
	//�t�F�[�h�N���X�̃|�C���^�ϐ�
	CFade *pFade = NULL;

	//���������m��
	pFade = new CFade;

	//���������m�ۂł��Ă�����
	if (pFade)
	{
		//�����������Ăяo��
		pFade->Init();
	}
	//�������m�ۂɎ��s�����Ƃ�
	else
	{
		return NULL;
	}

	return pFade;
}

//=============================================================================
//�t�F�[�h�N���X�̏���������
//=============================================================================
HRESULT CFade::Init(void)
{
	m_FadeMode = FADE_MODE_OUT;

	//�|���S���N���X�𐶐�
	m_pPolygon = CPolygon::Create(SCREEN_CNTER_POS, SCREEN_SIZE, CPolygon::TEX_TYPE_NORE);
	
	//�F��ݒ�
	m_pPolygon->SetColor(m_colorFade);

	return S_OK;
}

//=============================================================================
//�t�F�[�h�N���X�̏I������
//=============================================================================
void CFade::Uninit(void)
{
	if (m_pPolygon)
	{
		//�|���S���N���X�̏I�������Ăяo��
		m_pPolygon->Uninit();

		//�������̔j��
		delete m_pPolygon;

		//�������̃N���A
		m_pPolygon = NULL;
	}
}

//=============================================================================
//�t�F�[�h�N���X�̍X�V����
//=============================================================================
void CFade::Update(void)
{
	if (m_pPolygon)
	{
		if (m_FadeMode != FADE_MODE_NONE)
		{
			//�t�F�[�h�C��(���])����
			if (m_FadeMode == FADE_MODE_IN)
			{
				//���l�̌��Z
				m_colorFade.a -= FADE_RATE;

				//���l��0.0f��菬�����Ȃ�����t�F�[�h�X�V�I��
				if (m_colorFade.a <= MIN_ALPHA)
				{
					//�ŏ��l�ɏC��
					m_colorFade.a = MIN_ALPHA;
					m_FadeMode = FADE_MODE_NONE;
				}
			}

			//�t�F�[�h�A�E�g(�Ó])����
			else if (m_FadeMode == FADE_MODE_OUT)
			{
				//���l�̉��Z
				m_colorFade.a += FADE_RATE;

				//���l��1.0f���傫���Ȃ�����t�F�[�h�C���ֈڍs
				if (m_colorFade.a >= MAX_ALPHA)
				{
					//�ő�l�ɏC��
					m_colorFade.a = MAX_ALPHA;
					m_FadeMode = FADE_MODE_IN;

					//�Q�[�����[�h���w�肳�ꂽ���̂ֈڍs
					CManager::SetMode(m_modeNext);
				}
			}
			//�����x�̐ݒ�
			m_pPolygon->SetColor(m_colorFade);
		}
	}
}

//=============================================================================
//�t�F�[�h�N���X�̕`�揈��
//=============================================================================
void CFade::Draw(void)
{
	if (m_pPolygon)
	{
		if (m_FadeMode != FADE_MODE_NONE)
		{
			//�|���S���N���X�̕`�揈���Ăяo��
			m_pPolygon->Draw();
		}
	}
}

//=============================================================================
//�t�F�[�h�N���X�̐ݒ菈��
//=============================================================================
void CFade::SetFade(CManager::MODE_TYPE mode)
{
	//�t�F�[�h�A�E�g��Ԃɐݒ�
	m_FadeMode = FADE_MODE_OUT;

	//���̃��[�h����ۑ�
	m_modeNext = mode;
}
