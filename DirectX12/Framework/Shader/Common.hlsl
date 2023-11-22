
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

//�A���C�������g�����ق��������̂���
struct VERTEX_3D
{
	float3 Position : POSITION;//���W
	float3 Normal : NORMAL;//�@��
	float2 TexCoord : TEXCOORD;//UV�l
	float4 Diffuse : COLOR;//���_�J���[
};

struct OutputVS
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};
