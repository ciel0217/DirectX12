
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
	matrix invWorld;
	matrix transposeInvWorld;
};

//�A���C�������g�����ق��������̂���
struct VERTEX_3D
{
	float3 Position : POSITION;//���W
	float3 Normal : NORMAL;//�@��
	float2 TexCoord : TEXCOORD;//UV�l
	float4 Diffuse : COLOR;//���_�J���[
};

struct Light
{
	
	float3 lightDir;
	float3 lightColor;
	float lightPow;
	float attenuation;
	int lightType;//0:Dir 1:Point
	bool isEnable;
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

struct OutputVS2D
{
	float4 Position2D : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

static const float2 NDCPos[4] =
{
	float2(-1.0f, 1.0f),
	float2(-1.0f, -1.0f),
	float2(1.0f, 1.0f),
	float2(1.0f, -1.0f)
};

static const float2 NDCUV[4] =
{
	float2(0.0f, 0.0f),
	float2(0.0f, 1.0f),
	float2(1.0f, 0.0f),
	float2(1.0f, 1.0f)
};
