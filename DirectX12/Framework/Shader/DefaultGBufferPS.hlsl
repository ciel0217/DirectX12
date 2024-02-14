#include "Common.hlsl"

Texture2D		DiffuseTexture : register(t0);
Texture2D		NormalMapTexture: register(t1);
Texture2D		RoughMetaSpeTexture: register(t2);

SamplerState	Sampler : register(s0);


OutputPS main(OutputVS i)
{
	OutputPS o;
	float4 baseColor;
	baseColor = DiffuseTexture.Sample(Sampler, i.TexCoord);
	o.Normal = i.Normal;

	o.BaseColor = baseColor;


	return o;
}