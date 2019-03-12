#ifndef LIGHT_H_
#define LIGHT_H_

#include "../Actor/Actor.h"

// クラス：ライト
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Light : public Actor
{
public:
	// コンストラクタ
	Light(IWorld* world, const Vector3& position);
	// 描画
	virtual void draw() const override;
};

#endif // !LIGHT_H_