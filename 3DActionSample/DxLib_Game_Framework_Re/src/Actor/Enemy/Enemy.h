#ifndef ENEMY_H_
#define ENEMY_H_

#include "../Actor.h"
#include "../ActorPtr.h"

// クラス：敵キャラ共通
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

class Enemy : public Actor
{
public:
	// コンストラクタ
	Enemy(IWorld* world, const std::string& name, const Vector3& position, float angle = 0.0f, const IBodyPtr& body = std::shared_ptr<DummyBody>());

	// 地面との接触処理
	void intersect_ground();
	// 壁との接触処理
	void intersect_wall();
	// 座標の制限
	void clamp_position();

	// プレイヤーを取得
	ActorPtr get_player();
	// プレイヤーの位置を取得
	Vector3 get_player_position();
	// 目標への角度を取得（符号付き）
	float get_angle_to_target(Vector3 target) const;
	// 目標への角度を取得（符号無し）
	float get_unsigned_angle_to_target(Vector3 target) const;

	// プレイヤーは存在するか
	bool player_exists();
	// プレイヤーは前にいるか
	bool player_in_forward();
	// プレイヤーは攻撃距離内にいるか
	bool player_in_range_distance(float distance);
	// プレイヤーは攻撃できる角度にいるか
	bool player_in_range_angle(float angle);

	// プレイヤーを攻撃できるか
	virtual bool can_attack_player() = 0;

protected:
	// 現在の体力
	int				current_hp_;
	// 怯み累積値
	int				current_wince_{ 0 };
	// 行動待機時間
	float			interval_{ 0.0f };

};

#endif // !ENEMY_H_