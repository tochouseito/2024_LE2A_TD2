#pragma once
#include "CollisionManager/Collider.h"

enum NeedleDir {
	kUp = 0,
	kDown,
};

class Needle : public Collider {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos, NeedleDir direction);
	void Update();
	void Draw();

	void OnCollision(Collider* other) override;

	Vector3 GetCenterPosition() const override;

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 3Dモデル
	Model* model_ = nullptr;

	// 方向
	NeedleDir dir_ = kUp;

	// テクスチャハンドル
	uint32_t texturehandle_ = 0;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
};

