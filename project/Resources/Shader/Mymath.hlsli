float4x4 MakeAffineMatrix(float3 scale, float3 rotate, float3 translate)
{
    float4x4 scaleMatrix = float4x4(
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    
    float cosX = cos(rotate.x);
    float sinX = sin(rotate.x);
    float4x4 rotateXMatrix = float4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cosX, sinX, 0.0f,
        0.0f, -sinX, cosX, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    
    float cosY = cos(rotate.y);
    float sinY = sin(rotate.y);
    float4x4 rotateYMatrix = float4x4(
        cosY, 0.0f, -sinY, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sinY, 0.0f, cosY, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    
    float cosZ = cos(rotate.z);
    float sinZ = sin(rotate.z);
    float4x4 rotateZMatrix = float4x4(
        cosZ, sinZ, 0.0f, 0.0f,
        -sinZ, cosZ, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    
    float4x4 rotateMatrix = mul(rotateZMatrix, mul(rotateYMatrix, rotateXMatrix));
    
    float4x4 translateMatrix = float4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        translate.x, translate.y, translate.z, 1.0f);
    
    return mul(translateMatrix, mul(rotateMatrix, scaleMatrix));
}
float4x4 MakeBillboardMatrix(float4x4 cameraMatrix)
{
    // Rotate 180 degrees around the Y-axis to make the object face the camera
    float4x4 backToFrontMatrix = float4x4(
        -1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, -1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    );

    // Multiply the rotation matrix with the camera matrix to get the billboard matrix
    float4x4 billboardMatrix = mul(backToFrontMatrix, cameraMatrix);

    // Set the translation part of the matrix to zero
    billboardMatrix[3][0] = 0.0f;
    billboardMatrix[3][1] = 0.0f;
    billboardMatrix[3][2] = 0.0f;

    return billboardMatrix;
}