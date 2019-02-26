#include "Mouse.h"

// �}�E�X���̓N���X

// �R���X�g���N�^
Mouse::Mouse() :
	current_state_{ 0 }, previous_state_{ 0 }
{
	SetUseDirectInputFlag(TRUE);	// ��������s���Ȃ��ƁA�}�E�X5�{�^���܂ł����擾�ł��Ȃ�
}

// �{�^����������Ă��邩
bool Mouse::State(UINT button)
{
	return (current_state_ & button) != 0;
}

// �{�^���������ꂽ��
bool Mouse::Trigger(UINT button)
{
	return (current_state_ & ~previous_state_ & button) != 0;
}

// �{�^���������ꂽ��
bool Mouse::Release(UINT button)
{
	return (~current_state_ & previous_state_ & button) != 0;
}

// �}�E�X�J�[�\���̈ʒu���擾
Vector2 Mouse::cursor_point()
{
	int x = 0, y = 0;
	GetMousePoint(&x, &y);
	return Vector2(float(x), float(y));
}

// �}�E�X�J�[�\���̈ʒu��ݒ�
void Mouse::cursor_point(const Vector2& point)
{
	SetMousePoint(int(point.x), int(point.y));
}

// �}�E�X�J�[�\���̕\��
void Mouse::show_cursor()
{
	SetMouseDispFlag(TRUE);
}

// �}�E�X�J�[�\���̔�\��
void Mouse::hide_cursor()
{
	SetMouseDispFlag(FALSE);
}

// �X�V
void Mouse::update()
{
	previous_state_ = current_state_;
	current_state_ = GetMouseInput();
}