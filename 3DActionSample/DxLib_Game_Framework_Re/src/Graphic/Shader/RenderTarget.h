#ifndef RENDER_TARGET_H_
#define RENDER_TARGET_H_

#include <DxLib.h>

// �N���X�F�����_�[�^�[�Q�b�g
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class RenderTarget
{
public:
	// �R���X�g���N�^
	RenderTarget(int width, int height, bool hdr = false, bool z_
		= true, int bit_depth = 32, int channel = 4);
	// �f�X�g���N�^
	~RenderTarget();
	// �J�n
	void begin();
	// �I��
	void end();
	// �`��
	void draw() const;
	// ���̎擾
	int width() const;
	// �����̎擾
	int height() const;
	// �e�N�X�`���̎擾
	int texture() const;

private:
	int				width_;						// ��
	int				height_;					// ����
	int				texture_;					// �e�N�X�`��
	VERTEX2DSHADER	vertices_[4];				// ���_�f�[�^
	int				prev_draw_screen_{ -1 };	// �O��̕`��X�N���[��
};

#endif // !RENDER_TARGET_H_