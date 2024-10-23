#include "SceneManager.h"
#include"SrvManager.h"
#include"TextureManager.h"

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

SceneManager::~SceneManager() {

}

void SceneManager::Initialize() {
	sceneFactory_ = new SceneFactory();

	for (uint32_t i = 0; i < 3; i++) {
		isCleared_[i] = false;
	}
	titleBGM = Audio::GetInstance()->SoundLordWave("./Resources/it_takes_a_hero.wav");
	Audio::GetInstance()->SoundPlayWave(Audio::GetInstance()->GetXAudio2(), titleBGM, true);
}

void SceneManager::Finalize() {
	delete sceneFactory_;
	/*最後のシーンの終了と解放*/
	scene_->Finalize();
	delete scene_;
}

void SceneManager::Update() {
	if (nextScene_) {
		/*旧シーンの終了*/
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}
		/*シーン切り替え*/
		scene_ = nextScene_;
		nextScene_ = nullptr;
		/*シーンマネージャをセット*/
		scene_->SetSceneManager(this);
		SrvManager::GetInstance()->SetSRVIndex(defaultIndex);
		TextureManager::GetInstance()->TexReset();
		/*次のシーンを初期化する*/
		scene_->Initialize();
	}
	scene_->Update();
}

void SceneManager::Draw() {
	scene_->Draw();
}

void SceneManager::ChangeScene(const std::string& sceneName) {
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);
	if (sceneName == "GAMEPLAY") {
		Audio::GetInstance()->SoundUnLord(&titleBGM);
	}
	/*if (sceneName == "TITLE") {
		Audio::GetInstance()->SoundPlayWave(Audio::GetInstance()->GetXAudio2(), titleBGM, true);
	}*/
	/*次のシーン生成*/
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}

void SceneManager::SetCurrentStageNum(const uint32_t& currentStageNumber) {
	currentStageNumber_ = currentStageNumber;
}

void SceneManager::SetIsClear(const bool& isClear) {
	isClear_ = isClear;
}

void SceneManager::SetClearTime(const uint32_t& clearTime, uint32_t stage) {
	clearTime_[stage - 1] = clearTime;
}

void SceneManager::SetIsCleared(const bool& iscleared, uint32_t stage) {
	isCleared_[stage - 1] = iscleared;
}

uint32_t SceneManager::GetCurrentStageNumber() const {
	return currentStageNumber_;
}

bool SceneManager::GetIsClear() const {
	return isClear_;
}

uint32_t SceneManager::GetClearTime(uint32_t stage) const {
	return clearTime_[stage - 1];
}

void SceneManager::CulAndSetClearTime(uint32_t totalSeconds, uint32_t stage) {

	clearTime_[stage - 1] = totalSeconds;

	// 分と秒を計算
	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;

	// 分を十の位と一の位に分ける
	int minutesTens = minutes / 10;
	int minutesOnes = minutes % 10;

	// 秒を十の位と一の位に分ける
	int secondsTens = seconds / 10;
	int secondsOnes = seconds % 10;

	clearTimeMinutes_[stage - 1][0] = minutesTens;
	clearTimeMinutes_[stage - 1][1] = minutesOnes;
	clearTimeMinutes_[stage - 1][2] = secondsTens;
	clearTimeMinutes_[stage - 1][3] = secondsOnes;

}

uint32_t SceneManager::GetCulClearTime(uint32_t index, uint32_t stage) {
	return clearTimeMinutes_[stage - 1][index];
}

uint32_t SceneManager::GetIsCleared(uint32_t stage) {
	return isCleared_[stage - 1];
}
