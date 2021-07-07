//=============================================================================
//
// エネミーコントロール処理 [enemy_control.cpp]
// Author : 山田陵太
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//警告抑制マクロ
#include "enemy_control.h"
#include <stdio.h>

//=============================================================================
//マクロ定義
//=============================================================================
#define ENEMY_NUM 100	//敵の最大数
#define ENEMY_FILE_PATH "data/Text/enemydata.txt"	//敵情報のファイルパス
#define ENEMY_TEST_FILE_PATH "data/Text/enemy_Test.txt"	//敵情報のファイルパス
//=============================================================================
//エネミーコントロールクラスのコンストラクタ
//=============================================================================
CEnemyControl::CEnemyControl()
{
	m_pEnemyData = NULL;
	m_nSpoonTimer = 0;
	m_nIndex = 0;
	m_nMaxEnemyNum = 0;
}

//=============================================================================
//エネミーコントロールクラスのデストラクタ
//=============================================================================
CEnemyControl::~CEnemyControl()
{
}

//=============================================================================
//エネミーコントロールクラスの初期化処理
//=============================================================================
HRESULT CEnemyControl::Init(void)
{
	//敵情報の読み込み
	if (FAILED(ReadFile())) 
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//エネミーコントロールクラスの終了処理
//=============================================================================
void CEnemyControl::Uninit(void)
{
	if (m_pEnemyData)
	{
		//メモリの削除
		delete[] m_pEnemyData;

		//メモリのクリア
		m_pEnemyData = NULL;
	}
}

//=============================================================================
//エネミーコントロールクラスの更新処理
//=============================================================================
void CEnemyControl::Update(void)
{
	if (m_pEnemyData)
	{
		//現在数が敵の総数より少ない時
		if (m_nIndex < m_nMaxEnemyNum)
		{
			if (m_nSpoonTimer == m_pEnemyData[m_nIndex].nSpoonTime)
			{
				//エネミーを生成
				CEnemy::Create(
					m_pEnemyData[m_nIndex].pos,
					m_pEnemyData[m_nIndex].size,
					m_pEnemyData[m_nIndex].enemyType,
					m_pEnemyData[m_nIndex].nLife,
					m_pEnemyData[m_nIndex].nScore,
					m_pEnemyData[m_nIndex].action,
					m_pEnemyData[m_nIndex].deathAction);

				//タイマーをクリア
				m_nSpoonTimer = 0;

				//インデックスを1つ進める
				m_nIndex++;
				return;
			}
			m_nSpoonTimer++;
		}
	}
}

//=============================================================================
//エネミーコントロールクラスのファイル読み込み処理
//=============================================================================
HRESULT CEnemyControl::ReadFile(void)
{
	FILE *pFile;	//ファイルポインタ
	char aHeadData[1024];
	char aModeName[1024];	//

	m_pEnemyData = new ENEMY_DATA[ENEMY_NUM];

	if (m_pEnemyData)
	{
		//読み込みモードでファイルを開く
		pFile = fopen(ENEMY_FILE_PATH, "r");

		if (pFile)
		{
			do
			{
				//一列読み込んでモード情報を抽出
				fgets(aHeadData, sizeof(aHeadData), pFile);
				sscanf(aHeadData, "%s", aModeName);

				if (strcmp(aModeName, "SET_ENEMY") == 0)
				{
					while (strcmp(aModeName, "END_ENEMY") != 0)
					{
						//一列読み込んでモード情報を抽出
						fgets(aHeadData, sizeof(aHeadData), pFile);
						sscanf(aHeadData, "%s", aModeName);

						if (strcmp(aModeName, "SPOON_TIME") == 0)
						{
							//生成タイムの情報を取得
							sscanf(aHeadData, "%*s %*s %d", 
								&m_pEnemyData[m_nMaxEnemyNum].nSpoonTime);
						}
						if (strcmp(aModeName, "POS") == 0)
						{
							//位置情報を取得
							sscanf(aHeadData, "%*s %*s %f %f %f",
								&m_pEnemyData[m_nMaxEnemyNum].pos.x,
								&m_pEnemyData[m_nMaxEnemyNum].pos.y,
								&m_pEnemyData[m_nMaxEnemyNum].pos.z);
						}
						if (strcmp(aModeName, "SIZE") == 0)
						{
							//位置情報を取得
							sscanf(aHeadData, "%*s %*s %f %f %f",
								&m_pEnemyData[m_nMaxEnemyNum].size.x,
								&m_pEnemyData[m_nMaxEnemyNum].size.y,
								&m_pEnemyData[m_nMaxEnemyNum].size.z);
						}
						if (strcmp(aModeName, "ENEMY_TYPE") == 0)
						{
							//位置情報を取得
							sscanf(aHeadData, "%*s %*s %d",
								&m_pEnemyData[m_nMaxEnemyNum].enemyType);
						}
						if (strcmp(aModeName, "LIFE") == 0)
						{
							//位置情報を取得
							sscanf(aHeadData, "%*s %*s %d",
								&m_pEnemyData[m_nMaxEnemyNum].nLife);
						}
						if (strcmp(aModeName, "SCORE") == 0)
						{
							//位置情報を取得
							sscanf(aHeadData, "%*s %*s %d",
								&m_pEnemyData[m_nMaxEnemyNum].nScore);
						}
						if (strcmp(aModeName, "ACTION") == 0)
						{
							//位置情報を取得
							sscanf(aHeadData, "%*s %*s %d",
								&m_pEnemyData[m_nMaxEnemyNum].action);
						}
						if (strcmp(aModeName, "DEATH_ACTION") == 0)
						{
							//位置情報を取得
							sscanf(aHeadData, "%*s %*s %d",
								&m_pEnemyData[m_nMaxEnemyNum].deathAction);
						}
					}
					//エネミー総数を1つ増やす
					m_nMaxEnemyNum++;
				}

			} while (strcmp(aModeName, "END_SCRIPT") != 0);
		}
		else
		{
			return E_FAIL;
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	return S_OK;
}