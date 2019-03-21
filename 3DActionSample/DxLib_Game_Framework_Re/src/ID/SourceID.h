#ifndef SOURCE_ID_H_
#define SOURCE_ID_H_

// 列挙型：素材ID
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
enum
{
	// ステージ
	MESH_STAGE_CASTLE,	// 城フィールド
	MESH_SKYBOX,		// スカイボックス

	// モデル
	MESH_PALADIN,		// プレイヤー
	MESH_GHOUL,			// 雑魚敵
	MESH_DRAGONBOAR,	// ボス敵

	// テクスチャ
	TEXTURE_HPGAUGE,	// 体力ゲージ
	TEXTURE_HP,			// 体力表示
	TEXTURE_P1MESSAGE,	// フェーズ1目標メッセージ
	TEXTURE_P2MESSAGE,	// フェーズ2目標メッセージ
};

#endif // !SOURCE_ID_H_