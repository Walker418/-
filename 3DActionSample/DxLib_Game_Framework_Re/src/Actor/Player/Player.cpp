#include "Player.h"
#include "../../Math/MathHelper.h"
#include "../../Math/Quaternion.h"
#include "../../World/IWorld.h"
#include "../../ID/EventMessage.h"
#include "../../Field/Field.h"
#include "../Body/Line.h"
#include "../Body/BoundingSphere.h"
#include "../ActorGroup.h"
#include "PlayerAttack.h"
#include "../Damage.h"
#include "PlayerInput.h"

// クラス：プレイヤー
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
Player::Player(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Actor(world, "Player", position, body),
	mesh_{ MESH_PALADIN, MOTION_IDLE },
	motion_{ MOTION_IDLE },
	state_{ PlayerState::Normal },
	state_timer_{ 0.0f },
	is_ground_{ false },
	is_guard_{ false },
	attack_on_{ false },
	jump_attack_started_{ false },
	invincible_timer_{ 0.0f },
	skip_timer_{ 0.0f }
{
	rotation_ = Matrix::CreateRotationY(angle);
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
}

// 更新
void Player::update(float delta_time)
{
	// 落下処理
	velocity_ += Vector3::Down * Gravity;		// 重力加速度を計算
	position_.y += velocity_.y * delta_time;	// y軸座標を計算
	// 地面との接触処理
	intersect_ground();
	// 壁との接触処理
	intersect_wall();

	// ガード状態をリセット
	is_guard_ = false;

	// プレーヤーの状態を更新
	update_state(delta_time);
	// モーションを変更
	mesh_.change_motion(motion_);
	// メッシュを更新
	mesh_.update(delta_time);
	// 行列を計算
	mesh_.transform(pose());

	// HPが0以下になると、死亡状態に移行
	if (current_hp_ <= 0 && state_ != PlayerState::Death)
	{
		change_state(PlayerState::Death, MOTION_DEATH);
		return;
	}

	// 無敵時間タイマーを更新
	if (invincible_timer_ > 0.0f)	invincible_timer_ -= delta_time;
}

// 描画
void Player::draw() const
{
	mesh_.draw();	// メッシュを描画

	// コライダーを描画（デバッグモードのみ、調整用）
	body_->transform(pose())->draw();
	// 線分で方向を示す（デバッグモードのみ）
	unsigned int Cr;
	Cr = GetColor(255, 0, 0);
	DrawLine3D(position_, position_ + pose().Forward() * 10.0f, Cr);
}

// 衝突リアクション
void Player::react(Actor& other)
{
	// 死亡状態では反応しない
	if (state_ == PlayerState::Death) return;
}

// メッセージ処理
void Player::handle_message(EventMessage message, void* param)
{
	// 無敵時間中、怯みや死亡状態では反応しない
	if (is_invincible() || state_ == PlayerState::Death) return;

	// 敵からダメージを受ける
	if (message == EventMessage::PlayerDamage)
	{
		// メッセージから敵の攻撃を取得
		Damage* damage = (Damage*)param;
		Vector3 atk_pos = damage->position;

		// ガード判定
		if (is_guard_ && can_block(atk_pos))
		{
			// 攻撃判定の位置がプレイヤーより前の場合、ガードが成立する
			change_state(PlayerState::Blocking, MOTION_GUARD_BLOCK);
			return;
		}

		// ガードが成立しない場合、ダメージを計算する
		current_hp_ -= damage->power;						// ダメージ計算
		// スーパーアーマー状態でなければ、怯み状態へ移行
		if (!is_super_armor())
		{
			change_state(PlayerState::Damage, MOTION_IMPACT);
		}
	}
}

// 体力の取得
int Player::get_HP()
{
	return current_hp_;
}

// 状態の更新
void Player::update_state(float delta_time)
{
	// 現在の状態に応じて、プレイヤーを更新
	switch (state_)
	{
	case PlayerState::Normal:
		normal(delta_time);
		break;
	case PlayerState::Slash1:
		slash1(delta_time);
		break;
	case PlayerState::Slash2:
		slash2(delta_time);
		break;
	case PlayerState::Slash3:
		slash3(delta_time);
		break;
	case PlayerState::JumpAttack1:
		jump_attack1(delta_time);
		break;
	case PlayerState::JumpAttack2:
		jump_attack2(delta_time);
		break;
	case PlayerState::Damage:
		damage(delta_time);
		break;
	case PlayerState::Guard:
		guard(delta_time);
		break;
	case PlayerState::Blocking:
		blocking(delta_time);
		break;
	case PlayerState::GuardAttack:
		guard_attack(delta_time);
		break;
	case PlayerState::GuardEnd:
		guard_end(delta_time);
		break;
	case PlayerState::Death:
		death(delta_time);
		break;
	case PlayerState::ForwardEvasion:
		forward_evasion(delta_time);
		break;
	case PlayerState::LeftEvasion:
		left_evasion(delta_time);
		break;
	case PlayerState::RightEvasion:
		right_evasion(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;			// 状態タイマーを加算
}

// 状態の変更
void Player::change_state(PlayerState state, int motion)
{
	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;

	attack_on_ = false;
	jump_attack_started_ = false;

	if (state == PlayerState::Normal)	mesh_.reset_speed();	// 通常状態に戻るとき、必ずモーション速度をリセット
}

// 通常状態での更新
void Player::normal(float delta_time)
{
	// 攻撃、ガードの行動は接地状態でしか移行できない
	if (is_ground_)
	{
		// ジャンプ攻撃状態への移行
		if (PlayerInput::jump_attack())
		{
			mesh_.change_speed(1.5f);	// 次のモーション速度を設定
			change_state(PlayerState::JumpAttack1, MOTION_JUMP_ATTACK);
			return;
		}

		// 攻撃状態への移行
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.6f);	// 次のモーション速度を設定
			change_state(PlayerState::Slash1, MOTION_SLASH_1);
			return;
		}

		// ガード状態への移行
		if (PlayerInput::guard())
		{
			mesh_.change_speed(1.5f);	// 次のモーション速度を設定
			change_state(PlayerState::Guard, MOTION_GUARD_BEGIN);
			return;
		}
	}

	// ============================================================
	// 以下は移動処理
	int motion{ MOTION_IDLE };		// 何もしなければ、待機モーションに変更

	// カメラを取得
	auto camera = world_->camera()->pose();
	// カメラの正面ベクトルを取得
	auto camera_forward = camera.Forward();
	// カメラのy軸成分を無視する
	camera_forward.y = 0;
	// 正規化
	camera_forward.Normalize();

	// x,z軸移動量をリセット
	velocity_.x = 0;
	velocity_.z = 0;
	float forward_speed{ 0.0f };	// 前向き速度
	float left_speed{ 0.0f };		// 左向き速度

	// 移動は接地状態でしかできない
	if (is_ground_)
	{
		// WASDによる移動
		// 前後移動
		if (PlayerInput::move_forward())		// 前
		{
			forward_speed = DashSpeed;
		}
		else if (PlayerInput::move_backward())	// 後
		{
			forward_speed = -DashSpeed;
		}
		// 左右移動
		if (PlayerInput::move_left())			// 左
		{
			left_speed = DashSpeed;
		}
		else if (PlayerInput::move_right())		// 右
		{
			left_speed = -DashSpeed;
		}

		// 左スティックによる移動（WASD操作していなければ有効）
		if (!PlayerInput::keyboard_move())
		{
			forward_speed = DashSpeed * PlayerInput::L_stick_move().y;	// 前後
			left_speed = -DashSpeed * PlayerInput::L_stick_move().x;	// 左右
		}
	}
	// 移動していれば、歩行モーションに変更
	if (forward_speed != 0.0f || left_speed != 0.0f)
		motion = MOTION_DASH;

	// 状態を更新
	change_state(PlayerState::Normal, motion);

	// 移動量を計算し、プレイヤーを移動させる
	velocity_ += camera_forward * forward_speed;	// 前後速度を加算
	velocity_ += camera.Left() * left_speed;		// 左右速度を加算
	position_ += velocity_ * delta_time;			// 次の位置を計算

	// プレイヤーを回転させる
	if (velocity_.x != 0.0f || velocity_.z != 0.0f)		// 移動していれば
	{
		// rotation_ = Matrix::CreateWorld(Vector3::Zero, Vector3(velocity_.x, 0.0f, velocity_.z).Normalize(), Vector3::Up);

		Matrix new_rotation = Matrix::CreateWorld(Vector3::Zero, Vector3(velocity_.x, 0.0f, velocity_.z).Normalize(), Vector3::Up);	// 新しい方向を設定
		rotation_ = Matrix::Lerp(rotation_, new_rotation, RotateSpeed);	// 補間で方向を転換する
	}

	// 移動処理終了
	// ============================================================

	// 回避
	if (PlayerInput::evasion() && is_ground_)
	{
		ready_to_skip();
		change_state(PlayerState::ForwardEvasion, PlayerMotion::MOTION_DASH);
		return;
	}
}

// 攻撃（1段目）での更新
void Player::slash1(float delta_time)
{
	// 攻撃判定を発生
	if (state_timer_ >= 25.0f && !attack_on_)
	{

		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 12.0f + Vector3(0.0f, 9.5f, 0.0f);
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, attack_position, 3, 1));
		mesh_.change_speed(1.4f);	// 以降のモーション速度を少し遅くにする
	}

	// モーション終了の前に、次の攻撃への移行
	if (state_timer_ > 35.0f && state_timer_ <= 45.0f && is_ground_)
	{
		// 攻撃入力されると、攻撃の2段階目に移行
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.2f);
			change_state(PlayerState::Slash2, MOTION_SLASH_2);
			return;
		}
	}

	// モーション終了の前に、回避への移行
	if (state_timer_ > 33.0f && state_timer_ < 53.0f && is_ground_)
	{
		// 方向+回避入力されると、回避状態に移行
		// キーボード操作による入力
		if (!PlayerInput::gamepad_move() && PlayerInput::evasion())
		{
			attack_to_evasion_keyboard();
		}

		// パッド操作による入力
		if (!PlayerInput::keyboard_move() && PlayerInput::evasion())
		{
			attack_to_evasion_pad();
		}
	}

	// モーション終了後、通常状態に戻る
	if (state_timer_ >= 60.0f)
	{
		normal(delta_time);
	}
}

