#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"
class MapChipField;
enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上

	kNumCorner // 要素数
};
class Player {
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
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

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

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	/*ワールド座標を取得*/
	Vector3 GetWorldPosition();

	/*AABBを取得*/
	AABB GetAABB();

	/*衝突応答*/
	//void OnCollision(const Enemy* enemy);

	bool GetIsAlive() { return isAlive; }

private: // メンバ変数

	// マップチップフィールドによるフィールド
	MapChipField* mapChipField_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 3Dモデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};

	enum class LRDirection {
		kRight,
		kLeft,
	};
	bool isAlive = true;
	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;



	static inline const float kAcceleration = 0.03f;
	static inline const float kLimitRunSpeed = 0.2f;
	static inline const float kAttenuation = 0.3f;
	static inline const float kAttenuationLanding = 0.3f;
	static inline const float kTimeTurn = 0.3f;

	// 重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.03f;
	// 最大落下速度（下方向）
	static inline const float kLimitFallSpeed = 0.3f;
	// ジャンプ速度（上方向）
	static inline const float kJumpAcceleration = 0.4f;
	// 壁に接触時速度
	static inline const float kAttenuationWall = 0.6f;
	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.1f;
	static inline const float kCorrectBlank = 0.5f;
};

