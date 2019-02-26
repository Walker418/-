#ifndef MOUSE_H_
#define MOUSE_H_

#include <DxLib.h>
#include <array>
#include "Vector2.h"

// �}�E�X�̃{�^��
namespace MouseButton
{
	const UINT Left = MOUSE_INPUT_LEFT;			// ���{�^��
	const UINT Right = MOUSE_INPUT_RIGHT;		// �E�{�^��
	const UINT Middle = MOUSE_INPUT_MIDDLE;		// �����{�^��
	const UINT Button4 = MOUSE_INPUT_4;			// �{�^��4
	const UINT Button5 = MOUSE_INPUT_5;			// �{�^��5
	const UINT Button6 = MOUSE_INPUT_6;			// �{�^��6
	const UINT Button7 = MOUSE_INPUT_7;			// �{�^��7
	const UINT Button8 = MOUSE_INPUT_8;			// �{�^��8
}

// �}�E�X���̓N���X
class Mouse
{
private:
	// �R���X�g���N�^
	Mouse();
	// �f�X�g���N�^
	~Mouse() = default;

public:
	// �}�E�X�N���X�̎擾
	static Mouse &getInstance()
	{
		static Mouse m;
		return m;
	}

	// �{�^����������Ă��邩
	bool State(UINT button);
	// �{�^���������ꂽ��
	bool Trigger(UINT button);
	// �{�^���������ꂽ��
	bool Release(UINT button);

	// �}�E�X�J�[�\���̈ʒu���擾
	Vector2 cursor_point();
	// �}�E�X�J�[�\���̈ʒu��ݒ�
	void cursor_point(const Vector2& point);
	// �}�E�X�J�[�\���̕\��
	void show_cursor();
	// �}�E�X�J�[�\���̔�\��
	void hide_cursor();

	// �X�V
	void update();

	// �R�s�[�֎~
	Mouse(const Mouse&) = delete;
	Mouse& operator = (const Mouse&) = delete;
	// �ړ��֎~
	Mouse(Mouse&&) = delete;
	Mouse& operator = (Mouse&&) = delete;

private:
	// ���͏�ԕۑ��p�ϐ�
	int current_state_;		// ���t���[��
	int previous_state_;	// �O�t���[��
};

#endif // !MOUSE_H_