#ifndef ISCENE_H_
#define ISCENE_H_

// インターフェース：シーン
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

enum class Scene;

class IScene
{
public:
	// 仮想デストラクタ
	virtual ~IScene() {}
	// 開始
	virtual void start() = 0;
	// 更新
	virtual void update(float delta_time) = 0;
	// 描画
	virtual void draw() const = 0;
	// 終了しているか
	virtual bool is_end() const = 0;
	// 次のシーンの取得
	virtual Scene next() const = 0;
	// 終了
	virtual void end() = 0;
};

#endif // !ISCENE_H_