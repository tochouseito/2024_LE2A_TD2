#pragma once
#include <iostream>
#include <cmath>

/// <summary>
/// 3次元ベクトル
/// </summary>

struct Vector3 final {
	float x;
	float y;
	float z;
	// 0.0f 初期化
	void Initialize() {

		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	// 任意の数で初期化
	void SetInitialize(float value) {

		x = value;
		y = value;
		z = value;
	}
	// 等号演算子
	bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }

	// 不等号演算子
	bool operator!=(const Vector3& other) const { return !(*this == other); }

	// 小なりイコール演算子 (<=)
	bool operator<=(const Vector3& other) const {
		return (this->x <= other.x && this->y <= other.y && this->z <= other.z);
	}

	Vector3& operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3 operator+(const Vector3& other) const { return { x + other.x, y + other.y, z + other.z }; }
	// 負号演算子のオーバーロード
	Vector3 operator-() const { return { -x, -y, -z }; }
	Vector3 operator-(const Vector3& other) const { return { x - other.x, y - other.y, z - other.z }; }

	Vector3 operator*(const float& other) const { return { x * other, y * other, z * other }; }
	friend Vector3 operator*(float scalar, const Vector3& v) { return Vector3(v.x * scalar, v.y * scalar, v.z * scalar); }

	Vector3 operator/(const float& other) const { return { x / other, y / other, z / other }; }
	friend Vector3 operator/(float scalar, const Vector3& v) { return Vector3(v.x / scalar, v.y / scalar, v.z / scalar); }

	Vector3& operator-=(const Vector3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector3& operator=(float scalar) {
		x = scalar;
		y = scalar;
		z = scalar;
		return *this;
	}
	// スカラー倍代入
	Vector3& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	Vector3& operator/=(float scalar) {
		if (scalar != 0) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
		} else {
			std::cerr << "Error: Division by zero" << std::endl;
		}
		return *this;
	}

	Vector3 Abs() const {
		return Vector3(std::abs(x), std::abs(y), std::abs(z));
	}
};