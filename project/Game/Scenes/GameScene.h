#pragma once
// base
#include "DirectXCommon.h"
#include"Input.h"
#include"Audio.h"

// 3D
#include"Model.h"
#include"MainCamera.h"
#include"ViewProjection.h"
#include"Particle.h"
#include"ParticleManager.h"
#include"DebugCamera.h"
#include"MainCamera.h"

// 2D
#include"Sprite.h"

/*3Dオブジェクト*/
#include"Primitive.h"

/*Scene*/
#include"BaseScene.h"
#include"SceneManager.h"

/*externals*/
#include"SerialToArduino.h"

// c++

#include <memory>
#include <utility>
#include <algorithm>
#include <array>
#include <chrono>
#include <list>
#include <memory>
#include <set>
#include <random>
#include <vector>
#include <fstream>
#include <sstream>

// CSVLoader
#include"CSVLoader/CSVLoader.h"
#include"Game/MapChipField/MapChipField.h"

/*Objects*/
#include "CollisionManager/CollisionManager.h"

#include "GravityArrow/GravityArrow.h"
#include "Objects/Player/Player.h"
#include "Objects/Enemy/Enemy.h"
#include "Objects/Blocks/Blocks.h"
#include "Objects/Goal/Goal.h"

// DeltaTime
#include "Needle.h"
#include"Game/DeltaTime/DeltaTime.h"

class GameScene :
	public BaseScene {
public:
	GameScene();
	~GameScene();

	// Main
	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;

	// シーンの切り替え依頼
	void ChangeScene()override {}
	void SetSceneManager(SceneManager* sceneManager) override {
		sceneManager_ = sceneManager;
	}

	void GenerateBlocks();

	void CheckAllCollisions();

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Audio::SoundData SoundData1;
	SceneManager* sceneManager_ = nullptr;

	// テクスチャハンドル
	std::string textureHandle_[6]{};
	std::string electricGaugeTex_;

	Particle* particles_ = nullptr;
	Sprite* sprite_ = nullptr;

	/*ジオメトリ*/
	Primitive* primitive_ = nullptr;

	/*シリアルライブラリ*/
	SerialToArduino* serial = nullptr;
	int heartRate = 0;
	int gsrValue = 0;
	/*パーティクルマネージャ*/
	std::unique_ptr<ParticleManager> particleManager_;

	//MainCamera* mainCamera_ = nullptr;
	/*エミッターマネージャ*/
	//std::unique_ptr<EmitterManager> emitterManager_;


	// デバッグカメラ
	std::unique_ptr < DebugCamera> debugCamera_;
	bool useDebugCamera_ = false;

	std::unique_ptr<MainCamera> mainCamera_;

	// カメラ
	ViewProjection viewProjection_;

	///////////////////////////////////////////////////////////////////////////////////////////////////

	/*MapChip*/
	std::unique_ptr<MapChipField> mapChipField_ = nullptr;

	/*Objects*/

	// Player
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<Model> playerModel_ = nullptr;

	// Enemy
	std::unique_ptr<Enemy> enemy_ = nullptr;
	std::unique_ptr<Model> enemyModel_ = nullptr;

	// Blocks
	std::vector<std::vector<std::unique_ptr<Blocks>>> blocks_;
	//std::unique_ptr<Blocks> blocks_ = nullptr;
	std::unique_ptr<Model> blocksModel_ = nullptr;

	// Goal
	std::unique_ptr<Goal> goal_ = nullptr;
	std::unique_ptr<Model> goalModel_ = nullptr;

	// 針
	std::vector<std::unique_ptr<Needle>> needles_;
	std::unique_ptr<Model> upNeedleModel_ = nullptr;
	std::unique_ptr<Model> downNeedleModel_ = nullptr;

	std::unique_ptr<GravityArrow> gravityArrow_ = nullptr;
	std::vector<std::unique_ptr<Model>> gravityArrowModel_;

	// 衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	std::unique_ptr<DeltaTime> deltaTime_ = nullptr;
};

