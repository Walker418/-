#ifndef GAME_PLAY_MANAGER_H_
#define GAME_PLAY_MANAGER_H_

#include "../../Actor/Actor.h"
#include "../../Math/Vector2.h"
#include "../../Math/CountdownTimer.h"

// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// プレイ状況
enum GamePlayPhase
{
	Phase1,		// 第1段階（雑魚戦）
	Phase2,		// 第2段階（ボス戦）
};

// クラス：ゲームプレイシーン管理
class GamePlayManager : public Actor
{
private:
	const float PhaseChangeTime{ 180.0f };				// 第1段階終了後、第2段階に移行するまでのフレーム数
	const float SceneChangeTime_GameOver{ 120.0f };		// ゲームオーバー後、次のシーンに移行するまでのフレーム数
	const float SceneChangeTime_GameClear{ 180.0f };	// ゲームクリア後、次のシーンに移行するまでのフレーム数

	const int	EnemyPopNo{ 3 };						// 生成する雑魚敵の数

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
	void draw_HP_gauge() const;
	// 現在の目的を表示
	void draw_message() const;

	// 第1段階は終了しているか
	bool phase1_end() const;
	// 第1段階のスキップ
	void skip_phase1();

private:
	// プレイ状況
	GamePlayPhase	phase_{ GamePlayPhase::Phase1 };
	// フェーズ移行タイマー
	CountdownTimer	phase_change_timer_{ PhaseChangeTime };
	// ゲームオーバー後シーン移行タイマー
	CountdownTimer	gameover_scene_timer_{ SceneChangeTime_GameOver };
	// ゲームクリア後シーン移行タイマー
	CountdownTimer	gameclear_scene_timer_{ SceneChangeTime_GameClear };
	// 倒された敵の数（状況移行判定用）
	int				enemy_defeated_{ 0 };
	// ボスが倒されたか
	bool			boss_defeated_{ false };
	// 第2段階は終了したか
	bool			phase2_end_{ false };
	// プレイヤーが倒れたか
	bool			player_dead_{ false };
	// ゲーム終了フラグ
	bool			game_end_{ false };

	int fade_counter_{ 0 };		// フェードイン/フェードアウト用カウンター
};

#endif // !GAME_PLAY_MANAGER_H_