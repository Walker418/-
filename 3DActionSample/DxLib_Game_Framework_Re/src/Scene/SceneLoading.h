#ifndef SCENE_LOAD_GAME_PLAY_H_
#define SCENE_LOAD_GAME_PLAY_H_

#include "IScene.h"
#include "../Math/CountdownTimer.h"

// クラス：ローディングシーン
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class SceneLoading : public IScene
{
private:
	const float SceneChangeTime{ 120.0f };	// 次のシーンに移行するまでの時間

public:
	// コンストラクタ
	SceneLoading() = default;
	// 開始
	virtual void start() override;
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// 終了しているか
	virtual bool is_end() const override;
	// 次のシーンを返す
	virtual Scene next() const override;
	// 終了
	virtual void end() override;

private:
	bool			is_loaded_{ false };			// 読み込む判定
	// float	end_timer_{ 0.0f };		
	CountdownTimer	end_timer_{ SceneChangeTime };	// 終了タイマー
	bool			is_end_{ false };				// 終了フラグ
};

#endif // !SCENE_LOAD_GAME_PLAY_H_