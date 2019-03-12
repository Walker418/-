#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <string>
#include "../Graphic/ModelAsset.h"

// クラス：スカイボックス
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Skybox
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
	// メッシュのバインド
	static void bind(int id);
	// 描画
	static void draw();

private:
	// バインドのモデル
	static int			model_;
	// モデルアセット
	static ModelAsset	asset_;
};

#endif // !SKYBOX_H_