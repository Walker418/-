#ifndef ANIMATED_SPRITE_H_
#define ANIMATED_SPRITE_H_

#include <unordered_map>
#include <vector>
#include "../Math/Vector2.h"

// �N���X�F�A�j���[�V�����t���X�v���C�g
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class AnimatedSprite
{
public:
	// �f�t�H���g�R���X�g���N�^
	AnimatedSprite() = default;
	// �f�X�g���N�^
	~AnimatedSprite();
	// �摜�t�@�C���̓ǂݍ���
	void load(const std::string& file_name);
	// �摜�t�@�C���̓ǂݍ��݁i�����w�肠��j
	void load(const std::string& file_name, int row, int column);
	// �`��i�摜�S�́j
	void draw(const Vector2& position) const;
	// �`��i�摜�̈ꕔ����`��j
	void draw(const Vector2& position, int x, int y, int w, int h) const;
	// �`��i�����ԍ��w��j
	void draw(int graph_no, const Vector2& position, const Vector2& center, const Vector2& scale, float rotation) const;
	// �`��i�A�j���[�V�����w��j
	void draw(int animation, float time, const Vector2& position, const Vector2& center, const Vector2& scale, float rotation) const;
	// �A�j���[�V�����̒ǉ�
	void add_animation(int animation, int start, int end, float time);
	// �L�[�t���[���̒ǉ�
	void add_key_frame(int animation, float time, int graph_no);
	// �L�[�t���[���̒ǉ��i�����w�肠��j
	void add_key_frame(int animation, float time, int x, int y, int w, int h);
	// �A�j���[�V�����I�����Ԃ̎擾
	float animation_end_time(int animation) const;
	// ����
	void clear();

	// �R�s�[�֎~
	AnimatedSprite(const AnimatedSprite& other) = delete;
	AnimatedSprite& operator = (const AnimatedSprite& other) = delete;

private:
	// �����摜
	using DivGraphs = std::vector<int>;
	// �L�[�t���[��
	using KeyFrame = std::pair<float, int>;
	// �L�[�t���[���z��
	using KeyFrames = std::vector<KeyFrame>;
	// �A�j���[�V�����z��
	using Animations = std::unordered_map<int, KeyFrames>;

private:
	// �L�[�t���[���̌���
	const KeyFrame& find_key_frame(int animation, float timer) const;

private:
	// ���摜
	int graph_{ -1 };
	// �����摜
	DivGraphs div_graphs_;
	// �A�j���[�V�����f�[�^
	Animations animations_;
};

#endif // !ANIMATED_SPRITE_H_