#include "GamePlayManager.h"
#include "../../World/IWorld.h"
#include "../../Actor/ActorGroup.h"
#include "../../Actor/Enemy/Ghoul/Ghoul.h"
#include "../../Actor/Enemy/DragonBoar/DragonBoar.h"
#include "../../ID/EventMessage.h"
#include "../../Graphic/Graphics2D.h"
#include "../../ID/SourceID.h"

// クラス：ゲームプレイシーン管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
GamePlayManager::GamePlayManager(IWorld* world) :
	Actor(world, "GamePlayManager"),
	state_{ GamePlayState::Phase1 },
	state_timer_{ 0.0f },
	state_change_time_{ 0.0f },
	enemy_defeated_{ 0 },
	phase1_end_{ false },
	boss_defeated_{ false },
	phase2_end_{ false },
	player_dead_{ false },
	game_end_{ false }
{
	// ゲーム開始処理を行う
	game_start();
}

// 更新
void GamePlayManager::update(float delta_time)
{
	update_phase(delta_time);	// プレイ状況を更新
}

// 描画
void GamePlayManager::draw() const
{
	// プレイヤーキャラの体力を表示
	draw_HP_gauge(Vector2(10.0f, 10.0f));

	// 進行状況に応じて、現在の目的を表示
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);

	switch (state_)
	{
	case (GamePlayState::Phase1):
		DrawFormatString(750, 430, Cr, "グール3体を倒せ！！： %i / 3", enemy_defeated_);
		break;
	case (GamePlayState::Phase2):
		DrawString(750, 430, "巨大モンスターを倒せ！！", Cr);
		break;
	default:
		break;
	}
}

// メッセージ処理
void GamePlayManager::handle_message(EventMessage message, void* param)
{
	// 受け取ったメッセージの種類によって、処理を行う

	switch (message)
	{
	case EventMessage::EnemyDead:	// 雑魚敵が倒された場合
		++enemy_defeated_;			// 倒された敵の数を加算
		break;
	case EventMessage::BossDead:	// ボス敵が倒された場合
		boss_defeated_ = true;
		break;
	case EventMessage::PlayerDead:	// プレイヤーが倒れた場合
		player_dead_ = true;
		break;
	default:
		break;
	}
}

// プレイ状況の更新
void GamePlayManager::update_phase(float delta_time)
{
	switch (state_)
	{
	case (GamePlayState::Phase1):
		phase1(delta_time);
		break;
	case (GamePlayState::Phase2):
		phase2(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;

	// プレイヤーが死亡すると、ゲーム終了フラグを立って、しばらくしてゲームオーバーメッセージを送る
	if (player_dead_)
	{
		if (!game_end_)
		{
			state_change_time_ = state_timer_ + 60.0f;
			game_end_ = true;
		}

		if (state_timer_ >= state_change_time_)
		{
			world_->send_message(EventMessage::GameOver);
		}
	}
}

// ゲーム開始処理
void GamePlayManager::game_start()
{
	// 雑魚敵3体を生成する
	world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ 0.0f, 0.0f, -50.0f }, 180.0f));
	world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ 60.0f, 0.0f, -35.0f }, 160.0f));
	world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ -60.0f, 0.0f, -35.0f }, 200.0f));
}

// フェーズ移行
void GamePlayManager::change_phase()
{
	// ボスを生成し、ボス戦に移行する
	world_->add_actor(ActorGroup::Enemy, new_actor<DragonBoar>(world_, Vector3{ 0.0f, 0.0f, -50.0f }, 180.0f));
	state_ = GamePlayState::Phase2;
	state_timer_ = 0.0f;
}

// 第1段階の処理
void GamePlayManager::phase1(float delta_time)
{
	// 3秒後、ボス戦に移行
	if (enemy_defeated_ >= 3)
	{
		if (!phase1_end_)
		{
			state_change_time_ = state_timer_ + 180.0f;
			phase1_end_ = true;
		}

		if (state_timer_ >= state_change_time_)
		{
			change_phase();
		}
	}
}

// 第2段階の処理
void GamePlayManager::phase2(float delta_time)
{
	if (boss_defeated_)
	{
		if (!phase2_end_)
		{
			state_change_time_ = state_timer_ + 180.0f;
			phase2_end_ = true;
		}

		if (state_timer_ >= state_change_time_)
		{
			// ゲームクリアメッセージを送る
			world_->send_message(EventMessage::StageClear);
		}
	}
}

// プレイヤーの体力ゲージを表示
void GamePlayManager::draw_HP_gauge(Vector2 position) const
{
	Graphics2D::draw(TEXTURE_HPGAUGE, position);	// 体力ゲージを描画

	// プレイヤーの体力を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");
	int player_hp = (player != nullptr) ? player->get_HP() : 0;

	// ゲージの中身を描画
	if (player_hp > 0)
	{
		Vector2 draw_pos = position + Vector2(70.0f, 24.0f);	// 最初の描画位置

		for (int i = 0; i < player_hp; ++i)
		{
			
			Graphics2D::draw(TEXTURE_HP, Vector2(draw_pos.x + 4 * i, draw_pos.y));
		}
	}
}