// 攻撃（2段目）での更新
void Player::slash2(float delta_time)
{
	// 攻撃判定を発生
	if (state_timer_ >= 10.0f && !attack_on_)
	{
		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 12.0f + Vector3(0.0f, 9.5f, 0.0f);
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, attack_position, 2, 1));
		mesh_.reset_speed();	// 以降のモーション速度を少し遅くにする
	}

	// モーション終了の前に、次の攻撃への移行
	if (state_timer_ > 31.0f && state_timer_ < 45.0f && is_ground_)
	{
		// 攻撃入力されると、攻撃の3段階目に移行
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.2f);
			change_state(PlayerState::Slash3, MOTION_SLASH_3);
			return;
		}
	}

	// モーション終了の前に、回避への移行
	if (state_timer_ > 20.0f && state_timer_ < 45.0f && is_ground_)
	{
		// 方向+回避入力されると、回避状態に移行
		// キーボード操作による入力
		if (!PlayerInput::gamepad_move() && PlayerInput::evasion())
		{
			attack_to_evasion_keyboard();
		}

		// パッド操作による入力
		if (!PlayerInput::keyboard_move() && PlayerInput::evasion())
		{
			attack_to_evasion_pad();
		}
	}

	// モーション終了後、通常状態に戻る
	if (state_timer_ >= 45.0f)
	{
		normal(delta_time);
	}
}

