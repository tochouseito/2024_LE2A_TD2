#include "Enemy.h"
#include <cassert>
#include "numbers"
#include "algorithm"
#include "imgui.h"
#include "Input.h"
#include "math/Easing.h"


void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
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
