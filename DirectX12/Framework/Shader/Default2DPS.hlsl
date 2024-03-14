#include "common.hlsl"

Texture2D Texture : register(t0);

SamplerState Sampler : register(s0);

float4 main(OutputVS2D outputVS) : SV_Target0
{
	float4 ret;
	ret = Texture.Sample(Sampler, outputVS.TexCoord);
	return ret;
}