#ifndef SKELETAL_MESH_H_
#define SKELETAL_MESH_H_

#include <string>
#include "Matrix.h"
#include "ModelAsset.h"

// クラス：スケルタルメッシュ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class SkeletalMesh
{
public:
	// 初期化
	static void initialize();
	// 終了処理
	static void finalize();
	// 読み込み
	static bool load(int id, const std::string& file_name);
	// 削除
	static void erase(int id);
	// メッシュのバインド
	static void bind(int id);
	// アニメーションのバインド
	static void bind_animation(int motion, float time);
	// アニメーションのバインド（補間付き）
	static void bind_animation(int prev_motion, float prev_time, int motion, float time, float amount);
	// ワールド変換行列の計算
	static void transform(const Matrix& world);
	// 描画
	static void draw();
	// ローカル変換行列の取得
	static void get_local_matrices(Matrix local_matrices[]);
	// ローカル変換行列の設定
	static void set_local_matrices(const Matrix local_matrices[]);
	// ワールド変換行列の取得
	static void get_world_matrices(Matrix world_matrices[]);
	// ワールド変換行列の設定
	static void set_world_matrices(const Matrix world_matrices[]);
	// ボーン数の取得
	static int bone_count();
	// モーションの終了時間の取得
	static float end_time(int motion);

private:
	// バインド中のモデル
	static int			model_;
	// ボーンの最大数
	static const int	BoneMax{ 256 };
	// ボーンのローカル変換行列
	static Matrix		local_matrices_[BoneMax];
	// ボーンのワールド変換行列
	static Matrix		world_matrices_[BoneMax];
	// モデルアセット
	static ModelAsset	asset_;
};

#endif // !SKELETAL_MESH_H_