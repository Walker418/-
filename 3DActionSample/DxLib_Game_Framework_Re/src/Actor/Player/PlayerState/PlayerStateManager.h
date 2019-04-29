#ifndef PLAYER_STATE_H_
#define PLAYER_STATE_H_

#include "../../Actor.h"
#include "../../../World/IWorld.h"
#include "PlayerState.h"
#include <memory>

// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

using ActorWeak = std::weak_ptr<Actor>;		// ポインタでプレイヤーの参照を取得

// クラス：プレイヤー状態管理
class PlayerStateManager
{
public:
	// コンストラクタ
	PlayerStateManager(ActorWeak player);
	PlayerStateManager(IWorld* world, ActorWeak player);
	// 仮想デストラクタ
	virtual ~PlayerStateManager() { }
	// 開始
	virtual void start();
	// 更新
	virtual void update(float deltaTime);
	// 終了
	virtual void end();

protected:
	IWorld*		world_{ nullptr };	// ワールドの参照
	ActorWeak	player_;			// プレイヤーの参照6
};

#endif // !PLAYER_STATE_H_