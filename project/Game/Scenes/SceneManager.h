#pragma once
#include"SceneFactory.h"
class SceneManager {
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static SceneManager* GetInstance();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/*シーン切り替え*/
	void ChangeScene(const std::string& sceneName);

	void SetIndex(uint32_t index) { defaultIndex = index; }

	void SetCurrentStageNum(const uint32_t& currentStageNumber);

	uint32_t GetCurrentStageNumber()const;

private:
	/*今のシーン*/
	BaseScene* scene_ = nullptr;
	/*次のシーン*/
	BaseScene* nextScene_ = nullptr;
	/*シーンファクトリー*/
	AbstractSceneFactory* sceneFactory_ = nullptr;

	uint32_t defaultIndex;

	// 現在のステージ
	uint32_t currentStageNumber_ = 1;

};

