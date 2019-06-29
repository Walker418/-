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

// クラス：ゲームアプリケーション
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

//------------------------------------------------------------
// ウィンドウの設定定数
//------------------------------------------------------------

const char *WindowName = "モンスターコロシアム";	// ウィンドウ名

//------------------------------------------------------------

// コンストラクタ
Game::Game(int width, int height, bool full_screen) :
	window_width_{ width }, window_height_{ height }, is_full_screen_{ full_screen }
{ }

// 実行
int Game::run()
{
	//------------------------------------------------------------
	// ウィンドウの基本設定
	//------------------------------------------------------------

	// ログを出力しないように
	SetOutApplicationLogValidFlag(FALSE);
	// ウィンドウ名称を設定
	SetMainWindowText(WindowName);
	// ウィンドウサイズを設定
	SetWindowSize(window_width_, window_height_);
	// グラフィックモードを設定
	SetGraphMode(window_width_, window_height_, 32);
	// ウインドウモードかどうかを決定
	ChangeWindowMode(is_full_screen_ ? FALSE : TRUE);
	// DXライブラリを初期化
	if (DxLib_Init() == -1) return -1;	// 初期化に失敗
	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);
	// バイリニアフィルターを有効化
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	// マウスカーソルを隠す
	Mouse::getInstance().hide_cursor();

	//------------------------------------------------------------

	//------------------------------------------------------------
	// 各要素の初期化
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

	// 開始
	start();

	//------------------------------------------------------------
	// メインループ
	//------------------------------------------------------------

	while (ProcessMessage() == 0 && fps_.update() && is_running())
	{
		// ゲームパッドの状態を更新
		GamePad::getInstance().update();
		// キーボードの状態を更新
		Keyboard::getInstance().update();
		// マウスの状態を更新
		Mouse::getInstance().update();

		update(1.0f);		// ゲームを更新
		ClearDrawScreen();	// 画面をクリア
		draw();				// 画面を描画
		ScreenFlip();		// 裏画面の内容を表画面に反映
		fps_.wait();
	}

	//------------------------------------------------------------

	// 終了
	end();

	//------------------------------------------------------------
	// 各要素の終了処理
	//------------------------------------------------------------

	// 終了処理
	ShaderManager::finalize();
	Billboard::finalize();
	SkeletalMesh::finalize();
	CollisionMesh::finalize();
	Skybox::finalize();
	Graphics2D::finalize();
	Graphics3D::finalize();
	Sound::finalize();

	//------------------------------------------------------------

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