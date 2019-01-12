#include "SceneGamePlay.h"
#include "Scene.h"
#include "SourceID.h"

#include "Field.h"
#include "ActorGroup.h"
#include "EventMessage.h"
#include "FreeCamera.h"
#include "Light.h"
#include "Player.h"

#include "SkeletalMesh.h"
#include "CollisionMesh.h"
#include "Skybox.h"
#include "Billboard.h"

// クラス：ゲームプレイシーン
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// 開始
void SceneGamePlay::start()
{
	// 終了フラグをFalseにする
	is_end_ = false;

	// 初期化
	world_.initialize();

	// イベントメッセージリスナーを登録
	world_.add_event_message_listener([=](EventMessage message, void* param) {handle_message(message, param); });

	// フィールドを追加
	world_.add_field(new_field<Field>(MESH_STAGE_CASTLE, MESH_SKYBOX));
	// カメラを追加
	world_.add_camera(new_actor<FreeCamera>(&world_, Vector3{ -70.0f, 20.0f,-70.0f }, Matrix::Identity));
	// ライトを追加
	world_.add_light(new_actor<Light>(&world_, Vector3{ 0.0f, 30.0f, -20.0f }));

	// アクターはここに追加
	world_.add_actor(ActorGroup::Player, new_actor<Player>(&world_, Vector3::Zero, Matrix::Identity));
}

// 更新
void SceneGamePlay::update(float delta_time)
{
	world_.update(delta_time);
}

// 描画
void SceneGamePlay::draw() const
{
	world_.draw();
}

// 終了しているか
bool SceneGamePlay::is_end() const
{
	// 終了フラグを返す
	return is_end_;
}

// 次のシーンの取得
Scene SceneGamePlay::next() const
{
	return Scene::None;
}

// 終了
void SceneGamePlay::end()
{
	// ワールドをクリア
	world_.clear();

	// 素材を破棄
	SkeletalMesh::erase(MESH_PALADIN);

	CollisionMesh::erase(MESH_STAGE_CASTLE);
	Skybox::erase(MESH_SKYBOX);
	Billboard::erase(0);
}

// メッセージ処理
void SceneGamePlay::handle_message(EventMessage message, void* param)
{
	
}