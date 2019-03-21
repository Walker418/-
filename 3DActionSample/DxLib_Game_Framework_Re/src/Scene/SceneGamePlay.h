#ifndef SCENE_GAME_PLAY_H_
#define SCENE_GAME_PLAY_H_

#include "IScene.h"
#include "../World/World.h"
#include "Scene.h"

// クラス：ゲームプレイシーン
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class SceneGamePlay : public IScene
{
public:
	// デフォルトコンストラクタ
	SceneGamePlay() = default;
	// 開始
	virtual void start() override;
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// 終了しているか
	virtual bool is_end() const override;
	// 次のシーンの取得
	virtual Scene next() const override;
	// 終了
	virtual void end() override;
	// メッセージ処理
	void handle_message(EventMessage message, void* param);

private:
	// ワールド
	World	world_;
	// 終了フラグ
	bool	is_end_{ false };
	// 次のシーン（デフォルトはゲームオーバーシーン）
	// Scene	next_scene_{ Scene::GameOver };
	// Scene	next_scene_{ Scene::Title };
	Scene	next_scene_{ Scene::Loading };
};

#endif // !SCENE_GAME_PLAY_H_