#ifndef FIELD_H_
#define FIELD_H_

#include "../Math/Vector3.h"

// �N���X�F�t�B�[���h
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Field
{
public:
	// �R���X�g���N�^
	Field(int stage, int skybox);
	// �`��
	void draw() const;

	// �����Ƃ̏Փ˔���
	bool collide_line(const Vector3& start, const Vector3& end, Vector3* point = nullptr, Vector3* normal = nullptr);
	// ���̂Ƃ̏Փ˔���
	bool collide_sphere(const Vector3& center, float radius, Vector3* result = nullptr);
	// �J�v�Z���Ƃ̏Փ˔���
	bool collide_capsule(const Vector3& start, const Vector3& end, float radius, Vector3* result = nullptr);

	// �t�B�[���h�̍ő���W�̎擾
	Vector3 max_position() const;
	// �t�B�[���h�̍ŏ����W�̎擾
	Vector3 min_position() const;

	// �t�B�[���h������
	bool is_inside(const Vector3& position) const;
	// �t�B�[���h�O����
	bool is_outside(const Vector3& position) const;

	// �R�s�[�֎~
	Field(const Field& other) = delete;
	Field& operator = (const Field& other) = delete;

private:
	// �X�e�[�W���f��
	int stage_;
	// �X�J�C�{�b�N�X���f��
	int skybox_;
};

#endif // !FIELD_H_