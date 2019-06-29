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
#include "../AttackParameter.h"
#include "../Damage.h"
#include "PlayerInput.h"
#include "../../Sound/Sound.h"

// クラス：プレイヤー
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// ============================================================
// 以下は各モーション処理関連のフレーム数

const float FrameInOneSecond = 60.0f;		// 1秒 = 60フレーム

const float Atk1_Active = 25.0f;			// 攻撃1段目の判定発生フレーム数
const float Atk1_InputValid = 35.0f;		// 攻撃1段目の次の行動入力の開始フレーム数
const float Atk1_InputInvalid = 45.0f;		// 攻撃1段目の次の行動入力の終了フレーム数
const float Atk1_End = 60.0f;				// 攻撃1段目のモーション終了フレーム数

const float Atk2_Active = 10.0f;			// 攻撃2段目の判定発生フレーム数
const float Atk2_InputValid = 31.0f;		// 攻撃2段目の次の行動入力の開始フレーム数
const float Atk2_InputInvalid = 45.0f;		// 攻撃2段目の次の行動入力の終了フレーム数
const float Atk2_End = 45.0f;				// 攻撃2段目のモーション終了フレーム数

const float Atk3_MoveTime = 69.0f;			// 攻撃3段目の移動持続フレーム数
const float Atk3_MoveSpeed = 0.22f;			// 攻撃3段目の移動速度
const float Atk3_Active = 43.0f;			// 攻撃3段目の判定発生フレーム数
const float Atk3_InputValid = 50.0f;		// 攻撃3段目の次の行動入力の開始フレーム数
const float Atk3_InputInvalid = 72.0f;		// 攻撃3段目の次の行動入力の終了フレーム数
const float Atk3_End = 72.0f;				// 攻撃3段目のモーション終了フレーム数

const float JumpAtk1_MoveTime = 45.0f;		// ジャンプ攻撃1段目の移動持続フレーム数
const float JumpAtk1_MoveSpeed = 0.7f;		// ジャンプ攻撃1段目の移動速度
const float JumpAtk1_Active = 45.0f;		// ジャンプ攻撃1段目の判定発生フレーム数
const float JumpAtk1_InputValid = 50.0f;	// ジャンプ攻撃1段目の次の行動入力の開始フレーム数
const float JumpAtk1_InputInvalid = 65.0f;	// ジャンプ攻撃1段目の次の行動入力の終了フレーム数
const float JumpAtk1_End = 70.0f;			// ジャンプ攻撃1段目のモーション終了フレーム数

const float JumpAtk2_Active = 10.0f;		// ジャンプ攻撃2段目の判定発生フレーム数
const float JumpAtk2_InputValid = 25.0f;	// ジャンプ攻撃2段目の次の行動入力の開始フレーム数
const float JumpAtk2_InputInvalid = 43.0f;	// ジャンプ攻撃2段目の次の行動入力の終了フレーム数
const float JumpAtk2_End = 45.0f;			// ジャンプ攻撃2段目のモーション終了フレーム数

const float Damage_End = 31.0f;				// 被弾モーションの終了フレーム数
const float Block_End = 34.0f;				// ガードによるノックバックモーションの終了フレーム数

const float GuardAtk_Active = 35.0f;		// ガード攻撃の判定発生フレーム数
const float GuardAtk_InputValid = 45.0f;	// ガード攻撃の次の行動入力の開始フレーム数
const float GuardAtk_InputInvalid = 60.0f;	// ガード攻撃の次の行動入力の終了フレーム数
const float GuardAtk_End = 60.0f;			// ガード攻撃のモーション終了フレーム数

// ============================================================

