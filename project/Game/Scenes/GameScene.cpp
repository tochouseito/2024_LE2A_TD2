#define NOMINMAX
#include "GameScene.h"
#include "TextureManager.h"
#include <DirectXTex.h>
#include <cassert>
#include "LightGroup.h"
#include <fstream>
#include <algorithm>
#ifdef _DEBUG
#include "imgui.h"
#endif

#include "Mymath.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete particles_;
	delete primitive_;
	delete serial;
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
	mainCamera_->Initialize(Vector3(4.5f, 17.0f, -15.0f), &viewProjection_); // HACK : 動画提出用
	mainCamera_->rotation_ = Vector3(0.0f, 0.0f, 0.0f); // HACK : 動画提出用

	// MapChip
	mapChipField_ = std::make_unique<MapChipField>();
	mapChipField_->LoadMapChipCsv("Resources/Map1.csv");

	// Blocks
	blocksModel_.reset(Model::LordModel("Block"));
	upNeedleModel_.reset(Model::LordModel("UpNeedle"));
	downNeedleModel_.reset(Model::LordModel("DownNeedle"));
	GenerateBlocks();

	// 矢印の生成
	gravityArrowModel_.resize(2);
	gravityArrowModel_[0].reset(Model::LordModel("GravityDownArrow"));
	gravityArrowModel_[1].reset(Model::LordModel("GravityUpArrow"));
	gravityArrow_ = std::make_unique<GravityArrow>();
	Vector3 pos = Vector3(0.0f, 0.0f, 30.0f);
	gravityArrow_->Initialize(gravityArrowModel_[0].get(), gravityArrowModel_[1].get(), &viewProjection_, pos);

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	// Player
	playerModel_.reset(Model::LordModel("Player"));
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

	player_->Initialize(playerModel_.get(), &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_.get());

	// Enemy
	enemyModel_.reset(Model::CreateSphere());
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
	enemy_->Initialize(enemyModel_.get(), &viewProjection_, enemyPosition);

}
void GameScene::Update() {
	// 矢印
	gravityArrow_->Update();

	// player
	player_->Update();

	// Enemy
	enemy_->Update();

	gravityArrow_->SetGravityDir(player_->GetIsGravityInvert());

	mainCamera_->translation_ = Lerp(mainCamera_->translation_, player_->GetWorldPosition() + player_->GetVelocity(), 1.0f / 60.0f * 5.0f);
	mainCamera_->translation_.z = -15.0f;

	// ブロックの更新
	for (std::vector<std::unique_ptr<Blocks>>& blockLine : blocks_) {
		for (std::unique_ptr<Blocks>& block : blockLine) {
			if (!block) {
				continue;
			}
			block->Update();
		}
	}

	// 衝突判定と応答
	CheckAllCollisions();
	collisionManager_->UpdateWorldTransform();

#ifdef _DEBUG
	// スペースキーでデバッグカメラの切り替え
	ImGui::Begin("EngineDebug");
	if (ImGui::Button("DebugCamera")) {
		useDebugCamera_ = !useDebugCamera_;
	}
	if (ImGui::Button("ChangeScene")) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("TITLE");
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
}

void GameScene::Draw() {
	// 矢印
	gravityArrow_->Draw();

	// Player
	player_->Draw();

	// エネミー
	enemy_->Draw();

	// ブロックの更新
	for (std::vector<std::unique_ptr<Blocks>>& blockLine : blocks_) {
		for (std::unique_ptr<Blocks>& block : blockLine) {
			if (!block) {
				continue;
			}
			block->Draw();
		}
	}

	collisionManager_->Draw(viewProjection_);
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

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kUpNeedle) {
				blocks_[i][j] = std::make_unique<Blocks>();
				Vector3 pos = mapChipField_->GetMapChipPositionByIndex(j, i);
				blocks_[i][j]->Initialize(upNeedleModel_.get(), &viewProjection_, pos);
			}

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kDownNeedle) {
				blocks_[i][j] = std::make_unique<Blocks>();
				Vector3 pos = mapChipField_->GetMapChipPositionByIndex(j, i);
				blocks_[i][j]->Initialize(downNeedleModel_.get(), &viewProjection_, pos);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// コライダーをリストに登録
	collisionManager_->AddCollider(player_.get());
	// 針全てについて
	/*for (const std::unique_ptr<Enemy>& enemy : enemies_) {

	}*/

	// 衝突判定と応答
	collisionManager_->CheckAllCollision();
}
