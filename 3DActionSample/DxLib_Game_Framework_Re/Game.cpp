#include "Game.h"
#include <DxLib.h>

#include "Graphics3D.h"
#include "SkeletalMesh.h"
#include "CollisionMesh.h"
#include "Skybox.h"
#include "Billboard.h"

#include "Sound.h"
#include "GamePad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "PlayerInput.h"

// �N���X�F�Q�[���A�v���P�[�V����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
Game::Game(int width, int height, bool full_screen) :
	window_width_{ width }, window_height_{ height }, is_full_screen_{ full_screen }
{ }

// ���s
int Game::run()
{
	// �E�B���h�E�T�C�Y��ݒ�
	SetWindowSize(window_width_, window_height_);
	// �t���X�N���[�����[�h���̉𑜓x��ݒ�
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
	// �O���t���[�h��ݒ�
	SetGraphMode(window_width_, window_height_, 32);
	// �E�B���h�E���[�h�Ȃ̂�
	ChangeWindowMode(is_full_screen_ ? FALSE : TRUE);
	// DX���C�u������������
	if (DxLib_Init() == -1) return -1;	// �������Ɏ��s
	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	// ������
	Graphics3D::initialize();
	SkeletalMesh::initialize();
	CollisionMesh::initialize();
	Skybox::initialize();
	Billboard::initialize();
	Sound::initialize();

	// �}�E�X�J�[�\�����B��
	Mouse::getInstance().hide_cursor();

	// �J�n
	start();

	// ���C�����[�v
	while (ProcessMessage() == 0 && is_running())
	{
		// �Q�[���p�b�h���X�V
		GamePad::getInstance().update();
		// �L�[�{�[�h���X�V
		Keyboard::getInstance().update();
		// �}�E�X���X�V
		Mouse::getInstance().update();
		// �X�V
		update(1.0f);
		// ��ʂ��N���A
		ClearDrawScreen();
		// �`��
		draw();
		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}

	// �I��
	end();

	// �I������
	Billboard::finalize();
	SkeletalMesh::finalize();
	CollisionMesh::finalize();
	Skybox::finalize();
	Graphics3D::finalize();
	Sound::finalize();

	// DxLib���I��
	DxLib_End();

	// �v���O�������I��
	return 0;
}

// �J�n
void Game::start()
{ }

// �X�V
void Game::update(float delta_time)
{ }

// �`��
void Game::draw()
{ }

// �I��
void Game::end()
{ }

// ���s���Ȃ̂�
bool Game::is_running() const
{
	// Escape�L�[�ŋ����I��
	return PlayerInput::game_end() == 0;
}