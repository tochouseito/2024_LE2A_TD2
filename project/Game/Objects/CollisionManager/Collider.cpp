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
	ObjectColor color;
	color.Initialize();
	color.SetColor({ 1.0f,0.0f,0.0f,0.5f });
	model->Draw(transform, viewProjection, "", &color);
}

void Collider::SetRadius(const float newRadius) {
	radius_ = newRadius;
}
