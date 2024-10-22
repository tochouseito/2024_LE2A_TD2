#include "GravityArrow.h"

#include <utility>

#include "Mymath.h"

constexpr float parallaxFactor = 0.0125f;
constexpr float scrollSpeed = 0.0005f;

enum GravityArrowModels {
	UpArrow = 0,
	DownArrow,
	Bg
};

void GravityArrow::Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection) {
	models_ = models;

	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();

	viewProjection_ = viewProjection;
}

void GravityArrow::Update() {
	// uvスクロール
	uvX = viewProjection_->translation_.x * parallaxFactor;

	float parallaxY = viewProjection_->translation_.y * parallaxFactor;

	if (isUp_) {
		uvY += scrollSpeed;
	} else {
		uvY -= scrollSpeed;
	}

	float uvYTarget = uvY + parallaxY;

	models_[UpArrow]->GetMaterial()->GetMaterialData()->uvTransform.m[3][0] = uvX; // 横
	models_[UpArrow]->GetMaterial()->GetMaterialData()->uvTransform.m[3][1] = uvYTarget; // 縦
	models_[DownArrow]->GetMaterial()->GetMaterialData()->uvTransform.m[3][0] = uvX;
	models_[DownArrow]->GetMaterial()->GetMaterialData()->uvTransform.m[3][1] = uvYTarget;
	models_[Bg]->GetMaterial()->GetMaterialData()->uvTransform.m[3][0] = uvX * 20.0f;
	models_[Bg]->GetMaterial()->GetMaterialData()->uvTransform.m[3][1] = uvYTarget * 20.0f;

	worldTransform_.UpdateMatrix();
}

void GravityArrow::Draw() {
	models_[Bg]->Draw(worldTransform_, *viewProjection_);

	if (isUp_) {
		models_[UpArrow]->Draw(worldTransform_, *viewProjection_);
	} else {
		models_[DownArrow]->Draw(worldTransform_, *viewProjection_);
	}
}

void GravityArrow::SetPos(const Vector3 vector3) {
	worldTransform_.translation_ = vector3;
}
