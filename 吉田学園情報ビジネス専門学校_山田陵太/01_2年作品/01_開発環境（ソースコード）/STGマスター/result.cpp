//=============================================================================
//
// メイン処理 [resulet.h]
// Author : 山田陵太
//
//=============================================================================
#include "result.h"
#include "polygon.h"
#include "manager.h"
#include "keyboard.h"
#include "fade.h"
#include "ranking.h"
#include "sound.h"
#include "joystick.h"

//=============================================================================
//リザルトクラスのコンストラクタ
//=============================================================================
CResult::CResult(int nPriority) : CScene(nPriority)
{
	//メンバ変数のクリア
	m_pPolygon = NULL;
	m_bEnter = false;
}

//=============================================================================
//リザルトクラスのデストラクタ
//=============================================================================
CResult::~CResult()
{
}

//=============================================================================
//リザルトクラスのクリエイト処理
//=============================================================================
CResult * CResult::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//リザルトクラスのポインタ変数
	CResult *pResult = NULL;

	//メモリの確保
	pResult = new CResult;

	//メモリが確保できていたら
	if (pResult != NULL)
	{
		//初期化処理呼び出し
		pResult->Init(pos, size);
	}
	//メモリ確保に失敗したとき
	else
	{
		return NULL;
	}

	return pResult;
}

//=============================================================================
//リザルトクラスの初期化処理
//=============================================================================
HRESULT CResult::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//ポリゴンを生成
	m_pPolygon = CPolygon::Create(pos, size, CPolygon::TEX_TYPE_RESULET);

	CRanking::Create();
	return S_OK;
}

//=============================================================================
//リザルトクラスの終了処理
//=============================================================================
void CResult::Uninit(void)
{
	if (m_pPolygon != NULL)
	{
		//ポリゴンクラスの終了処理呼び出し
		m_pPolygon->Uninit();

		//メモリの削除
		delete m_pPolygon;

		//メモリのクリア
		m_pPolygon = NULL;
	}

	//オブジェクト破棄
	Release();
}

//=============================================================================
//リザルトクラスの更新
//=============================================================================
void CResult::Update(void)
{
	if (m_pPolygon != NULL)
	{
		//ポリゴンクラスの更新処理呼び出し
		m_pPolygon->Update();
	}

	//キーボードクラスの情報取得
	CInputKeyboard *pKeyBoard = CManager::GetKeyborad();
	CInputJoyStick *pJoyStick = CManager::GetJoyStick();
	//ENTERキーが押されたら
	if ((pKeyBoard->GetKeyBoardRelease(DIK_RETURN) || pJoyStick->GetJoyStickTrigger(CInputJoyStick::JOY_BUTTON_B))&& m_bEnter == false)
	{
		m_bEnter = true;

		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ENTER);
		//フェード情報を取得
		CFade *pFade = CManager::GetFade();

		if (pFade != NULL)
		{
			//フェードを行う
			pFade->SetFade(CManager::MODE_TYPE_TITLE);
		}

		return;
	}
}

//=============================================================================
//リザルトクラスの描画処理
//=============================================================================
void CResult::Draw(void)
{
	if (m_pPolygon != NULL)
	{
		//ポリゴンクラスの描画処理呼び出し
		m_pPolygon->Draw();
	}
}
