#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"
class Blocks
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,ViewProjection* viewProjection,Vector3& pos);

	void Update();

	void Draw();
private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;
};

