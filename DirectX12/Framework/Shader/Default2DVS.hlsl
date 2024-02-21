#include "common.hlsl"

static const float2 BasePos[4] = 
{
	float2(-1.0f, 1.0f),
	float2(-1.0f, -1.0f),
	float2(1.0f, 1.0f),
	float2(1.0f, -1.0f)
};

static const float2 BaseUV[4] = 
{
	float2(0.0f, 0.0f),
	float2(0.0f, 1.0f),
	float2(1.0f, 0.0f),
	float2(1.0f, 1.0f)
};


OutputVS2D main(uint VertexID : SV_VertexID)
{
	OutputVS2D ret;

	ret.Position2D = float4(BasePos[VertexID], 0.0f, 1.0f);
	ret.TexCoord = BaseUV[VertexID];

	return ret;
}