// 攻撃（3段目）での更新
void Player::slash3(float delta_time)
{
	// 地面に離れたら、通常状態に戻る
	if (!is_ground_)
	{
		normal(delta_time);
	}

	// モーション再生の間、キャラクターを前進させる
	if (state_timer_ <= 69.0f)
	{
		velocity_ = rotation_.Forward() * 0.22f;
		position_ += velocity_ * delta_time;
	}

	// 攻撃判定を発生
	if (state_timer_ >= 43.0f && !attack_on_)
	{
		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 15.0f + Vector3(0.0f, 9.5f, 0.0f);
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, attack_position, 5, 3));
	}

	// モーション終了の前に、回避への移行
	if (state_timer_ > 50.0f && state_timer_ <= mesh_.motion_end_time() + 70.0f && is_ground_)
	{
		// 方向+回避入力されると、回避状態に移行
		// キーボード操作による入力
		if (!PlayerInput::gamepad_move() && PlayerInput::evasion())
		{
			attack_to_evasion_keyboard();
		}

		// パッド操作による入力
		if (!PlayerInput::keyboard_move() && PlayerInput::evasion())
		{
			attack_to_evasion_pad();
		}
	}

	// モーション終了後、通常状態に戻る
	if (state_timer_ >= 72.0f)
	{
		normal(delta_time);
	}
}

