#ifndef BILLBOARD_H_
#define BILLBOARD_H_

#include <string>
#include <DxLib.h>
#include "TextureAsset.h"

// クラス：ビルボード
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Billboard
{
public:
	// 初期化
	static void initialize();
	// 終了処理
	static void finalize();
	// 読み込み
	static bool load(int id, const std::string& file_name);
	// 削除
	static void erase(int id);
	// バインド
	static void bind(int id);
	// 描画
	static void draw(const VECTOR& position, float size, float cx = 0.5f, float cy = 0.5f, float angle = 0.0f);

private:
	// バインド中のイメージ
	static int			image_;
	// テクスチャアセット
	static TextureAsset asset_;
};

#endif // !BILLBOARD_H_