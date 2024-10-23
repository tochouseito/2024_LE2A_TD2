#pragma once

#include <memory>
#include <string>

#include "math/Mymath.h"
#include "Sprite.h"

class ViewProjection;

class Fade {
public:
	enum Fase {
		kIn,
		kOut,
	};

	Fade() = default;
	~Fade() = default;

	void Initialize(ViewProjection viewProjection);

	void Update();

	void Draw();

	void SetFase(int fase);

private:
	Fase fase_ = kIn;
	

};