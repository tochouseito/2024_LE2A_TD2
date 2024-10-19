#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"
#include <optional>

#include "CollisionManager/Collider.h"

class Enemy: public Collider {
public:
	enum class Behavior {
		kRoot,			// 通常行動
		kPreliminary,	// 攻撃予備動作
		kAttack,		// 攻撃
	};

	Enemy() = default;
	~Enemy() = default;

	void Initialize(Model* models[], ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	void Move();

	void OnCollision(Collider* other) override;
	Vector3 GetCenterPosition() const override;

	void BehaviorInitialize();
	void BehaviorUpdate();

	void RootInitialize();
	void RootUpdate();

	void PreliminaryInitialize();
	void PreliminaryUpdate();

	void AttackInitialize();
	void AttackUpdate();

	// エネミーの攻撃Y座標を取得
	uint32_t GetAttackYIndex()const;
	Behavior GetBehavior()const;

	void SetPreliminaryYIndex(const uint32_t& yIndex);

private:

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	WorldTransform bodyTransform_;
	WorldTransform faceTransform_;
	WorldTransform leftEyeTransform_;
	WorldTransform rightEyeTransform_;

	// 3Dモデル
	Model* bodyModel_ = nullptr;
	Model* faceModel_ = nullptr;
	Model* eyeNormalModel_ = nullptr;
	Model* eyeHitModel_ = nullptr;

	const Vector3 leftEyeOffset_ = Vector3(-0.3f, 0.3f, -0.05f);
	const Vector3 rightEyeOffset_ = Vector3(0.5f, 0.3f, -0.05f);

	float hitTimer_ = 0.0f;
	const float kHitTime = 1.0f;

	// テクスチャハンドル
	uint32_t texturehandle_ = 0;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// 移動量
	Vector3 velocity_ = {};

	// 加速度
	const float acceleration_ = 0.0006f;

	// 最大速度
	const float kMaxSpeed_ = 0.12f;

	// 減速速度
	const float kSubtractSpeed_ = 0.03f;


	// 攻撃yインデックス
	uint32_t preliminaryYIndex_ = 0;
	uint32_t attackYIndex_ = 0;

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;

	// 次の振る舞いリスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;


	const uint32_t kRootTime_ = 180;
	const uint32_t kPreliminaryTime_ = 120;
	const uint32_t kAttackTime_ = 90;
	uint32_t behaviorTimer_ = 0;

};