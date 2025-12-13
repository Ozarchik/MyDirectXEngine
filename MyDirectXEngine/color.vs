cbuffer Matrix {
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexInput {
	float4 position: POSITION;
	float4 color: COLOR;
};

struct PixelInput {
	float4 position: SV_POSITION;
	float4 color: COLOR;
};


PixelInput ColorVertexShader(VectexInput input)
{
	PixelInput output;

	output.position.w = 1.0f;
	output.position = mul(output.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	output.color = input.color;

	return output;
}