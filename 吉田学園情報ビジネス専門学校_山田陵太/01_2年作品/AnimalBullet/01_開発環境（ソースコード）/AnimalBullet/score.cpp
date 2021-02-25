//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "score.h"
#include "number.h"
#include "scene2d.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
int CScore::m_nScore = 0;

//=============================================================================
//スコアクラスのコンストラクタ
//=============================================================================
CScore::CScore(int nPriority) :CScene(nPriority)
{
	//各メンバ変数のクリア
	memset(m_apNumber, 0, sizeof(m_apNumber));
}

//=============================================================================
//スコアクラスのデストラクタ
//=============================================================================
CScore::~CScore()
{
}

//=============================================================================
//スコアクラスのクリエイト処理
//=============================================================================
CScore * CScore::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//スコアクラスのポインタ変数
	CScore *pScore = NULL;

	//メモリの確保
	pScore = new CScore;

	//メモリが確保できていたら
	if (pScore != NULL)
	{
		//初期化処理呼び出し
		pScore->Init(pos, size);
	}
	//メモリ確保に失敗した時
	else
	{
		return NULL;
	}

	return pScore;
}

//=============================================================================
//スコアクラスの初期化処理
//=============================================================================
HRESULT CScore::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_nScore = 0;

	for (int nCount = 0; nCount < SCORE_MAX_NUM; nCount++)
	{
		//ナンバークラスをSCORE_MAX_NUM分生成
		m_apNumber[nCount] = CNumber::Create(0, CNumber::NUMBER_TYPE_000,	//表示する数字と種類
			D3DXVECTOR3((SCREEN_WIDTH - NUMBER_SIZE_X * SCORE_MAX_NUM) + ((nCount * NUMBER_SIZE_X)+ (NUMBER_SIZE_X / 2)),		//x軸の位置
				(NUMBER_SIZE_Y / 2), //yの位置
				0.0f),	//zの位置
			NUMBER_SIZE, COLOR_CYAN);	//サイズと色
	}
	return S_OK;
}

//=============================================================================
//スコアクラスの終了処理
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCount = 0; nCount < SCORE_MAX_NUM; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			//ナンバークラスの終了処理呼び出し
			m_apNumber[nCount]->Uninit();

			//メモリの削除
			delete m_apNumber[nCount];

			//メモリのクリア
			m_apNumber[nCount] = NULL;
		}
	}

	//オブジェクトの破棄
	SetDeathFlag();
}

//=============================================================================
//スコアクラスの更新処理
//=============================================================================
void CScore::Update(void)
{
}

//=============================================================================
//スコアクラスの描画処理
//=============================================================================
void CScore::Draw(void)
{
	for (int nCount = 0; nCount < SCORE_MAX_NUM; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			//ナンバークラスの描画処理呼び出し
			m_apNumber[nCount]->Draw();
		}
	}
}

//=============================================================================
//スコアクラスのスコア加算処理
//=============================================================================
void CScore::AddScore(const int nScore)
{
	//スコアの加算
	m_nScore += nScore;
	float fIndex;  //指数　(スコア表示用)
	float fRadix = 10.0f; //基数　(スコア表示用)

	for (fIndex = 0; fIndex < SCORE_MAX_NUM; fIndex++)
	{
		if (m_apNumber[(int)fIndex] != NULL)
		{
			//各桁の値を求める
			int nScore = (int)powf(fRadix, SCORE_MAX_NUM - fIndex);
			int nScore2 = (int)powf(fRadix, SCORE_MAX_NUM - fIndex - 1);
			int nAnswer = (m_nScore % nScore) / nScore2;

			//ナンバークラスの更新処理呼び出し
			m_apNumber[(int)fIndex]->Update();

			//数字の設定
			m_apNumber[(int)fIndex]->SetNumber(nAnswer);
		}
	}
}

//=============================================================================
//スコアクラスのスコア取得処理
//=============================================================================
int CScore::GetScore(void) 
{
	return m_nScore;
}
