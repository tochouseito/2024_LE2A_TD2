#define NOMINMAX
#include "Player.h"

#include <assert.h>

#include "algorithm"
#include "ConvertString.h"
#include "imgui.h"
#include "Input.h"
#include "Mymath.h"
#include "numbers"
#include"DirectXCommon.h"

#include "CollisionManager/CollisionTypeIdDef.h"

#include"Game/MapChipField/MapChipField.h"

Player::Player() = default;

Player::~Player() = default;

void Player::Initialize(const std::vector<Model*>& models,const std::vector<Model::Animation*>& animas, ViewProjection* viewProjection, const Vector3& position) {
	//assert(model);

	// ファイル名を指定してテクスチャを読み込む
	//textureHandle_ = textureHandel;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.scale_ *= 0.5f;
	// 3Dモデルの生成
	models_ = models;
	animas_ = animas;
	for (int i = 0; i < Animation::kCount; i++) {
		skeletons_.push_back(models_[i]->CreateSkeleton(models_[i]->GetModelData()->rootNode));
		for (std::string& name : models_[i]->GetModelData()->names) {
			models_[i]->SetSkinCluster(models_[i]->CreateSkinCluster(DirectXCommon::GetInstance()->GetDevice(), skeletons_[i], models_[i]->GetModelData()->object[name]));
		}
	}
	viewProjection_ = viewProjection;

	// コライダーの設定
	Collider::Initialize();
	// 半径を設定
	SetRadius(0.25f);

	SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));
}

float targetRotZ = 0.0f;

