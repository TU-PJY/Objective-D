cbuffer cbGameObjectInfo : register(b0) 
{
    matrix gmtxWorld : packoffset(c0);
    float3 gf3ObjectColor : packoffset(c4);
}

cbuffer cbCameraInfo : register(b1) 
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
    float3 gf3CameraPosition : packoffset(c8);
}

cbuffer cbFlipInfo : register(b2) 
{
    int X_Flip;
    int Y_Flip;
}

cbuffer cbAlphaInfo : register(b3) 
{
    float AlphaValue;
}

cbuffer cbLightInfo : register(b4) 
{
    float3 gLightDirection;
    float3 gLightColor;
    float3 gAmbientColor;
    float gShadowStrength;
}

cbuffer cbLightUseInfo : register(b5)
{
    int UseLight;
}

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

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

VS_OUTPUT VSTexColor(VS_INPUT input) 
{
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

float3 ComputeLightColor(VS_OUTPUT input, float4 texColor)
{
    float3 ambient = gAmbientColor * texColor.rgb;
    float3 lightDir = normalize(-gLightDirection); 
    float3 normal = normalize(input.normalW);
    float NdotL = max(dot(normal, lightDir), 0.0); 
    float3 diffuse = gLightColor * NdotL * texColor.rgb;
    diffuse *= gShadowStrength;

    float3 finalColorWithLight = ambient + diffuse;
    return finalColorWithLight;
}

float4 PSTexColor(VS_OUTPUT input) : SV_TARGET
{
    float4 texColor = gTexture.Sample(gSampler, input.uv);
    float3 meshColor = gf3ObjectColor;

    // UseLight가 0이면 조명 사용 안 함, 1이면 조명 사용함
    float3 finalColor = lerp(texColor.rgb, ComputeLightColor(input, texColor), UseLight);
    
    // 덮어씌울 색상이 있다면 덮어 씌움
    finalColor += meshColor;
    
    // 텍스처의 투명 부분을 제거
    if(texColor.a < 0.01)
        discard;
  
    float4 outputColor = float4(finalColor, texColor.a * AlphaValue);
    return outputColor;
}