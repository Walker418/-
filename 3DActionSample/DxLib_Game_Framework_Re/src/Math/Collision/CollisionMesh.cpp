#include "CollisionMesh.h"
#include "CollisionTriangle.h"
#include <DxLib.h>

// クラス：衝突判定用メッシュ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// バインド中のモデル
int CollisionMesh::model_{ -1 };
// モデルアセット
ModelAsset CollisionMesh::asset_;

// 初期化
void CollisionMesh::initialize()
{
	// 終了処理と同じ
	finalize();
}

// 終了処理
void CollisionMesh::finalize()
{
	asset_.clear();
	model_ = -1;
}

// 読み込み
bool CollisionMesh::load(int id, const std::string& file_name, int frame, int div_x, int div_y, int div_z)
{
	auto result = asset_.load(id, file_name);
	if (result)
	{
		// 衝突判定情報を構築
		MV1SetupCollInfo(asset_[id], frame, div_x, div_y, div_z);
	}

	return result;
}

// 削除
void CollisionMesh::erase(int id)
{
	model_ = (model_ == asset_[id]) ? -1 : model_;
	asset_.erase(id);
}

// メッシュのバインド
void CollisionMesh::bind(int id)
{
	model_ = asset_[id];
}

// 描画
void CollisionMesh::draw()
{
	MV1DrawModel(model_);
}

// 線分との衝突判定
bool CollisionMesh::collide_line(const Vector3& start, const Vector3& end, Vector3* point, Vector3* normal)
{
	const auto coll_poly = MV1CollCheck_Line(model_, 0, start, end);
	if (coll_poly.HitFlag == TRUE)
	{
		if (point != nullptr)
		{
			*point = coll_poly.HitPosition;
		}
		if (normal != nullptr)
		{
			*normal = coll_poly.Normal;
		}

		return true;
	}

	return false;
}

// 球体との衝突判定
bool CollisionMesh::collide_sphere(const Vector3& center, float radius, Vector3* result)
{
	// 球とメッシュの衝突判定
	const auto coll_poly = MV1CollCheck_Sphere(model_, 0, center, radius);
	// 衝突していなければ終了
	if (coll_poly.HitNum == 0)
	{
		// 衝突判定データの削除
		MV1CollResultPolyDimTerminate(coll_poly);

		return false;
	}

	VECTOR result_center = center;

	// ポリゴンの平面上に球体の位置を補正する
	for (int i = 0; i < coll_poly.HitNum; ++i)
	{
		// 平面上の座標の最近点座標を求める
		PLANE_POINT_RESULT plane_point_result;
		Plane_Point_Analyse(
			&coll_poly.Dim[i].Position[0],
			&coll_poly.Dim[i].Normal,
			&result_center,
			&plane_point_result);
		// 三角形の内部に平面上の座標の最近点座標があるか調べる
		if (CollisionTriangle(
			coll_poly.Dim[i].Position[0],
			coll_poly.Dim[i].Position[1],
			coll_poly.Dim[i].Position[2]).is_inside(plane_point_result.Plane_MinDist_Pos))
		{
			// 法線ベクトルの方向へ球体を押し出す
			const auto distance = std::sqrt(plane_point_result.Plane_Pnt_MinDist_Square);
			const auto offset = VScale(coll_poly.Dim[i].Normal, radius - distance);
			result_center = VAdd(result_center, offset);
		}
	}

	// ポリゴンのエッジに重なっている場合に位置を補正する
	for (int i = 0; i < coll_poly.HitNum; ++i)
	{
		CollisionTriangle(
			coll_poly.Dim[i].Position[0],
			coll_poly.Dim[i].Position[1],
			coll_poly.Dim[i].Position[2]).collide_edge_and_sphere(result_center, radius, &result_center);
	}
	// 補正後の座標を設定
	if (result != nullptr)
	{
		*result = result_center;
	}
	// 衝突判定データの削除
	MV1CollResultPolyDimTerminate(coll_poly);

	return true;
}

// カプセルとの衝突判定
bool CollisionMesh::collide_capsule(const Vector3& start, const Vector3& end, float radius, Vector3* result)
{
	bool is_hit = false;			// 衝突判定
	VECTOR result_start = start;	// カプセル始点の座標
	VECTOR result_end = end;		// カプセル終点の座標

	// 終点（足元）を基準にして押し出し判定を行う
	if (collide_sphere(result_end, radius, result))
	{
		// 足元から押し出しベクトルを作成
		Vector3 move_vec_bottom = VSub(result_end, end);
		// カプセルの始点に押し出しを適用
		result_start = VAdd(result_start, move_vec_bottom);
		is_hit = true;
	}

	// 足元押し出し時点の始点、終点情報を保存
	VECTOR save_start = result_start;
	VECTOR save_end = result_end;

	// 始点（頭）を基準にして押し出し判定を行う
	if (collide_sphere(result_start, radius, result))
	{
		//頭からの押し出しベクトルを作成
		Vector3 move_vec_top = VSub(result_start, save_start);
		//カプセルの終点に押し出しを適用
		result_end = VAdd(result_end, move_vec_top);
		is_hit = true;
	}
	// ここまでで始点終点の押し出しが完了

	// カプセルとメッシュの衝突判定
	MV1_COLL_RESULT_POLY_DIM HitDim = MV1CollCheck_Capsule(model_, -1, result_start, result_end, radius);
	for (int i = 0; i < HitDim.HitNum; i++)
	{
		VECTOR triangle[4]
		{
			HitDim.Dim[i].Position[0],
			HitDim.Dim[i].Position[1],
			HitDim.Dim[i].Position[2],
			HitDim.Dim[i].Position[0]
		};

		for (int loop = 0; loop < 3; loop++)
		{
			SEGMENT_SEGMENT_RESULT seg_seg_result;
			Segment_Segment_Analyse(&result_start, &result_end, &triangle[i], &triangle[i + 1], &seg_seg_result);
			const auto distance = std::sqrt(seg_seg_result.SegA_SegB_MinDist_Square);
			if (distance <= radius)
			{
				is_hit = true;
				VECTOR offset = VScale(VNorm(VSub(seg_seg_result.SegA_MinDist_Pos, seg_seg_result.SegB_MinDist_Pos)), radius - distance);
				result_start = VAdd(result_start, offset);
				result_end = VAdd(result_end, offset);
			}
		}
	}

	// 補正後の座標を設定
	if (is_hit) *result = (result_start + result_end) * 0.5f;
	// 衝突判定データの削除
	MV1CollResultPolyDimTerminate(HitDim);

	return is_hit;
}

// メッシュに含まれる頂点のローカル座標での最大値の取得
Vector3 CollisionMesh::max_position()
{
	return MV1GetMeshMaxPosition(model_, 0);
}

// メッシュに含まれる頂点のローカル座標での最小値の取得
Vector3 CollisionMesh::min_position()
{
	return MV1GetMeshMinPosition(model_, 0);
}
