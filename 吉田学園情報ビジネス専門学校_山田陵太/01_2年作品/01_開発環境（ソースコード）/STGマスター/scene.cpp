//=============================================================================
//
// メイン処理 [scene.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "scene.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
CScene *CScene::m_apScene[PRIORITY_MAX][MAX_OBJ_NUM] = {};	//オブジェクトクラスのポインタ変数
int CScene::m_nNumAll = 0;	//オブジェクトの総数カウント変数
//=============================================================================
//オブジェクトクラスのデフォルトコンストラクタ
//=============================================================================
CScene::CScene(int nPriority)
{
	//最大ポリゴン数分繰り返す
	for (int nCntScene = 0; nCntScene < MAX_OBJ_NUM; nCntScene++)
	{
		//メモリが確保されていないとき
		if (m_apScene[nPriority][nCntScene] == NULL)
		{
			//ポインタ情報の取得
			m_apScene[nPriority][nCntScene] = this;

			//オブジェクトタイプの初期設定
			m_apScene[nPriority][nCntScene]->m_ObjType = CScene::OBJTYPE_NONE;

			//作成したインスタンスの番号を保存
			m_nID = nCntScene;

			//優先順位の保存
			m_nPriority = nPriority;

			//総数のカウントアップ
			m_nNumAll++;
			break;
		}
	}
}

//=============================================================================
//オブジェクトクラスのデフォルトデストラクタ
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
//全ての終了処理
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nPriority = 0; nPriority < PRIORITY_MAX; nPriority++)
	{
		//最大ポリゴン数分繰り返す
		for (int nCntScene = 0; nCntScene < MAX_OBJ_NUM; nCntScene++)
		{
			//メモリが確保できていたら
			if (m_apScene[nPriority][nCntScene] != NULL)
			{
				//終了処理呼び出し
				m_apScene[nPriority][nCntScene]->Uninit();
			}
		}
	}
	
}

//=============================================================================
//全ての更新処理
//=============================================================================
void CScene::AllUpdate(void)
{
	for (int nPriority = 0; nPriority < PRIORITY_MAX; nPriority++)
	{
		//最大ポリゴン数分繰り返す
		for (int nCntScene = 0; nCntScene < MAX_OBJ_NUM; nCntScene++)
		{
			//メモリが確保できていたら
			if (m_apScene[nPriority][nCntScene] != NULL)
			{
				//更新処理呼び出し
				m_apScene[nPriority][nCntScene]->Update();
			}
		}
	}
}

//=============================================================================
//全ての描画処理
//=============================================================================
void CScene::AllDraw(void)
{
	for (int nPriority = 0; nPriority < PRIORITY_MAX; nPriority++)
	{
		//最大ポリゴン数分繰り返す
		for (int nCntScene = 0; nCntScene < MAX_OBJ_NUM; nCntScene++)
		{
			//メモリが確保できていたら
			if (m_apScene[nPriority][nCntScene] != NULL)
			{
				//描画処理呼び出し
				m_apScene[nPriority][nCntScene]->Draw();
			}
		}
	}
}

//=============================================================================
//削除処理
//=============================================================================
void CScene::Release(void)
{
	//メモリが確保できていたら
	if (m_apScene[m_nID] != NULL)
	{
 		int nID = m_nID;
		int nPriority = m_nPriority;

		//メモリを削除
		delete m_apScene[nPriority][nID];

		//メモリのクリア
		m_apScene[nPriority][nID] = NULL;

		//総数を減らす
		m_nNumAll--;
	}
}

//=============================================================================
//オブジェクトタイプ設定処理
//=============================================================================
void CScene::SetObjType(const OBJTYPE objtype)
{
	//メモリが確保できていたら
	if (m_apScene[m_nPriority][m_nID] != NULL)
	{
		m_apScene[m_nPriority][m_nID]->m_ObjType = objtype;
	}
}

//=============================================================================
//オブジェクトタイプ情報取得処理
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)const
{
	return m_apScene[m_nPriority][m_nID]->m_ObjType;
}

//=============================================================================
//オブジェクト情報取得処理
//=============================================================================
CScene * CScene::GetScene(const int nPriority, const int nNum)
{
	return m_apScene[nPriority][nNum];
}
