#ifndef TITLE_CAMERA_H_
#define TITLE_CAMERA_H_

#include "../Actor.h"

// �N���X�F�^�C�g����ʗp�J����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

class TitleCamera : public Actor
{
public:
	// �R���X�g���N�^
	TitleCamera(IWorld* world, const Vector3& position);

private:
	// �X�V
	void update(float delta_time) override;
	// �`��
	void draw() const override;

private:
	// ��ʉ�]�p�J�E���^�[
	float sin_count_{ 0.0f };

};

#endif // !TITLE_CAMERA_H_