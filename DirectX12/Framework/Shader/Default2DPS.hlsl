#include "common.hlsl"

Texture2D DiffuseTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D RoughMetaSpeTexture : register(t2);

SamplerState Sampler : register(s0);

float4 main(OutputVS2D outputVS) : SV_Target0
{
	float4 ret;
	ret = DiffuseTexture.Sample(Sampler, outputVS.TexCoord) + NormalTexture.Sample(Sampler, outputVS.TexCoord) + RoughMetaSpeTexture.Sample(Sampler, outputVS.TexCoord);
	return ret;
}