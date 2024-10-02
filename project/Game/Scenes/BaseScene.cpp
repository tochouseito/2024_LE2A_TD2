#include "BaseScene.h"
#include"SceneManager.h"

// ステージ数
const uint32_t BaseScene::totalStageNum_ = 3;
// ステージ番号: ここは仮の初期化
uint32_t BaseScene::stageNumber_ = 1;

void BaseScene::Initialize()
{

}

void BaseScene::Finalize()
{
}

void BaseScene::Update()
{
}

void BaseScene::Draw()
{
}

void BaseScene::ChangeScene()
{
	//sceneManager_->SetNextScene
}


