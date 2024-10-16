#include "PlayerBullet.h"
#include <cassert>

#include "Mymath.h"
#include "CollisionManager/CollisionTypeIdDef.h"

#include "imgui.h"

void PlayerBullet::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);

	// ファイル名を指定してテクスチャを読み込む
	//textureHandle_ = textureHandel;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.scale_ *= 0.5f;
	// 3Dモデルの生成
	model_ = model;

	viewProjection_ = viewProjection;

	// コライダーの設定
	Collider::Initialize();
	// 半径を設定
	SetRadius(0.25f);

	SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kBullet));
}

void PlayerBullet::Update() {
	// ビヘイビア初期化処理
	BehaviorInitialize();
	// ビヘイビア更新処理
	BehaviorUpdate();

	// 行列の更新
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw() {
	model_->Draw(worldTransform_, *viewProjection_);
}

void PlayerBullet::OnCollision(Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 通常状態でかつ衝突相手がプレイヤーなら
	if (behavior_ == Behavior::kRoot) {
		if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer)) {
			behaviorRequest_ = Behavior::kAttack;
		}
	}
	// 攻撃状態かつ、衝突相手がエネミーなら
	else if (behavior_ == Behavior::kAttack) {
		if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
			behaviorRequest_ = Behavior::kHit;
		}
	}
}

Vector3 PlayerBullet::GetCenterPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f,0.0f,0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

void PlayerBullet::BehaviorInitialize() {
	// 振る舞い
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
			case PlayerBullet::Behavior::kRoot:
				RootInitialize();
				break;
			case PlayerBullet::Behavior::kAttack:
				AttackInitialize();
				break;
			case PlayerBullet::Behavior::kHit:
				HitInitialize();
				break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

void PlayerBullet::BehaviorUpdate() {
	switch (behavior_) {
		case PlayerBullet::Behavior::kRoot:
			RootUpdate();
			break;
		case PlayerBullet::Behavior::kAttack:
			AttackUpdate();
			break;
		case PlayerBullet::Behavior::kHit:
			HitUpdate();
			break;
	}
}

void PlayerBullet::RootInitialize() {

}

void PlayerBullet::RootUpdate() {

}

void PlayerBullet::AttackInitialize() {

}

void PlayerBullet::AttackUpdate() {
	
	worldTransform_.translation_.x -= 0.1f;
}

void PlayerBullet::HitInitialize() {

}

void PlayerBullet::HitUpdate() {

}