// コンストラクタ
Player::Player(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Actor(world, "Player", position, body),
	mesh_{ MESH_PALADIN, MOTION_IDLE },
	motion_{ MOTION_IDLE },
	state_{ PlayerState::Normal },
	is_ground_{ false },
	is_guard_{ false },
	attack_on_{ false },
	jump_attack_started_{ false },
	is_defeated_{ false }
{
	rotation_ = Matrix::CreateRotationY(angle);
	velocity_ = Vector3::Zero;
	current_hp_ = PlayerParameter::HP;

	state_timer_.reset();
	invincible_timer_.shut();
	evasion_timer_.shut();
	hit_stop_timer_.shut();
}

// 更新
void Player::update(float delta_time)
{
	// HPが0以下になると、死亡状態に移行
	if (current_hp_ <= 0 && state_ != PlayerState::Death)
	{
		change_state(PlayerState::Death, MOTION_DEATH);
		return;
	}

	// 無敵時間タイマーを更新
	invincible_timer_.update(delta_time);

	// ヒットストップタイマーを更新
	hit_stop_timer_.update(delta_time);
	// ヒットストップしていれば、以降の更新はしない
	if (is_hit_stop()) return;

	// 落下処理
	velocity_ += Vector3::Down * PlayerParameter::Gravity;	// 重力加速度を計算
	position_.y += velocity_.y * delta_time;				// y軸座標を計算

	intersect_ground();		// 地面との接触処理
	intersect_wall();		// 壁との接触処理
	clamp_position();		// 座標制限

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
}

// 描画
void Player::draw() const
{
	mesh_.draw();	// メッシュを描画

	// コライダーを描画（デバッグモードのみ、調整用）
	body_->transform(pose())->draw();
}

// 衝突リアクション
void Player::react(Actor& other)
{
	// 死亡状態では反応しない
	if (state_ == PlayerState::Death) return;
	// 無敵状態では反応しない
	if (is_invincible()) return;
}

// メッセージ処理
void Player::handle_message(EventMessage message, void* param)
{
	// 無敵時間中、怯みや死亡状態では反応しない
	if (is_invincible() || state_ == PlayerState::Death) return;

	// 敵からダメージを受ける
	if (message == EventMessage::PlayerDamage)
	{
		// カメラを振動させる
		world_->send_message(EventMessage::Camera_Vibration);

		// メッセージから敵の攻撃を取得
		Damage* damage = (Damage*)param;
		Vector3 atk_pos = damage->position;

		// ガード判定
		if (is_guard_ && can_block(atk_pos))
		{
			// 攻撃判定の位置がプレイヤーより前の場合、ガードが成立する
			Sound::play_se(SE_BLOCK);	// SEを鳴らす
			change_state(PlayerState::Blocking, MOTION_GUARD_BLOCK);
			return;
		}

		// ガードが成立しない場合、ダメージを計算する
		current_hp_ -= damage->power;	// ダメージ計算
		// スーパーアーマー状態でなければ、怯み状態へ移行
		if (!is_super_armor())
		{
			change_state(PlayerState::Damage, MOTION_IMPACT);
		}
	}

	// ヒットストップ発生
	if (message == EventMessage::HitStop)
	{
		float* hit_stop_time = (float*)param;
		reset_hit_stop(*hit_stop_time);
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
		mesh_.reset_speed();
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

	state_timer_.update(delta_time);		// 状態タイマーを加算
}

// 状態の変更
void Player::change_state(PlayerState state, int motion)
{
	motion_ = motion;
	state_ = state;
	state_timer_.reset();

	attack_on_ = false;
	jump_attack_started_ = false;
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
		// 移動入力を取得
		forward_speed = PlayerParameter::DashSpeed * PlayerInput::move_input().y;	// 前後
		left_speed = -PlayerParameter::DashSpeed * PlayerInput::move_input().x;		// 左右
	}
	// 移動していれば、歩行モーションに変更
	if (forward_speed != 0.0f || left_speed != 0.0f)
		motion = MOTION_DASH;

	// 状態を更新
	change_state(PlayerState::Normal, motion);

	// 移動量を計算し、プレイヤーを移動させる
	velocity_ += camera_forward * forward_speed;	// 前後速度を加算
	velocity_ += camera.Left() * left_speed;		// 左右速度を加算
	velocity_.Normalize();							// 移動速度を正規化
	position_ += velocity_ * delta_time;			// 次の位置を計算

	// プレイヤーを回転させる
	if (velocity_.x != 0.0f || velocity_.z != 0.0f)		// 移動していれば
	{
		Matrix new_rotation = Matrix::CreateWorld(Vector3::Zero, Vector3(velocity_.x, 0.0f, velocity_.z).Normalize(), Vector3::Up);	// 新しい方向を設定
		rotation_ = Matrix::Lerp(rotation_, new_rotation, PlayerParameter::RotateSpeed);	// 補間で方向を転換する
	}

	// 移動処理終了
	// ============================================================

	// 回避
	if (PlayerInput::evasion() && is_ground_)
	{
		ready_for_evasion();
		change_state(PlayerState::ForwardEvasion, PlayerMotion::MOTION_DASH);
		return;
	}
}

// 攻撃（1段目）での更新
void Player::slash1(float delta_time)
{
	// 攻撃判定を発生
	if (state_timer_.get_time() >= Atk1_Active && !attack_on_)
	{
		attack_on_ = true;	// 攻撃判定は1回しか発生しない

		// 攻撃パラメータ構造体を生成
		const float distance = 12.0f;						// 攻撃判定の発生距離（前方からどれぐらい）
		const float height = 9.5f;							// 攻撃判定の高さ
		Vector3 atk_pos = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);	// 攻撃判定の位置
		int		power = PlayerParameter::Power_Atk1;		// 威力
		int		wince = PlayerParameter::Wince_Atk1;		// 怯み値
		float	hit_stop = PlayerParameter::HitStop_Short;	// ヒットストップ
		AttackParameter atk_para{ atk_pos, power, wince, hit_stop };
		// 攻撃判定を生成
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, atk_para));
		Sound::play_se(SE_SLASH);	// SEを再生

		mesh_.change_speed(1.4f);	// 以降のモーション速度を少し遅くにする
	}

	// モーション終了の前に、次の攻撃や回避への移行
	if (state_timer_.get_time() > Atk1_InputValid && state_timer_.get_time() <= Atk1_InputInvalid && is_ground_)
	{
		// 攻撃の2段階目への移行
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.2f);
			change_state(PlayerState::Slash2, MOTION_SLASH_2);
			return;
		}

		// 回避への移行
		if (PlayerInput::evasion())
		{
			attack_to_evasion();
		}
	}

	// モーション終了後、通常状態に戻る
	if (state_timer_.get_time() >= Atk1_End)
	{
		normal(delta_time);
	}
}

