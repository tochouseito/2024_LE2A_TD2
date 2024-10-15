#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"
#include "CollisionManager/Collider.h"

class Goal : public Collider {
public:
	Goal() = default;
	~Goal() = default;

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	void SetIsGoal(bool isGoal) { isGoal_ = isGoal; }

	bool GetIsGoal() const { return isGoal_; }

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

	bool isGoal_ = false;

};
