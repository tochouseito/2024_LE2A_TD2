#include "StageSelectScene.h"
#include"SceneManager.h"
#include"imgui.h"
#include "TextureManager.h"

// マップの右端座標
constexpr float kMap1maxX = 7035.0f;
constexpr float kMap2maxX = 3645.0f;
constexpr float kMap3maxX = 3835.0f;
constexpr float kMap4maxX = 5180.0f;

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
	numberSprite_->Initialize({ 764.0f,650.0f,0.0f }, &viewProjection_, numberTextureHandle_);
	numberSprite_->SetAnchorPoint(Vector3(0.5f, 0.5f, 0.0f));
	numberSprite_->SetSize(Vector3(48.0f, 48.0f, 0.0f));
	numberSprite_->SetTexSize(Vector3(48.0f, 48.0f, 0.0f));


	smallNumberTextureHandle_ = TextureManager::Load("./Resources/GUI/timeNumbers.png");
	for (uint32_t i = 0; i < 5; i++) {
		smallNumberSprite_[i] = std::make_unique<Sprite>();
		smallNumberSprite_[i]->Initialize({ 810.0f - 64.0f + i * 32.0f,94.0f }, &viewProjection_, smallNumberTextureHandle_);
		smallNumberSprite_[i]->SetAnchorPoint(Vector3(0.5f, 0.5f, 0.0f));
		smallNumberSprite_[i]->SetSize(Vector3(32.0f, 32.0f, 0.0f));
		smallNumberSprite_[i]->SetTexSize(Vector3(32.0f, 32.0f, 0.0f));
	}

	baseTextureHandle_ = TextureManager::Load("./Resources/GUI/selectBase.png");
	baseSprite_ = std::make_unique<Sprite>();
	baseSprite_->Initialize({ 640.0f,360.0f + 3.0f,0.0f }, &viewProjection_, baseTextureHandle_);
	baseSprite_->SetAnchorPoint({ 0.5f,0.5f,0.0f });

	greenTextureHandle_ = TextureManager::Load("./Resources/GUI/selectGreen.png");
	greenSprite_ = std::make_unique<Sprite>();
	greenSprite_->Initialize({ 640.0f,360.0f,0.0f }, &viewProjection_, greenTextureHandle_);
	greenSprite_->SetAnchorPoint({ 0.5f,0.5f,0.0f });


	grayTextureHandle_ = TextureManager::Load("./Resources/GUI/selectGray.png");

	for (size_t i = 0; i < 5; ++i) {
		graySprite_.push_back(new Sprite());
		graySprite_[i]->Initialize({ -448.0f + (544.0f * i),360.0f,0.0f }, &viewProjection_, grayTextureHandle_);
		graySprite_[i]->SetAnchorPoint({ 0.5f,0.5f,0.0f });
		graySprite_[i]->SetSize(graySprite_[i]->GetSize() * 0.5f);
	}


	map1TextureHandle_ = TextureManager::Load("./Resources/Map1.png");
	map1Sprite_ = std::make_unique<Sprite>();
	map1Sprite_->Initialize({ 640.0f,360.0f,0.0f }, &viewProjection_, map1TextureHandle_);
	map1Sprite_->SetAnchorPoint({ 0.5f,0.5f,0.0f });
	map1Sprite_->SetSize({ 512.0f, 256.0f,0.0f });
	map1Sprite_->SetTexSize({ 2280.0f, 1092.0f,0.0f });

	map2TextureHandle_ = TextureManager::Load("./Resources/Map2.png");
	map2Sprite_ = std::make_unique<Sprite>();
	map2Sprite_->Initialize({ 640.0f,360.0f,0.0f }, &viewProjection_, map2TextureHandle_);
	map2Sprite_->SetAnchorPoint({ 0.5f,0.5f,0.0f });
	map2Sprite_->SetSize({ 512.0f, 256.0f,0.0f });
	map2Sprite_->SetTexSize({ 2280.0f, 1092.0f,0.0f });

	map3TextureHandle_ = TextureManager::Load("./Resources/Map3.png");
	map3Sprite_ = std::make_unique<Sprite>();
	map3Sprite_->Initialize({ 640.0f,360.0f,0.0f }, &viewProjection_, map3TextureHandle_);
	map3Sprite_->SetAnchorPoint({ 0.5f,0.5f,0.0f });
	map3Sprite_->SetSize({ 512.0f, 256.0f,0.0f });
	map3Sprite_->SetTexSize({ 2280.0f, 1092.0f,0.0f });

	map4TextureHandle_ = TextureManager::Load("./Resources/Map4.png");
	map4Sprite_ = std::make_unique<Sprite>();
	map4Sprite_->Initialize({ 640.0f,360.0f,0.0f }, &viewProjection_, map4TextureHandle_);
	map4Sprite_->SetAnchorPoint({ 0.5f,0.5f,0.0f });
	map4Sprite_->SetSize({ 512.0f, 256.0f,0.0f });
	map4Sprite_->SetTexSize({ 2280.0f, 1092.0f,0.0f });

	currentStageNum_ = 1;
}

