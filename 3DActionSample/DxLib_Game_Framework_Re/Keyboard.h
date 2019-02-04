#ifndef KEYBOARD_H_
#define KEYBOARD_H_

// クラス：キーボード入力
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Keyboard
{
public:
	// キーボードのキーの定義
	static const int Esc;			// エスケープ
	static const int Enter;			// エンター
	static const int Space;			// スペース
	static const int LeftCtrl;		// 左コントロール
	static const int RightCtrl;		// 右コントロール
	static const int LeftShift;		// 左シフト
	static const int RightShift;	// 右シフト

	static const int Up;			// 上
	static const int Down;			// 下
	static const int Left;			// 左
	static const int Right;			// 右

	// 以下はテンキー
	static const int Num0;
	static const int Num1;
	static const int Num2;
	static const int Num3;
	static const int Num4;
	static const int Num5;
	static const int Num6;
	static const int Num7;
	static const int Num8;
	static const int Num9;

	// 以下はアルファベット
	static const int A;
	static const int B;
	static const int C;
	static const int D;
	static const int E;
	static const int F;
	static const int G;
	static const int H;
	static const int I;
	static const int J;
	static const int K;
	static const int L;
	static const int M;
	static const int N;
	static const int O;
	static const int P;
	static const int Q;
	static const int R;
	static const int S;
	static const int T;
	static const int U;
	static const int V;
	static const int W;
	static const int X;
	static const int Y;
	static const int Z;

	// 以下は数字
	static const int One;			// 1
	static const int Two;			// 2
	static const int Three;			// 3
	static const int Four;			// 4
	static const int Five;			// 5
	static const int Six;			// 6
	static const int Seven;			// 7
	static const int Eight;			// 8
	static const int Nine;			// 9
	static const int Zero;			// 0


	// 初期化
	static void initialize();
	// 更新
	static void update();
	// キーが押されているか
	static bool state(int key);
	// キーが押されたか
	static bool trigger(int key);
	// キーが離されたか
	static bool release(int key);
	// 終了処理
	static void finalize();

private:
	// 今回のキー入力状態
	static int current_state_;
	// 前回のキー入力状態
	static int previous_state_;
};

#endif // !KEYBOARD_H_