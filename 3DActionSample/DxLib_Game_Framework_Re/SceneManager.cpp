#include "SceneManager.h"
#include "SceneNull.h"

// クラス：シーン管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
// 最初は空のシーンを追加
SceneManager::SceneManager() :
	current_scene_{ new_scene<SceneNull>() }
{ }

// 初期化
void SceneManager::initialize()
{
	end();				// 終了時と同じ処理
	scenes_.clear();	// シーンリストをクリア
}

// 更新
void SceneManager::update(float delta_time)
{
	// 現在のシーンを更新
	current_scene_->update(delta_time);

	// 現在のシーンが終了したら、次のシーンへ移行
	if (current_scene_->is_end())
	{
		change(current_scene_->next());
	}
}

// 描画
void SceneManager::draw() const
{
	current_scene_->draw();
}

// 終了
void SceneManager::end()
{
	// 現在のシーンを終了し、空のシーンを置き換える
	current_scene_->end();
	current_scene_ = new_scene<SceneNull>();
}

// シーンの追加
void SceneManager::add(Scene name, const IScenePtr& scene)
{
	scenes_[name] = scene;
}

// シーンの変更
void SceneManager::change(Scene name)
{
	end();								// 現在のシーンを終了
	current_scene_ = scenes_[name];		// 現在のシーンに移行するシーンを登録
	current_scene_->start();			// 次のシーンを開始
}