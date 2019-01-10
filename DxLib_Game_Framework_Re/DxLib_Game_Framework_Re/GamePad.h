#ifndef GAME_PAD_H_
#define GAME_PAD_H_

// クラス：ゲームパッド入力
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class GamePad
{
public:
	// ゲームパッドのボタンの定義
	static const int Left;
	static const int Right;
	static const int Up;
	static const int Down;
	static const int A;
	static const int B;
	static const int X;
	static const int Y;
	static const int Back;
	static const int Start;

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