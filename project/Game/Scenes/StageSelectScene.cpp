#include "StageSelectScene.h"
#include"SceneManager.h"
#include"imgui.h"
#include "TextureManager.h"

void StageSelectScene::Initialize() {

	// シーンマネージャからステージ番号を取得
	currentStageNum_ = sceneManager_->GetCurrentStageNumber();

	viewProjection_.Initialize();
	DirectXCommon::GetInstance()->SetViewProjection(&viewProjection_);

	selectSceneTextureHandle_ = TextureManager::Load("./Resources/GUI/select.png");
	selectSceneSprite_ = std::make_unique<Sprite>();
	selectSceneSprite_->Initialize({ 0.0f,0.0f,0.0f }, &viewProjection_, selectSceneTextureHandle_);

	selectAllowTextureHandle_ = TextureManager::Load("./Resources/GUI/selectCursor.png");
	selectAllowSceneSprite_ = std::make_unique<Sprite>();
	selectAllowSceneSprite_->Initialize({ 640.0f,360.0f,0.0f }, &viewProjection_, selectAllowTextureHandle_);
	selectAllowSceneSprite_->SetAnchorPoint(Vector3(0.5f, 0.5f, 0.0f));

	numberTextureHandle_ = TextureManager::Load("./Resources/GUI/numbers.png");
	numberSprite_ = std::make_unique<Sprite>();
	numberSprite_->Initialize({ 0.0f,0.0f,0.0f }, &viewProjection_, numberTextureHandle_);


}

void StageSelectScene::Finalize() {
}

void StageSelectScene::Update() {
#ifdef _DEBUG
	ImGui::Begin("SelectScene");
	if (ImGui::Button("ChangeScene")) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

	ImGui::Text("currentStage: %u", currentStageNum_);
	ImGui::End();
#endif // _DEBUG

	// ステージ番号の選択と決定
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneManager_->SetCurrentStageNum(currentStageNum_);
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	} else if (Input::GetInstance()->TriggerKey(DIK_A)) {
		currentStageNum_--;
	} else if (Input::GetInstance()->TriggerKey(DIK_D)) {
		currentStageNum_++;
	}

	// 値超過排斥処理
	if (currentStageNum_ < 1) {
		currentStageNum_ = 1;
	} else if (currentStageNum_ > kMaxStageNum_) {
		currentStageNum_ = kMaxStageNum_;
	}

	selectSceneSprite_->Update();
	selectAllowSceneSprite_->Update();
}

void StageSelectScene::Draw() {
	selectSceneSprite_->Draw();
	selectAllowSceneSprite_->Draw();
}

void StageSelectScene::ChangeScene() {
}
