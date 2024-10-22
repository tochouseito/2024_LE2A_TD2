#define NOMINMAX
#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#ifdef _DEBUG
#include "imgui.h"
#endif

#include "Mymath.h"
#include "Needle/Needle.h"

constexpr float gravityArrowZPos = 25.0f;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete particles_;
	delete primitive_;
	delete serial;
	for (auto& model : playerModels_) {
		delete model;
	}
	for (auto& plAnima : plAnimas_) {
		delete plAnima;
	}
}

void GameScene::Finalize() {
	audio_->Finalize();
	audio_->SoundUnLord(&SoundData1);
}

void GameScene::Initialize() {
	/*ダミーテクスチャ*/
	textureHandle_[0] = TextureManager::Load("./Resources/white1x1.png");

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();


	// カメラ
	viewProjection_.Initialize();

	// ファイル名でテクスチャを読み込む

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	DirectXCommon::GetInstance()->SetViewProjection(&viewProjection_);
	input_ = Input::GetInstance();

	/*デバッグカメラの生成*/
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(Vector3(0.0f, 0.0f, -30.0f), &viewProjection_);

	/*3D軸モデル*/

	// メインカメラの生成
	mainCamera_ = std::make_unique<MainCamera>();
	mainCamera_->Initialize(Vector3(4.5f, 17.0f, -45.0f), &viewProjection_); // HACK : 動画提出用
	mainCamera_->rotation_ = Vector3(0.0f, 0.0f, 0.0f); // HACK : 動画提出用

	deltaTime_ = std::make_unique<DeltaTime>();
	deltaTime_->Update();

	particleManager_ = std::make_unique<ParticleManager>();
	particleManager_->Initialize(&viewProjection_, textureHandle_[0]);

	///////////////////////////////////////////////////////////////////////////////////////


	// 現在のステージ
	currentStageNum_ = sceneManager_->GetCurrentStageNumber();
	std::string str = std::to_string(currentStageNum_);

	// MapChip
	mapChipField_ = std::make_unique<MapChipField>();
	mapChipField_->LoadMapChipCsv("Resources/Map"+str+".csv");

	// 針
	upNeedleModel_.reset(Model::LordModel("UpNeedle"));
	downNeedleModel_.reset(Model::LordModel("DownNeedle"));
	for (uint32_t i = 0; i < mapChipField_->GetNumBlockVertical(); ++i) {
		for (uint32_t j = 0; j < mapChipField_->GetNumBlockHorizontal(); ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kUpNeedle) {
				// 配列のサイズを一つ増やす
				needles_.resize(needles_.size() + 1);
				// 作成
				needles_.back() = std::make_unique<Needle>();
				// 初期化
				needles_.back()->Initialize(upNeedleModel_.get(), &viewProjection_, mapChipField_->GetMapChipPositionByIndex(j, i), kUp);
			}

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kDownNeedle) {
				// 配列のサイズを一つ増やす
				needles_.resize(needles_.size() + 1);
				// 作成
				needles_.back() = std::make_unique<Needle>();
				// 初期化
				needles_.back()->Initialize(downNeedleModel_.get(), &viewProjection_, mapChipField_->GetMapChipPositionByIndex(j, i), kDown);
			}
		}
	}

	// Blocks
	blocksModel_.reset(Model::LordModel("Block"));
	GenerateBlocks();

	// Bullet
	bulletModel_.reset(Model::LordModel("PlayerBullet"));
	GenerateBullets();

	// enemyAttack
	enemyAttackModel_.reset(Model::LordModel("attack"));
	enemyAttackModel_->SetBlendMode(1); // ブレンドモード設定
	enemyAttackModel_->GetMaterial()->GetMaterialData()->enableLighting = false;
	enemyAttackModel_->GetMaterial()->GetMaterialData()->enableEmissive = true; // TODO : 動いてる?
	//enemyAttackModel_->SetBlendMode(1); // ブレンドモード設定
	enemyAttackWorldTransform_.Initialize();
	enemyPreliminaryModel_.reset(Model::LordModel("preliminary"));
	enemyPreliminaryModel_->SetBlendMode(1); // ブレンドモード設定
	enemyPreliminaryModel_->GetMaterial()->GetMaterialData()->color = { 1.0f,1.0f,1.0f,0.5f }; // 半透明に
	enemyAttackWorldTransform_.Initialize();

	// 矢印の生成
	gravityArrowModel_.resize(2);
	gravityArrowModel_[0].reset(Model::LordModel("GravityUpArrow"));
	gravityArrowModel_[1].reset(Model::LordModel("GravityDownArrow"));
	gravityArrow_ = std::make_unique<GravityArrow>();
	gravityArrow_->Initialize(gravityArrowModel_[0].get(), gravityArrowModel_[1].get(), &viewProjection_);

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	// Player
	playerModel_.reset(Model::LordModel("Player"));
	playerModels_.push_back(Model::LordModel("Idle", true));
	plAnimas_.push_back(Model::LordAnimationFile("./Resources", "Idle"));
	playerModels_.push_back(Model::LordModel("Jump", true));
	plAnimas_.push_back(Model::LordAnimationFile("./Resources", "Jump"));
	playerModels_.push_back(Model::LordModel("JumpLoop", true));
	plAnimas_.push_back(Model::LordAnimationFile("./Resources", "JumpLoop"));
	playerModels_.push_back(Model::LordModel("JumpStart", true));
	plAnimas_.push_back(Model::LordAnimationFile("./Resources", "JumpStart"));
	playerModels_.push_back(Model::LordModel("Land", true));
	plAnimas_.push_back(Model::LordAnimationFile("./Resources", "Land"));
	playerModels_.push_back(Model::LordModel("Run", true));
	plAnimas_.push_back(Model::LordAnimationFile("./Resources", "Run"));
	player_ = std::make_unique<Player>();
	// CSVからプレイヤーの開始位置を見つける
	Vector3 playerPosition{};
	for (uint32_t i = 0; i < mapChipField_->GetNumBlockVertical(); i++) {
		for (uint32_t j = 0; j < mapChipField_->GetNumBlockHorizontal(); j++) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kPlayer) {
				playerPosition = mapChipField_->GetMapChipPositionByIndex(j, i);
				break;
			}
		}
	}

	player_->Initialize(playerModels_, plAnimas_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_.get());

	// Enemy
	enemyModel_.reset(Model::LordModel("EnemySaw"));
	enemyFace_.reset(Model::LordModel("EnemyFace"));
	enemyEyeNormal.reset(Model::LordModel("EnemyEyeNormal"));
	enemyEyeHit.reset(Model::LordModel("EnemyEyeHit"));
	Model* models[] = {
		enemyModel_.get(),
		enemyFace_.get(),
		enemyEyeNormal.get(),
		enemyEyeHit.get(),
	};
	enemy_ = std::make_unique<Enemy>();
	// CSVからエネミーの開始位置を見つける
	Vector3 enemyPosition{};
	for (uint32_t i = 0; i < mapChipField_->GetNumBlockVertical(); i++) {
		for (uint32_t j = 0; j < mapChipField_->GetNumBlockHorizontal(); j++) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kEnemy) {
				enemyPosition = mapChipField_->GetMapChipPositionByIndex(j, i);
				break;
			}
		}
	}
	enemy_->Initialize(models, &viewProjection_, enemyPosition);

	// Goal
	goalModel_.reset(Model::CreateSphere());
	goal_ = std::make_unique<Goal>();
	// CSVからエネミーの開始位置を見つける
	Vector3 goalPosition{};
	for (uint32_t i = 0; i < mapChipField_->GetNumBlockVertical(); i++) {
		for (uint32_t j = 0; j < mapChipField_->GetNumBlockHorizontal(); j++) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kGoal) {
				goalPosition = mapChipField_->GetMapChipPositionByIndex(j, i);
				break;
			}
		}
	}
	goal_->Initialize(goalModel_.get(), &viewProjection_, goalPosition);
}

