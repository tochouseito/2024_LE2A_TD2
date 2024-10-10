#define NOMINMAX
#include "GameScene.h"
#include "TextureManager.h"
#include <DirectXTex.h>
#include <cassert>
#include"LightGroup.h"
#include<fstream>
#include <algorithm>
#ifdef _DEBUG
#include"imgui.h"
#endif

#include"Mymath.h"

GameScene::GameScene() {
}

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
	mainCamera_->Initialize(Vector3(0.0f, 0.0f, 30.0f), &viewProjection_);

	// MapChip
	mapChipField_ = std::make_unique<MapChipField>();
	mapChipField_->LoadMapChipCsv("Resources/Map1.csv");

	// Blocks
	blocksModel_.reset(Model::LordModel("Player"));
	GenerateBlocks();

	// Player
	playerModel_.reset(Model::LordModel("Player"));
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModel_.get(), &viewProjection_, Vector3(0.0f, 0.0f, 0.0f));
	player_->SetMapChipField(mapChipField_.get());



}
void GameScene::Update() {

	// player
	player_->Update();

	// ブロックの更新
	for (std::vector<std::unique_ptr<Blocks>>& blockLine : blocks_) {
		for (std::unique_ptr<Blocks>& block : blockLine) {
			if (!block) {
				continue;
			}
			block->Update();
		}
	}


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

	// Player
	player_->Draw();

	// ブロックの更新
	for (std::vector<std::unique_ptr<Blocks>>& blockLine : blocks_) {
		for (std::unique_ptr<Blocks>& block : blockLine) {
			if (!block) {
				continue;
			}
			block->Draw();
		}
	}
}

void GameScene::GenerateBlocks()
{
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
				Vector3 pos=mapChipField_->GetMapChipPositionByIndex(j, i);
				blocks_[i][j]->Initialize(blocksModel_.get(), &viewProjection_, pos);
			}
		}
	}
}
