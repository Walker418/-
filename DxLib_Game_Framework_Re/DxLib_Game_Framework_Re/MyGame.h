#ifndef MY_GAME_H_
#define MY_GAME_H_

#include "Game.h"
#include "Field.h"
#include "AnimatedMesh.h"

// �N���X�F3D�Q�[���t���[�����[�N�e�X�g
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class MyGame : public Game
{
private:
	// �X�e�[�W
	Field*			field_{ nullptr };
	// �L�����N�^�[
	AnimatedMesh*	mesh_{ nullptr };

public:
	// �R���X�g���N�^
	MyGame();

private:
	// �J�n
	void start() override;
	// �X�V
	void update(float delta_time) override;
	// �`��
	void draw() override;
	// �I��
	void end() override;

};

#endif // !MY_GAME_H_