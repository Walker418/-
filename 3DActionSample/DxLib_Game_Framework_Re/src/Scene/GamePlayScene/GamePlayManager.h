#ifndef GAME_PLAY_MANAGER_H_
#define GAME_PLAY_MANAGER_H_

#include "../../Actor/Actor.h"
#include "../../Math/Vector2.h"

// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// プレイ状況
enum GamePlayState
{
	Phase1,		// 第1段階（雑魚戦）
	Phase2,		// 第2段階（ボス戦）
};

// クラス：ゲームプレイシーン管理
class GamePlayManager : public Actor
{
public:
	// コンストラクタ
	explicit GamePlayManager(IWorld* world);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// メッセージ処理
	virtual void handle_message(EventMessage message, void* param = nullptr) override;

private:
	// プレイ状況の更新
	void update_phase(float delta_time);
	// ゲーム開始処理
	void game_start();
	// フェーズ移行
	void change_phase();
	// 第1段階の処理
	void phase1(float delta_time);
	// 第2段階の処理
	void phase2(float delta_time);
	// プレイヤーの体力ゲージを表示
	void draw_HP_gauge(Vector2 position) const;
	// 現在の目的を表示
	void draw_message(Vector2 position) const;

private:
	// プレイ状況
	GamePlayState	state_{ GamePlayState::Phase1 };
	// フェーズタイマー
	float			state_timer_{ 0.0f };
	// フェーズ移行タイマー
	float			state_change_time_{ 0.0f };
	// 倒された敵の数（状況移行判定用）
	int				enemy_defeated_{ 0 };
	// 第1段階は終了したか
	bool			phase1_end_{ false };
	// ボスが倒されたか
	bool			boss_defeated_{ false };
	// 第2段階は終了したか
	bool			phase2_end_{ false };
	// プレイヤーが倒れたか
	bool			player_dead_{ false };
	// ゲーム終了フラグ
	bool			game_end_{ false };
};

#endif // !GAME_PLAY_MANAGER_H_