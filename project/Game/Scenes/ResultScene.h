#pragma once

#include"BaseScene.h"
class ResultScene :public BaseScene {
private:
	enum NextScene {
		kTitle = 0,
		kSelect,
	};
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// シーンの切り替え依頼
	/// </summary>
	void ChangeScene()override;

	void SetSceneManager(SceneManager* sceneManager) override { sceneManager_ = sceneManager; }

private:
	SceneManager* sceneManager_ = nullptr;

	bool isClear_ = false;

	uint32_t clearTime_ = 0;
	uint32_t nextScene_ = kTitle;
};

