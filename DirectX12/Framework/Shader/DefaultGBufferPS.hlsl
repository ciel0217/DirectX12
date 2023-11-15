struct Output
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};



Texture2D		DiffuseTexture : register(t0);
SamplerState	Sampler : register(s0);


float4 main(Output i) : SV_TARGET
{
	float4 color;
	color = DiffuseTexture.Sample(Sampler, i.TexCoord);

	return color;
}