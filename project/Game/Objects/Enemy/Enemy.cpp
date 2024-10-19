#include "Enemy.h"
#include <cassert>

#include "imgui.h"
#include "numbers"
#include "Input.h"
#include "Mymath.h"
#include "CollisionManager/CollisionTypeIdDef.h"

void Enemy::Initialize(Model* models[], ViewProjection* viewProjection, const Vector3& position) {
	assert(models);

	// ファイル名を指定してテクスチャを読み込む
	//textureHandle_ = textureHandel;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	// 顔
	faceTransform_.Initialize();
	faceTransform_.translation_ = position;
	faceTransform_.rotation_.y = worldTransform_.rotation_.y;

	// 左目
	leftEyeTransform_.Initialize();
	leftEyeTransform_.translation_ = position + leftEyeOffset_;
	leftEyeTransform_.rotation_.y = worldTransform_.rotation_.y;

	// 右目
	rightEyeTransform_.Initialize();
	rightEyeTransform_.translation_ = position + rightEyeOffset_;
	rightEyeTransform_.rotation_.y = worldTransform_.rotation_.y;


	// 3Dモデルの生成
	bodyModel_ = models[0];
	faceModel_ = models[1];
	eyeNormalModel_ = models[2];
	eyeHitModel_ = models[3];

	viewProjection_ = viewProjection;

	// コライダーの設定
	Collider::Initialize();
	// 半径を設定
	SetRadius(1.0f);

	SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));
}

void Enemy::Update() {
#ifdef _DEBUG
	ImGui::Begin("Enemy");
	switch (behavior_) {
	case Enemy::Behavior::kRoot:
		ImGui::Text("Behavior: root");
		break;
	case Enemy::Behavior::kPreliminary:
		ImGui::Text("Behavior: preliminary");
		break;
	case Enemy::Behavior::kAttack:
		ImGui::Text("Behavior: attack");
		break;
	}

	ImGui::End();
#endif // _DEBUG

	worldTransform_.rotation_.z -= velocity_.x;

	if (hitTimer_ >= 0.0f) {
		hitTimer_ -= 1.0f / 60.0f;
	}

	hitTimer_ = std::clamp(hitTimer_, -0.9f, 1.0f);

	// ビヘイビア初期化処理
	BehaviorInitialize();
	// ビヘイビア更新処理
	BehaviorUpdate();

	worldTransform_.UpdateMatrix();
	faceTransform_.UpdateMatrix();
	leftEyeTransform_.UpdateMatrix();
	rightEyeTransform_.UpdateMatrix();
}

void Enemy::Draw() {
	// 3Dモデルを描画
	bodyModel_->Draw(worldTransform_, *viewProjection_);
	faceModel_->Draw(faceTransform_, *viewProjection_);
	if (hitTimer_ <= 0.0f) {
		eyeNormalModel_->Draw(leftEyeTransform_, *viewProjection_);
		eyeNormalModel_->Draw(rightEyeTransform_, *viewProjection_);
	} else {
		eyeHitModel_->Draw(leftEyeTransform_, *viewProjection_);
		eyeHitModel_->Draw(rightEyeTransform_, *viewProjection_);
	}
}

void Enemy::Move() {
	// 移動量が最大速度を超えていない場合
	if (velocity_.x < kMaxSpeed_) {
		// 加速量を加速量に加算
		velocity_.x += acceleration_;
	}
	// 移動量を加算
	worldTransform_.translation_ += velocity_;
	faceTransform_.translation_ = worldTransform_.translation_;

	// 左目の位置からプレイヤーの方向を計算
	Vector3 leftEyePosition = worldTransform_.translation_ + leftEyeOffset_;
	Vector3 dirToPlayerFromLeftEye = playerPos_ - leftEyePosition;
	dirToPlayerFromLeftEye = Normalize(dirToPlayerFromLeftEye);
	dirToPlayerFromLeftEye *= 0.25f;

	// 右目の位置からプレイヤーの方向を計算
	Vector3 rightEyePosition = worldTransform_.translation_ + rightEyeOffset_;
	Vector3 dirToPlayerFromRightEye = playerPos_ - rightEyePosition;
	dirToPlayerFromRightEye = Normalize(dirToPlayerFromRightEye);
	dirToPlayerFromRightEye *= 0.25f;

	// 左目と右目の位置をプレイヤーの方向に調整
	leftEyeTransform_.translation_ = leftEyePosition + dirToPlayerFromLeftEye;
	rightEyeTransform_.translation_ = rightEyePosition + dirToPlayerFromRightEye;
}

void Enemy::OnCollision(Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	// 衝突相手が弾なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kBulletActive)) {
		velocity_.x -= kSubtractSpeed_;
		hitTimer_ += kHitTime_;
	}
}

Vector3 Enemy::GetCenterPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f,0.0f,0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

void Enemy::BehaviorInitialize() {
	// 振る舞い
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Enemy::Behavior::kRoot:
			RootInitialize();
			break;
		case Enemy::Behavior::kPreliminary:
			PreliminaryInitialize();
			break;
		case Enemy::Behavior::kAttack:
			AttackInitialize();
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

void Enemy::BehaviorUpdate() {
	switch (behavior_) {
	case Enemy::Behavior::kRoot:
		RootUpdate();
		break;
	case Enemy::Behavior::kPreliminary:
		PreliminaryUpdate();
		break;
	case Enemy::Behavior::kAttack:
		AttackUpdate();
		break;
	}
}

void Enemy::RootInitialize() {
	// タイマーをリセット
	behaviorTimer_ = 0;
}

void Enemy::RootUpdate() {
	// 移動処理
	Move();

	if (behaviorTimer_ < kRootTime_) {
		behaviorTimer_++;
	} else {
		behaviorRequest_ = Behavior::kPreliminary;
	}
}

void Enemy::PreliminaryInitialize() {
	// タイマーをリセット
	behaviorTimer_ = 0;
	// 攻撃するレーンのインデックスをセット
	attackYIndex_ = preliminaryYIndex_;
}

void Enemy::PreliminaryUpdate() {
	if (behaviorTimer_ < kPreliminaryTime_) {
		behaviorTimer_++;
	} else {
		behaviorRequest_ = Behavior::kAttack;
	}
}

void Enemy::AttackInitialize() {
	// タイマーをリセット
	behaviorTimer_ = 0;
}

void Enemy::AttackUpdate() {
	if (behaviorTimer_ < kAttackTime_) {
		behaviorTimer_++;
	} else {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Enemy::SetPlayerPos(const Vector3 newPlayerPos) {
	playerPos_ = newPlayerPos;
}

uint32_t Enemy::GetAttackYIndex()const {
	return attackYIndex_;
}

Enemy::Behavior Enemy::GetBehavior() const {
	return behavior_;
}

void Enemy::SetPreliminaryYIndex(const uint32_t& yIndex) {
	preliminaryYIndex_ = yIndex;
}
