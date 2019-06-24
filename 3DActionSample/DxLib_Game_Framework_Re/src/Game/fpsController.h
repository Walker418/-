#ifndef FPS_CONTROLLER_H_
#define FPS_CONTROLLER_H_

// fps制御クラス
class fpsController
{
public:
	// デフォルトコンストラクタ
	fpsController() = default;
	// 更新
	bool update();
	// 描画
	void draw();
	// 待機
	void wait();

private:
	int start_time_ = 0;		// ゲームが起動した時間
	int count_ = 0;				// フレーム数カウンター
	float fps_ = 0;				// 現在のfps

	static const int N = 60;	// 平均を取るサンプル数
	static const int FPS = 60;	// 設定したfps
};

#endif // !FPS_CONTROLLER_H_