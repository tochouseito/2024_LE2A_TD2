#include "GravityArrow.h"

#include "TextureManager.h"

constexpr uint32_t kNumX = 3; // 並べる枚数
constexpr uint32_t kNumY = 3;

void GravityArrow::Initialize(Model* downModel, Model* upModel, ViewProjection* viewProjection, Vector3& pos) {
	upModel_ = upModel;
	downModel_ = downModel;

	viewProjection_ = viewProjection;

	for (uint32_t i = 0; i < kNumX; ++i) {
		for (uint32_t j = 0; j < kNumY; ++j) {
			WorldTransform worldTransform;
			worldTransform.Initialize();
			worldTransform.translation_ = Vector3(pos.x + static_cast<float>(i) * 20.0f, pos.y + static_cast<float>(j) * 10.0f, pos.z);
			worldTransform.Initialize();
			worldTransforms_.push_back(worldTransform);
		}
	}
	/*worldTransform_.Initialize();
	worldTransform_.translation_ = pos;*/
}

void GravityArrow::Update() const {
	for (WorldTransform worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
	upModel_->GetMaterial()->GetMaterialData()->color.r += 0.01f;
}

void GravityArrow::Draw() {
	for (WorldTransform worldTransform : worldTransforms_) {
		if (isUp_) {
			upModel_->Draw(worldTransform, *viewProjection_);
		} else {
			downModel_->Draw(worldTransform, *viewProjection_);
		}
	}
}
