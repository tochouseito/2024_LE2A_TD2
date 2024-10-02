#include "LightGroup.h"
#include"imgui.h"

void LightGroup::Initialize()
{
	directionalLight_ = new DirectionalLight();
	spotLight_ = new SpotLight();
	pointLight_ = new PointLight();
	directionalLight_->CreateDirectionalLightResource();
	pointLight_->CreatePointLightResource();
	spotLight_->CreateSpotLightResource();
}

void LightGroup::Update()
{
#ifdef _DEBUG
	ImGui::Begin("GameSceneLighting");

	if (ImGui::CollapsingHeader("Directional")) {
		ImGui::DragFloat4("dColor", &directionalLight_->GetDirectionalLightData()->color.r, 0.01f);
		ImGui::SliderFloat("dIntensity", &directionalLight_->GetDirectionalLightData()->intensity, 0.0f, 1.0f);
		ImGui::DragFloat3("dDirection", &directionalLight_->GetDirectionalLightData()->direction.x, 0.01f);
	}
	if (ImGui::CollapsingHeader("Spot")) {
		ImGui::DragFloat4("sColor", &spotLight_->GetSpotLightData()->color.r, 0.01f);
		ImGui::SliderFloat("sIntensity", &spotLight_->GetSpotLightData()->intensity, 0.0f, 1.0f);
		ImGui::DragFloat3("sDirection", &spotLight_->GetSpotLightData()->direction.x, 0.01f);
	}
	if (ImGui::CollapsingHeader("Point")) {
		ImGui::DragFloat4("pColor", &pointLight_->GetPointLightData()->color.r, 0.01f);
		ImGui::SliderFloat("pintensity", &pointLight_->GetPointLightData()->intensity, 0.0f, 1.0f);
	}

	ImGui::End();
#endif // _DEBUG

}

void LightGroup::Finalize()
{
	delete pointLight_;
	delete spotLight_;
	delete directionalLight_;
}
