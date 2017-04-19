

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
	float4 reflectionPosition : TEXCOORD1;
	float4 refractionPosition : TEXCOORD2;
};

//cbuffer MatrixBuffer: register(b0)
//{
//	matrix reflectionMatrix;
//	matrix worldMatrix;
//	matrix viewMatrix;
//	matrix projectionMatrix;
//};

VS_OUTPUT main(	float3 position : POSITION,
				float3 normal : NORMAL,
				float2 tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//matrix reflectProjectWorld;
	//matrix viewProjectWorld;

	//// Change the position vector to be 4 units for proper matrix calculations.
	////position.w = 1.0f;

	//// Calculate the position of the vertex against the world, view, and projection matrices.
	//output.position = mul(float4(position, 1.f), worldMatrix);
	//output.position = mul(output.position, viewMatrix);
	//output.position = mul(output.position, projectionMatrix);

	//// Store the texture coordinates for the pixel shader.
	//output.tex = tex;

	//// Create the reflection projection world matrix.
	////reflectProjectWorld = mul(reflectionMatrix, projectionMatrix);
	//reflectProjectWorld = mul(worldMatrix, reflectionMatrix);

	//// Calculate the input position against the reflectProjectWorld matrix.
	//output.reflectionPosition = mul(float4(position, 1.f), reflectProjectWorld);

	//// Create the view projection world matrix for refraction.
	//viewProjectWorld = mul(viewMatrix, projectionMatrix);
	//viewProjectWorld = mul(worldMatrix, viewProjectWorld);

	//// Calculate the input position against the viewProjectWorld matrix.
	//output.refractionPosition = mul(float4(position, 1.f), viewProjectWorld);

	//output.normal = mul(normal, (float3x3)worldMatrix);


	return output;
}