// 攻撃（2段目）での更新
void Player::slash2(float delta_time)
{
	// 攻撃判定を発生
	if (state_timer_.get_time() >= Atk2_Active && !attack_on_)
	{
		attack_on_ = true;	// 攻撃判定は1回しか発生しない

		// 攻撃パラメータ構造体を生成
		const float distance = 12.0f;						// 攻撃判定の発生距離（前方からどれぐらい）
		const float height = 9.5f;							// 攻撃判定の高さ
		Vector3 atk_pos = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);	// 攻撃判定の位置
		int		power = PlayerParameter::Power_Atk2;		// 威力
		int		wince = PlayerParameter::Wince_Atk2;		// 怯み値
		float	hit_stop = PlayerParameter::HitStop_Short;	// ヒットストップ
		AttackParameter atk_para{ atk_pos, power, wince, hit_stop };
		// 攻撃判定を生成
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, atk_para));
		Sound::play_se(SE_SLASH);	// SEを再生

		mesh_.reset_speed();		// 以降のモーション速度を少し遅くにする
	}

	// モーション終了の前に、次の攻撃や回避への移行
	if (state_timer_.get_time() > Atk2_InputValid && state_timer_.get_time() < Atk2_InputInvalid && is_ground_)
	{
		// 攻撃の3段階目への移行
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.2f);
			change_state(PlayerState::Slash3, MOTION_SLASH_3);
			return;
		}

		// 回避への移行
		if (PlayerInput::evasion())
		{
			attack_to_evasion();
		}
	}

	// モーション終了後、通常状態に戻る
	if (state_timer_.get_time() >= Atk2_End)
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
	if (state_timer_.get_time() <= Atk3_MoveTime)
	{
		velocity_ = rotation_.Forward() * Atk3_MoveSpeed;
		position_ += velocity_ * delta_time;
	}

	// 攻撃判定を発生
	if (state_timer_.get_time() >= Atk3_Active && !attack_on_)
	{
		attack_on_ = true;	// 攻撃判定は1回しか発生しない

		// 攻撃パラメータ構造体を生成
		const float distance = 15.0f;						// 攻撃判定の発生距離（前方からどれぐらい）
		const float height = 9.5f;							// 攻撃判定の高さ
		Vector3 atk_pos = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);	// 攻撃判定の位置
		int		power = PlayerParameter::Power_Atk3;		// 威力
		int		wince = PlayerParameter::Wince_Atk3;		// 怯み値
		float	hit_stop = PlayerParameter::HitStop_Long;	// ヒットストップ
		AttackParameter atk_para{ atk_pos, power, wince, hit_stop };
		// 攻撃判定を生成
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, atk_para));
		Sound::play_se(SE_SLASH);	// SEを再生
	}

	// モーション終了の前に、回避への移行
	if (state_timer_.get_time() > Atk3_InputValid && state_timer_.get_time() < Atk3_InputInvalid && is_ground_)
	{
		// 方向+回避入力されると、回避状態に移行
		if (PlayerInput::evasion())
		{
			attack_to_evasion();
		}
	}

	// モーション終了後、通常状態に戻る
	if (state_timer_.get_time() >= Atk3_End)
	{
		normal(delta_time);
	}
}

