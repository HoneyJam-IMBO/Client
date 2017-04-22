

static const float2 arrBasePos[4] = {
	float2(-1.0, -1.0),
	float2(-1.0, 1.0),
	float2(1.0, -1.0),
	float2(1.0, 1.0)
};
static const float2 arrUV[4] = {
	float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0)
};

struct VS_TEXTURED_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};


//¥Ÿ¡ﬂ ∑ª¥ı≈∏∞Ÿ
VS_TEXTURED_OUTPUT main(uint vertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output = (VS_TEXTURED_OUTPUT)0;
	output.position = float4(arrBasePos[vertexID], 0.0f, 1.0f);
	output.texCoord = arrUV[vertexID];
	//output.position = float4(position, 1.0f);
	//output.texCoord = texCoord;

	return(output);
}
