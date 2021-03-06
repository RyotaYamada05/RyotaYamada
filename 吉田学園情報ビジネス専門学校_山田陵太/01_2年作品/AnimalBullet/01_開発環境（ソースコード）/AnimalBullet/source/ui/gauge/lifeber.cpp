//=============================================================================
//
// ライフ処理 [lifeber.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "lifeber.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define TEN_PERCENT 10	//10割

//=============================================================================
//ライフクラスのコンストラクタ
//=============================================================================
CLifeber::CLifeber(int nPriority) : CGauge(nPriority)
{
}

//=============================================================================
//ライフクラスのデストラクタ
//=============================================================================
CLifeber::~CLifeber()
{
}

//=============================================================================
//ライフクラスのクリエイト処理
//=============================================================================
CLifeber * CLifeber::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col,
	const int nGaugeNum, const int nMaxGaugeNum)
{
	//ライフクラスのポインタ変数
	CLifeber *m_pLife = NULL;

	//メモリの確保
	m_pLife = new CLifeber;

	//メモリが確保できていたら
	if (m_pLife)
	{
		m_pLife->SetPos(pos);
		m_pLife->SetSize(size);
		m_pLife->SetColor(col);
		m_pLife->SetMaxGauge(nMaxGaugeNum);
		m_pLife->SetGauge(nGaugeNum);
		//初期化処理呼び出し
		m_pLife->Init();
	}
	//メモリ確保に失敗したとき
	else
	{
		return NULL;
	}

	return m_pLife;
}

//=============================================================================
//ライフクラスの初期化処理
//=============================================================================
HRESULT CLifeber::Init(void)
{
	//ゲージクラスの初期化
	CGauge::Init();

	return S_OK;
}

//=============================================================================
//ライフクラスの終了処理
//=============================================================================
void CLifeber::Uninit(void)
{
	//ゲージクラスの終了
	CGauge::Uninit();	
}

//=============================================================================
//ライフクラスの更新処理
//=============================================================================
void CLifeber::Update(void)
{
	//ゲージクラスの更新
	CGauge::Update();

	int nNowLife = GetGauge();
	int nMaxLife = GetMaxGague();

	//最大ライフの一割の値を求める
	int nPercent = nMaxLife / TEN_PERCENT;

	//現在ライフが最大値の2割以下の時
	if (nNowLife <= (nPercent * 2))
	{
		SetColor(COLOR_RED);
	}
	//現在ライフが最大値の5割以下の時
	else if (nNowLife <= (nPercent * 5))
	{
		SetColor(COLOR_ORANGE);
	}
	else
	{
		SetColor(COLOR_GREEN);
	}
}

//=============================================================================
//ライフクラスの描画処理
//=============================================================================
void CLifeber::Draw(void)
{
	//ゲージクラスの描画
	CGauge::Draw();
}
