#ifndef TITLE_CAMERA_H_
#define TITLE_CAMERA_H_

#include "../Actor.h"

// クラス：タイトル画面用カメラ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

class TitleCamera : public Actor
{
public:
	// コンストラクタ
	TitleCamera(IWorld* world, const Vector3& position);

private:
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw() const override;

private:
	// 画面回転用カウンター
	float sin_count_{ 0.0f };

};

#endif // !TITLE_CAMERA_H_