#ifndef SCENE_H_
#define SCENE_H_

// 列挙型：シーン種類
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
enum class Scene
{
	None,			// 空白のシーン
	Loading,		// 読み込み
	Title,			// タイトル
	GamePlay,		// ゲームプレイ
};

#endif // !SCENE_H_