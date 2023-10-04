//アラインメントしたほうがいいのかな
struct VERTEX_3D
{
	float3 Position : POSITION;//座標
	float3 Normal : NORMAL;//法線
	float2 TexCoord : TEXCOORD;//UV値
	float4 Diffuse : COLOR;//頂点カラー
};

struct Output
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

cbuffer wv : register(b0)
{
	matrix w;
	matrix v;
	matrix p;
};

cbuffer pv : register(b1)
{
	float s;
};



Output main(VERTEX_3D parameter)
{
	Output o;
	float4 position = float4(parameter.Position, 1.0);

	matrix a = mul(w, v);
	a = mul(a, p);

	o.Position = mul(position, a);

	o.TexCoord = parameter.TexCoord * s;
	return o;
}