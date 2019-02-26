#include "ActionSample.h"

#include "Graphics3D.h"
#include "SkeletalMesh.h"
#include "CollisionMesh.h"
#include "Skybox.h"
#include "Billboard.h"

#include "Scene.h"
#include "SceneLoadGamePlay.h"
#include "SceneGamePlay.h"

// クラス：3Dアクションゲームサンプル（アプリケーションクラス）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
ActionSample::ActionSample() :
	Game{ 1024, 768, false }
{ }

// 開始
void ActionSample::start()
{
	// シーンクラスの初期化
	scene_manager_.initialize();
	// シーンを読み込む
	scene_manager_.add(Scene::LoadGamePlay, new_scene<SceneLoadGamePlay>());	// ゲームプレイシーンの読み込み
	scene_manager_.add(Scene::GamePlay, new_scene<SceneGamePlay>());			// ゲームプレイシーン

	// 最初のシーンへ移行
	scene_manager_.change(Scene::LoadGamePlay);
}

// 更新
void ActionSample::update(float delta_time)
{
	scene_manager_.update(delta_time);
}

// 描画
void ActionSample::draw()
{
	scene_manager_.draw();
}

// 終了
void ActionSample::end()
{
	// シーンマネージャーを終了
	scene_manager_.end();
}