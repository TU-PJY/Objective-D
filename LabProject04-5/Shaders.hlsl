cbuffer cbFrameworkInfo : register(b0)
{
    float gfCurrentTime;
    float gfElapsedTime;
    float2 gf2CursorPos;
};

cbuffer cbGameObjectInfo : register(b1)
{
    matrix gmtxWorld : packoffset(c0);
    float3 gf3ObjectColor : packoffset(c4);
};

cbuffer cbCameraInfo : register(b2)
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
    float3 gf3CameraPosition : packoffset(c8);
};

Texture2D gTexture : register(t0); // �ؽ�ó ���ø��� ���� �ؽ�ó ���ҽ�
SamplerState gSampler : register(s0); // �ؽ�ó ���÷� ����

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

VS_OUTPUT VSPseudoLighting(VS_INPUT input)
{
    VS_OUTPUT output;

    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.positionH = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.normalW = mul(float4(input.normal, 0.0f), gmtxWorld).xyz;
    output.normal = input.normal;
    output.uv = input.uv;
    output.uv = float2(input.uv.x, 1.0f - input.uv.y);

    return (output);
}

float4 PSPseudoLighting(VS_OUTPUT input) : SV_TARGET
{
    float4 texColor = gTexture.Sample(gSampler, input.uv);
    float brightness = 3.0f; // ��� ���� 1.5�� ���� (�� ���� ���� ����)
    texColor.rgb *= brightness; // RGB ���� ��� ���ϱ�
    
    return texColor; // �ؽ�ó ������ ��ȯ
}