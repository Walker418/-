#ifndef RAY_H_
#define RAY_H_

#include "Vector3.h"

// �N���X�F���C
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Ray
{
public:
	// �f�t�H���g�R���X�g���N�^
	Ray() = default;
	// �R���X�g���N�^
	Ray(const Vector3& position, Vector3& direction) :
		position{ position }, direction{ direction } { }
	// ���W
	Vector3 position{ 0.0f, 0.0f, 0.0f };
	// ����
	Vector3 direction{ 0.0f, 0.0f, 0.0f };
};

#endif // !RAY_H_