// ジャンプ攻撃（1段目）での更新
void Player::jump_attack1(float delta_time)
{
	// モーション再生の間、キャラクターを前進させる
	if (state_timer_ < 45.0f)
	{
		velocity_ = rotation_.Forward() * 0.7f;
		position_ += velocity_ * delta_time;
	}

	// 攻撃判定を発生
	if (state_timer_ >= 45.0f && !attack_on_)
	{
		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 13.0f + Vector3(0.0f, 9.5f, 0.0f);
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, attack_position, 5, 3));
	}

	// モーション終了の前に、次の攻撃への移行
	if (state_timer_ > 50.0f && state_timer_ < 65.0f && is_ground_)
	{
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.2f);
			change_state(PlayerState::JumpAttack2, MOTION_SLASH_2);
			return;
		}
	}

	// モーション終了の前に、回避への移行
	if (state_timer_ > 46.0f && state_timer_ < mesh_.motion_end_time() + 70.0f && is_ground_)
	{
		// 方向+回避入力されると、回避状態に移行
		// キーボード操作による入力
		if (!PlayerInput::gamepad_move() && PlayerInput::evasion())
		{
			attack_to_evasion_keyboard();
		}

		// パッド操作による入力
		if (!PlayerInput::keyboard_move() && PlayerInput::evasion())
		{
			attack_to_evasion_pad();
		}
	}

	// モーション終了後、通常状態に戻る
	if (state_timer_ >= 70.0f)
	{
		normal(delta_time);
	}
}

// ジャンプ攻撃（2段目）での更新
void Player::jump_attack2(float delta_time)
{
	// 攻撃判定を発生
	if (state_timer_ >= 10.0f && !attack_on_)
	{
		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 12.0f + Vector3(0.0f, 9.5f, 0.0f);
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, attack_position, 2, 1));
		mesh_.reset_speed();	// 以降のモーション速度を少し遅くにする
	}

	// モーション終了の前に、次の攻撃への移行
	if (state_timer_ > 25.0f && state_timer_ < 43.0f && is_ground_)
	{
		// 攻撃入力されると、攻撃の1段階目に移行
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.6f);	// 次のモーション速度を設定
			change_state(PlayerState::Slash1, MOTION_SLASH_1);
			return;
		}
	}

	// モーション終了の前に、回避への移行
	if (state_timer_ > 20.0f && state_timer_ < 45.0f && is_ground_)
	{
		// 方向+回避入力されると、回避状態に移行
		// キーボード操作による入力
		if (!PlayerInput::gamepad_move() && PlayerInput::evasion())
		{
			attack_to_evasion_keyboard();
		}

		// パッド操作による入力
		if (!PlayerInput::keyboard_move() && PlayerInput::evasion())
		{
			attack_to_evasion_pad();
		}
	}

	// モーション終了後、通常状態に戻る
	if (state_timer_ >= 45.0f)
	{
		normal(delta_time);
	}
}

// 被弾状態での更新
void Player::damage(float delta_time)
{
	// モーション終了後、通常状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		normal(delta_time);
	}
}

// ガード時の更新
void Player::guard(float delta_time)
{
	// ガード判定の発生はモーションの再生より早い（0.1秒で有効）
	if (state_timer_ >= 6.0f)
	{
		is_guard_ = true;				// ガード判定を有効化
	}

	// ガード攻撃への移行
	if (PlayerInput::attack())
	{
		mesh_.change_speed(1.5f);
		change_state(PlayerState::GuardAttack, MOTION_GUARD_SLASH);
		return;
	}

	// ガード開始のモーション終了後、ガード待機モーションに移行
	if (state_timer_ >= mesh_.motion_end_time())
	{
		mesh_.reset_speed();
		motion_ = MOTION_GUARD_IDLE;
	}

	// Rキーが離れたら、ガード終了状態に移行
	if (PlayerInput::guard_end())
	{
		mesh_.reset_speed();
		change_state(PlayerState::GuardEnd, MOTION_GUARD_END);
	}
}