void GameScene::Update() {
	// もしゴールしていたら
	if (goal_->GetIsGoal()) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("RESULT");
	}

	// player
	player_->Update();
	particleManager_->SetEmit(player_->IsLand(), player_->GetWorldPosition(), player_->GetIsGravityInvert());

	// Enemy
	enemy_->SetPlayerPos(player_->GetWorldPosition());
	enemy_->Update();

	// Goal
	goal_->Update();

	// カメラ移動
	mainCamera_->translation_.x = std::lerp(mainCamera_->translation_.x, player_->GetWorldPosition().x + player_->GetVelocity().x * deltaTime_->GetDeltaTime(), deltaTime_->GetDeltaTime() * 5.0f);
	mainCamera_->translation_.y = std::lerp(mainCamera_->translation_.y, player_->GetWorldPosition().y + player_->GetVelocity().y * deltaTime_->GetDeltaTime(), deltaTime_->GetDeltaTime() * 5.0f);

	// プレイヤーの速度が上がると後ろに下がる
	float targetZ = -15.0f - std::clamp(std::abs(player_->GetVelocity().x * 50.0f), 0.0f, 10.0f);
	mainCamera_->translation_.z = Lerp(mainCamera_->translation_.z, targetZ, deltaTime_->GetDeltaTime() * 0.1f);

	// 矢印
	gravityArrow_->SetPos({ mainCamera_->translation_.x, mainCamera_->translation_.y, gravityArrowZPos });
	gravityArrow_->SetGravityDir(player_->GetIsGravityInvert());
	gravityArrow_->Update(); // カメラを移動した後に更新

	// ブロックの更新
	for (std::vector<std::unique_ptr<Blocks>>& blockLine : blocks_) {
		for (std::unique_ptr<Blocks>& block : blockLine) {
			if (!block) {
				continue;
			}
			block->Update();
		}
	}

	// バレットの更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	for (std::unique_ptr<Needle>& needle : needles_) {
		needle->Update();
	}

	particleManager_->Update();

	// 衝突判定と応答
	CheckAllCollisions();
	collisionManager_->UpdateWorldTransform();

	// エネミーの攻撃とプレイヤーの当たり判定処理
	EnemyAttack(enemy_->GetAttackYIndex(), enemy_->GetBehavior());

