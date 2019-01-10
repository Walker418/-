#include "AnimatedSprite.h"
#include "MathHelper.h"
#include <cmath>
#include <DxLib.h>

// �N���X�F�A�j���[�V�����t���X�v���C�g
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �f�X�g���N�^
AnimatedSprite::~AnimatedSprite()
{
	clear();
}

// �摜�t�@�C���̓ǂݍ���
void AnimatedSprite::load(const std::string& file_name)
{
	// �Â��f�[�^���폜
	clear();
	// �摜��ǂݍ���
	graph_ = LoadGraph(file_name.c_str());
}

// �摜�t�@�C���̓ǂݍ��݁i�����w�肠��j
void AnimatedSprite::load(const std::string& file_name, int row, int column)
{
	// �摜�f�[�^��ǂݍ���
	load(file_name);
	// �摜�T�C�Y���擾
	int size_x = 0, size_y = 0;
	GetGraphSize(graph_, &size_x, &size_y);
	// �摜�𕪊�
	int w = size_x / column;
	int h = size_y / row;
	for (int y = 0; y < size_y; y += h)
	{
		for (int x = 0; x < size_x; x += w)
		{
			div_graphs_.push_back(DerivationGraph(x, y, w, h, graph_));
		}
	}
}

// �`��i�摜�S�́j
void AnimatedSprite::draw(const Vector2& position) const
{
	DrawGraphF(position.x, position.y, graph_, TRUE);
}

// �`��i�摜�̈ꕔ����`��j
void AnimatedSprite::draw(const Vector2& position, int x, int y, int w, int h) const
{
	DrawRectGraphF(position.x, position.y, x, y, w, h, graph_, TRUE, FALSE);
}

// �`��i�����ԍ��w��j
void AnimatedSprite::draw(int graph_no, const Vector2& position, const Vector2& center, const Vector2& scale, float rotation) const
{
	DrawRotaGraph3F(position.x, position.y, center.x, center.y, scale.x, scale.y,
		MathHelper::degree_to_radian(rotation), div_graphs_[graph_no], TRUE, FALSE);
}

// �`��i�A�j���[�V�����w��j
void AnimatedSprite::draw(int animation, float time, const Vector2& position, const Vector2& center, const Vector2& scale, float rotation) const
{
	draw(find_key_frame(animation, time).second, position, center, scale, rotation);
}

// �A�j���[�V�����̒ǉ�
void AnimatedSprite::add_animation(int animation, int start, int end, float time)
{
	// �Â��f�[�^���폜
	animations_[animation].clear();
	// �A�j���[�V�����f�[�^��ǉ�
	for (int frame = 0; frame <= (end - start); ++frame)
	{
		add_key_frame(animation, (frame + 1) * time, start + frame);
	}
}

// �L�[�t���[���̒ǉ�
void AnimatedSprite::add_key_frame(int animation, float time, int graph_no)
{
	animations_[animation].emplace_back(time, graph_no);
}

// �L�[�t���[���̒ǉ��i�����w�肠��j
void AnimatedSprite::add_key_frame(int animation, float time, int x, int y, int w, int h)
{
	div_graphs_.push_back(DerivationGraph(x, y, w, h, graph_));
	add_key_frame(animation, time, int(div_graphs_.size()) - 1);
}

// �A�j���[�V�����I�����Ԃ̎擾
float AnimatedSprite::animation_end_time(int animation) const
{
	return animations_.at(animation).back().first;
}

// ����
void AnimatedSprite::clear()
{
	// �摜�f�[�^������
	if (graph_ != -1)
	{
		DeleteSharingGraph(graph_);
		graph_ = -1;
		div_graphs_.clear();
	}
	// �A�j���[�V�����f�[�^������
	animations_.clear();
}

// �L�[�t���[���̌���
const AnimatedSprite::KeyFrame& AnimatedSprite::find_key_frame(int animation, float timer) const
{
	const auto& key_frames = animations_.at(animation);
	timer = fmod(timer, animation_end_time(animation) + 1.0f);
	for (const auto& key_frame : key_frames)
	{
		if (timer <= key_frame.first)
		{
			return key_frame;
		}
	}

	return key_frames.back();
}