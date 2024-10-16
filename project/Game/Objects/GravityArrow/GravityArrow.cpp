#include "GravityArrow.h"

#include "Mymath.h"
#include "TextureManager.h"

void GravityArrow::Initialize(Model* downModel, Model* upModel, ViewProjection* viewProjection, Vector3& pos) {
	upModel_ = upModel;
	downModel_ = downModel;

	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();

	viewProjection_ = viewProjection;
}

void GravityArrow::Update() {
	worldTransform_.UpdateMatrix();

	upModel_->GetMaterial()->GetMaterialData()->uvTransform.m[3][1] += 0.0025f; // 横
	upModel_->GetMaterial()->GetMaterialData()->uvTransform.m[3][0] = viewProjection_->translation_.x * 0.0125f; // 縦
}

void GravityArrow::Draw() {
	if (isUp_) {
		upModel_->Draw(worldTransform_, *viewProjection_);
	} else {
		downModel_->Draw(worldTransform_, *viewProjection_);
	}
}

void GravityArrow::SetPos(Vector3 vector3) {
	worldTransform_.translation_ = vector3;
}
