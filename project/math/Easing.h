#pragma once
#include"Vector3.h"
namespace Easing {
	inline float easeInOutQuad(float t) {
		return t < 0.5 ? 2 * t * t : 1 - (float)pow(-2 * t + 2, 2) / 2;
	}

} // namespace Easing
inline Vector3 Lerp(const Vector3& start, const Vector3& end, float t) {
	return Vector3{
		start.x + t * (end.x - start.x), start.y + t * (end.y - start.y),
		start.z + t * (end.z - start.z)
	};
}