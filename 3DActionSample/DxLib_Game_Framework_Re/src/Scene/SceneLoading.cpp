#include "SceneLoading.h"
#include "Scene.h"
#include "../Graphic/SkeletalMesh.h"
#include "../Math//Collision/CollisionMesh.h"
#include "../Field/Skybox.h"
#include "../Graphic/Billboard.h"
#include "../Graphic/Graphics2D.h"
#include "../ID/SourceID.h"

// クラス：ローディングシーン
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// 開始
void SceneLoading::start()
{
	is_loaded_ = false;
	end_timer_ = 0.0f;
	is_end_ = false;
}

// 更新
void SceneLoading::update(float delta_time)
{
	// データをまだ読み込んでいない場合、データを読み込む
	if (!is_loaded_)
	{
		// データを読み込む
		CollisionMesh::load(MESH_STAGE_CASTLE, "res/test_assets/castle/SampleStage_Castle.mv1");	// ステージモデル
		Skybox::load(MESH_SKYBOX, "res/test_assets/skybox/skydome.mv1");							// スカイボックスモデル
		Billboard::load(0, "res/test_assets/Particle02.png");										// ビルボード

		SkeletalMesh::load(MESH_PALADIN, "res/assets/Paladin/Paladin.mv1");							// プレイヤーモデル
		SkeletalMesh::load(MESH_GHOUL, "res/assets/Ghoul/ghoul.mv1");								// 雑魚敵モデル
		SkeletalMesh::load(MESH_DRAGONBOAR, "res/assets/DragonBoar/DragonBoar.mv1");				// ボス敵モデル

		Graphics2D::load(TEXTURE_HPGAUGE, "res/assets/textures/HPgauge.png");						// 体力ゲージ
		Graphics2D::load(TEXTURE_HP, "res/assets/textures/HP.png");									// 体力表示

		// 読み込む判定をTrueにする（読み込み終了）
		is_loaded_ = true;
	}
	// データを読み込んだ場合、終了タイマーを作動し、2秒後にプレイシーンへ移行
	else
	{
		// 60フレーム = 1秒
		if (end_timer_ >= 120.0f)
		{
			is_end_ = true;
		}

		end_timer_ += delta_time;
	}
}

// 描画
void SceneLoading::draw() const
{
	DrawString(0, 0, "データ読み込み中…", GetColor(255, 255, 255));
}

// 終了しているか
bool SceneLoading::is_end() const
{
	return is_end_;
}

// 次のシーンを返す
Scene SceneLoading::next() const
{
	return Scene::GamePlay;
}

// 終了
void SceneLoading::end()
{
	// 何もしない
}