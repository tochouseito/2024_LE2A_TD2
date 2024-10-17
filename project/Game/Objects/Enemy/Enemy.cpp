#include "Enemy.h"
#include <cassert>
#include "numbers"
#include "algorithm"
#include "imgui.h"
#include "Input.h"
#include "math/Easing.h"
#include "Mymath.h"
#include "CollisionManager/CollisionTypeIdDef.h"

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);

	// ファイル名を指定してテクスチャを読み込む
	//textureHandle_ = textureHandel;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	// 3Dモデルの生成
	model_ = model;

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

	// 攻撃処理
	Attack();

	// 行列更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}

void Enemy::Move() {
	// 移動量が最大速度を超えていない場合
	if (velocity_.x < kMaxSpeed_) {
		// 加速量を加速量に加算
		velocity_.x += acceleration_;
	}
	// 移動量を加算
	worldTransform_.translation_ += velocity_;
}

void Enemy::Attack() {}

void Enemy::OnCollision(Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	// 衝突相手が弾なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kBulletActive)) {
		velocity_.x -= kSubtractSpeed_;
	}
}

Vector3 Enemy::GetCenterPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f,0.0f,0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}
