#include "GravityArrow.h"

#include "Mymath.h"

constexpr float parallaxFactor = 0.025f;
constexpr float scrollSpeed = 0.0005f;

void GravityArrow::Initialize(Model* upModel, Model* downModel, ViewProjection* viewProjection) {
	upModel_ = upModel;
	downModel_ = downModel;

	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();

	viewProjection_ = viewProjection;
}

void GravityArrow::Update() {
	// uvスクロール
	uvX = viewProjection_->translation_.x * parallaxFactor;

	if (isUp_) {
		uvY += scrollSpeed;
	} else {
		uvY -= scrollSpeed;
	}

	upModel_->GetMaterial()->GetMaterialData()->uvTransform.m[3][0] = uvX; // 横
	upModel_->GetMaterial()->GetMaterialData()->uvTransform.m[3][1] = uvY; // 縦
	downModel_->GetMaterial()->GetMaterialData()->uvTransform.m[3][0] = uvX;
	downModel_->GetMaterial()->GetMaterialData()->uvTransform.m[3][1] = uvY;

	worldTransform_.UpdateMatrix();
}

void GravityArrow::Draw() {
	if (isUp_) {
		upModel_->Draw(worldTransform_, *viewProjection_);
	} else {
		downModel_->Draw(worldTransform_, *viewProjection_);
	}
}

void GravityArrow::SetPos(const Vector3 vector3) {
	worldTransform_.translation_ = vector3;
}
