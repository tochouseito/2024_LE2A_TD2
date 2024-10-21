#include "TitleScene.h"
#include"SceneManager.h"
#include"imgui.h"
#include "TextureManager.h"

void TitleScene::Initialize() {
	viewProjection_.Initialize();
	DirectXCommon::GetInstance()->SetViewProjection(&viewProjection_);


	guiTextureHandle_ = TextureManager::GetInstance()->Load("./Resources/GUI/gui.png");

	guiSprite_ = std::make_unique<Sprite>();
	guiSprite_->Initialize({ 0.0f,0.0f,0.0f }, &viewProjection_, guiTextureHandle_);
}

void TitleScene::Finalize() {}

void TitleScene::Update() {
#ifdef _DEBUG
	ImGui::Begin("TitleScene");
	if (ImGui::Button("GoStageSelect")) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("SELECT");
	}
	ImGui::End();
#endif // _DEBUG

	//XINPUT_STATE controllerState;
	//bool controllerConnected = Input::GetInstance()->GetJoystickState(0, controllerState);

	//float stickX = 0.0f;
	//if (controllerConnected) {s
	//	stickX = static_cast<float>(controllerState.Gamepad.sThumbLX) / 32767.0f;  // 正規化したスティックのX入力（-1.0 ~ 1.0）
	//}
	//// 左右移動入力
	//bool keyboardMoveLeft = Input::GetInstance()->PushKey(DIK_A);
	//bool keyboardMoveRight = Input::GetInstance()->PushKey(DIK_D);
	//bool controllerMoveLeft = stickX < -0.2f;
	//bool controllerMoveRight = stickX > 0.2f;
	//if (keyboardMoveLeft || keyboardMoveRight || controllerMoveLeft || controllerMoveRight) {
	//	// 左右処理
	//	Vector3 acceleration = {};
	//	if (keyboardMoveRight || controllerMoveRight) {
	//		// 左移動中の右入力
	//	} else if (keyboardMoveLeft || controllerMoveLeft) {
	//		// 右移動中の左入力
	//	}
	//}

	guiSprite_->Update();

	viewProjection_.UpdateMatrix();
}

void TitleScene::Draw() {
	guiSprite_->Draw();
}

void TitleScene::ChangeScene() {

}
