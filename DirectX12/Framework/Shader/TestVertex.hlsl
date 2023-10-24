//�A���C�������g�����ق��������̂���
struct VERTEX_3D
{
	float3 Position : POSITION;//���W
	float3 Normal : NORMAL;//�@��
	float2 TexCoord : TEXCOORD;//UV�l
	float4 Diffuse : COLOR;//���_�J���[
};

struct Output
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

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



Output main(VERTEX_3D parameter)
{
	Output o;
	float4 position = float4(parameter.Position, 1.0);

	matrix a = mul(world, view);
	a = mul(a, proj);

	o.Position = mul(position, a);

	o.TexCoord = parameter.TexCoord;
	return o;
}