// ジャンプ攻撃（1段目）での更新
void Player::jump_attack1(float delta_time)
{
	// モーション再生の間、キャラクターを前進させる
	if (state_timer_.get_time() < JumpAtk1_MoveTime)
	{
		velocity_ = rotation_.Forward() * JumpAtk1_MoveSpeed;
		position_ += velocity_ * delta_time;
	}

	// 攻撃判定を発生
	if (state_timer_.get_time() >= JumpAtk1_Active && !attack_on_)
	{
		attack_on_ = true;	// 攻撃判定は1回しか発生しない

		// 攻撃パラメータ構造体を生成
		const float distance = 13.0f;						// 攻撃判定の発生距離（前方からどれぐらい）
		const float height = 9.5f;							// 攻撃判定の高さ
		Vector3 atk_pos = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);	// 攻撃判定の位置
		int		power = PlayerParameter::Power_JumpAtk1;	// 威力
		int		wince = PlayerParameter::Wince_JumpAtk1;	// 怯み値
		float	hit_stop = PlayerParameter::HitStop_Long;	// ヒットストップ
		AttackParameter atk_para{ atk_pos, power, wince, hit_stop };
		// 攻撃判定を生成
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, atk_para));
		Sound::play_se(SE_SLASH);	// SEを再生
	}

	// モーション終了の前に、次の攻撃や回避への移行
	if (state_timer_.get_time() > JumpAtk1_InputValid && state_timer_.get_time() < JumpAtk1_InputInvalid && is_ground_)
	{
		// ジャンプ攻撃の2段階への移行
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.2f);
			change_state(PlayerState::JumpAttack2, MOTION_SLASH_2);
			return;
		}

		// 回避への移行
		if (PlayerInput::evasion())
		{
			attack_to_evasion();
		}
	}

	// モーション終了後、通常状態に戻る
	if (state_timer_.get_time() >= JumpAtk1_End)
	{
		normal(delta_time);
	}
}

// ジャンプ攻撃（2段目）での更新
void Player::jump_attack2(float delta_time)
{
	// 攻撃判定を発生
	if (state_timer_.get_time() >= JumpAtk2_Active && !attack_on_)
	{
		attack_on_ = true;	// 攻撃判定は1回しか発生しない

		// 攻撃パラメータ構造体を生成
		const float distance = 12.0f;						// 攻撃判定の発生距離（前方からどれぐらい）
		const float height = 9.5f;							// 攻撃判定の高さ
		Vector3 atk_pos = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);	// 攻撃判定の位置
		int		power = PlayerParameter::Power_JumpAtk2;	// 威力
		int		wince = PlayerParameter::Wince_JumpAtk2;	// 怯み値
		float	hit_stop = PlayerParameter::HitStop_Short;	// ヒットストップ
		AttackParameter atk_para{ atk_pos, power, wince, hit_stop };
		// 攻撃判定を生成
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, atk_para));
		Sound::play_se(SE_SLASH);	// SEを再生

		mesh_.reset_speed();		// 以降のモーション速度を少し遅くにする
	}

	// モーション終了の前に、次の攻撃や回避への移行
	if (state_timer_.get_time() > JumpAtk2_InputValid && state_timer_.get_time() < JumpAtk2_InputInvalid && is_ground_)
	{
		// 通常攻撃の1段階目への移行
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.6f);	// 次のモーション速度を設定
			change_state(PlayerState::Slash1, MOTION_SLASH_1);
			return;
		}

		// 回避への移行
		if (PlayerInput::evasion())
		{
			attack_to_evasion();
		}
	}

	// モーション終了後、通常状態に戻る
	if (state_timer_.get_time() >= JumpAtk2_End)
	{
		normal(delta_time);
	}
}

