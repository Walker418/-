#ifndef GRAPHICS_2D_H_
#define GRAPHICS_2D_H_

#include "Vector2.h"
#include "AnimatedSprite.h"
#include <string>
#include <unordered_map>

// �N���X�F2D�O���t�B�b�N�X
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Graphics2D
{
public:
	// ������
	static void initialize();
	// �摜�t�@�C���̓ǂݍ���
	static void load(int id, const std::string& file_name);
	// �摜�t�@�C���̓ǂݍ��݁i�����w�肠��j
	static void load(int id, const std::string& file_name, int row, int column);
	// �`��i�摜�S�́j
	static void draw(int id, const Vector2& position);
	// �`��i�摜�̈ꕔ����`��j
	static void draw(int id, const Vector2& position, int x, int y, int w, int h);
	// �`��i�����ԍ��w��j
	static void draw(int id, int div_no, const Vector2& position,
		const Vector2& center = Vector2::Zero, const Vector2& scale = Vector2::One, float rotation = 0.0f);
	// �`��i�A�j���[�V�����w��j
	static void draw(int id, int animation, float time, const Vector2& position,
		const Vector2& center = Vector2::Zero, const Vector2& scale = Vector2::One, float rotation = 0.0f);
	// �A�j���[�V�����̒ǉ�
	static void add_animation(int id, int animation, int start, int end, float time);
	// �L�[�t���[���̒ǉ�
	static void add_key_frame(int id, int animation, float time, int div_no);
	// �L�[�t���[���̒ǉ�
	static void add_key_frame(int id, int animation, float time, int x, int y, int w, int h);
	// �X�v���C�g�̍폜
	static void delete_sprite(int id);
	// �A�j���[�V�����̏I�����Ԃ��擾
	static float get_sprite_animation_end_time(int id, int animation);
	// �w�i�F�̐ݒ�
	static void clear_color(float r, float g, float b);
	// �I��
	static void finalize();

private:
	// �X�v���C�g�f�[�^
	static std::unordered_map<int, AnimatedSprite> sprite_map_;
};

#endif // !GRAPHICS_2D_H_