#ifdef _DEBUG
	// スペースキーでデバッグカメラの切り替え
	ImGui::Begin("EngineDebug");
	if (ImGui::Button("DebugCamera")) {
		useDebugCamera_ = !useDebugCamera_;
	}
	//if (ImGui::Button("ChangeScene")) {
	//	/*シーン切り替え依頼*/
	//	SceneManager::GetInstance()->ChangeScene("TITLE");
	//}
	ImGui::End();

	// シーン切り替えウィンドウ
	ImGui::Begin("GameScene");
	if (ImGui::Button("GoResult")) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("RESULT");
	}
	ImGui::End();
#endif // _DEBUG

	if (useDebugCamera_) {
		debugCamera_->Update();
		viewProjection_.TransferMatrix();
	} else {
		// メインカメラの処理
		mainCamera_->Update();
		viewProjection_.UpdateMatrix();
	}

	deltaTime_->Update();
}

void GameScene::Draw() {
	// 矢印
	gravityArrow_->Draw();

	// Player
	player_->Draw();

	// エネミー
	//enemy_->Draw();
	switch (enemy_->GetBehavior()) {
	case Enemy::Behavior::kRoot:

		break;
	case Enemy::Behavior::kPreliminary:
		enemyPreliminaryModel_->Draw(enemyAttackWorldTransform_, viewProjection_);
		break;
	case Enemy::Behavior::kAttack:
		enemyAttackModel_->Draw(enemyAttackWorldTransform_, viewProjection_);
		break;
	}

	// ゴール
	goal_->Draw();

	// ブロックの更新
	for (std::vector<std::unique_ptr<Blocks>>& blockLine : blocks_) {
		for (std::unique_ptr<Blocks>& block : blockLine) {
			if (!block) {
				continue;
			}
			block->Draw();
		}
	}

	// バレットの描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	// 針の描画
	for (std::unique_ptr<Needle>& needle : needles_) {
		needle->Draw();

	}

	collisionManager_->Draw(viewProjection_);

	particleManager_->DrawGPU();

}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVertical = mapChipField_->GetNumBlockVertical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定（縦方向のブロック）
	blocks_.resize(numBlockVertical);
	for (uint32_t i = 0; i < numBlockVertical; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		blocks_[i].resize(numBlockHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVertical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				blocks_[i][j] = std::make_unique<Blocks>();
				Vector3 pos = mapChipField_->GetMapChipPositionByIndex(j, i);
				blocks_[i][j]->Initialize(blocksModel_.get(), &viewProjection_, pos);
			}
		}
	}
}

