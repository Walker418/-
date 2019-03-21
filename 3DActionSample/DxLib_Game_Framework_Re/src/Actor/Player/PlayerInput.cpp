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
	if (Keyboard::getInstance().KeyTriggerDown(Keycode::C))
		return true;

	return false;
}

// �K�[�h���͂�������
bool PlayerInput::guard()
{
	return (GamePad::getInstance().ButtonStateDown(PadButton::R) || Keyboard::getInstance().KeyStateDown(Keycode::Z));
}

// �K�[�h���͂��I��������
bool PlayerInput::guard_end()
{
	return (GamePad::getInstance().ButtonStateUp(PadButton::R) && Keyboard::getInstance().KeyStateUp(Keycode::Z));
}

// �����͂�������
bool PlayerInput::evasion()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::A) || Keyboard::getInstance().KeyTriggerDown(Keycode::X));
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

// ���X�e�B�b�N�ɂ��ړ�
Vector2 PlayerInput::L_stick_move()
{
	// �L�[�{�[�h�ňړ����Ă���ꍇ�͔������Ȃ��iVector2::Zero��Ԃ��j
	if (keyboard_move())	return Vector2::Zero;

	return (GamePad::getInstance().L_Stick());
}

// ���X�e�B�b�N�ňړ����Ă��邩
bool PlayerInput::gamepad_move()
{
	// ���X�e�B�b�N�̓��͂�0�łȂ���΁ATrue��Ԃ�
	return L_stick_move() != Vector2::Zero;
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

// �Q�[���I��
bool PlayerInput::game_end()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::Back) || Keyboard::getInstance().KeyTriggerDown(Keycode::Escape));
}