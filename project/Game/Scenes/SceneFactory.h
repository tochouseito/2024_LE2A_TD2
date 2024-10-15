#pragma once

#include "AbstractSceneFactory.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "StageSelectScene.h"
#include "ResultScene.h"

class SceneFactory :public AbstractSceneFactory {
public:
	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName"></param>
	/// <returns></returns>
	BaseScene* CreateScene(const std::string& sceneName) override;
};

