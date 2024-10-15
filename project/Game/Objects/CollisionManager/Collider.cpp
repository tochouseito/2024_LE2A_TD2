#include "Collider.h"

void Collider::Initialize() {
	worldTransform_.Initialize();
}

void Collider::UpdateWorldTransform() {
	// ワールド座標をワールドトランスフォームに適用
	worldTransform_.translation_ = GetCenterPosition();
	worldTransform_.UpdateMatrix();
}

void Collider::Draw(Model* model, const ViewProjection& viewProjection) const {
	WorldTransform transform = worldTransform_;
	transform.scale_ = transform.scale_ * radius_;
	transform.UpdateMatrix();
	model->Draw(transform, viewProjection);
}

void Collider::SetRadius(const float newRadius) {
	radius_ = newRadius;
}
