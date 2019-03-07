#ifndef PLAYER_INPUT_H_
#define PLAYER_INPUT_H_

#include "Vector2.h"

// �N���X�F�v���C���[����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

class PlayerInput
{
public:
	// �U�����͂�������
	static bool attack();
	// �K�[�h���͂�������
	static bool guard();
	// �K�[�h���͂��I��������
	static bool guard_end();
	// �����͂�������
	static bool evasion();

	// �O�ړ�
	static bool move_forward();
	// ��ړ�
	static bool move_backward();
	// ���ړ�
	static bool move_left();
	// �E�ړ�
	static bool move_right();
	// �L�[�{�[�h�ňړ����Ă��邩
	static bool keyboard_move();

	// ���X�e�B�b�N�ɂ��ړ�
	static Vector2 L_stick_move();
	// ���X�e�B�b�N�ňړ����Ă��邩
	static bool gamepad_move();

	// �J��������]
	static bool camera_turn_left();
	// �J�����E��]
	static bool camera_turn_right();
	// �J�������]
	static bool camera_turn_up();
	// �J��������]
	static bool camera_turn_down();

	// �Q�[���I��
	static bool game_end();
};

#endif // !PLAYER_INPUT_H_