#include "RenderTarget.h"

// レンダーターゲットクラス

// コンストラクタ
RenderTarget::RenderTarget(int width, int height, bool hdr, bool z_buffer, int bit_depth, int channel) :
	width_{ width }, height_{ height }
{
	if (hdr)
	{
		// 浮動小数点テクスチャの場合
		SetDrawValidFloatTypeGraphCreateFlag(TRUE);
		SetCreateDrawValidGraphChannelNum(channel);
		SetCreateGraphChannelBitDepth(bit_depth);
	}
	else
	{
		SetDrawValidFloatTypeGraphCreateFlag(FALSE);
		SetCreateGraphColorBitDepth(bit_depth);
	}

	// Zバッファ作成フラグを設定
	SetDrawValidGraphCreateZBufferFlag(z_buffer ? TRUE : FALSE);
	// レンダーターゲット用のテクスチャを作成
	texture_ = MakeScreen(width_, height_, TRUE);
	// 設定をデフォルトに戻す
	SetDrawValidGraphCreateZBufferFlag(TRUE);
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateDrawValidGraphChannelNum(0);
	SetCreateGraphChannelBitDepth(0);
	SetCreateGraphColorBitDepth(4);
	// レンダーターゲット用のポリゴンデータを作成
	const float w{ static_cast<float>(width_) };
	const float h{ static_cast<float>(height_) };
	vertices_[0] = { { 0, 0, 0 }, 1,{ 255, 255, 255, 255 }, {0, 0, 0, 0 }, 0, 0, 0, 0 };	// 左上
	vertices_[1] = { { w, 0, 0 }, 1,{ 255, 255, 255, 255 }, {0, 0, 0, 0 }, 1, 0, 1, 0 };	// 右上
	vertices_[2] = { { 0, h, 0 }, 1,{ 255, 255, 255, 255 }, {0, 0, 0, 0 }, 0, 1, 0, 1 };	// 左下
	vertices_[3] = { { w, h, 0 }, 1,{ 255, 255, 255, 255 }, {0, 0, 0, 0 }, 1, 1, 1, 1 };	// 右下
}

// デストラクタ
RenderTarget::~RenderTarget()
{
	DeleteGraph(texture_);
}

// 開始
void RenderTarget::begin()
{
	prev_draw_screen_ = GetDrawScreen();
	SetDrawScreen(texture_);
}

// 終了
void RenderTarget::end()
{
	SetDrawScreen(prev_draw_screen_);
}

// 描画
void RenderTarget::draw() const
{
	DrawPrimitive2DToShader(vertices_, 4, DX_PRIMTYPE_TRIANGLESTRIP);
}

// 幅の取得
int RenderTarget::width() const
{
	return width_;
}

// 高さの取得
int RenderTarget::height() const
{
	return height_;
}

// テクスチャの取得
int RenderTarget::texture() const
{
	return texture_;
}