#include "Blocks.h"

void Blocks::Initialize(Model* model, ViewProjection* viewProjection, Vector3& pos)
{
	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.scale_ *= 0.5f;
}

void Blocks::Update()
{
	worldTransform_.UpdateMatrix();
}

void Blocks::Draw()
{
	model_->Draw(worldTransform_, *viewProjection_);
}
