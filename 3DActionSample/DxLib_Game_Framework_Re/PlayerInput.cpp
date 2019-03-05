#include "PlayerInput.h"
#include "GamePad.h"
#include "Keyboard.h"

// �N���X�F�v���C���[����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �U�����͂�������
bool PlayerInput::attack()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::B) || Keyboard::getInstance().KeyTriggerDown(Keycode::Space));
}

// �K�[�h���͂�������
bool PlayerInput::guard()
{
	return (GamePad::getInstance().ButtonStateDown(PadButton::R) || Keyboard::getInstance().KeyStateDown(Keycode::L_Control));
}

// �K�[�h���͂��I��������
bool PlayerInput::guard_end()
{
	return (GamePad::getInstance().ButtonStateUp(PadButton::R) && Keyboard::getInstance().KeyStateUp(Keycode::L_Control));
}

// �����͂�������
bool PlayerInput::skip()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::A) || Keyboard::getInstance().KeyTriggerDown(Keycode::L_Shift));
}

// �O�ړ�
bool PlayerInput::move_forward()
{
	// return (GamePad::getInstance().GetCurrent_L_Stick().y > 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::W));
	return Keyboard::getInstance().KeyStateDown(Keycode::W);
}

// ��ړ�
bool PlayerInput::move_backward()
{
	// return (GamePad::getInstance().GetCurrent_L_Stick().y < 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::S));
	return Keyboard::getInstance().KeyStateDown(Keycode::S);
}

// ���ړ�
bool PlayerInput::move_left()
{
	// return (GamePad::getInstance().GetCurrent_L_Stick().x < 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::A));
	return Keyboard::getInstance().KeyStateDown(Keycode::A);
}

// �E�ړ�
bool PlayerInput::move_right()
{
	// return (GamePad::getInstance().GetCurrent_L_Stick().x > 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::D));
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