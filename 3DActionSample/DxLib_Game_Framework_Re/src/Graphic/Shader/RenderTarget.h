#ifndef RENDER_TARGET_H_
#define RENDER_TARGET_H_

#include <DxLib.h>

// クラス：レンダーターゲット
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class RenderTarget
{
public:
	// コンストラクタ
	RenderTarget(int width, int height, bool hdr = false, bool z_
		= true, int bit_depth = 32, int channel = 4);
	// デストラクタ
	~RenderTarget();
	// 開始
	void begin();
	// 終了
	void end();
	// 描画
	void draw() const;
	// 幅の取得
	int width() const;
	// 高さの取得
	int height() const;
	// テクスチャの取得
	int texture() const;

private:
	int				width_;						// 幅
	int				height_;					// 高さ
	int				texture_;					// テクスチャ
	VERTEX2DSHADER	vertices_[4];				// 頂点データ
	int				prev_draw_screen_{ -1 };	// 前回の描画スクリーン
};

#endif // !RENDER_TARGET_H_