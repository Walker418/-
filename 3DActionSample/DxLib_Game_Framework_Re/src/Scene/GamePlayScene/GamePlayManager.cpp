#include "GamePlayManager.h"
#include "../../World/IWorld.h"
#include "../../Actor/ActorGroup.h"
#include "../../Actor/Enemy/Ghoul/Ghoul.h"
#include "../../Actor/Enemy/DragonBoar/DragonBoar.h"
#include "../../ID/EventMessage.h"

// クラス：ゲームプレイシーン管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
GamePlayManager::GamePlayManager(IWorld* world) :
	Actor(world, "GamePlayManager"),
	state_{ GamePlayState::Phase1 },
	state_change_timer_{ 0.0f },
	enemy_defeated_{ 0 }
{
	// ゲーム開始処理を行う
	game_start();
}

// 更新
void GamePlayManager::update(float delta_time)
{

}

// 描画
void GamePlayManager::draw() const
{
	/*
	// デバッグメッセージ
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	DrawFormatString(0, 15, Cr, "倒した敵の数： %i", enemy_defeated_);
	*/
}

// メッセージ処理
void GamePlayManager::handle_message(EventMessage message, void* param)
{
	if (message == EventMessage::EnemyDead)
	{
		// 倒された敵の数を加算
		++enemy_defeated_;
	}
}

// ゲーム開始処理
void GamePlayManager::game_start()
{
	// 雑魚敵3体を生成する
	// world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ 0.0f, 0.0f, -50.0f }, 180.0f));
	// world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ 60.0f, 0.0f, -35.0f }, 160.0f));
	// world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ -60.0f, 0.0f, -35.0f }, 200.0f));

	// ボス敵を生成する
	world_->add_actor(ActorGroup::Enemy, new_actor<DragonBoar>(world_, Vector3{ 0.0f, 0.0f, -50.0f }, 180.0f));
}