void Player::Update() {
	// 衝突判定を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動入力
	// 設置状態

	CharMove();
	// 移動量に速度の値をコピー
	collisionMapInfo.movement = velocity_;
	// 移動量を加味して衝突判定する
	MapCollision(collisionMapInfo);
	// 判定結果を反映して移動させる
	HitMove(collisionMapInfo);
	// 天井に接触している場合の処理
	HitCeiling(collisionMapInfo);
	// 壁に接触している場合の処理
	HitWall(collisionMapInfo);
	// 設置状態の切り替え
	OnGround(collisionMapInfo);
	// 旋回制御
	TurnControl();

	if (isGravityInvert) {
		targetRotZ = std::numbers::pi_v<float>;
		// Y軸の回転を反転
		worldTransform_.rotation_.y = std::lerp(worldTransform_.rotation_.y,
			(lrDirection_ == LRDirection::kRight) ?
			std::numbers::pi_v<float> *3.0f / 2.0f : std::numbers::pi_v<float> / 2.0f,
			0.1f); // 補間係数を調整
	} else {
		targetRotZ = 0.0f;
		// 通常のY軸の回転
		worldTransform_.rotation_.y = std::lerp(worldTransform_.rotation_.y,
			(lrDirection_ == LRDirection::kRight) ?
			std::numbers::pi_v<float> / 2.0f : std::numbers::pi_v<float> *3.0f / 2.0f,
			0.1f); // 補間係数を調整
	}

	if (isHitNeedle) {
		slownessTimer -= 1.0f / 60.0f; // TODO : deltaTimeにする
	}

	if (slownessTimer <= 0.0f) {
		isHitNeedle = false;
	}

	// 徐々に回転
	worldTransform_.rotation_.z = std::lerp(worldTransform_.rotation_.z, targetRotZ, 1.0f / 60.0f * 16.0f); // TODO : deltaTimeにする

#ifdef _DEBUG
	ImGui::Begin("Player");

	// ジャンプと重力反転の状態を並べて表示
	ImGui::Checkbox("isJumping", &isJumping);
	ImGui::SameLine();
	ImGui::Checkbox("isGravityInvert", &isGravityInvert);

	ImGui::Separator();

	// 速度やヒット状態のスライダーとチェックボックスをわかりやすく並べる
	ImGui::DragFloat("NeedleSlowness", &kNeedleSlowness);
	ImGui::SameLine();
	ImGui::Checkbox("IsHitNeedle", &isHitNeedle);

	// スローモーションタイマーの進行状況をプログレスバーで表示
	ImGui::Text("Slowness Timer :");
	ImGui::ProgressBar(slownessTimer / kMaxSlownessTime, ImVec2(0.0f, 0.0f));

	// スライダーでスローモーションの時間を調整
	ImGui::SliderFloat("kMaxSlownessTime", &kMaxSlownessTime, 0.0f, 10.0f, "%.2f");

	ImGui::Separator();

	// Parameters をグループ化して見やすくする
	if (ImGui::CollapsingHeader("Player Parameters", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("Movement Settings:");

		// Movement Parameters Group
		ImGui::BeginGroup();
		ImGui::DragFloat("LimitRunSpeed", &kLimitRunSpeed, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("Acceleration", &kAcceleration, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("Attenuation", &kAttenuation, 0.01f, 0.0f, 100.0f);
		ImGui::EndGroup();

		ImGui::Separator(); // セクションの区切り

		ImGui::Text("Jump and Gravity Settings:");

		// Jump & Gravity Parameters Group
		ImGui::BeginGroup();
		ImGui::DragFloat("JumpAcceleration", &kJumpAcceleration, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("GravityAcceleration", &kGravityAcceleration, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("LimitFallSpeed", &kLimitFallSpeed, 0.01f, 0.0f, 100.0f);
		ImGui::EndGroup();

		ImGui::Separator();

		ImGui::Text("Collision Settings:");

		// Collision Parameters Group
		ImGui::BeginGroup();
		ImGui::DragFloat("Width", &kWidth, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("Height", &kHeight, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("Blank", &kBlank, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("CorrectBlank", &kCorrectBlank, 0.01f, 0.0f, 100.0f);
		ImGui::EndGroup();
	}

	ImGui::End();
#endif

	// 移動
	//worldTransform_.translation_.x += velocity_.x;
	//worldTransform_.translation_.y += velocity_.y;
	//worldTransform_.translation_.z += velocity_.z;

	// 行列を定数バッファに転送
	//worldTransform_.TransferMatrix();

	animationTime += 1.0f / 60.0f;// 時刻を進める。1/60で固定してあるが、計測した時間を使って可変フレーム対応するほうが望ましい
	animationTime = std::fmod(animationTime, animas_[nowAnima_]->duration);

	models_[nowAnima_]->ApplyAnimation(skeletons_[nowAnima_], animas_[nowAnima_], animationTime);
	models_[nowAnima_]->SkeletonUpdata(skeletons_[nowAnima_]);
	models_[nowAnima_]->SkinClusterUpdata(models_[nowAnima_]->GetSkinCluster(), skeletons_[nowAnima_]);

	worldTransform_.UpdateMatrix();
}

void Player::Draw() {
	// 3Dモデルを描画
	/*if (isAlive_) {
		models_[nowAnima_]->Draw(worldTransform_, *viewProjection_);
	}*/
	models_[nowAnima_]->ApplyCS();
	models_[nowAnima_]->DrawCS(worldTransform_, *viewProjection_, "none");
}

// 移動入力
void Player::CharMove() {
	XINPUT_STATE controllerState;
	bool controllerConnected = Input::GetInstance()->GetJoystickState(0, controllerState);

	float stickX = 0.0f;
	if (controllerConnected) {
		stickX = static_cast<float>(controllerState.Gamepad.sThumbLX) / 32767.0f;  // 正規化したスティックのX入力（-1.0 ~ 1.0）
	}

	if (onGround_ == true) {
		// 左右移動入力
		bool keyboardMoveLeft = Input::GetInstance()->PushKey(DIK_A);
		bool keyboardMoveRight = Input::GetInstance()->PushKey(DIK_D);
		bool controllerMoveLeft = stickX < -0.2f;
		bool controllerMoveRight = stickX > 0.2f;

		if (keyboardMoveLeft || keyboardMoveRight || controllerMoveLeft || controllerMoveRight) {
			// 左右処理
			Vector3 acceleration = {};
			if (keyboardMoveRight || controllerMoveRight) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					// 旋回開始時の角度を設定する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
			} else if (keyboardMoveLeft || controllerMoveLeft) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					// 旋回開始時の角度を設定する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
			}
			// 加速／減速
			velocity_.x += acceleration.x;
			velocity_.y += acceleration.y;
			velocity_.z += acceleration.z;
			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		// 針に接触している場合は速度を下げる
		if (isHitNeedle) {
			velocity_.x *= kNeedleSlowness;
		}

		// ジャンプ入力
		bool keyboardJump = Input::GetInstance()->TriggerKey(DIK_SPACE);
		bool controllerJump = controllerConnected && Input::GetInstance()->TriggerControllerButton(0, XINPUT_GAMEPAD_A);
		if (onGround_ && (keyboardJump || controllerJump)) {
			// ジャンプ初速
			velocity_ += Vector3(0, isGravityInvert ? -kJumpAcceleration : kJumpAcceleration, 0);
			isJumping = true;
		}
	}

	// 重力反転入力
	bool keyboardInvertGravity = Input::GetInstance()->TriggerKey(DIK_SPACE);
	bool controllerInvertGravity = controllerConnected && Input::GetInstance()->TriggerControllerButton(0, XINPUT_GAMEPAD_A);
	if (!onGround_ && (keyboardInvertGravity || controllerInvertGravity)) {
		isGravityInvert = !isGravityInvert;
	}
}
void Player::OnGround(const CollisionMapInfo& info) {
	// 着地フラグ
	if (info.landingCollision_) {
		// 着地状態に切り替える（落下を止める）
		onGround_ = true;
		isJumping = false; // ジャンプ状態を解除
		// 着地時にX速度を減衰
		velocity_.x *= (1.0f - kAttenuationLanding);
		// Y速度をゼロにする
		velocity_.y = 0.0f;
	}

	// ジャンプ開始
	if (isGravityInvert) {
		if (velocity_.y > 0.0f) {
			// 空中状態に以降
			onGround_ = false;
		}
	} else {
		if (velocity_.y < 0.0f) {
			// 空中状態に以降
			onGround_ = false;
		}
	}

	// 移動後４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.movement, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;

	// 通常重力
	if (!isGravityInvert) {
		// 左下点の判定
		MapChipField::IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kCorrectBlank, 0));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xInDex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
		// 右下点の判定
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kCorrectBlank, 0));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xInDex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
	} else { // 反転重力
		// 左上点の判定
		MapChipField::IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop] + Vector3(0, kCorrectBlank, 0));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xInDex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
		// 右上点の判定
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop] + Vector3(0, kCorrectBlank, 0));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xInDex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
	}

	// 落下開始
	if (!hit) {
		// 空中状態に切り替える
		onGround_ = false;
	}

	if (!onGround_) {// 空中状態
		// 落下速度
		velocity_.x += 0.0f;
		velocity_.y += (isGravityInvert ? kGravityAcceleration : -kGravityAcceleration);
		velocity_.z += 0.0f;
		// 落下速度制御
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

		// 着地フラグ
		landing = false;

		// 地面との当たり判定
		// 下降中？
		if (velocity_.y < 0) {
			// Y座標が地面以下になったら着地
			if (worldTransform_.translation_.y <= 1.0f) {
				landing = true;
			}
		}
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuationLanding);
			// 下方向の速度をリセット
			velocity_.y = 0.0f;
			// 設置状態に移行
			onGround_ = true;
		}
	}
	if (isGravityInvert) {
		if (!preLand && hit) {
			land = true;
		} else
		{
			land = false;
		}
		preLand = hit;
	} else
	{
		if (!preLand && onGround_) {
			land = true;
		} else
		{
			land = false;
		}
		preLand = onGround_;
	}
}