// ガードによるノックバック中の更新
void Player::blocking(float delta_time)
{
	is_guard_ = true;				// ガード状態を有効化

	// モーション終了後、ガード状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{

		state_ = PlayerState::Guard;
		guard(delta_time);
	}
}

// ガード攻撃での更新
void Player::guard_attack(float delta_time)
{
	// 攻撃判定を発生
	if (state_timer_ >= 35.0f && !attack_on_)
	{
		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 15.0f + Vector3(0.0f, 9.5f, 0.0f);
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, attack_position, 3, 1));
	}

	// モーション終了の前に、次の攻撃への移行
	if (state_timer_ > 45.0f && state_timer_ < 60.0f)
	{
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.6f);	// 次のモーション速度を設定
			change_state(PlayerState::Slash1, MOTION_SLASH_1);
			return;
		}
	}

	// モーション終了の前に、回避への移行
	if (state_timer_ > 45.0f && state_timer_ < 60.0f && is_ground_)
	{
		// 方向+回避入力されると、回避状態に移行
		// キーボード操作による入力
		if (!PlayerInput::gamepad_move() && PlayerInput::evasion())
		{
			attack_to_evasion_keyboard();
		}

		// パッド操作による入力
		if (!PlayerInput::keyboard_move() && PlayerInput::evasion())
		{
			attack_to_evasion_pad();
		}
	}

	// 通常状態への移行
	if (state_timer_ >= mesh_.motion_end_time() + 15.0f)
	{
		normal(delta_time);
	}
}

// ガード終了時の更新
void Player::guard_end(float delta_time)
{
	// モーション終了後、通常状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() + 4.0f)
	{
		normal(delta_time);
	}
}

// 死亡状態での更新
void Player::death(float delta_time)
{
	// モーションが終了すると、死亡判定を有効に
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		world_->send_message(EventMessage::PlayerDead);
		die();
	}
}

// 前回避状態での更新
void Player::forward_evasion(float delta_time)
{
	// 地面に離れたら、通常状態に戻る
	if (!is_ground_)
	{
		invincible_timer_ = 0.0f;	// 無敵時間を強制終了
		normal(delta_time);
	}

	// プレイヤーの座標を移動
	velocity_ = rotation_.Forward() * 1.2f;
	position_ += velocity_ * delta_time;

	// 回避終了後、通常状態に戻る
	if (skip_timer_ <= 0.0f)
	{
		normal(delta_time);
	}

	// 回避タイマーの更新
	skip_timer_ -= delta_time;
}

// 左回避状態での更新
void Player::left_evasion(float delta_time)
{
	// 地面に離れたら、通常状態に戻る
	if (!is_ground_)
	{
		invincible_timer_ = 0.0f;	// 無敵時間を強制終了
		normal(delta_time);
	}

	// プレイヤーの座標を移動
	velocity_ = rotation_.Left() * 1.2f;
	position_ += velocity_ * delta_time;

	// 回避終了後、通常状態に戻る
	if (skip_timer_ <= 0.0f)
	{
		normal(delta_time);
	}

	// 回避タイマーの更新
	skip_timer_ -= delta_time;
}

// 右回避状態での更新
void Player::right_evasion(float delta_time)
{
	// 地面に離れたら、通常状態に戻る
	if (!is_ground_)
	{
		invincible_timer_ = 0.0f;	// 無敵時間を強制終了
		normal(delta_time);
	}

	// プレイヤーの座標を移動
	velocity_ = rotation_.Right() * 1.2f;
	position_ += velocity_ * delta_time;

	// 回避終了後、通常状態に戻る
	if (skip_timer_ <= 0.0f)
	{
		normal(delta_time);
	}

	// 回避タイマーの更新
	skip_timer_ -= delta_time;
}

