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

// クラス：ゲームアプリケーション
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
Game::Game(int width, int height, bool full_screen) :
	window_width_{ width }, window_height_{ height }, is_full_screen_{ full_screen }
{ }

// 実行
int Game::run()
{
	// ウィンドウサイズを設定
	SetWindowSize(window_width_, window_height_);
	// フルスクリーンモード時の解像度を設定
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
	// グラフモードを設定
	SetGraphMode(window_width_, window_height_, 32);
	// ウィンドウモードなのか
	ChangeWindowMode(is_full_screen_ ? FALSE : TRUE);
	// DXライブラリを初期化
	if (DxLib_Init() == -1) return -1;	// 初期化に失敗
	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// 初期化
	Graphics3D::initialize();
	SkeletalMesh::initialize();
	CollisionMesh::initialize();
	Skybox::initialize();
	Billboard::initialize();
	Sound::initialize();

	// マウスカーソルを隠す
	Mouse::getInstance().hide_cursor();

	// 開始
	start();

	// メインループ
	while (ProcessMessage() == 0 && is_running())
	{
		// ゲームパッドを更新
		GamePad::getInstance().update();
		// キーボードを更新
		Keyboard::getInstance().update();
		// マウスを更新
		Mouse::getInstance().update();
		// 更新
		update(1.0f);
		// 画面をクリア
		ClearDrawScreen();
		// 描画
		draw();
		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}

	// 終了
	end();

	// 終了処理
	Billboard::finalize();
	SkeletalMesh::finalize();
	CollisionMesh::finalize();
	Skybox::finalize();
	Graphics3D::finalize();
	Sound::finalize();

	// DxLibを終了
	DxLib_End();

	// プログラムを終了
	return 0;
}

// 開始
void Game::start()
{ }

// 更新
void Game::update(float delta_time)
{ }

// 描画
void Game::draw()
{ }

// 終了
void Game::end()
{ }

// 実行中なのか
bool Game::is_running() const
{
	// Escapeキーで強制終了
	return PlayerInput::game_end() == 0;
}