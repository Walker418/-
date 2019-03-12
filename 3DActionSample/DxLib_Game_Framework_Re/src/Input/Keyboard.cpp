#include "Keyboard.h"

// キーボード入力クラス

// コンストラクタ
Keyboard::Keyboard()
{
	// 全てのキー状態を初期化する
	for (int i = 0; i < 256; ++i)
	{
		m_onkey_[i] = 0;
		m_offkey_[i] = 0;
	}
}

// 特定のキーが押されたか
bool Keyboard::KeyTriggerDown(UINT keycode)
{
	return m_onkey_[keycode] == 1;
}

// 特定のキーが離されたか
bool Keyboard::KeyTriggerUp(UINT keycode)
{
	return m_offkey_[keycode] == 1;
}

// 特定のキーが押されているか
bool Keyboard::KeyStateDown(UINT keycode)
{
	return m_keycode_[keycode] == 1;
}

// 特定のキーが離されているか
bool Keyboard::KeyStateUp(UINT keycode)
{
	return m_keycode_[keycode] == 0;
}

// いずれかのキーが押されたか
bool Keyboard::AnyKeyTriggerDown(UINT keycode)
{
	for (int i = 0; i < 256; ++i)
	{
		if (m_onkey_[i] == 1)	return true;
	}

	return false;
}

// いずれかのキーが離されたか
bool Keyboard::AnyKeyTriggerUp(UINT keycode)
{
	for (int i = 0; i < 256; ++i)
	{
		if (m_offkey_[i] == 1)	return true;
	}

	return false;
}

// いずれかのキーが押されているか
bool Keyboard::AnyKeyStateDown(UINT keycode)
{
	for (int i = 0; i < 256; ++i)
	{
		if (m_keycode_[i] == 1)	return true;
	}

	return false;
}

// いずれかのキーが離されているか
bool Keyboard::AnyKeyStateUp(UINT keycode)
{
	for (int i = 0; i < 256; ++i)
	{
		if (m_keycode_[i] == 0)	return true;
	}

	return false;
}

// 更新
void Keyboard::update()
{
	// 全てのキーの状態を取得
	GetHitKeyStateAll(m_keycode_);

	// 全てのキーの状態を更新
	for (int i = 0; i < 256; ++i)
	{
		update_key(m_keycode_[i], i);
	}
}

// キーの状態を更新
void Keyboard::update_key(char key, int num)
{
	// 押されていれば
	if (key != 0)
	{
		m_offkey_[num] = 0;
		m_onkey_[num]++;
	}
	// 押されていなければ
	else
	{
		m_onkey_[num] = 0;
		m_offkey_[num]++;
	}
}