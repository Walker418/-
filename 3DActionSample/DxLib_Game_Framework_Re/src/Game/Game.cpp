#include "Game.h"
#include <DxLib.h>

#include "../Graphic/Graphics2D.h"
#include "../Graphic/Graphics3D.h"
#include "../Graphic/SkeletalMesh.h"
#include "../Math/Collision/CollisionMesh.h"
#include "../Field/Skybox.h"
#include "../Graphic/Billboard.h"
#include "../Graphic/Shader/ShaderManager.h"
#include "../Sound/Sound.h"
#include "../Input/GamePad.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"
#include "../Actor/Player/PlayerInput.h"

// �N���X�F�Q�[���A�v���P�[�V����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
Game::Game(int width, int height, bool full_screen) :
	window_width_{ width }, window_height_{ height }, is_full_screen_{ full_screen }
{ }

// ���s
int Game::run()
{
	// ���O���o�͂��Ȃ��悤��
	SetOutApplicationLogValidFlag(FALSE);
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
	// �o�C���j�A�t�B���^�[��L���ɂ���
	SetDrawMode(DX_DRAWMODE_BILINEAR);

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
	ShaderManager::finalize();
	Billboard::finalize();
	SkeletalMesh::finalize();
	CollisionMesh::finalize();
	Skybox::finalize();
	Graphics2D::finalize();
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