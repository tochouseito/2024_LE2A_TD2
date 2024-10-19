#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"
#include "CollisionManager/Collider.h"

class Enemy : public Collider {
public:
	Enemy() = default;
	~Enemy() = default;

	void Initialize(Model* models[], ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	void Move();
	void Attack();

	void OnCollision(Collider* other) override;
	Vector3 GetCenterPosition() const override;

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
	uint32_t attackYIndex_ = 0;

};