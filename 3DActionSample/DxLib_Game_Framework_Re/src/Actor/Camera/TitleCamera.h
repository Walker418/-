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
	// カメラの移動、回転
	void rotate(float delta_time);

private:
	Vector3		target_{ Vector3::Zero };	// 注視点
	float		yaw_angle_{ 0.0f };			// y軸角度
	float		pitch_angle_{ -28.0f };		// x軸角度
	
	const float CameraHeight{ 50.0f };		// カメラの高さ
	const float	CameraDistance{ 500.0f };	// カメラの距離
	const float	RotateSpeed{ 0.3f };		// 回転速度
};

#endif // !TITLE_CAMERA_H_