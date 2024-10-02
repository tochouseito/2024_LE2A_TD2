#pragma once
#include <cstdint>
#include"Vector4.h"
#include"Color.h"
#include"Matrix4x4.h"
#include<d3d12.h>
#include<wrl.h>
#include"fstream"
#include"sstream"
// 定数バッファ用データ構造体
struct ConstBufferDataColor {
	Color color;
};
class ObjectColor
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	Color color_ = { 1.0f,1.0f,1.0f,1.0f };
	uint32_t numInstance_ = 1;
	ObjectColor() = default;
	~ObjectColor() = default;

	void CreateColorResource();
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU() { return instancingSrvHandleGPU_; }
	ID3D12Resource* GetResource()const { return colorResource_.Get(); }
	void SetColor(const Color& color) { colorData_->color = color; }
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();
private:
	Microsoft::WRL::ComPtr < ID3D12Resource> colorResource_;
	ConstBufferDataColor* colorData_ = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_;
	uint32_t kNumMaxInstance_ = 0;
	uint32_t srvIndex = 0;
};

