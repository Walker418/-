#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include "IScenePtr.h"
#include <unordered_map>

// クラス：シーン管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

enum class Scene;

class SceneManager
{
public:
	// コンストラクタ
	SceneManager();
	// 初期化
	void initialize();
	// 更新
	void update(float delta_time);
	// 描画
	void draw() const;
	// 終了
	void end();
	// シーンの追加
	void add(Scene name, const IScenePtr& scene);
	// シーンの変更
	void change(Scene name);

	// コピー禁止
	SceneManager(const SceneManager& other) = delete;
	SceneManager& operator = (const SceneManager& other) = delete;

private:
	// シーンリスト
	std::unordered_map<Scene, IScenePtr> scenes_;
	// 現在のシーン
	IScenePtr current_scene_;
};

#endif // !SCENE_MANAGER_H_