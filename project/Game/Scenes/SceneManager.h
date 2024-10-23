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

	void SetIndex(uint32_t index) {
		defaultIndex = index;
	}

	void SetCurrentStageNum(const uint32_t& currentStageNumber);
	void SetIsClear(const bool& isClear);
	void SetClearTime(const uint32_t& clearTime, uint32_t stage);

	void SetIsCleared(const bool& iscleared, uint32_t stage);


	uint32_t GetCurrentStageNumber()const;
	bool GetIsClear()const;
	uint32_t GetClearTime(uint32_t stage)const;

	void CulAndSetClearTime(uint32_t totalSeconds, uint32_t stage);
	uint32_t GetCulClearTime(uint32_t index, uint32_t stage);

	uint32_t GetIsCleared(uint32_t stage);


private:
	/*今のシーン*/
	BaseScene* scene_ = nullptr;
	/*次のシーン*/
	BaseScene* nextScene_ = nullptr;
	/*シーンファクトリー*/
	AbstractSceneFactory* sceneFactory_ = nullptr;

	uint32_t defaultIndex;

	static const uint32_t kMaxStageNum_ = 4;

	// 現在のステージ
	uint32_t currentStageNumber_ = 1;

	// クリアしたかどうか
	bool isClear_ = false;

	// クリア時間
	uint32_t clearTime_[kMaxStageNum_] = { 0,0,0 };
	uint32_t clearTimeMinutes_[kMaxStageNum_][4]{};


	Audio::SoundData titleBGM;

	bool isCleared_[kMaxStageNum_] = {};


};

