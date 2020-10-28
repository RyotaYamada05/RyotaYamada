//=============================================================================
//
// ���C������ [joystick.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "input.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_JoyStick_NUM 2	//�R���g���[���[�̐ڑ��ő吔
#define CROSS_KEY_UP	0				//�\���L�[��
#define CROSS_KEY_TOP_RIGHT 4500		//�\���L�[�E��
#define CROSS_KEY_RIGHT	9000			//�\���L�[�E
#define CROSS_KEY_BOTTOM_RIGHT 13500	//�\���L�[�E��
#define CROSS_KEY_DOWN	18000			//�\���L�[��
#define CROSS_KEY_BOTTOM_LEFT 22500		//�\���L�[����
#define CROSS_KEY_LEFT	27000			//�\���L�[��
#define CROSS_KEY_TOP_LEFT	31500		//�\���L�[����

//=============================================================================
//�W���C�X�e�B�b�N�N���X
//=============================================================================
class CInputJoyStick :public CInput
{
public:
	//=============================================================================
	//�W���C�X�e�B�b�N�̃{�^���p�̗񋓌^��`
	//=============================================================================
	typedef enum
	{
		JOY_BUTTON_X = 0,		//X�{�^��
		JOY_BUTTON_Y,			//Y�{�^��
		JOY_BUTTON_A,			//A�{�^��
		JOY_BUTTON_B,			//B�{�^��
		JOY_BUTTON_L_TRIGGER,	//L�g���K�[�{�^��
		JOY_BUTTON_R_TRIGGER,	//R�g���K�[�{�^��
		JOY_BUTTON_L_TRIGGER2,	//L�g���K�[�{�^��2
		JOY_BUTTON_R_TRIGGER2,	//R�g���K�[�{�^��2
		JOY_BUTTON_L3,			//L�{�^��
		JOY_BUTTON_R3,			//R�{�^��
		JOY_BUTTON_SELECT,		//SELECT�{�^��
		JOY_BUTTON_START,		//START�{�^��
		JOY_BUTTON_HOME,		//HOME�{�^��
		JOY_BUTTON_MAX,			//�ő�{�^����
	}JOY_BUTTON;

	//=============================================================================
	//�W���C�X�e�B�b�N��Ԕ��ʗp�̍\���̒�`
	//=============================================================================
	typedef struct
	{
		DIJOYSTATE	Old;		//�p�b�h�{�^���̑O����
		DIJOYSTATE	Press;		//�p�b�h�{�^���̃v���X���
		DIJOYSTATE	Trigger;	//�p�b�h�{�^���̃g���K�[���
		DIJOYSTATE	Release;	//�p�b�h�{�^���̃����[�X���
	}JoyStickState;

	//=============================================================================
	//�����o�֐��錾
	//=============================================================================
	CInputJoyStick();
	~CInputJoyStick();

	HRESULT InIt(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetJoyStickPress(int nButton);
	bool GetJoyStickTrigger(int nButton);
	bool GetJoyStickReleas(int nButton);

	bool GetPushCross(int nButtond);
	bool GetPushRelease(int nButtond);

	DIJOYSTATE GetJoyStick(void);
	static BOOL CALLBACK DeviceFindCallBack(const DIDEVICEINSTANCE * pdidInstance, VOID * pContext);
private:
	//=============================================================================
	//�����o�ϐ��錾
	//=============================================================================
	static LPDIRECTINPUTDEVICE8 m_pJoyDevice;	//�W���C�X�e�B�b�N�̃f�o�C�X���
	JoyStickState m_JoyState;	//�W���C�X�e�B�b�N�̏��
};


#endif
