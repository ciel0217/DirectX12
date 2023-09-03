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

Output main(VERTEX_3D parameter)
{
	Output o;
	o.Position = float4(parameter.Position, 1.0);
	o.TexCoord = parameter.TexCoord;
	return o;
}