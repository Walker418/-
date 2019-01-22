#ifndef GAME_PAD_H_
#define GAME_PAD_H_

// クラス：ゲームパッド入力
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class GamePad
{
public:
	// ゲームパッドのボタンの定義
	static const int Left;		// 左
	static const int Right;		// 右
	static const int Up;		// 上
	static const int Down;		// 下
	static const int A;			// A
	static const int B;			// B
	static const int X;			// X
	static const int Y;			// Y
	static const int L;			// L
	static const int R;			// R
	static const int Back;		// Back
	static const int Start;		// Start

	// 初期化
	static void initialize();
	// 更新
	static void update();
	// ボタンが押されているか
	static bool state(int button);
	// ボタンが押されたか
	static bool trigger(int button);
	// ボタンが離されたか
	static bool release(int button);
	// 終了処理
	static void finalize();

private:
	// 今回の入力状態
	static int current_state_;
	// 前回の入力状態
	static int previous_state_;
};

#endif // !GAME_PAD_H_