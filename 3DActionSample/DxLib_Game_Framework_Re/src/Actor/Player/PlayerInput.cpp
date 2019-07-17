#include "PlayerInput.h"
#include "../../Input/GamePad.h"
#include "../../Input/Keyboard.h"

// �N���X�F�v���C���[����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �Q�[���X�^�[�g���͂�������
bool PlayerInput::game_start()
{
	// �Q�[���p�b�h�̃X�^�[�g�{�^�����������ƁATrue��Ԃ�
	if (GamePad::getInstance().ButtonTriggerDown(PadButton::Start)) return true;
	// �L�[�{�[�h�̃X�y�[�X�L�[���������ƁATrue��Ԃ�
	if (Keyboard::getInstance().KeyTriggerDown(Keycode::Space)) return true;

	return false;
}

// �U�����͂�������
bool PlayerInput::attack()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::Y) || Keyboard::getInstance().KeyTriggerDown(Keycode::Space));
}

// �W�����v�U�����͂�������
bool PlayerInput::jump_attack()
{
	if (GamePad::getInstance().ButtonTriggerDown(PadButton::Y) && GamePad::getInstance().ButtonTriggerDown(PadButton::B))
		return true;
	if (Keyboard::getInstance().KeyTriggerDown(Keycode::B))
		return true;

	return false;
}

// �K�[�h���͂�������
bool PlayerInput::guard()
{
	return (GamePad::getInstance().ButtonStateDown(PadButton::R) || Keyboard::getInstance().KeyStateDown(Keycode::M));
}

// �K�[�h���͂��I��������
bool PlayerInput::guard_end()
{
	return (GamePad::getInstance().ButtonStateUp(PadButton::R) && Keyboard::getInstance().KeyStateUp(Keycode::M));
}

// �����͂�������
bool PlayerInput::evasion()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::A) || Keyboard::getInstance().KeyTriggerDown(Keycode::N));
}

// �O�ړ�
bool PlayerInput::move_forward()
{
	return Keyboard::getInstance().KeyStateDown(Keycode::W);
}

// ��ړ�
bool PlayerInput::move_backward()
{
	return Keyboard::getInstance().KeyStateDown(Keycode::S);
}

// ���ړ�
bool PlayerInput::move_left()
{
	return Keyboard::getInstance().KeyStateDown(Keycode::A);
}

// �E�ړ�
bool PlayerInput::move_right()
{
	return Keyboard::getInstance().KeyStateDown(Keycode::D);
}

// �L�[�{�[�h�ňړ����Ă��邩
bool PlayerInput::keyboard_move()
{
	// �����ꂩ�̕������͂�����΁ATrue��Ԃ�
	return (move_forward() || move_backward() || move_left() || move_right());
}

// �ړ������̓���
Vector2 PlayerInput::move_input()
{
	Vector2 input = Vector2::Zero;	// ���͏���Ԃ��ϐ���錾���Ă���

	// �L�[�{�[�h�̓��͂��擾
	input.y = (move_forward()) ? 1.0f : (move_backward()) ? -1.0f : 0.0f;	// �O��
	input.x = (move_left()) ? -1.0f : (move_right()) ? 1.0f : 0.0f;			// ���E

	// �L�[�{�[�h�̓��͂��Ȃ���΁A�Q�[���p�b�h�̓��͂��擾
	if (!keyboard_move()) input = GamePad::getInstance().L_Stick();

	// ���K�������������͂�Ԃ�
	return input.Normalize();
}

// ���X�e�B�b�N�ňړ����Ă��邩
bool PlayerInput::gamepad_move()
{
	// ���X�e�B�b�N�̓��͂�0�łȂ���΁ATrue��Ԃ�
	return move_input() != Vector2::Zero;
}

// �J��������]
bool PlayerInput::camera_turn_left()
{
	return (GamePad::getInstance().GetCurrent_R_Stick().x < 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::Left));
}

// �J�����E��]
bool PlayerInput::camera_turn_right()
{
	return (GamePad::getInstance().GetCurrent_R_Stick().x > 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::Right));
}

// �J�������]
bool PlayerInput::camera_turn_up()
{
	return (GamePad::getInstance().GetCurrent_R_Stick().y > 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::Up));
}

// �J��������]
bool PlayerInput::camera_turn_down()
{
	return (GamePad::getInstance().GetCurrent_R_Stick().y < 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::Down));
}

// �J�������Z�b�g
bool PlayerInput::camera_reset()
{
	if (GamePad::getInstance().ButtonTriggerDown(PadButton::L))		return true;
	if (Keyboard::getInstance().KeyTriggerDown(Keycode::R_Shift))	return true;

	return false;
}

// �G����X�L�b�v
bool PlayerInput::skip_phase1()
{
	return (Keyboard::getInstance().KeyTriggerDown(Keycode::T));
}

// �|�[�Y
bool PlayerInput::pause()
{
	// �Q�[���p�b�h�̃X�^�[�g�{�^�����������ƁATrue��Ԃ�
	if (GamePad::getInstance().ButtonTriggerDown(PadButton::Start)) return true;
	// �L�[�{�[�h��Y���������ƁATrue��Ԃ�
	if (Keyboard::getInstance().KeyTriggerDown(Keycode::Y)) return true;

	return false;
}

// �Q�[���I��
bool PlayerInput::game_end()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::Back) || Keyboard::getInstance().KeyTriggerDown(Keycode::Escape));
}