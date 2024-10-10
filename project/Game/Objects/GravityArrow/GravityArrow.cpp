#include "GravityArrow.h"

void GravityArrow::Initialize(Model* model, ViewProjection* viewProjection, Vector3& pos) {
	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.scale_ *= 0.5f;
}

void GravityArrow::Update() {
	worldTransform_.UpdateMatrix();
	model_->GetMaterial()->GetMaterialData()->color.r += 0.01f;
}

void GravityArrow::Draw() {
	model_->Draw(worldTransform_, *viewProjection_);
}
