#pragma once
#include <list>

#include "Collider.h"
#include "Model.h"

//-----------------------------------------------------------------------------
// 衝突マネージャ
//-----------------------------------------------------------------------------
class CollisionManager {
public:

	// 初期化
	void Initialize();

	// リセット
	void Reset();

	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	// すべての当たり判定をチェック
	void CheckAllCollision();
	// コライダーの追加
	void AddCollider(Collider* add);

	// ワールドトランスフォームの更新
	void UpdateWorldTransform() const;

	// 描画
	void Draw(const ViewProjection& viewProjection) const;

private:
#ifdef _DEBUG
	bool bShowCollision = true;
#else
	bool bShowCollision = false;
#endif

	// コライダー
	std::list<Collider*> colliders_;

	// デバッグ表示用モデル
	std::unique_ptr<Model> model_;
};

