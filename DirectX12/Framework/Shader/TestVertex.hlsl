//アラインメントしたほうがいいのかな
struct VERTEX_3D
{
	float3 Position : POSITION;//座標
	float3 Normal : NORMAL;//法線
	float2 TexCoord : TEXCOORD;//UV値
	float4 Diffuse : COLOR;//頂点カラー
};

float4 main(VERTEX_3D parameter) : SV_POSITION
{
	return float4(parameter.Position, 0.0);
}