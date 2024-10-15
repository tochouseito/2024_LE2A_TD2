#pragma once
#include"stdint.h"
#include"vector"
#include"Vector3.h"
#include"string"
#include"iosfwd"


enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
	kUpNeedle, // 上向き針
	kDownNeedle, // 下向き針
	kPlayer,	 // プレイヤー初期位置
	kEnemy,		 // エネミー初期位置
	kGoal,		 // ゴール
	kBullet,	 // プレイヤーの弾
};

class MapChipField {
public:
	struct IndexSet {
		uint32_t xInDex;
		uint32_t yIndex;
	};
	struct MapChipData {
		std::vector<std::vector<MapChipType>> data;
	};
	struct Rect {
		float left;   // 左端
		float right;  // 右端
		float bottom; // 下端
		float top;    // 上端
	};
	MapChipData mapChipData_;
	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	uint32_t GetNumBlockVertical() {
		return kNumBlockVertical;
	}
	uint32_t GetNumBlockHorizontal() {
		return kNumBlockHorizontal;
	}
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
		Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);
		// 指定ブロックの中心座標を取得する
		Rect rect;
		rect.left = center.x - kBlockWidth / 2.0f;
		rect.right = center.x + kBlockWidth / 2.0f;
		rect.bottom = center.y - kBlockHeight / 2.0f;
		rect.top = center.y + kBlockHeight / 2.0f;
		return rect;
	}

private:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 125;
};