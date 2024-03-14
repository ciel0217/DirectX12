#include "common.hlsl"

Texture2D BaseColor : register(t0);
Texture2D Normal : register(t1);
Texture2D RoughMetaSpe : register(t2);

StructuredBuffer<Light> Lights : register(t3);

SamplerState Sampler : register(s0);

float4 main(OutputVS2D outputVS) : SV_Target0
{
	Light light = Lights[0];
	float lightColor = saturate(dot(Normal.Sample(Sampler, outputVS.TexCoord) * 2.0 - 1.0, light.lightDir));
	
	return BaseColor.Sample(Sampler, outputVS.TexCoord)* lightColor + RoughMetaSpe.Sample(Sampler, outputVS.TexCoord);
}