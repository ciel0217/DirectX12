struct Output
{
	float4 BaseColor : SV_Target0;
};


Texture2D		DiffuseTexture : register(t0);
SamplerState	Sampler : register(s0);


float4 main(Output i) : SV_TARGET
{
	float4 color;
	color = DiffuseTexture.Sample(Sampler, i.TexCoord);

	return color;
}