#include "PackGbuffer.hlsli"


texture2D gtxtBase		: register(t0);
Texture2D gtxtReflection: register(t1);
Texture2D gtxtRefrection: register(t2);
texture2D gtxtNormal	: register(t3);

SamplerState gssWRAP_LINEAR : register(s0);
SamplerState gssWRAP_POINT	: register(s1);
SamplerState gssCLAMP_LINEAR: register(s2);
SamplerState gssCLAMP_POINT : register(s3);
SamplerState gssShadow : register(s4);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
	float4 reflectionPosition : TEXCOORD1;
	float4 refractionPosition : TEXCOORD2;
};

cbuffer WaterBuffer: register(b4)
{
	float waterTranslation;
	float reflectRefractScale;
	float2 padding;
};


PS_GBUFFER_OUT main(PixelInputType input) : SV_TARGET
{
	float2 reflectTexCoord;
	float2 refractTexCoord;
	float4 normalMap;
	float3 normal;
	float4 reflectionColor;
	float4 refractionColor;
	float4 color;

	PS_GBUFFER_OUT output = (PS_GBUFFER_OUT)0;


	// Move the position the water normal is sampled from to simulate moving water.	
	input.tex.y += 10.f;

	// Calculate the projected reflection texture coordinates.
	reflectTexCoord.x = (input.reflectionPosition.x / input.reflectionPosition.w) * 0.5f + 0.5f;
	reflectTexCoord.y = -((input.reflectionPosition.y) / input.reflectionPosition.w) * 0.5f + 0.5f;

	// Calculate the projected refraction texture coordinates.
	//refractTexCoord.x = (input.refractionPosition.x / input.refractionPosition.w) * 0.5f + 0.5f;
	//refractTexCoord.y = -(input.refractionPosition.y / input.refractionPosition.w) * 0.5f + 0.5f;

	//// Sample the normal from the normal map texture.
	//normalMap = gtxtNormal.Sample(gssWRAP_LINEAR, input.tex);

	//// Expand the range of the normal from (0,1) to (-1,+1).
	//normal = (normalMap.xyz * 2.0f) - 1.0f;

	//// Re-position the texture coordinate sampling position by the normal map value to simulate the rippling wave effect.
	//reflectTexCoord = reflectTexCoord + (normal.xy * reflectRefractScale);
	//refractTexCoord = refractTexCoord + (normal.xy * reflectRefractScale);

	// Sample the texture pixels from the textures using the updated texture coordinates.
	//reflectTexCoord.y *= -1.f;
	reflectionColor = gtxtReflection.Sample(gssShadow, reflectTexCoord);
	reflectionColor.w = 0.5f;
	//reflectionColor = gtxtReflection.Sample(gssWRAP_LINEAR, input.tex);
	//refractionColor = gtxtRefrection.Sample(gssWRAP_LINEAR, refractTexCoord);

	// Combine the reflection and refraction results for the final color.
	//color = lerp(reflectionColor, refractionColor, 0.6f);
//	float4 cSpec;

	//return reflectionColor;
	return (PackGBuffer(reflectionColor.xyz, normalize(input.normal), float3(0.f, 0.f, 0.f), 0.f));
}