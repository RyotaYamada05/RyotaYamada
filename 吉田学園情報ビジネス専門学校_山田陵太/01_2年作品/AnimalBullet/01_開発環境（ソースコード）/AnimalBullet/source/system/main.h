//=============================================================================
//
// ���C������ [main.h]
// Author : 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//=============================================================================
// �w�b�_�t�@�C���̃C���N���[�h
//=============================================================================
#include <windows.h>
#define DIRECTINPUT_VERSION (0x0800)	//�x���Ώ��p�i"dinput.h"�̒��O�ɒ�`�j
#include <dinput.h>
#include "d3dx9.h"
#include "XAudio2.h"

//=============================================================================
// ���C�u�����t�@�C���̃����N
//=============================================================================
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")	//���͏����ɕK�v

//=============================================================================
// �}�N����`
//=============================================================================
#define	WINDOW_POS_X	(0)
#define	WINDOW_POS_Y	(0)
#define SCREEN_WIDTH	(1280)	//�E�B���h�E�̉���
#define SCREEN_HEIGHT	(720)	//�E�B���h�E�̏c��
#define SCREEN_CNTER_POS D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f)	//�E�B���h�E�̒��S�ʒu
#define SCREEN_SIZE D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f)	//�E�B���h�E�̃T�C�Y
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//2D�|���S���̒��_�t�H�[�}�b�g
#define NUM_VERTEX (4)	//���_��
#define NUM_POLYGON (2)	//�|���S����
#define VECTOR3_ZERO_INITIALIZE D3DXVECTOR3(0.0f, 0.0f, 0.0f)	//3D�x�N�g���̃[���������}�N��
//#undef _DEBUG	//_DEBUG�t���O�𖳌�������
//#define _RSTICK_	//�R�����g�A�E�g�F�E�X�e�B�b�N�ړ��̖�����

//=============================================================================
//�F�̃}�N����`
//=============================================================================
#define COLOR_RED D3DCOLOR_RGBA(255,50,50,255)		//�ԐF
#define COLOR_BLUE D3DCOLOR_RGBA(50,50,255,255)		//�F
#define COLOR_CYAN D3DCOLOR_RGBA(50,255,255,255)	//�V�A��
#define COLOR_GREEN D3DCOLOR_RGBA(50,255,50,255)	//�ΐF
#define COLOR_YELLOW D3DCOLOR_RGBA(255,255,50,255)	//���F
#define COLOR_PURPLE D3DCOLOR_RGBA(128,50,128,255)	//��
#define COLOR_ORANGE D3DCOLOR_RGBA(255,165,0,255)	//�I�����W
#define COLOR_BLACK D3DCOLOR_RGBA(0,0,0,255)		//���F
#define COLOR_WHITE D3DCOLOR_RGBA(255,255,255,255)	//���F
#define COLOR_ALPHA D3DCOLOR_RGBA(255,255,255,50)	//�������̔��F

//=============================================================================
// 2D�|���S���̒��_���̍\���̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//���W�ϊ��p�W���i1.0f�ŌŒ�j
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_2D;

#ifdef _DEBUG
//=============================================================================
//�v���g�^�C�v�֐��錾
//=============================================================================
int GetFPS(void);
#endif

#endif