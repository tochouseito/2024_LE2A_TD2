#include "StageSelectScene.h"
#include"SceneManager.h"
#include"imgui.h"
#include "TextureManager.h"

constexpr float kMap1maxX = 7035.0f;

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
}

void StageSelectScene::Finalize() {}

void StageSelectScene::Update() {
#ifdef _DEBUG
	ImGui::Begin("SelectScene");
	if (ImGui::Button("ChangeScene")) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

	ImGui::Text("currentStage: %u", currentStageNum_);
	ImGui::End();

	ImGui::Begin("map1");
	Vector3 anchorPoint = map1Sprite_->GetAnchorPoint();
	if (ImGui::DragFloat3("Anchor Point", &anchorPoint.x, 0.1f)) {
		map1Sprite_->SetAnchorPoint(anchorPoint);
	}

	Vector3 size = map1Sprite_->GetSize();
	if (ImGui::DragFloat3("Size", &size.x, 0.1f)) {
		map1Sprite_->SetSize(size);
	}

	Vector3 texLeftTop = map1Sprite_->GetTexLeftTop();
	if (ImGui::DragFloat3("Tex Left Top", &texLeftTop.x, 0.1f)) {
		map1Sprite_->SetTexLeftTop(texLeftTop);
	}

	Vector3 texSize = map1Sprite_->GetTexSize();
	if (ImGui::DragFloat3("Tex Size", &texSize.x, 0.1f)) {
		map1Sprite_->SetTexSize(texSize);
	}
	ImGui::End();
#endif // _DEBUG

	// ステージ番号の選択と決定
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneManager_->SetCurrentStageNum(currentStageNum_);
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	} else if (Input::GetInstance()->TriggerKey(DIK_A)) {
		currentStageNum_--;
		t = 0.0f;
	} else if (Input::GetInstance()->TriggerKey(DIK_D)) {
		currentStageNum_++;
		t = 0.0f;
	}

	// 値超過排斥処理
	if (currentStageNum_ < 1) {
		currentStageNum_ = 1;
	} else if (currentStageNum_ > kMaxStageNum_) {
		currentStageNum_ = kMaxStageNum_;
	}

	numberSprite_->SetTexLeftTop(Vector3(currentStageNum_ * numberSprite_->GetTexSize().x, 0.0f, 0.0f));

	selectSceneSprite_->Update();
	selectAllowSceneSprite_->Update();
	numberSprite_->Update();

	baseSprite_->Update();
	greenSprite_->Update();

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

	map1Sprite_->SetTexLeftTop({ (kMap1maxX * 0.5f) + std::cos(t) * (kMap1maxX * 0.5f),map1Sprite_->GetTexLeftTop().y,map1Sprite_->GetTexLeftTop().z });
	map2Sprite_->SetTexLeftTop({ (kMap1maxX * 0.5f) + std::cos(t) * (kMap1maxX * 0.5f),map2Sprite_->GetTexLeftTop().y,map2Sprite_->GetTexLeftTop().z });
	map3Sprite_->SetTexLeftTop({ (kMap1maxX * 0.5f) + std::cos(t) * (kMap1maxX * 0.5f),map3Sprite_->GetTexLeftTop().y,map3Sprite_->GetTexLeftTop().z });
	t += 0.0025f;
}

void StageSelectScene::Draw() {
	selectSceneSprite_->Draw();
	selectAllowSceneSprite_->Draw();
	numberSprite_->Draw();

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
		default:
			break;
	}

	for (uint32_t i = 0; i < 5; i++) {
		smallNumberSprite_[i]->Draw();
	}

	greenSprite_->Draw();
}

void StageSelectScene::ChangeScene() {}