// 攻撃後の回避行動移行（ゲームパッド）
void Player::attack_to_evasion_pad()
{
	// プレイヤーの方向入力を取得
	auto input = PlayerInput::L_stick_move();

	// カメラを取得
	auto camera = world_->camera()->pose();
	// カメラの正面ベクトルを取得
	auto camera_forward = camera.Forward();
	// カメラのy軸成分を無視する
	camera_forward.y = 0;
	// 正規化
	camera_forward.Normalize();

	// 入力した方向を、カメラからのベクトルに変換
	Vector3 direction = Vector3::Zero;
	direction += camera_forward * input.y;
	direction += camera.Left() * -input.x;
	direction.Normalize();

	// 入力した方向とプレイヤーの方向ベクトルの差が少なかったら、回避行動に移る
	// 左回避
	if (Vector3::Angle(rotation_.Left(), direction) <= 45.0f)
	{
		ready_to_skip();
		change_state(PlayerState::LeftEvasion, PlayerMotion::MOTION_STRAFE_LEFT);
		return;
	}
	// 右回避
	else if (Vector3::Angle(rotation_.Right(), direction) <= 45.0f)
	{
		ready_to_skip();
		change_state(PlayerState::RightEvasion, PlayerMotion::MOTION_STRAFE_RIGHT);
		return;
	}
	// 前回避
	else
	{
		ready_to_skip();
		change_state(PlayerState::ForwardEvasion, PlayerMotion::MOTION_DASH);
		return;
	}
}

// 攻撃後の回避行動移行（キーボード）
void Player::attack_to_evasion_keyboard()
{
	// 左回避
	if (PlayerInput::move_left())
	{
		ready_to_skip();
		change_state(PlayerState::LeftEvasion, PlayerMotion::MOTION_STRAFE_LEFT);
		return;
	}
	// 右回避
	else if (PlayerInput::move_right())
	{
		ready_to_skip();
		change_state(PlayerState::RightEvasion, PlayerMotion::MOTION_STRAFE_RIGHT);
		return;
	}
	// 前回避
	else
	{
		ready_to_skip();
		change_state(PlayerState::ForwardEvasion, PlayerMotion::MOTION_DASH);
		return;
	}
}

// 地面との接触処理
void Player::intersect_ground()
{
	// フィールドを取得
	auto& field = world_->field();
	// 地面との接触点
	Vector3 intersect;
	// 接触判定用線分
	Line line = Line(position_ + Vector3(0.0f, 10.0f, 0.0f), position_ - Vector3(0.0f, 1.0f, 0.0f));

	// 地面との接触点を取得
	if (field.collide_line(line.start, line.end, &intersect))
	{
		// 接地した場合、y軸座標を補正する（地面にめり込まない）
		if (intersect.y >= position_.y)
		{
			velocity_.y = 0;			// y軸移動量を0にする
			position_.y = intersect.y;	// y軸位置を補正
			is_ground_ = true;			// 接地判定をtrueにする
		}
	}
	else
	{
		is_ground_ = false;				// 接地判定をfalseにする（接地していない）
	}
}

// 壁との接触処理
void Player::intersect_wall()
{
	// フィールドを取得
	auto& field = world_->field();
	// 壁との接触点
	Vector3 intersect;
	// 接触判定用球体
	BoundingSphere sphere = BoundingSphere(position_ + Vector3(0.0f, 8.5f, 0.0f), 4.5f);

	// 壁との接触点を取得
	if (field.collide_sphere(sphere.position(), sphere.radius(), &intersect))
	{
		// プレイヤーの座標を補正
		position_.x = intersect.x;
		position_.z = intersect.z;
	}
}

// 回避準備
void Player::ready_to_skip()
{
	// モーションの再生速度を引き上げる
	mesh_.change_speed(1.5f);
	// 回避時間と無敵時間を設定
	skip_timer_ = 30.0f;			// 回避時間：0.5秒
	invincible_timer_ = 12.0f;		// 無敵時間：0.2秒
}

// ガードは成立するか
bool Player::can_block(Vector3 atk_pos)
{
	// 内積で攻撃判定のある方向を判定
	Vector3 to_attack = atk_pos - position_;									// 攻撃判定方向のベクトル
	float forward_dot_target = Vector3::Dot(rotation_.Forward(), to_attack);	// 前方向と攻撃判定の内積

	// 前方向とプレイヤーの内積が0以上であれば、Trueを返す
	return (forward_dot_target >= 0.0f);
}

// 無敵時間内であるか
bool Player::is_invincible()
{
	return (invincible_timer_ > 0.0f || state_ == PlayerState::Damage);
}

// スーパーアーマー状態であるか
bool Player::is_super_armor()
{
	return state_ == PlayerState::Slash3;
}