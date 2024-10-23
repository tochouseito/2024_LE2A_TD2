#pragma once
#include "BaseCharacter.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include<vector>

class MapChipField;

enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上

	kNumCorner // 要素数
};

enum Animation {
	idle,
	Jump,
	JumpLoop,
	JumpStart,
	Land,
	Run,
	kCount,
};

class Player : public BaseCharacter {
public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models,const std::vector<Model::Animation*>& animas, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	struct CollisionMapInfo {
		bool ceilingCollision_ = false;
		bool landingCollision_ = false;
		bool wallCollision_ = false;

		Vector3 movement;
	};
	// 設置状態フラグ
	bool onGround_ = true;

	// 移動入力
	void CharMove();

	// 移動量を加味して衝突判定する
	void MapCollision(CollisionMapInfo& info);
	// 判定結果を反映して移動させる
	void HitMove(const CollisionMapInfo& info);
	// 天井に接触している場合の処理
	void HitCeiling(const CollisionMapInfo& info);
	// 壁に接触している場合の処理
	void HitWall(const CollisionMapInfo& info);
	// 設置状態の切り替え
	void OnGround(const CollisionMapInfo& info);
	// 旋回制御
	void TurnControl();
	// 行列計算

	// マップ衝突判定

	// 各方向衝突判定
	// 上方向
	void TopCollision(CollisionMapInfo& info);
	// 下方向
	void BottomCollision(CollisionMapInfo& info);
	// 右方向
	void RightCollision(CollisionMapInfo& info);
	// 左方向
	void LeftCollision(CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner) const;

	/*ワールド座標を取得*/
	Vector3 GetWorldPosition() const;

	/*AABBを取得*/
	AABB GetAABB() const;

	/*衝突応答*/
	//void OnCollision(const Enemy* enemy);

	bool GetIsAlive() { return isAlive_; }

	bool GetIsGravityInvert() const { return isGravityInvert; }

	Vector3 GetCenterPosition() const override;

	void OnCollision(Collider* other) override;

	void SetLandingTexture(const std::string& handle);

	void SetIsAllive(const bool& isAllive);

	void SetIsHitEnemyAttack(const bool& isHitEnemyAttack);

	// 接地したか
	bool IsLand()const { return land; }

	void SetIsPlayStartAnimation(const bool& isStartAnimation);

private: // メンバ変数

	// マップチップフィールドによるフィールド
	MapChipField* mapChipField_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 3Dモデル
	Model* model_ = nullptr;
	std::vector<Model*> models_;
	std::vector<Model::Animation*> animas_;
	std::vector<Model::Skeleton*> skeletons_;
	float animationTime = 0.0f;
	float limitAnimaTime = 0.0f;
	bool isRun = false;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	std::string landingTexture_;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};

	enum class LRDirection {
		kRight,
		kLeft,
	};
	bool isAlive_ = true;
	LRDirection lrDirection_ = LRDirection::kRight;

	Animation nowAnima_ = Animation::idle;

	bool isGravityInvert = false;
	bool isJumping = false;
	bool landing;
	bool preLand = false;
	bool land;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;

	float kLimitRunSpeed = 0.11f;
	float kAcceleration = 0.03f;
	float kAttenuation = 0.3f;
	float kAttenuationLanding = 0.3f;
	float kTimeTurn = 0.125f;

	// 重力加速度（下方向）
	float kGravityAcceleration = 0.01f;
	// 最大落下速度（下方向）
	float kLimitFallSpeed = 0.3f;
	// ジャンプ速度（上方向）
	float kJumpAcceleration = 0.1f;
	// 壁に接触時速度
	float kAttenuationWall = 0.6f;
	// キャラクターの当たり判定サイズ
	float kWidth = 0.8f;
	float kHeight = 0.8f;
	float kBlank = 0.1f;
	float kCorrectBlank = 0.5f;

	// 針に接触したときの原則係数
	float kNeedleSlowness = 0.5f;
	// 針に再度接触するまでの時間(秒)
	float kMaxSlownessTime = 0.5f;
	float slownessTimer = 0.0f;
	// 針に接触しているか?
	bool isHitNeedle = false;
	// エネミーの攻撃を食らっているかどうか
	bool isHitEnemyAttack_ = false;

	// スタートアニメーションフラグ
	bool isPlayStartAnimation_ = true;
};

