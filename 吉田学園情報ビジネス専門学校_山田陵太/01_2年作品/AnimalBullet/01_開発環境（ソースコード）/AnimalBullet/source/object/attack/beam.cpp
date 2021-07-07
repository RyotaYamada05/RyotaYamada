//=============================================================================
//
// �r�[������ [beam.h]
// Author : �R�c�ˑ�
//
//=============================================================================
#include "beam.h"
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "boss.h"
#include "explosion.h"
#include "sound.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define TEX_MOVE_RATE 0.05f

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CBeam::m_pTexture = NULL;

//=============================================================================
//�r�[���N���X�̃R���X�g���N�^
//=============================================================================
CBeam::CBeam(int nPriority):CScene(nPriority)
{
	//�e�����o�ϐ��̃N���A
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntLimit = 0;
	m_fTexMove = 0.0f;
}

//=============================================================================
//�r�[���N���X�̃f�X�g���N�^
//=============================================================================
CBeam::~CBeam()
{
}

//=============================================================================
//�r�[���N���X�̃N���G�C�g����
//=============================================================================
CBeam * CBeam::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,
	 const int nLimit)
{
	//�r�[���N���X�̃|�C���^�ϐ�
	CBeam *pBeam = NULL;

	//�C���X�^���X����
	pBeam = new CBeam;

	//���������m�ۂł��Ă�����
	if (pBeam)
	{
		pBeam->SetLimit(nLimit);
		pBeam->m_pos = pos;
		pBeam->m_size = size;
		//�����������Ăяo��
		pBeam->Init();
	}
	else
	{
		return NULL;
	}

	return pBeam;
}

//=============================================================================
//�r�[���N���X�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CBeam::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/Texture/Effect/Beam.png", &m_pTexture);

	return S_OK;
}
//=============================================================================
//�r�[���N���X�̃e�N�X�`���̔j������
//=============================================================================
void CBeam::UnLoad(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//�r�[���N���X�̏���������
//=============================================================================
HRESULT CBeam::Init(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�̍쐬
	pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX,	//�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,	//�i�Œ�j
		FVF_VERTEX_2D,	//�t�H�[�}�b�g
		D3DPOOL_MANAGED,	//(�Œ�)
		&m_pVtxBuff,
		NULL
	);

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + (m_size.y / 2), 0.0f);

	//rhw�̐ݒ�i�l��1.0�ŌŒ�j
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();

	//�r�[�����̍Đ�
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BEAM);

	return S_OK;
}

//=============================================================================
//�r�[���N���X�̏I������
//=============================================================================
void CBeam::Uninit(void)
{
	//�r�[�����̒�~
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_SE_BEAM);

	//���_�o�b�t�@�̊J��
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	SetDeathFlag();
}

//=============================================================================
//�r�[���N���X�̍X�V����
//=============================================================================
void CBeam::Update(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//�v���C���[�̈ʒu�擾
	m_pos = CGame::GetPlayer()->GetPos();

	//�������ԃJ�E���^�[��0�ȉ��ɂȂ�����
	if (m_nCntLimit <= 0)
	{
		Uninit();

		return;
	}
	else if (m_nCntLimit <= 20)
	{
		//�T�C�Y������������
		m_size.y -= 1.0f;
	}

	m_fTexMove += TEX_MOVE_RATE;

	//�����蔻��
	Collision();

	//�������ԃJ�E���^�[�̌���
	m_nCntLimit--;

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�����X�V
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + (m_size.y / 2), 0.0f);

	//�e�N�X�`�����W���X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f - m_fTexMove, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f - m_fTexMove, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f - m_fTexMove, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f - m_fTexMove, 1.0f);

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�r�[���N���X�̕`�揈��
//=============================================================================
void CBeam::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pD3DDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pD3DDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,
		NUM_POLYGON);	//�|���S����

	//�e�N�X�`���̐ݒ�����ɖ߂�
	pD3DDevice->SetTexture(0, NULL);
}

