cbuffer cbFrameworkInfo : register(b0) {
    float gfCurrentTime;
    float gfElapsedTime;
    float2 gf2CursorPos;
};

cbuffer cbGameObjectInfo : register(b1) {
    matrix gmtxWorld : packoffset(c0);
    float3 gf3ObjectColor : packoffset(c4);
};

cbuffer cbCameraInfo : register(b2) {
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
    float3 gf3CameraPosition : packoffset(c8);
};

// �ؽ�ó ����
cbuffer cdFlipInfo : register(b3) {
    int X_Flip = 0; // �ؽ�ó �¿���� ����
    int Y_Flip = 0; // �ؽ�ó ���Ϲ��� ����
}

// �ؽ�ó ����
cbuffer cdAlphaInfo : register(b4) {
    float AlphaValue = 0.0f;
}

Texture2D gTexture : register(t0); // �ؽ�ó ���ø��� ���� �ؽ�ó ���ҽ�
SamplerState gSampler : register(s0); // �ؽ�ó ���÷� ����

struct VS_INPUT {
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXTURECOORD;
};

struct VS_OUTPUT {
    float4 positionH : SV_POSITION;
    float3 positionW : POSITION;
    float3 normal : NORMAL0;
    float3 normalW : NORMAL1;
    float2 uv : TEXTURECOORD;
};

VS_OUTPUT VSPseudoLighting(VS_INPUT input) {
    VS_OUTPUT output;

    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.positionH = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.normalW = mul(float4(input.normal, 0.0f), gmtxWorld).xyz;
    output.normal = input.normal;
    output.uv = input.uv;
    
    if(X_Flip == 1)
        output.uv.x = 1.0f - output.uv.x;
    
    if(Y_Flip == 1)
        output.uv.y = 1.0f - output.uv.y;

    return (output);
}

float4 PSPseudoLighting(VS_OUTPUT input) : SV_TARGET {
    float4 texColor = gTexture.Sample(gSampler, input.uv);
    texColor.a = AlphaValue;
    
    return texColor; // �ؽ�ó ������ ��ȯ
}