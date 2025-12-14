cbuffer Matrix {
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexInput {
	float3 position: POSITION;
	float3 color: COLOR;
};

struct PixelInput {
	float4 position: SV_POSITION;
	float4 color: COLOR;
};


PixelInput ColorVertexShader(VertexInput input)
{
	PixelInput output;

	output.position = float4(input.position, 1.0f);
	output.position = mul(output.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	output.color = float4(input.color, 1.0f);

	return output;
}