#pragma once
class DeltaTime
{
public:
	void Update();

	float  GetDeltaTime()const {return value;}

private:

	float value;
};

