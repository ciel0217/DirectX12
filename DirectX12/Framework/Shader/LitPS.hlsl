#include "common.hlsl"

Texture2D BaseColor : register(t0);
Texture2D Normal : register(t1);
Texture2D RoughMetaSpe : register(t2);

StructuredBuffer<Light> Lights : register(t3);

SamplerState Sampler : register(s0);

float4 main(OutputVS2D outputVS) : SV_Target0
{
	Light light = Lights[0];
	return BaseColor.Sample(Sampler, outputVS.TexCoord) + Normal.Sample(Sampler, outputVS.TexCoord) + RoughMetaSpe.Sample(Sampler, outputVS.TexCoord) + float4(light.lightColor, 1.0f);
}