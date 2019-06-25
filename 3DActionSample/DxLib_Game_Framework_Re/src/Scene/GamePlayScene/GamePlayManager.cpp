#include "GamePlayManager.h"
#include "../../World/IWorld.h"
#include "../../Field/Field.h"
#include "../../Actor/ActorGroup.h"
#include "../../Actor/Enemy/Ghoul/Ghoul.h"
#include "../../Actor/Enemy/DragonBoar/DragonBoar.h"
#include "../../ID/EventMessage.h"
#include "../../Graphic/Graphics2D.h"
#include "../../ID/SourceID.h"
#include "../../Math/MathHelper.h"

// クラス：ゲームプレイシーン管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
GamePlayManager::GamePlayManager(IWorld* world) :
	Actor(world, "GamePlayManager"),
	phase_{ GamePlayPhase::Phase1 },
	enemy_defeated_{ 0 },
	boss_defeated_{ false },
	phase2_end_{ false },
	player_dead_{ false },
	game_end_{ false },
	fade_counter_{ 0 }
{
	// ゲーム開始処理を行う
	game_start();
}

// 更新
void GamePlayManager::update(float delta_time)
{
	update_phase(delta_time);	// プレイ状況を更新

	// フェイドイン/フェイドアウト用カウンターの値を制限
	fade_counter_ = (int)MathHelper::clamp((float)fade_counter_, 0.0f, 255.0f);
}

// 描画
void GamePlayManager::draw() const
{
	// 描画輝度をセットし、フェイドイン/フェイドアウト演出をする
	SetDrawBright(fade_counter_, fade_counter_, fade_counter_);

	// プレイヤーキャラの体力を表示
	draw_HP_gauge(Vector2(20.0f, 15.0f));

	// 現在の目的を表示
	draw_message(Vector2(750.0f, 430.0f));
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
	switch (phase_)
	{
	case (GamePlayPhase::Phase1):
		phase1(delta_time);
		break;
	case (GamePlayPhase::Phase2):
		phase2(delta_time);
		break;
	default:
		break;
	}

	// プレイヤーが死亡すると、しばらくしてゲームオーバーメッセージを送る（ゲームオーバー処理を行う）
	if (player_dead_)
	{
		gameover_scene_timer_.update(delta_time);

		if (gameover_scene_timer_.is_time_out())
		{
			world_->send_message(EventMessage::GameOver);
		}
	}
}

// ゲーム開始処理
void GamePlayManager::game_start()
{
	// 各種タイマーをリセット
	phase_change_timer_.reset();
	gameover_scene_timer_.reset();
	gameclear_scene_timer_.reset();

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
	phase_ = GamePlayPhase::Phase2;
}

// 第1段階の処理
void GamePlayManager::phase1(float delta_time)
{
	// ゲーム開始時のフェードイン
	if (fade_counter_ < 255) fade_counter_ += 4;

	// 雑魚敵が全部倒れたの3秒後、ボス戦に移行
	if (phase1_end())
	{
		if (phase_change_timer_.is_time_out())
		{
			change_phase();
			return;
		}

		phase_change_timer_.update(delta_time);
	}
}

// 第2段階の処理
void GamePlayManager::phase2(float delta_time)
{
	if (boss_defeated_)
	{
		if (gameclear_scene_timer_.is_time_out())
		{
			// ゲームクリアメッセージを送る（ゲームクリア処理を行う）
			world_->send_message(EventMessage::StageClear);
			return;
		}

		gameclear_scene_timer_.update(delta_time);
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

// 現在の目的を表示
void GamePlayManager::draw_message(Vector2 position) const
{
	// 進行状況に応じて、現在の目的を表示
	switch (phase_)
	{
	case (GamePlayPhase::Phase1):
		Graphics2D::draw(TEXTURE_P1MESSAGE, position);
		break;
	case (GamePlayPhase::Phase2):
		Graphics2D::draw(TEXTURE_P2MESSAGE, position);
		break;
	default:
		break;
	}
}

// 第1段階は終了しているか
bool GamePlayManager::phase1_end() const
{
	// 雑魚敵が全部倒されたら、Trueを返す
	return enemy_defeated_ >= EnemyPopNo;
}
