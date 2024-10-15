#include "Needle.h"

#include "Mymath.h"
#include "CollisionManager/CollisionTypeIdDef.h"

void Needle::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos) {
	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.scale_ *= 0.5f;

	// コライダーの設定
	Collider::Initialize();
	// 半径を設定
	SetRadius(0.25f);

	SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kNeedle));
}

void Needle::Update() {
	worldTransform_.UpdateMatrix();
}

void Needle::Draw() {
	model_->Draw(worldTransform_, *viewProjection_);
}

void Needle::OnCollision(Collider* other) {
	Collider::OnCollision(other);
}

Vector3 Needle::GetCenterPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f,0.0f,0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}
