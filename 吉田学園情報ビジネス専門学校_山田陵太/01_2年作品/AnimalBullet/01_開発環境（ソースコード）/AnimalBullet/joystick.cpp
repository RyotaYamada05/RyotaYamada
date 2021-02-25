//=============================================================================
//
// �W���C�X�e�B�b�N���� [joystick.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "joystick.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAN_RANGE 1000	//�X�e�B�b�N�̍ő�͈�
#define MIN_RANGE -1000	//�X�e�B�b�N�̍ŏ��͈�

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECTINPUTDEVICE8 CInputJoyStick::m_pJoyDevice = NULL;

//=============================================================================
//�W���C�X�e�B�b�N�N���X�̃R���X�g���N�^
//=============================================================================
CInputJoyStick::CInputJoyStick()
{
	memset(&m_JoyState, 0, sizeof(m_JoyState));
}

//=============================================================================
//�W���C�X�e�B�b�N�N���X�̃f�X�g���N�^
//=============================================================================
CInputJoyStick::~CInputJoyStick()
{
}

//=============================================================================
//�W���C�X�e�B�b�N�N���X�̏���������
//=============================================================================
HRESULT CInputJoyStick::InIt(HINSTANCE hInstance, HWND hWnd)
{
	//���͏����̏�����
	CInput::InIt(hInstance, hWnd);

	//�W���C�X�e�B�b�N�̗�
	if (FAILED(m_pInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,	//�񋓂������f�o�C�X�̎��
		DeviceFindCallBack,		//�f�o�C�X���񋓂����ۂɎ��s�����֐��|�C���^(�R�[���o�b�N�֐�)
		NULL,					//���������s���Ɉ����Ƃ��ēn�����f�[�^�̃|�C���^
		DIEDFL_ATTACHEDONLY)))	//�擾����f�o�C�X�����肷�邩�ǂ����̃t���O
	{
		MessageBox(hWnd, "�W���C�X�e�B�b�N�f�o�C�X�̗񋓂Ɏ��s���܂���", "�x��",MB_OK);

		return E_FAIL;
	}

	//�W���C�X�e�B�b�N��������Ȃ������ꍇ
	if (!m_pJoyDevice)
	{
#ifdef _DEBUG
		MessageBox(hWnd, "�W���C�X�e�B�b�N��������܂���ł���", "�x��", MB_OK);
#endif
		return E_FAIL;
	}

	if (m_pJoyDevice != NULL)
	{
		//�f�[�^�t�H�[�}�b�g�̐ݒ�
		if (FAILED(m_pJoyDevice->SetDataFormat(&c_dfDIJoystick)))
		{
			MessageBox(NULL, "�W���C�X�e�B�b�N�f�o�C�X�̃f�[�^�t�H�[�}�b�g�̐ݒ肪���s���܂���", "�x��", MB_OK);
			m_pJoyDevice->Release();
			return DIENUM_STOP;
		}

		//�������[�h�̐ݒ�
		if (FAILED(m_pJoyDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		{
			MessageBox(NULL, "�W���C�X�e�B�b�N�f�o�C�X�̋������[�h�̐ݒ肪���s���܂���", "�x��", MB_OK);
			m_pJoyDevice->Release();
			return DIENUM_STOP;
		}

		//���͔͈͂̐ݒ�
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(diprg);	//�w�b�_���ێ����Ă���\���̂̃T�C�Y
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);	//DIPROPHEADER�\���̃T�C�Y
		diprg.diph.dwHow = DIPH_BYOFFSET;	//�I�u�W�F�N�g�̉��ߕ��@
		diprg.lMax = MAN_RANGE;	//���̒l�̍ő�͈�
		diprg.lMin = MIN_RANGE;	//���̒l�̍ŏ��͈�

		//X���̐ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		m_pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		//Y���̐ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		m_pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		//Z���̐ݒ�
		diprg.diph.dwObj = DIJOFS_Z;
		m_pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		//RX���̐ݒ�
		diprg.diph.dwObj = DIJOFS_RX;
		m_pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		//RY���̐ݒ�
		diprg.diph.dwObj = DIJOFS_RY;
		m_pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		//RZ���̐ݒ�
		diprg.diph.dwObj = DIJOFS_RZ;
		m_pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		//�N����������
		m_pJoyDevice->Poll();

#ifdef _DEBUG
		MessageBox(NULL, "�W���C�X�e�B�b�N�f�o�C�X�쐬�ɐ���", "��", MB_OK);
#endif 
		//����J�n
		if (m_pJoyDevice == NULL)
		{
			Uninit();
		}

		m_pJoyDevice->Acquire();
	}

	return S_OK;
}

