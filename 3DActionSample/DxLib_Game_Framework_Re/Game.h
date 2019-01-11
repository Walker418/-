#ifndef GAME_H_
#define GAME_H_

// main�֐����G���g���[�|�C���g�ɂ���
#pragma comment(linker, "/entry:mainCRTStartup")

// �N���X�F�Q�[���A�v���P�[�V����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Game
{
public:
	// �f�t�H���g�R���X�g���N�^
	Game() = default;
	// �R���X�g���N�^
	Game(int width, int height, bool full_screen);
	// ���s
	int run();

	// �R�s�[�֎~
	Game(const Game& other) = delete;
	Game& operator = (const Game& other) = delete;

private:
	// �J�n
	virtual void start();
	// �X�V
	virtual void update(float delta_time);
	// �`��
	virtual void draw();
	// �I��
	virtual void end();
	// ���s���Ȃ̂�
	virtual bool is_running() const;

private:
	// �E�B���h�E�̕�
	int window_width_{ 640 };
	// �E�B���h�E�̍���
	int window_height_{ 480 };
	// �t���X�N���[�����[�h
	bool is_full_screen_{ false };
};

#endif // !GAME_H_