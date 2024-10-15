#include "TitleScene.h"
#include"SceneManager.h"
#include"imgui.h"

void TitleScene::Initialize()
{
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
#ifdef _DEBUG
	ImGui::Begin("TitleScene");
	if (ImGui::Button("GoStageSelect")) {
		/*シーン切り替え依頼*/
		SceneManager::GetInstance()->ChangeScene("SELECT");
	}
	ImGui::End();
#endif // _DEBUG
}

void TitleScene::Draw()
{
}

void TitleScene::ChangeScene()
{
	
}
