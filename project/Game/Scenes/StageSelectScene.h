#pragma once

#include <memory>

// 2D
#include"Sprite.h"

#include"BaseScene.h"
class StageSelectScene :public BaseScene {
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

	// カメラ
	ViewProjection viewProjection_;

	// セレクトシーンのテクスチャ
	std::string selectSceneTextureHandle_;
	std::unique_ptr<Sprite> selectSceneSprite_;

	// 矢印
	std::string selectAllowTextureHandle_;
	std::unique_ptr<Sprite> selectAllowSceneSprite_;

};

