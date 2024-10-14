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


	// 行列更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}
