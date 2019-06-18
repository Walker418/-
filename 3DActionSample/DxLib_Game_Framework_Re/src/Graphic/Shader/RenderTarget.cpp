#include "RenderTarget.h"

// �����_�[�^�[�Q�b�g�N���X

// �R���X�g���N�^
RenderTarget::RenderTarget(int width, int height, bool hdr, bool z_buffer, int bit_depth, int channel) :
	width_{ width }, height_{ height }
{
	if (hdr)
	{
		// ���������_�e�N�X�`���̏ꍇ
		SetDrawValidFloatTypeGraphCreateFlag(TRUE);
		SetCreateDrawValidGraphChannelNum(channel);
		SetCreateGraphChannelBitDepth(bit_depth);
	}
	else
	{
		SetDrawValidFloatTypeGraphCreateFlag(FALSE);
		SetCreateGraphColorBitDepth(bit_depth);
	}

	// Z�o�b�t�@�쐬�t���O��ݒ�
	SetDrawValidGraphCreateZBufferFlag(z_buffer ? TRUE : FALSE);
	// �����_�[�^�[�Q�b�g�p�̃e�N�X�`�����쐬
	texture_ = MakeScreen(width_, height_, TRUE);
	// �ݒ���f�t�H���g�ɖ߂�
	SetDrawValidGraphCreateZBufferFlag(TRUE);
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateDrawValidGraphChannelNum(0);
	SetCreateGraphChannelBitDepth(0);
	SetCreateGraphColorBitDepth(4);
	// �����_�[�^�[�Q�b�g�p�̃|���S���f�[�^���쐬
	const float w{ static_cast<float>(width_) };
	const float h{ static_cast<float>(height_) };
	vertices_[0] = { { 0, 0, 0 }, 1,{ 255, 255, 255, 255 }, {0, 0, 0, 0 }, 0, 0, 0, 0 };	// ����
	vertices_[1] = { { w, 0, 0 }, 1,{ 255, 255, 255, 255 }, {0, 0, 0, 0 }, 1, 0, 1, 0 };	// �E��
	vertices_[2] = { { 0, h, 0 }, 1,{ 255, 255, 255, 255 }, {0, 0, 0, 0 }, 0, 1, 0, 1 };	// ����
	vertices_[3] = { { w, h, 0 }, 1,{ 255, 255, 255, 255 }, {0, 0, 0, 0 }, 1, 1, 1, 1 };	// �E��
}

// �f�X�g���N�^
RenderTarget::~RenderTarget()
{
	DeleteGraph(texture_);
}

// �J�n
void RenderTarget::begin()
{
	prev_draw_screen_ = GetDrawScreen();
	SetDrawScreen(texture_);
}

// �I��
void RenderTarget::end()
{
	SetDrawScreen(prev_draw_screen_);
}

// �`��
void RenderTarget::draw() const
{
	DrawPrimitive2DToShader(vertices_, 4, DX_PRIMTYPE_TRIANGLESTRIP);
}

// ���̎擾
int RenderTarget::width() const
{
	return width_;
}

// �����̎擾
int RenderTarget::height() const
{
	return height_;
}

// �e�N�X�`���̎擾
int RenderTarget::texture() const
{
	return texture_;
}