// ParticleShader.hlsl
struct VSOutput
{
    float3 position : POSITION;
    float4 color : COLOR;
    float size : SIZE;
};

struct GSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};

// Vertex Shader
VSOutput VS_Main(float3 position : POSITION, float4 color : COLOR, float size : SIZE)
{
    VSOutput output;
    output.position = position;
    output.color = color;
    output.size = size;
    return output;
}

// Geometry Shader
[maxvertexcount(4)]
void GS_Main(point VSOutput input[1], inout TriangleStream<GSOutput> stream)
{
    float3 pos = input[0].position;
    float size = input[0].size;

    float3 right = float3(size, 0, 0);
    float3 up = float3(0, size, 0);

    GSOutput vertices[4] =
    {
        { float4(pos - right - up, 1.0), input[0].color, float2(0, 1) },
        { float4(pos + right - up, 1.0), input[0].color, float2(1, 1) },
        { float4(pos + right + up, 1.0), input[0].color, float2(1, 0) },
        { float4(pos - right + up, 1.0), input[0].color, float2(0, 0) }
    };

    for (int i = 0; i < 4; ++i)
    {
        stream.Append(vertices[i]);
    }
}

// Pixel Shader
float4 PS_Main(GSOutput input) : SV_TARGET
{
    return float4(1.0, 0.0, 0.0, 1.0); // 강제로 빨간색 출력
}