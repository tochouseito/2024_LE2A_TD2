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

	// ビヘイビア初期化処理
	BehaviorInitialize();
	// ビヘイビア更新処理
	BehaviorUpdate();

	// 行列の更新
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

uint32_t Enemy::GetAttackYIndex()const {
	return attackYIndex_;
}

Enemy::Behavior Enemy::GetBehavior() const {
	return behavior_;
}

void Enemy::SetPreliminaryYIndex(const uint32_t& yIndex) {
	preliminaryYIndex_ = yIndex;
}
