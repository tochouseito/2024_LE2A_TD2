#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"
class Blocks {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(Model* model, ViewProjection* viewProjection, Vector3& pos);

	virtual void Update();

	virtual void Draw();
protected:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;
};

