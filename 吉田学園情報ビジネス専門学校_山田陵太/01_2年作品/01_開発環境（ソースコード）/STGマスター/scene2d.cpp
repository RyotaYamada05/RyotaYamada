//=============================================================================
//
// ���C������ [scene2d.cpp]
// Author : �R�c�ˑ�
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//2D�|���S���N���X�̃R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPriority) :CScene(nPriority)
{
	//�e�����o�ϐ��̃N���A
	m_pTexture = NULL;	//�e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;	//���_�o�b�t�@�ւ̃|�C���^
	m_Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�|���S���̃T�C�Y
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�|���S���̈ʒu
	m_fAngle = 0.0f;	//���S�̊p�x
	m_fAddScale = 0.05f;	//�g����Z��
	m_fScale = 1.5f;	//�g��
	m_fLength = 0.0f;	//�Ίp���̒���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����

}

//=============================================================================
//2D�|���S���N���X�̃f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{
}

//=============================================================================
//2D�|���S���N���X�̐�������
//=============================================================================
CScene2D *CScene2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nPriority)
{
	CScene2D *pScene2D;	//2D�|���S���N���X�̃|�C���^�ϐ�
	
	//2D�|���S���N���X�̃C���X�^���X����
	pScene2D = new CScene2D(nPriority);

	//���������m�ۂł��Ă�����
	if (pScene2D != NULL)
	{
		//�����������Ăяo��
		pScene2D->Init(pos,size);
	}
	return pScene2D;
}


//=============================================================================
//2D�|���S���N���X�̏���������
//=============================================================================
HRESULT CScene2D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//�����ʒu�̐ݒ�
	m_pos = pos;

	//�T�C�Y�̐ݒ�
	m_Size = size;

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
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-m_Size.x / 2), m_pos.y + (-m_Size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (-m_Size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);

	//rhw�̐ݒ�i�l��1.0�ŌŒ�j
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//2D�|���S���N���X�̏I������
//=============================================================================
void CScene2D::Uninit(void)
{
	//���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//2D�|���S���N���X�̍X�V����
//=============================================================================
void CScene2D::Update(void)
{
	//�Ίp���̌v�Z
	m_fLength = sqrtf((m_Size.x / 2)*(m_Size.x / 2) + (m_Size.y / 2)*(m_Size.y / 2));

	//�Ίp���̊p�x�̌v�Z
	m_fAngle = atan2f(m_Size.y / 2, m_Size.x / 2);

	// ��]
	m_rot.z += D3DX_PI * 0.01f;
	if (m_rot.z >= D3DX_PI)
	{
		m_rot.z -= D3DX_PI * 2.0f;
	}

	// �g��k��
	m_fScale += m_fAddScale;
	if (m_fScale >= 3.0f || m_fScale <= 1.0f)
	{
		m_fAddScale *= -1;
	}

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
#if 0
	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-POLIGON_SIZE_X / 2), m_pos.y + (-POLIGON_SIZE_Y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (POLIGON_SIZE_X / 2), m_pos.y + (-POLIGON_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-POLIGON_SIZE_X / 2), m_pos.y + (POLIGON_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (POLIGON_SIZE_X / 2), m_pos.y + (POLIGON_SIZE_Y / 2), 0.0f);

#elif 0
	//���_���W�̐ݒ�
	pVtx[0].pos.x = m_pos.x - cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.y = m_pos.y - sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.y = m_pos.y - sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x - cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.y = m_pos.y + sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.y = m_pos.y + sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.z = 0.0f;

#elif 0
	//���_���W�̐ݒ�
	pVtx[0].pos.x = m_pos.x - cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	pVtx[0].pos.y = m_pos.y - sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	pVtx[1].pos.y = m_pos.y - sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x - cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	pVtx[2].pos.y = m_pos.y + sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	pVtx[3].pos.y = m_pos.y + sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	pVtx[3].pos.z = 0.0f;

#endif
	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//2D�|���S���N���X�̕`�揈��
//=============================================================================
void CScene2D::Draw(void)
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
//2D�|���S���N���X�̋�`�̓����蔻�菈��
//=============================================================================
CScene2D *CScene2D::Collision(CScene::OBJTYPE objtype, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	for (int nCntObjNum = 0; nCntObjNum < MAX_OBJ_NUM; nCntObjNum++)
	{
		for (int nCntPrity = 0; nCntPrity < CScene::PRIORITY_MAX; nCntPrity++)
		{
			//�I�u�W�F�N�g�����擾
			CScene *pScene = GetScene(nCntPrity, nCntObjNum);

			//���������m�ۂł��Ă�����
			if (pScene != NULL)
			{
				//�I�u�W�F�N�g�^�C�v���擾
				OBJTYPE targetobjType = pScene->GetObjType();

				//�I�u�W�F�N�g�^�C�v���ړI�̂��̂������ꍇ
				if (targetobjType == objtype)
				{
					//2D�|���S���N���X�|�C���^�ɃL���X�g���A�����擾
					CScene2D *pScene2D = (CScene2D*)pScene;

					//�^�[�Q�b�g�̈ʒu�ƃT�C�Y�����擾
					D3DXVECTOR3 targetPos = pScene2D->GetPos();
					D3DXVECTOR3 targetSize = pScene2D->GetSize();

					//�����蔻��
					if (targetPos.x + (targetSize.x / 2) >= pos.x - (size.x / 2) &&
						targetPos.x - (targetSize.x / 2) <= pos.x + (size.x / 2) &&
						targetPos.y + (targetSize.y / 2) >= pos.y - (size.y / 2) &&
						targetPos.y - (targetSize.y / 2) <= pos.y + (size.y / 2))
					{
						return pScene2D;
					}
				}
			}
		}
	}
	return NULL;
}

//=============================================================================
//2D�|���S���N���X�̈ʒu�ݒ菈��
//=============================================================================
void CScene2D::SetPos(const D3DXVECTOR3 pos)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//�ʒu�̐ݒ�
	m_pos = pos;
	
	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�����X�V
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-m_Size.x / 2), m_pos.y + (-m_Size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (-m_Size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//2D�|���S���N���X�̈ʒu�擾����
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void) const
{
	return m_pos;
}

//=============================================================================
//2D�|���S���N���X�̃T�C�Y�擾����
//=============================================================================
D3DXVECTOR3 CScene2D::GetSize(void) const
{
	return m_Size;
}

//=============================================================================
//2D�|���S���N���X�̃T�C�Y�ݒ菈��
//=============================================================================
void CScene2D::SetSize(const D3DXVECTOR3 size)
{
	m_Size = size;

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�����X�V
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-m_Size.x / 2), m_pos.y + (-m_Size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (-m_Size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//2D�|���S���N���X��UV���W�ݒ菈��
//=============================================================================
void CScene2D::SetUV(const D3DXVECTOR2 *UVpos)
{
	//���_���ւ̃|�C���^
	VERTEX_2D*pVtx;

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = UVpos[0];
	pVtx[1].tex = UVpos[1];
	pVtx[2].tex = UVpos[2];
	pVtx[3].tex = UVpos[3];

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//2D�|���S���N���X�̐F�̐ݒ菈��
//=============================================================================
void CScene2D::SetColor(const D3DXCOLOR col)
{
	//���_���ւ̃|�C���^
	VERTEX_2D*pVtx;

	//���_�f�[�^�����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�F�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//2D�|���S���N���X�̃e�N�X�`�����蓖�ď���
//=============================================================================
void CScene2D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
