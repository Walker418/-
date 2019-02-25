#ifndef RAY_H_
#define RAY_H_

#include "Vector3.h"

// クラス：レイ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Ray
{
public:
	// デフォルトコンストラクタ
	Ray() = default;
	// コンストラクタ
	Ray(const Vector3& position, Vector3& direction) :
		position{ position }, direction{ direction } { }
	// 座標
	Vector3 position{ 0.0f, 0.0f, 0.0f };
	// 方向
	Vector3 direction{ 0.0f, 0.0f, 0.0f };
};

#endif // !RAY_H_