#include "Light.h"

// クラス：ライト
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
Light::Light(IWorld* world, const Vector3& position) :
	Actor(world, "Light", position)
{ }

// 描画
void Light::draw() const
{

}