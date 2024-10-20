#include "DeltaTime.h"
#include"imgui.h"

void DeltaTime::Update() {
	value = 1.0f / 60.0f; // RELEASE
#ifdef _DEBUG
	value = ImGui::GetIO().DeltaTime;
	ImGui::Begin("DeltaTime");
	ImGui::Text("FrameRate: %.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Text("Value: %.2f", value);
	ImGui::End();
#endif // _DEBUG

}
