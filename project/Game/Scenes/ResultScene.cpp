#include "ResultScene.h"
#include"SceneManager.h"
#include"imgui.h"

void ResultScene::Initialize() {
	// クリアしたかどうかをセット
	isClear_ = sceneManager_->GetIsClear();
	// クリア時間をセット
	clearTime_ = sceneManager_->GetClearTime();
}

void ResultScene::Finalize() {

}

void ResultScene::Update() {
#ifdef _DEBUG
	ImGui::Begin("ResultScene");
	if (ImGui::Button("GoTitle")) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	if (ImGui::Button("GoStageSelect")) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("SELECT");
	}
	switch (nextScene_) {
	case ResultScene::kTitle:
		ImGui::Text("title");
		break;
	case ResultScene::kSelect:
		ImGui::Text("select");
		break;
	}

	ImGui::End();
#endif // _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		switch (nextScene_) {
		case ResultScene::kTitle:
			SceneManager::GetInstance()->ChangeScene("TITLE");
			break;
		case ResultScene::kSelect:
			SceneManager::GetInstance()->ChangeScene("SELECT");
			break;
		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_A)) {
		nextScene_--;
	} else if (Input::GetInstance()->TriggerKey(DIK_D)) {
		nextScene_++;
	}

	if (nextScene_ < 0) {
		nextScene_ = 0;
	} else if (nextScene_ > 1) {
		nextScene_ = 1;
	}


}

void ResultScene::Draw() {

}

void ResultScene::ChangeScene() {

}