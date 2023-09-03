struct Output
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);

float4 main(Output i) : SV_TARGET
{
	float4 color;
	color = g_Texture.Sample(g_SamplerState, i.TexCoord);
	return color;
}