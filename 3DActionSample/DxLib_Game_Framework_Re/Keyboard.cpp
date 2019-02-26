#include "Keyboard.h"

// �L�[�{�[�h���̓N���X

// �R���X�g���N�^
Keyboard::Keyboard()
{
	// �S�ẴL�[��Ԃ�����������
	for (int i = 0; i < 256; ++i)
	{
		m_onkey_[i] = 0;
		m_offkey_[i] = 0;
	}
}

// ����̃L�[�������ꂽ��
bool Keyboard::KeyTriggerDown(UINT keycode)
{
	return m_onkey_[keycode] == 1;
}

// ����̃L�[�������ꂽ��
bool Keyboard::KeyTriggerUp(UINT keycode)
{
	return m_offkey_[keycode] == 1;
}

// ����̃L�[��������Ă��邩
bool Keyboard::KeyStateDown(UINT keycode)
{
	return m_keycode_[keycode] == 1;
}

// ����̃L�[��������Ă��邩
bool Keyboard::KeyStateUp(UINT keycode)
{
	return m_keycode_[keycode] == 0;
}

// �����ꂩ�̃L�[�������ꂽ��
bool Keyboard::AnyKeyTriggerDown(UINT keycode)
{
	for (int i = 0; i < 256; ++i)
	{
		if (m_onkey_[i] == 1)	return true;
	}

	return false;
}

// �����ꂩ�̃L�[�������ꂽ��
bool Keyboard::AnyKeyTriggerUp(UINT keycode)
{
	for (int i = 0; i < 256; ++i)
	{
		if (m_offkey_[i] == 1)	return true;
	}

	return false;
}

// �����ꂩ�̃L�[��������Ă��邩
bool Keyboard::AnyKeyStateDown(UINT keycode)
{
	for (int i = 0; i < 256; ++i)
	{
		if (m_keycode_[i] == 1)	return true;
	}

	return false;
}

// �����ꂩ�̃L�[��������Ă��邩
bool Keyboard::AnyKeyStateUp(UINT keycode)
{
	for (int i = 0; i < 256; ++i)
	{
		if (m_keycode_[i] == 0)	return true;
	}

	return false;
}

// �X�V
void Keyboard::update()
{
	// �S�ẴL�[�̏�Ԃ��擾
	GetHitKeyStateAll(m_keycode_);

	// �S�ẴL�[�̏�Ԃ��X�V
	for (int i = 0; i < 256; ++i)
	{
		update_key(m_keycode_[i], i);
	}
}

// �L�[�̏�Ԃ��X�V
void Keyboard::update_key(char key, int num)
{
	// ������Ă����
	if (key != 0)
	{
		m_offkey_[num] = 0;
		m_onkey_[num]++;
	}
	// ������Ă��Ȃ����
	else
	{
		m_onkey_[num] = 0;
		m_offkey_[num]++;
	}
}