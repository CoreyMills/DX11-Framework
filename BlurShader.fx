//--------------------------------------------------------------------------------------
// File: BlurShader.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D screenTex : register(t0);
SamplerState samLinear : register(s0);

//why does moving the pos of the vars change the output in scene
cbuffer ConstantBuffer : register( b0 )
{
	float texelOffset;
	float3 pad;
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// Vertex Shader
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float texelWidth, texelHeight;

	input.pos.w = 1.0f;

	output.pos = input.pos;
	output.tex = input.tex;

    return output;
}

// Pixel Shader
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = screenTex.Sample(samLinear, input.tex);

	//adjacent
	color += screenTex.Sample(samLinear, (input.tex, input.tex - texelOffset ));
	color += screenTex.Sample(samLinear, (input.tex + texelOffset, input.tex));
	color += screenTex.Sample(samLinear, (input.tex, input.tex + texelOffset));
	color += screenTex.Sample(samLinear, (input.tex - texelOffset, input.tex));

	///corners
	color += screenTex.Sample(samLinear, (input.tex + texelOffset, input.tex - texelOffset));
	color += screenTex.Sample(samLinear, (input.tex + texelOffset, input.tex + texelOffset));
	color += screenTex.Sample(samLinear, (input.tex - texelOffset, input.tex - texelOffset));
	color += screenTex.Sample(samLinear, (input.tex + texelOffset, input.tex - texelOffset));

	color.rgb = color.rgb / 9.0f;
	color.a = 1.0f;

	return color;
}