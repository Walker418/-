#ifndef SCENE_NULL_H_
#define SCENE_NULL_H_

#include "IScene.h"

// クラス：空白のシーン
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class SceneNull : public IScene
{
public:
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
};

#endif // !SCENE_NULL_H_