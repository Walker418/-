#include "World.h"
#include "../Field/Field.h"
#include "../Actor/ActorGroup.h"
#include "../Actor/Actor.h"
#include "../Graphic/Shader/BloomCB.h"
#include "../Graphic/Shader/ShaderID.h"
#include "../Graphic/Graphics3D.h"
#include "../Graphic/Shader/ShaderManager.h"

// クラス：ワールド
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
World::World()
{
	// 初期化処理を行う
	initialize();
}

// 初期化
void World::initialize()
{
	// 全てのデータを消去
	clear();
	// アクターグループをアクターグループマネージャーに追加
	actors_.add_group(ActorGroup::None);			// 所属無し
	actors_.add_group(ActorGroup::Player);			// プレイヤー
	actors_.add_group(ActorGroup::PlayerAttack);	// プレイヤーの攻撃
	actors_.add_group(ActorGroup::Enemy);			// 敵
	actors_.add_group(ActorGroup::EnemyAttack);		// 敵の攻撃
	actors_.add_group(ActorGroup::Effect);			// エフェクト
	actors_.add_group(ActorGroup::UI);				// UI

	// シェーダー用定数バッファを生成
	cb_bloom_ = CreateShaderConstantBuffer(sizeof(BloomCB));
}

// 更新
void World::update(float delta_time)
{
	// 各アクターの状態を更新
	actors_.update(delta_time);
	// 接触判定を行う
	actors_.collide(ActorGroup::Player, ActorGroup::Enemy);
	actors_.collide(ActorGroup::Player, ActorGroup::EnemyAttack);
	actors_.collide(ActorGroup::PlayerAttack, ActorGroup::Enemy);
	actors_.collide(ActorGroup::Enemy);
	// 死亡したアクターを削除
	actors_.remove();

	// カメラの状態を更新
	camera_->update(delta_time);
	// ライトの状態を更新
	light_->update(delta_time);
}

