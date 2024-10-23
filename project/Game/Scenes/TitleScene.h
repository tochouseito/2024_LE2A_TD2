#pragma once
#include <memory>

#include"BaseScene.h"
#include "Sprite.h"
#include"Audio.h"

class TitleScene:public BaseScene {
public:

	struct EaseState {
		float t;
		int direction;
	};

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

	void SetSceneManager(SceneManager* sceneManager) override {
		sceneManager_ = sceneManager;
	}

	// イーズイン/アウトを行う関数
	Vector3 EaseInOutBetweenTwoPoints(const Vector3& start, const Vector3& end, EaseState& state, float deltaTime, float speed);

private:
	SceneManager* sceneManager_ = nullptr;

	// 状態を初期化
	EaseState state = { 0.0f, 1 };

	// アニメーション用変数
	const Vector3 kButtonStartPos_ = { 640.0f,480.0f,0.0f };
	const Vector3 kButtonEndPos_ = { 640.0f,512.0f,0.0f };

	float deltaTime = 0.016f;
	float speed = 0.8f;

	ViewProjection viewProjection_;

	std::string titleLogoTextureHandle_;
	std::unique_ptr<Sprite> titleLogoSprite_;

	std::string pressButtonTextureHandle_;
	std::unique_ptr<Sprite> pressButtonSprite_;

	// フェードスプライト
	std::string fadeTerxtureHandle_;
	std::unique_ptr<Sprite> fadeSprite_ = nullptr;
	Color color = { 1.0f,0.0f,0.0f,1.0f };

	Audio::SoundData titleBGM;
};