void StageSelectScene::Finalize() {
	for (Sprite* const graySprite : graySprite_) {
		delete graySprite;
	}
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

	{
		ImGui::Begin("map1");
		Vector3 anchorPoint = map1Sprite_->GetAnchorPoint();
		if (ImGui::DragFloat3("Anchor Point##map1", &anchorPoint.x, 0.1f)) {
			map1Sprite_->SetAnchorPoint(anchorPoint);
		}

		Vector3 size = map1Sprite_->GetSize();
		if (ImGui::DragFloat3("Size##map1", &size.x, 0.1f)) {
			map1Sprite_->SetSize(size);
		}

		Vector3 texLeftTop = map1Sprite_->GetTexLeftTop();
		if (ImGui::DragFloat3("Tex Left Top", &texLeftTop.x, 0.1f)) {
			map1Sprite_->SetTexLeftTop(texLeftTop);
		}

		Vector3 texSize = map1Sprite_->GetTexSize();
		if (ImGui::DragFloat3("Tex Size##map1", &texSize.x, 0.1f)) {
			map1Sprite_->SetTexSize(texSize);
		}
		ImGui::End();
	}

	{
		ImGui::Begin("map2");
		Vector3 anchorPoint = map2Sprite_->GetAnchorPoint();
		if (ImGui::DragFloat3("Anchor Point##map2", &anchorPoint.x, 0.1f)) {
			map2Sprite_->SetAnchorPoint(anchorPoint);
		}

		Vector3 size = map2Sprite_->GetSize();
		if (ImGui::DragFloat3("Size##map2", &size.x, 0.1f)) {
			map2Sprite_->SetSize(size);
		}

		Vector3 texLeftTop = map2Sprite_->GetTexLeftTop();
		if (ImGui::DragFloat3("Tex Left Top##map2", &texLeftTop.x, 0.1f)) {
			map2Sprite_->SetTexLeftTop(texLeftTop);
		}

		Vector3 texSize = map2Sprite_->GetTexSize();
		if (ImGui::DragFloat3("Tex Size##map2", &texSize.x, 0.1f)) {
			map2Sprite_->SetTexSize(texSize);
		}
		ImGui::End();
	}

	{
		ImGui::Begin("map3");
		Vector3 anchorPoint = map3Sprite_->GetAnchorPoint();
		if (ImGui::DragFloat3("Anchor Point##map3", &anchorPoint.x, 0.1f)) {
			map3Sprite_->SetAnchorPoint(anchorPoint);
		}

		Vector3 size = map3Sprite_->GetSize();
		if (ImGui::DragFloat3("Size##map3", &size.x, 0.1f)) {
			map3Sprite_->SetSize(size);
		}

		Vector3 texLeftTop = map3Sprite_->GetTexLeftTop();
		if (ImGui::DragFloat3("Tex Left Top##map3", &texLeftTop.x, 0.1f)) {
			map3Sprite_->SetTexLeftTop(texLeftTop);
		}

		Vector3 texSize = map3Sprite_->GetTexSize();
		if (ImGui::DragFloat3("Tex Size##map3", &texSize.x, 0.1f)) {
			map3Sprite_->SetTexSize(texSize);
		}
		ImGui::End();
	}

	{
		ImGui::Begin("map4");
		Vector3 anchorPoint = map4Sprite_->GetAnchorPoint();
		if (ImGui::DragFloat3("Anchor Point##map4", &anchorPoint.x, 0.1f)) {
			map4Sprite_->SetAnchorPoint(anchorPoint);
		}

		Vector3 size = map4Sprite_->GetSize();
		if (ImGui::DragFloat3("Size##map4", &size.x, 0.1f)) {
			map4Sprite_->SetSize(size);
		}

		Vector3 texLeftTop = map4Sprite_->GetTexLeftTop();
		if (ImGui::DragFloat3("Tex Left Top##map4", &texLeftTop.x, 0.1f)) {
			map4Sprite_->SetTexLeftTop(texLeftTop);
		}

		Vector3 texSize = map4Sprite_->GetTexSize();
		if (ImGui::DragFloat3("Tex Size##map4", &texSize.x, 0.1f)) {
			map4Sprite_->SetTexSize(texSize);
		}
		ImGui::End();
	}
#endif // _DEBUG

	XINPUT_STATE controllerState;
	bool controllerConnected = Input::GetInstance()->GetJoystickState(0, controllerState);

	float stickX = 0.0f;
	if (controllerConnected) {
		stickX = static_cast<float>(controllerState.Gamepad.sThumbLX) / 32767.0f;  // 正規化したスティックのX入力（-1.0 ~ 1.0）
	}

	// 左右移動入力
	bool keyboardMoveLeft = Input::GetInstance()->PushKey(DIK_A);
	bool keyboardMoveRight = Input::GetInstance()->PushKey(DIK_D);
	bool controllerStickMoveLeft = stickX < -0.5f;  // スティックをある程度倒したときに左移動
	bool controllerStickMoveRight = stickX > 0.5f;  // スティックをある程度倒したときに右移動
	bool controllerDPadMoveLeft = Input::GetInstance()->PushControllerButton(0, XINPUT_GAMEPAD_DPAD_LEFT);
	bool controllerDPadMoveRight = Input::GetInstance()->PushControllerButton(0, XINPUT_GAMEPAD_DPAD_RIGHT);

	bool keyboardStart = Input::GetInstance()->TriggerKey(DIK_SPACE);
	bool controllerStart = controllerConnected && Input::GetInstance()->TriggerControllerButton(0, XINPUT_GAMEPAD_A);

	// ステージ番号の選択と決定
	if (keyboardStart || controllerStart) {
		sceneManager_->SetCurrentStageNum(currentStageNum_);
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

	// 初回入力の処理
	if (!moveTriggered && (keyboardMoveLeft || controllerStickMoveLeft || controllerDPadMoveLeft)) {
		currentStageNum_--;
		scrollTimer_ = 0.0f;
		moveTimer = keyRepeatDelay;  // タイマーをリピート遅延時間でリセット
		moveTriggered = true;        // 初回移動トリガー
	} else if (!moveTriggered && (keyboardMoveRight || controllerStickMoveRight || controllerDPadMoveRight)) {
		currentStageNum_++;
		scrollTimer_ = 0.0f;
		moveTimer = keyRepeatDelay;
		moveTriggered = true;
	}

	// タイマーをリセットする（移動が止まったとき）
	if (!keyboardMoveLeft && !controllerStickMoveLeft && !controllerDPadMoveLeft && !keyboardMoveRight && !controllerStickMoveRight && !controllerDPadMoveRight) {
		moveTriggered = false;
		moveTimer = 0.0f;  // タイマーをリセット
	}

	// リピート処理
	if (moveTriggered && moveTimer > 0.0f) {
		moveTimer -= 1.0f / 60.0f;  // タイマーを減少させる
	} else if (moveTriggered && moveTimer <= 0.0f) {
		// タイマーが0以下ならリピート処理を行う
		if (keyboardMoveLeft || controllerStickMoveLeft || controllerDPadMoveLeft) {
			currentStageNum_--;
			scrollTimer_ = 0.0f;
			moveTimer = keyRepeatInterval;  // リピート間隔でタイマーをリセット
		} else if (keyboardMoveRight || controllerStickMoveRight || controllerDPadMoveRight) {
			currentStageNum_++;
			scrollTimer_ = 0.0f;
			moveTimer = keyRepeatInterval;
		}
	}

	// 値超過排斥処理
	if (currentStageNum_ < 1) {
		currentStageNum_ = 1;
	} else if (currentStageNum_ > kMaxStageNum_) {
		currentStageNum_ = kMaxStageNum_;
	}

	// ステージを切り替えたら
	if (currentStageNum_ != oldStageNum_) {
		numberSprite_->SetAnchorPoint({ 0.0f,-0.5f,0.0f });
		if (currentStageNum_ < oldStageNum_) {
			for (size_t i = 0; i < 5; ++i) {
				graySprite_[i]->SetAnchorPoint({ 2.0f, 0.5f,0.0f });
			}
		} else if (currentStageNum_ > oldStageNum_) {
			for (size_t i = 0; i < 5; ++i) {
				graySprite_[i]->SetAnchorPoint({ -2.0f, 0.5f,0.0f });
			}
		}
	}

	oldStageNum_ = currentStageNum_;

	for (size_t i = 0; i < 5; ++i) {
		graySprite_[i]->SetAnchorPoint({ std::lerp(graySprite_[i]->GetAnchorPoint().x,0.5f,1.0f / 60.0f * 15.0f),graySprite_[i]->GetAnchorPoint().y,graySprite_[i]->GetAnchorPoint().z });
	}

	numberSprite_->SetTexLeftTop(Vector3(currentStageNum_ * numberSprite_->GetTexSize().x, 0.0f, 0.0f));
	numberSprite_->SetAnchorPoint({ numberSprite_->GetAnchorPoint().x,std::lerp(numberSprite_->GetAnchorPoint().y, 0.5f,1.0f / 60.0f * 15.0f),numberSprite_->GetAnchorPoint().z });

	selectSceneSprite_->Update();
	selectAllowSceneSprite_->Update();
	numberSprite_->Update();

	baseSprite_->Update();
	greenSprite_->Update();

	for (Sprite* const graySprite : graySprite_) {
		graySprite->Update();
	}

	smallNumberSprite_[0]->SetTexLeftTop(Vector3(32.0f * sceneManager_->GetCulClearTime(0, currentStageNum_), 0.0f, 0.0f));
	smallNumberSprite_[1]->SetTexLeftTop(Vector3(32.0f * sceneManager_->GetCulClearTime(1, currentStageNum_), 0.0f, 0.0f));
	smallNumberSprite_[2]->SetTexLeftTop(Vector3(32.0f * 10.0f, 0.0f, 0.0f));
	smallNumberSprite_[3]->SetTexLeftTop(Vector3(32.0f * sceneManager_->GetCulClearTime(2, currentStageNum_), 0.0f, 0.0f));
	smallNumberSprite_[4]->SetTexLeftTop(Vector3(32.0f * sceneManager_->GetCulClearTime(3, currentStageNum_), 0.0f, 0.0f));


	for (uint32_t i = 0; i < 5; i++) {
		smallNumberSprite_[i]->Update();
	}

	map1Sprite_->Update();
	map2Sprite_->Update();
	map3Sprite_->Update();
	map4Sprite_->Update();

	map1Sprite_->SetTexLeftTop({ (1.0f - std::cos(scrollTimer_)) * (kMap1maxX * 0.5f),map1Sprite_->GetTexLeftTop().y,map1Sprite_->GetTexLeftTop().z });
	map2Sprite_->SetTexLeftTop({ (1.0f - std::cos(scrollTimer_)) * (kMap2maxX * 0.5f),map2Sprite_->GetTexLeftTop().y,map2Sprite_->GetTexLeftTop().z });
	map3Sprite_->SetTexLeftTop({ (1.0f - std::cos(scrollTimer_)) * (kMap3maxX * 0.5f),map3Sprite_->GetTexLeftTop().y,map3Sprite_->GetTexLeftTop().z });
	map4Sprite_->SetTexLeftTop({ (1.0f - std::cos(scrollTimer_)) * (kMap4maxX * 0.5f),map4Sprite_->GetTexLeftTop().y,map4Sprite_->GetTexLeftTop().z });
	scrollTimer_ += 0.0025f;
}

void StageSelectScene::Draw() {
	selectSceneSprite_->Draw();
	numberSprite_->Draw();

	for (Sprite* const graySprite : graySprite_) {
		graySprite->Draw();
	}

	selectAllowSceneSprite_->Draw();

	baseSprite_->Draw();

	// map1
	switch (currentStageNum_) {
	case 1:
		map1Sprite_->Draw();
		break;
	case 2:
		map2Sprite_->Draw();
		break;
	case 3:
		map3Sprite_->Draw();
		break;
	case 4:
		map4Sprite_->Draw();
		break;
	default:
		break;
	}


	greenSprite_->Draw();

	for (uint32_t i = 0; i < 5; i++) {
		smallNumberSprite_[i]->Draw();
	}
}

void StageSelectScene::ChangeScene() {}
