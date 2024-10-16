#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"
#include "CollisionManager/Collider.h"

class Enemy: public Collider {
public:
	Enemy() = default;
	~Enemy() = default;

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	void Move();
	void Attack();

	void OnCollision(Collider* other) override;
	Vector3 GetCenterPosition() const override;

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
	const float acceleration_ = 0.0002f;

	// 最大速度
	const float kMaxSpeed_ = 0.12f;

};