//=============================================================================
//�W���C�X�e�B�b�N�N���X�̏I������
//=============================================================================
void CInputJoyStick::Uninit(void)
{
	//�f�o�C�X����̒�~
	if (m_pJoyDevice != NULL)
	{
		m_pJoyDevice->Unacquire();
		m_pJoyDevice->Release();
		m_pJoyDevice = NULL;
	}

	CInput::Uninit();
}

//=============================================================================
//�W���C�X�e�B�b�N�N���X�̍X�V����
//=============================================================================
void CInputJoyStick::Update(void)
{
	DIJOYSTATE js;
	SecureZeroMemory(&js, sizeof(js));

	if (m_pJoyDevice != NULL)
	{
		//1�t���[���O�̏���ۑ�
		m_JoyState.Old.rgdwPOV[0] = m_JoyState.Press.rgdwPOV[0];

		m_pJoyDevice->Poll();

		if (SUCCEEDED(m_pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE), &js)))
		{
			//�\���L�[�̏���ۑ�
			m_JoyState.Press.rgdwPOV[0] = js.rgdwPOV[0];

			//�{�^�����
			for (int nCntButton = 0; nCntButton < JOY_BUTTON_MAX; nCntButton++)
			{
				//�g���K�[����ۑ�
				m_JoyState.Trigger.rgbButtons[nCntButton] = (m_JoyState.Press.rgbButtons[nCntButton] ^ js.rgbButtons[nCntButton])&js.rgbButtons[nCntButton];

				//�����[�X����ۑ�
				m_JoyState.Release.rgbButtons[nCntButton] = (m_JoyState.Press.rgbButtons[nCntButton] ^ js.rgbButtons[nCntButton])&~js.rgbButtons[nCntButton];

				//�v���X�ۑ�
				m_JoyState.Press.rgbButtons[nCntButton] = js.rgbButtons[nCntButton];
			}
		}
		else
		{
			//���s�����ꍇ�ēx�ڑ������݂�
			m_pJoyDevice->Acquire();
		}
	}
}

//=============================================================================
//�W���C�X�e�B�b�N�N���X�̃v���X���̎擾
//=============================================================================
bool CInputJoyStick::GetJoyStickPress(int nButton)
{
	return (m_JoyState.Press.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
//�W���C�X�e�B�b�N�N���X�̃g���K�[���̎擾
//=============================================================================
bool CInputJoyStick::GetJoyStickTrigger(int nButton)
{
	return (m_JoyState.Trigger.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
//�W���C�X�e�B�b�N�N���X�̃����[�X���̎擾
//=============================================================================
bool CInputJoyStick::GetJoyStickReleas(int nButton)
{
	return (m_JoyState.Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
//�W���C�X�e�B�b�N�N���X�̏\���L�[�������ꂽ�u�Ԃ̔��菈��
//=============================================================================
bool CInputJoyStick::GetPushCross(int nButton)
{
	//�O�񂪉���������Ă��Ȃ����A���݂�������Ă�����
	if (m_JoyState.Old.rgdwPOV[0] == 0xFFFFFFFF && 
		m_JoyState.Press.rgdwPOV[0] == nButton)
	{
		return true;
	}
	return false;
}

//=============================================================================
//�W���C�X�e�B�b�N�N���X�̏\���L�[���������u�Ԃ̔��菈��
//=============================================================================
bool CInputJoyStick::GetPushRelease(int nButtond)
{
	//�O�񂪉�����Ă��邩�A���݂�������Ă��Ȃ�
	if (m_JoyState.Old.rgdwPOV[0] == nButtond && 
		m_JoyState.Press.rgdwPOV[0] == 0xFFFFFFFF)
	{
		return true;
	}
	return false;
}

//=============================================================================
//�W���C�X�e�B�b�N�N���X�̃X�e�B�b�N���̎擾
//=============================================================================
DIJOYSTATE CInputJoyStick::GetJoyStick(void)
{
	DIJOYSTATE js;
	SecureZeroMemory(&js, sizeof(js));

	if (m_pJoyDevice != NULL)
	{
		m_pJoyDevice->Poll();
		m_pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE), &js);
	}

	return js;
}


//=============================================================================
//�W���C�X�e�B�b�N�N���X�̃f�o�C�X�񋓃R�[���o�b�N�֐�
//=============================================================================
BOOL CALLBACK CInputJoyStick::DeviceFindCallBack(const DIDEVICEINSTANCE * pdidInstance, VOID * pContext)
{
	//�W���C�X�e�B�b�N�̍쐬
	if (FAILED(m_pInput->CreateDevice(
		pdidInstance->guidInstance,
		&m_pJoyDevice,
		NULL)))
	{
		MessageBox(NULL, "�W���C�X�e�B�b�N�f�o�C�X�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return DIENUM_STOP;
	}

	//�������I������
	return DIENUM_STOP;
}



