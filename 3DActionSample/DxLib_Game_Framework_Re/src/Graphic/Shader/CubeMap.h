#ifndef CUBE_MAP_H_
#define CUBE_MAP_H_

#include <string>
#include <unordered_map>

// クラス：キューブマップ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class CubeMap
{
public:
	// デフォルトコンストラクタ
	CubeMap() = default;
	// デストラクタ
	~CubeMap();
	// 読み込み
	bool load(int id, const std::string& file_name);
	// 環境マップの削除
	void erase(int id);
	// データの消去
	void clear();
	// ハンドルの取得
	int operator[](int id) const;

	// コピー禁止
	CubeMap(const CubeMap& other) = delete;
	CubeMap& operator = (const CubeMap& other) = delete;

private:
	// 環境マップデータマップ
	std::unordered_map<int, int> cube_map_;
};

#endif // !CUBE_MAP_H_