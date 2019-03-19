#include "SceneTitle.h"
#include "Scene.h"
#include "../ID/SourceID.h"

#include "../Field/Field.h"
#include "../Actor/ActorGroup.h"
#include "../Actor/Camera/TitleCamera.h"
#include "../Graphic/Light.h"

// 開始
void SceneTitle::start()
{
	// 終了フラグをFalseにする
	is_end_ = false;
	// ワールドを初期化
	world_.initialize();

	// フィールドを追加
	world_.add_field(new_field<Field>(MESH_STAGE_CASTLE, MESH_SKYBOX));
	// カメラを追加
	world_.add_camera(new_actor<TitleCamera>(&world_, Vector3::Zero));
	// ライトを追加
	world_.add_light(new_actor<Light>(&world_, Vector3{ 0.0f, 30.0f, -20.0f }));
}

// 更新
void SceneTitle::update(float delta_time)
{
	world_.update(delta_time);
}

// 描画
void SceneTitle::draw() const
{
	world_.draw();
}

// 終了しているか
bool SceneTitle::is_end() const
{
	// 終了フラグを返す
	return is_end_;
}

// 次のシーンの取得
Scene SceneTitle::next() const
{
	return Scene::GamePlay;
}

// 終了
void SceneTitle::end()
{
	
}

// メッセージ処理
void SceneTitle::handle_message(EventMessage message, void* param)
{

}