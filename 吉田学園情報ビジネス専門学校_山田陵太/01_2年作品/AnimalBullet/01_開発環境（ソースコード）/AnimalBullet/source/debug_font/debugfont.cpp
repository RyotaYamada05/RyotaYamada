//=============================================================================
//
// �f�o�b�O�t�H���g���� [debugfont.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//�x���}��
#include "debugfont.h"
//#include <stdarg.h>
#include <stdio.h>
#include "manager.h"
#include "renderer.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPD3DXFONT CDebugFont::m_pFont = NULL;
char CDebugFont::m_aString[DEBUG_LAYER_MAX][STR_LENGTH] = {};

//=============================================================================
//�f�o�b�O���\���N���X�̃R���X�g���N�^
//=============================================================================
CDebugFont::CDebugFont()
{
}

//=============================================================================
//�f�o�b�O���\���N���X�̃f�X�g���N�^
//=============================================================================
CDebugFont::~CDebugFont()
{
}

//=============================================================================
//�f�o�b�O���\���N���X�̏���������
//=============================================================================
void CDebugFont::Init(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(pD3DDevice,	//�f�o�C�X
		18,						//�����̍���
		0,						//������
		0,						//�t�H���g�̑���
		0,						//����
		FALSE,					//�Α̂��ǂ���
		SHIFTJIS_CHARSET,		//�����Z�b�g
		OUT_DEFAULT_PRECIS,		//�o�͐��x
		DEFAULT_QUALITY,		//�o�͕i��
		DEFAULT_PITCH,			//�t�H���g�s�b�`
		"Terminal",				//�t�H���g�̎��
		&m_pFont);				//�t�H���g�|�C���^�ւ̃A�h���X
}

//=============================================================================
//�f�o�b�O���\���N���X�̏I������
//=============================================================================
void CDebugFont::Uninit(void)
{
	if (m_pFont)
	{
		//�t�H���g�̃|�C���^�̉��
		m_pFont->Release();

		//�|�C���^�̃N���A
		m_pFont = NULL;
	}
}

//=============================================================================
//�f�o�b�O���\���N���X�̏��ݒ菈��
//=============================================================================
void CDebugFont::Print(DEBUG_LAYER nLayer, char * pFmt, ...)
{
	char pMoji[256];

	ZeroMemory(pMoji, sizeof(pMoji));

	//���X�g�ϐ�
	va_list list;

	//�ψ������X�g�̏�����
	va_start(list, pFmt);

	vsprintf(pMoji, pFmt, list);

	//���������������
	strcat(m_aString[nLayer], pMoji);

	//���X�g�̏I��
	va_end(list);
}


//=============================================================================
//�f�o�b�O���\���N���X�̕`�揈��
//=============================================================================
void CDebugFont::Draw(void)
{
	RECT rect;

	if (m_pFont)
	{
		for (int nCntLayer = 0; nCntLayer < DEBUG_LAYER_MAX; nCntLayer++)
		{
			if (nCntLayer == 0)
			{
				SetRect(&rect, 0, 18, SCREEN_WIDTH, SCREEN_HEIGHT);

				//�`��͈͂̎擾
				m_pFont->DrawText(NULL, m_aString[nCntLayer], -1, &rect,
					DT_LEFT | DT_CALCRECT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

				OffsetRect(&rect, 0, 0);

				// �e�L�X�g�`��
				m_pFont->DrawText(NULL, m_aString[nCntLayer], -1, &rect,
					DT_LEFT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
			}
			else
			{
				SetRect(&rect, 0, 18, SCREEN_WIDTH, SCREEN_HEIGHT);

				//�`��͈͂̎擾
				m_pFont->DrawText(NULL, m_aString[nCntLayer], -1, &rect,
					DT_RIGHT | DT_CALCRECT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

				OffsetRect(&rect, 0, 0);

				// �e�L�X�g�`��
				m_pFont->DrawText(NULL, m_aString[nCntLayer], -1, &rect,
					DT_RIGHT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
			}
		}
		//���ׂĂ̏���\�����I������烁�������N���A����
		ZeroMemory(m_aString, sizeof(m_aString));
	}
}