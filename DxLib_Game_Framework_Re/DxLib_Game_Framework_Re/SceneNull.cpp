#include "SceneNull.h"
#include "Scene.h"

// クラス：空白のシーン
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// 開始
void SceneNull::start()
{
	// 何もしない
}

// 更新
void SceneNull::update(float delta_time)
{
	// 何もしない
}

// 描画
void SceneNull::draw() const
{
	// 何もしない
}

// 終了しているか
bool SceneNull::is_end() const
{
	// Falseを返す
	return false;
}

// 次のシーンの取得
Scene SceneNull::next() const
{
	// 空白のシーンを返す（シーン遷移はない）
	return Scene::None;
}

// 終了
void SceneNull::end()
{
	// 何もしない
}