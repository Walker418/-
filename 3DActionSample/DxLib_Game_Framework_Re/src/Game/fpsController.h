#ifndef FPS_CONTROLLER_H_
#define FPS_CONTROLLER_H_

// fps����N���X
class fpsController
{
public:
	// �f�t�H���g�R���X�g���N�^
	fpsController() = default;
	// �X�V
	bool update();
	// �`��
	void draw();
	// �ҋ@
	void wait();

private:
	int start_time_ = 0;		// �Q�[�����N����������
	int count_ = 0;				// �t���[�����J�E���^�[
	float fps_ = 0;				// ���݂�fps

	static const int N = 60;	// ���ς����T���v����
	static const int FPS = 60;	// �ݒ肵��fps
};

#endif // !FPS_CONTROLLER_H_