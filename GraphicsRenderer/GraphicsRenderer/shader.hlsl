
cbuffer ConstantBuffer
{
	matrix worldViewProj;
	float4 ambientColour;
};

Texture2D Texture;
SamplerState ss;

struct VertexIn
{
	float4 Position : POSITION;
	float2 TexCoord : TEXCOORD;
};

struct VertexOut
{
	float4 Position  : SV_POSITION;
	float4 Colour	 : COLOR;
	float2 TexCoord	 : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// Transform to homogeneous clip space.
	vout.Position = mul(worldViewProj, float4(vin.Position));

	// We will just use the ambient colour to illuminate the cube
	vout.Colour = ambientColour;
	vout.TexCoord = vin.TexCoord;

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return pin.Colour * Texture.Sample(ss, pin.TexCoord);
}


