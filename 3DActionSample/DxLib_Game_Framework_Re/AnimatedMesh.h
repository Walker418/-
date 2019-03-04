#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

#include <array>
#include "Matrix.h"
#include "Animation.h"

// クラス：アニメーション付きメッシュ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class AnimatedMesh
{
private:
	// スケルトンの変換行列
	using BoneMatrices = std::array<Matrix, 256>;

public:
	// コンストラクタ
	AnimatedMesh(int mesh, int motion = 0);
	// 更新
	void update(float delta_time);
	// 描画
	void draw() const;
	// モーションの変更
	void change_motion(int motion);
	// スケルトンの変換行列の計算
	void transform(const Matrix& world);
	// 変換行列の取得
	const Matrix& bone_matrix(int no) const;
	// モーションの終了時間の取得
	float motion_end_time() const;

	// モーションの再生速度の変更
	void change_speed(float speed);
	// モーションの再生速度のリセット
	void reset_speed();

private:
	// メッシュ
	int				mesh_;
	// アニメーション
	Animation		animation_;
	// スケルトンの変換行列
	BoneMatrices	world_matrices_;
};

#endif // !ANIMATED_MESH_H_