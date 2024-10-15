#include "SceneFactory.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	/*次のシーン生成*/
	BaseScene* newScene = nullptr;
	if (sceneName == "TITLE") {
		newScene = new TitleScene();
	} else if (sceneName == "SELECT") {
		newScene = new StageSelectScene();
	} else if (sceneName == "GAMEPLAY") {
		newScene = new GameScene();
	} else if (sceneName == "RESULT") {
		newScene = new ResultScene();
	}
	return newScene;
}