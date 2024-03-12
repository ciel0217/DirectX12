#include "common.hlsl"

OutputVS2D main(uint VertexID : SV_VertexID)
{
	OutputVS2D ret;

	ret.Position2D = float4(NDCPos[VertexID], 0.0f, 1.0f);
	ret.TexCoord = NDCUV[VertexID];

	return ret;
}