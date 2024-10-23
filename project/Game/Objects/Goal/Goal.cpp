#include "Goal.h"
#include <cassert>
#include "numbers"
#include "Input.h"
#include "Mymath.h"
#include "CollisionManager/CollisionTypeIdDef.h"


void Goal::Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection, const Vector3& position) {
	// ファイル名を指定してテクスチャを読み込む
	//textureHandle_ = textureHandel;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.scale_ *= 0.5f;
	// 3Dモデルの生成
	models_ = models;

	models_[1]->GetMaterial()->GetMaterialData()->enableLighting = false;

	viewProjection_ = viewProjection;

	// コライダーの設定
	Collider::Initialize();
	// 半径を設定
	SetRadius(1.0f);

	SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));
}

void Goal::Update() {
	// 行列更新
	worldTransform_.UpdateMatrix();
}

void Goal::Draw() {
	// 3Dモデルを描画
	for (const auto value : models_) {
		value->Draw(worldTransform_, *viewProjection_);
	}
}

void Goal::OnCollision(Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	// 衝突相手が針なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer)) {
		SetIsGoal(true);
	}
}

Vector3 Goal::GetCenterPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f,0.0f,0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}
