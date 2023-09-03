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

Output main(VERTEX_3D parameter)
{
	Output o;
	o.Position = float4(parameter.Position, 1.0);
	o.TexCoord = parameter.TexCoord;
	return o;
}