#ifndef TEXTURE_ASSET_H_
#define TEXTURE_ASSET_H_

#include <unordered_map>
#include <string>

// クラス：テクスチャアセット
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class TextureAsset
{
public:
	// デフォルトコンストラクタ
	TextureAsset() = default;
	// デストラクタ
	~TextureAsset();
	// テクスチャの読み込み
	bool load(int id, const std::string& file_name);
	// テクスチャの削除
	void erase(int id);
	// データの消去
	void clear();
	// ハンドルの取得
	int operator[](int id) const;

	// コピー禁止
	TextureAsset(const TextureAsset& other) = delete;
	TextureAsset& operator = (const TextureAsset& other) = delete;

private:
	// テクスチャデータマップ
	std::unordered_map<int, int> textures_;
};

#endif // !TEXTURE_ASSET_H_