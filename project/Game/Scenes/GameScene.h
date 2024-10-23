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
#include "Objects/PlayerBullet/PlayerBullet.h"

// DeltaTime
#include"Game/DeltaTime/DeltaTime.h"
#include "Needle/Needle.h"

class GameScene :

	public BaseScene {

public:
	GameScene();
	~GameScene();

	// Main
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	// シーンの切り替え依頼
	void ChangeScene() override {}

	void SetSceneManager(SceneManager* sceneManager) override {
		sceneManager_ = sceneManager;
	}

	void GenerateBlocks();

	void GenerateBullets();

	void CheckAllCollisions();

	void EnemyAttack(const uint32_t& enemyAttackYIndex, const Enemy::Behavior& behavior);

	bool AABBIntersects(const AABB& a, const AABB& b);


	void StartAnimation();

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
	std::unique_ptr<DebugCamera> debugCamera_;
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
	std::vector < Model*> playerModels_;
	std::vector<Model::Animation*> plAnimas_;

	// Enemy
	std::unique_ptr<Enemy> enemy_ = nullptr;
	std::unique_ptr<Model> enemyModel_ = nullptr;
	std::unique_ptr<Model> enemyFace_ = nullptr;
	std::unique_ptr<Model> enemyEyeNormal = nullptr;
	std::unique_ptr<Model> enemyEyeHit = nullptr;

	// エネミーの攻撃用モデル
	std::unique_ptr<Model> enemyPreliminaryModel_ = nullptr;
	std::unique_ptr<Model> enemyAttackModel_ = nullptr;
	WorldTransform enemyAttackWorldTransform_;

	// Blocks
	std::vector<std::vector<std::unique_ptr<Blocks>>> blocks_;
	//std::unique_ptr<Blocks> blocks_ = nullptr;
	std::unique_ptr<Model> blocksModel_ = nullptr;

	// Goal
	std::unique_ptr<Goal> goal_ = nullptr;
	std::unique_ptr<Model> goalModel_ = nullptr;


	// Bullet
	std::vector<std::unique_ptr<PlayerBullet>> bullets_;
	std::unique_ptr<Model> bulletModel_ = nullptr;

	// 針
	std::vector<std::unique_ptr<Needle>> needles_;

	std::unique_ptr<Model> upNeedleModel_ = nullptr;
	std::unique_ptr<Model> downNeedleModel_ = nullptr;

	std::unique_ptr<GravityArrow> gravityArrow_ = nullptr;
	std::vector<Model*> gravityArrowModels_;

	// 衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	std::unique_ptr<DeltaTime> deltaTime_ = nullptr;

	// プレイヤーのAABB
	AABB playerAABB_{};
	// エネミーの攻撃判定AABB
	AABB enemyAttackAABB_{};

	uint32_t currentStageNum_ = 1;

	// アニメーションの時間
	const uint32_t kSceneStartAnimationTime_ = 240;
	// アニメーションの現在タイマー
	uint32_t sceneStartAnimationTimer_ = 0;

	// アニメーション
	bool isPlayStartAnimation_ = true;
};
