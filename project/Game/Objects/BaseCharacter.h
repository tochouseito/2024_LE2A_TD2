#pragma once
#include "WorldTransform.h"

#include "CollisionManager/Collider.h"

class BaseCharacter : public Collider {
public:
	// 中心座標を取得
	virtual Vector3 GetCenterPosition() const override;

protected:
	WorldTransform worldTransform_;
};

