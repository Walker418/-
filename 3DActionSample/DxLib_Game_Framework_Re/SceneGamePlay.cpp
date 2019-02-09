#include "SceneGamePlay.h"
#include "Scene.h"
#include "SourceID.h"

#include "Field.h"
#include "ActorGroup.h"
#include "EventMessage.h"
#include "TPCamera.h"
#include "Light.h"
#include "Player.h"
#include "DragonBoar.h"

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
	world_.add_camera(new_actor<TPCamera>(&world_));
	// ライトを追加
	world_.add_light(new_actor<Light>(&world_, Vector3{ 0.0f, 30.0f, -20.0f }));

	// アクターはここに追加
	world_.add_actor(ActorGroup::Player, new_actor<Player>(&world_, Vector3{ 0.0f, 0.0f, 100.0f }, Matrix::Identity));		// プレイヤー
	world_.add_actor(ActorGroup::Enemy, new_actor<DragonBoar>(&world_, Vector3{ 0.0f, 0.0f, 0.0f }, Matrix::Identity));		// 敵
}

// 更新
void SceneGamePlay::update(float delta_time)
{
	world_.update(delta_time);

	// プレイヤーが死亡した場合、ゲーム終了
	/*
	auto player = world_.find_actor(ActorGroup::Player, "Player");	// プレイヤーを検索
	if (!player)
	{
		is_end_ = true;
	}
	*/
}

// 描画
void SceneGamePlay::draw() const
{
	world_.draw();

	// デバッグメッセージ
	/*
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);

	auto player = world_.find_actor(ActorGroup::Player, "Player");
	if (!player) return;
	auto player_pos = player->position();
	DrawFormatString(0, 0, Cr, "プレイヤーの座標： %f、%f、%f", player_pos.x, player_pos.y, player_pos.z);
	auto player_forward = player->pose().Forward();
	float player_angle = Vector3::Angle(player_forward, Vector3::Forward);
	DrawFormatString(0, 15, Cr, "プレイヤーの角度： %f", player_angle);
	*/
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
	return Scene::LoadGamePlay;
}

// 終了
void SceneGamePlay::end()
{
	// ワールドをクリア
	world_.clear();

	// 素材を破棄
	SkeletalMesh::erase(MESH_PALADIN);
	SkeletalMesh::erase(MESH_DRAGONBOAR);

	CollisionMesh::erase(MESH_STAGE_CASTLE);
	Skybox::erase(MESH_SKYBOX);
	Billboard::erase(0);
}

// メッセージ処理
void SceneGamePlay::handle_message(EventMessage message, void* param)
{
	// 受け取ったメッセージの種類によって、処理を行う
	switch (message)
	{
	case EventMessage::PlayerDead:	// プレイヤーが死亡した場合
		is_end_ = true;				// シーン終了
		break;
	}
}