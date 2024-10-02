#include "ObjectColor.h"
#include"DirectXCommon.h"
#include"SrvManager.h"
#include"Mymath.h"
/// <summary>
/// 初期化
/// </summary>
void ObjectColor::Initialize() {
	CreateColorResource();
	
	// SRV確保
	/*srvIndex = SrvManager::GetInstance()->Allocate();
	instancingSrvHandleCPU_ = SrvManager::GetInstance()->GetCPUDescriptorHandle(srvIndex);
	instancingSrvHandleGPU_ = SrvManager::GetInstance()->GetGPUDescriptorHandle(srvIndex);
	SrvManager::GetInstance()->CreateSRVforStructuredBuffer(srvIndex, colorResource_.Get(), kNumMaxInstance_, sizeof(ConstBufferDataColor));*/
}
void ObjectColor::CreateColorResource() {
	//kNumMaxInstance_ = Instance;
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	colorResource_ = DirectXCommon::GetInstance()->CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(ConstBufferDataColor));
	// 書き込むためのアドレスを取得
	colorResource_->Map(0, nullptr, reinterpret_cast<void**>(&colorData_));
	// 単位行列を書き込んでいく
	colorData_->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
}
/// <summary>
/// 行列を転送する
/// </summary>
void ObjectColor::TransferMatrix() {
	//if (numInstance_ < kNumMaxInstance_) {
		colorData_->color = color_;
		colorData_->color.a = color_.a;
	//}
}
