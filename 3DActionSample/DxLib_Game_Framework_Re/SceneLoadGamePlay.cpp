#include "SceneLoadGamePlay.h"
#include "Scene.h"
#include "SkeletalMesh.h"
#include "CollisionMesh.h"
#include "Skybox.h"
#include "Billboard.h"
#include "SourceID.h"

// クラス：ゲームプレイシーンリソースの読み込み
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// 開始
void SceneLoadGamePlay::start()
{
	is_loaded_ = false;
	end_timer_ = 0.0f;
	is_end_ = false;
}

// 更新
void SceneLoadGamePlay::update(float delta_time)
{
	// データをまだ読み込んでいない場合、データを読み込む
	if (!is_loaded_)
	{
		// データを読み込む
		SkeletalMesh::load(0, "test_assets/player.mv1");										// 3Dモデル
		CollisionMesh::load(MESH_STAGE_CASTLE, "test_assets/castle/SampleStage_Castle.mv1");	// ステージモデル
		Skybox::load(MESH_SKYBOX, "test_assets/skybox/skydome.mv1");							// スカイボックスモデル
		Billboard::load(0, "test_assets/Particle02.png");										// ビルボード

		// 読み込む判定をTrueにする（読み込み終了）
		is_loaded_ = true;
	}
	// データを読み込んだ場合、終了タイマーを作動し、2秒後にプレイシーンへ移行
	else
	{
		if (end_timer_ >= 120.0f)
		{
			is_end_ = true;
		}

		end_timer_ += delta_time;
	}
}

// 描画
void SceneLoadGamePlay::draw() const
{
	DrawString(0, 0, "データ読み込み中…", GetColor(255, 255, 255));
}

// 終了しているか
bool SceneLoadGamePlay::is_end() const
{
	return is_end_;
}

// 次のシーンを返す
Scene SceneLoadGamePlay::next() const
{
	return Scene::GamePlay;
}

// 終了
void SceneLoadGamePlay::end()
{
	// 何もしない
}