#include "CollisionMesh.h"
#include "CollisionTriangle.h"
#include <DxLib.h>

// �N���X�F�Փ˔���p���b�V��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �o�C���h���̃��f��
int CollisionMesh::model_{ -1 };
// ���f���A�Z�b�g
ModelAsset CollisionMesh::asset_;

// ������
void CollisionMesh::initialize()
{
	// �I�������Ɠ���
	finalize();
}

// �I������
void CollisionMesh::finalize()
{
	asset_.clear();
	model_ = -1;
}

// �ǂݍ���
bool CollisionMesh::load(int id, const std::string& file_name, int frame, int div_x, int div_y, int div_z)
{
	auto result = asset_.load(id, file_name);
	if (result)
	{
		// �Փ˔�������\�z
		MV1SetupCollInfo(asset_[id], frame, div_x, div_y, div_z);
	}

	return result;
}

// �폜
void CollisionMesh::erase(int id)
{
	model_ = (model_ == asset_[id]) ? -1 : model_;
	asset_.erase(id);
}

// ���b�V���̃o�C���h
void CollisionMesh::bind(int id)
{
	model_ = asset_[id];
}

// �`��
void CollisionMesh::draw()
{
	MV1DrawModel(model_);
}

// �����Ƃ̏Փ˔���
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

// ���̂Ƃ̏Փ˔���
bool CollisionMesh::collide_sphere(const Vector3& center, float radius, Vector3* result)
{
	// ���ƃ��b�V���̏Փ˔���
	const auto coll_poly = MV1CollCheck_Sphere(model_, 0, center, radius);
	// �Փ˂��Ă��Ȃ���ΏI��
	if (coll_poly.HitNum == 0)
	{
		// �Փ˔���f�[�^�̍폜
		MV1CollResultPolyDimTerminate(coll_poly);

		return false;
	}

	VECTOR result_center = center;

	// �|���S���̕��ʏ�ɋ��̂̈ʒu��␳����
	for (int i = 0; i < coll_poly.HitNum; ++i)
	{
		// ���ʏ�̍��W�̍ŋߓ_���W�����߂�
		PLANE_POINT_RESULT plane_point_result;
		Plane_Point_Analyse(
			&coll_poly.Dim[i].Position[0],
			&coll_poly.Dim[i].Normal,
			&result_center,
			&plane_point_result);
		// �O�p�`�̓����ɕ��ʏ�̍��W�̍ŋߓ_���W�����邩���ׂ�
		if (CollisionTriangle(
			coll_poly.Dim[i].Position[0],
			coll_poly.Dim[i].Position[1],
			coll_poly.Dim[i].Position[2]).is_inside(plane_point_result.Plane_MinDist_Pos))
		{
			// �@���x�N�g���̕����֋��̂������o��
			const auto distance = std::sqrt(plane_point_result.Plane_Pnt_MinDist_Square);
			const auto offset = VScale(coll_poly.Dim[i].Normal, radius - distance);
			result_center = VAdd(result_center, offset);
		}
	}

	// �|���S���̃G�b�W�ɏd�Ȃ��Ă���ꍇ�Ɉʒu��␳����
	for (int i = 0; i < coll_poly.HitNum; ++i)
	{
		CollisionTriangle(
			coll_poly.Dim[i].Position[0],
			coll_poly.Dim[i].Position[1],
			coll_poly.Dim[i].Position[2]).collide_edge_and_sphere(result_center, radius, &result_center);
	}
	// �␳��̍��W��ݒ�
	if (result != nullptr)
	{
		*result = result_center;
	}
	// �Փ˔���f�[�^�̍폜
	MV1CollResultPolyDimTerminate(coll_poly);

	return true;
}

// �J�v�Z���Ƃ̏Փ˔���
bool CollisionMesh::collide_capsule(const Vector3& start, const Vector3& end, float radius, Vector3* result)
{
	bool is_hit = false;			// �Փ˔���
	VECTOR result_start = start;	// �J�v�Z���n�_�̍��W
	VECTOR result_end = end;		// �J�v�Z���I�_�̍��W

	// �I�_�i�����j����ɂ��ĉ����o��������s��
	if (collide_sphere(result_end, radius, result))
	{
		// �������牟���o���x�N�g�����쐬
		Vector3 move_vec_bottom = VSub(result_end, end);
		// �J�v�Z���̎n�_�ɉ����o����K�p
		result_start = VAdd(result_start, move_vec_bottom);
		is_hit = true;
	}

	// ���������o�����_�̎n�_�A�I�_����ۑ�
	VECTOR save_start = result_start;
	VECTOR save_end = result_end;

	// �n�_�i���j����ɂ��ĉ����o��������s��
	if (collide_sphere(result_start, radius, result))
	{
		//������̉����o���x�N�g�����쐬
		Vector3 move_vec_top = VSub(result_start, save_start);
		//�J�v�Z���̏I�_�ɉ����o����K�p
		result_end = VAdd(result_end, move_vec_top);
		is_hit = true;
	}
	// �����܂łŎn�_�I�_�̉����o��������

	// �J�v�Z���ƃ��b�V���̏Փ˔���
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

	// �␳��̍��W��ݒ�
	if (is_hit) *result = (result_start + result_end) * 0.5f;
	// �Փ˔���f�[�^�̍폜
	MV1CollResultPolyDimTerminate(HitDim);

	return is_hit;
}

// ���b�V���Ɋ܂܂�钸�_�̃��[�J�����W�ł̍ő�l�̎擾
Vector3 CollisionMesh::max_position()
{
	return MV1GetMeshMaxPosition(model_, 0);
}

// ���b�V���Ɋ܂܂�钸�_�̃��[�J�����W�ł̍ŏ��l�̎擾
Vector3 CollisionMesh::min_position()
{
	return MV1GetMeshMinPosition(model_, 0);
}
