#include "StageSelectScene.h"
#include"SceneManager.h"
#include"imgui.h"
#include "TextureManager.h"

void StageSelectScene::Initialize() {
	viewProjection_.Initialize();
	DirectXCommon::GetInstance()->SetViewProjection(&viewProjection_);

	selectSceneTextureHandle_ = TextureManager::Load("./Resources/GUI/select.png");
	selectSceneSprite_ = std::make_unique<Sprite>();
	selectSceneSprite_->Initialize({ 0.0f,0.0f,0.0f }, &viewProjection_, selectSceneTextureHandle_);

	selectAllowTextureHandle_ = TextureManager::Load("./Resources/GUI/selectCursor.png");
	selectAllowSceneSprite_ = std::make_unique<Sprite>();
	selectAllowSceneSprite_->Initialize({ 640.0f,360.0f,0.0f }, &viewProjection_, selectAllowTextureHandle_);
	selectAllowSceneSprite_->SetAnchorPoint(Vector3(0.5f, 0.5f, 0.0f));

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
	ImGui::End();
#endif // _DEBUG
	selectSceneSprite_->Update();
	selectAllowSceneSprite_->Update();
}

void StageSelectScene::Draw() {
	selectSceneSprite_->Draw();
	selectAllowSceneSprite_->Draw();
}

void StageSelectScene::ChangeScene() {
}
