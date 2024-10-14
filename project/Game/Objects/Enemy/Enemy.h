#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"

class Enemy {
public:
	Enemy() = default;
	~Enemy() = default;

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

private:

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 3Dモデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t texturehandle_ = 0;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// 移動量
	Vector3 velocity_ = {};

	// 速度
	float speed_ = 0.125f;

};