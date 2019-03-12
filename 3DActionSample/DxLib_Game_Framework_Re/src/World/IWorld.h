#ifndef IWORLD_H_
#define IWORLD_H_

#include "../Actor/ActorPtr.h"
#include <string>
#include <functional>

// インターフェース：ワールド
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

enum class ActorGroup;
enum class EventMessage;
class Field;

class IWorld
{
public:
	// 仮想デストラクタ
	virtual ~IWorld() { }
	// アクターの追加
	virtual void add_actor(ActorGroup group, const ActorPtr& actor) = 0;
	// アクターの取得
	virtual ActorPtr find_actor(ActorGroup group, const std::string& name) const = 0;
	// アクター数の取得
	virtual unsigned int count_actor(ActorGroup group) const = 0;
	// アクターリストの巡回
	virtual void each_actor(ActorGroup group, std::function<void(const ActorPtr&)> fn) const = 0;
	// メッセージの送信
	virtual void send_message(EventMessage message, void* param = nullptr) = 0;
	// フィールドの取得
	virtual Field& field() = 0;
	// カメラの取得
	virtual ActorPtr camera() = 0;
};

#endif // !IWORLD_H_