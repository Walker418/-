#include "GamePad.h"
#include <string>

// �Q�[���p�b�h���̓N���X

// �R���X�g���N�^
GamePad::GamePad() :
	current_L_stick_{ Vector2::Zero }, current_R_stick_{ Vector2::Zero }
{
	// ���͏�Ԃ��N���A
	m_inputs_.clear();
	m_onbuttons_.clear();
	m_offbuttons_.clear();
	m_pads_.clear();
	m_onPOVbuttons_.clear();

	// ���͔����������
	m_inputs_.push_back(PAD_INPUT_DOWN);
	m_inputs_.push_back(PAD_INPUT_UP);
	m_inputs_.push_back(PAD_INPUT_LEFT);
	m_inputs_.push_back(PAD_INPUT_RIGHT);
	m_inputs_.push_back(PAD_INPUT_1);
	m_inputs_.push_back(PAD_INPUT_2);
	m_inputs_.push_back(PAD_INPUT_3);
	m_inputs_.push_back(PAD_INPUT_4);
	m_inputs_.push_back(PAD_INPUT_5);
	m_inputs_.push_back(PAD_INPUT_6);
	m_inputs_.push_back(PAD_INPUT_7);
	m_inputs_.push_back(PAD_INPUT_8);
	m_inputs_.push_back(PAD_INPUT_9);
	m_inputs_.push_back(PAD_INPUT_10);
	m_inputs_.push_back(PAD_INPUT_11);
	m_inputs_.push_back(PAD_INPUT_12);

	// �p�b�h�̐ڑ�����������
	m_pads_.push_back(DX_INPUT_PAD1);
	m_pads_.push_back(DX_INPUT_PAD2);
	m_pads_.push_back(DX_INPUT_PAD3);
	m_pads_.push_back(DX_INPUT_PAD4);

	// ���͏�Ԃ�������
	for (auto& j : m_pads_)
	{
		for (auto& i : m_inputs_)
		{
			m_onbuttons_[j][i] = 0;
			m_offbuttons_[j][i] = 0;
		}
		m_onPOVbuttons_[j] = 0;
	}
}

// ����̃{�^���������ꂽ��
bool GamePad::ButtonTriggerDown(UINT button, int pad)
{
	Exception(pad, button);

	return m_onbuttons_[pad][button] == 1;
}

// ����̃{�^���������ꂽ��
bool GamePad::ButtonTriggerUp(UINT button, int pad)
{
	Exception(pad, button);

	return m_offbuttons_[pad][button] == 1;
}

// ����̃{�^����������Ă��邩
bool GamePad::ButtonStateDown(UINT button, int pad)
{
	Exception(pad, button);

	return m_onbuttons_[pad][button] > 0;
}

// ����̃{�^����������Ă��邩
bool GamePad::ButtonStateUp(UINT button, int pad)
{
	Exception(pad, button);

	return m_offbuttons_[pad][button] > 0;
}

// �����ꂩ�̃{�^���������ꂽ��
bool GamePad::AnyButtonTriggerDown(int pad)
{
	for (auto& button : m_inputs_)
	{
		Exception(pad, button);

		if (m_onbuttons_[pad][button] == 1)
		{
			return true;
		}
	}

	return false;
}

// �����ꂩ�̃{�^���������ꂽ��
bool GamePad::AnyButtonTriggerUp(int pad)
{
	for (auto& button : m_inputs_)
	{
		Exception(pad, button);

		if (m_offbuttons_[pad][button] == 1)
		{
			return true;
		}
	}

	return false;
}

// �����ꂩ�̃{�^����������Ă��邩
bool GamePad::AnyButtonStateDown(int pad)
{
	for (auto& button : m_inputs_)
	{
		Exception(pad, button);

		if (m_onbuttons_[pad][button] > 0)
		{
			return true;
		}
	}

	return false;
}

// �����ꂩ�̃{�^����������Ă��邩
bool GamePad::AnyButtonStateUp(int pad)
{
	for (auto& button : m_inputs_)
	{
		Exception(pad, button);

		if (m_offbuttons_[pad][button] > 0)
		{
			return true;
		}
	}

	return false;
}

