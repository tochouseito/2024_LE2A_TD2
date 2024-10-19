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

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
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

private:

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 3Dモデル
	Model* model_ = nullptr;

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
	uint32_t attackYIndex_ = 0;

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;

	// 次の振る舞いリスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;


};