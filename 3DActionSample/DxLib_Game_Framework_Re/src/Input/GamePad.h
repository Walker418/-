#ifndef GAME_PAD_H_
#define GAME_PAD_H_

#include <DxLib.h>
#include "../Math/Vector2.h"
#include <unordered_map>
#include <vector>
#include <array>

// �ڑ����Ă���Q�[���p�b�h�̔ԍ�
namespace PadNo
{
	const UINT Pad1 = DX_INPUT_PAD1;
	const UINT Pad2 = DX_INPUT_PAD2;
	const UINT Pad3 = DX_INPUT_PAD3;
	const UINT Pad4 = DX_INPUT_PAD4;
}

// �Q�[���p�b�h�̃{�^��
// p.s.�\���L�[�Ə����Ă��邪���ۂ̓X�e�B�b�N�̂��Ƃ炵��
namespace PadButton
{
	const UINT Left = PAD_INPUT_LEFT;		// ���{�^��
	const UINT Right = PAD_INPUT_RIGHT;		// �E�{�^��
	const UINT Up = PAD_INPUT_UP;			// ��{�^��
	const UINT Down = PAD_INPUT_DOWN;		// ���{�^��
	const UINT A = PAD_INPUT_1;				// A�{�^��
	const UINT B = PAD_INPUT_2;				// B�{�^��
	const UINT X = PAD_INPUT_3;				// X�{�^��
	const UINT Y = PAD_INPUT_4;				// Y�{�^��
	const UINT L = PAD_INPUT_5;				// L�{�^��
	const UINT R = PAD_INPUT_6;				// R�{�^��
	const UINT Back = PAD_INPUT_7;			// Back�{�^��
	const UINT Start = PAD_INPUT_8;			// Start�{�^��
	const UINT NUM9 = PAD_INPUT_9;
	const UINT NUM10 = PAD_INPUT_10;
	const UINT NUM11 = PAD_INPUT_11;
	const UINT NUM12 = PAD_INPUT_12;
}

// �Q�[���p�b�h���̓N���X
class GamePad
{
private:
	// �R���X�g���N�^
	GamePad();
	// �f�X�g���N�^
	~GamePad() = default;

public:
	// �Q�[���p�b�h�N���X�̎擾
	static GamePad &getInstance()
	{
		static GamePad g;
		return g;
	}

	// ����̃{�^���������ꂽ��
	bool ButtonTriggerDown(UINT button, int pad = PadNo::Pad1);
	// ����̃{�^���������ꂽ��
	bool ButtonTriggerUp(UINT button, int pad = PadNo::Pad1);
	// ����̃{�^����������Ă��邩
	bool ButtonStateDown(UINT button, int pad = PadNo::Pad1);
	// ����̃{�^����������Ă��邩
	bool ButtonStateUp(UINT button, int pad = PadNo::Pad1);

	// �����ꂩ�̃{�^���������ꂽ��
	bool AnyButtonTriggerDown(int pad = PadNo::Pad1);
	// �����ꂩ�̃{�^���������ꂽ��
	bool AnyButtonTriggerUp(int pad = PadNo::Pad1);
	// �����ꂩ�̃{�^����������Ă��邩
	bool AnyButtonStateDown(int pad = PadNo::Pad1);
	// �����ꂩ�̃{�^����������Ă��邩
	bool AnyButtonStateUp(int pad = PadNo::Pad1);
	
	// POV�i�\���L�[�j�̓��͂��ꂽ�u�Ԃ̕������擾
	int POVTriggerDown(int pad = PadNo::Pad1);
	// POV�̓��͂���Ă�������擾
	int POVStateDown(int pad = PadNo::Pad1);

	// L�X�e�B�b�N�̓��͋���擾
	// (x = -1.0 ~ 1.0, y = -1.0 ~ 1.0)
	Vector2 L_Stick(int pad = PadNo::Pad1);
	// ���t���[����L�X�e�B�b�N�̓��͂��擾
	Vector2 GetCurrent_L_Stick();
	// �O�t���[����L�X�e�B�b�N�̓��͂��擾
	Vector2 GetPrevious_L_Stick();
	// R�X�e�B�b�N�̓��͋���擾
	// (x = -1.0 ~ 1.0, y = -1.0 ~ 1.0)
	Vector2 R_Stick(int pad = PadNo::Pad1);
	// ���t���[����R�X�e�B�b�N�̓��͂��擾
	Vector2 GetCurrent_R_Stick();
	// �O�t���[����R�X�e�B�b�N�̓��͂��擾
	Vector2 GetPrevious_R_Stick();

	// �Q�[���p�b�h�̐U�����J�n������
	void VibrationStart(int power, int time, int pad = PadNo::Pad1);
	// �Q�[���p�b�h�̐U�����~������
	void VibrationStop(int pad = PadNo::Pad1);

	// �X�V
	void update();

	// �R�s�[�֎~
	GamePad(const GamePad&) = delete;
	GamePad& operator = (const GamePad&) = delete;
	// �ړ��֎~
	GamePad(GamePad&&) = delete;
	GamePad& operator = (GamePad&&) = delete;

private:
	void Exception(int pad, int button);

private:
	// �{�^���̏�ԕۑ��p�}�b�v
	std::unordered_map<int, std::unordered_map<int, int>> m_onbuttons_;			// ������Ă���
	std::unordered_map<int, std::unordered_map<int, int>> m_offbuttons_;		// ������Ă��Ȃ�
	// �\���L�[�̏��
	std::unordered_map<int, int> m_onPOVbuttons_;
	// �Q�[���p�b�h�̔ԍ�
	std::vector<int> m_pads_;
	// �{�^���̓��͔���
	std::vector<int> m_inputs_;

	// ���t���[����L�X�e�B�b�N����
	Vector2 current_L_stick_;
	// �O�t���[����L�X�e�B�b�N����
	Vector2 previous_L_stick_;
	// ���t���[����R�X�e�B�b�N����
	Vector2 current_R_stick_;
	// �O�t���[����R�X�e�B�b�N����
	Vector2 previous_R_stick_;
};

#endif // !GAME_PAD_H_