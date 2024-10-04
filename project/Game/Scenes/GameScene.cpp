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

}
void GameScene::Update() {


#ifdef _DEBUG
	// スペースキーでデバッグカメラの切り替え
	ImGui::Begin("EngineDebug");
	if (ImGui::Button("ChangeScene")) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	ImGui::DragFloat("hue", &DirectXCommon::GetInstance()->GetHSVData()->hue, 0.01f);
	ImGui::DragFloat("saturation", &DirectXCommon::GetInstance()->GetHSVData()->saturation, 0.01f);
	ImGui::DragFloat("value", &DirectXCommon::GetInstance()->GetHSVData()->value, 0.01f);
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

}
