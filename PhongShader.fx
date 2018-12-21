//--------------------------------------------------------------------------------------
// File: PhongShader.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;

	float4 AmbientMaterial;
	float4 AmbientLight;

	float4 DiffuseMtrl;
	float4 DiffuseLight;

	float4 SpecularMtrl;
	float4 SpecularLight;
	float SpecularPower;
	float3 EyePosW;

	float3 LightVecW;
	float GTime;
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 pos : POSITION;
	float3 normalL : NORMAL;
	float2 tex : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 norm : NORMAL;
	float3 posW :POSITION;
	float2 tex : TEXCOORD0;
};

// Vertex Shader
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

    output.pos = mul(input.pos, World);
	output.posW = output.pos.xyz;
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);

	//output.Pos.xy += 0.5f * sin(output.Pos.x) * sin(3.0f * GTime);
	//output.Pos.z *= 0.6f + 0.4f * sin(2.0f * GTime);

	// Convert from local space to world space 
	// W component of vector is 0 as vectors cannot be translated
	float3 normalW = mul(float4(input.normalL, 0.0f), World).xyz;
	output.norm = normalize(normalW);

	output.tex = input.tex;

    return output;
}

// Pixel Shader
float4 PS(VS_OUTPUT input ) : SV_Target
{
	float4 textureColour = txDiffuse.Sample(samLinear, input.tex);

	//normalize the input vars
	float3 normalW = normalize(input.norm);
	float3 toEye = normalize(EyePosW - input.posW);

	//Compute ambient
	float3 ambient = AmbientMaterial.rgb * AmbientLight.rgb;

	//Compute diffuse
	float3 lightVecNorm = normalize(-LightVecW);
	float diffuseAmount = max(dot(lightVecNorm, normalW), 0.0f);
	float3 diffuse = (diffuseAmount * (DiffuseMtrl * DiffuseLight).rgb);

	//Compute specular
	float3 specular; 
	if (dot(lightVecNorm, normalW) >= 0.0f)
	{
		float3 specReflect = reflect(-lightVecNorm, normalW);
		float specularAmount = pow(max(dot(specReflect, toEye), 0.0f), SpecularPower);
		specular = specularAmount * (SpecularMtrl * SpecularLight).rgb;
	}
	else
	{
		specular = float3 (0.0f, 0.0f, 0.0f);
	}

	float4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
	color.rgb = ((textureColour * (ambient + diffuse)) + specular);
	color.a = textureColour.a;

	clip(color.a - 0.2f);

    return color;
}