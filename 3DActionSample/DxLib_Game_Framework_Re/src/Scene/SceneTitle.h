#ifndef SCENE_TITLE_H_
#define SCENE_TITLE_H_

#include "IScene.h"
#include "../World/World.h"

// クラス：タイトルシーン
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

class SceneTitle : public IScene
{
public:
	// デフォルトコンストラクタ
	SceneTitle() = default;
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
	// ワールドクラス
	World	world_;
	// 終了フラグ
	bool	is_end_{ false };
};

#endif // !SCENE_TITLE_H_