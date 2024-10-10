#pragma once
#include "Blocks/Blocks.h"

class GravityArrow {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, Vector3& pos);

	void Update();

	void Draw();

	void SetPos(const Vector3 newPos) {
		worldTransform_.translation_ = newPos;
	}

	void SetScale(const Vector3 newScale) {
		worldTransform_.scale_ = newScale;
	}

	void SetRot(const Vector3 newRot) {
		worldTransform_.rotation_ = newRot;
	}

	void SetUVPos(const Vector2 newUVPos) {
		model_->SetUVPos(newUVPos);
	}

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;
};