// 被弾状態での更新
void Player::damage(float delta_time)
{
	// モーション終了後、通常状態に戻る
	if (state_timer_.get_time() >= Damage_End)
	{
		normal(delta_time);
	}
}

// ガード時の更新
void Player::guard(float delta_time)
{
	// ガード判定を発生
	is_guard_ = true;

	// ガード攻撃への移行
	if (PlayerInput::attack())
	{
		mesh_.change_speed(1.5f);
		change_state(PlayerState::GuardAttack, MOTION_GUARD_SLASH);
		return;
	}

	// ガード開始のモーション終了後、ガード待機モーションに移行
	if (state_timer_.get_time() >= mesh_.motion_end_time())
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
	if (state_timer_.get_time() >= Block_End)
	{

		state_ = PlayerState::Guard;
		guard(delta_time);
	}
}

// ガード攻撃での更新
void Player::guard_attack(float delta_time)
{
	// 攻撃判定を発生
	if (state_timer_.get_time() >= GuardAtk_Active && !attack_on_)
	{
		attack_on_ = true;	// 攻撃判定は1回しか発生しない

		// 攻撃パラメータ構造体を生成
		const float distance = 15.0f;						// 攻撃判定の発生距離（前方からどれぐらい）
		const float height = 9.5f;							// 攻撃判定の高さ
		Vector3 atk_pos = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);	// 攻撃判定の位置
		int		power = PlayerParameter::Power_GuardAtk;	// 威力
		int		wince = PlayerParameter::Wince_GuardAtk;	// 怯み値
		float	hit_stop = PlayerParameter::HitStop_Short;	// ヒットストップ
		AttackParameter atk_para{ atk_pos, power, wince, hit_stop };
		// 攻撃判定を生成
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, atk_para));
		Sound::play_se(SE_SLASH);	// SEを再生
	}

	// モーション終了の前に、次の攻撃や回避への移行
	if (state_timer_.get_time() > GuardAtk_InputValid && state_timer_.get_time() < GuardAtk_InputInvalid)
	{
		// 通常攻撃の1段階目への移行
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.6f);	// 次のモーション速度を設定
			change_state(PlayerState::Slash1, MOTION_SLASH_1);
			return;
		}

		// 回避への移行
		if (PlayerInput::evasion())
		{
			attack_to_evasion();
		}
	}

	// 通常状態への移行
	if (state_timer_.get_time() >= GuardAtk_End)
	{
		normal(delta_time);
	}
}

// ガード終了時の更新
void Player::guard_end(float delta_time)
{
	// モーション終了後、通常状態に戻る
	if (state_timer_.get_time() >= mesh_.motion_end_time() + 4.0f)
	{
		normal(delta_time);
	}
}

// 死亡状態での更新
void Player::death(float delta_time)
{
	// プレイヤー死亡メッセージを送る（1回だけ）
	if (!is_defeated_)
	{
		world_->send_message(EventMessage::PlayerDead);
		is_defeated_ = true;
	}

	// モーションが終了すると、死亡判定を有効に
	if (state_timer_.get_time() >= mesh_.motion_end_time() * 2.0f)
	{
		die();
	}
}

// 前回避状態での更新
void Player::forward_evasion(float delta_time)
{
	// 地面に離れたら、通常状態に戻る
	if (!is_ground_)
	{
		invincible_timer_.shut();	// 無敵時間を強制終了
		normal(delta_time);
	}

	// プレイヤーの座標を移動
	velocity_ = rotation_.Forward() * PlayerParameter::EvasionSpeed;
	position_ += velocity_ * delta_time;

	// 回避終了後、通常状態に戻る
	if (evasion_timer_.is_time_out())
	{
		normal(delta_time);
		return;
	}

	// 回避タイマーを更新
	evasion_timer_.update(delta_time);
}

