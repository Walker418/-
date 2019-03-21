#include "SceneTitle.h"
#include "Scene.h"
#include "../ID/SourceID.h"

#include "../Field/Field.h"
#include "../Actor/ActorGroup.h"
#include "../Actor/Camera/TitleCamera.h"
#include "../Graphic/Light.h"
#include "../Actor/Player/PlayerInput.h"
#include "../Graphic/Graphics2D.h"

// 開始
void SceneTitle::start()
{
	// 終了フラグをFalseにする
	is_end_ = false;
	// ワールドを初期化
	world_.initialize();
	// スタートボタンの状況を初期化
	is_started_ = false;
	show_start_ = true;
	show_start_timer_ = 0.0f;
	end_timer_ = 0.0f;

	// フィールドを追加
	world_.add_field(new_field<Field>(MESH_STAGE_CASTLE, MESH_SKYBOX));
	// カメラを追加
	world_.add_camera(new_actor<TitleCamera>(&world_, Vector3::Zero));
	// ライトを追加
	world_.add_light(new_actor<Light>(&world_, Vector3{ 0.0f, 30.0f, -20.0f }));
}

// 更新
void SceneTitle::update(float delta_time)
{
	world_.update(delta_time);

	// ゲーム開始
	if (PlayerInput::game_start())
	{
		is_started_ = true;
	}

	// スペースキーが押されてから3秒後、ゲームプレイシーンに移行
	if (is_started_)
	{
		if (end_timer_ >= 180.0f)
		{
			is_end_ = true;
		}

		end_timer_ += delta_time;
	}

	// スタートメッセージの点滅制御
	// スペースキーが押されたら、激しく点滅する
	if (is_started_)
	{
		if (show_start_timer_ >= 5.0f)
		{
			(show_start_) ? show_start_ = false : show_start_ = true;

			show_start_timer_ = 0.0f;
		}
	}
	// 普段は1秒ごと点滅
	else
	{
		if (show_start_timer_ >= 60.0f)
		{
			(show_start_) ? show_start_ = false : show_start_ = true;

			show_start_timer_ = 0.0f;
		}
	}

	show_start_timer_ += delta_time;
}

// 描画
void SceneTitle::draw() const
{
	world_.draw();	// 背景を描画

	draw_logo();	// タイトルロゴを描画
	draw_start();	// スタートメッセージを描画（点滅付き）
}

// 終了しているか
bool SceneTitle::is_end() const
{
	// 終了フラグを返す
	return is_end_;
}

// 次のシーンの取得
Scene SceneTitle::next() const
{
	return Scene::GamePlay;
}

// 終了
void SceneTitle::end()
{

}

// メッセージ処理
void SceneTitle::handle_message(EventMessage message, void* param)
{

}

// タイトルロゴの描画
void SceneTitle::draw_logo() const
{
	Graphics2D::draw(TEXTURE_TITLELOGO, Vector2(115.0f, 80.0f));
}

// スタートメッセージの描画
void SceneTitle::draw_start() const
{
	if (!show_start_) return;

	Graphics2D::draw(TEXTURE_START, Vector2(290.0f, 490.0f));
}