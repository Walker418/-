#include "GamePad.h"
#include <DxLib.h>

// �N���X�F�Q�[���p�b�h����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �Q�[���p�b�h�̃{�^���̒�`
const int GamePad::Left{ PAD_INPUT_LEFT };		// ��
const int GamePad::Right{ PAD_INPUT_RIGHT };	// �E
const int GamePad::Up{ PAD_INPUT_UP };			// ��
const int GamePad::Down{ PAD_INPUT_DOWN };		// ��
const int GamePad::A{ PAD_INPUT_1 };			// A
const int GamePad::B{ PAD_INPUT_2 };			// B
const int GamePad::X{ PAD_INPUT_3 };			// X
const int GamePad::Y{ PAD_INPUT_4 };			// Y
const int GamePad::L{ PAD_INPUT_5 };			// L
const int GamePad::R{ PAD_INPUT_6 };			// R
const int GamePad::Back{ PAD_INPUT_9 };			// Back
const int GamePad::Start{ PAD_INPUT_10 };		// Start

// ����̓��͏��
int GamePad::current_state_{ 0 };
// �O��̓��͏��
int GamePad::previous_state_{ 0 };

// ������
void GamePad::initialize()
{
	current_state_ = 0;
	previous_state_ = 0;
}

// �X�V
void GamePad::update()
{
	previous_state_ = current_state_;
	current_state_ = GetJoypadInputState(DX_INPUT_KEY_PAD1);
}

// �{�^����������Ă��邩
bool GamePad::state(int button)
{
	return (current_state_ &button) != 0;
}

// �{�^���������ꂽ��
bool GamePad::trigger(int button)
{
	return (current_state_ & ~previous_state_ &button) != 0;
}

// �{�^���������ꂽ��
bool GamePad::release(int button)
{
	return (~current_state_ & previous_state_ &button) != 0;
}

// �I������
void GamePad::finalize()
{
	// �������Ɠ�������
	initialize();
}