//=============================================================================
//�r�[���N���X�̓����蔻�菈��
//=============================================================================
void CBeam::Collision(void)
{
	//�G�l�~�[�Ɠ����������ǂ��������擾
	CEnemy *pEnemy = (CEnemy*)GetCollision(CScene2D::OBJTYPE_ENEMY);

	if (pEnemy)
	{
		//�G�l�~�[�̈ʒu�擾
		D3DXVECTOR3 enemyPos = pEnemy->GetPos();

		//�_���[�W��^����
		pEnemy->Damage(1);

		pEnemy = NULL;

		//�����̐���
		CExplosion::Create(enemyPos, EXPLOSION_SIZE*2.5f, COLOR_GREEN);
	}

	//�{�X�Ɠ����������ǂ��������擾
	CBoss *pBoss = (CBoss*)GetCollision(CScene2D::OBJTYPE_BOSS);

	if (pBoss)
	{
		//�{�X�̈ʒu�擾
		D3DXVECTOR3 bossPos = pBoss->GetPos();

		//�_���[�W��^����
		pBoss->Damage(1);

		pBoss = NULL;

		//�����̐���
		CExplosion::Create(bossPos, EXPLOSION_SIZE, COLOR_GREEN);
	}

	//�o���b�g�Ɠ����������ǂ��������擾
	CBullet *pBullet = (CBullet*)GetCollision(CScene2D::OBJTYPE_BULLET);

	if (pBullet)
	{
		//�o���b�g�̎g�p�҂��G�̂��̂̂ݔ�������s����
		if (pBullet->GetUser() == CBullet::BULLET_USER_ENEMY)
		{
			//�o���b�g�̈ʒu�擾
			D3DXVECTOR3 bulletPos = pBullet->GetPos();

			//�o���b�g���폜
			pBullet->Uninit();
			pBullet = NULL;

			//�����̐���
			CExplosion::Create(bulletPos, EXPLOSION_SIZE, COLOR_GREEN);
		}
	}
}

//=============================================================================
//�r�[���N���X�̐������Ԃ̐ݒ�̏���
//=============================================================================
void CBeam::SetLimit(const int nLimit)
{
	m_nCntLimit = nLimit;
}

//=============================================================================
//�r�[���N���X�̓�������̎擾�̏���
//=============================================================================
CScene2D *CBeam::GetCollision(OBJTYPE objtype)
{
	for (int nCntPrity = 0; nCntPrity < CScene::PRIORITY_MAX; nCntPrity++)
	{
		//���X�g�̐擪���̎擾
		CScene* pScene = CScene::GetTop(nCntPrity);

		//NULL�ɂȂ�܂ŌJ��Ԃ�
		while (pScene)
		{
			OBJTYPE type = pScene->GetObjType();

			//�I�u�W�F�N�g�^�C�v���ړI�̂��̂������ꍇ
			if (type == objtype)
			{
				//2D�|���S���N���X�|�C���^�ɃL���X�g���A�����擾
				CScene2D *pScene2D = (CScene2D*)pScene;

				//�^�[�Q�b�g�̈ʒu�ƃT�C�Y�����擾
				D3DXVECTOR3 targetPos = pScene2D->GetPos();
				D3DXVECTOR3 targetSize = pScene2D->GetSize();

				//�����蔻��
				if (targetPos.x + (targetSize.x / 2) >= m_pos.x &&
					targetPos.x - (targetSize.x / 2) <= m_pos.x + m_size.x &&
					targetPos.y + (targetSize.y / 2) >= m_pos.y - (m_size.y / 2) &&
					targetPos.y - (targetSize.y / 2) <= m_pos.y + (m_size.y / 2))
				{
					return pScene2D;
				}
			}
			//���̏����擾
			pScene = pScene->GetNext();
		}
	}
	return NULL;
}
