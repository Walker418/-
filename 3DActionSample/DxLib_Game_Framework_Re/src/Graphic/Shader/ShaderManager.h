#ifndef SHADER_MANAGER_H_
#define SHADER_MANAGER_H_

#include <string>
#include <unordered_map>

#include "Shader.h"
#include "CubeMap.h"

// クラス：シェーダー管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class ShaderManager
{
public:
	// 初期化
	static void initialize();
	// 終了処理
	static void finalize();
	// 頂点シェーダーの読み込み
	static bool load_vs(int id, const std::string& file_name);
	// ピクセルシェーダーの読み込み
	static bool load_ps(int id, const std::string& file_name);
	// キューブマップの読み込み
	static bool load_cubemap(int id, const std::string& file_name);
	// 頂点シェーダーの削除
	static void erase_vs(int id);
	// ピクセルシェーダーの削除
	static void erase_ps(int id);
	// キューブマップの削除
	static void erase_cubemap(int id);
	// 頂点シェーダーの指定
	static void set_vs(int id);
	// ピクセルシェーダーの指定
	static void set_ps(int id);
	// キューブマップの指定
	static void set_cubemap(int id, int stage);
	// キューブマップの解除
	static void off_cubemap(int stage);

	// シェーダーの有効化
	static void shader_on();
	// シェーダーの無効化
	static void shader_off();

private:
	// シェーダーアセット
	static Shader	shader_asset_;
	// キューブマップアセット
	static CubeMap	cubemap_asset_;
	// ピクセルシェーダー用の変数バッファ
	static int		pscbhandle_;
};

#endif // !SHADER_MANAGER_H_