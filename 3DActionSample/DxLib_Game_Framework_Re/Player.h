#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor.h"

// クラス：プレイヤー
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Player : public Actor
{
public:
	// コンストラクタ
	Player(IWorld* world, const Vector3& position, const Matrix& rotation);


};

#endif // !PLAYER_H_