// POV�̓��͂��ꂽ�u�Ԃ̕������擾
int GamePad::POVTriggerDown(int pad)
{
	// ���͂��Ȃ��ꍇ�A-1��Ԃ�
	if (m_onPOVbuttons_[pad] != 1)
		return -1;

	// ���͂���Ă�������擾
	return POVStateDown(pad);
}

// POV�̓��͂���Ă�������擾
int GamePad::POVStateDown(int pad)
{
	return GetJoypadPOVState(pad, 0) / 100;
}

// L�X�e�B�b�N�̓��͋���擾
Vector2 GamePad::L_Stick(int pad)
{
	int x = 0, y = 0;
	GetJoypadAnalogInput(&x, &y, pad);

	return Vector2((float)x / 1000.0f, (float)-y / 1000.0f);
}

// ���t���[����L�X�e�B�b�N�̓��͂��擾
Vector2 GamePad::GetCurrent_L_Stick()
{
	return current_L_stick_;
}

// �O�t���[����L�X�e�B�b�N�̓��͂��擾
Vector2 GamePad::GetPrevious_L_Stick()
{
	return previous_L_stick_;
}

// R�X�e�B�b�N�̓��͋���擾
Vector2 GamePad::R_Stick(int pad)
{
	int x = 0, y = 0;
	GetJoypadAnalogInputRight(&x, &y, pad);
	return Vector2((float)x / 1000.0f, (float)-y / 1000.0f);
}

// ���t���[����R�X�e�B�b�N�̓��͂��擾
Vector2 GamePad::GetCurrent_R_Stick()
{
	return current_R_stick_;
}

// �O�t���[����R�X�e�B�b�N�̓��͂��擾
Vector2 GamePad::GetPrevious_R_Stick()
{
	return previous_R_stick_;
}

// �Q�[���p�b�h�̐U�����J�n������
void GamePad::VibrationStart(int power, int time, int pad)
{
	StartJoypadVibration(pad, power, time);
}

// �Q�[���p�b�h�̐U�����~������
void GamePad::VibrationStop(int pad)
{
	StopJoypadVibration(pad);
}

// �X�V
void GamePad::update()
{
	// �e�{�^���̏�Ԃ��X�V
	for (auto& pad : m_pads_)
	{
		for (auto& button : m_inputs_)
		{
			if ((GetJoypadInputState(pad) &button) == 0)
			{
				m_onbuttons_[pad][button] = 0;
				m_offbuttons_[pad][button]++;
			}
			else
			{
				m_offbuttons_[pad][button] = 0;
				m_onbuttons_[pad][button]++;
			}
		}
		if (GetJoypadPOVState(pad, 0) == -1)
		{
			m_onPOVbuttons_[pad] = 0;
		}
		else
		{
			m_onPOVbuttons_[pad]++;
		}
	}

	// L�X�e�B�b�N�̏�Ԃ��X�V
	previous_L_stick_ = current_L_stick_;
	int x = 0, y = 0;
	GetJoypadAnalogInput(&x, &y, 1);
	current_L_stick_ = Vector2((float)x / 1000.0f, (float)-y / 1000.0f);

	// R�X�e�B�b�N�̏�Ԃ��X�V
	previous_R_stick_ = current_R_stick_;
	GetJoypadAnalogInputRight(&x, &y, 1);
	current_R_stick_ = Vector2((float)x / 1000.0f, (float)-y / 1000.0f);
}

void GamePad::Exception(int pad, int button)
{
	if (m_onbuttons_.find(pad) == m_onbuttons_.end())
		throw std::string("�c�O�Ȃ���A�w�肵���p�b�h�ԍ��͎g�p�ł��܂���");

	if (m_onbuttons_[pad].find(button) == m_onbuttons_[pad].end())
		throw std::string("�c�O�Ȃ���A�w�肵���{�^���ԍ��͎g�p�ł��܂���");
}