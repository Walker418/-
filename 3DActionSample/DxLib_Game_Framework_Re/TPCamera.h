#ifndef T_P_CAMERA_H_
#define T_P_CAMERA_H_

#include "Actor.h"

// クラス：三人称カメラ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class TPCamera : public Actor
{
public:
	// コンストラクタ
	TPCamera(IWorld* world);

private:
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw() const override;

private:
	// カメラの移動、回転処理
	void move(float delta_time);

private:
	Vector3 target_;						// 注視点（プレイヤー）
	float yaw_angle_{ 0.0f };				// y軸角度
	float pitch_angle_{ 0.0f };				// x軸角度

	const float RotateSpeed{ 3.0f };		// カメラの回転速度
	const float CameraHeight{ 22.0f };		// 注視点の高さ
	const float CameraDistance{ 60.0f };	// 注視点との距離
	const float PitchMax{ 15.0f };			// 仰角最大値
	const float PitchMin{ -20.0f };			// 仰角最小値
};

#endif // !T_P_CAMERA_H_