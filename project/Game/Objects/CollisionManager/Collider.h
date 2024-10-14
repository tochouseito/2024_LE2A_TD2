#pragma once
#include "Model.h"
#include "Vector3.h"
#include "WorldTransform.h"

class Collider {
public:
	virtual ~Collider() = default;

	void Initialize();
	// ワールドトランスフォームの更新
	void UpdateWorldTransform();
	// 描画
	void Draw(Model* model, const ViewProjection& viewProjection) const;

	// 衝突時に呼ばれる関数
	virtual void OnCollision() {}
	virtual Vector3 GetCenterPosition() const = 0;


public: // Getter And Setter
	// 半径を取得
	float GetRadius() const { return radius_; }
	void SetRadius(float newRadius);

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	float radius_ = 1.5f;
};
