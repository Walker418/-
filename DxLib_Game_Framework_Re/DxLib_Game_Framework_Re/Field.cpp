#include "Field.h"
#include "CollisionMesh.h"
#include "Skybox.h"

// クラス：フィールド
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
Field::Field(int stage, int skybox) :
	stage_{ stage }, skybox_{ skybox }
{
	//モデルが設定されていなかったら返す
	if (stage_ < 0 || skybox_ < 0)	return;
}

// 描画
void Field::draw() const
{
	// スカイボックスを描画
	Skybox::bind(skybox_);
	Skybox::draw();

	// ステージを描画
	CollisionMesh::bind(stage_);
	CollisionMesh::draw();
}