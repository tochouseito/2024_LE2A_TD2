#include "Enemy.h"
#include <cassert>
#include "numbers"
#include "Input.h"
#include "math/Easing.h"
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

	// 移動処理
	Move();

	worldTransform_.rotation_.z -= velocity_.x;

	// 攻撃処理
	Attack();

	if (hitTimer_ >= 0.0f) {
		hitTimer_ -= 1.0f / 60.0f;
	}

	hitTimer_ = std::clamp(hitTimer_, -0.9f, 1.0f);

	// 行列更新
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

	// TODO : プレイヤーに向ける
	leftEyeTransform_.translation_ = worldTransform_.translation_ + leftEyeOffset_;
	rightEyeTransform_.translation_ = worldTransform_.translation_ + rightEyeOffset_;
}

void Enemy::Attack() {

}

void Enemy::OnCollision(Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	// 衝突相手が弾なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kBulletActive)) {
		velocity_.x -= kSubtractSpeed_;
		hitTimer_ += kHitTime;
	}
}

Vector3 Enemy::GetCenterPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f,0.0f,0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}
