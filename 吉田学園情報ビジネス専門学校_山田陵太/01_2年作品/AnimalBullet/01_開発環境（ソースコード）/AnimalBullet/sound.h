//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//�T�E���h�N���X
//=============================================================================
class CSound
{
public:
	//=========================================================================
	//�񋓌^��`
	//=========================================================================
	typedef enum
	{
		SOUND_LABEL_SE_SHOUT = 0,	//���ˉ�
		SOUND_LABEL_SE_EXEPLOSION,	//������
		SOUND_LABEL_SE_ENTER,	//�G���^�[�L�[
		SOUND_LABEL_SE_BEAM,	//�r�[����
		SOUND_LABEL_SE_GUARD,	//�K�[�h��
		SOUND_LABEL_SE_WARNING,	//�x����
		SOUND_LABEL_SE_GAME_CLEAR,	//�Q�[���N���ASE
		SOUND_LABEL_SE_GAME_OVER,	//�Q�[���I�[�o�[SE
		SOUND_LABEL_BGM_TITLE,	//�^�C�g��BGM
		SOUND_LABEL_BGM_GAME,	//�Q�[��BGM
		SOUND_LABEL_BGM_BOSS,	//�{�X��pBGM
		SOUND_LABEL_MAX	//���x���ő吔
	}SOUND_LABEL;

	//=========================================================================
	//�T�E���h�̃��[�v���邩�ǂ����̗񋓌^
	//=========================================================================
	typedef enum
	{
		SOUND_LOOP_ON = -1,	//���[�v����
		SOUND_LOOP_OFF,		//���[�v���Ȃ�
	}SOUND_LOOP;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);

private:
	//=========================================================================
	//�\���̒�`
	//=========================================================================
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	}PARAM;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];

	static PARAM m_aParam[SOUND_LABEL_MAX];
};

#endif 