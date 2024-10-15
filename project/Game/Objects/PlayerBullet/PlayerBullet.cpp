#include "PlayerBullet.h"
#include <cassert>

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

}

void PlayerBullet::HitInitialize() {

}

void PlayerBullet::HitUpdate() {

}
