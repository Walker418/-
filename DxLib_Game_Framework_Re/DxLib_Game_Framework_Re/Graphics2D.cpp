#include "Graphics2D.h"
#include <DxLib.h>

// �N���X�F2D�O���t�B�b�N�X
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �X�v���C�g�f�[�^
std::unordered_map<int, AnimatedSprite> Graphics2D::sprite_map_;

// ������
void Graphics2D::initialize()
{
	sprite_map_.clear();
}

// �摜�t�@�C���̓ǂݍ���
void Graphics2D::load(int id, const std::string& file_name)
{
	sprite_map_[id].load(file_name);
}

// �摜�t�@�C���̓ǂݍ��݁i�����w�肠��j
void Graphics2D::load(int id, const std::string& file_name, int row, int column)
{
	sprite_map_[id].load(file_name, row, column);
}

// �`��i�摜�S�́j
void Graphics2D::draw(int id, const Vector2& position)
{
	sprite_map_[id].draw(position);
}

// �`��i�摜�̈ꕔ����`��j
void Graphics2D::draw(int id, const Vector2& position, int x, int y, int w, int h)
{
	sprite_map_[id].draw(position, x, y, w, h);
}

// �`��i�����ԍ��w��j
void Graphics2D::draw(int id, int div_no, const Vector2& position,
	const Vector2& center, const Vector2& scale, float rotation)
{
	sprite_map_[id].draw(div_no, position, center, scale, rotation);
}

// �`��i�A�j���[�V�����w��j
void Graphics2D::draw(int id, int animation, float time, const Vector2& position,
	const Vector2& center, const Vector2& scale, float rotation)
{
	sprite_map_[id].draw(animation, time, position, center, scale, rotation);
}

// �A�j���[�V�����̒ǉ�
void Graphics2D::add_animation(int id, int animation, int start, int end, float time)
{
	sprite_map_[id].add_animation(animation, start, end, time);
}

// �L�[�t���[���̒ǉ�
void Graphics2D::add_key_frame(int id, int animation, float time, int div_no)
{
	sprite_map_[id].add_key_frame(animation, time, div_no);
}

// �L�[�t���[���̒ǉ�
void Graphics2D::add_key_frame(int id, int animation, float time, int x, int y, int w, int h)
{
	sprite_map_[id].add_key_frame(animation, time, x, y, w, h);
}

// �X�v���C�g�̍폜
void Graphics2D::delete_sprite(int id)
{
	sprite_map_.erase(id);
}

// �A�j���[�V�����̏I�����Ԃ��擾
float Graphics2D::get_sprite_animation_end_time(int id, int animation)
{
	return sprite_map_[id].animation_end_time(animation);
}

// �w�i�F�̐ݒ�
void Graphics2D::clear_color(float r, float g, float b)
{
	SetBackgroundColor(int(r * 255.0f), int(g * 255.0f), int(b * 255.0f));
}

// �I��
void Graphics2D::finalize()
{
	// �������Ɠ�������
	initialize();
}