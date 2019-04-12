#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <unordered_map>

// クラス：シェーダー
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Shader
{
public:
	// デフォルトコンストラクタ
	Shader() = default;
	// デストラクタ
	~Shader();
	// 頂点シェーダーの読み込み
	bool load_vs(int id, const std::string& file_name);
	// ピクセルシェーダーの読み込み
	bool load_ps(int id, const std::string& file_name);
	// シェーダーの削除
	void erase(int id);
	// データの消去
	void clear();
	// ハンドルの取得
	int operator[](int id) const;
	// 頂点シェーダーの指定
	void set_vs(int id);
	// ピクセルシェーダーの指定
	void set_ps(int id);

	// コピー禁止
	Shader(const Shader& other) = delete;
	Shader& operator = (const Shader& other) = delete;

private:
	// シェーダーデータマップ
	std::unordered_map<int, int> shader_;
};

#endif // !SHADER_H_