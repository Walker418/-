#ifndef GAME_H_
#define GAME_H_

// main関数をエントリーポイントにする
#pragma comment(linker, "/entry:mainCRTStartup")

// クラス：ゲームアプリケーション
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Game
{
public:
	// デフォルトコンストラクタ
	Game() = default;
	// コンストラクタ
	Game(int width, int height, bool full_screen);
	// 実行
	int run();

	// コピー禁止
	Game(const Game& other) = delete;
	Game& operator = (const Game& other) = delete;

private:
	// 開始
	virtual void start();
	// 更新
	virtual void update(float delta_time);
	// 描画
	virtual void draw();
	// 終了
	virtual void end();
	// 実行中なのか
	virtual bool is_running() const;

private:
	// ウィンドウの幅
	int window_width_{ 640 };
	// ウィンドウの高さ
	int window_height_{ 480 };
	// フルスクリーンモード
	bool is_full_screen_{ false };
};

#endif // !GAME_H_