#pragma once

#include"DirectionalLight.h"
#include"PointLight.h"
#include"SpotLight.h"
class LightGroup
{
public:/*シングルトンクラス*/
    // インスタンスを取得するための静的メソッド
    static LightGroup* GetInstance()
    {
        static LightGroup instance; // 静的なインスタンス
        return &instance;
    }

public:
    // コピーコンストラクタと代入演算子を削除して、複数のインスタンスが作成されることを防ぐ
    LightGroup(const LightGroup&) = delete;
    LightGroup& operator=(const LightGroup&) = delete;
private:
    // コンストラクタをプライベートにして、外部からの直接インスタンス化を防ぐ
    LightGroup() {}
    ~LightGroup() {}

public:/*メンバ関数*/

    void Initialize();
    void Update();
    void Finalize();

    ID3D12Resource* GetDirectionalResource()const { return directionalLight_->GetLightResource(); }
    ID3D12Resource* GetSpotResource()const { return spotLight_->GetLightResource(); }
    ID3D12Resource* GetPointResource()const { return pointLight_->GetLightResource(); }

private:
    DirectionalLight* directionalLight_ = nullptr;
    SpotLight* spotLight_ = nullptr;
    PointLight* pointLight_ = nullptr;
};

