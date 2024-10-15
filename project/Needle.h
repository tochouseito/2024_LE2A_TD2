#pragma once
#include "Blocks/Blocks.h"
#include "CollisionManager/Collider.h"

class Needle : public Collider {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos);
	void Update();
	void Draw();

	void OnCollision(Collider* other) override;

	Vector3 GetCenterPosition() const override;

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 3Dモデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t texturehandle_ = 0;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
};

