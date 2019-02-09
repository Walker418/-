#include "World.h"
#include "Field.h"
#include "ActorGroup.h"
#include "Actor.h"

// クラス：ワールド
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
World::World()
{
	// 初期化処理を行う
	initialize();
}

// 初期化
void World::initialize()
{
	// 全てのデータを消去
	clear();
	// アクターグループをアクターグループマネージャーに追加
	actors_.add_group(ActorGroup::None);			// 所属無し
	actors_.add_group(ActorGroup::Player);			// プレイヤー
	actors_.add_group(ActorGroup::PlayerAttack);	// プレイヤーの攻撃
	actors_.add_group(ActorGroup::Enemy);			// 敵
	actors_.add_group(ActorGroup::EnemyAttack);		// 敵の攻撃
	actors_.add_group(ActorGroup::Effect);			// エフェクト
}

// 更新
void World::update(float delta_time)
{
	// 各アクターの状態を更新
	actors_.update(delta_time);
	// 接触判定を行う
	actors_.collide(ActorGroup::Player, ActorGroup::Enemy);
	actors_.collide(ActorGroup::Player, ActorGroup::EnemyAttack);
	actors_.collide(ActorGroup::PlayerAttack, ActorGroup::Enemy);
	// 死亡したアクターを削除
	actors_.remove();

	// カメラの状態を更新
	camera_->update(delta_time);
	// ライトの状態を更新
	light_->update(delta_time);
}

// 描画
void World::draw() const
{
	camera_->draw();
	light_->draw();
	field_->draw();
	actors_.draw();
}

// メッセージ処理
void World::handle_message(EventMessage message, void* param)
{
	// ワールドのメッセージ処理
	listener_(message, param);

	// アクターのメッセージ処理
	actors_.handle_message(message, param);
	camera_->handle_message(message, param);
	light_->handle_message(message, param);
}

// メッセージリスナーの登録
void World::add_event_message_listener(EventMessageListener listener)
{
	listener_ = listener;
}

// フィールドの追加
void World::add_field(const FieldPtr& field)
{
	field_ = field;
}

// カメラの追加
void World::add_camera(const ActorPtr& camera)
{
	camera_ = camera;
}

// ライトの追加
void World::add_light(const ActorPtr& light)
{
	light_ = light;
}

// 全データの消去
void World::clear()
{
	actors_.clear();
	field_ = nullptr;
	light_ = nullptr;
	camera_ = nullptr;
	listener_ = [](EventMessage, void*) {};
}

// アクターを追加
void World::add_actor(ActorGroup group, const ActorPtr& actor)
{
	actors_.add_actor(group, actor);
}

// アクターの取得
ActorPtr World::find_actor(ActorGroup group, const std::string& name) const
{
	return actors_.find(group, name);
}

// アクター数の取得
unsigned int World::count_actor(ActorGroup group) const
{
	return actors_.count(group);
}

// アクターリストの巡回
void World::each_actor(ActorGroup group, std::function<void(const ActorPtr&)> fn) const
{
	actors_.each(group, fn);
}

// メッセージの送信
void World::send_message(EventMessage message, void * param)
{
	handle_message(message, param);
}

// フィールドの取得
Field& World::field()
{
	return *field_;
}

// カメラの取得
ActorPtr World::camera()
{
	return camera_;
}
