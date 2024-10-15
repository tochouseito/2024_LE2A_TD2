#include "CollisionManager.h"

#include "imgui.h"
#include "Mymath.h"

void CollisionManager::Initialize() {
	model_.reset(Model::CreateSphere());
}

void CollisionManager::Reset() {
	// リストを空っぽにする
	colliders_.clear();
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 posA = colliderA->GetCenterPosition();
	// コライダーBの座標を取得
	Vector3 posB = colliderB->GetCenterPosition();
	// 座標の差分ベクトル
	Vector3 subtract = posB - posA;
	// 座標Aと座標Bの距離を求める
	float distance = Length(subtract);
	// 球と球の交差判定
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		// コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision(colliderB);
		// コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision(colliderA);
	}
}

void CollisionManager::CheckAllCollision() {
#ifdef _DEBUG
	ImGui::Begin("Collision Manager");
	ImGui::Checkbox("ShowCollision", &bShowCollision);
	ImGui::End();
#endif

	// リスト内のペアを総当り
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す ( 重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		++itrB;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::AddCollider(Collider* add) {
	colliders_.push_back(add);
}

void CollisionManager::UpdateWorldTransform() const {
	// すべてのコライダーについて
	for (Collider* collider : colliders_) {
		// 更新
		collider->UpdateWorldTransform();
	}
}

void CollisionManager::Draw(const ViewProjection& viewProjection) const {
	if (bShowCollision) {
		for (Collider* collider : colliders_) {
			collider->Draw(model_.get(), viewProjection);
		}
	}
}
