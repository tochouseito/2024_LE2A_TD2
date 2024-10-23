#pragma once
#include <memory>

#include"BaseScene.h"
#include "Sprite.h"
#include"Audio.h"

class TitleScene :public BaseScene {
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

	ViewProjection viewProjection_;

	std::string titleLogoTextureHandle_;
	std::unique_ptr<Sprite> titleLogoSprite_;

	std::string pressButtonTextureHandle_;
	std::unique_ptr<Sprite> pressButtonSprite_;

	// フェードスプライト
	std::string fadeTerxtureHandle_;
	std::unique_ptr<Sprite> fadeSprite_ = nullptr;
	Color color = { 1.0f,0.0f,0.0f,1.0f };

	//Audio::SoundData titleBGM;
};