// 左回避状態での更新
void Player::left_evasion(float delta_time)
{
	// 地面に離れたら、通常状態に戻る
	if (!is_ground_)
	{
		invincible_timer_.shut();	// 無敵時間を強制終了
		normal(delta_time);
	}

	// プレイヤーの座標を移動
	velocity_ = rotation_.Left() * PlayerParameter::EvasionSpeed;
	position_ += velocity_ * delta_time;

	// 回避終了後、通常状態に戻る
	if (evasion_timer_.is_time_out())
	{
		normal(delta_time);
		return;
	}

	// 回避タイマーを更新
	evasion_timer_.update(delta_time);
}

// 右回避状態での更新
void Player::right_evasion(float delta_time)
{
	// 地面に離れたら、通常状態に戻る
	if (!is_ground_)
	{
		invincible_timer_.shut();	// 無敵時間を強制終了
		normal(delta_time);
	}

	// プレイヤーの座標を移動
	velocity_ = rotation_.Right() * PlayerParameter::EvasionSpeed;
	position_ += velocity_ * delta_time;

	// 回避終了後、通常状態に戻る
	if (evasion_timer_.is_time_out())
	{
		normal(delta_time);
		return;
	}

	// 回避タイマーを更新
	evasion_timer_.update(delta_time);
}

// 攻撃後の回避行動移行
void Player::attack_to_evasion()
{
	// プレイヤーの方向入力を取得
	auto input = PlayerInput::move_input();

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

	// 入力した方向とプレイヤーの方向ベクトルの差が45°以下の場合、回避行動に移る
	float angle = 45.0f;

	// 左回避
	if (Vector3::Angle(rotation_.Left(), direction) <= angle)
	{
		ready_for_evasion();
		change_state(PlayerState::LeftEvasion, PlayerMotion::MOTION_STRAFE_LEFT);
		return;
	}
	// 右回避
	else if (Vector3::Angle(rotation_.Right(), direction) <= angle)
	{
		ready_for_evasion();
		change_state(PlayerState::RightEvasion, PlayerMotion::MOTION_STRAFE_RIGHT);
		return;
	}
	// 前回避
	else
	{
		ready_for_evasion();
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

// 座標制限
void Player::clamp_position()
{
	// フィールドを取得
	auto& field = world_->field();
	// フィールドの最大と最小座標を取得
	auto max_pos = field.max_position();
	auto min_pos = field.min_position();

	position_.x = MathHelper::clamp(position_.x, min_pos.x, max_pos.x);
	position_.z = MathHelper::clamp(position_.z, min_pos.z, max_pos.z);
}

// 回避準備
void Player::ready_for_evasion()
{
	// モーションの再生速度を引き上げる
	mesh_.change_speed(1.5f);
	// 回避時間と無敵時間をリセット
	evasion_timer_.reset();
	invincible_timer_.reset();
}

// ヒットストップタイマーのリセット
void Player::reset_hit_stop(float time)
{
	hit_stop_timer_.reset_time(time);
}

// ガードは成立するか
bool Player::can_block(Vector3 atk_pos) const
{
	// 内積で攻撃判定のある方向を判定
	Vector3 to_attack = atk_pos - position_;									// 攻撃判定方向のベクトル
	float forward_dot_target = Vector3::Dot(rotation_.Forward(), to_attack);	// 前方向と攻撃判定の内積

	// 前方向とプレイヤーの内積が0以上であれば、Trueを返す
	return (forward_dot_target >= 0.0f);
}

// 無敵時間内であるか
bool Player::is_invincible() const
{
	return (!invincible_timer_.is_time_out() || state_ == PlayerState::Damage);
}

// スーパーアーマー状態であるか
bool Player::is_super_armor() const
{
	return state_ == PlayerState::Slash3;
}

// ヒットストップ中なのか
bool Player::is_hit_stop() const
{
	return !hit_stop_timer_.is_time_out();
}