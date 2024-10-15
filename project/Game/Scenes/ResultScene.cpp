#include "ResultScene.h"
#include"SceneManager.h"
#include"imgui.h"

void ResultScene::Initialize() {

}

void ResultScene::Finalize() {

}

void ResultScene::Update() {
	ImGui::Begin("ResultScene");
	if (ImGui::Button("GoTitle")) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	if (ImGui::Button("GoStageSelect")) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("SELECT");
	}
	ImGui::End();

}

void ResultScene::Draw() {

}

void ResultScene::ChangeScene() {

}
