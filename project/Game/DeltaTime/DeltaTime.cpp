#include "DeltaTime.h"
#include"imgui.h"

void DeltaTime::Update()
{
	value = ImGui::GetIO().DeltaTime;
#ifdef _DEBUG
	ImGui::Begin("DeltaTime");
	ImGui::Text("FrameRate: %.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Text("Value: %.2f", value);
	ImGui::End();
#endif // _DEBUG

}
