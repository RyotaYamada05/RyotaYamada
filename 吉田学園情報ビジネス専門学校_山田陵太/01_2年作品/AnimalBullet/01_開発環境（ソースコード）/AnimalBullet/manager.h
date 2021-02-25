//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : 
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//�O���錾
//=============================================================================
class CRenderer;
class CInputKeyboard;
class CSound;
class CTitle;
class CGame;
class CResult;
class CFade;
class CInputJoyStick;

//=============================================================================
//�}�l�[�W���[�N���X
//=============================================================================
class CManager
{
public:
	//=========================================================================
	//�񋓌^��`
	//=========================================================================
	typedef enum
	{ 
		MODE_TYPE_TITLE = 0,	//�^�C�g�����[�h
		MODE_TYPE_GAME,	//�Q�[�����[�h
		MODE_TYPE_RESULT,	//���U���g���[�h
		MODE_TYPE_MAX,
	}MODE_TYPE;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void LoadAll(void);
	void UnLoadAll(void);

	static void SetMode(MODE_TYPE mode);
	static MODE_TYPE GetMode(void);

	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetKeyborad(void);
	static CSound *GetSound(void);
	static CFade *GetFade(void);
	static CInputJoyStick *GetJoyStick(void);

private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static MODE_TYPE m_mode;	//�Q�[�����[�h����p�ϐ�
	static CRenderer *m_pRenderer;	//�����_���[�N���X�̃|�C���^
	static CInputKeyboard *m_pKeyBoard;	//�L�[�{�[�h�N���X�̃|�C���^
	static CSound *m_pSound;	//�T�E���h�N���X�̃|�C���^
	static CTitle *m_pTitle;	//�^�C�g���N���X�̃|�C���^�ϐ�
	static CGame *m_pGame;		//�Q�[���N���X�̃|�C���^�ϐ�
	static CResult *m_pResult;		//���U���g�N���X�̃|�C���^�ϐ�
	static CFade *m_pFade;		//�t�F�[�h�N���X�̃|�C���^�ϐ�
	static CInputJoyStick *m_pJoyStick;	//�W���C�X�e�B�b�N�N���X�̃|�C���^�ϐ�
};

#endif