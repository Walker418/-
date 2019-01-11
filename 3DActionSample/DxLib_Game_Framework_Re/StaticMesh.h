#ifndef STATIC_MESH_H_
#define STATIC_MESH_H_

#include <string>
#include "Matrix.h"
#include "ModelAsset.h"

// クラス：スタティックメッシュ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class StaticMesh
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
	// 描画
	static void draw();
	// ワールド変換行列の計算
	static void transform(const Matrix& world);

private:
	// バインド中のモデル
	static int			model_;
	// ボーンのワールド変換行列
	static Matrix		world_matrix_;
	// モデルアセット
	static ModelAsset	asset_;
};

#endif // !STATIC_MESH_H_