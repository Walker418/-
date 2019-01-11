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
	// 移動
	void move(
		const Vector3& rest_position,		// バネの静止位置
		float stiffness,					// バネの強さ
		float friction,						// 摩擦力
		float mass							// 質量
	);
};

#endif // !T_P_CAMERA_H_