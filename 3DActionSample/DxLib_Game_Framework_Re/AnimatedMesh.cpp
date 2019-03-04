#include "AnimatedMesh.h"
#include "SkeletalMesh.h"

// クラス：アニメーション付きメッシュ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
AnimatedMesh::AnimatedMesh(int mesh, int motion) :
	mesh_{ mesh }, animation_{ mesh, motion }
{ }

// 更新
void AnimatedMesh::update(float delta_time)
{
	animation_.update(delta_time);
}

// 描画
void AnimatedMesh::draw() const
{
	SkeletalMesh::bind(mesh_);
	SkeletalMesh::set_world_matrices(world_matrices_.data());
	SkeletalMesh::draw();
}

// モーションの変更
void AnimatedMesh::change_motion(int motion)
{
	animation_.change_motion(motion);
}

// スケルトンの変換行列の計算
void AnimatedMesh::transform(const Matrix& world)
{
	SkeletalMesh::bind(mesh_);
	SkeletalMesh::set_local_matrices(animation_.local_matrices().data());
	SkeletalMesh::transform(world);
	SkeletalMesh::get_world_matrices(world_matrices_.data());
}

// 変換行列の取得
const Matrix& AnimatedMesh::bone_matrix(int no) const
{
	return world_matrices_[no];
}

// モーションの終了時間の取得
float AnimatedMesh::motion_end_time() const
{
	return animation_.end_time();
}

// モーションの再生速度の変更
void AnimatedMesh::change_speed(float speed)
{
	animation_.change_speed(speed);
}

// モーションの再生速度のリセット
void AnimatedMesh::reset_speed()
{
	animation_.reset_speed();
}