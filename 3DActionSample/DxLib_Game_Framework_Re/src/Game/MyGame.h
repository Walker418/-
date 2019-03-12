#ifndef MY_GAME_H_
#define MY_GAME_H_

#include "Game.h"
#include "../Field/Field.h"
#include "../Graphic/AnimatedMesh.h"

// クラス：3Dゲームフレームワークテスト（アプリケーションクラス）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class MyGame : public Game
{
private:
	// ステージ
	Field*			field_{ nullptr };
	// キャラクター
	AnimatedMesh*	mesh_{ nullptr };

public:
	// コンストラクタ
	MyGame();

private:
	// 開始
	void start() override;
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw() override;
	// 終了
	void end() override;

};

#endif // !MY_GAME_H_