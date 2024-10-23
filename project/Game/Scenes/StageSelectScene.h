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

	// 数字
	std::string numberTextureHandle_;
	std::unique_ptr<Sprite> numberSprite_;

	// 小さい数字
	std::string smallNumberTextureHandle_;
	std::unique_ptr<Sprite> smallNumberSprite_[5];

	// 後ろのやつ
	std::string baseTextureHandle_;
	std::unique_ptr<Sprite> baseSprite_;

	// 緑のやつ
	std::string greenTextureHandle_;
	std::unique_ptr<Sprite> greenSprite_;

	// 灰色のやつ
	std::string grayTextureHandle_;
	std::vector<Sprite*> graySprite_;

	// map1Thumb
	std::string map1TextureHandle_;
	std::unique_ptr<Sprite> map1Sprite_;

	// map2Thumb
	std::string map2TextureHandle_;
	std::unique_ptr<Sprite> map2Sprite_;

	// map3Thumb
	std::string map3TextureHandle_;
	std::unique_ptr<Sprite> map3Sprite_;


	// キーリピート用のタイマー
	float keyRepeatDelay = 0.2f;    // 初回リピート遅延時間（0.5秒）
	float keyRepeatInterval = 0.1f; // リピート間隔（0.1秒）
	float moveTimer = 0.0f;         // タイマーの初期化
	bool moveTriggered = false;     // 初回の移動がトリガーされたかどうか

	// 時間
	float scrollTimer_ = 0.0f;

	// 最大ステージ数
	const uint32_t kMaxStageNum_ = 4;

	// 現在選択しているステージ数
	uint32_t currentStageNum_ = 1;



	uint32_t clearTime_ = 0;
	uint32_t clearTimeMinutes_[4]{};


	// 前のフレームで選択していたステージ
	uint32_t oldStageNum_ = 0;

};

