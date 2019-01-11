#ifndef SCENE_LOAD_GAME_PLAY_H_
#define SCENE_LOAD_GAME_PLAY_H_

#include "IScene.h"

// クラス：ゲームプレイシーンリソースの読み込み
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class SceneLoadGamePlay : public IScene
{
public:
	// コンストラクタ
	SceneLoadGamePlay() = default;
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
	bool	is_loaded_{ false };	// 読み込む判定
	float	end_timer_{ 0.0f };		// 終了タイマー
	bool	is_end_{ false };		// 終了フラグ
};

#endif // !SCENE_LOAD_GAME_PLAY_H_