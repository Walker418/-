#ifndef ACTION_SAMPLE_H_
#define ACTION_SAMPLE_H_

#include "Game.h"
#include "SceneManager.h"

// クラス：3Dアクションゲームサンプル（アプリケーションクラス）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class ActionSample : public Game
{
public:
	// コンストラクタ
	ActionSample();

private:
	// 開始
	void start() override;
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw() override;
	// 終了
	void end() override;

private:
	// シーンマネージャー
	SceneManager scene_manager_;
};

#endif // !ACTION_SAMPLE_H_