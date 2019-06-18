#include "ActionSample.h"

#include "../Graphic/Graphics3D.h"
#include "../Graphic/SkeletalMesh.h"
#include "../Math/Collision/CollisionMesh.h"
#include "../Field/Skybox.h"
#include "../Graphic/Billboard.h"

#include "../Scene/Scene.h"
#include "../Scene/SceneLoading.h"
#include "../Scene/SceneTitle.h"
#include "../Scene/SceneGamePlay.h"

#include "WindowSetting.h"

// クラス：3Dアクションゲームサンプル（アプリケーションクラス）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
ActionSample::ActionSample() :
	Game{ WindowSetting::WindowWidth, WindowSetting::WindowHeight, false }
{ }

// 開始
void ActionSample::start()
{
	// シーンクラスの初期化
	scene_manager_.initialize();
	// シーンを読み込む
	scene_manager_.add(Scene::Loading, new_scene<SceneLoading>());		// 読み込み
	scene_manager_.add(Scene::Title, new_scene<SceneTitle>());			// タイトル
	scene_manager_.add(Scene::GamePlay, new_scene<SceneGamePlay>());	// ゲームプレイシーン

	// 最初のシーンへ移行
	scene_manager_.change(Scene::Loading);
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