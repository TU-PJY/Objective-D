cbuffer cbGameObjectInfo : register(b0)
{
    matrix gmtxWorld : packoffset(c0);
    float3 gf3ObjectColor : packoffset(c4);
};

cbuffer cbCameraInfo : register(b1)
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
    float3 gf3CameraPosition : packoffset(c8);
};

// 텍스처 반전
cbuffer cbFlipInfo : register(b2)
{
    int X_Flip; // 텍스처 좌우반전 여부
    int Y_Flip; // 텍스처 상하반전 여부
}

// 텍스처 투명도
cbuffer cbAlphaInfo : register(b3)
{
    float AlphaValue;
}

Texture2D gTexture : register(t0); // 텍스처 샘플링을 위한 텍스처 리소스
SamplerState gSampler : register(s0); // 텍스처 샘플러 상태

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXTURECOORD;
};

struct VS_OUTPUT
{
    float4 positionH : SV_POSITION;
    float3 positionW : POSITION;
    float3 normal : NORMAL0;
    float3 normalW : NORMAL1;
    float2 uv : TEXTURECOORD;
};

VS_OUTPUT VSImageColor(VS_INPUT input)
{
    VS_OUTPUT output;

    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.positionH = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.normalW = mul(float4(input.normal, 0.0f), gmtxWorld).xyz;
    output.normal = input.normal;
    output.uv = input.uv;
    
    if (X_Flip == 1)
        output.uv.x = 1.0f - output.uv.x;
    
    if (Y_Flip == 1)
        output.uv.y = 1.0f - output.uv.y;

    return (output);
}

float4 PSImageColor(VS_OUTPUT input) : SV_TARGET
{
    float4 texColor = gTexture.Sample(gSampler, input.uv);
    float3 meshColor = gf3ObjectColor;
    
    texColor.rgb += meshColor;

    if (texColor.a < 0.01)
        discard;
  
    float4 outputColor = float4(texColor.rgb, texColor.a * AlphaValue);
    return outputColor;
}