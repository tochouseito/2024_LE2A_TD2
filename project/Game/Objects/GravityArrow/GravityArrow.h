#pragma once
#include "Blocks/Blocks.h"

class GravityArrow {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* upModel, Model* downModel, ViewProjection* viewProjection, Vector3& pos);

	void Update() const;

	void Draw();

	void SetGravityDir(const bool up) {
		isUp_ = up;
	}

	/*void SetPos(const Vector3 newPos) {
		worldTransform_.translation_ = newPos;
	}

	void SetScale(const Vector3 newScale) {
		worldTransform_.scale_ = newScale;
	}

	void SetRot(const Vector3 newRot) {
		worldTransform_.rotation_ = newRot;
	}*/


private:
	//WorldTransform worldTransform_;
	std::vector<WorldTransform> worldTransforms_;
	ViewProjection* viewProjection_ = nullptr;

	Model* upModel_ = nullptr;
	Model* downModel_ = nullptr;

	bool isUp_ = false;

	std::string upTextureHandle;
};


