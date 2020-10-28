//=============================================================================
//
// メイン処理 [scene.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_OBJ_NUM 2000	//最大数

//=============================================================================
//オブジェクトクラス
//=============================================================================
class CScene
{
public:
	//=========================================================================
	//オブジェクトタイプの列挙型
	//=========================================================================
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,	//プレイヤータイプ
		OBJTYPE_BULLET,	//バレットタイプ
		OBJTYPE_ENEMY,	//エネミータイプ
		OBJTYPE_BOSS,
		OBJTYPE_EXPLOSION,	//爆発タイプ
		OBJTYPE_BOME,	//ボムタイプ
		OBJTYPE_ITEM,	//アイテムタイプ
		OBJTYPE_MAX	//オブジェクトタイプの最大数
	}OBJTYPE;	//オブジェクトタイプ

	//=========================================================================
	//描画優先順位の列挙型
	//=========================================================================
	typedef enum
	{
		PRIORITY_BG = 0, 
		PRIORITY_EFFECT,	//エフェクト
		PRIORITY_BULEET,	//バレット
		PRIORITY_DEFAULT,	//初期数
		PRIORITY_ENEMY,	//エネミー
		PRIORITY_PLAYER,	//プレイヤー
		PRIORITY_EXPLOSION,	//爆発
		PRIORITY_UI,	//UI
		PRIORITY_TOPUI,	//TOPUI
		PRIORITY_FADE,
		PRIORITY_MAX	//優先順位の最大数
	}PRIORITY;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CScene(int nPriority = PRIORITY_DEFAULT);
	virtual ~CScene();
	static void ReleaseAll(void);
	static void AllUpdate(void);
	static void AllDraw(void);

	virtual HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	void SetObjType(const OBJTYPE objtype);
	OBJTYPE GetObjType(void)const;
	static CScene *GetScene(const int nPriority, const int nCntObjNum);

protected:
	void Release(void);

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static CScene *m_apScene[PRIORITY_MAX][MAX_OBJ_NUM];	//オブジェクトクラスのポインタ配列
	static int m_nNumAll;	//総数
	int m_nID;	//ナンバーの保存
	int m_nPriority;	//優先順位
	OBJTYPE m_ObjType;	//オブジェクトの種類
};

#endif