#pragma once

#include <optional>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "CollisionManager/Collider.h"


class PlayerBullet : public Collider {
public:
	enum class Behavior {
		kRoot,	 // 通常行動
		kAttack, // 攻撃行動
		kHit,	 // 直撃時
	};
public:
	PlayerBullet() = default;
	~PlayerBullet() = default;

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	bool GetIsAlive()const;

	void OnCollision(Collider* other) override;
	Vector3 GetCenterPosition() const override;


private:

	void BehaviorInitialize();
	void BehaviorUpdate();

	void RootInitialize();
	void RootUpdate();

	void AttackInitialize();
	void AttackUpdate();

	void HitInitialize();
	void HitUpdate();

private:
	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 3Dモデル
	Model* model_ = nullptr;

	// 死亡アニメーションタイマー
	uint32_t deadAnimationTimer_ = 0;

	// 生存フラグ
	bool isAlive_ = true;

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;

	float velocity_ = 0.0f;

	// 弾の加速量
	const float kAccelerate_ = 0.2f;

	// 次の振る舞いリスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
};