void Player::TurnControl() {
	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 0.015f;
		float t = 1.0f - (turnTimer_ / kTimeTurn);
		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> *3.0f / 2.0f
		};
		// 重力反転時の角度調整
		if (isGravityInvert) {
			destinationRotationYTable[0] = std::numbers::pi_v<float> *3.0f / 2.0f;
			destinationRotationYTable[1] = std::numbers::pi_v<float> / 2.0f;
		}
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度をLerpで補間する
		worldTransform_.rotation_.y = std::lerp(turnFirstRotationY_, destinationRotationY, t);
	}
}

// マップ衝突判定
void Player::MapCollision(CollisionMapInfo& info) {
	// 各方向衝突判定
	// 上方向
	TopCollision(info);
	// 下方向
	BottomCollision(info);
	// 右方向
	RightCollision(info);
	// 左方向
	LeftCollision(info);
}

Vector3 Player::CornerPosition(const Vector3& center, const Corner corner) const {
	Vector3 offsetTable[kNumCorner] = {
		{+kWidth / 2.0f, -kHeight / 2.0f, 0},//kRightBottom
		{-kWidth / 2.0f, -kHeight / 2.0f, 0},//kLeftBottom
		{+kWidth / 2.0f, +kHeight / 2.0f, 0},//kRightTop
		{-kWidth / 2.0f, +kHeight / 2.0f, 0},//kLeftTop
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

Vector3 Player::GetWorldPosition() const {
	/*ワールド座標を入れる変数*/
	Vector3 worldPos;
	/*ワールド行列の平行移動成分を取得*/
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Player::GetAABB() const {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {
		worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f };
	aabb.max = {
		worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f };
	return aabb;
}

Vector3 Player::GetCenterPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f,0.0f,0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

void Player::OnCollision(Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	// 衝突相手がエネミーなら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		isAlive_ = false;
	}
	// 衝突相手が針なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kNeedle)) {
		if (!isHitNeedle) {
			isHitNeedle = true;
			slownessTimer = kMaxSlownessTime;
		}
	}

}

void Player::SetLandingTexture(const std::string& handle) {
	landingTexture_ = handle;
}

void Player::SetIsAllive(const bool& isAllive) {
	isAlive_ = isAllive;
}

//void Player::OnCollision(const Enemy* enemy) {
//	(void)enemy;
//	isAlive = false;
//	///*ジャンプ開始*/
//	//velocity_ += Vector3(0.0f, kJumpAcceleration, 0.0f);
//}
// 各方向衝突判定
// 上方向
void Player::TopCollision(CollisionMapInfo& info) {
	// 上昇アリ？
	if (info.movement.y <= 0) {
		return;
	}
	// 移動後４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + Vector3(0, info.movement.y, 0), static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xInDex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xInDex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, info.movement.y, 0) + Vector3(0, kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xInDex, indexSet.yIndex);
		info.movement.y = std::max(0.0f, (rect.bottom - worldTransform_.translation_.y) - ((kHeight / 2.0f) + kBlank));
		// 天井に当たったことを記録する
		info.ceilingCollision_ = true;

		info.landingCollision_ = true;
	}
}
// 下方向
void Player::BottomCollision(CollisionMapInfo& info) {
	// 下降アリ？
	if (info.movement.y >= 0) {
		return;
	}
	// 移動後４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + Vector3(0, info.movement.y, 0), static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左下点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xInDex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xInDex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, info.movement.y, 0) - Vector3(0, kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xInDex, indexSet.yIndex);
		info.movement.y = std::min(0.0f, (rect.top - worldTransform_.translation_.y) + ((kHeight / 2.0f) + kBlank));
		// 地面に当たったことを記録する
		info.landingCollision_ = true;
	}
}
// 右方向
void Player::RightCollision(CollisionMapInfo& info) {
	// 右移動あり？
	if (info.movement.x <= 0.0f) {
		return;
	}
	// 移動後４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.movement, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 右の当たり判定を行う
	bool hit = false;
	// 右上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xInDex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xInDex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(info.movement.x, 0, 0) + Vector3(kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xInDex, indexSet.yIndex);
		info.movement.x = std::max(0.0f, (rect.left - worldTransform_.translation_.x) - ((kWidth / 2.0f) + kBlank));
		// 右壁に当たったことを記録する
		info.wallCollision_ = true;
	}
}
// 左方向
void Player::LeftCollision(CollisionMapInfo& info) {
	// 左移動あり？
	if (info.movement.x >= 0.0f) {
		return;
	}
	// 移動後４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + Vector3(info.movement.x, 0, 0), static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 左の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xInDex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xInDex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(info.movement.x, 0, 0) - Vector3(kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xInDex, indexSet.yIndex);
		info.movement.x = std::min(0.0f, (rect.right - worldTransform_.translation_.x) + ((kWidth / 2.0f) + kBlank));
		// 地面に当たったことを記録する
		info.wallCollision_ = true;
	}
}
// 移動
void Player::HitMove(const CollisionMapInfo& info) {
	worldTransform_.translation_ += info.movement;
}
void Player::HitCeiling(const CollisionMapInfo& info) {
	// 天井に当たった
	if (info.ceilingCollision_) {
		//DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}
void Player::HitWall(const CollisionMapInfo& info) {
	// 壁接触による減衰
	if (info.wallCollision_) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}
