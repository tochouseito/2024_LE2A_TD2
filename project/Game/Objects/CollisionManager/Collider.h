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
	virtual void OnCollision([[maybe_unused]] Collider* other) {}
	virtual Vector3 GetCenterPosition() const = 0;


public: // Getter And Setter
	// 半径を取得
	float GetRadius() const { return radius_; }
	void SetRadius(float newRadius);

	uint32_t GetTypeID() const { return typeID_; }
	void SetTypeID(const uint32_t typeID) { typeID_ = typeID; }

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	float radius_ = 1.5f;

	// 種別ID
	uint32_t typeID_ = 0u;
};
