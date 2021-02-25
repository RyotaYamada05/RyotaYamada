//=============================================================================
//
// �Q�[������ [game.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
//�O���錾
//=============================================================================
class CUi;
class CPlayer;
class CPolygon;

//=============================================================================
//�Q�[���N���X
//=============================================================================
class CGame : public CScene
{
public:
	//=========================================================================
	//�Q�[����Ԕ���p�񋓌^��`
	//========================================================================
	typedef enum
	{
		GAME_STATE_NOMRAL = 0,	//�ʏ�Q�[��
		GAME_STATE_BOSS_START,	//�{�X��J�n�O����
		GAME_STATE_BOSS_BATTLE,	//�{�X��
		GAME_STATE_GAME_CLEAR,	//�Q�[���N���A����
		GAME_STATE_GAME_OVER,	//�Q�[���I�[�o�[����
		GAME_STATE_MAX,	//�Q�[����Ԃ̍ő吔
	}GAME_STATE;

	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CGame(int nPriority = CScene::PRIORITY_EXPLOSION);
	~CGame();

	static CGame *Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetGameState(const GAME_STATE state);
	static GAME_STATE GetGameState(void);
	static CUi *GetUi(void);
	static CPlayer *GetPlayer(void);

private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	static CUi *m_pUi;	//UI�N���X�̃|�C���^
	static CPlayer *m_pPlayer;	//�v���C���[�N���X�̃|�C���^�ϐ�
	static CPolygon *m_pPolygon;	//�|���S���N���X�̃|�C���^�ϐ�
	static GAME_STATE m_state;	//�Q�[����Ԃ̕ϐ�
	int m_nEnemyCounter;	//�G�l�~�[�����p�J�E���^�[
	int m_nStaeCounter;	//��ԗp�J�E���^�[
};
#endif