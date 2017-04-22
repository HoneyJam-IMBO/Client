#include "PackGbuffer.hlsli"
//#include "GetWorldNormal.hlsli"

//나중에 cBuffer로 바꿀 것임
#define SPLATTING_NUM 1

texture2D gtxtBase : register(t0);
Texture2DArray gtxtDetail : register(t1);
Texture2DArray gtxtBlendInfo : register(t2);
texture2D gtxtPicpos : register(t3);
SamplerState gssWRAP_LINEAR : register(s0);
SamplerState gssWRAP_POINT : register(s1);
SamplerState gssCLAMP_LINEAR : register(s2);
SamplerState gssCLAMP_POINT : register(s3);

//sampler gssPicpos : register(s2);


texture2D gtxtNormal : register(t5);
sampler gssNormal : register(s5);


struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

cbuffer gMaterialInfo : register(b3) {
	float4 gMaterialColor : packoffset(c0);
	float gSpecExp : packoffset(c1.x);
	float gSpecIntensity : packoffset(c1.y);
}
cbuffer gSplattingInfo : register(b5) {
	uint gSplattingNum : packoffset(c0.x);
}
struct DS_OUT {
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float2 texCoord : TEXCOORD;
	float2 detailTexCoord : TEXCOORD1;
	//float3 tangentW : TANGENT;
	//float3 bitangentW : BITANGET;
};

PS_GBUFFER_OUT main(DS_OUT input){
	PS_GBUFFER_OUT output = (PS_GBUFFER_OUT)0;

	//set base color
	float4 cBaseTexColor = gtxtBase.Sample(gssWRAP_LINEAR, input.texCoord);
	float4 cDetailTexColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float cDetailAlpha = 1.0f;

	float4 cCurrentTexColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 cPreviousTexColor = cBaseTexColor;
	cDetailTexColor = cBaseTexColor;
	//set base color

	//인덱스 
	float CurrentTextureIndex = 0;

	//set splatting detail color
	while (CurrentTextureIndex < gSplattingNum){
		cCurrentTexColor = gtxtDetail.Sample(gssWRAP_LINEAR, float3(input.detailTexCoord, CurrentTextureIndex));//get detail
		//cCurrentTexColor = saturate((cPreviousTexColor * 0.5f) + (cCurrentTexColor * 0.5f)); 
		cDetailAlpha = gtxtBlendInfo.Sample(gssWRAP_LINEAR, float3(input.texCoord, CurrentTextureIndex)).x;
		cDetailTexColor = lerp(cPreviousTexColor, cCurrentTexColor, cDetailAlpha);
		cPreviousTexColor = cDetailTexColor;
		CurrentTextureIndex++;
	}

	float4 cColor = saturate(cDetailTexColor);

	//set splatting detail color
	
	
	//get world normal
	//float3 normalW = GetWorldNormal(input.tangentW, input.bitangentW, input.texCoord);
	float3 normalW = gtxtNormal.Sample(gssWRAP_LINEAR, input.texCoord).rgb;
	normalW = normalW*2-1;//0-2 //-1 1
	return (PackGBuffer((float3)cColor, normalW, gSpecIntensity, gSpecExp));
}