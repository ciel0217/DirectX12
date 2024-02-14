
cbuffer VP : register(b0)
{
	matrix view;
	matrix proj;
	matrix inverseView;
	matrix inverseProj;
};

cbuffer World : register(b1)
{
	matrix world;
};

//アラインメントしたほうがいいのかな
struct VERTEX_3D
{
	float3 Position : POSITION;//座標
	float3 Normal : NORMAL;//法線
	float2 TexCoord : TEXCOORD;//UV値
	float4 Diffuse : COLOR;//頂点カラー
};

struct OutputVS
{
	float4 Position : SV_POSITION;
	float4 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
};

struct OutputPS
{
	float4 BaseColor : SV_Target0;
	float4 Normal : SV_Target1;
	float4 RoughMetaSpe : SV_Target2;
};