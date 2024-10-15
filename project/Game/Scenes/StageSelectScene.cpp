#include "StageSelectScene.h"
#include"SceneManager.h"
#include"imgui.h"

void StageSelectScene::Initialize() {
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
}

void StageSelectScene::Draw() {
}

void StageSelectScene::ChangeScene() {
}
