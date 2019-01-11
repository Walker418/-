#include "MyGame.h"
#include "Graphics3D.h"
#include "SkeletalMesh.h"
#include "CollisionMesh.h"
#include "Skybox.h"
#include "Billboard.h"
#include "Matrix.h"
#include "Vector3.h"

// クラス：3Dゲームフレームワークテスト
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
MyGame::MyGame() :
	Game{ 1024, 768, false }
{ }

// 開始
void MyGame::start()
{
	// 3Dモデルを読み込む
	SkeletalMesh::load(0, "test_assets/player.mv1");
	// ステージモデルを読み込む
	CollisionMesh::load(0, "test_assets/castle/SampleStage_Castle.mv1");
	// スカイボックスモデルを読み込む
	Skybox::load(0, "test_assets/skybox/skydome.mv1");
	// ビルボードを読み込む
	Billboard::load(0, "test_assets/Particle02.png");
	
	// フィールドを生成
	field_ = new Field(0, 0);
	// キャラクターを生成
	mesh_ = new AnimatedMesh{ 0, 0 };
}

// 更新
void MyGame::update(float delta_time)
{
	mesh_->update(delta_time);
	mesh_->transform(Matrix::Identity);
}

// 描画
void MyGame::draw()
{
	// 画面をクリア
	Graphics3D::clear();

	// カメラを設定
	Graphics3D::set_view_matrix(Matrix::CreateLookAt({ -70.0f, 20.0f,-70.0f }, { 0.0f, 20.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }));
	Graphics3D::set_projection_matrix(Matrix::CreatePerspectiveFieldOfView(45.0f, 640.0f / 480.0f, 0.3f, 1000.0f));

	// フィールドを描画
	field_->draw();

	// キャラクターを描画
	mesh_->draw();

	// ビルボードを描画
	Graphics3D::blend_mode(BlendMode::Add);
	Billboard::bind(0);
	Billboard::draw({ 0.0f, 30.0f, 0.0f }, 10.0f);
	Graphics3D::blend_mode(BlendMode::None);
}

// 終了
void MyGame::end()
{
	// キャラクターを削除
	delete mesh_;
	SkeletalMesh::erase(0);

	// フィールドを削除
	delete field_;
	CollisionMesh::erase(0);
	Skybox::erase(0);
	Billboard::erase(0);
}