// 描画
void World::draw() const
{
	// ブルームシェーダー用のパラメータを設定
	BloomCB bloom_param;						// ブルームシェーダー用定数バッファを宣言
	bloom_param.g_BrightPassThreshold = 0.5f;	// 抽出する輝度の最小値
	bloom_param.g_BloomIntensity = 0.75f;		// ブルームテクスチャの輝度
	bloom_param.g_BloomSaturation = 0.75f;		// ブルームテクスチャの彩度
	bloom_param.g_BaseIntensity = 0.8f;			// 元テクスチャの輝度
	bloom_param.g_BaseSaturation = 0.8f;		// 元テクスチャの彩度

	// ピクセルシェーダー用の定数バッファのアドレスを取得
	auto cb = (BloomCB*)GetBufferShaderConstantBuffer(cb_bloom_);
	// 定数バッファの値を設定
	*cb = bloom_param;
	// ピクセルシェーダー用の定数バッファを更新して、内容を反映させる
	UpdateShaderConstantBuffer(cb_bloom_);
	// ピクセルシェーダーの定数バッファを4番目のスロットに指定
	SetShaderConstantBuffer(cb_bloom_, DX_SHADERTYPE_PIXEL, 4);

	// --------------------------------------------------
	// 元画像にレンダリング
	// --------------------------------------------------
	rt_source_.begin();		// 元画像のレンダーターゲットを起動
	Graphics3D::clear();	// 画面をクリア

	// ここでシェーダーを適用するアクターを描画
	camera_->draw();		// カメラを設定
	light_->draw();
	field_->draw();
	actors_.draw(ActorGroup::Player);
	actors_.draw(ActorGroup::Enemy);
	actors_.draw(ActorGroup::PlayerAttack);
	actors_.draw(ActorGroup::EnemyAttack);

	rt_source_.end();		// 元画像のレンダーターゲットを終了
	// --------------------------------------------------

	// --------------------------------------------------
	// 輝度を抽出
	// --------------------------------------------------
	rt_bright_.begin();										// 輝度のレンダーターゲットを起動
	ShaderManager::set_ps((int)ShaderID::PS_BrightPass);	// 輝度抽出シェーダーを指定
	SetUseTextureToShader(0, rt_source_.texture());			// 元画像にシェーダーをかける
	rt_bright_.draw();										// 輝度のレンダーターゲットを描画
	rt_bright_.end();										// 輝度のレンダーターゲットを終了

	// --------------------------------------------------

	// --------------------------------------------------
	// 水平方向ブラーをかける
	// --------------------------------------------------

	rt_bluer_h_.begin();									// 水平方向ブラーのレンダーターゲットを起動
	ShaderManager::set_ps((int)ShaderID::PS_GaussianBlurH);	// 水平方向ブラーシェーダーを指定
	SetUseTextureToShader(0, rt_bright_.texture());			// 輝度抽出した画像にシェーダーをかける
	rt_bluer_h_.draw();										// 水平方向ブラーのレンダーターゲットを描画
	rt_bluer_h_.end();										// 水平方向ブラーのレンダーターゲットを終了

	// --------------------------------------------------

	// --------------------------------------------------
	// 垂直方向ブラーをかける
	// --------------------------------------------------

	rt_bluer_v_.begin();									// 垂直方向ブラーのレンダーターゲットを起動
	ShaderManager::set_ps((int)ShaderID::PS_GaussianBlurV);	// 垂直方向ブラーシェーダーを指定
	SetUseTextureToShader(0, rt_bluer_h_.texture());		// 水平方向ブラーと合成した画像にシェーダーをかける
	rt_bluer_v_.draw();										// 垂直方向ブラーのレンダーターゲットを描画
	rt_bluer_v_.end();										// 垂直方向ブラーのレンダーターゲットを終了

	// --------------------------------------------------

	// --------------------------------------------------
	// 元画像とブラー画像を合成
	// --------------------------------------------------

	rt_bloom_combine_.begin();								// ブルーム合成用のレンダーターゲットを起動
	ShaderManager::set_ps((int)ShaderID::PS_BloomCombine);	// ブルーム合成シェーダーを指定
	// 元画像とブラー画像を合成
	SetUseTextureToShader(0, rt_source_.texture());
	SetUseTextureToShader(1, rt_bluer_v_.texture());
	rt_bloom_combine_.draw();								// ブルーム合成用のレンダーターゲットを描画
	rt_bloom_combine_.end();								// ブルーム合成用のレンダーターゲットを終了

	// --------------------------------------------------

	// --------------------------------------------------
	// 再度元画像にレンダリングして、シェーダーを適用しない画面と適用する画面を合成
	// --------------------------------------------------

	rt_source_.begin();									// 元画像のレンダーターゲットを起動
	DrawGraph(0, 0, rt_bloom_combine_.texture(), TRUE);	// シェーダー合成した映像を描画
	// ここでシェーダーを適用しないアクターを描画
	camera_->draw();				// カメラを設定
	actors_.draw(ActorGroup::UI);	// アクターを描画
	rt_source_.end();									// 元画像のレンダーターゲットを終了

	// --------------------------------------------------

	// 最終画像を描画
	DrawGraph(0, 0, rt_source_.texture(), FALSE);
}

// メッセージ処理
void World::handle_message(EventMessage message, void* param)
{
	// ワールドのメッセージ処理
	listener_(message, param);

	// アクターのメッセージ処理
	actors_.handle_message(message, param);
	camera_->handle_message(message, param);
	light_->handle_message(message, param);
}

// メッセージリスナーの登録
void World::add_event_message_listener(EventMessageListener listener)
{
	listener_ = listener;
}

// フィールドの追加
void World::add_field(const FieldPtr& field)
{
	field_ = field;
}

// カメラの追加
void World::add_camera(const ActorPtr& camera)
{
	camera_ = camera;
}

// ライトの追加
void World::add_light(const ActorPtr& light)
{
	light_ = light;
}

// 全データの消去
void World::clear()
{
	actors_.clear();
	field_ = nullptr;
	light_ = nullptr;
	camera_ = nullptr;
	listener_ = [](EventMessage, void*) {};
}

// アクターを追加
void World::add_actor(ActorGroup group, const ActorPtr& actor)
{
	actors_.add_actor(group, actor);
}

// アクターの取得
ActorPtr World::find_actor(ActorGroup group, const std::string& name) const
{
	return actors_.find(group, name);
}

// アクター数の取得
unsigned int World::count_actor(ActorGroup group) const
{
	return actors_.count(group);
}

// アクターリストの巡回
void World::each_actor(ActorGroup group, std::function<void(const ActorPtr&)> fn) const
{
	actors_.each(group, fn);
}

// メッセージの送信
void World::send_message(EventMessage message, void* param)
{
	handle_message(message, param);
}

// フィールドの取得
Field& World::field()
{
	return *field_;
}

// カメラの取得
ActorPtr World::camera()
{
	return camera_;
}

// ライトの取得
ActorPtr World::light()
{
	return light_;
}