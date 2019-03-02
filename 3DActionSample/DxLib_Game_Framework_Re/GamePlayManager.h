#ifndef GAME_PLAY_MANAGER_H_
#define GAME_PLAY_MANAGER_H_

#include "Actor.h"

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
	// ゲーム開始処理
	void game_start();

private:
	// プレイ状況
	GamePlayState	state_{ GamePlayState::Phase1 };
	// 状態移行タイマー
	float			state_change_timer_{ 0.0f };
	// 倒された敵の数（状況移行判定用）
	int				enemy_defeated_{ 0 };

	// 生成する雑魚敵の数
	const unsigned int EnemyQuantity{ 3 };
};

#endif // !GAME_PLAY_MANAGER_H_