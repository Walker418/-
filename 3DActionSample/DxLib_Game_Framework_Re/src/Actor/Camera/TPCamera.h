#ifndef T_P_CAMERA_H_
#define T_P_CAMERA_H_

#include "../Actor.h"
#include "../../Math/CountdownTimer.h"
#include "../../Math/Random.h"

// クラス：三人称カメラ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class TPCamera : public Actor
{
public:
	// コンストラクタ
	TPCamera(IWorld* world);
	// メッセージ処理
	virtual void handle_message(EventMessage message, void* param = nullptr) override;

private:
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw() const override;

private:
	// カメラの移動、回転処理
	void move(float delta_time);
	// 壁との接触処理
	void intersect_wall();
	// カメラの上下振動
	void camera_vibration_V(float delta_time);
	// 振動開始
	void start_vibration();

private:
	const float YawSpeed{ 3.0f };			// カメラのy軸回転速度
	const float PitchSpeed{ 1.5f };			// カメラのy軸回転速度
	const float CameraHeight{ 22.0f };		// 注視点の高さ
	const float CameraDistance{ 60.0f };	// 注視点との距離
	const float PitchMax{ 15.0f };			// 仰角最大値
	const float PitchMin{ -20.0f };			// 仰角最小値
	const float VibrationTime{ 20.0f };		// カメラの振動時間
	const float VibrationPower{ 0.3f };		// カメラの振動力

	Vector3			target_;							// 注視点（プレイヤー）
	float			yaw_angle_{ 0.0f };					// y軸角度
	float			pitch_angle_{ 0.0f };				// x軸角度

	CountdownTimer	vibration_timer_{ VibrationTime };	// カメラ振動タイマー
	float			current_power_{ VibrationPower };	// 現在の振動力
	float			min_pos_y_{ 0.0f };					// 振動時のy軸最小座標
	float			max_pos_y_{ 0.0f };					// 振動時のy軸最大座標
	Random			rand_;								// 乱数生成器
};

#endif // !T_P_CAMERA_H_