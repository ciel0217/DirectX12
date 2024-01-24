#include "Common.hlsl"

OutputVS main(VERTEX_3D parameter)
{
	OutputVS o;
	float4 position = float4(parameter.Position, 1.0);

	matrix a = mul(world, view);
	a = mul(a, proj);

	o.Position = mul(position, a);
	o.TexCoord = parameter.TexCoord;

	return o;
}