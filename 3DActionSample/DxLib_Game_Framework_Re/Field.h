#ifndef FIELD_H_
#define FIELD_H_

// クラス：フィールド
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Field
{
public:
	// コンストラクタ
	Field(int stage, int skybox);
	// 描画
	void draw() const;

	// コピー禁止
	Field(const Field& other) = delete;
	Field& operator = (const Field& other) = delete;

private:
	// ステージモデル
	int stage_;
	// スカイボックスモデル
	int skybox_;
};

#endif // !FIELD_H_