#ifndef FREE_CAMERA_H_
#define FREE_CAMERA_H_

#include "../Actor.h"

// クラス：カメラ（自由に動かせる）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class FreeCamera : public Actor
{
public:
	// コンストラクタ
	FreeCamera(IWorld* world, const Vector3& position, const Matrix& rotation);

private:
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw() const override;
};

#endif // !FREE_CAMERA_H_