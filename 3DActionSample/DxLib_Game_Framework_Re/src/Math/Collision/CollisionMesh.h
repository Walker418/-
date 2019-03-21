#ifndef COLLISION_MESH_H_
#define COLLISION_MESH_H_

#include <string>
#include "../../Math/Vector3.h"
#include "../../Graphic/ModelAsset.h"

// クラス：衝突判定用メッシュ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class CollisionMesh
{
public:
	// 初期化
	static void initialize();
	// 終了処理
	static void finalize();
	// 読み込み
	static bool load(int id, const std::string& file_name, int frame = -1, int div_x = 32, int div_y = 8, int div_z = 32);
	// 削除
	static void erase(int id);
	// メッシュのバインド
	static void bind(int id);
	// 描画
	static void draw();
	// 線分との衝突判定
	static bool collide_line(const Vector3& start, const Vector3& end, Vector3* point = nullptr, Vector3* normal = nullptr);
	// 球体との衝突判定
	static bool collide_sphere(const Vector3& center, float radius, Vector3* result = nullptr);
	// カプセルとの衝突判定
	static bool collide_capsule(const Vector3& start, const Vector3& end, float radius, Vector3* result = nullptr);

	// メッシュに含まれる頂点のローカル座標での最大値の取得
	static Vector3 max_position();
	// メッシュに含まれる頂点のローカル座標での最小値の取得
	static Vector3 min_position();


private:
	// バインド中のモデル
	static int			model_;
	// モデルアセット
	static ModelAsset	asset_;
};

#endif // !COLLISION_MESH_H_