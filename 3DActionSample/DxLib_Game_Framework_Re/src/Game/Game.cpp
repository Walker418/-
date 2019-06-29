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

//------------------------------------------------------------
// �E�B���h�E�̐ݒ�萔
//------------------------------------------------------------

const char *WindowName = "�����X�^�[�R���V�A��";	// �E�B���h�E��

//------------------------------------------------------------

// �R���X�g���N�^
Game::Game(int width, int height, bool full_screen) :
	window_width_{ width }, window_height_{ height }, is_full_screen_{ full_screen }
{ }

// ���s
int Game::run()
{
	//------------------------------------------------------------
	// �E�B���h�E�̊�{�ݒ�
	//------------------------------------------------------------

	// ���O���o�͂��Ȃ��悤��
	SetOutApplicationLogValidFlag(FALSE);
	// �E�B���h�E���̂�ݒ�
	SetMainWindowText(WindowName);
	// �E�B���h�E�T�C�Y��ݒ�
	SetWindowSize(window_width_, window_height_);
	// �O���t�B�b�N���[�h��ݒ�
	SetGraphMode(window_width_, window_height_, 32);
	// �E�C���h�E���[�h���ǂ���������
	ChangeWindowMode(is_full_screen_ ? FALSE : TRUE);
	// DX���C�u������������
	if (DxLib_Init() == -1) return -1;	// �������Ɏ��s
	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);
	// �o�C���j�A�t�B���^�[��L����
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	// �}�E�X�J�[�\�����B��
	Mouse::getInstance().hide_cursor();

	//------------------------------------------------------------

	//------------------------------------------------------------
	// �e�v�f�̏�����
	//------------------------------------------------------------

	Graphics2D::initialize();
	Graphics3D::initialize();
	SkeletalMesh::initialize();
	CollisionMesh::initialize();
	Skybox::initialize();
	Billboard::initialize();
	ShaderManager::initialize();
	Sound::initialize();

	//------------------------------------------------------------

	// �J�n
	start();

	//------------------------------------------------------------
	// ���C�����[�v
	//------------------------------------------------------------

	while (ProcessMessage() == 0 && fps_.update() && is_running())
	{
		// �Q�[���p�b�h�̏�Ԃ��X�V
		GamePad::getInstance().update();
		// �L�[�{�[�h�̏�Ԃ��X�V
		Keyboard::getInstance().update();
		// �}�E�X�̏�Ԃ��X�V
		Mouse::getInstance().update();

		update(1.0f);		// �Q�[�����X�V
		ClearDrawScreen();	// ��ʂ��N���A
		draw();				// ��ʂ�`��
		ScreenFlip();		// ����ʂ̓��e��\��ʂɔ��f
		fps_.wait();
	}

	//------------------------------------------------------------

	// �I��
	end();

	//------------------------------------------------------------
	// �e�v�f�̏I������
	//------------------------------------------------------------

	// �I������
	ShaderManager::finalize();
	Billboard::finalize();
	SkeletalMesh::finalize();
	CollisionMesh::finalize();
	Skybox::finalize();
	Graphics2D::finalize();
	Graphics3D::finalize();
	Sound::finalize();

	//------------------------------------------------------------

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