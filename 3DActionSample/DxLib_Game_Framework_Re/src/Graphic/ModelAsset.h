#ifndef MODEL_ASSET_H_
#define MODEL_ASSET_H_

#include <string>
#include <unordered_map>

// クラス：モデルアセット
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class ModelAsset
{
public:
	// デフォルトコンストラクタ
	ModelAsset() = default;
	// デストラクタ
	~ModelAsset();
	// モデルの読み込み
	bool load(int id, const std::string& file_name);
	// モデルの削除
	void erase(int id);
	// データの消去
	void clear();
	// ハンドルの取得
	int operator[](int id) const;

	// コピー禁止
	ModelAsset(const ModelAsset& other) = delete;
	ModelAsset& operator = (const ModelAsset& other) = delete;

private:
	// モデルデータマップ
	std::unordered_map<int, int> asset_;
};

#endif // !MODEL_ASSET_H_