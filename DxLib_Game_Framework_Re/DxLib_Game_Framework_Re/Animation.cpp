#include "Animation.h"
#include "SkeletalMesh.h"

// クラス：アニメーション制御
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
Animation::Animation(int model, int motion) :
	model_{ model },
	motion_{ motion },
	prev_motion_{ motion }
{
	SkeletalMesh::bind(model_);									// メッシュをバインド
	SkeletalMesh::bind_animation(motion_, motion_timer_);		// アニメーションをバインド
	SkeletalMesh::get_local_matrices(local_matrices_.data());	// ローカル変換行列を取得
}

// 更新
void Animation::update(float delta_time)
{
	// メッシュをバインド
	SkeletalMesh::bind(model_);
	// アニメーションをバインド
	SkeletalMesh::bind_animation(prev_motion_, prev_motion_timer_, motion_, motion_timer_, lerp_timer_ / LerpTime);
	// ローカル変換行列を取得
	SkeletalMesh::get_local_matrices(local_matrices_.data());

	// アニメーションタイマーを更新
	motion_timer_ = std::fmod(motion_timer_ + 0.5f * delta_time, end_time());
	// 補間タイマーを更新
	lerp_timer_ = std::fmin(lerp_timer_ + delta_time, LerpTime);
}

// モーションの変更
void Animation::change_motion(int motion)
{
	// 現在と同じモーションの場合は何もしない
	if (motion == motion_) return;

	prev_motion_ = motion_;				// 前回のモーション番号を記録
	prev_motion_timer_ = motion_timer_;	// 最終アニメーションタイマーを記録
	motion_ = motion;					// モーションを変更
	motion_timer_ = 0.0f;				// アニメーションタイマーをリセット
	lerp_timer_ = 0.0f;					// 補間タイマーをリセット
}

// 現在再生中のモーションの取得
int Animation::motion() const
{
	return motion_;
}

// 変換行列の取得
const Animation::Matrices& Animation::local_matrices() const
{
	return local_matrices_;
}

// ボーン数の取得
int Animation::bone_count() const
{
	SkeletalMesh::bind(model_);

	return SkeletalMesh::bone_count();
}

// 終了時間の取得
float Animation::end_time() const
{
	SkeletalMesh::bind(model_);

	return SkeletalMesh::end_time(motion_);
}