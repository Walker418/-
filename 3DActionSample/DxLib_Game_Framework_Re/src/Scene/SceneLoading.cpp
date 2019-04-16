#include "SceneLoading.h"
#include "Scene.h"
#include "../Graphic/SkeletalMesh.h"
#include "../Math//Collision/CollisionMesh.h"
#include "../Field/Skybox.h"
#include "../Graphic/Billboard.h"
#include "../Graphic/Graphics2D.h"
#include "../Graphic/Graphics3D.h"
#include "../Graphic/Shader/ShaderManager.h"
#include "../Sound/Sound.h"
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
		// 初期化
		Graphics2D::initialize();
		Graphics3D::initialize();
		SkeletalMesh::initialize();
		CollisionMesh::initialize();
		Skybox::initialize();
		Billboard::initialize();
		ShaderManager::initialize();
		Sound::initialize();

		// データを読み込む
		// ステージ
		CollisionMesh::load(MESH_STAGE_CASTLE, "res/test_assets/castle/SampleStage_Castle.mv1");	// ステージモデル
		Skybox::load(MESH_SKYBOX, "res/test_assets/skybox/skydome1.mv1");							// スカイボックスモデル
		Billboard::load(0, "res/test_assets/Particle02.png");										// ビルボード
		// キャラクター
		SkeletalMesh::load(MESH_PALADIN, "res/assets/Paladin/Paladin.mv1");							// プレイヤーモデル
		SkeletalMesh::load(MESH_GHOUL, "res/assets/Ghoul/ghoul.mv1");								// 雑魚敵モデル
		SkeletalMesh::load(MESH_DRAGONBOAR, "res/assets/DragonBoar/DragonBoar.mv1");				// ボス敵モデル
		// 2D画像
		Graphics2D::load(TEXTURE_TITLELOGO, "res/assets/texture/TitleLogo.png");					// タイトルロゴ
		Graphics2D::load(TEXTURE_START, "res/assets/texture/Start.png");							// スタート表示
		Graphics2D::load(TEXTURE_HPGAUGE, "res/assets/texture/HPgauge.png");						// 体力ゲージ
		Graphics2D::load(TEXTURE_HP, "res/assets/texture/HP.png");									// 体力表示
		Graphics2D::load(TEXTURE_P1MESSAGE, "res/assets/texture/P1message.png");					// フェーズ1目標メッセージ
		Graphics2D::load(TEXTURE_P2MESSAGE, "res/assets/texture/P2message.png");					// フェーズ2目標メッセージ
		// シェーダー
		ShaderManager::load_vs(SHADER_VERTEX_SHADER, "VertexShader.cso");							// 頂点シェーダー
		ShaderManager::load_ps(SHADER_PIXEL_SHADER, "PixelShader.cso");								// ピクセルシェーダー
		ShaderManager::load_vs(SHADER_SKYBOX_VERTEX_SHADER, "SkyboxVertexShader.cso");				// スカイボックス用頂点シェーダー
		ShaderManager::load_ps(SHADER_SKYBOX_PIXEL_SHADER, "SkyboxPixelShader.cso");				// スカイボックス用ピクセルシェーダー
		// 環境マップ
		ShaderManager::load_cubemap(CUBEMAP_TEST_SPECULAR_HDR, "res/assets/cubemap/TestSpecularHDR.dds");
		ShaderManager::load_cubemap(CUBEMAP_TEST_DIFFUSE_HDR, "res/assets/cubemap/TestDiffuseHDR.dds");
		ShaderManager::load_cubemap(CUBEMAP_TEST_BRDF, "res/assets/cubemap/TestBrdf.dds");
		ShaderManager::load_cubemap(CUBEMAP_TEST_ENV_HDR, "res/assets/cubemap/TestEnvHDR.dds");

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
	return Scene::Title;
}

// 終了
void SceneLoading::end()
{
	// 何もしない
}