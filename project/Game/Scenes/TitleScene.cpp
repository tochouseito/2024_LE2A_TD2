#include "TitleScene.h"
#include"SceneManager.h"
#include"imgui.h"
#include "TextureManager.h"

void TitleScene::Initialize() {
	viewProjection_.Initialize();
	DirectXCommon::GetInstance()->SetViewProjection(&viewProjection_);

	titleLogoTextureHandle_ = TextureManager::Load("./Resources/GUI/title.png");
	titleLogoSprite_ = std::make_unique<Sprite>();
	titleLogoSprite_->Initialize({ 0.0f,0.0f,0.0f }, &viewProjection_, titleLogoTextureHandle_);

	pressButtonTextureHandle_ = TextureManager::Load("./Resources/GUI/pressbutton.png");
	pressButtonSprite_ = std::make_unique<Sprite>();
	pressButtonSprite_->Initialize({ 640.0f,512.0f,0.0f }, &viewProjection_, pressButtonTextureHandle_);
	pressButtonSprite_->SetAnchorPoint(Vector3(0.5f, 0.5f, 0.0f));

	fadeTerxtureHandle_ = TextureManager::Load("./Resources/white.png");
	fadeSprite_ = std::make_unique<Sprite>();
	fadeSprite_->Initialize({ 640.0f,360.0f,0.0f }, &viewProjection_, fadeTerxtureHandle_);
	fadeSprite_->SetSize(Vector3(1280.0f, 720.0f));
	fadeSprite_->SetAnchorPoint(Vector3(0.5f, 0.5f, 0.0f));
	fadeSprite_->SetColor(color);

	state = { 0.0f, 1 };
	//Audio::GetInstance()->Initialize();
	titleBGM = Audio::GetInstance()->SoundLordWave("./Resources/it_takes_a_hero.wav");
	Audio::GetInstance()->SoundPlayWave(Audio::GetInstance()->GetXAudio2(), titleBGM, true);
}

void TitleScene::Finalize() {
	/*Audio::GetInstance()->SoundStop(titleBGM);
	Audio::GetInstance()->Finalize();*/
	Audio::GetInstance()->SoundStop(titleBGM);
 	Audio::GetInstance()->SoundUnLord(&titleBGM);
}

void TitleScene::Update() {
#ifdef _DEBUG
	ImGui::Begin("TitleScene");
	if (ImGui::Button("GoStageSelect")) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("SELECT");
	}
	ImGui::End();
#endif // _DEBUG

	XINPUT_STATE controllerState;
	bool controllerConnected = Input::GetInstance()->GetJoystickState(0, controllerState);

	bool keyboardStart = Input::GetInstance()->TriggerKey(DIK_SPACE);
	bool controllerStart = controllerConnected && Input::GetInstance()->TriggerControllerButton(0, XINPUT_GAMEPAD_A);

	if (keyboardStart || controllerStart) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("SELECT");
	}

	pressButtonSprite_->SetPosition(EaseInOutBetweenTwoPoints(kButtonStartPos_, kButtonEndPos_, state, deltaTime, speed));

	titleLogoSprite_->Update();
	pressButtonSprite_->Update();
	fadeSprite_->Update();

	viewProjection_.UpdateMatrix();
}

void TitleScene::Draw() {
	titleLogoSprite_->Draw();
	pressButtonSprite_->Draw();
	//fadeSprite_->Draw();
}

void TitleScene::ChangeScene() {

}

Vector3 TitleScene::EaseInOutBetweenTwoPoints(const Vector3& start, const Vector3& end, EaseState& state, float deltaTime, float speed) {
	// `t`をイーズインに応じて進行
	state.t += state.direction * speed * deltaTime;

	// `t`のクランプ処理 (0.0f ～ 1.0fの範囲を維持)
	if (state.t >= 1.0f) {
		state.t = 1.0f;
		state.direction = -1; // 逆方向に
	} else if (state.t <= 0.0f) {
		state.t = 0.0f;
		state.direction = 1; // 順方向に
	}

	// イーズインの関数 (t^2 を使用)
	float easedT = std::powf(state.t, 6);

	// 線形補間を使って2点の間を移動
	Vector3 result;
	result.x = start.x + (end.x - start.x) * easedT;
	result.y = start.y + (end.y - start.y) * easedT;
	result.z = start.z + (end.z - start.z) * easedT;

	return result;
}