void GameScene::GenerateBullets() {
	// 要素数
	uint32_t numBlockVertical = mapChipField_->GetNumBlockVertical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVertical; ++i) {

		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBullet) {
				// 配列のサイズを一つ増やす
				bullets_.resize(bullets_.size() + 1);
				// 作成
				bullets_.back() = std::make_unique<PlayerBullet>();
				// 初期化
				bullets_.back()->Initialize(bulletModel_.get(), &viewProjection_, mapChipField_->GetMapChipPositionByIndex(j, i));
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// コライダーをリストに登録
	collisionManager_->AddCollider(player_.get());
	collisionManager_->AddCollider(goal_.get());
	collisionManager_->AddCollider(enemy_.get());
	// 針全てについて
	for (std::unique_ptr<Needle>& needle : needles_) {
		collisionManager_->AddCollider(needle.get());
	}
	// 弾すべて
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		collisionManager_->AddCollider(bullet.get());
	}

	// 衝突判定と応答
	collisionManager_->CheckAllCollision();
}

void GameScene::EnemyAttack(const uint32_t& enemyAttackYIndex, const Enemy::Behavior& behavior) {
	// エネミーの攻撃のy座標を取得
	float enemyAttackYTranslate = mapChipField_->GetMapChipPositionYByIndex(enemyAttackYIndex);
	playerAABB_ = player_->GetAABB();
	enemyAttackAABB_.min = { 0.0f,enemyAttackYTranslate - 0.5f,0.0f };
	enemyAttackAABB_.max = { static_cast<float>(mapChipField_->GetNumBlockHorizontal()),enemyAttackYTranslate + 0.5f,0.0f };
	Vector3 enemyAttackTranslate = { viewProjection_.translation_.x,enemyAttackYTranslate,0.0f };
	enemyAttackWorldTransform_.translation_ = enemyAttackTranslate;
	enemyAttackWorldTransform_.UpdateMatrix();

	switch (behavior) {
	case Enemy::Behavior::kRoot:
		Vector3 playerWorldPosition = player_->GetWorldPosition();
		enemy_->SetPreliminaryYIndex(mapChipField_->GetMapChipIndexSetByPosition(playerWorldPosition).yIndex);
		break;
	case Enemy::Behavior::kPreliminary:

		break;
	case Enemy::Behavior::kAttack:

		if (AABBIntersects(playerAABB_, enemyAttackAABB_)) {
			// 攻撃がヒットした時の処理

		}
		break;
	}
}

bool GameScene::AABBIntersects(const AABB& a, const AABB& b) {
	// 各軸についてAABBが重なっているかを確認する
	bool xOverlap = a.min.x < b.max.x && a.max.x > b.min.x;
	bool yOverlap = a.min.y < b.max.y && a.max.y > b.min.y;
	bool zOverlap = a.min.z < b.max.z && a.max.z > b.min.z;

	// すべての軸で重なっている場合に衝突と判断する
	return xOverlap && yOverlap && zOverlap;
}

