#pragma once
#include "Blocks/Blocks.h"

class GravityArrow {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection);
	void Update();

	void Draw();

	void SetGravityDir(const bool up) {
		isUp_ = up;
	}

	void SetPos(Vector3 vector3);

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;


	std::vector<Model*> models_;

	float uvY = 0.0f;
	float uvX = 0.0f;

	bool isUp_ = false;

	std::string upTextureHandle;
};
