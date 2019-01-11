#ifndef LINE_H_
#define LINE_H_

#include "Vector3.h"

// �N���X�F����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Line
{
public:
	// �f�t�H���g�R���X�g���N�^
	Line() = default;
	// �R���X�g���N�^
	Line(const Vector3& start, const Vector3& end) :
		start{ start }, end{ end } { }
	// �n�_
	Vector3 start{ 0.0f,0.0f,0.0f };
	// �I�_
	Vector3 end{ 0.0f,0.0f,0.0f };
};

#endif // !LINE_H_